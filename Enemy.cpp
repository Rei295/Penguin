//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//
//		敵キャラクターの処理
//																Enemy.cpp
//=============================================================================
#include  "Enemy.h"
#include  "Playchar.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "BackFore.h"


//============================================================================
//
// 敵メインプロシージャのコンストラクタ
//
//   なお、プロシージャのdeleteはCBaseProcのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CEnmProc::CEnmProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pGMain = pGMain;
	m_pObjArray.push_back(new CEnmObj(this));	// m_pObjArrayにオブジェクトを登録する

	const int nWait = 500;                      // 敵が発生するまでの待ち時間
	m_dwProcID = ENM_RS_ID;                     // 敵ＲＳのID
	m_nNum = m_nMaxNum = ENM_RS_ALLMAX;         // 敵ＲＳの最大出現数

	// Fbxスキンメッシュの設定
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Enemy/Enm_Pengin.mesh"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy/Enm_Pengin_Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy/Enm_Pengin_Walk.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy/Enm_Pengin_Jump.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy/Enm_Pengin_Jump.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 8;

	for (int i = 0; i < ENM_RS_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmObj(this));  // m_pObjArrayにオブジェクトを生成する
	}

	m_nWaitTime = m_nMaxwaitTime = nWait;
}
//============================================================================
//
// 全ての敵プロシージャの発生数を初期値にリセットする
// （最大発生数(m_nMaxNum)を発生数m_nNumに設定する
//
// 引数    CBaseProc* pProc  対象とするプロシージャ。省略値はNULL
//                           NULLの時は自プロシージャ
// 戻り値  なし
// ---------------------------------------------------------------------------
void CEnmProc::ResetEnmNum(CBaseProc* pProc)
{
	if (pProc)   // 対象とするプロシージャ
	{
		pProc->SetMaxNum();  // 最大発生数(m_nMaxNum)を発生数m_nNumに設定する

		// 下位のプロシージャを探索していく
		for (DWORD i = 0; i < pProc->GetProcArrayPtr().size(); i++)
		{
			ResetEnmNum(pProc->GetProcArrayPtr()[i]);  // 再帰呼び出し
		}
	}
	else {   // 自プロシージャ
		SetMaxNum();  // 最大発生数(m_nMaxNum)を発生数m_nNumに設定する

		// 下位のプロシージャを探索していく
		for (DWORD i = 0; i < m_pProcArray.size(); i++)
		{
			ResetEnmNum(m_pProcArray[i]);  // 再帰呼び出し
		}
	}
}

//============================================================================
//
// 全ての敵オブジェクトの残っている敵の数を数える処理
// （残っている敵の数とは、まだ発生していない数(m_nNumの値)＋現在生きている数のことである）
//
// 引数    CBaseProc* pProc  対象とするプロシージャ。NULLの時は自プロシージャ。省略値はNULL
//         int        nNum   上位のプロシージャから与えられた敵の数。省略値は0
//
// 戻り値  int 残っている敵の数
// ---------------------------------------------------------------------------
int CEnmProc::CountActiveEnm(CBaseProc* pProc, int nNum)
{

	if (pProc)   // 対象とするプロシージャ
	{
		nNum += pProc->GetNum();									// プロシージャ中に残っている敵の数（まだ発生していない数）
		for (DWORD j = 0; j < pProc->GetObjArrayPtr().size(); j++)
		{
			if (pProc->GetObjArrayPtr()[j]->GetActive()) nNum++;	// オブジェクト中にすでに発生している敵の数（現在生きている数）
		}

		// 下位のプロシージャを探索していく
		for (DWORD i = 0; i < pProc->GetProcArrayPtr().size(); i++)
		{
			nNum = CountActiveEnm(pProc->GetProcArrayPtr()[i], nNum);  // 再帰呼び出し
		}
	}
	else {   // 自プロシージャ
		nNum += GetNum();											// プロシージャ中に残っている敵の数（まだ発生していない数）
		for (DWORD j = 0; j < m_pObjArray.size(); j++)
		{
			if (m_pObjArray[j]->GetActive()) nNum++;				// オブジェクト中にすでに発生している敵の数（現在生きている数）
		}

		// 下位のプロシージャを探索していく
		for (DWORD i = 0; i < m_pProcArray.size(); i++)
		{
			nNum = CountActiveEnm(m_pProcArray[i], nNum);  // 再帰呼び出し
		}
	}

	return nNum;
}

//============================================================================
//
// 敵ＲＳオブジェクトのコンストラクタ
//
//  引数
//  CBaseProc* pProc;		// 親のプロシージャクラスへのポインタ
//
// ---------------------------------------------------------------------------
CEnmObj::CEnmObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nMaxHP = 800;      // 体力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_vTarget = GetPositionVector(m_mWorld);

	// バウンディングボックスの設定
	VECTOR3 vMin(-1.0f, -0.05f, -1.0f);
	VECTOR3 vMax(1.0f, 2.7f, 1.5f);
	m_pBBox = new CBBox(m_pGMain->m_pShader, vMin, vMax);
	m_pBBox->m_mWorld = m_mWorld;                            // -- 2021.1.11

	m_nHp = m_nMaxHp = nMaxHP;

	m_AnimStatus.SetNum(0);
	m_AnimStatus.animFrame = 0;

}
// ---------------------------------------------------------------------------
//
// 敵ＲＳオブジェクトのデストラクタ
//
//   なお、CBBoxのdeleteはCBaseObjのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CEnmObj::~CEnmObj()
{
	;
}

// ---------------------------------------------------------------------------
//
// 敵ＲＳプロシージャの自動発生処理
//
//
// ---------------------------------------------------------------------------
void CEnmProc::Update()
{
	std::vector<DWORD> Nav;   // 選択ナビマップ

	// まだ発生できる数の場合のみ発生させる
	if ( m_nNum > 0 && m_pGMain->m_pMapProc->GetNavMap().size() > 0)
	{
		// ナビマップの敵ＩＤが一致するものを選択ナビマップNavに登録する
		for (int i = 0; i < m_pGMain->m_pMapProc->GetNavMap().size(); i++)
		{
			for (int j = 0; j < m_pGMain->m_pMapProc->GetNavMap()[i].m_dwEnmID.size(); j++)
			{
				if ( m_pGMain->m_pMapProc->GetNavMap()[i].m_dwEnmID[j] == m_dwProcID)
				{
					Nav.push_back(i);
				}
			}
		}
		if (Nav.size() > 0)  // 選択ナビマップNavに登録されているとき
		{	
			if (m_nWaitTime <= 0)
			{
				// 選択ナビマップNavの中から発生するナビマップをランダムに選ぶ
				int k = Random(0, Nav.size() -1);
				int no = Nav[k];
				for (DWORD i = 0; i < m_pObjArray.size(); i++)
				{
					if (!m_pObjArray[i]->GetActive() )
					{
						((CEnmObj*)m_pObjArray[i])->SetNav( m_pGMain->m_pMapProc->GetNavMap()[no]);  // 選ばれたナビマップをオブジェクトに登録する
						m_pObjArray[i]->Start();	// 敵を発生させる
						m_nNum--;			// 敵が発生したら発生できる敵の数を１減らす
						SetMaxWaitTime();
						break;
					}
				}
			}
		}
		else {
			m_nNum = 0;			// ナビマップNavが選択されていない場合、発生数は０となる
		}
	}

	//  (注意)m_nWaitTimeのカウントダウンは、CBaseProcで行っている

}
// ---------------------------------------------------------------------------
//
// 敵ＲＳオブジェクトの発生
//
// ---------------------------------------------------------------------------
BOOL CEnmObj::Start()
{
	m_bActive = TRUE;	// 敵ＲＳオブジェクト

	// ナビゲーションマップに従って発生させる
	m_mWorld = XMMatrixTranslation((float)Random(m_Nav.vMin.x, m_Nav.vMax.x),
						m_Nav.vMax.y, (float)Random(m_Nav.vMin.z, m_Nav.vMax.z));
	m_mWorldOld = m_mWorld;
	m_vTarget = VECTOR3((float)Random(m_Nav.vMin.x, m_Nav.vMax.x), m_Nav.vMax.y,
						(float)Random(m_Nav.vMin.z, m_Nav.vMax.z));
	m_pBBox->m_mWorld = m_mWorld;                            // -- 2021.1.11

	m_nHp = m_nMaxHp;

	return TRUE;
}

//-----------------------------------------------------------------------------
// 敵ＲＳオブジェクトの制御
//
//　キーボードで敵を動かす
//　敵のアニメーション、ジャンプ、自然落下
//　ワールド座標変換、マップとの接触判定
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmObj::Update()
{
	if (!m_bActive) return;  // 非表示のときは処理をしない

	// 変数の初期化   ---------------------------------------
	m_vPosUp.x = m_vPosUp.y = m_vPosUp.z = 0.0f;
	m_vRotUp.x = m_vRotUp.y = m_vRotUp.z = 0.0f;
	m_mWorldOld = m_mWorld;  // 一つ前の位置

	// ステータスごとの処理  --------------------------------
	switch (m_dwStatus)
	{
	case  FLASH: // 敵がダメージから復帰したときの処理。breakをつけずに次のNORMALを行う。
	case  NORMAL: // 敵が通常状態のときの処理
		UpdateNormal();
		break;

	case  DAMAGE: // 敵がダメージ状態のときの処理
		UpdateDamage();
		break;

	case  DEAD:	// 敵が死亡状態のときの処理
		UpdateDead();
		break;
	}

	// 他の敵と重ならないようにする処理  --------------------
	SetKeepOffset(m_pGMain->m_pEnmProc);

	// マップコリジョンと自然落下  --------------------------
	m_fJumpY += (0.5f*GRAVITY*0.00005f)*m_fJumpTime;	// 自然落下
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x) != 3)  // マップコリジョン
	{
		m_fJumpY = 0.0f;  // 自然落下の停止
		m_fJumpTime = 0.0f;
	}

	// 描画処理  ---------------------------------------------
	// スキンメッシュレンダリング 
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);        // -- 2022.2.16
	//GetMesh()->RenderDisplace(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	// バウンディングボックス
	m_pBBox->m_mWorld = m_mWorld;
	//   バウンディングボックスレンダリング
	//m_pBBox->Render(m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	// 手に持つ武器やアイテムの表示をする     // -- 2021.2.4
	if (GetHoldItem().m_nHoldObjNo != 0) // 手に持つ武器やアイテムがあるとき        // -- 2022.2.16
	{
		MATRIX4X4 mHold;
		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // 手の位置のボーンマトリックス
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->Disp(mHold, GetHoldItem()); // アイテム表示
	}

	// 敵の頭上のステータス表示  -----------------------------
	VECTOR3 vEnmPos = GetPositionVector(m_mWorld);
	vEnmPos.y += 3.0f;
	float fHp = (float)m_nHp / m_nMaxHp;
	// ＨＰバーの表示
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pBackForeProc->GetImage(), vEnmPos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, VECTOR2(1.2f*fHp, 0.2f), VECTOR2(320, 96), VECTOR2(125 * fHp, 16));
	// 敵の名前表示
	vEnmPos.y += 0.5f;
	m_pGMain->m_pFont->Draw3D(vEnmPos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, _T("ロボットＲＳ"), VECTOR2(1.2f, 0.5f), RGB(255, 0, 0), 1.0f, _T("HGP創英角ｺﾞｼｯｸUB"));

}

//-----------------------------------------------------------------------------
// 敵ＲＳオブジェクトの制御
//
// 敵が通常状態のときの処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateNormal()
{

	CPcObj* pPcObj = m_pGMain->m_pPcProc->GetPcObjPtr();
	VECTOR3 vPc    = GetPositionVector(pPcObj->GetWorld());
	VECTOR3 vEnm   = GetPositionVector(m_mWorld);
	MATRIX4X4 mTemp;
	float fLen;
	const float fNormalSpeed = 0.02f, fAttackSpeed = 0.03f;             // 移動スピード(通常、攻撃時)
	const float fNormalDistLimit = 20.0f, fAttackDistLimit = 4.0f;      // 近接限界値（通常接近、攻撃接近）
	const float fRotateSpeed = 3.0f;                                    // 回転スピード
	float fForward;


	// 敵がダメージから復帰したときの処理  ------------------------------
	if (m_dwStatus == FLASH)
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
	}

	// 攻撃ステータスごとの処理  -----------------------------------------
	switch (m_dwStatusSub) 
	{
	case ATTACKWAIT:		// ウエイトモードの時  ------------

		if (m_AnimStatus.animNum != 3) m_AnimStatus.SetNum(3);		// ウエイトのモーションになっていなかったら切り替える 3番

		fLen = magnitude(vPc - vEnm);	// ＰＣとの距離を測る
		m_nCnt1--;

		// ウエイトのタイムアウトかＰＣが近づいてきたときは、通常モードにする
		if (m_nCnt1 <= 0 || (fLen <= fNormalDistLimit && pPcObj->GetStatus() == NORMAL))
		{
			m_dwStatusSub = ATTACKNONE;	// 通常モードに
			m_dwStatusPhase = 0;
			m_nCnt1 = 0;
		}
		break;

	case ATTACKNONE:		// 攻撃モードでないとき  -------------

		fLen = magnitude(vPc - vEnm);	// ＰＣとの距離を測る

		if (m_AnimStatus.animNum != 0) m_AnimStatus.SetNum(0);		// 通常のモーションになっていなかったら切り替える 0番

		// ---------------------------------------------------
		// １　ＰＣと離れている（通常接近限界値超）ので、うろうろする
		// ---------------------------------------------------
		if (fLen > fNormalDistLimit || pPcObj->GetStatus() != NORMAL)
		{
			if (GetMesh()->m_RootAnimType[m_AnimStatus.animNum] == eRootAnimNone)    // ルートボーンアニメなしか   // -- 2021.1.11
			{
				fForward = fNormalSpeed;
			}else{
				fForward = GetPositionVector( GetMesh()->GetRootAnimUpMatrices(m_AnimStatus)).z;  // ルートボーンアニメのとき
			}
			if (!TargetMove(m_vTarget, fForward))	// 目的地（Target）に達したかどうか
			{
				mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// 目的地（Target）へ向かう角度
				m_mWorld = mTemp * m_mWorld;
				mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// 目的地（Target）へ向かう
				m_mWorld = mTemp * m_mWorld;
			}
			else {
				if (Random(0, 4) == 1)   // ウエイトモードか通常移動モードにするかを判定する(確率１／５)
				{
					do {
						m_vTarget.x = (float)Random(m_Nav.vMin.x, m_Nav.vMax.x);	// 次の目的地（Target）をセットする
						m_vTarget.z = (float)Random(m_Nav.vMin.z, m_Nav.vMax.z);
					} while (magnitude(m_vTarget - vEnm) < 10.0f);  // 目的地が現在地から１０ｍ以内のときはやり直し
				}
				else {
					m_nCnt1 = 400;				// ウエイトタイム 80フレームの倍数
					m_dwStatusSub = ATTACKWAIT;	// ウエイトモードに
					m_AnimStatus.SetNum(3);		// ウエイトのモーションに切り替わる 3番
				}
			}
		}
		
	}
}

//-----------------------------------------------------------------------------
// 敵ＲＳオブジェクトの制御
//
// 敵がダメージ状態のときの処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateDamage()
{
	MATRIX4X4 mTemp;
	const int nFlashTime = 5;  // 無敵状態の時間

	mTemp = XMMatrixTranslation(0, 0, -0.5f);	// バックする
	m_mWorld = mTemp * m_mWorld;

	m_nHp -= m_pHitObj->GetAtc();	// 攻撃を受けたダメージ
	if (m_nHp <= 0)
	{
		m_nHp = 0;
		m_dwStatus = DEAD;		// HPが０なので死亡へ
		m_AnimStatus.SetNum(2);		// 死亡モーション
	}
	else {
		m_nCnt1 = nFlashTime;   // 無敵状態の時間
		m_dwStatus = FLASH;     // ダメージからの復帰処理を行う
	}

}
//-----------------------------------------------------------------------------
// 敵ＲＳオブジェクトの制御
//
// 敵が死亡状態のときの処理
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateDead()
{
	if (m_AnimStatus.isEnd(2)) { // 死亡モーションが終わったとき
		m_bActive = FALSE;          // 敵を消す
		m_AnimStatus.SetNum(0);	// 死亡モーションが終わったら元に戻す
		m_nHp = m_nMaxHp;
		m_dwStatus = NORMAL;
		m_dwStatusSub = ATTACKNONE;
		m_dwStatusPhase = 0;
		m_pProc->SetMaxWaitTime();
		m_pGMain->m_nScore += m_nMaxHp / 10;   // MaxHPの1/10をスコアとして加算
	}
}

