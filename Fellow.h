//����
#pragma once
#include "GameMain.h"
#include "Playchar.h"
#include "Map.h"

//���Ԃ�ID
#define FELLOW_ID   101
//�����o����
#define FELLOW_MAX  70
//�ő�o����
#define FELLOW_ALL  70

//�I�u�W�F�N�g�N���X
class CFellowProc;
class CFellowObj : public CBaseObj
{
protected:
	VECTOR3				m_vTarget;		// �^�[�Q�b�g
	NAVIGATIONMAP		m_Nav;			// �i�r�Q�[�V�����}�b�v

public:
	BOOL Start() override;
	void Update() override;
	void UpdateNormal();
	void UpdateDamage();
	void SetNav(NAVIGATIONMAP  Nav) { m_Nav = Nav; }

	CFellowObj(CBaseProc* pProc);	// �R���X�g���N�^        // -- 2022.2.16
	virtual	~CFellowObj();	// �f�X�g���N�^
};

//�v���V�[�W���N���X
class CFellowProc :public CBaseProc
{
public:
	// �A�N�Z�X�֐�
	CFellowObj* GetFellowObjPtr() { return (CFellowObj*)GetObjArrayPtr()[2]; }
public:
	void Update() override;
	void ResetFellowNum(CBaseProc* pProc = NULL);
	int  CountActiveFellow(CBaseProc* pProc = NULL, int nNum = 0);
	CFellowProc(CGameMain* pGMain);	// �R���X�g���N�^
	virtual	~CFellowProc();	// �f�X�g���N�^
};