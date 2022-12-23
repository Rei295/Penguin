//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//
//		�J��������
//
//		(���_�ύX�����E��Q����������������Ă���)
//
//																Camera.cpp
//=============================================================================
#include  "Camera.h"
#include  "Playchar.h"
#include  "Map.h"

//==========================================================================================================================
//
// �J�����v���V�[�W���̃R���X�g���N�^
//
//   �Ȃ��A�I�u�W�F�N�g��delete��CBaseProc�̃f�X�g���N�^�ōs�����ߕs�v
//
// -------------------------------------------------------------------------------------------------------------------------
CCameraProc::CCameraProc(CGameMain* pGMain) : CBaseProc(pGMain)
{

	m_pObjArray.push_back(new CCameraObj(this));  // m_pObjArray�ɃI�u�W�F�N�g��o�^����

}

// --------------------------------------------------------------------------- // -- 2020.11.19
//
// �J�����I�u�W�F�N�g�̃R���X�g���N�^
//
// ---------------------------------------------------------------------------
CCameraObj::CCameraObj(CBaseProc* pProc) : CBaseObj(pProc)
{
	// �J��������t���O
	m_nCtrl = 0;      // �ŏ���TPS���_�̃J�����ʒu

	// ��_���[���h�}�g���b�N�X�̏����l
	m_mBaseWorld = XMMatrixTranslation(0, 0, 0);

	// ��_���[���h�}�g���b�N�X����̕ψʏ����l�̐ݒ�
	// �@ TPS���_�̂Ƃ��̊�_���[���h�}�g���b�N�X����̕ψʒl
	// �E�����_�́A�ψʋ�����m_vBaseLocalLookatTPS�ɐݒ肷��
	m_vBaseLocalLookatTPS = VECTOR3(0.0f, m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox()->m_vMax.y, 0.0f);

	// �E���_�́A�ψʊp�x�w�C�x��m_vBaseLocalEyeRotDistTPS.x m_vBaseLocalEyeRotDistTPS.y�ɁA�ψʒ���������m_vBaseLocalEyeRotDistTPS.z�ɐݒ肷��
	m_vBaseLocalEyeRotDistTPS.z = -15.0f;  // �ψʒ�������
	m_vBaseLocalEyeRotDistTPS.x = XMConvertToDegrees( atan2f(m_pGMain->m_pPcProc->GetPcObjPtr()->GetBBox()->m_vMax.y + 0.5f, -m_vBaseLocalEyeRotDistTPS.z)); // �ψʊp�x�w
	m_vBaseLocalEyeRotDistTPS.y = 0.0f;   // �ψʊp�x�x

	// �A FPS���_�̂Ƃ��̊�_���[���h�}�g���b�N�X����̕ψʒl
	// �E�����_�E���_���ɁA�ψʋ�����m_vBaseLocalEyeFPS�ɐݒ肷��
	m_vBaseLocalLookatFPS = VECTOR3(0.0f, 1.5f, 7.0f);
	m_vBaseLocalEyeFPS    = VECTOR3(0.0f, 1.5f, 0.3f);

	// �J�����ψʏ����l��ݒ肷��
	SetCamBase();
}
// ---------------------------------------------------------------------------
//
// �J�����I�u�W�F�N�g�̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CCameraObj::~CCameraObj()
{
	;
}
//-----------------------------------------------------------------------------  // -- 2020.11.19
// �J�����I�u�W�F�N�g�̐���
//
//�@�o�b�̓����ɍ��킹�ăJ�����𐧌䂷��
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CCameraObj::Update()
{
	// �����_�E���_�̊�_�ƂȂ��_���[���h�}�g���b�N�X�̐ݒ�
	// (�����ł́A�o�b�̃��[�J�����}�g���b�N�X��ݒ肷��)
	m_mBaseWorld = m_pGMain->m_pPcProc->GetPcObjPtr()->GetLocalMatrix();

	// �J�����I�u�W�F�N�g�̈ʒu���蓮����
	// �܂��A�J���������ύX�i�s�o�r���_���e�o�r���_���j
	ControlCam();

	// �����_�̃x�N�g�����쐬���鏈��
	// (�����_�̃��[�J�����W�Ɗ�_���[���h�}�g���b�N�X���|�����킹�A�����_�}�g���b�N�X�𓾂�)
	MATRIX4X4 mLookat = XMMatrixTranslationFromVector(m_vLocalLookat) * m_mBaseWorld;
	m_pGMain->m_vLookatPt = GetPositionVector(mLookat);    // �����_�}�g���b�N�X���璍���_�ʒu�𓾂�

	// �J�����i���_�j�x�N�g�����쐬���鏈��
	// (�J�����i���_�j�̃��[�J�����W�Ɗ�_���[���h�}�g���b�N�X���|�����킹�A���_�}�g���b�N�X�𓾂�)
	MATRIX4X4 mEye = XMMatrixTranslationFromVector(m_vLocalEye) * m_mBaseWorld;
	m_pGMain->m_vEyePt = GetPositionVector( mEye );        // ���_�}�g���b�N�X���王�_�ʒu�𓾂�

	// TPS���_�̎��A��Q����������鏈��
	if (m_nCtrl == 0) EvasiveObstacle();

	// ���_�x�N�g���ƒ����_�x�N�g������J�����̃��[���h�}�g���b�N�X�𓾂�i���͎g�p���Ă��Ȃ��j
	m_mWorld = GetLookatMatrix(m_pGMain->m_vEyePt, m_pGMain->m_vLookatPt);

	// �r���[�}�g���b�N�X�̍쐬
	VECTOR3 vUpVec(0.0f, 1.0f, 0.0f);    // ����ʒu���΍��W�̏�����Ƃ���
	// �J����(���_)�̈ʒu�A������A����ђ����_���g�p���āA������W�n�̃r���[�s����쐬
	m_pGMain->m_mView = XMMatrixLookAtLH(m_pGMain->m_vEyePt, m_pGMain->m_vLookatPt, vUpVec);

	// �J�����̈ʒu�A������A����уJ�����̌������g�p���āA������W�n�̃r���[�s����쐬�i�g�p���Ă��Ȃ��j  // -- 2020.11.19
	//VECTOR3 vEyeDir = XMVector3TransformCoord(VECTOR3(0, 0, 1), GetRotateMatrix(m_mWorld));
	//m_pGMain->m_mView = XMMatrixLookToLH(m_pGMain->m_vEyePt, vEyeDir, vUpVec);

	// ���C�g���_����̃r���[�g�����X�t�H�[���iShadowMap�p�j���C�g���_���o�b�̈ʒu�ɍ��킹��Ƃ��i�g�p���Ă��Ȃ��j
	//m_pGMain->m_vLightEye = m_pGMain->m_vLookatPt + m_pGMain->m_vLightDir * 100.0f;	        // �J�����i���_�j�ʒu�������̕���100���̈ʒu�ɂ���
	//m_pGMain->m_mLightView = XMMatrixLookAtLH(m_pGMain->m_vLightEye, m_pGMain->m_vLookatPt, vUpVec);

}
//-----------------------------------------------------------------------------  // -- 2020.11.19
// ��Q����������鏈��
//
// �E���_�ƒ����_�Ƃ̊Ԃɏ�Q�����������ꍇ�A���_����Q���̑O�Ɉړ����鏈��
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CCameraObj::EvasiveObstacle()
{
	VECTOR3 vHit;
	VECTOR3 vNorm;
	if (m_pGMain->m_pMapProc->Hitcheck(m_pGMain->m_vEyePt, m_pGMain->m_vLookatPt, &vHit, &vNorm))   // ��Q���Ƃ̐ڐG�`�F�b�N
	{
		MATRIX4X4 mTemp;
		mTemp = GetLookatMatrix(vHit, m_pGMain->m_vLookatPt);        // ��Q���Ƃ̐ڐG�_���璍���ʒu������}�g���b�N�X
		mTemp = XMMatrixTranslation(0.0f, 0.0f, 0.01f) * mTemp;      // ��Q���Ƃ̐ڐG�_����1cm���������Ɉړ��������_�ʒu�𓾂�
		m_pGMain->m_vEyePt = GetPositionVector(mTemp);
	}
}
//-----------------------------------------------------------------------------  // -- 2020.11.19
// �J�����I�u�W�F�N�g�̎蓮���쐧��
//
//   �J�����I�u�W�F�N�g�̈ʒu���蓮�ő��삷��
//   �܂��A�J���������ύX�i�s�o�r���_���e�o�r���_���j
//
//   �����@�@�@�Ȃ�
//-----------------------------------------------------------------------------
void	CCameraObj::ControlCam()
{
	const float fRotUp  = 1.5f;     // �p�x�ύX�̑����l
	const float fDistUp = 0.05f;    // �����ύX�̑����l

	// �J�������_�̑��Έʒu�̕ύX
	if (m_nCtrl == 0)   // TPS���_�̎�
	{
	
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_RBRACKET)) // ��  ]
		{
			SetLocalEyeTPS(fRotUp, 0, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_SEMICOLON)) // ��  ;
		{
			SetLocalEyeTPS(-fRotUp, 0, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_AT)) // ��  @
		{
			SetLocalEyeTPS(0, fRotUp, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_COLON)) // ��  :
		{
			SetLocalEyeTPS(0, -fRotUp, 0);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_P)) // �߂Â���  P
		{
			SetLocalEyeTPS(0, 0, -fDistUp);
		}
		if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_LBRACKET)) // ��������  [
		{
			SetLocalEyeTPS(0, 0, fDistUp);
		}
		
	}
	


	// ��l�ɖ߂�����
	if (m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_HOME) || m_pGMain->m_pDI->CheckKey(KD_DAT, DIK_H))
	{
		SetCamBase();    // �J�����ψʏ����l��ݒ肷��
	}
}

//----------------------------------------------------------------------------- // -- 2020.11.19
// �s�o�r�̂Ƃ��J�����ʒu�̐ݒ菈��
//
//   m_vLocalEyeRotDist.x��m_vLocalEyeRotDist.y�ɉ�]�p�x�Am_vLocalEyeRotDist.z��
//   �����_���王�_�܂ł̒��������������Ă���A
//   ���̒l�ɁA�����̉�]�Ƌ��������Z���A���̌��ʂ���ɁA�ψʒl���v�Z���A
//   ���̌��ʂ�m_vLocalEye�ɐݒ肷��
//
//   ����	float fRotY : �x��]
//			float fRotX : �w��]
//			float fDist : ����
//
//   �߂�l�@�Ȃ�
//-----------------------------------------------------------------------------
void	CCameraObj::SetLocalEyeTPS(float fRotY, float fRotX, float fDist)
{
	m_vLocalEyeRotDist.x += fRotX;
	m_vLocalEyeRotDist.y += fRotY;
	m_vLocalEyeRotDist.z += fDist;

	MATRIX4X4 mRotY = XMMatrixRotationY(XMConvertToRadians(m_vLocalEyeRotDist.y));
	MATRIX4X4 mRotX = XMMatrixRotationX(XMConvertToRadians(m_vLocalEyeRotDist.x));
	MATRIX4X4 mPos  = XMMatrixTranslation(0, 0, m_vLocalEyeRotDist.z);
	mPos = mPos * mRotX * mRotY;
	m_vLocalEye = GetPositionVector(mPos);
}
//----------------------------------------------------------------------------- // -- 2020.11.19
// �J�����ψʏ����l��ݒ肷�鏈��
//
// �@ TPS���_�̂Ƃ�
//    ���_�́A�ψʊp�x�ƕψʒ����������A��Um_vLocalEyeRotDist�ɐݒ肵�A
//    ���̌�SetLocalEyeTPS�֐��ɂ���āAm_vLocalEye�ɐݒ肷��
//    �����_�́A�ψʋ�����m_vLocalLookatTPS�ɐݒ肷��
//
// �A FPS���_�̂Ƃ�
//    ���_�́A�ψʋ�����m_vLocalEye�ɐݒ肷��
//    �����_�́A�ψʋ�����m_vLocalLookat�ɐݒ肷��
//
//   ����	�Ȃ�
//
//   �߂�l�@�Ȃ�
//-----------------------------------------------------------------------------
void	CCameraObj::SetCamBase()
{
	// �J�����ψʂ̏����l��ݒ肷��
	if (m_nCtrl == 0)
	{
		m_vLocalEyeRotDist = m_vBaseLocalEyeRotDistTPS;
		m_vLocalLookat     = m_vBaseLocalLookatTPS;
		SetLocalEyeTPS(0, 0, 0);
	}
	else {
		m_vLocalEye    = m_vBaseLocalEyeFPS;
		m_vLocalLookat = m_vBaseLocalLookatFPS;
	}
}