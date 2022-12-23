//����

#include  "Fellow.h"
#include  "Enemy.h"
#include  "Playchar.h"
#include  "Weapon.h"
#include  "Map.h"
#include  "Effect.h"
#include  "BackFore.h"

using namespace std;         //  ���O��Ԏw��


//�v���V�[�W���̃R���X�g���N�^
CFellowProc::CFellowProc(CGameMain* pGMain) : CBaseProc(pGMain)
{
	m_pGMain = pGMain;

	const int nWait = 0;                   // �G����������܂ł̑҂�����
	m_dwProcID = FELLOW_ID;                // ���Ԃ�ID
	m_nNum = m_nMaxNum = FELLOW_ALL;       // ���Ԃ̍ő�o����

	//Fbx�X�L�����b�V��
	m_pMesh = new CFbxMesh(m_pGMain->m_pFbxMeshCtrl, _T("Data/Char/Pengin/Pengin.mesh"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_Idle.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_walk.anmx"));
	m_pMesh->LoadAnimation(_T("Data/Char/Pengin/Pengin_Jump.anmx"));
	m_pMesh->m_fHeightMax = 0.01f;
	m_pMesh->m_iMaxDevide = 64;

	for (int i = 0; i < FELLOW_MAX; i++)
	{
		m_pObjArray.push_back(new CFellowObj(this));  // m_pObjArray�ɃI�u�W�F�N�g�𐶐�����
	}

	m_nWaitTime = m_nMaxwaitTime = nWait;
}

//�v���V�[�W���̔������������l�Ƀ��Z�b�g
void CFellowProc::ResetFellowNum(CBaseProc* pProc)
{
	if (pProc)   // �ΏۂƂ���v���V�[�W��
	{
		pProc->SetMaxNum();  // �ő唭����(m_nMaxNum)�𔭐���m_nNum�ɐݒ肷��

		// ���ʂ̃v���V�[�W����T�����Ă���
		for (DWORD i = 0; i < pProc->GetProcArrayPtr().size(); i++)
		{
			ResetFellowNum(pProc->GetProcArrayPtr()[i]);  // �ċA�Ăяo��
		}
	}
	else {   // ���v���V�[�W��
		SetMaxNum();  // �ő唭����(m_nMaxNum)�𔭐���m_nNum�ɐݒ肷��

		// ���ʂ̃v���V�[�W����T�����Ă���
		for (DWORD i = 0; i < m_pProcArray.size(); i++)
		{
			ResetFellowNum(m_pProcArray[i]);  // �ċA�Ăяo��
		}
	}
}

int CFellowProc::CountActiveFellow(CBaseProc* pProc, int nNum)
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
			nNum = CountActiveFellow(pProc->GetProcArrayPtr()[i], nNum);  // �ċA�Ăяo��
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
			nNum = CountActiveFellow(m_pProcArray[i], nNum);  // �ċA�Ăяo��
		}
	}

	return nNum;
}


//�v���V�[�W���̍X�V
void CFellowProc::Update()
{
	std::vector<DWORD> Nav;   // �I���i�r�}�b�v

	// �܂������ł��鐔�̏ꍇ�̂ݔ���������
	if (m_nNum > 0 && m_pGMain->m_pMapProc->GetNavMap().size() > 0)
	{
		// �i�r�}�b�v�̓G�h�c����v������̂�I���i�r�}�b�vNav�ɓo�^����
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
		if (Nav.size() > 0)  // �I���i�r�}�b�vNav�ɓo�^����Ă���Ƃ�
		{
			if (m_nWaitTime <= 0)
			{
				// �I���i�r�}�b�vNav�̒����甭������i�r�}�b�v�������_���ɑI��
				int k = Random(0, Nav.size() - 1);
				int no = Nav[k];
				for (DWORD i = 0; i < m_pObjArray.size(); i++)
				{
					if (!m_pObjArray[i]->GetActive())
					{
						((CFellowObj*)m_pObjArray[i])->SetNav(m_pGMain->m_pMapProc->GetNavMap()[no]);  // �I�΂ꂽ�i�r�}�b�v���I�u�W�F�N�g�ɓo�^����
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
}

//�v���V�[�W���̃f�X�g���N�^
CFellowProc::~CFellowProc()
{
	;
}

//�I�u�W�F�N�g�̃R���X�g���N�^
CFellowObj::CFellowObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	const int nMaxHP = 1200;     // �̗�

	m_bActive = FALSE;
	m_dwStatus = NORMAL;

	m_vTarget = GetPositionVector(m_mWorld);

	// �I�u�W�F�N�g�̃o�E���f�B���O�{�b�N�X�̐ݒ�
	VECTOR3 vMin(-0.6f, -0.08f, -0.6f);
	VECTOR3 vMax(0.6f, 0.8f, 0.6f);
	m_pBBox = new CBBox(m_pGMain->m_pShader, vMin, vMax);
	m_pBBox->m_mWorld = m_mWorld;                            // -- 2021.1.11

	m_nHp = m_nMaxHp = nMaxHP;

	m_AnimStatus.SetNum(0);
	m_AnimStatus.animFrame = 0;
}

//�I�u�W�F�N�g�̃f�X�g���N�^
CFellowObj::~CFellowObj()
{
	;
}

//�I�u�W�F�N�g�X�^�[�g
BOOL CFellowObj::Start()
{
	m_bActive = TRUE;	

	// �i�r�Q�[�V�����}�b�v�ɏ]���Ĕ���������
	m_mWorld = XMMatrixTranslation((float)Random(m_Nav.vMin.x, m_Nav.vMax.x),
		m_Nav.vMax.y, (float)Random(m_Nav.vMin.z, m_Nav.vMax.z));
	m_mWorldOld = m_mWorld;
	m_vTarget = VECTOR3((float)Random(m_Nav.vMin.x, m_Nav.vMax.x), m_Nav.vMax.y,
		(float)Random(m_Nav.vMin.z, m_Nav.vMax.z));
	m_pBBox->m_mWorld = m_mWorld;                            // -- 2021.1.11

	return TRUE;
}

//�I�u�W�F�N�g�̍X�V
void CFellowObj::Update()
{
	if (!m_bActive) return;  // ��\���̂Ƃ��͏��������Ȃ�

	// �ϐ��̏�����  ---------------------------------------
	m_vPosUp.x = m_vPosUp.y = m_vPosUp.z = 0.0f;
	m_vRotUp.x = m_vRotUp.y = m_vRotUp.z = 0.0f;
	m_mWorldOld = m_mWorld;  // ��O�̈ʒu

	// �e�X�e�[�^�X���Ƃ̏��� ------------------------------
	switch (m_dwStatus)
	{
	case  FLASH:  // ���Ԃ��_���[�W���畜�A�����Ƃ��̏����Bbreak�������Ɏ���NORMAL���s���B
	case  NORMAL: // ���Ԃ��ʏ��Ԃ̂Ƃ��̏���
		UpdateNormal();
		break;
	case  DAMAGE: // ���Ԃ��_���[�W��Ԃ̂Ƃ��̏���
		UpdateDamage();
		break;
	}

	// ���̓G�Əd�Ȃ�Ȃ��悤�ɂ��鏈��  -------------------
	SetKeepOffset(m_pGMain->m_pFellowProc);
	SetKeepOffset(m_pGMain->m_pEnmProc);

	
	// �}�b�v�R���W�����Ǝ��R����  -------------------------
	m_fJumpY += (0.5f * GRAVITY * 0.00005f) * m_fJumpTime;	// ���R����
	m_fJumpTime += 1.0f;
	if (m_pGMain->m_pMapProc->isCollisionMoveGravity(&m_mWorld, m_mWorldOld, m_pBBox->m_vMax.x) != 3)  // �}�b�v�R���W����
	{
		m_fJumpY = 0.0f;  // ���R�����̒�~
		m_fJumpTime = 0.0f;
	}
	
	

	// �`�揈��  ---------------------------------------------
	// �X�L�����b�V�������_�����O
	//GetMesh()->RenderDisplace(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);
	GetMesh()->Render(m_AnimStatus, m_mWorld, m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);        // -- 2022.2.16

	// �o�E���f�B���O�{�b�N�X
	m_pBBox->m_mWorld = m_mWorld;
	//   �o�E���f�B���O�{�b�N�X�����_�����O
	//m_pBBox->Render( m_pGMain->m_mView, m_pGMain->m_mProj, m_pGMain->m_vLightDir, m_pGMain->m_vEyePt);

}

void	CFellowObj::UpdateNormal()
{
	CPcObj* pPcObj = m_pGMain->m_pPcProc->GetPcObjPtr();
	VECTOR3 vPc = GetPositionVector(pPcObj->GetWorld());
	VECTOR3 vFellow = GetPositionVector(m_mWorld);
	MATRIX4X4 mTemp;
	float fLen;
	const float fNormalSpeed = 0.05f, fAttackSpeed = 0.07f;             // �ړ��X�s�[�h(�ʏ�A�U����)
	const float fNormalDistLimit = 10.0f, fAttackDistLimit = 1.0f;      // �ߐڌ��E�l�i�ʏ�ڋ߁A�U���ڋ߁j

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
	case ATTACKWAIT:		// �E�G�C�g���[�h�̎�  ----------------

		if (m_AnimStatus.animNum != 3) m_AnimStatus.SetNum(0);		// �E�G�C�g�̃��[�V�����ɂȂ��Ă��Ȃ�������؂�ւ��� 3��

		fLen = magnitude(vPc - vFellow);	// �o�b�Ƃ̋����𑪂�
		m_nCnt1--;

		// �E�G�C�g�̃^�C���A�E�g���o�b���߂Â��Ă����Ƃ��́A�ʏ탂�[�h�ɂ���
		if (m_nCnt1 <= 0 || (fLen <= fNormalDistLimit && pPcObj->GetStatus() == NORMAL))
		{
			m_dwStatusSub = ATTACKNONE;	// �ʏ탂�[�h��
			m_dwStatusPhase = 0;
			m_nCnt1 = 0;
		}
		break;

	case ATTACKNONE:		// �U�����[�h�łȂ��Ƃ�  --------------

		fLen = magnitude(vPc - vFellow);	// �o�b�Ƃ̋����𑪂�

		if (m_AnimStatus.animNum != 0) m_AnimStatus.SetNum(0);		// �ʏ�̃��[�V�����ɂȂ��Ă��Ȃ�������؂�ւ��� 0��

		
		// ---------------------------------------------------
		// �P�@�o�b�Ɨ���Ă���i�ʏ�ڋߌ��E�l���j�̂ŁA���낤�낷��
		// ---------------------------------------------------
		if (fLen > fNormalDistLimit || pPcObj->GetStatus() != NORMAL)
		{
			if (GetMesh()->m_RootAnimType[m_AnimStatus.animNum] == eRootAnimNone)    // ���[�g�{�[���A�j���Ȃ���   // -- 2021.1.11
			{
				fForward = fNormalSpeed;
			}
			else {
				fForward = GetPositionVector(GetMesh()->GetRootAnimUpMatrices(m_AnimStatus)).z;  // ���[�g�{�[���A�j���̂Ƃ�
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
					} while (magnitude(m_vTarget - vFellow) < 10.0f);  // �ړI�n�����ݒn����P�O���ȓ��̂Ƃ��͂�蒼��
				}
				else {
					m_nCnt1 = 480;				// �E�G�C�g�^�C�� 120�t���[���̔{��
					m_dwStatusSub = ATTACKWAIT;	// �E�G�C�g���[�h��
					m_AnimStatus.SetNum(0);		// �E�G�C�g�̃��[�V�����ɐ؂�ւ�� 3��
				}
			}
		}
		break;
	}
}

//�_���\�W
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
	const float fNormalSpeed = 0.05f, fAttackSpeed = 0.17f;             // �ړ��X�s�[�h(�ʏ�A�U����)
	const float fNormalDistLimit = 3.0f;                                // ���̋���
	const float fNormal2DistLimit = 5.0f;                               // ���̋���
	const float fRotateSpeed = 1.0f;                                    // ��]�X�s�[�h
	float fForward;

	fLen = magnitude(vPc - vFellow);                                    // PC�Ƃ̋���
	fChange = magnitude(vFellow - vFellow);                             // ���ԓ��m�̋���

#if 1
		//PC�ɓ����������߂��Ă���
		vPc.y = vFellow.y;
		m_vRotUp.y = GetTargetRotateVector(m_mWorld, vPc).y;	// �o�b�̕�������������Y���p�x�����߂�
		if (m_vRotUp.y >= 180)  m_vRotUp.y -= 360;
		if (m_vRotUp.y <= -180) m_vRotUp.y += 360;
		if (m_vRotUp.y > fRotateSpeed)  m_vRotUp.y = fRotateSpeed;	// fRotateSpeed�x�ȏ�̕����ϊ��͍s��Ȃ�
		if (m_vRotUp.y < -fRotateSpeed) m_vRotUp.y = -fRotateSpeed;	// fRotateSpeed�x�ȏ�̕����ϊ��͍s��Ȃ�
		mTemp = XMMatrixRotationY(XMConvertToRadians(m_vRotUp.y));
		m_mWorld = mTemp * m_mWorld;
		SetKeepOffset(m_pGMain->m_pFellowProc);
		if (fLen > 0 && fLen < fNormalDistLimit) {
			mTemp = XMMatrixTranslation(0.0f, m_fJumpY, fAttackSpeed);	// �ړI�n�iTarget�j�֌�����
			m_mWorld = mTemp * m_mWorld;
		}
		else if(fLen >= fNormalDistLimit){
			mTemp = XMMatrixTranslation(0.0f, m_fJumpY, fAttackSpeed * 2);	// �ړI�n�iTarget�j�֌�����
			m_mWorld = mTemp * m_mWorld;
		}
		else if (fLen >= fNormal2DistLimit) {
			mTemp = XMMatrixTranslation(0.0f, m_fJumpY, fAttackSpeed * 3);	// �ړI�n�iTarget�j�֌�����
			m_mWorld = mTemp * m_mWorld;
		}
		

		//if (GetMesh()->m_RootAnimType[m_AnimStatus.animNum] == eRootAnimNone)    // ���[�g�{�[���A�j���Ȃ���   // -- 2021.1.11
		//{
		//	mTemp = XMMatrixTranslation(0.0f, m_fJumpY, fAttackSpeed);	// �ړI�n�iTarget�j�֌�����
		//}
		//else {
		//	mTemp = XMMatrixTranslation(0.0f, m_fJumpY, fAttackSpeed) * GetMesh()->GetRootAnimUpMatrices(m_AnimStatus);  // ���[�g�{�[���A�j���̂Ƃ�
		//}
		//m_mWorld = mTemp * m_mWorld;
#endif
}