//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//
//		��{�v���V�[�W���E��{�I�u�W�F�N�g�N���X����
//																BaseProc.h
//=============================================================================
#pragma once
#include "GameMain.h"

// -------------------------------------------------------------------------------------------------  // -- 2021.2.4
// 
// ��Ɏ��A�C�e���═����w�肷��\����
// 
// -------------------------------------------------------------------------------------------------
struct BASEHOLDITEM
{
	int				m_nHoldObjNo;	// ��Ɏ��A�C�e���̃I�u�W�F�N�g�ԍ�
	int				m_nPosMesh;		// �A�C�e������������X�L�����b�V���̈ʒu�@���b�V���ԍ�
	int				m_nPosBone;		// �A�C�e������������X�L�����b�V���̈ʒu�@�{�[���ԍ�
	VECTOR3			m_nOffsetPos;	// ��Ɏ��A�C�e���̈ʒu�I�t�Z�b�g
	VECTOR3			m_nOffsetRot;	// ��Ɏ��A�C�e���̊p�x�I�t�Z�b�g
	BASEHOLDITEM()
	{
		m_nHoldObjNo = 0;				// ��Ɏ��A�C�e���̃I�u�W�F�N�g�ԍ�
		m_nPosMesh = 0;					// �A�C�e������������X�L�����b�V���̈ʒu�@���b�V���ԍ�
		m_nPosBone = 0;					// �A�C�e������������X�L�����b�V���̈ʒu�@�{�[���ԍ�
		m_nOffsetPos = VECTOR3(0, 0, 0);	// ��Ɏ��A�C�e���̈ʒu�I�t�Z�b�g
		m_nOffsetRot = VECTOR3(0, 0, 0);	// ��Ɏ��A�C�e���̊p�x�I�t�Z�b�g
	}
};

class CBBox;
class CShader;
class CGameMain;
class CBaseProc;
// --------------------------------------------------------------------------------------------------
// ��{�I�u�W�F�N�g�N���X
// 
// ���@�I�u�W�F�N�g�Ƃ́A�Q�[�����ɓo�ꂷ����̕��́A�Ⴆ�΂o�b��A����̂P�Ȃǂ��w���B
// �@�@�Ȃ��A�I�u�W�F�N�g�́A�e�v���V�[�W���̃����o�[�z��Ƃ��Đ錾����
// 
// --------------------------------------------------------------------------------------------------
class CBaseObj
{
protected:
	CGameMain*		m_pGMain;			// �Q�[�����C���N���X
	CBaseProc*		m_pProc;			// �e�̃v���V�[�W��                     // -- 2022.2.16
	CBaseObj*       m_pObj;             //�e�̃I�u�W�F
	DWORD			m_dwStatus;			// �X�e�[�^�X
	DWORD			m_dwStatusSub;		// �X�e�[�^�X�T�u
	DWORD			m_dwStatusPhase;	// �X�e�[�^�X�t�F�[�Y
	BOOL			m_bActive;			// TRUE:�\��  FALSE:��\��
	DWORD			m_dwObjID;			// �I�u�W�F�N�g�h�c
	DWORD			m_dwObjNo;			// �I�u�W�F�N�g�m�n
	ANIMATION_STATUS m_AnimStatus;		// �A�j���[�V�����X�e�[�^�X
	CBBox*			m_pBBox;			// �o�E���f�B���O�{�b�N�X�N���X
	MATRIX4X4		m_mWorldOld;		// ���[���h�}�g���N�X�i��O�j
	MATRIX4X4		m_mWorld;			// ���[���h�}�g���N�X
	VECTOR3			m_vPosUp;			// �ړ�����
	VECTOR3			m_vRotUp;			// ��]����
	float			m_fJumpY;			// �W�����v����
	float			m_fJumpTime;		// �W�����v����
	CBaseObj*		m_pOyaObj;			// �e�I�u�W�F�N�g�i����I�u�W�F���j
	CBaseObj*		m_pHitObj;			// ������������̃I�u�W�F�N�g
	CBaseObj*       m_pEnmObj;          //�G�I�u�W�F�N�g
	VECTOR3			m_vHitPos;			// ���������ꏊ�̍��W
	VECTOR3			m_vHitNormal;		// ���������ꏊ�̖@�����W
	int             m_nCnt1;			// �J�E���^�[�P
	int             m_nCnt2;			// �J�E���^�[�Q
	int				m_nMaxHp;			// �ő�̗�
	int				m_nHp;				// �̗�
	int				m_nAtc;				// �U����

public:
	// �I�u�W�F�N�g�̔�������
	virtual BOOL	Start() { return TRUE; }
	virtual BOOL	Start(VECTOR3 vPos) { return TRUE; }
	virtual BOOL	Start(VECTOR3 vPos, VECTOR3 vNormal) { return TRUE; }
	virtual BOOL	Start(MATRIX4X4 mGun, DWORD dwOwner) { return TRUE; }
	virtual BOOL	Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner) { return TRUE; }
	virtual BOOL	Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner) { return TRUE; }
	virtual BOOL	Start(MATRIX4X4 mGun, VECTOR3 vOffset, MATRIX4X4 mOwnerWorld, DWORD dwOwner) { return TRUE; }

	// �����蔻�菈��
	virtual BOOL    Hitcheck(CBaseObj* pOtherObj, VECTOR3 vNow, VECTOR3 vOld);
	virtual BOOL    Hitcheck(CBaseObj* pOtherObj, CBBox* pBBoxAtack);

	// �X�V����
	virtual	void    Update() { ; }

	// �ړI�n�ivTargetX,Z�j�ւ̈ړ�����
	// (�Ȃ��A��]�X�s�[�h�̏ȗ��l��3.0f�Ƃ���)
	virtual	BOOL    TargetMove(VECTOR3 vTarget, float fSpeedIn, float fRotSpeed = 3.0f);

	// �I�u�W�F�N�g���m���d�Ȃ�Ȃ��悤�ɂ��郁���o�֐�
	virtual	CBaseObj*	SetKeepOffset(CBaseProc* pProj);
	// �I�u�W�F�N�g��mWorld�ƈ����ʒu�Ƃ̋��������߂�
	virtual	float   GetDistance(VECTOR3 vPos);    // -- 2018.8.2
	// �X�e�[�^�X�����Z�b�g����
	virtual	void    ResetStatus();      // -- 2019.6.8

	// �A�N�Z�X�֐�
	BOOL	GetActive() { return m_bActive; }
	void	SetActive(BOOL bActive) { m_bActive = bActive; }
	CFbxMesh* GetMesh();											// ���b�V���A�h���X    // -- 2022.2.16
	BASEHOLDITEM GetHoldItem();										// ��Ɏ��A�C�e��    // -- 2022.2.16
	DWORD	GetObjID() { return m_dwObjID; }						// �I�u�W�F�N�g�h�c    // -- 2022.2.16
	DWORD	GetObjNo() { return m_dwObjNo; }						// �I�u�W�F�N�g�m�n
	DWORD	GetStatus() { return m_dwStatus; }
	CBBox*	GetBBox() { return m_pBBox; }
	MATRIX4X4  GetWorld() { return m_mWorld; }
	MATRIX4X4  GetWorldOld() { return m_mWorldOld; }
	void    SetWorld(MATRIX4X4 mWorld) { m_mWorld = mWorld; }
	void    SetWorldOld(MATRIX4X4 mWorldOld) { m_mWorldOld = mWorldOld; }
	VECTOR3 GetPosUp() { return m_vPosUp; }
	void    SetPosUp(VECTOR3 vPosUp) { m_vPosUp = vPosUp; }
	int		GetMaxHp() { return m_nMaxHp; }
	int		GetHp() { return m_nHp; }
	int		GetAtc() { return m_nAtc; }
	ANIMATION_STATUS GetAnimStatus() { return m_AnimStatus; }    // -- 2018.8.2
	void    SetAnimStatus(ANIMATION_STATUS as) { m_AnimStatus = as; }    // -- 2018.8.2

	// �R���X�g���N�^�i��{�I�u�W�F�N�g�̏������j
	CBaseObj(CBaseProc* pProc);
	// �f�X�g���N�^
	virtual		~CBaseObj();
};

// --------------------------------------------------------------------------------------------------
// ��{�v���V�[�W���N���X
//
//   ���@�v���V�[�W���Ƃ́A�u�����̏�������ɂ܂Ƃ߂����́v�Ƃ����Ӗ�
//       �����̃I�u�W�F�N�g�≺�ʃv���V�[�W�����R���g���[������N���X�̂���
//
// --------------------------------------------------------------------------------------------------
class  CBaseProc
{
protected:
	CGameMain*		m_pGMain;				// �Q�[�����C���N���X

	// 
	// ���ʂ̃v���V�[�W���|�C���^�z��́A�v���V�[�W�����K�w�\���ɂ��ĊǗ�����Ƃ��Ɏg�p����
	// �I�u�W�F�N�g�|�C���^�z��́A�v���V�[�W���̒��ɃI�u�W�F�N�g��z�u���ĊǗ�����Ƃ��Ɏg�p����
	// ��ʓI�ɂ́A�ǂ��炩�Е��݂̂��g�p���邱��
	// 
	std::vector<CBaseProc*>  m_pProcArray;  // ���ʂ̃v���V�[�W���|�C���^�z��
	std::vector<CBaseObj*>   m_pObjArray;	// �I�u�W�F�N�g�|�C���^�z��

	CFbxMesh*		m_pMesh;				// Fbx���b�V��            // -- 2022.2.16
	BASEHOLDITEM	m_HoldItem;				// ��Ɏ��A�C�e��       // -- 2022.2.16
	DWORD			m_dwProcID;				// �v���V�[�W���h�c
	DWORD			m_dwPcProcID;			// PC�̃v���V�[�W���h�c
	DWORD			m_dwEnmProcID;			// �G�̃v���V�[�W���h�c
	DWORD			m_dwFellowProcID;	    // ���Ԃ̃v���V�[�W���h�c
	int				m_nMaxNum;				// �ő唭����
	int				m_nNum;					// ������
	int				m_nMaxwaitTime;			// �ő�҂�����
	int				m_nWaitTime;			// ���݂̑҂�����

public:
	// �S�̍X�V�����i�S�Ẵv���V�[�W����I�u�W�F�N�g��ǂ݉����čX�V����������j
	virtual void	UpdateAll() final;

	// �X�V�����iUpdateAll()�̒��ŋN���B�v���V�[�W���̍X�V�����j
	virtual void    Update() { ; }

	// �����蔻��i�S�Ẵv���V�[�W����I�u�W�F�N�g��ǂ݉����ď���������j
	virtual BOOL    Hitcheck(CBaseObj* pOtherObj, VECTOR3 vNow, VECTOR3 vOld) final;
	virtual BOOL    Hitcheck(CBaseObj* pOtherObj, CBBox* pBBoxAtack) final;

	//	�v���V�[�W���ƃI�u�W�F�N�g��T�����S�ẴI�u�W�F�N�g���m���A�N�e�B�u�ɂ���
	virtual void	SetNonActive();

	//	�������̐ݒ�
	virtual void	SetMaxNum() { m_nNum = m_nMaxNum; }

	//	�E�F�C�g�^�C���̐ݒ�
	virtual void	SetMaxWaitTime() { m_nWaitTime = m_nMaxwaitTime; }

	// �A�N�Z�X�֐�
	std::vector<CBaseProc*> GetProcArrayPtr() { return m_pProcArray; } // ���ʂ̃v���V�[�W���|�C���^�z��̃A�h���X��Ԃ�
	std::vector<CBaseObj*>  GetObjArrayPtr() { return m_pObjArray; }  // �I�u�W�F�N�g�|�C���^�z��̃A�h���X��Ԃ�
	CBaseObj* GetObj(int i=0) { return m_pObjArray[i]; } // -- 2022.2.16
	CGameMain* GetGMain() { return m_pGMain; }           // -- 2022.2.16
	CFbxMesh* GetMesh() { return m_pMesh; }              // -- 2022.2.16
	BASEHOLDITEM GetHoldItem() { return m_HoldItem; }    // -- 2022.2.16
	DWORD GetProcID() { return m_dwProcID; }             // -- 2022.2.16
	int  GetNum() { return m_nNum; }
	void AddNum(int n) { m_nNum += n; }

	// �R���X�g���N�^�i��{�v���V�[�W���̏������j
	CBaseProc(CGameMain* pGMain)
	{
		m_pGMain = pGMain;  // �Q�[�����C���N���X
		m_dwProcID = 0;     // �v���V�[�W���h�c
		m_nMaxwaitTime = 0;	// �ő�҂�����
		m_nWaitTime = 0;	// ���݂̑҂�����
		m_pMesh = NULL;		// Fbx���b�V��            // -- 2022.2.16
		m_nMaxNum = 0;		// �ő唭����
		m_nNum = 0;			// ������
	}
	// �f�X�g���N�^�i�v���V�[�W���ƃI�u�W�F�N�g�̉���j
	virtual		~CBaseProc();
};


