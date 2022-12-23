//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//		�v���C�L�����N�^�[�@�w�b�_�t�@�C��							Playchar.h
//=============================================================================
#pragma once
#include "GameMain.h"

#define PC_ID  201;


//======================================================================
// �v���C�L�����N�^�[�@�I�u�W�F�N�g�N���X
//======================================================================
class CPcObj : public CBaseObj
{
protected:
	float				m_fLocalRotY;		// ���[�J����(Y��)    // -- 2018.8.4
	BOOL				m_bSide;			// ���ړ����[�h       // -- 2018.8.4

	float				m_fSpeed;			// �ړ����x           // -- 2019.3.5
	float				m_fRotSpeed;		// ��]���x           // -- 2019.3.5
	float				m_fJumpSpeed;		// �W�����v���x       // -- 2019.3.5

public:
	void Update() override;
	void UpdateNormal();       // -- 2019.3.5
	void UpdateNormalMove();   // -- 2019.3.5
	MATRIX4X4 UpdateNormalMoveKeystate(DWORD DIKey);  // -- 2021.1.10
	void UpdateDamage();       // -- 2019.3.5
	void UpdateDead();         // -- 2019.3.5
	MATRIX4X4  GetLocalMatrix();			// ���[�J�����}�g���b�N�X�𓾂�
	void SetLocalRotY(float fRotY) { m_fLocalRotY = fRotY; }   // ���[�J����(Y��)���Z�b�g����

	CPcObj(CBaseProc* pProc );	// �R���X�g���N�^        // -- 2022.2.16
	virtual	~CPcObj();
};

//======================================================================
// �v���C�L�����N�^�[�@�v���V�[�W���N���X
//======================================================================
class CPcProc : public CBaseProc
{
protected:
	;

public:
	// �A�N�Z�X�֐�
	CPcObj* GetPcObjPtr() { return (CPcObj*)GetObjArrayPtr()[0]; }

	// �R���X�g���N�^
	CPcProc(CGameMain* pGMain);
	virtual	~CPcProc();
};

