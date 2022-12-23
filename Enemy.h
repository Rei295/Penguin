//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//		�G�L�����N�^�[�@�w�b�_�t�@�C��							Enemy.h
//=============================================================================
#pragma once
#include "GameMain.h"
#include "Map.h"

// �G�̂h�c�Ɣ�����
// 
//   ID    :�G�̂h�c�B�P���珇�ԂɐU��
//   MAX   :�G�I�u�W�F�N�g�̐��B�i�����������j
//   ALLMAX:�G�̍ő�o�����B�i�������܂߂�j
// 

#define  ENM_RS_ID           2
#define  ENM_RS_MAX          100
#define  ENM_RS_ALLMAX       100

//======================================================================
// �G�q�r�@�I�u�W�F�N�g�N���X
//======================================================================
class CEnmProc;
class CEnmObj : public CBaseObj
{
protected:
	VECTOR3				m_vTarget;	// �^�[�Q�b�g
	NAVIGATIONMAP		m_Nav;		// �i�r�Q�[�V�����}�b�v

public:
	BOOL Start() override;
	void Update() override;
	void UpdateNormal();
	void UpdateDamage();
	void UpdateDead();
	void SetNav(NAVIGATIONMAP  Nav) { m_Nav = Nav; }
	CEnmObj(CBaseProc* pProc);	// �R���X�g���N�^        // -- 2022.2.16
	virtual	~CEnmObj();	// �f�X�g���N�^
};

//======================================================================
// �G�L�����N�^�[�S�̂̃��C���v���V�[�W���N���X
//======================================================================
class CEnmProc : public CBaseProc
{
public:
	// �A�N�Z�X�֐�
	CEnmObj* GetEnmObjPtr() { return (CEnmObj*)GetObjArrayPtr()[1]; }

public:
	void Update()override;
	void ResetEnmNum(CBaseProc* pProc = NULL);
	int  CountActiveEnm(CBaseProc* pProc = NULL, int nNum = 0);
	CEnmProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CEnmProc() { ; }	// �f�X�g���N�^

};
