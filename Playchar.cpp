//=============================================================================
//		RcQ[vO
//		bRc`PPQ                             ver 3.1        2022.2.16
//
//		vCLN^[Μ§δ
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
// obvV[WΜRXgN^
//
//   Θ¨AIuWFNgΜdeleteΝCBaseProcΜfXgN^Εs€½ίsv
//
// ---------------------------------------------------------------------------
CPcProc::CPcProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	//IDΜέθ
	m_dwProcID = PC_ID;
	m_dwPcProcID = PC_ID;

	// FbxXLbVΜέθ
	// yM@||||||||||||||||||||||||||||||
#if 1
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Pengin/Pengin.mesh"));   
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_walk.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_Jump.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 64;
#endif


	// -------------------------------------------------------------------------
	m_pObjArray.push_back(new CPcObj(this));	// m_pObjArrayΙIuWFNgπo^·ι

	// -------------------------------------------------------------------------  // -- 2022.2.16
	// obvV[WΜϊ»
	const int nNum = 3;    // PCΜΕεo»
	m_nNum = nNum;

}

// ---------------------------------------------------------------------------
//
// obvV[WΜfXgN^
//
// ---------------------------------------------------------------------------
CPcProc::~CPcProc()
{
	;
}


//============================================================================
//
// obIuWFNgΜRXgN^
//
//  ψ
//  CBaseProc* pProc;		// evV[WΦΜ|C^
//
// ---------------------------------------------------------------------------
CPcObj::CPcObj(CBaseProc* pProc) : CBaseObj( pProc )
{
	const int nHP = 1000;    // ΜΝ
	
	const int m_nCnt1 = 60;//JEg

	m_fLocalRotY = 0.0f;                   // -- 2018.8.4
	m_bSide = TRUE;                        // -- 2018.8.4

	m_fSpeed = 0.4f;      // Ϊ?¬x      // -- 2019.3.5
	m_fRotSpeed = 8.0f;   // ρ]¬x      // -- 2019.3.5
	m_fJumpSpeed = 0.1f;  // Wv¬x  // -- 2019.3.5

	// oEfBO{bNXΜέθ
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.6f, -0.08f, -0.6f), VECTOR3(0.6f, 0.8f, 0.6f));
	m_pBBox->m_mWorld = m_mWorld;                            // -- 2021.1.11

	m_nMaxHp = nHP;

	// -----------------------------------------------------------------------  // -- 2022.2.16
	// obIuWFNgΜϊ»
	m_bActive = TRUE;
	ResetStatus();

	m_AnimStatus.playAnim = true;
	m_AnimStatus.SetNum(0);
	m_AnimStatus.animFrame = 0;

	m_nHp = m_nMaxHp;

}
// ---------------------------------------------------------------------------
//
// obIuWFNgΜfXgN^
//
// ---------------------------------------------------------------------------
CPcObj::~CPcObj() 
{
	;
}

// ---------------------------------------------------------------------------
//
// obIuWFNgΜ[J²}gbNXπΎιΦ
//
//  m_mWorldΜΚuξρΖm_fLocalRotYΜpxξρπ³Ι
//  [J²}gbNXπΤ·
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
// obIuWFNgΜXV
//
//@L[{[hΕobπ?©·
//@obΜAj[VAWvA©RΊ
//@[hΐWΤ?A}bvΖΜΪG»θ
//@obΜUπs€
//
//   ψ@@@Θ΅
//-----------------------------------------------------------------------------
void	CPcObj::Update()
{
	// κΒOΜΚuπΫΆ
	m_mWorldOld = m_mWorld;

	// eσΤΙΆ½
	switch( m_dwStatus ){
	case  FLASH:  // obͺ_[W©ηA΅½Ζ«Μ
	case  NORMAL: // obͺΚνσΤΜΖ«Μ
				UpdateNormal();  // obIuWFNgΜXV ΚνσΤiNORMALj
				break;

		case  DAMAGE: // obͺ_[WσΤΜΖ«Μ
				UpdateDamage();  // obIuWFNgΜXV _[WσΤiDAMAGEj
				break;

		case  DEAD:	// obͺSσΤΜΖ«Μ
				UpdateDead();  // obIuWFNgΜXV SσΤiDEADj
				break;

	}

	// }bvRWΖ©RΊ

	m_fJumpY += (0.5f*GRAVITY*0.00005f)*m_fJumpTime;	// ©RΊ
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld) != 3)  // }bvRW
	{
		m_fJumpY = 0.0f;  // ©RΊΜβ~
		m_fJumpTime = 0.0f;
	}

	

	//_O
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);        // -- 2022.2.16
	//GetMesh()->RenderDisplace(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	// oEfBO{bNX
	m_pBBox->m_mWorld = m_mWorld;
	//m_pBBox->m_mWorld = m_pMesh->GetFrameMatrices(m_AnimStatus, m_mWorld, 0);  // [g{[
	//m_pBBox->Render( m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);


	// obΜͺγΜΌO\¦
	VECTOR3 vPcPos = GetPositionVector(m_mWorld);
	vPcPos.y += 1.01f;
	// €₯Μ\¦
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pBackForeProc->GetImage(), vPcPos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt,VECTOR2(0.5f,0.5f) ,VECTOR2(385, 0) ,VECTOR2(32,32));

}

//-----------------------------------------------------------------------------   // -- 2019.3.5
// obIuWFNgΜXV ΚνσΤiNORMALj
//
//@L[{[hΕobπ?©·
//@obΜAj[VAWvA©RΊ
//@[hΐWΤ?A}bvΖΜΪG»θ
//@obΜUπs€
//
//
//   ψ@Θ΅
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormal()
{

	if (m_dwStatus == FLASH) // obͺ_[W©ηA΅½Ζ«Μ
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
		//m_pGMain->m_pRenderBufProc->SetDrawFont(300, 10, _T("** ³GσΤ **"), 16, RGB(255, 0, 0));
	}
	UpdateNormalMove();      // obIuWFNgΜΪ?

	// SΔΜGΜoEfBO{bNXΖΜ ½θ»θ
	if (m_dwStatus != FLASH) m_pGMain->m_pEnmProc->Hitcheck(this, m_pBBox);   // -- 2019.6.8
	if (m_dwStatus != FLASH) m_pGMain->m_pFellowProc->Hitcheck(this, m_pBBox);   // -- 2019.6.8

}
//-----------------------------------------------------------------------------   // -- 2019.3.5
// obIuWFNgΜΪ?
//
//@L[{[h^}EXΕobπ?©·
//@obΜAj[VAWvA©RΊ
//@[hΐWΤ?A}bvΖΜΪG»θ
//
//
//   ψ@Θ΅
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormalMove()
{
	CDirectInput* pDI = m_pGMain->m_pDI;

	MATRIX4X4 mYaw;  // obx²ρ]}gbNX
	MATRIX4X4 mPos;  // obΪ?}gbNX
	BOOL	  bKeyPush = FALSE;// L[π΅½©Η€©Μ`FbN
	const float fRotLowSpeed = m_fRotSpeed / 8;   // [J²ix²jΜρ]¬x

	// L[{[hA}EXAWCXeBbNμ

	// Wv   ------------------------------------------------
	if ((pDI->CheckKey(KD_TRG, DIK_J) || pDI->CheckJoy(KD_TRG, DIJ_Z)) && m_fJumpY == 0.0f) {
		bKeyPush = TRUE;
		m_fJumpY = m_fJumpSpeed;
		m_fJumpTime = 1.0f;
		m_AnimStatus.SetNum(2);
	}
	// ©RΊΜϊl
	mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f);

#if 0
	// [J²ix²jΜρ]  --------------------------------
	if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckMouse(KD_DAT, DIM_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_R)) //	¨L[
	{
		m_fLocalRotY += fRotLowSpeed;
		if (m_fLocalRotY >= 360.0f) m_fLocalRotY -= 360.0f;
	}

	if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckMouse(KD_DAT, DIM_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_L)) //	©L[
	{
		m_fLocalRotY -= fRotLowSpeed;
		if (m_fLocalRotY < 0.0f) m_fLocalRotY += 360.0f;
	}
#endif

	// obΜΪ?  ---------------------------------------------
	m_AnimStatus.SetNum(1);
	mPos = UpdateNormalMoveKeystate(DIK_W);

	// γή
	if (pDI->CheckKey(KD_DAT, DIK_S) || pDI->CheckKey(KD_DAT, DIK_DOWN) || pDI->CheckJoy(KD_DAT, DIJ_DOWN)) //«L[
	{
		bKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_S);
	}
	// EΪ?
	if (pDI->CheckKey(KD_DAT, DIK_D) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))//¨L[
	{
		bKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_W);
		mPos = UpdateNormalMoveKeystate(DIK_D);
	}
	// ΆΪ?
	if (pDI->CheckKey(KD_DAT, DIK_A) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))//©L[
	{
		bKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_W);
		mPos = UpdateNormalMoveKeystate(DIK_A);
	}

	if (!bKeyPush && m_AnimStatus.animNum != 3)  // L[π΅Δ’Θ’Ζ«
	{
		m_AnimStatus.SetNum(0);
	}

	// [J²πξΖ΅ΔAobπΪ?³Ήι[h}gbNXπέθ
	mYaw = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));
	m_mWorld = mPos * mYaw * GetLocalMatrix();

}

//-----------------------------------------------------------------------------   // -- 2021.1.10
// obIuWFNgΜΪ?@L[μΜeνΪ?
//
//   ψ@DWORD Key L[νΚ@DirectInputΜL[R[h
//
//   ίθl  Ϊ?}gbNX
//-----------------------------------------------------------------------------
MATRIX4X4 CPcObj::UpdateNormalMoveKeystate(DWORD DIKey)
{
	MATRIX4X4 mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f);

	if (m_AnimStatus.animNum == 0) m_AnimStatus.SetNum(1);
	if (m_AnimStatus.isEnd(1))     m_AnimStatus.SetNum(1);

	if (m_bSide && DIKey != DIK_W)   // TCh[hΕOiΘOΜΖ«
	{
		switch (DIKey)
		{
		case DIK_S:    // γή
			// ϋόπΟ¦ΈΙγή
			mPos = XMMatrixTranslation(0.0f, m_fJumpY, -m_fSpeed);
			break;
		case DIK_D:    // E
			// ϋόπΟ¦ΈΙEΪ?
			mPos = XMMatrixTranslation(m_fSpeed * 0.6f, m_fJumpY, 0.0f);
			break;
		case DIK_A:    // Ά
			// ϋόπΟ¦ΈΙΆΪ?
			mPos = XMMatrixTranslation(-m_fSpeed * 0.6f, m_fJumpY, 0.0f);
			break;
		}
	}
	else {
		// isϋόΙρ]΅Δ©ηOi

		// [g{[Aj[Vπs€©Η€©[gAj[V^CvπmF·ι
		if (GetMesh()->GetRootAnimType(m_AnimStatus.animNum) == eRootAnimNone)
		{
			// [g{[Aj[VπsνΈΕθΜOiΪ?l
			mPos = XMMatrixTranslation(0.0f, m_fJumpY, m_fSpeed);
		}
		else {
			// [g{[Aj[VΕΜOiΪ?l
			mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f) * GetMesh()->GetRootAnimUpMatrices(m_AnimStatus);
		}

		// isϋόΙρ]·ι
		// Eρ]pxΝm_vRotUp.yΙ ιB-180`180x
		// EPρΜρ]¬xΝm_fRotSpeed
		if (m_vRotUp.y > 180.0f)  m_vRotUp.y -= 360.0f;
		if (m_vRotUp.y < -180.0f) m_vRotUp.y += 360.0f;

		float fAngle = 0.0f;  // ΪWρ]px
		switch (DIKey)
		{
		case DIK_W:    // Oi
			fAngle = 0.0f;
			break;
		case DIK_S:    // γή
			if (m_vRotUp.y >= 0)  // Ε¬ρ]ΙΘιζ€ΙϋόπνΉι
			{
				fAngle = 180.0f;
			}
			else {
				fAngle = -180.0f;
			}
			break;
		case DIK_D:    // E
			fAngle = 90.0f;
			if (m_vRotUp.y == -180.0f) m_vRotUp.y = 180.0f;  // Ε¬ρ]ΙΘιζ€ΙϋόπνΉι
			break;
		case DIK_A:    // Ά
			fAngle = -90.0f;
			if (m_vRotUp.y == 180.0f) m_vRotUp.y = -180.0f;  // Ε¬ρ]ΙΘιζ€ΙϋόπνΉι
			break;
		}

		if (m_vRotUp.y > fAngle)  // Άρ]
		{
			m_vRotUp.y -= m_fRotSpeed;
			if (m_vRotUp.y < fAngle) m_vRotUp.y = fAngle;
		}
		if (m_vRotUp.y < fAngle)  // Eρ]
		{
			m_vRotUp.y += m_fRotSpeed;
			if (m_vRotUp.y > fAngle) m_vRotUp.y = fAngle;
		}

	}
	return mPos;
}

//-----------------------------------------------------------------------------   // -- 2019.3.5
// obIuWFNgΜXV _[WσΤiDAMAGEj
//
//@_[Wπσ―½Ζ«Μ
//
//
//   ψ@Θ΅
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDamage()
{
	MATRIX4X4 mTemp;
	const int nDeadTime = 200;    // SΤ
	const int nFlashTime = 5;      // ³GσΤΜΤ
	CEnmObj* pEnmObj = m_pGMain->m_pEnmProc->GetEnmObjPtr();
	CFellowObj* pFellowObj = m_pGMain->m_pFellowProc->GetFellowObjPtr();
	VECTOR3 vEnm = GetPositionVector(pEnmObj->GetWorld());
	VECTOR3 vFellow = GetPositionVector(pFellowObj->GetWorld());

	//m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 255, 255), 1.0f); // ζΚπκu­tbV

	mTemp = XMMatrixTranslation(0, 0, -0.25f);	// obN·ι
	m_mWorld = mTemp * m_mWorld;

	m_nHp -= m_pHitObj->GetAtc();	// Uπσ―½_[W
	if (m_nHp <= 0)		// HPͺOΘΜΕSΦ
	{
		m_nHp = 0;
		m_dwStatus = DEAD;		// HPͺOΘΜΕSΦ
		m_AnimStatus.SetNum(2);	// S[VΙ·ι
		m_nCnt1 = nDeadTime;			// SΤΜέθ
	}
	else {
		m_nCnt1 = nFlashTime;   // ³GσΤΜΤ
		m_dwStatus = FLASH;     // _[W©ηΜAπs€
	}


		//XRAΑZ
		m_pGMain->m_nScore = m_pGMain->m_nScore + 1;
}
//-----------------------------------------------------------------------------   // -- 2019.3.5
// obIuWFNgΜXV SσΤiDEADj
//
//@SσΤΜΖ«Μ
//
//
//   ψ@Θ΅
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDead()
{
	const int nFlashTime = 400;      // ³GσΤΜΤ

	m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 0, 0), 0.5f); // ζΚπΤ­·ι

	if (m_AnimStatus.isEnd(4))  // S[VͺIνΑ½Ζ«
	{
		m_AnimStatus.playAnim = false;	// Aj[Vπ~ίι
		m_AnimStatus.animFrame = m_AnimStatus.endFrame - 1; // t[GhΜκΒθOΙ·ι
	}
	if (--m_nCnt1 <= 0) // SΤͺIνΑ½Ζ«
	{
		m_pProc->AddNum(-1);	// obπκΒΈη·
		if (m_pProc->GetNum() <= 0)
		{
			m_pGMain->m_dwGameStatus = GAMEOVER;	// Q[I[o[Φ
		}
		else {
			m_AnimStatus.SetNum(0);	// SΤͺIνΑ½ΜΕ³Ιί·
			m_AnimStatus.playAnim = true;
			m_nHp = m_nMaxHp;
			m_dwStatus = FLASH;     // S©ηΜA
			m_nCnt1 = nFlashTime;   // ³GσΤΜΤ
		}
	}
}
