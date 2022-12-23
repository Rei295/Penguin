//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//
//		プレイキャラクターの制御
//																Playchar.cpp
//=============================================================================
#include  "Playchar.h"
#include  "Camera.h"
#include  "BackFore.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "Enemy.h"
#include  "Fellow.h"

//============================================================================
//
// ＰＣプロシージャのコンストラクタ
//
//   なお、オブジェクトのdeleteはCBaseProcのデストラクタで行うため不要
//
// ---------------------------------------------------------------------------
CPcProc::CPcProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	//IDの設定
	m_dwProcID = PC_ID;
	m_dwPcProcID = PC_ID;

	// Fbxスキンメッシュの設定
	// ペンギン　－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
#if 1
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Pengin/Pengin.mesh"));   
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_walk.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_Jump.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 64;
#endif


	// -------------------------------------------------------------------------
	m_pObjArray.push_back(new CPcObj(this));	// m_pObjArrayにオブジェクトを登録する

	// -------------------------------------------------------------------------  // -- 2022.2.16
	// ＰＣプロシージャの初期化処理
	const int nNum = 3;    // PCの最大出現数
	m_nNum = nNum;

}

// ---------------------------------------------------------------------------
//
// ＰＣプロシージャのデストラクタ
//
// ---------------------------------------------------------------------------
CPcProc::~CPcProc()
{
	;
}


//============================================================================
//
// ＰＣオブジェクトのコンストラクタ
//
//  引数
//  CBaseProc* pProc;		// 親プロシージャへのポインタ
//
// ---------------------------------------------------------------------------
CPcObj::CPcObj(CBaseProc* pProc) : CBaseObj( pProc )
{
	const int nHP = 1000;    // 体力
	
	const int m_nCnt1 = 60;//カウント

	m_fLocalRotY = 0.0f;                   // -- 2018.8.4
	m_bSide = TRUE;                        // -- 2018.8.4

	m_fSpeed = 0.4f;      // 移動速度      // -- 2019.3.5
	m_fRotSpeed = 8.0f;   // 回転速度      // -- 2019.3.5
	m_fJumpSpeed = 0.1f;  // ジャンプ速度  // -- 2019.3.5

	// バウンディングボックスの設定
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.6f, -0.08f, -0.6f), VECTOR3(0.6f, 0.8f, 0.6f));
	m_pBBox->m_mWorld = m_mWorld;                            // -- 2021.1.11

	m_nMaxHp = nHP;

	// -----------------------------------------------------------------------  // -- 2022.2.16
	// ＰＣオブジェクトの初期化処理
	m_bActive = TRUE;
	ResetStatus();

	m_AnimStatus.playAnim = true;
	m_AnimStatus.SetNum(0);
	m_AnimStatus.animFrame = 0;

	m_nHp = m_nMaxHp;

}
// ---------------------------------------------------------------------------
//
// ＰＣオブジェクトのデストラクタ
//
// ---------------------------------------------------------------------------
CPcObj::~CPcObj() 
{
	;
}

// ---------------------------------------------------------------------------
//
// ＰＣオブジェクトのローカル軸マトリックスを得る関数
//
//  m_mWorldの位置情報とm_fLocalRotYの角度情報を元に
//  ローカル軸マトリックスを返す
//
// ---------------------------------------------------------------------------
MATRIX4X4 CPcObj::GetLocalMatrix()
{
	MATRIX4X4 mPos = GetPositionMatrix(m_mWorld);
	MATRIX4X4 mRotY;

	mRotY = XMMatrixRotationY(XMConvertToRadians(m_fLocalRotY));
	mRotY = mRotY * mPos;
	return mRotY;
}
//-----------------------------------------------------------------------------
// ＰＣオブジェクトの更新
//
//　キーボードでＰＣを動かす
//　ＰＣのアニメーション、ジャンプ、自然落下
//　ワールド座標返還、マップとの接触判定
//　ＰＣの攻撃処理を行う
//
//   引数　　　なし
//-----------------------------------------------------------------------------
void	CPcObj::Update()
{
	// 一つ前の位置を保存
	m_mWorldOld = m_mWorld;

	// 各状態に応じた処理
	switch( m_dwStatus ){
	case  FLASH:  // ＰＣがダメージから復帰したときの処理
	case  NORMAL: // ＰＣが通常状態のときの処理
				UpdateNormal();  // ＰＣオブジェクトの更新 通常状態（NORMAL）
				break;

		case  DAMAGE: // ＰＣがダメージ状態のときの処理
				UpdateDamage();  // ＰＣオブジェクトの更新 ダメージ状態（DAMAGE）
				break;

		case  DEAD:	// ＰＣが死亡状態のときの処理
				UpdateDead();  // ＰＣオブジェクトの更新 死亡状態（DEAD）
				break;

	}

	// マップコリジョンと自然落下

	m_fJumpY += (0.5f*GRAVITY*0.00005f)*m_fJumpTime;	// 自然落下
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld) != 3)  // マップコリジョン
	{
		m_fJumpY = 0.0f;  // 自然落下の停止
		m_fJumpTime = 0.0f;
	}

	

	//レンダリング
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);        // -- 2022.2.16
	//GetMesh()->RenderDisplace(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	// バウンディングボックス
	m_pBBox->m_mWorld = m_mWorld;
	//m_pBBox->m_mWorld = m_pMesh->GetFrameMatrices(m_AnimStatus, m_mWorld, 0);  // ルートボーン
	//m_pBBox->Render( m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);


	// ＰＣの頭上の名前表示
	VECTOR3 vPcPos = GetPositionVector(m_mWorld);
	vPcPos.y += 1.01f;
	// 王冠の表示
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pBackForeProc->GetImage(), vPcPos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt,VECTOR2(0.5f,0.5f) ,VECTOR2(385, 0) ,VECTOR2(32,32));

}

//-----------------------------------------------------------------------------   // -- 2019.3.5
// ＰＣオブジェクトの更新 通常状態（NORMAL）
//
//　キーボードでＰＣを動かす
//　ＰＣのアニメーション、ジャンプ、自然落下
//　ワールド座標返還、マップとの接触判定
//　ＰＣの攻撃処理を行う
//
//
//   引数　なし
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormal()
{

	if (m_dwStatus == FLASH) // ＰＣがダメージから復帰したときの処理
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
		//m_pGMain->m_pRenderBufProc->SetDrawFont(300, 10, _T("** 無敵状態 **"), 16, RGB(255, 0, 0));
	}
	UpdateNormalMove();      // ＰＣオブジェクトの移動処理

	// 全ての敵のバウンディングボックスとのあたり判定
	if (m_dwStatus != FLASH) m_pGMain->m_pEnmProc->Hitcheck(this, m_pBBox);   // -- 2019.6.8
	if (m_dwStatus != FLASH) m_pGMain->m_pFellowProc->Hitcheck(this, m_pBBox);   // -- 2019.6.8

}
//-----------------------------------------------------------------------------   // -- 2019.3.5
// ＰＣオブジェクトの移動処理
//
//　キーボード／マウスでＰＣを動かす
//　ＰＣのアニメーション、ジャンプ、自然落下
//　ワールド座標返還、マップとの接触判定
//
//
//   引数　なし
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormalMove()
{
	CDirectInput* pDI = m_pGMain->m_pDI;

	MATRIX4X4 mYaw;  // ＰＣＹ軸回転マトリックス
	MATRIX4X4 mPos;  // ＰＣ移動マトリックス
	BOOL	  bKeyPush = FALSE;// キーを押したかどうかのチェック
	const float fRotLowSpeed = m_fRotSpeed / 8;   // ローカル軸（Ｙ軸）の回転速度

	// キーボード、マウス、ジョイスティック操作

	// ジャンプ   ------------------------------------------------
	if ((pDI->CheckKey(KD_TRG, DIK_J) || pDI->CheckJoy(KD_TRG, DIJ_Z)) && m_fJumpY == 0.0f) {
		bKeyPush = TRUE;
		m_fJumpY = m_fJumpSpeed;
		m_fJumpTime = 1.0f;
		m_AnimStatus.SetNum(2);
	}
	// 自然落下の初期値
	mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f);

#if 0
	// ローカル軸（Ｙ軸）の回転処理  --------------------------------
	if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckMouse(KD_DAT, DIM_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_R)) //	→キー
	{
		m_fLocalRotY += fRotLowSpeed;
		if (m_fLocalRotY >= 360.0f) m_fLocalRotY -= 360.0f;
	}

	if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckMouse(KD_DAT, DIM_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_L)) //	←キー
	{
		m_fLocalRotY -= fRotLowSpeed;
		if (m_fLocalRotY < 0.0f) m_fLocalRotY += 360.0f;
	}
#endif

	// ＰＣの移動処理  ---------------------------------------------
	m_AnimStatus.SetNum(1);
	mPos = UpdateNormalMoveKeystate(DIK_W);

	// 後退処理
	if (pDI->CheckKey(KD_DAT, DIK_S) || pDI->CheckKey(KD_DAT, DIK_DOWN) || pDI->CheckJoy(KD_DAT, DIJ_DOWN)) //↓キー
	{
		bKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_S);
	}
	// 右移動処理
	if (pDI->CheckKey(KD_DAT, DIK_D) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))//→キー
	{
		bKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_W);
		mPos = UpdateNormalMoveKeystate(DIK_D);
	}
	// 左移動処理
	if (pDI->CheckKey(KD_DAT, DIK_A) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))//←キー
	{
		bKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_W);
		mPos = UpdateNormalMoveKeystate(DIK_A);
	}

	if (!bKeyPush && m_AnimStatus.animNum != 3)  // キーを押していないとき
	{
		m_AnimStatus.SetNum(0);
	}

	// ローカル軸を基準として、ＰＣを移動させるワールドマトリックスを設定
	mYaw = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));
	m_mWorld = mPos * mYaw * GetLocalMatrix();

}

//-----------------------------------------------------------------------------   // -- 2021.1.10
// ＰＣオブジェクトの移動処理　キー操作毎の各種移動処理
//
//   引数　DWORD Key キー種別　DirectInputのキーコード
//
//   戻り値  移動マトリックス
//-----------------------------------------------------------------------------
MATRIX4X4 CPcObj::UpdateNormalMoveKeystate(DWORD DIKey)
{
	MATRIX4X4 mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f);

	if (m_AnimStatus.animNum == 0) m_AnimStatus.SetNum(1);
	if (m_AnimStatus.isEnd(1))     m_AnimStatus.SetNum(1);

	if (m_bSide && DIKey != DIK_W)   // サイドモードで前進以外のとき
	{
		switch (DIKey)
		{
		case DIK_S:    // 後退
			// 方向を変えずに後退
			mPos = XMMatrixTranslation(0.0f, m_fJumpY, -m_fSpeed);
			break;
		case DIK_D:    // 右
			// 方向を変えずに右移動
			mPos = XMMatrixTranslation(m_fSpeed * 0.6f, m_fJumpY, 0.0f);
			break;
		case DIK_A:    // 左
			// 方向を変えずに左移動
			mPos = XMMatrixTranslation(-m_fSpeed * 0.6f, m_fJumpY, 0.0f);
			break;
		}
	}
	else {
		// 進行方向に回転してから前進

		// ルートボーンアニメーションを行うかどうかルートアニメーションタイプを確認する
		if (GetMesh()->GetRootAnimType(m_AnimStatus.animNum) == eRootAnimNone)
		{
			// ルートボーンアニメーションを行わず固定の前進移動値
			mPos = XMMatrixTranslation(0.0f, m_fJumpY, m_fSpeed);
		}
		else {
			// ルートボーンアニメーションでの前進移動値
			mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f) * GetMesh()->GetRootAnimUpMatrices(m_AnimStatus);
		}

		// 進行方向に回転する処理
		// ・回転角度はm_vRotUp.yにある。-180～180度
		// ・１回の回転速度はm_fRotSpeed
		if (m_vRotUp.y > 180.0f)  m_vRotUp.y -= 360.0f;
		if (m_vRotUp.y < -180.0f) m_vRotUp.y += 360.0f;

		float fAngle = 0.0f;  // 目標回転角度
		switch (DIKey)
		{
		case DIK_W:    // 前進
			fAngle = 0.0f;
			break;
		case DIK_S:    // 後退
			if (m_vRotUp.y >= 0)  // 最小回転になるように方向を合わせる
			{
				fAngle = 180.0f;
			}
			else {
				fAngle = -180.0f;
			}
			break;
		case DIK_D:    // 右
			fAngle = 90.0f;
			if (m_vRotUp.y == -180.0f) m_vRotUp.y = 180.0f;  // 最小回転になるように方向を合わせる
			break;
		case DIK_A:    // 左
			fAngle = -90.0f;
			if (m_vRotUp.y == 180.0f) m_vRotUp.y = -180.0f;  // 最小回転になるように方向を合わせる
			break;
		}

		if (m_vRotUp.y > fAngle)  // 左回転
		{
			m_vRotUp.y -= m_fRotSpeed;
			if (m_vRotUp.y < fAngle) m_vRotUp.y = fAngle;
		}
		if (m_vRotUp.y < fAngle)  // 右回転
		{
			m_vRotUp.y += m_fRotSpeed;
			if (m_vRotUp.y > fAngle) m_vRotUp.y = fAngle;
		}

	}
	return mPos;
}

//-----------------------------------------------------------------------------   // -- 2019.3.5
// ＰＣオブジェクトの更新 ダメージ状態（DAMAGE）
//
//　ダメージを受けたときの処理
//
//
//   引数　なし
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDamage()
{
	MATRIX4X4 mTemp;
	const int nDeadTime = 200;    // 死亡時間
	const int nFlashTime = 5;      // 無敵状態の時間
	CEnmObj* pEnmObj = m_pGMain->m_pEnmProc->GetEnmObjPtr();
	CFellowObj* pFellowObj = m_pGMain->m_pFellowProc->GetFellowObjPtr();
	VECTOR3 vEnm = GetPositionVector(pEnmObj->GetWorld());
	VECTOR3 vFellow = GetPositionVector(pFellowObj->GetWorld());

	//m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 255, 255), 1.0f); // 画面を一瞬白くフラッシュ

	mTemp = XMMatrixTranslation(0, 0, -0.25f);	// バックする
	m_mWorld = mTemp * m_mWorld;

	m_nHp -= m_pHitObj->GetAtc();	// 攻撃を受けたダメージ
	if (m_nHp <= 0)		// HPが０なので死亡へ
	{
		m_nHp = 0;
		m_dwStatus = DEAD;		// HPが０なので死亡へ
		m_AnimStatus.SetNum(2);	// 死亡モーションにする
		m_nCnt1 = nDeadTime;			// 死亡時間の設定
	}
	else {
		m_nCnt1 = nFlashTime;   // 無敵状態の時間
		m_dwStatus = FLASH;     // ダメージからの復帰処理を行う
	}


		//スコア加算
		m_pGMain->m_nScore = m_pGMain->m_nScore + 1;
}
//-----------------------------------------------------------------------------   // -- 2019.3.5
// ＰＣオブジェクトの更新 死亡状態（DEAD）
//
//　死亡状態のときの処理
//
//
//   引数　なし
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDead()
{
	const int nFlashTime = 400;      // 無敵状態の時間

	m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 0, 0), 0.5f); // 画面を赤くする

	if (m_AnimStatus.isEnd(4))  // 死亡モーションが終わったとき
	{
		m_AnimStatus.playAnim = false;	// アニメーションを止める
		m_AnimStatus.animFrame = m_AnimStatus.endFrame - 1; // フレームエンドの一つ手前にする
	}
	if (--m_nCnt1 <= 0) // 死亡時間が終わったとき
	{
		m_pProc->AddNum(-1);	// ＰＣを一つ減らす
		if (m_pProc->GetNum() <= 0)
		{
			m_pGMain->m_dwGameStatus = GAMEOVER;	// ゲームオーバーへ
		}
		else {
			m_AnimStatus.SetNum(0);	// 死亡時間が終わったので元に戻す
			m_AnimStatus.playAnim = true;
			m_nHp = m_nMaxHp;
			m_dwStatus = FLASH;     // 死亡からの復帰処理
			m_nCnt1 = nFlashTime;   // 無敵状態の時間
		}
	}
}
