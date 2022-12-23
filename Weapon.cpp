//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//		武器の処理
//															Weapon.cpp
//=============================================================================
#include  "Enemy.h"
#include  "Playchar.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "BackFore.h"

//=============================================================================================================================
//
// 武器メインプロシージャのコンストラクタ
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
// ----------------------------------------------------------------------------------------------------------------------------
CWeaponProc::CWeaponProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pWeaponLaserProc = new CWeaponLaserProc(pGMain);		// 武器・レーザープロシージャ
	m_pProcArray.push_back(m_pWeaponLaserProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponFireBallProc = new CWeaponFireBallProc(pGMain);	// 武器・火の玉プロシージャ
	m_pProcArray.push_back(m_pWeaponFireBallProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponSwordProc = new CWeaponSwordProc(pGMain);		// 武器・剣プロシージャ
	m_pProcArray.push_back(m_pWeaponSwordProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponFlameProc = new CWeaponFlameProc(pGMain);		// 武器・火炎プロシージャ
	m_pProcArray.push_back(m_pWeaponFlameProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponBallProc = new CWeaponBallProc(pGMain);		// 武器・ボールプロシージャ
	m_pProcArray.push_back(m_pWeaponBallProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponSprayProc = new CWeaponSprayProc(pGMain);		// 武器・スプレープロシージャ
	m_pProcArray.push_back(m_pWeaponSprayProc);				// プロシージャをプロシージャ配列に登録する

	m_pWeaponHoldProc = new CWeaponHoldProc(pGMain);		// 武器・手に持つアイテムのプロシージャ
	m_pProcArray.push_back(m_pWeaponHoldProc);				// プロシージャをプロシージャ配列に登録する


}


// ================================================================================================================================
// 
// 各武器プロシージャ毎の処理
// 
// ================================================================================================================================


// ================================================================================================================================
// ---------------------------------------------------------------------------
//
// 武器・レーザープロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CWeaponLaserProc::CWeaponLaserProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// レーザーメッシュの設定
	m_pMesh = new CFbxMesh( m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Laser2.mesh"));

	for (int i = 0; i < LASER_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponLaserObj(this));	// m_pObjArrayに武器・レーザーオブジェクトを生成する
	}
}
// ---------------------------------------------------------------------------
//
// 武器・レーザープロシージャのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponLaserProc::~CWeaponLaserProc()
{
	;
}
// ---------------------------------------------------------------------------
//
// 武器・レーザープロシージャの発射
//
//  MATRIX4X4  mGun         ：銃の位置マトリックス
//  VECTOR3 vOffset      ：位置の微調整
//  MATRIX4X4  mOwnerWorld  ：発射するキャラのマトリックス
//  DWORD       dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
// ---------------------------------------------------------------------------
BOOL CWeaponLaserProc::Start(MATRIX4X4 mGun, VECTOR3 vOffset, MATRIX4X4 mOwnerWorld, DWORD dwOwner)
{
	// レーザーの発射
	MATRIX4X4 mLay, mOwnerRot, mOffset;
	
	mLay = GetPositionMatrix(mGun);               // 銃の位置(回転を取り除く)
	mOwnerRot = GetRotateMatrix(mOwnerWorld);     // 発射するキャラの回転マトリックスのみ(位置を取り除く)にする
	mLay = mOwnerRot * mLay;                      // 銃の位置から、キャラの方向で発射するマトリックスを作る
	mOffset = XMMatrixTranslation(vOffset.x, vOffset.y, vOffset.z);	// 位置の微調整
	mLay = mOffset * mLay;

	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mLay, dwOwner);	       // 武器・レーザーオブジェクトの発射
			return TRUE;
		}
	}

	return FALSE;
}

// ---------------------------------------------------------------------------
//
// 武器・レーザープロシージャの発射
//
//  MATRIX4X4  mWorld       ：レーザーの発射位置のマトリックス
//  DWORD       dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
// ---------------------------------------------------------------------------
BOOL CWeaponLaserProc::Start(MATRIX4X4 mWorld, DWORD dwOwner)
{
	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mWorld, dwOwner);	       // 武器・レーザーオブジェクトの発射
			return TRUE;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
//
// 武器・レーザーオブジェクトのコンストラクタ
//
// ---------------------------------------------------------------------------
CWeaponLaserObj::CWeaponLaserObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = 10;				// レーザーの攻撃力

	m_vTarget = VECTOR3(0, 0, 0);		// ターゲット
	m_vStart = VECTOR3(0, 0, 0);		// 発射位置
	m_vEnd = VECTOR3(0, 0, 0);			// 最終到達点

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_nAtc = nAtc;

	m_dwOwner = 0;


}
// ---------------------------------------------------------------------------
//
// 武器・レーザーオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponLaserObj::~CWeaponLaserObj()
{
	;
}

// ---------------------------------------------------------------------------
//
// 武器・レーザーオブジェクトの発射
//
//     MATRIX4X4 mStartWorld     レーザーの発射位置のマトリックス
//     DWORD      dwOwner         武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponLaserObj::Start(MATRIX4X4 mStartWorld, DWORD dwOwner)
{
	const float fLength = 25.0f;		// レーザーの長さ

	if (m_bActive) return FALSE;

	m_bActive = TRUE;
	m_mWorldOld = mStartWorld;  // レーザーの発射位置のマトリックスを保存しておく

	MATRIX4X4 mScale;
	mScale = XMMatrixScaling(1.0f, 1.0f, fLength);  // レーザーの長さの拡大マトリックスを作成する
	m_mWorld = mScale * mStartWorld;                // レーザーの発射位置からレーザーの長さに拡大したマトリックスを作成する

	MATRIX4X4 mTrans;
	mTrans = XMMatrixTranslation(0.0f, 0.0f, fLength);
	mTrans = mTrans * mStartWorld;     // レーザーの到達点のマトリックスを作成する

	m_vStart = GetPositionVector(m_mWorldOld);	// レーザーの発射位置
	m_vEnd = GetPositionVector(mTrans);			// レーザーの到達位置

	m_dwOwner = dwOwner;

	return TRUE;
}

// ---------------------------------------------------------------------------
//
// 武器・レーザーオブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponLaserObj::Update()
{

	if (m_bActive)
	{
		// 敵やＰＣとのあたり判定
		if ( ( m_dwOwner != ENM && m_pGMain->m_pEnmProc->Hitcheck(this, m_vEnd, m_vStart) )
			|| ( m_dwOwner != PC && m_pGMain->m_pPcProc->Hitcheck(this, m_vEnd, m_vStart)) )
		{
			m_pGMain->m_pEffectProc->m_pParticleProc->Start(m_vHitPos, m_vHitNormal);	// パーティクルの発生
			//m_pGMain->m_pEffectProc->m_pBillProc->Start(m_vHitPos);	// 爆発ビルボードの発生
			m_pGMain->m_pSeDead->Play();// 爆発効果音

		}
		else {
			// マップとのあたり判定
			VECTOR3 vHit, vNormal;
			if (m_pGMain->m_pMapProc->Hitcheck(m_vEnd, m_vStart, &vHit, &vNormal))
			{
				m_pGMain->m_pEffectProc->m_pParticleProc->Start(vHit, vNormal);	// パーティクルの発生
				//m_pGMain->m_pEffectProc->m_pBillProc->Start(vHit);	// 爆発ビルボードの発生
				m_pGMain->m_pSeDead->Play();// 爆発効果音
			}
		}

		// レンダリング
		Render();

	}

	m_bActive = FALSE;	// １回ですぐ消す

}

// ---------------------------------------------------------------------------
//
// 武器・レーザーオブジェクトのレンダリング
//
//   陰影をつけないレンダリング　光の方向が(0,0,0)
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponLaserObj::Render()
{

	// 加算合成色のブレンディングを設定
	UINT mask = 0xffffffff;
	m_pGMain->m_pD3D->m_pDeviceContext->OMSetBlendState(m_pGMain->m_pD3D->m_pBlendStateAdd, NULL, mask);
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);        // -- 2022.2.16

	// 通常のブレンディングを設定
	m_pGMain->m_pD3D->m_pDeviceContext->OMSetBlendState(m_pGMain->m_pD3D->m_pBlendStateTrapen, NULL, mask);

}


// ================================================================================================================================
//============================================================================
//
// 武器・火の玉プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CWeaponFireBallProc::CWeaponFireBallProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// 火の玉メッシュの設定
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/FireBall.mesh"));

	for (int i = 0; i < FIREBALL_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponFireBallObj(this));	// m_pObjArrayに武器・火の玉オブジェクトを生成する
	}
}
//============================================================================
//
// 武器・火の玉プロシージャのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponFireBallProc::~CWeaponFireBallProc()
{
	;
}
// ---------------------------------------------------------------------------
//
// 武器・火の玉プロシージャの発射
//
//  VECTOR3 vStart       ：発射位置
//  VECTOR3 vTarget      ：目標位置
//  DWORD       dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
// ---------------------------------------------------------------------------
BOOL CWeaponFireBallProc::Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner)
{
	const int nNum = 5;      // 火の玉の数

	int n = nNum;
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vStart, vTarget, dwOwner);	// 武器・火の玉オブジェクト
			n--;
			if (n > 0) continue;
			return TRUE;
		}
	}
	return FALSE;
}

//============================================================================
//
// 武器・火の玉オブジェクトのコンストラクタ
//
// ---------------------------------------------------------------------------
CWeaponFireBallObj::CWeaponFireBallObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = 500;  // 火の玉の攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;


	m_nAtc = nAtc;

	m_dwOwner = 0;

}
// ---------------------------------------------------------------------------
//
// 武器・火の玉オブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponFireBallObj::~CWeaponFireBallObj()
{
	;
}

// ---------------------------------------------------------------------------
//
// 武器・火の玉オブジェクトの発射
//
//     VECTOR3 vStart       ：火の玉の発射位置
//     VECTOR3 vTarget      ：火の玉のターゲット
//     DWORD       dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponFireBallObj::Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner)
{
	const float fSpeed = 0.5f;		// 火の玉の速さ
	const int nLifeTime = 80;		// 火の玉の持続時間

	VECTOR3 vError, vError2;

	if (m_bActive) return FALSE;

	m_bActive = TRUE;

	// ターゲット位置変位を乱数で決定
	vError.x = Random(-10, 10) / 10.0f;
	vError.y = Random(-5, 5)   / 10.0f;
	vError.z = Random(-10, 10) / 10.0f;

	// 発射位置変位を乱数で決定
	vError2.x = Random(-5, 5) / 10.0f;
	vError2.y = Random(-5, 5) / 10.0f;
	vError2.z = Random(-5, 5) / 10.0f;

	vStart += vError2;
	vTarget += vError;

	m_mWorld = GetLookatMatrix(vStart, vTarget);
	m_mWorldOld = m_mWorld;

	m_vPosUp = VECTOR3(0, 0, fSpeed);

	m_nCnt1 = nLifeTime;

	m_dwOwner = dwOwner;

	// 火の玉を囲む炎のスタート
	for (DWORD i = 0; i < FIREBALL_FIRE_MAX; i++)
	{
		m_pBillFire[i] = m_pGMain->m_pEffectProc->m_pBillFireProc->Start();
		if (m_pBillFire[i])
		{
			// 発射位置変位と進行方向を乱数で決定
			vError.x = Random(-3, 3) / 10.0f;
			vError.y = Random(-3, 3) / 10.0f;
			vError.z = Random(-3, 3) / 10.0f;
			m_pBillFire[i]->m_vPos = vStart + vError;
			m_pBillFire[i]->SetPosUp(vError);
		}
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//
// 武器・火の玉オブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponFireBallObj::Update()
{

	if (m_bActive)
	{
		m_mWorldOld = m_mWorld;

		MATRIX4X4 mTran;
		mTran = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y, m_vPosUp.z);
		m_mWorld = mTran * m_mWorld;

		// 敵やＰＣとのあたり判定
		if ((m_dwOwner != ENM && m_pGMain->m_pEnmProc->Hitcheck(this, GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld))) ||
			(m_dwOwner != PC  && m_pGMain->m_pPcProc->Hitcheck(this, GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld))))
		{
			//m_pGMain->m_pEffectProc->m_pParticleProc->Start(m_vHitPos, m_vHitNormal);	// パーティクルの発生
			m_pGMain->m_pEffectProc->m_pBillProc->Start(m_vHitPos);	// 爆発ビルボードの発生
			m_pGMain->m_pSeDead->Play();// 爆発効果音
			m_bActive = FALSE;
			for (DWORD i = 0; i < FIREBALL_FIRE_MAX; i++)
			{
				if (m_pBillFire[i]) m_pBillFire[i]->SetActive(FALSE);
			}
		}
		else {
			// マップとのあたり判定
			VECTOR3 vHit, vNormal;
			if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
			{
				//m_pGMain->m_pEffectProc->m_pParticleProc->Start(vHit, vNormal);	// パーティクルの発生
				m_pGMain->m_pEffectProc->m_pBillProc->Start(vHit);	// 爆発ビルボードの発生
				m_pGMain->m_pSeDead->Play();// 爆発効果音
				m_bActive = FALSE;
				for (DWORD i = 0; i < FIREBALL_FIRE_MAX; i++)
				{
					if (m_pBillFire[i]) m_pBillFire[i]->SetActive(FALSE);
				}
			}
		}

		m_nCnt1--;
		if (m_nCnt1 <= 0)
		{
			m_bActive = FALSE;
			for (DWORD i = 0; i < FIREBALL_FIRE_MAX; i++)
			{
				if (m_pBillFire[i]) m_pBillFire[i]->SetActive(FALSE);
			}
		}

		// レンダリング
		Render();

		// 火の玉を囲む炎の更新
		for (DWORD i = 0; i < FIREBALL_FIRE_MAX; i++)
		{
			if (m_pBillFire[i]) m_pBillFire[i]->m_vPos = GetPositionVector(m_mWorld) + m_pBillFire[i]->GetPosUp();
		}
	}

}

// ---------------------------------------------------------------------------
//
// 武器・火の玉オブジェクトのレンダリング
//
//   陰影をつけないレンダリング　光の方向が(0,0,0)
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponFireBallObj::Render()
{

	// 火の玉のレンダリング
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);        // -- 2022.2.16
}


// ===================================================================================
//============================================================================
//
// 武器・剣プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CWeaponSwordProc::CWeaponSwordProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pObjArray.push_back(new CWeaponSwordObj(this));	// 武器・剣オブジェクト
}

// ---------------------------------------------------------------------------
//
// 武器剣・プロシージャの発生
//
// ---------------------------------------------------------------------------

BOOL CWeaponSwordProc::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	m_pObjArray[0]->Start(mMat, vMin, vMax, dwOwner);	// 武器・剣オブジェクト

	return TRUE;
}


//============================================================================
//
// 武器・剣オブジェクトのコンストラクタ
//
// ---------------------------------------------------------------------------
CWeaponSwordObj::CWeaponSwordObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = 300;    // 剣の攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	// BBoxの設定(大きさは、仮に設定)
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.1f, -0.1f, -0.1f), VECTOR3(0.1f, 0.1f, 0.1f));
	m_nAtc = nAtc;
	m_dwOwner = 0;
}
// ---------------------------------------------------------------------------
//
// 武器・剣オブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponSwordObj::~CWeaponSwordObj()
{
	;
}
// ---------------------------------------------------------------------------
//
// 武器・剣オブジェクトの発生と処理、描画
//
//     MATRIX4X4  mMat        ：剣の位置
//     VECTOR3 vMin        ：バウンディングボックスの最小値
//     VECTOR3 vMax        ：バウンディングボックスの最大値
//     DWORD       dwOwner     ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponSwordObj::Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner)
{
	m_bActive = TRUE;  // スタート時に表示開始

	// バウンディングボックスのサイズが変わっていたら作り替える
	if (m_pBBox->m_vMin.x != vMin.x || m_pBBox->m_vMin.y != vMin.y || m_pBBox->m_vMin.z != vMin.z
		||
		m_pBBox->m_vMax.x != vMax.x || m_pBBox->m_vMax.y != vMax.y || m_pBBox->m_vMax.z != vMax.z)
	{
		m_pBBox->InitBBox(vMin, vMax);   // バウンディングボックスの大きさ変更
	}
	m_pBBox->m_mWorld = mMat;   // バウンディングボックスのワールドマトリックスをセット

	m_dwOwner = dwOwner;

	// 敵やＰＣとのあたり判定
	if ((dwOwner != ENM  && m_pGMain->m_pEnmProc->Hitcheck(this, m_pBBox)) ||
		(dwOwner != PC   && m_pGMain->m_pPcProc->Hitcheck(this, m_pBBox)))
	{
		// ヒットしたとき
		m_pGMain->m_pEffectProc->m_pParticleProc->Start(m_vHitPos, m_vHitNormal);	// パーティクルの発生
		//m_pGMain->m_pEffectProc->m_pBillProc->Start(m_vHitPos);	// 爆発ビルボードの発生
		m_pGMain->m_pSeDead->Play();// 爆発効果音
	}

	// バウンディングボックスのレンダリング
	Render();

	m_bActive = FALSE;  // １回処理したら、消す

	return TRUE;
}
// ---------------------------------------------------------------------------
//
// 武器・剣オブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponSwordObj::Update()
{
	;
}

// ---------------------------------------------------------------------------
//
// 武器・剣オブジェクトのレンダリング
// (バウンディングボックスのレンダリング)
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponSwordObj::Render()
{

	//   バウンディングボックスレンダリング
	//m_pBBox->Render(m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

}

// ================================================================================================================================
//============================================================================
//
// 武器・火炎プロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CWeaponFlameProc::CWeaponFlameProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// 火炎メッシュの設定
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/FireBall2.mesh"));

	for (int i = 0; i < FLAME_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponFlameObj(this));	// m_pObjArrayに武器・火炎オブジェクトを生成する
	}
}
//============================================================================
//
// 武器・火炎プロシージャのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponFlameProc::~CWeaponFlameProc()
{
	;
}
// ---------------------------------------------------------------------------
//
// 武器・火炎プロシージャの発射
//
//  VECTOR3 vStart       ：発射位置
//  VECTOR3 vTarget      ：目標位置
//  DWORD       dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
// ---------------------------------------------------------------------------
BOOL CWeaponFlameProc::Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner)
{
	const int nNum = 5;   // 火炎の数

	int n = nNum;
	for (DWORD i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(vStart, vTarget, dwOwner);	// 武器・火炎オブジェクト
			n--;
			if (n > 0) continue;
			return TRUE;
		}
	}
	return FALSE;
}

//============================================================================
//
// 武器・火炎オブジェクトのコンストラクタ
//
// ---------------------------------------------------------------------------
CWeaponFlameObj::CWeaponFlameObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = 500;  // 攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;
	m_nAtc = nAtc;
	m_dwOwner = 0;

}
// ---------------------------------------------------------------------------
//
// 武器・火炎オブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponFlameObj::~CWeaponFlameObj()
{
	;
}

// ---------------------------------------------------------------------------
//
// 武器・火炎オブジェクトの発射
//
//     VECTOR3 vStart       ：火炎の発射位置
//     VECTOR3 vTarget      ：火炎のターゲット
//     DWORD   dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponFlameObj::Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner)
{
	const float fSpeed = 1.0f;		// 火炎の速さ
	const int nLifeTime = 80;		// 火の玉の持続時間
	VECTOR3 vError, vError2;

	if (m_bActive) return FALSE;

	m_bActive = TRUE;

	// ターゲット位置変位を乱数で決定
	vError.x = Random(-10, 10) / 10.0f;
	vError.y = Random(-5, 5) / 10.0f;
	vError.z = Random(-10, 10) / 10.0f;

	// 発射位置変位を乱数で決定
	vError2.x = Random(-5, 5) / 10.0f;
	vError2.y = Random(-5, 5) / 10.0f;
	vError2.z = Random(-5, 5) / 10.0f;

	vStart += vError2;
	vTarget += vError;

	m_mWorld = GetLookatMatrix(vStart, vTarget);
	m_mWorldOld = m_mWorld;

	m_vPosUp = VECTOR3(0, 0, fSpeed);

	m_nCnt1 = nLifeTime;

	m_dwOwner = dwOwner;

	// 火炎を囲む炎のスタート
	for (DWORD i = 0; i < FLAME_FIRE_MAX; i++)
	{
		m_pBillFire[i] = m_pGMain->m_pEffectProc->m_pBillFireProc->Start();
		if (m_pBillFire[i])
		{
			vError.x = Random(-3, 3) / 10.0f;
			vError.y = Random(-3, 3) / 10.0f;
			vError.z = Random(-3, 3) / 10.0f;
			m_pBillFire[i]->m_vPos = vStart + vError;
			m_pBillFire[i]->SetPosUp(vError);
		}
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//
// 武器・火炎オブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponFlameObj::Update()
{

	if (m_bActive)
	{
		m_mWorldOld = m_mWorld;

		MATRIX4X4 mTran;
		mTran = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y, m_vPosUp.z);
		m_mWorld = mTran * m_mWorld;

		// 敵やＰＣとのあたり判定
		if ((m_dwOwner != ENM && m_pGMain->m_pEnmProc->Hitcheck(this, GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld))) ||
			(m_dwOwner != PC  && m_pGMain->m_pPcProc->Hitcheck(this, GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld))))
		{
			m_pGMain->m_pEffectProc->m_pBillProc->Start(m_vHitPos);	// 爆発ビルボードの発生
			m_pGMain->m_pSeDead->Play();// 爆発効果音
			m_bActive = FALSE;
			for (DWORD i = 0; i < FLAME_MAX; i++)
			{
				if (m_pBillFire[i]) m_pBillFire[i]->SetActive(FALSE);
			}
		}
		else {
			// マップとのあたり判定
			VECTOR3 vHit, vNormal;
			if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
			{
				m_pGMain->m_pEffectProc->m_pBillProc->Start(vHit);	// 爆発ビルボードの発生
				m_pGMain->m_pSeDead->Play();// 爆発効果音
				m_bActive = FALSE;
				for (DWORD i = 0; i < FLAME_MAX; i++)
				{
					if (m_pBillFire[i]) m_pBillFire[i]->SetActive(FALSE);
				}
			}
		}

		m_nCnt1--;
		if (m_nCnt1 <= 0)
		{
			m_bActive = FALSE;
			for (DWORD i = 0; i < FLAME_MAX; i++)
			{
				if (m_pBillFire[i]) m_pBillFire[i]->SetActive(FALSE);
			}
		}

		// レンダリング
		Render();

		// 火炎を囲む炎の更新
		for (DWORD i = 0; i < FLAME_MAX; i++)
		{
			if (m_pBillFire[i]) m_pBillFire[i]->m_vPos = GetPositionVector(m_mWorld) + m_pBillFire[i]->GetPosUp();
		}
	}

}

// ---------------------------------------------------------------------------
//
// 武器・火炎オブジェクトのレンダリング
//
//   陰影をつけないレンダリング　光の方向が(0,0,0)
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponFlameObj::Render()
{

	// 火炎のレンダリング
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);        // -- 2022.2.16
}



// ================================================================================================================================
//============================================================================
//
// 武器・ボールプロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CWeaponBallProc::CWeaponBallProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// ボールメッシュの設定
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Mark.mesh"));

	for (int i = 0; i < BALL_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponBallObj(this));	// m_pObjArrayに武器・ボールオブジェクトを生成する
	}
}
//============================================================================
//
// 武器・ボールプロシージャのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponBallProc::~CWeaponBallProc()
{
	;
}
// ---------------------------------------------------------------------------
//
// 武器・ボールプロシージャの発射
//
//  MATRIX4X4  mGun         ：銃の位置マトリックス
//  VECTOR3    vOffset      ：位置の微調整
//  MATRIX4X4  mOwnerWorld  ：発射するキャラのマトリックス
//  DWORD      dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
// ---------------------------------------------------------------------------
BOOL CWeaponBallProc::Start(MATRIX4X4 mGun, VECTOR3 vOffset, MATRIX4X4 mOwnerWorld, DWORD dwOwner)
{
	MATRIX4X4 mLay, mOwnerRot, mOffset;

	mLay = GetPositionMatrix(mGun);               // 銃の位置(回転を取り除く)
	mOwnerRot = GetRotateMatrix(mOwnerWorld);     // 発射するキャラの回転マトリックスのみ(位置を取り除く)にする
	mLay = mOwnerRot * mLay;                      // 銃の位置から、キャラの方向で発射するマトリックスを作る
	mOffset = XMMatrixTranslation(vOffset.x, vOffset.y, vOffset.z);	// 位置の微調整
	mLay = mOffset * mLay;

	for (int i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mLay, dwOwner);	       // 武器・ボールオブジェクトの発射
			return TRUE;
		}
	}
	return FALSE;
}

//============================================================================
//
// 武器・ボールオブジェクトのコンストラクタ
//
// ---------------------------------------------------------------------------
CWeaponBallObj::CWeaponBallObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = 100;  // 攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;
	m_nAtc = nAtc;
	m_dwOwner = 0;

}
// ---------------------------------------------------------------------------
//
// 武器・ボールオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponBallObj::~CWeaponBallObj()
{
	;
}

// ---------------------------------------------------------------------------
//
// 武器・ボールオブジェクトの発射
//
//     MATRIX4X4 mStartWorld     ボールの発射位置のマトリックス
//     DWORD      dwOwner        武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponBallObj::Start(MATRIX4X4 mStartWorld, DWORD dwOwner)
{
	const float fSpeed = 0.7f;		// ボールの速さ
	const int nLifeTime = 150;		// ボールの持続時間

	if (m_bActive) return FALSE;

	m_bActive = TRUE;
	m_mWorld    = mStartWorld;
	m_mWorldOld = mStartWorld;

	m_vPosUp  = VECTOR3(0, 0, fSpeed);
	m_nCnt1   = nLifeTime;
	m_dwOwner = dwOwner;

	return TRUE;
}

// ---------------------------------------------------------------------------
//
// 武器・ボールオブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponBallObj::Update()
{

	if (m_bActive)
	{
		m_mWorldOld = m_mWorld;

		MATRIX4X4 mTran;
		mTran = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y, m_vPosUp.z);
		m_mWorld = mTran * m_mWorld;

		// 敵やＰＣとのあたり判定
		if ((m_dwOwner != ENM && m_pGMain->m_pEnmProc->Hitcheck(this, GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld))) ||
			(m_dwOwner != PC  && m_pGMain->m_pPcProc->Hitcheck(this, GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld))))
		{
			m_pGMain->m_pEffectProc->m_pParticleProc->Start(m_vHitPos, m_vHitNormal);	// パーティクルの発生
			m_pGMain->m_pSeDead->Play();// 爆発効果音
			m_bActive = FALSE;
		}
		else {
			// マップとのあたり判定
			VECTOR3 vHit, vNormal;
			if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
			{
				VECTOR3 vL = normalize(GetPositionVector(m_mWorld) - GetPositionVector(m_mWorldOld));  // 移動ベクトル（正規化）
				VECTOR3 vN = normalize(vNormal);            // 接触ポリゴンの法線（正規化）
				VECTOR3 vR = (2 * dot(-vL, vN) * vN) + vL;  // 反射ベクトルの公式
				m_mWorldOld = m_mWorld = GetLookatMatrix(vHit, vHit+vR);  // 接触点から反射先を見るマトリックスを作成し、ワールドマトリックスに代入

				m_pGMain->m_pEffectProc->m_pParticleProc->Start(vHit, vNormal);	// パーティクルの発生
				m_pGMain->m_pSeDead->Play();// 爆発効果音
			}
		}

		m_nCnt1--;
		if (m_nCnt1 <= 0)
		{
			m_bActive = FALSE;
		}

		// レンダリング
		Render();

	}
}

// ---------------------------------------------------------------------------
//
// 武器・ボールオブジェクトのレンダリング
//
//   陰影をつけないレンダリング　光の方向が(0,0,0)
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponBallObj::Render()
{

	// ボールのレンダリング
	GetMesh()->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);        // -- 2022.2.16
}


// ================================================================================================================================
//============================================================================
//
// 武器・スプレープロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CWeaponSprayProc::CWeaponSprayProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	// スプライトイメージの読み込み
	m_pImage = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/spray.png"));

	for (int i = 0; i < SPRAY_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponSprayObj(this));	// m_pObjArrayに武器・スプレーオブジェクトを生成する
		((CWeaponSprayObj*)m_pObjArray.back())->CreateSprite(m_pImage);  // スプライトをオブジェクトに設定する
	}
}
//============================================================================
//
// 武器・スプレープロシージャのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponSprayProc::~CWeaponSprayProc()
{
	SAFE_DELETE(m_pImage);
}
// ---------------------------------------------------------------------------
//
// 武器・スプレープロシージャの発射
//
//  MATRIX4X4  mGun         ：銃の位置マトリックス。マトリックスの位置のみ使用する
//  VECTOR3    vOffset      ：位置の微調整
//  MATRIX4X4  mOwnerWorld  ：発射するキャラのマトリックス。マトリックスの角度のみ使用する
//  DWORD      dwOwner      ：武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
// ---------------------------------------------------------------------------
BOOL CWeaponSprayProc::Start(MATRIX4X4 mGun, VECTOR3 vOffset, MATRIX4X4 mOwnerWorld, DWORD dwOwner)
{
	MATRIX4X4 mLay, mOwnerRot, mOffset;
	const int nStartMax = 30;   // 一度に何個発射させるか

	mLay = GetPositionMatrix(mGun);               // 銃の位置(回転を取り除く)
	mOwnerRot = GetRotateMatrix(mOwnerWorld);     // 発射するキャラの回転マトリックスのみ(位置を取り除く)にする
	mLay = mOwnerRot * mLay;                      // 銃の位置から、キャラの方向で発射するマトリックスを作る
	mOffset = XMMatrixTranslation(vOffset.x, vOffset.y, vOffset.z);	// 位置の微調整
	mLay = mOffset * mLay;

	for (int n = nStartMax, i = 0; i < m_pObjArray.size(); i++)
	{
		if (!m_pObjArray[i]->GetActive())
		{
			m_pObjArray[i]->Start(mLay, dwOwner);	       // 武器・スプレーオブジェクトの発射
			n--;
			if (n<0)	return TRUE;
		}
	}
	return FALSE;
}

//============================================================================
//
// 武器・スプレーオブジェクトのコンストラクタ
//
// ---------------------------------------------------------------------------
CWeaponSprayObj::CWeaponSprayObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nAtc = 30;  // 攻撃力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;
	m_nAtc = nAtc;
	m_dwOwner = 0;

}
// ---------------------------------------------------------------------------
//
// 武器・スプレーオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponSprayObj::~CWeaponSprayObj()
{
	SAFE_DELETE(m_pSprite);
}

// ---------------------------------------------------------------------------
//
// 武器・スプレー  スプライトの設定
//
// ---------------------------------------------------------------------------
void CWeaponSprayObj::CreateSprite(CSpriteImage*  pImage)
{
	m_pSprite = new CSprite(pImage);

	// スプレーの煙
	// (表示サイズwidth,Height    スプライト位置x,y   スプライトサイズwidth,Height)
	m_pSprite->SetSrc3D(0.5f, 0.5f, Random(0, 2) * 85, 0, 85, 85);

	//m_pSprite->m_nBlend = 2;  // 加算合成の指定

}

// ---------------------------------------------------------------------------
//
// 武器・スプレーオブジェクトの発射
//
//     MATRIX4X4 mStartWorld     スプレーの発射位置のマトリックス
//     DWORD      dwOwner        武器を発射したキャラ区分（PC:PC(省略値)　ENM:敵）
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponSprayObj::Start(MATRIX4X4 mStartWorld, DWORD dwOwner)
{
	const float fSpeed = 1.5f;		// スプレーの速さ
	const int nLifeTime = 10;		// スプレーの持続時間
	const float fRotMax = 3.0f;		// スプレーの広がり角度

	float fRotX = 0.0f, fRotY = 0.0f;

	if (m_bActive) return FALSE;

	m_bActive = TRUE;

	// 発射方向を上下左右にブレさす
	fRotX = (Random(0, 100) - 50) / 50.0f * fRotMax;
	fRotY = (Random(0, 100) - 50) / 50.0f * fRotMax;

	mStartWorld = XMMatrixRotationX(XMConvertToRadians(fRotX)) * XMMatrixRotationY(XMConvertToRadians(fRotY)) * mStartWorld;

	m_mWorld = mStartWorld;
	m_mWorldOld = mStartWorld;

	// 発射速度をランダムに
	float fSp = fSpeed * 0.7f + Random(0, 50) / 50.0f * fSpeed;

	m_vPosUp = VECTOR3(0, 0, fSp);
	m_nCnt1 = nLifeTime;
	m_dwOwner = dwOwner;

	return TRUE;
}

// ---------------------------------------------------------------------------
//
// 武器・スプレーオブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponSprayObj::Update()
{

	if (m_bActive)
	{
		m_mWorldOld = m_mWorld;

		MATRIX4X4 mTran;
		mTran = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y, m_vPosUp.z);
		m_mWorld = mTran * m_mWorld;

		// 敵やＰＣとのあたり判定
		if ((m_dwOwner != ENM && m_pGMain->m_pEnmProc->Hitcheck(this, GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld))) ||
			(m_dwOwner != PC  && m_pGMain->m_pPcProc->Hitcheck(this, GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld))))
		{
			m_pGMain->m_pEffectProc->m_pParticleProc->Start(m_vHitPos, m_vHitNormal);	// パーティクルの発生
			m_pGMain->m_pSeDead->Play();// 爆発効果音
			m_bActive = FALSE;
		}
		else {
			// マップとのあたり判定
			VECTOR3 vHit, vNormal;
			if (m_pGMain->m_pMapProc->Hitcheck(GetPositionVector(m_mWorld), GetPositionVector(m_mWorldOld), &vHit, &vNormal))
			{
				//m_pGMain->m_pEffectProc->m_pParticleProc->Start(vHit, vNormal);	// パーティクルの発生
				//m_pGMain->m_pSeDead->Play();// 爆発効果音
				m_bActive = FALSE;
			}
		}

		m_nCnt1--;
		if (m_nCnt1 <= 0)
		{
			m_bActive = FALSE;
		}

		// レンダリング
		Render();

	}
}

// ---------------------------------------------------------------------------
//
// 武器・スプレーオブジェクトのレンダリング
//
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponSprayObj::Render()
{

	// スプレーの描画
	m_pSprite->Draw3D(GetPositionVector(m_mWorld), m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt);        // -- 2022.2.16


}


// ============================================================================================================================
//============================================================================
//
// 武器・手に持つアイテムプロシージャのコンストラクタ
//
// ・直接攻撃する武器ではなく、武器を表示したり、位置情報を返したりする
//
//   メッシュはプロシージャではなく、オブジェクトで読み込む
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CWeaponHoldProc::CWeaponHoldProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	for (int i = 0; i < HOLDITEM_MAX; i++)
	{
		m_pObjArray.push_back(new CWeaponHoldObj(this));	// m_pObjArrayにオブジェクトを生成する
		((CWeaponHoldObj*)m_pObjArray.back())->Init();   // オブジェクトの初期化
	}
}
// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムプロシージャのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponHoldProc::~CWeaponHoldProc()
{
	;
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムプロシージャの情報を得る
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//     MATRIX4X4& mMat        :手に持つ武器のマトリックス(Out)
//     VECTOR3& vMin          :バウンディングボックスのMin(Out)
//     VECTOR3& vMax          :バウンディングボックスのMax(Out)
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
void  CWeaponHoldProc::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax)
{
	((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetInfo(mHold, HoldItem, mMat, vMin, vMax);	// 武器・剣オブジェクト

	return;
}
// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムプロシージャの情報を得る
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//     MATRIX4X4& mMat        :手に持つ武器のマトリックス(Out)
//     VECTOR3& vOffset       :銃口までのオフセット(Out)
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
void  CWeaponHoldProc::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset)
{
	((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetInfo(mHold, HoldItem, mMat, vOffset);	// 武器・銃オブジェクト

	return;
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムプロシージャのバウンディングボックスを得る
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//
//     戻り値　　CBBox* バウンディングボックス
// ---------------------------------------------------------------------------
CBBox*  CWeaponHoldProc::GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{
	return ((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetBBox(mHold, HoldItem);	// 武器・剣オブジェクト
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムプロシージャの表示
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponHoldProc::Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{

	((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->Disp(mHold, HoldItem);	// 武器・剣オブジェクト

	return TRUE;
}
//============================================================================
//
// 武器・手に持つアイテムオブジェクトのコンストラクタ
//
// ・直接攻撃する武器ではなく、武器を表示したり、位置情報を返したりする
//
// ---------------------------------------------------------------------------
CWeaponHoldObj::CWeaponHoldObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	m_nKindNo = 0;
	m_pMesh = NULL;
	m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // オフセットの設定
}
//============================================================================
//
// 武器・手に持つアイテムオブジェクトの初期化
//
// ・メッシュは、プロシージャではなくオブジェクトの中に設定する
//
// ---------------------------------------------------------------------------
void CWeaponHoldObj::Init()
{

	switch (m_dwObjNo)
	{
	case 0:     // 未設定
		m_nKindNo = 0;
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = NULL; // BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // オフセットなし

	case 1:     // 透明な剣の設定
		m_nKindNo = 1;  // 1:種別　剣
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, -0.15f, -0.15f), VECTOR3(0.15f, 1.8f, 0.15f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 2:     // 剣（日本刀）メッシュの設定
		m_nKindNo = 1;  // 1:種別　剣
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Sword.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.8f, 0.15f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 3:     // 剣（ソード）メッシュの設定
		m_nKindNo = 1;  // 1:種別　剣
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Sword2.mesh"));
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.15f, 0.1f, -0.15f), VECTOR3(0.15f, 1.8f, 0.15f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	case 4:     // 透明な銃の設定
		m_nKindNo = 2;  // 2:種別　銃
		m_pMesh = NULL; // オブジェクトは無し
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.51f);   // 銃口までのオフセットの設定
		break;

	case 5:     // 銃（ライフル）メッシュの設定
		m_nKindNo = 2;  // 2:種別　銃
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Gun.mesh"));
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.51f);   // 銃口までのオフセットの設定
		break;

	case 6:     // 銃（ピストル）メッシュの設定
		m_nKindNo = 2;  // 2:種別　銃
		m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Item/Pistol.mesh"));
		m_vOffset = VECTOR3(0.0f, 0.07f, 0.17f);   // 銃口までのオフセットの設定
		break;

	case 7:     // 透明な盾の設定
		m_nKindNo = 1;  // 1:種別　剣
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-1.0f, -1.5f, -0.3f), VECTOR3(1.0f, 1.5f, 0.3f));// BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // 銃口までのオフセットの設定
		break;

	default:
		m_nKindNo = 0;
		m_pMesh = NULL; // オブジェクトは無し
		m_pBBox = NULL; // BBoxの設定
		m_vOffset = VECTOR3(0.0f, 0.0f, 0.0f);   // オフセットなし
		break;
	}

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

}
// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CWeaponHoldObj::~CWeaponHoldObj()
{
	SAFE_DELETE(m_pMesh);
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトの情報を得る
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//     MATRIX4X4& mMat        :手に持つ武器のマトリックス(Out)
//     VECTOR3& vMin          :バウンディングボックスのMin(Out)
//     VECTOR3& vMax          :バウンディングボックスのMax(Out)
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
void  CWeaponHoldObj::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax)
{
	// オフセットマトリックスの計算
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// 
	// ワールドマトリックスの計算
	// 
	// 但し、このときm_mWorldやm_pBBox->m_mWorldの設定は行っていない!!
	// 
	mMat = mOffset * mHold;     // 剣の位置のワールドマトリックス

	vMin = m_pBBox->m_vMin;
	vMax = m_pBBox->m_vMax;

	return;
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトの情報を得る
//
//     MATRIX4X4  mHold       ：銃の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//     MATRIX4X4& mMat        :手に持つ武器のマトリックス(Out)
//     VECTOR3& vOffset       :銃口までのオフセット(Out)
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
void  CWeaponHoldObj::GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset)
{
	// オフセットマトリックスの計算
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// ワールドマトリックスの計算
	// 
	// 但し、このときm_mWorldの設定は行っていない!!
	// 
	mMat = mOffset * mHold;     // 銃の位置のワールドマトリックス

	vOffset = m_vOffset;

	return;
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトのバウンディングボックスを得る
//
//     MATRIX4X4  mHold       ：剣の位置
//     BASEHOLDITEM HoldItem  :手に持つ武器の情報
//
//     戻り値　　CBBox* バウンディングボックス
// ---------------------------------------------------------------------------
CBBox* CWeaponHoldObj::GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{

	// オフセットマトリックスの計算
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// ワールドマトリックスの計算
	// 
	// 但し、このときm_mWorldとm_pBBox->m_mWorldの設定を行っている!!
	// 
	m_mWorld = mOffset * mHold;     // 剣の位置のワールドマトリックス
	m_pBBox->m_mWorld = m_mWorld;   // バウンディングボックスのワールドマトリックスをセット

	return m_pBBox;

}


// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトの描画
//
//     MATRIX4X4  mHold        ：アイテムの位置
//     VECTOR3    vPos         :位置のオフセット
//     VECTOR3    vRot         :角度のオフセット
//
//     戻り値　　なし
// ---------------------------------------------------------------------------
BOOL CWeaponHoldObj::Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem)
{
	m_bActive = TRUE;  // スタート時に表示開始

	// オフセットマトリックスの計算
	MATRIX4X4 mPos = XMMatrixTranslation(HoldItem.m_nOffsetPos.x, HoldItem.m_nOffsetPos.y, HoldItem.m_nOffsetPos.z);

	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(HoldItem.m_nOffsetRot.x));
	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(HoldItem.m_nOffsetRot.y));
	MATRIX4X4 mRotZ = XMMatrixRotationZ(XMConvertToRadians(HoldItem.m_nOffsetRot.z));

	MATRIX4X4 mOffset = mRotY * mRotX * mRotZ;
	mOffset = mOffset * mPos;

	// ワールドマトリックスの計算
	m_mWorld = mOffset * mHold;     // 剣の位置のワールドマトリックス
	if (m_pBBox) m_pBBox->m_mWorld = m_mWorld;   // バウンディングボックスのワールドマトリックスをセット

	// レンダリング
	Render();

	m_bActive = FALSE;  // １回処理したら、消す

	return TRUE;
}

// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトの更新
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponHoldObj::Update()
{
	;
}
// ---------------------------------------------------------------------------
//
// 武器・手に持つアイテムオブジェクトの描画
//
//     引数　　　なし
//     戻り値　　なし
// ---------------------------------------------------------------------------
void CWeaponHoldObj::Render()
{

	// 手に持つアイテムのレンダリング
	if (m_pMesh) m_pMesh->Render(m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	// バウンディングボックスレンダリング
	//if (m_pBBox) m_pBBox->Render(m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);
}
