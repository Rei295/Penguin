//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//
//		エフェクトの処理
//										2021.1.11					Effect.cpp
//=============================================================================

#include "Effect.h"


// ========================================================================================
//
//	エフェクト　メインプロシージャクラス
//
// ========================================================================================
//------------------------------------------------------------------------
//
//	エフェクト　メインプロシージャクラスのコンストラクタ	
//
//------------------------------------------------------------------------
CEffectProc::CEffectProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pParticleProc = new CParticleProc(pGMain);     // パーティクルプロシージャの生成
	m_pProcArray.push_back(m_pParticleProc);   // プロシージャをプロシージャ配列に登録する

	m_pBillProc     = new CBillboardProc(pGMain);     // ビルボードプロシージャの生成
	m_pProcArray.push_back(m_pBillProc);        // プロシージャをプロシージャ配列に登録する

	m_pBillFireProc     = new CBillFireProc(pGMain);     // ビルボード炎プロシージャの生成
	m_pProcArray.push_back(m_pBillFireProc);        // プロシージャをプロシージャ配列に登録する

};


// =====================================================================================================
// 
// 各プロシージャ毎の処理
// 
// =====================================================================================================
//------------------------------------------------------------------------
//
//	パーティクルプロシージャクラスのコンストラクタ	
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CParticleProc::CParticleProc(CGameMain* pGMain) : CBaseProc( pGMain)
{
	const int ParticleNumMax = 100; // 一つのパーティクルオブジェクト中のパーティクルの数

	// パーティクルテクスチャ
	TCHAR szFName[] = _T("Data/Image/particle3.png");

	//テクスチャー読み込み
	m_pTexture = NULL;
	if (FAILED(m_pGMain->m_pD3D->CreateShaderResourceViewFromFile(szFName, &m_pTexture, 3)))
	{
		MessageBox(0, _T("パーティクル　テクスチャーを読み込めません"),_T(""), MB_OK);
		return;
	}
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		// 一つのパーティクルオブジェクト中のパーティクルの数を設定する
		m_pObjArray.push_back(new CParticleObj( this, ParticleNumMax));  // m_pObjArrayにオブジェクトを生成する
		((CParticleObj*)(m_pObjArray.back()))->SetTexture( m_pTexture ); // パーティクルテクスチャのセット       // -- 2022.2.16
	}

};
//------------------------------------------------------------------------
//
//	パーティクルプロシージャクラスのデストラクタ	
//
//------------------------------------------------------------------------
CParticleProc::~CParticleProc()
{
	SAFE_RELEASE(m_pTexture);
};

//------------------------------------------------------------------------
//
//	パーティクルプロシージャクラスの開始	
//
//	・パーティクルオブジェクト配列の中から、空いているパーティクルを探して開始する
//
//  VECTOR3 vEmitPos          パーティクルの発生位置
//  VECTOR3 vNormal           パーティクルの発生面の法線（省略可）
//
//	戻り値 BOOL
//		TRUE	= 正常
//		FALSE	= 異常
//
//------------------------------------------------------------------------
BOOL CParticleProc::Start(VECTOR3 vEmitPos, VECTOR3 vNormal)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vEmitPos, vNormal);
			return TRUE;
		}
	}
	return FALSE;
};

// ========================================================================================
//
//	パーティクルオブジェクトクラス
//（ポイントスプライトでパーティクルを実現）
//
// ========================================================================================
//------------------------------------------------------------------------
//
//	パーティクルオブジェクトのコンストラクタ	
//
//  int MaxParticle          パーティクルの数
//
//------------------------------------------------------------------------
CParticleObj::CParticleObj(CBaseProc* pProc, int MaxParticle) : CBaseObj( pProc)
{
	m_pD3D = m_pGMain->m_pD3D;
	m_pShader = m_pGMain->m_pShader;
	m_fAlpha = 0.0f;
	m_fSize = 0.0f;
	m_MaxParticle = 0;
	m_vEmitPos = VECTOR3(0,0,0);
	m_pPartArray = NULL;
	m_Frame = 0;
	m_FrameEnd = 0;
	m_fSpeed = 0.0f;  // パーティクルの移動スピード
	m_iBarthFrame = 0;     // パーティクルの移動開始までの最大待ち時間（ランダム）

	m_pVertexBuffer = NULL;
	m_pTexture = NULL; //テクスチャー

	//	初期化
	Init(MaxParticle);

	// バーテックスバッファを作成
	SetSrc();
}
//------------------------------------------------------------------------
//
//	パーティクルオブジェクトのデストラクタ	
//
//------------------------------------------------------------------------
CParticleObj::~CParticleObj()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_DELETE_ARRAY(m_pPartArray);
}

//------------------------------------------------------------------------
//	パーティクルオブジェクトの初期化関数
//
//  各種設定値の初期化
//
//  int MaxParticle        パーティクルの数
//
//  戻り値　　　なし
//------------------------------------------------------------------------
void CParticleObj::Init( int MaxParticle )
{
	m_bActive = FALSE;
	m_fAlpha = 1.0f;
	m_fSize = 0.1f;    // パーティクルのサイズ // -- 2018.8.23 
	m_MaxParticle = MaxParticle;   // 上記パーティクルプロシージャで指定したパーティクル数
	m_pPartArray = new PART[m_MaxParticle];    // パーティクル配列の生成
	m_FrameEnd = 60;	// パーティクルを表示している時間
	m_fSpeed = 0.015f;  // パーティクルの移動スピード
	m_iBarthFrame = 20;     // パーティクルの移動開始までの最大待ち時間（ランダム）
}

//------------------------------------------------------------------------
//
//	パーティクルオブジェクトのバーティクスバッファ作成とテクスチャの読み込み	
//	（ジオメトリ関連を初期化．ここでは”点”用）	
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CParticleObj::SetSrc()
{
	//バーテックス
	PARTICLE_VERTEX vertices[] =
	{
		VECTOR3(0.0f, 0.0f, 0.0f)
	};

	// ２度目以降に初期化されることの対策
	SAFE_RELEASE(m_pVertexBuffer);

	// バーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PARTICLE_VERTEX) * 1;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//------------------------------------------------------------------------
//
//	パーティクルオブジェクトの開始	
//  ・パーティクルを指定位置から発生させる。
//  ・発生方向を法線座標で指定できる。省略すると全方向へ発生する。
//
//  VECTOR3 vEmitPos          パーティクルの発生位置
//  VECTOR3 vNormal           パーティクルの発生面の法線（省略可）
//
//	戻り値 BOOL
//		TRUE	= 正常
//		FALSE	= 異常
//
//------------------------------------------------------------------------
BOOL CParticleObj::Start(VECTOR3 vEmitPos, VECTOR3 vNormal)
{
	if (m_bActive)
	{
		// すでに表示中のとき
		return FALSE;

	}else {
		// 開始処理
		m_bActive = TRUE;
		m_fAlpha  = 0.9f;
		m_vEmitPos = vEmitPos;
		m_Frame = 0;

		// 法線方向を中心としてランダム方向に発生させる
		VECTOR3 vDist, vMin, vMax;
		vDist.x = (1.0f - fabsf(vNormal.x))/2;
		vDist.y = (1.0f - fabsf(vNormal.y))/2;
		vDist.z = (1.0f - fabsf(vNormal.z))/2;

		if (vNormal.x < 0)
		{
			vMin.x = vNormal.x - vDist.x;
			vMax.x = 0.0f + vDist.x;
		}else {
			vMin.x = 0.0f - vDist.x;
			vMax.x = vNormal.x + vDist.x;
		}
		if (vNormal.y < 0)
		{
			vMin.y = vNormal.y - vDist.y;
			vMax.y = 0.0f + vDist.y;
		}else {
			vMin.y = 0.0f - vDist.y;
			vMax.y = vNormal.y + vDist.y;
		}
		if (vNormal.z < 0)
		{
			vMin.z = vNormal.z - vDist.z;
			vMax.z = 0.0f + vDist.z;
		}else {
			vMin.z = 0.0f - vDist.z;
			vMax.z = vNormal.z + vDist.z;
		}

		// パーティクルの開始
		for (int i = 0; i<m_MaxParticle; i++)
		{
			m_pPartArray[i].Pos = m_vEmitPos;
			m_pPartArray[i].Dir.x = ((float)rand() / (float)RAND_MAX) * (vMax.x - vMin.x) + vMin.x;
			m_pPartArray[i].Dir.y = ((float)rand() / (float)RAND_MAX) * (vMax.y - vMin.y) + vMin.y;
			m_pPartArray[i].Dir.z = ((float)rand() / (float)RAND_MAX) * (vMax.z - vMin.z) + vMin.z;
			m_pPartArray[i].Dir = normalize(m_pPartArray[i].Dir);
			m_pPartArray[i].Speed = (5 + ((float)rand() / (float)RAND_MAX)) * m_fSpeed;
			m_pPartArray[i].BirthFrame = (int)(((float)rand() / RAND_MAX) * m_iBarthFrame);
		}
		return TRUE;
	}
}

//------------------------------------------------------------------------
//
//	パーティクルオブジェクトの移動・更新処理	
//  ・パーティクルを更新する
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CParticleObj::Update()
{
	if (m_bActive)
	{
		m_Frame++;	// フレームを進める
		for (int i = 0; i < m_MaxParticle; i++)
		{
			if (m_Frame > m_pPartArray[i].BirthFrame) // 一個のパーティクルが表示開始時間になったとき
			{
				// 移動
				m_pPartArray[i].Pos = m_pPartArray[i].Pos + m_pPartArray[i].Dir * m_pPartArray[i].Speed;

				//m_pPartArray[i].Pos;

				// 重力
				m_pPartArray[i].Dir = m_pPartArray[i].Dir + VECTOR3(0, -0.049f, 0);
				// 地面でのバウンド
				//if (m_pPartArray[i].Pos.y < 0)
				//{
				//	m_pPartArray[i].Dir.y = -m_pPartArray[i].Dir.y;
				//	m_pPartArray[i].Speed /= 1.8f;
				//}
			}
		}

		// レンダリング
		Render();

		if (m_Frame >= m_FrameEnd) // 表示時間終了か
		{
			m_bActive = FALSE;
		}

	}
}

//------------------------------------------------------------------------
//
//	パーティクルオブジェクトを画面にレンダリング	
//  ・パーティクルを表示する
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CParticleObj::Render()
{
	// 表示状態でないとき
	if (!m_bActive)  return;

	//使用するシェーダーのセット
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pEffect3D_VS_POINT, NULL, 0);
	m_pD3D->m_pDeviceContext->GSSetShader(m_pShader->m_pEffect3D_GS_POINT, NULL, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pEffect3D_PS, NULL, 0);

	//バーテックスバッファーをセット
	UINT stride = sizeof(PARTICLE_VERTEX);
	UINT offset = 0;
	m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// 加算合成色のブレンディングを設定
	UINT mask = 0xffffffff;
	m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateAdd, NULL, mask);

	//パーティクル１粒を１枚ポイントスプライトとしてm_MaxParticle枚描画
	for (int i = 0; i<m_MaxParticle; i++)
	{
		if (m_FrameEnd < m_Frame + (m_iBarthFrame - m_pPartArray[i].BirthFrame)) continue;

		//個々のパーティクルの、視点を向くワールドトランスフォームを求める
		MATRIX4X4 mWorld = GetLookatMatrix( m_pPartArray[i].Pos, m_pGMain->m_vEyePt);

		RenderParticle(mWorld, m_pGMain->m_mView, m_pGMain->m_mProj);
	}

	// 通常のブレンディングに戻す
	m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, NULL, mask);

	//ジオメトリシェーダーのリセット
	m_pD3D->m_pDeviceContext->GSSetShader(NULL, NULL, 0);

	return;
}
//------------------------------------------------------------------------
//
//	パーティクルをレンダリングする	
//
//  MATRIX4X4    W    ワールドマトリックス
//  MATRIX4X4    V    ビューマトリックス
//  MATRIX4X4    P    プロジェクションマトリックス
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CParticleObj::RenderParticle(MATRIX4X4 W, MATRIX4X4 V, MATRIX4X4 P)
{

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_EFFECT cb;
	ZeroMemory(&cb, sizeof(cb));
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferEffect, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す // -- 2017.8.25
		cb.mW = XMMatrixTranspose(W);
		cb.mV = XMMatrixTranspose(V);
		cb.mP = XMMatrixTranspose(P);

		cb.fAlpha    = m_fAlpha;
		cb.fSize     = m_fSize;          // パーティクルのサイズ  // -- 2018.8.23
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferEffect, 0);
	}

	//このコンスタントバッファーをどのシェーダーで使うか
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferEffect);
	m_pD3D->m_pDeviceContext->GSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferEffect);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferEffect);
	//頂点インプットレイアウトをセット
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pEffect3D_VertexLayout);
	//プリミティブ・トポロジーをセット
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//テクスチャーをシェーダーに渡す
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
	m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);
	//プリミティブをレンダリング
	m_pD3D->m_pDeviceContext->Draw(1, 0);

}

// ========================================================================================
//
//	ビルボードのクラス
//
// ========================================================================================

//------------------------------------------------------------------------
//
//	ビルボードプロシージャクラスのコンストラクタ	
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CBillboardProc::CBillboardProc(CGameMain* pGMain) : CBaseProc( pGMain)
{
	// ビルボードテクスチャ
	TCHAR szFName[] = _T("Data/Image/Bom3.png");

	// 半球ビルボードスタティックメッシュの読み込み  // -- 2019.7.17
	m_pMesh = new CFbxMesh(pGMain->m_pFbxMeshCtrl, _T("Data/Item/BillSphere.mesh"));

	// テクスチャの読み込み
	m_pTexture = NULL;
	if (FAILED(Load(szFName)))
	{
		return;
	}

	for (DWORD i = 0; i < BILLBOARD_MAX; i++)
	{
		m_pObjArray.push_back(new CBillboardObj(this));  // m_pObjArrayにオブジェクトを生成する
		((CBillboardObj*)m_pObjArray.back())->m_pTexture = m_pTexture;            // プロシージャのテクスチャアドレスをセットする
		((CBillboardObj*)m_pObjArray.back())->m_dwImageHeight = m_dwImageHeight;
		((CBillboardObj*)m_pObjArray.back())->m_dwImageWidth  = m_dwImageWidth;
		((CBillboardObj*)m_pObjArray.back())->SetSrc(4, 4, 0, 0, 64, 64, 4, 4);  //  ビルボード用のバーテックスバッファ作成	
																			 //  float fDestWidth         表示幅
																			 //  float fDestHeight        表示高さ
																			 //  DWORD dwSrcX             スプライトの位置　Ｘ座標
																			 //  DWORD dwSrcY             スプライトの位置　Ｙ座標
																			 //  DWORD dwSrcWidth         スプライトの幅
																			 //  DWORD dwSrcHeight        スプライトの高さ
																			 //  DWORD dwNumX             アニメーションさせるスプライトの数　Ｘ方向
																			 //  DWORD dwNumY             アニメーションさせるスプライトの数　Ｙ方向
	}
};

//------------------------------------------------------------------------
//
//	ビルボードプロシージャクラスのデストラクタ	
//
//------------------------------------------------------------------------
CBillboardProc::~CBillboardProc()
{
	SAFE_RELEASE(m_pTexture);
};

//------------------------------------------------------------------------
//
//	スプライトイメージの読み込み	
//
//  TCHAR* szFName            ビルボードのテクスチャファイル名
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CBillboardProc::Load(TCHAR* szFName)
{

	//テクスチャー読み込み	（用意するのはリソースビューだけでいい）
	if (FAILED(m_pGMain->m_pD3D->CreateShaderResourceViewFromFile(szFName, &m_pTexture, m_dwImageWidth, m_dwImageHeight, 3)))
	{
		MessageBox(0, _T("ビルボード　テクスチャーを読み込めません"), szFName, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//------------------------------------------------------------------------
//
//	ビルボードプロシージャの開始	
//
//	・ビルボードオブジェクト配列の中から、空いているビルボードを探して開始する
//
//  VECTOR3 vPos          ビルボードの発生位置
//
//	戻り値 BOOL
//		TRUE	= 正常
//		FALSE	= 異常
//
//------------------------------------------------------------------------
BOOL CBillboardProc::Start(VECTOR3 vPos)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos);
			return TRUE;
		}
	}
	return FALSE;
};


//------------------------------------------------------------------------
//
//	ビルボードオブジェクトのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CBillboardObj::CBillboardObj(CBaseProc* pProc) : CBaseObj( pProc)
{
	m_pD3D = m_pGMain->m_pD3D;
	m_pShader = m_pGMain->m_pShader;

	m_fDestWidth  = 0.0f;
	m_fDestHeight = 0.0f;
	m_dwSrcX = 0;
	m_dwSrcY = 0;
	m_dwSrcWidth = 0;
	m_dwSrcHeight = 0;
	m_vPos = VECTOR3(0,0,0);
	m_vUVOffset = VECTOR2(0,0);
	m_vUVScale = VECTOR2(1,1);              // -- 2019.7.17
	m_dwNumX = 0;
	m_dwNumY = 0;
	m_dwFrame = 0;
	m_fAlpha = 0.0f;
	m_dwImageWidth = 0;
	m_dwImageHeight = 0;
	m_pVertexBuffer = NULL;
	m_pTexture = NULL;  //テクスチャー（用意するのはリソースビューだけでいい）

	//	初期化
	Init();
}

//------------------------------------------------------------------------
//
//	ビルボードオブジェクトのデストラクタ	
//
//------------------------------------------------------------------------
CBillboardObj::~CBillboardObj()
{
	SAFE_RELEASE(m_pVertexBuffer);
}
//------------------------------------------------------------------------
//	ビルボードオブジェクトの初期化関数
//
//  各種設定値の初期化
//
//  引数　　なし
//
//  戻り値　　　なし
//------------------------------------------------------------------------
void CBillboardObj::Init()
{
	m_bActive = FALSE;
	m_fAlpha = 0.9f;
}
//------------------------------------------------------------------------
//
//	ビルボードオブジェクトのバーティクスバッファ作成	
//	（ジオメトリ関連を初期化）	
//
//  float fDestWidth         表示幅
//  float fDestHeight        表示高さ
//  DWORD dwSrcX             スプライトの位置　Ｘ座標
//  DWORD dwSrcY             スプライトの位置　Ｙ座標
//  DWORD dwSrcWidth         スプライトの幅
//  DWORD dwSrcHeight        スプライトの高さ
//  DWORD dwNumX             アニメーションさせるスプライトの数　Ｘ方向
//  DWORD dwNumY             アニメーションさせるスプライトの数　Ｙ方向
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CBillboardObj::SetSrc(float fDestWidth, float fDestHeight, DWORD dwSrcX, DWORD dwSrcY, DWORD dwSrcWidth, DWORD dwSrcHeight, DWORD dwNumX, DWORD dwNumY)
{
	// ビルボード用の板ポリゴンを作成し、バーテックスバッファを作成する
	m_fDestWidth   = fDestWidth;
	m_fDestHeight  = fDestHeight;
	m_dwSrcX       = dwSrcX;
	m_dwSrcY       = dwSrcY;
	m_dwSrcWidth   = dwSrcWidth;
	m_dwSrcHeight  = dwSrcHeight;
	m_dwNumX       = dwNumX;
	m_dwNumY       = dwNumY;

	//バーテックスバッファー作成(裏向きの座標で作成)
	BILLBOARD_VERTEX vertices[] =
	{
		VECTOR3(-m_fDestWidth / 2,  m_fDestHeight / 2, 0), VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / m_dwImageWidth, (float)m_dwSrcY / m_dwImageHeight),                     //頂点1  左上
		VECTOR3(m_fDestWidth / 2,  m_fDestHeight / 2, 0), VECTOR2((float)m_dwSrcX / m_dwImageWidth,                (float)m_dwSrcY / m_dwImageHeight),                        //頂点2　右上    // -- 2021.1.11
		VECTOR3(-m_fDestWidth / 2, -m_fDestHeight / 2, 0), VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / m_dwImageWidth, (float)(m_dwSrcY + m_dwSrcHeight) / m_dwImageHeight),   //頂点3　左下    // -- 2021.1.11
		VECTOR3(m_fDestWidth / 2, -m_fDestHeight / 2, 0), VECTOR2((float)m_dwSrcX / m_dwImageWidth,                (float)(m_dwSrcY + m_dwSrcHeight) / m_dwImageHeight),      //頂点4　右下
	};

	// ２度目以降に初期化されることの対策
	SAFE_RELEASE(m_pVertexBuffer);

	// バーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(BILLBOARD_VERTEX) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//------------------------------------------------------------------------
//
//	ビルボードオブジェクトの開始	
//
//  VECTOR3   vPos     表示位置（ビルボードの中心点位置を指定）
//
//	戻り値 BOOL
//		TRUE      表示継続
//		FALSE     表示終了
//
//------------------------------------------------------------------------
BOOL CBillboardObj::Start(VECTOR3 vPos)
{
	if (m_bActive) return FALSE;  // すでに開始中

	m_bActive = TRUE;
	m_dwFrame = 0;
	m_vPos = vPos;

	return TRUE;
}

//------------------------------------------------------------------------
//
//	ビルボードオブジェクトの更新	
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CBillboardObj::Update()
{
	const DWORD dwSpeed = 5;   // ビルボードのアニメーションスピード
	DWORD dwIdx;

	if (!m_bActive) return;

	m_dwFrame++;
	dwIdx = m_dwFrame / dwSpeed;

	if (dwIdx >= m_dwNumX * m_dwNumY)	// パターンの最後に達したとき
	{
		m_bActive = FALSE;
		return;
	}

	// アニメーションのためのテクスチャオフセットを設定する
	m_vUVOffset.x = (float)(dwIdx % m_dwNumX * GetSrcWidth());   // Ｘ方向の変位
	m_vUVOffset.y = (float)(dwIdx / m_dwNumX * GetSrcHeight());  // Ｙ方向の変位

	//Render();// レンダリング
	RenderMesh();// レンダリング(半球ビルボードメッシュ)

}
//------------------------------------------------------------------------
//
//	ビルボードオブジェクトを画面にレンダリング	
//
//	戻り値 BOOL
//		TRUE      表示継続
//		FALSE     表示終了
//
//------------------------------------------------------------------------
BOOL CBillboardObj::Render()
{

	if (!m_bActive) return FALSE;

	//ビルボードの、視点を向くワールドトランスフォームを求める
	MATRIX4X4 mWorld =	GetLookatMatrix( m_vPos, m_pGMain->m_vEyePt);

	//使用するシェーダーのセット
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pEffect3D_VS_BILL, NULL, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pEffect3D_PS, NULL, 0);

	//バーテックスバッファーをセット
	UINT stride = sizeof(BILLBOARD_VERTEX);
	UINT offset = 0;
	m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// 加算合成色のブレンディングを設定
	UINT mask = 0xffffffff;
	m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateAdd, NULL, mask);

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_EFFECT cb;
	ZeroMemory(&cb, sizeof(cb));

	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferEffect, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列、テクスチャーオフセットを渡す
		cb.mWVP = XMMatrixTranspose(mWorld * m_pGMain->m_mView * m_pGMain->m_mProj);

		cb.vUVOffset.x = m_vUVOffset.x / m_dwImageWidth;		// テクスチャアニメーションのオフセット
		cb.vUVOffset.y = m_vUVOffset.y / m_dwImageHeight;		// テクスチャアニメーションのオフセット
		cb.vUVScale.x = 1;
		cb.vUVScale.y = 1;
		cb.fAlpha      = m_fAlpha;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferEffect, 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferEffect);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferEffect);
	//頂点インプットレイアウトをセット
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pEffect3D_VertexLayout_BILL);
	//プリミティブ・トポロジーをセット
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャーをシェーダーに渡す
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
	m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);
	//プリミティブをレンダリング
	m_pD3D->m_pDeviceContext->Draw(4, 0);

	// 通常のブレンディングに戻す
	m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, NULL, mask);

	return TRUE;
}

//------------------------------------------------------------------------  // -- 2019.7.17
//
//	ビルボードメッシュを画面にレンダリングする処理
//
// 引数
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
void CBillboardObj::RenderMesh()
{

	//使用するシェーダーの登録	
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pEffect3D_VS_BILLMESH, NULL, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pEffect3D_PS, NULL, 0);

	//ビルボードの、視点を向くワールドトランスフォームを求める
	MATRIX4X4 mWorld = GetLookatMatrix(m_vPos, m_pGMain->m_vEyePt);
	// メッシュの大きさに拡大する設定する（元々のメッシュは１ｍ四方の半球）
	MATRIX4X4 mScale = XMMatrixScaling(GetDestWidth(), GetDestHeight(), (GetDestWidth() + GetDestHeight()) / 2);
	mWorld = mScale * mWorld;

	// 加算合成色のブレンディングを設定
	UINT mask = 0xffffffff;
	m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateAdd, NULL, mask);

	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_EFFECT cb;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferEffect, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列、テクスチャーオフセットを渡す
		cb.mWVP = XMMatrixTranspose(mWorld * m_pGMain->m_mView * m_pGMain->m_mProj);

		cb.vUVOffset.x = m_vUVOffset.x / m_dwImageWidth;			// テクスチャアニメーションのオフセット
		cb.vUVOffset.y = m_vUVOffset.y / m_dwImageHeight;			// テクスチャアニメーションのオフセット
		cb.vUVScale.x = (float)GetSrcWidth() / m_dwImageWidth;		// テクスチャアニメーションの1/横回数
		cb.vUVScale.y = (float)GetSrcHeight() / m_dwImageHeight;	// テクスチャアニメーションの1/縦回数

		cb.fAlpha = m_fAlpha;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferEffect, 0);
	}

	//このコンスタントバッファーをどのシェーダーで使うか
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferEffect);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferEffect);

	//頂点インプットレイアウトをセット
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pFbxStaticMesh_VertexLayout);

	//プリミティブ・トポロジーをセット
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//テクスチャーサンプラーをシェーダーに渡す
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// メッシュの描画順を決定するm_dwRenderIdxArrayの設定
	GetMesh()->SetRenderIdxArray(mWorld, m_pGMain->m_vEyePt);

	// メッシュの数だけテクスチャー、バーテックスバッファ、インデックスバッファをセットして、レンダリングする
	for (DWORD mi = 0; mi < GetMesh()->m_dwMeshNum; mi++)
	{
		DWORD i = GetMesh()->m_dwRenderIdxArray[mi];

		//テクスチャーをシェーダーに渡す
		//m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &GetMesh()->m_pMeshArray[i].m_pTexture);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

		// バーテックスバッファーをセット
		UINT stride = sizeof(StaticVertexNormal);
		UINT offset = 0;
		m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &GetMesh()->m_pMeshArray[i].m_pVertexBuffer, &stride, &offset);

		//インデックスバッファーをセット
		m_pD3D->m_pDeviceContext->IASetIndexBuffer(GetMesh()->m_pMeshArray[i].m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//プリミティブをレンダリング
		// （インデックスの数を指定してレンダリング）
		m_pD3D->m_pDeviceContext->DrawIndexed(GetMesh()->m_pMeshArray[i].m_dwIndicesNum, 0, 0);
	}

	// 通常のブレンディングに戻す
	m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, NULL, mask);

}


// ========================================================================================
//
//	ビルボード　炎のクラス
//
// ========================================================================================

//------------------------------------------------------------------------
//
//	ビルボード　炎プロシージャクラスのコンストラクタ	
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
//------------------------------------------------------------------------
CBillFireProc::CBillFireProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// ビルボード　炎テクスチャ
	TCHAR szFName[] = _T("Data/Image/sparklen3.png");

	// テクスチャの読み込み
	m_pTexture = NULL;
	if (FAILED(Load(szFName)))
	{
		return;
	}

	for (DWORD i = 0; i < BILLFIRE_MAX; i++)
	{
		m_pObjArray.push_back(new CBillFireObj(this));  // m_pObjArrayにオブジェクトを生成する
		((CBillFireObj*)m_pObjArray.back())->m_pTexture = m_pTexture;            // プロシージャのテクスチャアドレスをセットする       // -- 2022.2.16
		((CBillFireObj*)m_pObjArray.back())->m_dwImageHeight = m_dwImageHeight;
		((CBillFireObj*)m_pObjArray.back())->m_dwImageWidth = m_dwImageWidth;
		((CBillFireObj*)m_pObjArray.back())->SetSrc(0.7f, 0.7f, 0, 0, 85, 85, 3, 1);  // ビルボード　炎用のバーティクスバッファ作成
																				  //  float fDestWidth         表示幅
																				  //  float fDestHeight        表示高さ
																				  //  DWORD dwSrcX             スプライトの位置　Ｘ座標
																				  //  DWORD dwSrcY             スプライトの位置　Ｙ座標
																				  //  DWORD dwSrcWidth         スプライトの幅
																				  //  DWORD dwSrcHeight        スプライトの高さ
																				  //  DWORD dwNumX             アニメーションさせるスプライトの数　Ｘ方向
																				  //  DWORD dwNumY             アニメーションさせるスプライトの数　Ｙ方向
	}
};

//------------------------------------------------------------------------
//
//	ビルボード　炎プロシージャクラスのデストラクタ	
//
//------------------------------------------------------------------------
CBillFireProc::~CBillFireProc()
{
	SAFE_RELEASE(m_pTexture);
};

//------------------------------------------------------------------------
//
//	スプライトイメージの読み込み	
//
//  TCHAR* szFName            ビルボード　炎のテクスチャファイル名
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CBillFireProc::Load(TCHAR* szFName)
{

	//テクスチャー読み込み	（用意するのはリソースビューだけでいい）
	if (FAILED(m_pGMain->m_pD3D->CreateShaderResourceViewFromFile(szFName, &m_pTexture, m_dwImageWidth, m_dwImageHeight, 3)))
	{
		MessageBox(0, _T("ビルボード　炎　テクスチャーを読み込めません"), szFName, MB_OK);
		return E_FAIL;
	}


	return S_OK;
}

//------------------------------------------------------------------------
//
//	ビルボード　炎プロシージャの開始	
//  (炎の制御を自分自身で行うとき)
//
//	・ビルボード　炎オブジェクト配列の中から、空いているビルボード　炎を探して開始する
//
//  VECTOR3 vPos          ビルボード　炎の発生位置
//
//	戻り値 BOOL
//		TRUE	= 正常
//		FALSE	= 異常
//
//------------------------------------------------------------------------
BOOL CBillFireProc::Start(VECTOR3 vPos)
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vPos);
			return TRUE;
		}
	}
	return FALSE;
};

//------------------------------------------------------------------------
//
//	ビルボード　炎プロシージャの開始	
//  (炎の制御をＷｅａｐｏｎ等の親が行うとき)
//
//
//	・ビルボード　炎オブジェクト配列の中から、空いているビルボード　炎を探して開始する
//
//	戻り値 CBillFireObj*
//		発生できたとき　　ビルボード　炎オブジェクトアドレス
//		発生できないとき　NULL
//
//------------------------------------------------------------------------
CBillFireObj*  CBillFireProc::Start()
{
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->SetActive(TRUE);
			((CBillFireObj*)(m_pObjArray[i]))->m_bAuto = FALSE;  // 炎の制御をＷｅａｐｏｎ等の親が行う
			((CBillFireObj*)(m_pObjArray[i]))->m_dwFrame = 0;
			return ((CBillFireObj*)(m_pObjArray[i]));
		}
	}
	return NULL;
};

//------------------------------------------------------------------------
//
//	ビルボード　炎オブジェクトのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CBillFireObj::CBillFireObj(CBaseProc* pProc) : CBaseObj( pProc)
{
	m_pD3D = m_pGMain->m_pD3D;
	m_pShader = m_pGMain->m_pShader;
	m_fDestWidth  = 0.0f;
	m_fDestHeight = 0.0f;
	m_dwSrcX = 0;
	m_dwSrcY = 0;
	m_dwSrcWidth = 0;
	m_dwSrcHeight = 0;
	m_vPos = VECTOR3(0, 0, 0);
	m_vUVOffset = VECTOR2(0, 0);
	m_dwNumX = 0;
	m_dwNumY = 0;
	m_dwFrame = 0;
	m_fAlpha = 0.0f;
	m_dwImageWidth = 0;
	m_dwImageHeight = 0;
	m_pVertexBuffer = NULL;
	m_pTexture = NULL;  //テクスチャー（用意するのはリソースビューだけでいい）

	//	初期化
	Init();
}

//------------------------------------------------------------------------
//
//	ビルボード　炎オブジェクトのデストラクタ	
//
//------------------------------------------------------------------------
CBillFireObj::~CBillFireObj()
{
	SAFE_RELEASE(m_pVertexBuffer);
}
//------------------------------------------------------------------------
//	ビルボード　炎オブジェクトの初期化関数
//
//  各種設定値の初期化
//
//  引数　　なし
//
//  戻り値　　　なし
//------------------------------------------------------------------------
void CBillFireObj::Init()
{
	m_bActive = FALSE;
	m_bAuto  = FALSE;
	m_fAlpha = 0.9f;
}
//------------------------------------------------------------------------
//
//	ビルボード　炎オブジェクトのバーティクスバッファ作成	
//	（ジオメトリ関連を初期化）	
//
//  float fDestWidth         表示幅
//  float fDestHeight        表示高さ
//  DWORD dwSrcX             スプライトの位置　Ｘ座標
//  DWORD dwSrcY             スプライトの位置　Ｙ座標
//  DWORD dwSrcWidth         スプライトの幅
//  DWORD dwSrcHeight        スプライトの高さ
//  DWORD dwNumX             アニメーションさせるスプライトの数　Ｘ方向
//  DWORD dwNumY             アニメーションさせるスプライトの数　Ｙ方向
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CBillFireObj::SetSrc(float fDestWidth, float fDestHeight, DWORD dwSrcX, DWORD dwSrcY, DWORD dwSrcWidth, DWORD dwSrcHeight, DWORD dwNumX, DWORD dwNumY)
{
	// ビルボード　炎用の板ポリゴンを作成し、バーテックスバッファを作成する
	m_fDestWidth = fDestWidth;
	m_fDestHeight = fDestHeight;
	m_dwSrcX = dwSrcX;
	m_dwSrcY = dwSrcY;
	m_dwSrcWidth = dwSrcWidth;
	m_dwSrcHeight = dwSrcHeight;
	m_dwNumX = dwNumX;
	m_dwNumY = dwNumY;

	//バーテックスバッファー作成(裏向きの座標で作成)
	BILLBOARD_VERTEX vertices[] =
	{
		VECTOR3(-m_fDestWidth / 2,  m_fDestHeight / 2, 0), VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / m_dwImageWidth, (float)m_dwSrcY / m_dwImageHeight),                     //頂点1  左上
		VECTOR3(m_fDestWidth / 2,  m_fDestHeight / 2, 0), VECTOR2((float)m_dwSrcX / m_dwImageWidth,                (float)m_dwSrcY / m_dwImageHeight),                        //頂点2　右上    // -- 2021.1.11
		VECTOR3(-m_fDestWidth / 2, -m_fDestHeight / 2, 0), VECTOR2((float)(m_dwSrcX + m_dwSrcWidth) / m_dwImageWidth, (float)(m_dwSrcY + m_dwSrcHeight) / m_dwImageHeight),   //頂点3　左下    // -- 2021.1.11
		VECTOR3(m_fDestWidth / 2, -m_fDestHeight / 2, 0), VECTOR2((float)m_dwSrcX / m_dwImageWidth,                (float)(m_dwSrcY + m_dwSrcHeight) / m_dwImageHeight),      //頂点4　右下
	};

	// ２度目以降に初期化されることの対策
	SAFE_RELEASE(m_pVertexBuffer);

	// バーテックスバッファー作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(BILLBOARD_VERTEX) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//------------------------------------------------------------------------
//
//	ビルボード　炎オブジェクトの開始	
//
//  VECTOR3   vPos     表示位置（ビルボード　炎の中心点位置を指定）
//
//	戻り値 BOOL
//		TRUE      表示継続
//		FALSE     表示終了
//
//------------------------------------------------------------------------
BOOL CBillFireObj::Start(VECTOR3 vPos)
{
	if (m_bActive) return FALSE;  // すでに開始中

	m_bActive = TRUE;
	m_bAuto   = TRUE;  // 自動モード
	m_dwFrame = 0;
	m_vPos    = vPos;

	return TRUE;
}

//------------------------------------------------------------------------
//
//	ビルボード　炎オブジェクトの更新	
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CBillFireObj::Update()
{
	const DWORD dwSpeed = 5;   // アニメーションスピード
	DWORD dwIdx;

	if (!m_bActive) return;

	m_dwFrame++;
	dwIdx = m_dwFrame / dwSpeed;

	if (dwIdx >= m_dwNumX * m_dwNumY)	// パターンの最後に達したとき
	{
		if (m_bAuto){  // 自動モードのときは終了
			m_bActive = FALSE;
			return;
		}
		else {
			dwIdx = 0; // アニメーションを繰り返す
		}
	}

	// アニメーションのためのテクスチャオフセットを設定する
	m_vUVOffset.x = (float)(dwIdx % m_dwNumX * GetSrcWidth());   // Ｘ方向の変位
	m_vUVOffset.y = (float)(dwIdx / m_dwNumX * GetSrcHeight());  // Ｙ方向の変位

	Render();// レンダリング

}
//------------------------------------------------------------------------
//
//	ビルボード　炎オブジェクトを画面にレンダリング	
//
//	戻り値 BOOL
//		TRUE      表示継続
//		FALSE     表示終了
//
//------------------------------------------------------------------------
BOOL CBillFireObj::Render()
{

	if (!m_bActive) return FALSE;

	//ビルボード　炎の、視点を向くワールドトランスフォームを求める
	MATRIX4X4 mWorld = GetLookatMatrix(m_vPos, m_pGMain->m_vEyePt);

	//使用するシェーダーのセット
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pEffect3D_VS_BILL, NULL, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pEffect3D_PS, NULL, 0);

	//バーテックスバッファーをセット
	UINT stride = sizeof(BILLBOARD_VERTEX);
	UINT offset = 0;
	m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// 加算合成色のブレンディングを設定
	UINT mask = 0xffffffff;
	m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateAdd, NULL, mask);

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_EFFECT cb;
	ZeroMemory(&cb, sizeof(cb));

	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferEffect, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列、テクスチャーオフセットを渡す
		cb.mWVP = XMMatrixTranspose(mWorld * m_pGMain->m_mView * m_pGMain->m_mProj);

		cb.vUVOffset.x = m_vUVOffset.x / m_dwImageWidth;		// テクスチャアニメーションのオフセット
		cb.vUVOffset.y = m_vUVOffset.y / m_dwImageHeight;		// テクスチャアニメーションのオフセット
		cb.vUVScale.x = 1;
		cb.vUVScale.y = 1;
		cb.fAlpha = m_fAlpha;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferEffect, 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferEffect);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferEffect);
	//頂点インプットレイアウトをセット
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pEffect3D_VertexLayout_BILL);
	//プリミティブ・トポロジーをセット
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャーをシェーダーに渡す
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
	m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	//プリミティブをレンダリング
	m_pD3D->m_pDeviceContext->Draw(4, 0);

	// 通常のブレンディングに戻す
	m_pD3D->m_pDeviceContext->OMSetBlendState(m_pD3D->m_pBlendStateTrapen, NULL, mask);

	return TRUE;
}



