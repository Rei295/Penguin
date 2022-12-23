//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//		  マップの処理
//																	Map.cpp
//=============================================================================
#include  "BaseProc.h"
#include  "Map.h"
#include  "Playchar.h"
#include  "Enemy.h"
#include  "Weapon.h"
#include  "Fellow.h"


//=============================================================================
//
// マッププロシージャのコンストラクタ
//
//-----------------------------------------------------------------------------
CMapProc::CMapProc(CGameMain* pGMain)
{
	// クラス全体のクリヤー
	// (vectorクラスは初期化不要)
	m_pGMain = pGMain;
	m_dwMapNo = 0;
	m_bActive = FALSE;
	m_pColMesh = NULL;					// コリジョンメッシュへのポインタ
	m_nEvtIdx = 0;							// -- 2021.2.4

}

//-----------------------------------------------------------------------------
//
// マッププロシージャのデストラクタ
//
//-----------------------------------------------------------------------------
CMapProc::~CMapProc()
{
	DestroyAll();
}

//-----------------------------------------------------------------------------
//
// マップの削除
//
//-----------------------------------------------------------------------------
void CMapProc::DestroyAll()
{
	m_dwMapNo = 0;
	m_bActive = FALSE;
	m_nEvtIdx = 0;							// -- 2021.2.4

	// メッシュのデリート
	for (DWORD i = 0; i < m_SkyMap.size(); i++)
	{
		SAFE_DELETE(m_SkyMap[i].m_pMesh);
		SAFE_DELETE(m_SkyMap[i].m_pWave);    // 使用していないが念のため削除
	}
	for (DWORD i = 0; i < m_StageMap.size(); i++)
	{
		SAFE_DELETE(m_StageMap[i].m_pMesh);
		SAFE_DELETE(m_StageMap[i].m_pWave);
	}
	for (DWORD i = 0; i < m_MoveMap.size(); i++)
	{
		SAFE_DELETE(m_MoveMap[i].m_pMesh);
		SAFE_DELETE(m_MoveMap[i].m_pColMesh);
	}
	for (DWORD i = 0; i < m_EventMap.size(); i++)   // -- 2021.2.4
	{
		SAFE_DELETE(m_EventMap[i].m_pBBox);
	}
	SAFE_DELETE(m_pColMesh);

	// vectorの削除
	m_SkyMap.clear();					// 空のマップ  配列削除
	m_SkyMap.shrink_to_fit();			// 空のマップ  不要メモリ解放
	m_StageMap.clear();					// ステージのマップ  配列削除
	m_StageMap.shrink_to_fit();			// ステージのマップ 不要メモリ解放
	m_MoveMap.clear();					// 移動マップ  配列削除
	m_MoveMap.shrink_to_fit();			// 移動マップ 不要メモリ解放
	m_NavMap.clear();					// ナビゲーションマップ配列削除
	m_NavMap.shrink_to_fit();			// ナビゲーションマップ不要メモリ解放
	m_EventMap.clear();					// イベントマップ配列削除
	m_EventMap.shrink_to_fit();			// イベントマップ不要メモリ解放

}

//-----------------------------------------------------------------------------
//
// マップ１の作成と移動の処理
//
//-----------------------------------------------------------------------------
void CMapProc::LoadMap1()
{
	// vector初期化用の構造体
	const STAGEMAP      sm;
	const MOVEMAP       mm;
	const NAVIGATIONMAP nv;

	// 全マップの削除 -----------------------------------------------
	DestroyAll();

	// 全ての敵・武器・効果の非表示化 
	m_pGMain->m_pEnmProc->SetNonActive();
	m_pGMain->m_pWeaponProc->SetNonActive();
	m_pGMain->m_pEffectProc->SetNonActive();
	m_pGMain->m_pEnmProc->ResetEnmNum();      // 敵の発生数のリセット

	// ＰＣを置く -------------------------------------
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetWorld(XMMatrixTranslation(11.0f, 0.8f, 500.0f));        // -- 2022.2.16
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetWorldOld(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
	m_pGMain->m_pPcProc->GetPcObjPtr()->SetLocalRotY(90.0f);  // ローカル軸(Y軸)を0度にする

	// ステージマップの設定・コリジョンマップの設定 -----------------------------------------
	m_pColMesh = new CCollision(m_pGMain->m_pFbxMeshCtrl);     // コリジョンマップの生成

	// マップメッシュの設定[0]
	m_StageMap.push_back(sm);
	m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/MapSnow/MapSnow.mesh"));       // -- 2022.2.16
	m_StageMap.back().m_mWorld = XMMatrixTranslation(500.0f, 0.0f, 500.0f);
	m_pColMesh->AddFbxLoad(_T("Data/Map/MapSnow/MapSnow02.mesh"), VECTOR3(500.0f, 0.0f, 500.0f)); // マップメッシュコリジョンの設定

	// ドアメッシュの設定[1]
	m_StageMap.push_back(sm);
	m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/MapSnow/Castle.mesh"));
	m_StageMap.back().m_mWorld = XMMatrixTranslation(950.0f, 0.0f, 500.0f);
	m_pColMesh->AddFbxLoad(_T("Data/Map/MapSnow/Castle.mesh"), VECTOR3(-100.0f, 0.0f, 100.0f)); // ドアメッシュコリジョンの設定
	SetEvent(VECTOR3(-5.0f, -0.05f, -100.0f), VECTOR3(100.0f, 100.0f, 513.0f), m_StageMap.back().m_mWorld, eEvtChangeMap, 2);  // イベントの設定(マップ２へ移動する)

	// 立木メッシュの設定[2]
	m_StageMap.push_back(sm);
	m_StageMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/Tree/tree5.mesh"));
	m_StageMap.back().m_mWorld = XMMatrixTranslation(-80.0f, 0.0f, 20.0f);
	m_StageMap.back().m_nMaterialFlag = 1;   // 透明色有効
	m_pColMesh->AddFbxLoad(_T("Data/Map/Tree/tree5_chk.mesh"), VECTOR3(-80.0f, 0.0f, 20.0f));  // 立木メッシュコリジョンの設定

	// 水面の設定[3]
	m_StageMap.push_back(sm);
	m_StageMap.back().m_pWave = new CWave(m_pGMain->m_pShader, 1000, 10000, _T("Data/Map/MapItem/sea4.png"));             // 水面の大きさ（メートル単位）とノーマルマップテクスチャの指定
	m_StageMap.back().m_mWorld = XMMatrixTranslation(-1.0f, -0.5f, -5.0f);      // 水面を置く位置（作成した水面の左端、手前端の位置を指定する）
	m_StageMap.back().m_nMaterialFlag = 3;   // 水面のディスプレイスメントマッピング

	
	// 空マップの設定[0]
	m_SkyMap.push_back(sm);
	m_SkyMap.back().m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Map/Sky/sky2.mesh"));
	m_SkyMap.back().m_mWorld = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	// ナビゲーションマップの設定 --------------------------------------
	// ナビマップの設定[0]
	m_NavMap.push_back(nv);
	m_NavMap.back().vMin = VECTOR3(20.0f, 0.8f, 490.0f);               // -- 2022.2.16
	m_NavMap.back().vMax = VECTOR3(950.0f, 0.8f, 513.0f);
	m_NavMap.back().m_dwFellowID.push_back(FELLOW_ID);   // 仲間のＩＤ  

#if 0
	// ナビマップの設定[1]
	m_NavMap.push_back(nv);
	m_NavMap.back().vMin = VECTOR3(20.0f, 0.8f, 490.0f);               // -- 2022.2.16
	m_NavMap.back().vMax = VECTOR3(950.0f, 0.8f, 513.0f);
	m_NavMap.back().m_dwEnmID.push_back(ENM_RS_ID);   // 敵のＩＤ  
#endif	

	// マップＮＯ１
	m_dwMapNo = 1;
	m_bActive = TRUE;

}

//-----------------------------------------------------------------------------
// マッププロシージャのあたり判定
//
// ・相手（主にＰＣの武器）とコリジョンマップとのあたり判定
//
//   CBaseObj*    pObj      相手のオブジェクト
//   VECTOR3* pHit		接触点の座標（出力）
//   VECTOR3* pNormal	接触点の法線ベクトル（出力）wwwwwwwwwwwwwwwwwwwww

//
//   戻り値　BOOL
//           TRUE:ヒットしたとき       FALSE：ヒットしていないとき
//
//-----------------------------------------------------------------------------
BOOL   CMapProc::Hitcheck(CBaseObj* pObj, VECTOR3* pHit, VECTOR3* pNormal)
{
	return  Hitcheck(GetPositionVector(pObj->GetWorld()), GetPositionVector(pObj->GetWorldOld()), pHit, pNormal);
}
//-----------------------------------------------------------------------------
// マッププロシージャのあたり判定
//
// ・相手（主にＰＣの武器）とコリジョンマップとのあたり判定
//
//   VECTOR3 vNow       相手の現在位置
//   VECTOR3 vOld       相手の一つ前の位置
//   VECTOR3* vHit		接触点の座標（出力）
//   VECTOR3* vNormal	接触点の法線ベクトル（出力）
//
//   戻り値　BOOL
//           TRUE:ヒットしたとき       FALSE：ヒットしていないとき
//
//-----------------------------------------------------------------------------
BOOL   CMapProc::Hitcheck(VECTOR3 vNow, VECTOR3 vOld, VECTOR3* pHit, VECTOR3* pNormal)
{
	BOOL  bRet;
	for (int i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
		{
			bRet = m_MoveMap[i].m_pColMesh->isCollisionLay(vNow, vOld, *pHit, *pNormal);
			if (bRet) return bRet;
		}
	}

	if (m_pColMesh && m_pColMesh->isCollisionLay(vNow, vOld, *pHit, *pNormal))
	{
		return TRUE;
	}
	else {
		return FALSE;
	}

}
//-----------------------------------------------------------------------------  // -- 2020.12.11
// マッププロシージャのあたり判定
//
// ・相手（主にＰＣの武器）の球とコリジョンマップとのあたり判定
//
//   VECTOR3 vNow       相手の現在のオブジェクト中心位置
//   VECTOR3 vOld       相手の一つ前のオブジェクト中心位置
//   float& fRadius     球の半径
//   VECTOR3 &vHit      接触時のオブジェクト中心位置の座標（出力）
//   VECTOR3 &vNormal   接触点の法線ベクトル（出力）
//
//   戻り値　BOOL
//           TRUE:ヒットしたとき       FALSE：ヒットしていないとき
//
//-----------------------------------------------------------------------------
BOOL   CMapProc::Hitcheck(VECTOR3 vNow, VECTOR3 vOld, float fRadius, VECTOR3* pHit, VECTOR3* pNormal)
{
	BOOL  bRet;
	for (int i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
		{
			bRet = m_MoveMap[i].m_pColMesh->isCollisionSphere(vNow, vOld, fRadius, *pHit, *pNormal);
			if (bRet) return bRet;
		}
	}

	if (m_pColMesh && m_pColMesh->isCollisionSphere(vNow, vOld, fRadius, *pHit, *pNormal))
	{
		return TRUE;
	}
	else {
		return FALSE;
	}

}

//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かす制御
//	高低差と重力を考慮した、接触判定を行う									2019.9.3
//  
//  MATRIX4X4* pWorld     オブジェクトの現在のマトリックス(in,out)
//  MATRIX4X4  mWorldOld  オブジェクトの一つ前のマトリックス		
//  float fRadius          オブジェクトの半径（省略値は0.2）
//  
//  戻り値　int
//		エラー		= -1
//		面上を移動	= 1
//		着地		= 2
//		落下中		= 3
//-----------------------------------------------------------------------------
int  CMapProc::isCollisionMoveGravity(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius)
{
	int  nRet = 0, nRetMove = 0;  // -- 2019.9.3

	// 移動マップとの接触判定と移動
	for (int i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
		{
			nRetMove = m_MoveMap[i].m_pColMesh->isCollisionMoveGravity(*pWorld, mWorldOld, fRadius);  // -- 2019.9.3
			if (nRetMove == 1 || nRetMove == 2) break;   // 移動マップと接触したとき
		}
	}

	// 通常のマップとの接触判定と移動
	if (m_pColMesh)
	{
		nRet = m_pColMesh->isCollisionMoveGravity(*pWorld, mWorldOld, fRadius);  // -- 2019.9.3
	}

	if (nRetMove == 1 || nRetMove == 2)  // 移動マップと接触していたとき   // -- 2019.9.3
	{
		return nRetMove;
	}
	else {
		return nRet;
	}
}
//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かす制御
//	重力を考えず、接触判定を行う											2019.9.3
//  
//  MATRIX4X4* pWorld     オブジェクトの現在のマトリックス(in,out)
//  MATRIX4X4  mWorldOld  オブジェクトの一つ前のマトリックス		
//  float fRadius          オブジェクトの半径（省略値は0.2）
//  
//  戻り値　int
//		接触したとき　		１
//		接触していないとき	０
//-----------------------------------------------------------------------------
int CMapProc::isCollisionMove(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius)
{
	int  nRet = 0, nRetMove = 0;  // -- 2019.9.3

	// 移動マップとの接触判定と移動
	for (int i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)
		{
			nRetMove = m_MoveMap[i].m_pColMesh->isCollisionMove(*pWorld, mWorldOld, fRadius);  // -- 2019.9.3
			if (nRetMove == 1) break;
		}
	}

	// 通常のマップとの接触判定と移動
	if (m_pColMesh)
	{
		nRet = m_pColMesh->isCollisionMove(*pWorld, mWorldOld, fRadius);  // -- 2019.9.3
	}

	if (nRetMove == 1)  // 移動マップと接触していたとき     // -- 2019.9.3
	{
		return nRetMove;
	}
	else {
		return nRet;
	}
}

//-----------------------------------------------------------------------------
//
// マッププロシージャの更新
//
//-----------------------------------------------------------------------------
void CMapProc::Update()
{
	CDirectInput* pDI = m_pGMain->m_pDI;


	
	

	// イベントの更新処理                         // -- 2021.2.4
	UpdateEvent();

	// 空の更新　　カメラを中心とした位置に置き、Ｙ軸回転させる
	if (m_SkyMap.size() > 0)
	{
		MATRIX4X4 mRot;
		mRot = XMMatrixRotationY(XMConvertToRadians(0.005f));
		mRot = mRot * GetRotateMatrix(m_SkyMap[0].m_mWorld);
		m_SkyMap[0].m_mWorld = XMMatrixTranslation(m_pGMain->m_vEyePt.x, m_pGMain->m_vEyePt.y, m_pGMain->m_vEyePt.z);
		m_SkyMap[0].m_mWorld = mRot * m_SkyMap[0].m_mWorld;
	}

	// 移動マップ更新処理
	UpdateMoveMap();

	//レンダリング
	Render();

}
//-----------------------------------------------------------------------------   // -- 2021.2.4
//
// イベントのセット
//
// 引数
//   VECTOR3 vMin         : バウンディングボックス最小値
//   VECTOR3 vMax         : バウンディングボックス最大値
//   MATRIX4X4 mWorld     : バウンディングボックスのワールドマトリックス
//   EVENTKIND nEvtKind   : イベントの種類 eEvtChangeMap:マップ移動, eEvtMoveMap:移動マップの制御
//   int nEvtNo           : イベント番号 eEvtChangeMapのときは移動先のマップ番号、eEvtMoveMapのときは移動マップの要素番号
//   int nEvtOpe1         : eEvtMoveMapのみ。1:移動マップの移動Activeの指定。2:移動マップの移動フラグの指定(省略値0)
//   int nEvtOpe2         : eEvtMoveMapのみ。移動Activeのときは0:非表示 1:表示  移動フラグのときは　0:移動無し　1:平行移動　2:回転　3:拡大縮小(省略値0)
//   int nEvtKeyPush      : 0:バウンディングボックスに接触するとイベント発動、 1:バウンディングボックスに接触して、ENTERキーを押すとイベント発動(省略値0)
//
// 戻り値
//   int セットしたイベント配列の要素番号
//
//-----------------------------------------------------------------------------
int  CMapProc::SetEvent(VECTOR3 vMin, VECTOR3 vMax, MATRIX4X4 mWorld, EVENTKIND nEvtKind, int nEvtNo, int nEvtOpe1, int nEvtOpe2, int nEvtKeyPush)
{
	const EVENTMAP      em;

	m_EventMap.push_back(em);

	// バウンディングボックスの設定
	m_EventMap[m_nEvtIdx].m_pBBox = new CBBox(m_pGMain->m_pShader, vMin, vMax);
	m_EventMap[m_nEvtIdx].m_pBBox->m_mWorld = mWorld;

	// 各項目の設定
	m_EventMap[m_nEvtIdx].m_nEvtCycle = 0;
	m_EventMap[m_nEvtIdx].m_nEvtKind = nEvtKind;
	m_EventMap[m_nEvtIdx].m_nEvtNo = nEvtNo;
	m_EventMap[m_nEvtIdx].m_nEvtOpe1 = nEvtOpe1;
	m_EventMap[m_nEvtIdx].m_nEvtOpe2 = nEvtOpe2;
	m_EventMap[m_nEvtIdx].m_nEvtKeyPush = nEvtKeyPush;

	m_nEvtIdx++;
	return  m_nEvtIdx - 1;
}

//-----------------------------------------------------------------------------   // -- 2021.2.4
//
// イベントの更新・実行
//
//-----------------------------------------------------------------------------
void  CMapProc::UpdateEvent()
{
	VECTOR3 vHit, vNrm;

	for (int i = 0; i < m_EventMap.size(); i++)
	{
		// PCのバウンディングボックスに接触しているかの判定
		if (m_EventMap[i].m_pBBox && m_EventMap[i].m_pBBox->OBBCollisionDetection(m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox(), &vHit, &vNrm))
		{
			// PCのバウンディングボックスに接触しているとき
			if (m_EventMap[i].m_nEvtCycle == 0)  // イベントサイクルが０のとき(まだ接触していなかったとき)
			{
				if (m_EventMap[i].m_nEvtKeyPush == 1 && !(m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_RETURN) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A)))  // Enterキープッシュが必要なとき
				{
					VECTOR3 vPc = GetPositionVector(m_pGMain->m_pPcProc->GetPcObjPtr()->GetWorld());
					vPc.y += 2.0f;
					// PCの頭上に、Enterキープッシュが行われていない事の表示をする
					m_pGMain->m_pFont->Draw3D(vPc, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, _T("Push Enter Key!"), VECTOR2(0.8f, 0.2f), RGB(255, 0, 0), 1.0f, _T("HGP創英角ｺﾞｼｯｸUB"));
				}
				else {
					// イベント実行
					RunEvent(m_EventMap[i]);
				}
			}
		}
		else {
			// PCのバウンディングボックスに接触していないとき
			if (m_EventMap[i].m_nEvtCycle == 2)  // イベントサイクルが２のとき(既に接触していて終了待ちのとき)
			{
				m_EventMap[i].m_nEvtCycle = 0;  // イベントサイクルを０に戻す
			}
		}
	}
}

//-----------------------------------------------------------------------------   // -- 2021.2.4
//
// イベントの実行
//
// 引数  EVENTMAP& EventMap:イベントマップ
//
//-----------------------------------------------------------------------------
void  CMapProc::RunEvent(EVENTMAP& EventMap)
{
	int i;

	if (EventMap.m_nEvtKind == eEvtChangeMap)   // マップ移動のとき
	{
		switch (EventMap.m_nEvtNo)
		{
		case 1:
			LoadMap1();
			break;
		case 2:
			m_pGMain->m_dwGameStatus = GAMECLEAR;
			break;
		}
	}
	else if (EventMap.m_nEvtKind == eEvtMoveMap)  // 移動マップの制御のとき
	{
		i = EventMap.m_nEvtNo;
		if (EventMap.m_nEvtOpe1 == 1)   // 表示有無に関する処理
		{
			m_MoveMap[i].m_bActive = EventMap.m_nEvtOpe2;
		}
		else if (EventMap.m_nEvtOpe1 == 2)   // 移動に関する処理
		{
			m_MoveMap[i].m_nMoveFlag = EventMap.m_nEvtOpe2;
		}
		EventMap.m_nEvtCycle = 2;  // イベントサイクルを２にする   // -- 2021.4.4
	}
}

//-----------------------------------------------------------------------------
//
// 移動マップの更新
//
// 移動マップの平行移動や回転の更新を行う
// 限界値（上端下端等）に達したときは、反転（change）を行う
// 反転（change）をしたときは、m_nChangeFlagに１が返る
// ループをしないときは反転のタイミングで停止する         // -- 2021.2.4
//
//-----------------------------------------------------------------------------
void CMapProc::UpdateMoveMap()
{
	for (DWORD i = 0; i < m_MoveMap.size(); i++)
	{
		if (m_MoveMap[i].m_pColMesh && m_MoveMap[i].m_bActive)   // 移動マップの更新処理
		{
			m_MoveMap[i].m_pColMesh->m_mWorldOld = m_MoveMap[i].m_pColMesh->m_mWorld;   // ワールドマトリックス一つ前
			m_MoveMap[i].m_nChangeFlag = 0;      // チェンジフラグのリセット
			if (m_MoveMap[i].m_nMoveFlag == 1)   // 平行移動の処理
			{
				MATRIX4X4 mPosUp;
				mPosUp = XMMatrixTranslation(m_MoveMap[i].m_vUp.x, m_MoveMap[i].m_vUp.y, m_MoveMap[i].m_vUp.z);
				m_MoveMap[i].m_pColMesh->SetWorldMatrix(mPosUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // 移動増分の処理
				if (m_MoveMap[i].m_vUp.x != 0)  // 左右に動かす
				{
					if (GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).x < m_MoveMap[i].m_vMin.x ||  // 左右に動かすため右端左端で反転
						GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).x > m_MoveMap[i].m_vMax.x)
					{
						m_MoveMap[i].m_vUp.x *= -1;
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
				if (m_MoveMap[i].m_vUp.y != 0)  // 上下に動かす
				{
					if (GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).y < m_MoveMap[i].m_vMin.y ||  // 上下に動かすため上端下端で反転
						GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).y > m_MoveMap[i].m_vMax.y)
					{
						m_MoveMap[i].m_vUp.y *= -1;
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
				if (m_MoveMap[i].m_vUp.z != 0)  // 前後に動かす
				{
					if (GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).z < m_MoveMap[i].m_vMin.z ||  // 前後に動かすため前端後端で反転
						GetPositionVector(m_MoveMap[i].m_pColMesh->m_mWorld).z > m_MoveMap[i].m_vMax.z)
					{
						m_MoveMap[i].m_vUp.z *= -1;
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
			}
			else if (m_MoveMap[i].m_nMoveFlag == 2) {   // 回転の処理
				MATRIX4X4 mRotUp;
				if (m_MoveMap[i].m_vUp.x != 0)  // Ｘ回転
				{
					mRotUp = XMMatrixRotationX(XMConvertToRadians(m_MoveMap[i].m_vUp.x));
					m_MoveMap[i].m_pColMesh->SetWorldMatrix(mRotUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // Ｘ回転増分の処理
					if (GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).x < m_MoveMap[i].m_vMin.x ||  // Ｘ回転するため端で反転
						GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).x > m_MoveMap[i].m_vMax.x)
					{
						m_MoveMap[i].m_vUp.x *= -1;  // Ｘ回転するため端で反転
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
				else if (m_MoveMap[i].m_vUp.y != 0)  // Ｙ回転
				{
					mRotUp = XMMatrixRotationY(XMConvertToRadians(m_MoveMap[i].m_vUp.y));
					m_MoveMap[i].m_pColMesh->SetWorldMatrix(mRotUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // Ｘ回転増分の処理
					if (GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).y < m_MoveMap[i].m_vMin.y ||  // Ｙ回転するため端で反転
						GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).y > m_MoveMap[i].m_vMax.y)
					{
						m_MoveMap[i].m_vUp.y *= -1;  // Ｙ回転するため端で反転
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
				else if (m_MoveMap[i].m_vUp.z != 0)  // Ｚ回転
				{
					mRotUp = XMMatrixRotationZ(XMConvertToRadians(m_MoveMap[i].m_vUp.z));
					m_MoveMap[i].m_pColMesh->SetWorldMatrix(mRotUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // Ｘ回転増分の処理
					if (GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).z < m_MoveMap[i].m_vMin.z ||  // Ｚ回転するため端で反転
						GetRotateVector(m_MoveMap[i].m_pColMesh->m_mWorld).z > m_MoveMap[i].m_vMax.z)
					{
						m_MoveMap[i].m_vUp.z *= -1;  // Ｚ回転するため端で反転
						m_MoveMap[i].m_nChangeFlag = 1;
					}
				}
			}
			else if (m_MoveMap[i].m_nMoveFlag == 3) {   // 拡大縮小の処理      // -- 2019.12.30
				MATRIX4X4 mScaleUp;
				mScaleUp = XMMatrixScaling(m_MoveMap[i].m_vUp.x, m_MoveMap[i].m_vUp.y, m_MoveMap[i].m_vUp.z);
				m_MoveMap[i].m_pColMesh->SetWorldMatrix(mScaleUp * m_MoveMap[i].m_pColMesh->m_mWorld);  // 拡大縮小増分の処理
				VECTOR3 vScaleNow = GetScaleVector(m_MoveMap[i].m_pColMesh->m_mWorld);     // 現在の拡大率を得る
				if (vScaleNow.x < m_MoveMap[i].m_vMin.x ||      // 規定の拡大縮小に達したか
					vScaleNow.x > m_MoveMap[i].m_vMax.x ||
					vScaleNow.y < m_MoveMap[i].m_vMin.y ||
					vScaleNow.y > m_MoveMap[i].m_vMax.y ||
					vScaleNow.z < m_MoveMap[i].m_vMin.z ||
					vScaleNow.z > m_MoveMap[i].m_vMax.z)
				{
					m_MoveMap[i].m_vUp.x = 1 / avoidZero(m_MoveMap[i].m_vUp.x);  // 拡大縮小の反転
					m_MoveMap[i].m_vUp.y = 1 / avoidZero(m_MoveMap[i].m_vUp.y);  // 拡大縮小の反転
					m_MoveMap[i].m_vUp.z = 1 / avoidZero(m_MoveMap[i].m_vUp.z);  // 拡大縮小の反転
					m_MoveMap[i].m_nChangeFlag = 1;
				}
			}
			// ループをしないときの停止判断。反転のタイミングで停止する             // -- 2021.2.4
			if (m_MoveMap[i].m_nLoop == 0 && m_MoveMap[i].m_nChangeFlag == 1)
			{
				m_MoveMap[i].m_nMoveFlag = 0;    // 移動しない
			}
		}
	}
}
//============================================================================
//  マッププロシージャのレンダリング
// 
//    引数　　なし
// 
//    戻り値　なし
//============================================================================
void  CMapProc::Render(void)
{
	if (m_bActive) {

		// 空のレンダリング
		if (m_SkyMap.size() > 0)
		{
			// 空のレンダリング   陰影をつけないレンダリング 
			// Zバッファを無効化
			m_pGMain->m_pD3D->SetZBuffer(FALSE);
			m_SkyMap[0].m_pMesh->Render(m_SkyMap[0].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);  // 陰影をつけないレンダリング 
			// Zバッファを有効化
			m_pGMain->m_pD3D->SetZBuffer(TRUE);
		}

		// マップレンダリング
		for (DWORD i = 0; i < m_StageMap.size(); i++)
		{
			if (m_StageMap[i].m_nMaterialFlag == 0)  // 通常のブレンドステート
			{
				if (m_StageMap[i].m_pMesh) m_StageMap[i].m_pMesh->Render(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
			else if (m_StageMap[i].m_nMaterialFlag == 2)  // ディスプレイスメントマッピング
			{
				// ディスプレイスメントマップレンダリング
				if (m_StageMap[i].m_pMesh) 	m_StageMap[i].m_pMesh->RenderDisplace(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
		}

		// 移動マップレンダリング     * このプログラムでは使用していない
		for (DWORD i = 0; i < m_MoveMap.size(); i++)
		{
			if (m_MoveMap[i].m_nMaterialFlag == 0)  // 通常のブレンドステート
			{
				if (m_MoveMap[i].m_pMesh && m_MoveMap[i].m_bActive)	m_MoveMap[i].m_pMesh->Render(m_MoveMap[i].m_pColMesh->m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
			else if (m_MoveMap[i].m_nMaterialFlag == 2)  // ディスプレイスメントマッピング
			{
				if (m_MoveMap[i].m_pMesh && m_MoveMap[i].m_bActive)	m_MoveMap[i].m_pMesh->RenderDisplace(m_MoveMap[i].m_pColMesh->m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
		}

	}
}

//============================================================================
//  マッププロシージャのレンダリング その２
// 
//  半透明のマップオブジェクトを描画するため、主ループの最後に実行される
// 
//    引数　　なし
// 
//    戻り値　なし
//============================================================================
void  CMapProc::Render2(void)
{
	if (m_bActive) {

		// マップレンダリング
		for (DWORD i = 0; i < m_StageMap.size(); i++)
		{
			if (m_StageMap[i].m_nMaterialFlag == 1)  // 透明色のブレンドステート
			{
				// 透明色のレンダリング
				if (m_StageMap[i].m_pMesh) m_StageMap[i].m_pMesh->Render(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, VECTOR3(0, 0, 0), m_pGMain->m_vEyePt);
			}
		}

		// 移動マップレンダリング     * このプログラムでは使用していない
		for (DWORD i = 0; i < m_MoveMap.size(); i++)
		{
			if (m_MoveMap[i].m_nMaterialFlag == 1)  // 透明色のブレンドステート
			{
				if (m_MoveMap[i].m_pMesh && m_MoveMap[i].m_bActive)	m_MoveMap[i].m_pMesh->Render(m_MoveMap[i].m_pColMesh->m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
		}

		// 波のレンダリング
		for (DWORD i = 0; i < m_StageMap.size(); i++)
		{
			if (m_StageMap[i].m_nMaterialFlag == 3)
			{
				// 波レンダリング
				if (m_StageMap[i].m_pWave)
					m_StageMap[i].m_pWave->Render(m_StageMap[i].m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
			}
		}

	}
}

