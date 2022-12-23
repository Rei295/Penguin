//仲間

#include  "Fellow.h"
#include  "Enemy.h"
#include  "Playchar.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "BackFore.h"

using namespace std;         //  名前空間指定


//プロシージャのコンストラクタ
CFellowProc::CFellowProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pGMain = pGMain;

	const int nWait = 0;                   // 敵が発生するまでの待ち時間
	m_dwProcID = FELLOW_ID;                // 仲間のID
	m_nNum = m_nMaxNum = FELLOW_ALL;       // 仲間の最大出現数

	//Fbxスキンメッシュ
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Pengin/Pengin.mesh"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_walk.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_Jump.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 64;

	for (int i = 0; i < FELLOW_MAX; i++)
	{
		m_pObjArray.push_back(new CFellowObj(this));  // m_pObjArrayにオブジェクトを生成する
	}

	m_nWaitTime = m_nMaxwaitTime = nWait;
}

//プロシージャの発生数を初期値にリセット
void CFellowProc::ResetFellowNum(CBaseProc* pProc)
{
	if (pProc)   // 対象とするプロシージャ
	{
		pProc->SetMaxNum();  // 最大発生数(m_nMaxNum)を発生数m_nNumに設定する

		// 下位のプロシージャを探索していく
		for (DWORD i = 0; i < pProc->GetProcArrayPtr().size(); i++)
		{
			ResetFellowNum(pProc->GetProcArrayPtr()[i]);  // 再帰呼び出し
		}
	}
	else {   // 自プロシージャ
		SetMaxNum();  // 最大発生数(m_nMaxNum)を発生数m_nNumに設定する

		// 下位のプロシージャを探索していく
		for (DWORD i = 0; i < m_pProcArray.size(); i++)
		{
			ResetFellowNum(m_pProcArray[i]);  // 再帰呼び出し
		}
	}
}

int CFellowProc::CountActiveFellow(CBaseProc* pProc, int nNum)
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
			nNum = CountActiveFellow(pProc->GetProcArrayPtr()[i], nNum);  // 再帰呼び出し
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
			nNum = CountActiveFellow(m_pProcArray[i], nNum);  // 再帰呼び出し
		}
	}

	return nNum;
}


//プロシージャの更新
void CFellowProc::Update()
{
	std::vector<DWORD> Nav;   // 選択ナビマップ

	// まだ発生できる数の場合のみ発生させる
	if (m_nNum > 0 && m_pGMain->m_pMapProc->GetNavMap().size() > 0)
	{
		// ナビマップの敵ＩＤが一致するものを選択ナビマップNavに登録する
		for (int i = 0; i < m_pGMain->m_pMapProc->GetNavMap().size(); i++)
		{
			for (int j = 0; j < m_pGMain->m_pMapProc->GetNavMap()[i].m_dwFellowID.size(); j++)
			{
				if (m_pGMain->m_pMapProc->GetNavMap()[i].m_dwFellowID[j] == m_dwProcID)
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
				int k = Random(0, Nav.size() - 1);
				int no = Nav[k];
				for (DWORD i = 0; i < m_pObjArray.size(); i++)
				{
					if (!m_pObjArray[i]->GetActive())
					{
						((CFellowObj*)m_pObjArray[i])->SetNav(m_pGMain->m_pMapProc->GetNavMap()[no]);  // 選ばれたナビマップをオブジェクトに登録する
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
}

//プロシージャのデストラクタ
CFellowProc::~CFellowProc()
{
	;
}

//オブジェクトのコンストラクタ
CFellowObj::CFellowObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nMaxHP = 1200;     // 体力

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_vTarget = GetPositionVector(m_mWorld);

	// オブジェクトのバウンディングボックスの設定
	VECTOR3 vMin(-0.6f, -0.08f, -0.6f);
	VECTOR3 vMax(0.6f, 0.8f, 0.6f);
	m_pBBox = new CBBox(m_pGMain->m_pShader, vMin, vMax);
	m_pBBox->m_mWorld = m_mWorld;                            // -- 2021.1.11

	m_nHp = m_nMaxHp = nMaxHP;

	m_AnimStatus.SetNum(0);
	m_AnimStatus.animFrame = 0;
}

//オブジェクトのデストラクタ
CFellowObj::~CFellowObj()
{
	;
}

//オブジェクトスタート
BOOL CFellowObj::Start()
{
	m_bActive = TRUE;	

	// ナビゲーションマップに従って発生させる
	m_mWorld = XMMatrixTranslation((float)Random(m_Nav.vMin.x, m_Nav.vMax.x),
		m_Nav.vMax.y, (float)Random(m_Nav.vMin.z, m_Nav.vMax.z));
	m_mWorldOld = m_mWorld;
	m_vTarget = VECTOR3((float)Random(m_Nav.vMin.x, m_Nav.vMax.x), m_Nav.vMax.y,
		(float)Random(m_Nav.vMin.z, m_Nav.vMax.z));
	m_pBBox->m_mWorld = m_mWorld;                            // -- 2021.1.11

	return TRUE;
}

//オブジェクトの更新
void CFellowObj::Update()
{
	if (!m_bActive) return;  // 非表示のときは処理をしない

	// 変数の初期化  ---------------------------------------
	m_vPosUp.x = m_vPosUp.y = m_vPosUp.z = 0.0f;
	m_vRotUp.x = m_vRotUp.y = m_vRotUp.z = 0.0f;
	m_mWorldOld = m_mWorld;  // 一つ前の位置

	// 各ステータスごとの処理 ------------------------------
	switch (m_dwStatus)
	{
	case  FLASH:  // 仲間がダメージから復帰したときの処理。breakをつけずに次のNORMALを行う。
	case  NORMAL: // 仲間が通常状態のときの処理
		UpdateNormal();
		break;
	case  DAMAGE: // 仲間がダメージ状態のときの処理
		UpdateDamage();
		break;
	}

	// 他の敵と重ならないようにする処理  -------------------
	SetKeepOffset(m_pGMain->m_pFellowProc);
	SetKeepOffset(m_pGMain->m_pEnmProc);

	
	// マップコリジョンと自然落下  -------------------------
	m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// 自然落下
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x) != 3)  // マップコリジョン
	{
		m_fJumpY = 0.0f;  // 自然落下の停止
		m_fJumpTime = 0.0f;
	}
	
	

	// 描画処理  ---------------------------------------------
	// スキンメッシュレンダリング
	//GetMesh()->RenderDisplace(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);        // -- 2022.2.16

	// バウンディングボックス
	m_pBBox->m_mWorld = m_mWorld;
	//   バウンディングボックスレンダリング
	//m_pBBox->Render( m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

}

void	CFellowObj::UpdateNormal()
{
	CPcObj* pPcObj = m_pGMain->m_pPcProc->GetPcObjPtr();
	VECTOR3 vPc = GetPositionVector(pPcObj->GetWorld());
	VECTOR3 vFellow = GetPositionVector(m_mWorld);
	MATRIX4X4 mTemp;
	float fLen;
	const float fNormalSpeed = 0.05f, fAttackSpeed = 0.07f;             // 移動スピード(通常、攻撃時)
	const float fNormalDistLimit = 10.0f, fAttackDistLimit = 1.0f;      // 近接限界値（通常接近、攻撃接近）

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
	case ATTACKWAIT:		// ウエイトモードの時  ----------------

		if (m_AnimStatus.animNum != 3) m_AnimStatus.SetNum(0);		// ウエイトのモーションになっていなかったら切り替える 3番

		fLen = magnitude(vPc - vFellow);	// ＰＣとの距離を測る
		m_nCnt1--;

		// ウエイトのタイムアウトかＰＣが近づいてきたときは、通常モードにする
		if (m_nCnt1 <= 0 || (fLen <= fNormalDistLimit && pPcObj->GetStatus() == NORMAL))
		{
			m_dwStatusSub = ATTACKNONE;	// 通常モードに
			m_dwStatusPhase = 0;
			m_nCnt1 = 0;
		}
		break;

	case ATTACKNONE:		// 攻撃モードでないとき  --------------

		fLen = magnitude(vPc - vFellow);	// ＰＣとの距離を測る

		if (m_AnimStatus.animNum != 0) m_AnimStatus.SetNum(0);		// 通常のモーションになっていなかったら切り替える 0番

		
		// ---------------------------------------------------
		// １　ＰＣと離れている（通常接近限界値超）ので、うろうろする
		// ---------------------------------------------------
		if (fLen > fNormalDistLimit || pPcObj->GetStatus() != NORMAL)
		{
			if (GetMesh()->m_RootAnimType[m_AnimStatus.animNum] == eRootAnimNone)    // ルートボーンアニメなしか   // -- 2021.1.11
			{
				fForward = fNormalSpeed;
			}
			else {
				fForward = GetPositionVector(GetMesh()->GetRootAnimUpMatrices(m_AnimStatus)).z;  // ルートボーンアニメのとき
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
					} while (magnitude(m_vTarget - vFellow) < 10.0f);  // 目的地が現在地から１０ｍ以内のときはやり直し
				}
				else {
					m_nCnt1 = 480;				// ウエイトタイム 120フレームの倍数
					m_dwStatusSub = ATTACKWAIT;	// ウエイトモードに
					m_AnimStatus.SetNum(0);		// ウエイトのモーションに切り替わる 3番
				}
			}
		}
		break;
	}
}

//ダメ―ジ
void	CFellowObj::UpdateDamage()
{
	CPcObj* pPcObj = m_pGMain->m_pPcProc->GetPcObjPtr();
	CEnmObj* pEnmObj = m_pGMain->m_pEnmProc->GetEnmObjPtr();
	VECTOR3 vPc = GetPositionVector(pPcObj->GetWorld());
	VECTOR3 vEnm = GetPositionVector(pEnmObj->GetWorld());
	VECTOR3 vFellow = GetPositionVector(m_mWorld);
	MATRIX4X4 mTemp;
	float fLen;
	float fChange;
	const float fNormalSpeed = 0.05f, fAttackSpeed = 0.17f;             // 移動スピード(通常、攻撃時)
	const float fNormalDistLimit = 3.0f;                                // 一定の距離
	const float fNormal2DistLimit = 5.0f;                               // 一定の距離
	const float fRotateSpeed = 1.0f;                                    // 回転スピード
	float fForward;

	fLen = magnitude(vPc - vFellow);                                    // PCとの距離
	fChange = magnitude(vFellow - vFellow);                             // 仲間同士の距離

#if 1
		//PCに当たったためついていく
		vPc.y = vFellow.y;
		m_vRotUp.y = GetTargetRotateVector(m_mWorld, vPc).y;	// ＰＣの方向を向かせるY軸角度を求める
		if (m_vRotUp.y >= 180)  m_vRotUp.y -= 360;
		if (m_vRotUp.y <= -180) m_vRotUp.y += 360;
		if (m_vRotUp.y > fRotateSpeed)  m_vRotUp.y = fRotateSpeed;	// fRotateSpeed度以上の方向変換は行わない
		if (m_vRotUp.y < -fRotateSpeed) m_vRotUp.y = -fRotateSpeed;	// fRotateSpeed度以上の方向変換は行わない
		mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));
		m_mWorld = mTemp * m_mWorld;
		SetKeepOffset(m_pGMain->m_pFellowProc);
		if (fLen > 0 && fLen < fNormalDistLimit) {
			mTemp = XMMatrixTranslation(0.0f, m_fJumpY, fAttackSpeed);	// 目的地（Target）へ向かう
			m_mWorld = mTemp * m_mWorld;
		}
		else if(fLen >= fNormalDistLimit){
			mTemp = XMMatrixTranslation(0.0f, m_fJumpY, fAttackSpeed * 2);	// 目的地（Target）へ向かう
			m_mWorld = mTemp * m_mWorld;
		}
		else if (fLen >= fNormal2DistLimit) {
			mTemp = XMMatrixTranslation(0.0f, m_fJumpY, fAttackSpeed * 3);	// 目的地（Target）へ向かう
			m_mWorld = mTemp * m_mWorld;
		}
		

		//if (GetMesh()->m_RootAnimType[m_AnimStatus.animNum] == eRootAnimNone)    // ルートボーンアニメなしか   // -- 2021.1.11
		//{
		//	mTemp = XMMatrixTranslation(0.0f, m_fJumpY, fAttackSpeed);	// 目的地（Target）へ向かう
		//}
		//else {
		//	mTemp = XMMatrixTranslation(0.0f, m_fJumpY, fAttackSpeed) * GetMesh()->GetRootAnimUpMatrices(m_AnimStatus);  // ルートボーンアニメのとき
		//}
		//m_mWorld = mTemp * m_mWorld;
#endif
}