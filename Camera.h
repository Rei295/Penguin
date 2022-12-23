//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//
//		�J��������
//
//		(���_�ύX�����E��Q����������������Ă���)
//
//																Camera.h
//=============================================================================
#pragma once
#include "GameMain.h"

//======================================================================
// �J�����@�I�u�W�F�N�g�N���X
//======================================================================
class CCameraObj : public CBaseObj
{
protected:
	int     m_nCtrl;                       // �J��������i0:�s�o�r���_  1:�e�o�r���_�j
	MATRIX4X4 m_mBaseWorld;                // �����_�E���_�̊�_�ƂȂ��_���[���h�}�g���b�N�X
	VECTOR3 m_vLocalLookat;                // ���[�J�������_�x�N�g��
	VECTOR3 m_vLocalEye;                   // ���[�J�����_�x�N�g��
	VECTOR3 m_vLocalEyeRotDist;            // ���[�J�����_�x�N�g���𐶐����邽�߂̊p�x�ƕψ�(TPS�̂�)
	VECTOR3 m_vBaseLocalLookatTPS;         // ���[�J�������_�x�N�g�������l TPS
	VECTOR3 m_vBaseLocalEyeRotDistTPS;     // ���[�J�����_�x�N�g�������l(�p�x�ƕψ�) TPS
	VECTOR3 m_vBaseLocalLookatFPS;         // ���[�J�������_�x�N�g�������l FPS
	VECTOR3 m_vBaseLocalEyeFPS;            // ���[�J�����_�x�N�g�������l FPS

public:
	void Update() override;
	void EvasiveObstacle();                // -- 2020.11.19
	void ControlCam();                     // -- 2020.11.19
	void SetLocalEyeTPS(float fRotY, float fRotX, float fDist);
	void SetCamBase();
	int  GetCtrl() { return m_nCtrl; }
	CCameraObj(CBaseProc* pProc);	// �R���X�g���N�^    // -- 2022.2.16
	virtual	~CCameraObj();
};


//======================================================================
// �J�����@�v���V�[�W���N���X
//======================================================================
class CCameraProc : public CBaseProc
{
protected:
	;

public:
	CCameraObj*	 GetCameraObjPtr() { return (CCameraObj*)GetObjArrayPtr()[0]; }
	CCameraProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CCameraProc() { ; }
};

