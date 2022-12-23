//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//
//		�v���C�L�����N�^�[�̐���
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
// �o�b�v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
// ---------------------------------------------------------------------------
CPcProc::CPcProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	//ID�̐ݒ�
	m_dwProcID = PC_ID;
	m_dwPcProcID = PC_ID;

	// Fbx�X�L�����b�V���̐ݒ�
	// �y���M���@�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
#if 1
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Pengin/Pengin.mesh"));   
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_walk.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_Jump.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 64;
#endif


	// -------------------------------------------------------------------------
	m_pObjArray.push_back(new CPcObj(this));	// m_pObjArray�ɃI�u�W�F�N�g��o�^����

	// -------------------------------------------------------------------------  // -- 2022.2.16
	// �o�b�v���V�[�W���̏���������
	const int nNum = 3;    // PC�̍ő�o����
	m_nNum = nNum;

}

// ---------------------------------------------------------------------------
//
// �o�b�v���V�[�W���̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CPcProc::~CPcProc()
{
	;
}


//============================================================================
//
// �o�b�I�u�W�F�N�g�̃R���X�g���N�^
//
//  ����
//  CBaseProc* pProc;		// �e�v���V�[�W���ւ̃|�C���^
//
// ---------------------------------------------------------------------------
CPcObj::CPcObj(CBaseProc* pProc) : CBaseObj( pProc )
{
	const int nHP = 1000;    // �̗�
	
	const int m_nCnt1 = 60;//�J�E���g

	m_fLocalRotY = 0.0f;                   // -- 2018.8.4
	m_bSide = TRUE;                        // -- 2018.8.4

	m_fSpeed = 0.4f;      // �ړ����x      // -- 2019.3.5
	m_fRotSpeed = 8.0f;   // ��]���x      // -- 2019.3.5
	m_fJumpSpeed = 0.1f;  // �W�����v���x  // -- 2019.3.5

	// �o�E���f�B���O�{�b�N�X�̐ݒ�
	m_pBBox = new CBBox(m_pGMain->m_pShader, VECTOR3(-0.6f, -0.08f, -0.6f), VECTOR3(0.6f, 0.8f, 0.6f));
	m_pBBox->m_mWorld = m_mWorld;                            // -- 2021.1.11

	m_nMaxHp = nHP;

	// -----------------------------------------------------------------------  // -- 2022.2.16
	// �o�b�I�u�W�F�N�g�̏���������
	m_bActive = TRUE;
	ResetStatus();

	m_AnimStatus.playAnim = true;
	m_AnimStatus.SetNum(0);
	m_AnimStatus.animFrame = 0;

	m_nHp = m_nMaxHp;

}
// ---------------------------------------------------------------------------
//
// �o�b�I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CPcObj::~CPcObj() 
{
	;
}

// ---------------------------------------------------------------------------
//
// �o�b�I�u�W�F�N�g�̃��[�J�����}�g���b�N�X�𓾂�֐�
//
//  m_mWorld�̈ʒu����m_fLocalRotY�̊p�x��������
//  ���[�J�����}�g���b�N�X��Ԃ�
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
// �o�b�I�u�W�F�N�g�̍X�V
//
//�@�L�[�{�[�h�ło�b�𓮂���
//�@�o�b�̃A�j���[�V�����A�W�����v�A���R����
//�@���[���h���W�ԊҁA�}�b�v�Ƃ̐ڐG����
//�@�o�b�̍U���������s��
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CPcObj::Update()
{
	// ��O�̈ʒu��ۑ�
	m_mWorldOld = m_mWorld;

	// �e��Ԃɉ���������
	switch( m_dwStatus ){
	case  FLASH:  // �o�b���_���[�W���畜�A�����Ƃ��̏���
	case  NORMAL: // �o�b���ʏ��Ԃ̂Ƃ��̏���
				UpdateNormal();  // �o�b�I�u�W�F�N�g�̍X�V �ʏ��ԁiNORMAL�j
				break;

		case  DAMAGE: // �o�b���_���[�W��Ԃ̂Ƃ��̏���
				UpdateDamage();  // �o�b�I�u�W�F�N�g�̍X�V �_���[�W��ԁiDAMAGE�j
				break;

		case  DEAD:	// �o�b�����S��Ԃ̂Ƃ��̏���
				UpdateDead();  // �o�b�I�u�W�F�N�g�̍X�V ���S��ԁiDEAD�j
				break;

	}

	// �}�b�v�R���W�����Ǝ��R����

	m_fJumpY += (0.5f*GRAVITY*0.00005f)*m_fJumpTime;	// ���R����
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld) != 3)  // �}�b�v�R���W����
	{
		m_fJumpY = 0.0f;  // ���R�����̒�~
		m_fJumpTime = 0.0f;
	}

	

	//�����_�����O
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);        // -- 2022.2.16
	//GetMesh()->RenderDisplace(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	// �o�E���f�B���O�{�b�N�X
	m_pBBox->m_mWorld = m_mWorld;
	//m_pBBox->m_mWorld = m_pMesh->GetFrameMatrices(m_AnimStatus, m_mWorld, 0);  // ���[�g�{�[��
	//m_pBBox->Render( m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);


	// �o�b�̓���̖��O�\��
	VECTOR3 vPcPos = GetPositionVector(m_mWorld);
	vPcPos.y += 1.01f;
	// �����̕\��
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pBackForeProc->GetImage(), vPcPos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt,VECTOR2(0.5f,0.5f) ,VECTOR2(385, 0) ,VECTOR2(32,32));

}

//-----------------------------------------------------------------------------   // -- 2019.3.5
// �o�b�I�u�W�F�N�g�̍X�V �ʏ��ԁiNORMAL�j
//
//�@�L�[�{�[�h�ło�b�𓮂���
//�@�o�b�̃A�j���[�V�����A�W�����v�A���R����
//�@���[���h���W�ԊҁA�}�b�v�Ƃ̐ڐG����
//�@�o�b�̍U���������s��
//
//
//   �����@�Ȃ�
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormal()
{

	if (m_dwStatus == FLASH) // �o�b���_���[�W���畜�A�����Ƃ��̏���
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
		//m_pGMain->m_pRenderBufProc->SetDrawFont(300, 10, _T("** ���G��� **"), 16, RGB(255, 0, 0));
	}
	UpdateNormalMove();      // �o�b�I�u�W�F�N�g�̈ړ�����

	// �S�Ă̓G�̃o�E���f�B���O�{�b�N�X�Ƃ̂����蔻��
	if (m_dwStatus != FLASH) m_pGMain->m_pEnmProc->Hitcheck(this, m_pBBox);   // -- 2019.6.8
	if (m_dwStatus != FLASH) m_pGMain->m_pFellowProc->Hitcheck(this, m_pBBox);   // -- 2019.6.8

}
//-----------------------------------------------------------------------------   // -- 2019.3.5
// �o�b�I�u�W�F�N�g�̈ړ�����
//
//�@�L�[�{�[�h�^�}�E�X�ło�b�𓮂���
//�@�o�b�̃A�j���[�V�����A�W�����v�A���R����
//�@���[���h���W�ԊҁA�}�b�v�Ƃ̐ڐG����
//
//
//   �����@�Ȃ�
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateNormalMove()
{
	CDirectInput* pDI = m_pGMain->m_pDI;

	MATRIX4X4 mYaw;  // �o�b�x����]�}�g���b�N�X
	MATRIX4X4 mPos;  // �o�b�ړ��}�g���b�N�X
	BOOL	  bKeyPush = FALSE;// �L�[�����������ǂ����̃`�F�b�N
	const float fRotLowSpeed = m_fRotSpeed / 8;   // ���[�J�����i�x���j�̉�]���x

	// �L�[�{�[�h�A�}�E�X�A�W���C�X�e�B�b�N����

	// �W�����v   ------------------------------------------------
	if ((pDI->CheckKey(KD_TRG, DIK_J) || pDI->CheckJoy(KD_TRG, DIJ_Z)) && m_fJumpY == 0.0f) {
		bKeyPush = TRUE;
		m_fJumpY = m_fJumpSpeed;
		m_fJumpTime = 1.0f;
		m_AnimStatus.SetNum(2);
	}
	// ���R�����̏����l
	mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f);

#if 0
	// ���[�J�����i�x���j�̉�]����  --------------------------------
	if (pDI->CheckKey(KD_DAT, DIK_RIGHT) || pDI->CheckMouse(KD_DAT, DIM_RIGHT) || pDI->CheckJoy(KD_DAT, DIJ_R)) //	���L�[
	{
		m_fLocalRotY += fRotLowSpeed;
		if (m_fLocalRotY >= 360.0f) m_fLocalRotY -= 360.0f;
	}

	if (pDI->CheckKey(KD_DAT, DIK_LEFT) || pDI->CheckMouse(KD_DAT, DIM_LEFT) || pDI->CheckJoy(KD_DAT, DIJ_L)) //	���L�[
	{
		m_fLocalRotY -= fRotLowSpeed;
		if (m_fLocalRotY < 0.0f) m_fLocalRotY += 360.0f;
	}
#endif

	// �o�b�̈ړ�����  ---------------------------------------------
	m_AnimStatus.SetNum(1);
	mPos = UpdateNormalMoveKeystate(DIK_W);

	// ��ޏ���
	if (pDI->CheckKey(KD_DAT, DIK_S) || pDI->CheckKey(KD_DAT, DIK_DOWN) || pDI->CheckJoy(KD_DAT, DIJ_DOWN)) //���L�[
	{
		bKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_S);
	}
	// �E�ړ�����
	if (pDI->CheckKey(KD_DAT, DIK_D) || pDI->CheckJoy(KD_DAT, DIJ_RIGHT))//���L�[
	{
		bKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_W);
		mPos = UpdateNormalMoveKeystate(DIK_D);
	}
	// ���ړ�����
	if (pDI->CheckKey(KD_DAT, DIK_A) || pDI->CheckJoy(KD_DAT, DIJ_LEFT))//���L�[
	{
		bKeyPush = TRUE;
		mPos = UpdateNormalMoveKeystate(DIK_W);
		mPos = UpdateNormalMoveKeystate(DIK_A);
	}

	if (!bKeyPush && m_AnimStatus.animNum != 3)  // �L�[�������Ă��Ȃ��Ƃ�
	{
		m_AnimStatus.SetNum(0);
	}

	// ���[�J��������Ƃ��āA�o�b���ړ������郏�[���h�}�g���b�N�X��ݒ�
	mYaw = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));
	m_mWorld = mPos * mYaw * GetLocalMatrix();

}

//-----------------------------------------------------------------------------   // -- 2021.1.10
// �o�b�I�u�W�F�N�g�̈ړ������@�L�[���얈�̊e��ړ�����
//
//   �����@DWORD Key �L�[��ʁ@DirectInput�̃L�[�R�[�h
//
//   �߂�l  �ړ��}�g���b�N�X
//-----------------------------------------------------------------------------
MATRIX4X4 CPcObj::UpdateNormalMoveKeystate(DWORD DIKey)
{
	MATRIX4X4 mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f);

	if (m_AnimStatus.animNum == 0) m_AnimStatus.SetNum(1);
	if (m_AnimStatus.isEnd(1))     m_AnimStatus.SetNum(1);

	if (m_bSide && DIKey != DIK_W)   // �T�C�h���[�h�őO�i�ȊO�̂Ƃ�
	{
		switch (DIKey)
		{
		case DIK_S:    // ���
			// ������ς����Ɍ��
			mPos = XMMatrixTranslation(0.0f, m_fJumpY, -m_fSpeed);
			break;
		case DIK_D:    // �E
			// ������ς����ɉE�ړ�
			mPos = XMMatrixTranslation(m_fSpeed * 0.6f, m_fJumpY, 0.0f);
			break;
		case DIK_A:    // ��
			// ������ς����ɍ��ړ�
			mPos = XMMatrixTranslation(-m_fSpeed * 0.6f, m_fJumpY, 0.0f);
			break;
		}
	}
	else {
		// �i�s�����ɉ�]���Ă���O�i

		// ���[�g�{�[���A�j���[�V�������s�����ǂ������[�g�A�j���[�V�����^�C�v���m�F����
		if (GetMesh()->GetRootAnimType(m_AnimStatus.animNum) == eRootAnimNone)
		{
			// ���[�g�{�[���A�j���[�V�������s�킸�Œ�̑O�i�ړ��l
			mPos = XMMatrixTranslation(0.0f, m_fJumpY, m_fSpeed);
		}
		else {
			// ���[�g�{�[���A�j���[�V�����ł̑O�i�ړ��l
			mPos = XMMatrixTranslation(0.0f, m_fJumpY, 0.0f) * GetMesh()->GetRootAnimUpMatrices(m_AnimStatus);
		}

		// �i�s�����ɉ�]���鏈��
		// �E��]�p�x��m_vRotUp.y�ɂ���B-180�`180�x
		// �E�P��̉�]���x��m_fRotSpeed
		if (m_vRotUp.y > 180.0f)  m_vRotUp.y -= 360.0f;
		if (m_vRotUp.y < -180.0f) m_vRotUp.y += 360.0f;

		float fAngle = 0.0f;  // �ڕW��]�p�x
		switch (DIKey)
		{
		case DIK_W:    // �O�i
			fAngle = 0.0f;
			break;
		case DIK_S:    // ���
			if (m_vRotUp.y >= 0)  // �ŏ���]�ɂȂ�悤�ɕ��������킹��
			{
				fAngle = 180.0f;
			}
			else {
				fAngle = -180.0f;
			}
			break;
		case DIK_D:    // �E
			fAngle = 90.0f;
			if (m_vRotUp.y == -180.0f) m_vRotUp.y = 180.0f;  // �ŏ���]�ɂȂ�悤�ɕ��������킹��
			break;
		case DIK_A:    // ��
			fAngle = -90.0f;
			if (m_vRotUp.y == 180.0f) m_vRotUp.y = -180.0f;  // �ŏ���]�ɂȂ�悤�ɕ��������킹��
			break;
		}

		if (m_vRotUp.y > fAngle)  // ����]
		{
			m_vRotUp.y -= m_fRotSpeed;
			if (m_vRotUp.y < fAngle) m_vRotUp.y = fAngle;
		}
		if (m_vRotUp.y < fAngle)  // �E��]
		{
			m_vRotUp.y += m_fRotSpeed;
			if (m_vRotUp.y > fAngle) m_vRotUp.y = fAngle;
		}

	}
	return mPos;
}

//-----------------------------------------------------------------------------   // -- 2019.3.5
// �o�b�I�u�W�F�N�g�̍X�V �_���[�W��ԁiDAMAGE�j
//
//�@�_���[�W���󂯂��Ƃ��̏���
//
//
//   �����@�Ȃ�
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDamage()
{
	MATRIX4X4 mTemp;
	const int nDeadTime = 200;    // ���S����
	const int nFlashTime = 5;      // ���G��Ԃ̎���
	CEnmObj* pEnmObj = m_pGMain->m_pEnmProc->GetEnmObjPtr();
	CFellowObj* pFellowObj = m_pGMain->m_pFellowProc->GetFellowObjPtr();
	VECTOR3 vEnm = GetPositionVector(pEnmObj->GetWorld());
	VECTOR3 vFellow = GetPositionVector(pFellowObj->GetWorld());

	//m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 255, 255), 1.0f); // ��ʂ���u�����t���b�V��

	mTemp = XMMatrixTranslation(0, 0, -0.25f);	// �o�b�N����
	m_mWorld = mTemp * m_mWorld;

	m_nHp -= m_pHitObj->GetAtc();	// �U�����󂯂��_���[�W
	if (m_nHp <= 0)		// HP���O�Ȃ̂Ŏ��S��
	{
		m_nHp = 0;
		m_dwStatus = DEAD;		// HP���O�Ȃ̂Ŏ��S��
		m_AnimStatus.SetNum(2);	// ���S���[�V�����ɂ���
		m_nCnt1 = nDeadTime;			// ���S���Ԃ̐ݒ�
	}
	else {
		m_nCnt1 = nFlashTime;   // ���G��Ԃ̎���
		m_dwStatus = FLASH;     // �_���[�W����̕��A�������s��
	}


		//�X�R�A���Z
		m_pGMain->m_nScore = m_pGMain->m_nScore + 1;
}
//-----------------------------------------------------------------------------   // -- 2019.3.5
// �o�b�I�u�W�F�N�g�̍X�V ���S��ԁiDEAD�j
//
//�@���S��Ԃ̂Ƃ��̏���
//
//
//   �����@�Ȃ�
//
//-----------------------------------------------------------------------------
void	CPcObj::UpdateDead()
{
	const int nFlashTime = 400;      // ���G��Ԃ̎���

	m_pGMain->m_pBackForeProc->GetSprite()->DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(255, 0, 0), 0.5f); // ��ʂ�Ԃ�����

	if (m_AnimStatus.isEnd(4))  // ���S���[�V�������I������Ƃ�
	{
		m_AnimStatus.playAnim = false;	// �A�j���[�V�������~�߂�
		m_AnimStatus.animFrame = m_AnimStatus.endFrame - 1; // �t���[���G���h�̈��O�ɂ���
	}
	if (--m_nCnt1 <= 0) // ���S���Ԃ��I������Ƃ�
	{
		m_pProc->AddNum(-1);	// �o�b������炷
		if (m_pProc->GetNum() <= 0)
		{
			m_pGMain->m_dwGameStatus = GAMEOVER;	// �Q�[���I�[�o�[��
		}
		else {
			m_AnimStatus.SetNum(0);	// ���S���Ԃ��I������̂Ō��ɖ߂�
			m_AnimStatus.playAnim = true;
			m_nHp = m_nMaxHp;
			m_dwStatus = FLASH;     // ���S����̕��A����
			m_nCnt1 = nFlashTime;   // ���G��Ԃ̎���
		}
	}
}
