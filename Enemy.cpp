//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//
//		�G�L�����N�^�[�̏���
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
// �G���C���v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�v���V�[�W����delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
// ---------------------------------------------------------------------------
CEnmProc::CEnmProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pGMain = pGMain;
	m_pObjArray.push_back(new CEnmObj(this));	// m_pObjArray�ɃI�u�W�F�N�g��o�^����

	const int nWait = 500;                      // �G����������܂ł̑҂�����
	m_dwProcID = ENM_RS_ID;                     // �G�q�r��ID
	m_nNum = m_nMaxNum = ENM_RS_ALLMAX;         // �G�q�r�̍ő�o����

	// Fbx�X�L�����b�V���̐ݒ�
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Enemy/Enm_Pengin.mesh"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy/Enm_Pengin_Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy/Enm_Pengin_Walk.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy/Enm_Pengin_Jump.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Enemy/Enm_Pengin_Jump.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 8;

	for (int i = 0; i < ENM_RS_MAX; i++)
	{
		m_pObjArray.push_back(new CEnmObj(this));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}

	m_nWaitTime = m_nMaxwaitTime = nWait;
}
//============================================================================
//
// �S�Ă̓G�v���V�[�W���̔������������l�Ƀ��Z�b�g����
// �i�ő唭����(m_nMaxNum)�𔭐���m_nNum�ɐݒ肷��
//
// ����    CBaseProc* pProc  �ΏۂƂ���v���V�[�W���B�ȗ��l��NULL
//                           NULL�̎��͎��v���V�[�W��
// �߂�l  �Ȃ�
// ---------------------------------------------------------------------------
void CEnmProc::ResetEnmNum(CBaseProc* pProc)
{
	if (pProc)   // �ΏۂƂ���v���V�[�W��
	{
		pProc->SetMaxNum();  // �ő唭����(m_nMaxNum)�𔭐���m_nNum�ɐݒ肷��

		// ���ʂ̃v���V�[�W����T�����Ă���
		for (DWORD i = 0; i < pProc->GetProcArrayPtr().size(); i++)
		{
			ResetEnmNum(pProc->GetProcArrayPtr()[i]);  // �ċA�Ăяo��
		}
	}
	else {   // ���v���V�[�W��
		SetMaxNum();  // �ő唭����(m_nMaxNum)�𔭐���m_nNum�ɐݒ肷��

		// ���ʂ̃v���V�[�W����T�����Ă���
		for (DWORD i = 0; i < m_pProcArray.size(); i++)
		{
			ResetEnmNum(m_pProcArray[i]);  // �ċA�Ăяo��
		}
	}
}

//============================================================================
//
// �S�Ă̓G�I�u�W�F�N�g�̎c���Ă���G�̐��𐔂��鏈��
// �i�c���Ă���G�̐��Ƃ́A�܂��������Ă��Ȃ���(m_nNum�̒l)�{���ݐ����Ă��鐔�̂��Ƃł���j
//
// ����    CBaseProc* pProc  �ΏۂƂ���v���V�[�W���BNULL�̎��͎��v���V�[�W���B�ȗ��l��NULL
//         int        nNum   ��ʂ̃v���V�[�W������^����ꂽ�G�̐��B�ȗ��l��0
//
// �߂�l  int �c���Ă���G�̐�
// ---------------------------------------------------------------------------
int CEnmProc::CountActiveEnm(CBaseProc* pProc, int nNum)
{

	if (pProc)   // �ΏۂƂ���v���V�[�W��
	{
		nNum += pProc->GetNum();									// �v���V�[�W�����Ɏc���Ă���G�̐��i�܂��������Ă��Ȃ����j
		for (DWORD j = 0; j < pProc->GetObjArrayPtr().size(); j++)
		{
			if (pProc->GetObjArrayPtr()[j]->GetActive()) nNum++;	// �I�u�W�F�N�g���ɂ��łɔ������Ă���G�̐��i���ݐ����Ă��鐔�j
		}

		// ���ʂ̃v���V�[�W����T�����Ă���
		for (DWORD i = 0; i < pProc->GetProcArrayPtr().size(); i++)
		{
			nNum = CountActiveEnm(pProc->GetProcArrayPtr()[i], nNum);  // �ċA�Ăяo��
		}
	}
	else {   // ���v���V�[�W��
		nNum += GetNum();											// �v���V�[�W�����Ɏc���Ă���G�̐��i�܂��������Ă��Ȃ����j
		for (DWORD j = 0; j < m_pObjArray.size(); j++)
		{
			if (m_pObjArray[j]->GetActive()) nNum++;				// �I�u�W�F�N�g���ɂ��łɔ������Ă���G�̐��i���ݐ����Ă��鐔�j
		}

		// ���ʂ̃v���V�[�W����T�����Ă���
		for (DWORD i = 0; i < m_pProcArray.size(); i++)
		{
			nNum = CountActiveEnm(m_pProcArray[i], nNum);  // �ċA�Ăяo��
		}
	}

	return nNum;
}

//============================================================================
//
// �G�q�r�I�u�W�F�N�g�̃R���X�g���N�^
//
//  ����
//  CBaseProc* pProc;		// �e�̃v���V�[�W���N���X�ւ̃|�C���^
//
// ---------------------------------------------------------------------------
CEnmObj::CEnmObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nMaxHP = 800;      // �̗�

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_vTarget = GetPositionVector(m_mWorld);

	// �o�E���f�B���O�{�b�N�X�̐ݒ�
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
// �G�q�r�I�u�W�F�N�g�̃f�X�g���N�^
//
//   �Ȃ��ACBBox��delete��CBaseObj�̃f�X�g���N�^�ōs�����ߕs�v
//
// ---------------------------------------------------------------------------
CEnmObj::~CEnmObj()
{
	;
}

// ---------------------------------------------------------------------------
//
// �G�q�r�v���V�[�W���̎�����������
//
//
// ---------------------------------------------------------------------------
void CEnmProc::Update()
{
	std::vector<DWORD> Nav;   // �I���i�r�}�b�v

	// �܂������ł��鐔�̏ꍇ�̂ݔ���������
	if ( m_nNum > 0 && m_pGMain->m_pMapProc->GetNavMap().size() > 0)
	{
		// �i�r�}�b�v�̓G�h�c����v������̂�I���i�r�}�b�vNav�ɓo�^����
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
		if (Nav.size() > 0)  // �I���i�r�}�b�vNav�ɓo�^����Ă���Ƃ�
		{	
			if (m_nWaitTime <= 0)
			{
				// �I���i�r�}�b�vNav�̒����甭������i�r�}�b�v�������_���ɑI��
				int k = Random(0, Nav.size() -1);
				int no = Nav[k];
				for (DWORD i = 0; i < m_pObjArray.size(); i++)
				{
					if (!m_pObjArray[i]->GetActive() )
					{
						((CEnmObj*)m_pObjArray[i])->SetNav( m_pGMain->m_pMapProc->GetNavMap()[no]);  // �I�΂ꂽ�i�r�}�b�v���I�u�W�F�N�g�ɓo�^����
						m_pObjArray[i]->Start();	// �G�𔭐�������
						m_nNum--;			// �G�����������甭���ł���G�̐����P���炷
						SetMaxWaitTime();
						break;
					}
				}
			}
		}
		else {
			m_nNum = 0;			// �i�r�}�b�vNav���I������Ă��Ȃ��ꍇ�A�������͂O�ƂȂ�
		}
	}

	//  (����)m_nWaitTime�̃J�E���g�_�E���́ACBaseProc�ōs���Ă���

}
// ---------------------------------------------------------------------------
//
// �G�q�r�I�u�W�F�N�g�̔���
//
// ---------------------------------------------------------------------------
BOOL CEnmObj::Start()
{
	m_bActive = TRUE;	// �G�q�r�I�u�W�F�N�g

	// �i�r�Q�[�V�����}�b�v�ɏ]���Ĕ���������
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
// �G�q�r�I�u�W�F�N�g�̐���
//
//�@�L�[�{�[�h�œG�𓮂���
//�@�G�̃A�j���[�V�����A�W�����v�A���R����
//�@���[���h���W�ϊ��A�}�b�v�Ƃ̐ڐG����
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmObj::Update()
{
	if (!m_bActive) return;  // ��\���̂Ƃ��͏��������Ȃ�

	// �ϐ��̏�����   ---------------------------------------
	m_vPosUp.x = m_vPosUp.y = m_vPosUp.z = 0.0f;
	m_vRotUp.x = m_vRotUp.y = m_vRotUp.z = 0.0f;
	m_mWorldOld = m_mWorld;  // ��O�̈ʒu

	// �X�e�[�^�X���Ƃ̏���  --------------------------------
	switch (m_dwStatus)
	{
	case  FLASH: // �G���_���[�W���畜�A�����Ƃ��̏����Bbreak�������Ɏ���NORMAL���s���B
	case  NORMAL: // �G���ʏ��Ԃ̂Ƃ��̏���
		UpdateNormal();
		break;

	case  DAMAGE: // �G���_���[�W��Ԃ̂Ƃ��̏���
		UpdateDamage();
		break;

	case  DEAD:	// �G�����S��Ԃ̂Ƃ��̏���
		UpdateDead();
		break;
	}

	// ���̓G�Əd�Ȃ�Ȃ��悤�ɂ��鏈��  --------------------
	SetKeepOffset(m_pGMain->m_pEnmProc);

	// �}�b�v�R���W�����Ǝ��R����  --------------------------
	m_fJumpY += (0.5f*GRAVITY*0.00005f)*m_fJumpTime;	// ���R����
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x) != 3)  // �}�b�v�R���W����
	{
		m_fJumpY = 0.0f;  // ���R�����̒�~
		m_fJumpTime = 0.0f;
	}

	// �`�揈��  ---------------------------------------------
	// �X�L�����b�V�������_�����O 
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);        // -- 2022.2.16
	//GetMesh()->RenderDisplace(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	// �o�E���f�B���O�{�b�N�X
	m_pBBox->m_mWorld = m_mWorld;
	//   �o�E���f�B���O�{�b�N�X�����_�����O
	//m_pBBox->Render(m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

	// ��Ɏ������A�C�e���̕\��������     // -- 2021.2.4
	if (GetHoldItem().m_nHoldObjNo != 0) // ��Ɏ������A�C�e��������Ƃ�        // -- 2022.2.16
	{
		MATRIX4X4 mHold;
		mHold = GetMesh()->GetFrameMatrices(m_AnimStatus, m_mWorld, GetHoldItem().m_nPosBone, GetHoldItem().m_nPosMesh);  // ��̈ʒu�̃{�[���}�g���b�N�X
		m_pGMain->m_pWeaponProc->m_pWeaponHoldProc->Disp(mHold, GetHoldItem()); // �A�C�e���\��
	}

	// �G�̓���̃X�e�[�^�X�\��  -----------------------------
	VECTOR3 vEnmPos = GetPositionVector(m_mWorld);
	vEnmPos.y += 3.0f;
	float fHp = (float)m_nHp / m_nMaxHp;
	// �g�o�o�[�̕\��
	m_pGMain->m_pBackForeProc->GetSprite()->Draw3D(m_pGMain->m_pBackForeProc->GetImage(), vEnmPos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, VECTOR2(1.2f*fHp, 0.2f), VECTOR2(320, 96), VECTOR2(125 * fHp, 16));
	// �G�̖��O�\��
	vEnmPos.y += 0.5f;
	m_pGMain->m_pFont->Draw3D(vEnmPos, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vEyePt, _T("���{�b�g�q�r"), VECTOR2(1.2f, 0.5f), RGB(255, 0, 0), 1.0f, _T("HGP�n�p�p�޼��UB"));

}

//-----------------------------------------------------------------------------
// �G�q�r�I�u�W�F�N�g�̐���
//
// �G���ʏ��Ԃ̂Ƃ��̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateNormal()
{

	CPcObj* pPcObj = m_pGMain->m_pPcProc->GetPcObjPtr();
	VECTOR3 vPc    = GetPositionVector(pPcObj->GetWorld());
	VECTOR3 vEnm   = GetPositionVector(m_mWorld);
	MATRIX4X4 mTemp;
	float fLen;
	const float fNormalSpeed = 0.02f, fAttackSpeed = 0.03f;             // �ړ��X�s�[�h(�ʏ�A�U����)
	const float fNormalDistLimit = 20.0f, fAttackDistLimit = 4.0f;      // �ߐڌ��E�l�i�ʏ�ڋ߁A�U���ڋ߁j
	const float fRotateSpeed = 3.0f;                                    // ��]�X�s�[�h
	float fForward;


	// �G���_���[�W���畜�A�����Ƃ��̏���  ------------------------------
	if (m_dwStatus == FLASH)
	{
		m_nCnt1--;
		if (m_nCnt1 <= 0) m_dwStatus = NORMAL;
	}

	// �U���X�e�[�^�X���Ƃ̏���  -----------------------------------------
	switch (m_dwStatusSub) 
	{
	case ATTACKWAIT:		// �E�G�C�g���[�h�̎�  ------------

		if (m_AnimStatus.animNum != 3) m_AnimStatus.SetNum(3);		// �E�G�C�g�̃��[�V�����ɂȂ��Ă��Ȃ�������؂�ւ��� 3��

		fLen = magnitude(vPc - vEnm);	// �o�b�Ƃ̋����𑪂�
		m_nCnt1--;

		// �E�G�C�g�̃^�C���A�E�g���o�b���߂Â��Ă����Ƃ��́A�ʏ탂�[�h�ɂ���
		if (m_nCnt1 <= 0 || (fLen <= fNormalDistLimit && pPcObj->GetStatus() == NORMAL))
		{
			m_dwStatusSub = ATTACKNONE;	// �ʏ탂�[�h��
			m_dwStatusPhase = 0;
			m_nCnt1 = 0;
		}
		break;

	case ATTACKNONE:		// �U�����[�h�łȂ��Ƃ�  -------------

		fLen = magnitude(vPc - vEnm);	// �o�b�Ƃ̋����𑪂�

		if (m_AnimStatus.animNum != 0) m_AnimStatus.SetNum(0);		// �ʏ�̃��[�V�����ɂȂ��Ă��Ȃ�������؂�ւ��� 0��

		// ---------------------------------------------------
		// �P�@�o�b�Ɨ���Ă���i�ʏ�ڋߌ��E�l���j�̂ŁA���낤�낷��
		// ---------------------------------------------------
		if (fLen > fNormalDistLimit || pPcObj->GetStatus() != NORMAL)
		{
			if (GetMesh()->m_RootAnimType[m_AnimStatus.animNum] == eRootAnimNone)    // ���[�g�{�[���A�j���Ȃ���   // -- 2021.1.11
			{
				fForward = fNormalSpeed;
			}else{
				fForward = GetPositionVector( GetMesh()->GetRootAnimUpMatrices(m_AnimStatus)).z;  // ���[�g�{�[���A�j���̂Ƃ�
			}
			if (!TargetMove(m_vTarget, fForward))	// �ړI�n�iTarget�j�ɒB�������ǂ���
			{
				mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));					// �ړI�n�iTarget�j�֌������p�x
				m_mWorld = mTemp * m_mWorld;
				mTemp = XMMatrixTranslation(m_vPosUp.x, m_vPosUp.y + m_fJumpY, m_vPosUp.z);	// �ړI�n�iTarget�j�֌�����
				m_mWorld = mTemp * m_mWorld;
			}
			else {
				if (Random(0, 4) == 1)   // �E�G�C�g���[�h���ʏ�ړ����[�h�ɂ��邩�𔻒肷��(�m���P�^�T)
				{
					do {
						m_vTarget.x = (float)Random(m_Nav.vMin.x, m_Nav.vMax.x);	// ���̖ړI�n�iTarget�j���Z�b�g����
						m_vTarget.z = (float)Random(m_Nav.vMin.z, m_Nav.vMax.z);
					} while (magnitude(m_vTarget - vEnm) < 10.0f);  // �ړI�n�����ݒn����P�O���ȓ��̂Ƃ��͂�蒼��
				}
				else {
					m_nCnt1 = 400;				// �E�G�C�g�^�C�� 80�t���[���̔{��
					m_dwStatusSub = ATTACKWAIT;	// �E�G�C�g���[�h��
					m_AnimStatus.SetNum(3);		// �E�G�C�g�̃��[�V�����ɐ؂�ւ�� 3��
				}
			}
		}
		
	}
}

//-----------------------------------------------------------------------------
// �G�q�r�I�u�W�F�N�g�̐���
//
// �G���_���[�W��Ԃ̂Ƃ��̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateDamage()
{
	MATRIX4X4 mTemp;
	const int nFlashTime = 5;  // ���G��Ԃ̎���

	mTemp = XMMatrixTranslation(0, 0, -0.5f);	// �o�b�N����
	m_mWorld = mTemp * m_mWorld;

	m_nHp -= m_pHitObj->GetAtc();	// �U�����󂯂��_���[�W
	if (m_nHp <= 0)
	{
		m_nHp = 0;
		m_dwStatus = DEAD;		// HP���O�Ȃ̂Ŏ��S��
		m_AnimStatus.SetNum(2);		// ���S���[�V����
	}
	else {
		m_nCnt1 = nFlashTime;   // ���G��Ԃ̎���
		m_dwStatus = FLASH;     // �_���[�W����̕��A�������s��
	}

}
//-----------------------------------------------------------------------------
// �G�q�r�I�u�W�F�N�g�̐���
//
// �G�����S��Ԃ̂Ƃ��̏���
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CEnmObj::UpdateDead()
{
	if (m_AnimStatus.isEnd(2)) { // ���S���[�V�������I������Ƃ�
		m_bActive = FALSE;          // �G������
		m_AnimStatus.SetNum(0);	// ���S���[�V�������I������猳�ɖ߂�
		m_nHp = m_nMaxHp;
		m_dwStatus = NORMAL;
		m_dwStatusSub = ATTACKNONE;
		m_dwStatusPhase = 0;
		m_pProc->SetMaxWaitTime();
		m_pGMain->m_nScore += m_nMaxHp / 10;   // MaxHP��1/10���X�R�A�Ƃ��ĉ��Z
	}
}

