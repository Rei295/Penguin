//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//		�}�b�v�@�w�b�_�t�@�C��											Map.h
//=============================================================================
#pragma once
#include "GameMain.h"

// --------------------------------------------------------------------
//
// �X�e�[�W�}�b�v
//
// --------------------------------------------------------------------
struct STAGEMAP
{
	CFbxMesh*				m_pMesh;			// �X�^�e�B�b�N���b�V���ւ̃|�C���^
	CWave*					m_pWave;			// �g�̃��b�V���ւ̃|�C���^
	MATRIX4X4				m_mWorld;			// ���b�V���̃��[���h�}�g���b�N�X
	int						m_nMaterialFlag;	// �}�e���A���t���O�@0:�ʏ�@1:�����F�@2:�f�B�X�v���C�X�����g�}�b�s���O 3:�g

	STAGEMAP()    // �R���X�g���N�^
	{
		Init();
	}
	~STAGEMAP()    // �f�X�g���N�^
	{
		// ���̒��Ń��b�V����DELETE�����Ă͂Ȃ�Ȃ�
	}
	void Init()    // ���������\�b�h
	{
		m_pMesh = NULL;
		m_pWave = NULL;
		m_mWorld = XMMatrixIdentity();
		m_nMaterialFlag = 0;
	}
};
// --------------------------------------------------------------------
//
// �ړ��}�b�v            �����̃v���O�������ł́A�g�p���Ȃ�
//
// --------------------------------------------------------------------
struct MOVEMAP
{
	BOOL					m_bActive;			// �ړ�Active
	CFbxMesh*				m_pMesh;			// �X�^�e�B�b�N���b�V���ւ̃|�C���^
	int						m_nMaterialFlag;	// �}�e���A���t���O�@0:�ʏ�@1:�����F�@2:�f�B�X�v���C�X�����g�}�b�s���O
	CCollision*				m_pColMesh;			// �ړ��R���W�������b�V���ւ̃|�C���^
	int						m_nMoveFlag;		// �ړ��t���O�@0:�ړ������@1:���s�ړ��@2:��]�@3:�g��k��      // -- 2019.12.30
	VECTOR3					m_vUp;				// �ړ�����
	VECTOR3					m_vMin;				// �ړ��ŏ��l
	VECTOR3					m_vMax;				// �ړ��ő�l
	int						m_nChangeFlag;		// �ړ������E�l�ɓ��B���ĕ���������ւ��Ƃ��@0:�ړ����@1:�����`�F���W
	int						m_nLoop;			// �ړ����J��Ԃ���  0:�`�F���W�̉ӏ��Œ�~���ړ��t���O��0:�ړ������ɖ߂��B 1:�ړ����J��Ԃ�  // -- 2021.2.4

	MOVEMAP()    // �R���X�g���N�^
	{
		Init();
	}
	~MOVEMAP()    // �f�X�g���N�^
	{
		// ���̒��Ń��b�V����DELETE�����Ă͂Ȃ�Ȃ�
	}
	void Init()
	{
		m_bActive = FALSE;
		m_pMesh = NULL;
		m_nMaterialFlag = 0;
		m_pColMesh = NULL;
		m_nMoveFlag = 0;
		m_vUp = VECTOR3(0, 0, 0);
		m_vMin = VECTOR3(-9999, -9999, -9999);
		m_vMax = VECTOR3(9999, 9999, 9999);
		m_nChangeFlag = 0;
		m_nLoop = 1;           // �����ݒ�̓��[�v        // -- 2021.2.4
	}
};

// --------------------------------------------------------------------
//
// �i�r�Q�[�V�����}�b�v
//
// --------------------------------------------------------------------
struct NAVIGATIONMAP
{
	VECTOR3 vMin;
	VECTOR3 vMax;
	std::vector<DWORD>  m_dwEnmID;  // �ΏۂƂȂ�G�̂h�c
	std::vector<DWORD>  m_dwFellowID;  // �ΏۂƂȂ钇�Ԃ̂h�c
	NAVIGATIONMAP()    // �R���X�g���N�^
	{
		Init();
	}
	void Init()
	{
		vMin = VECTOR3(0.0f, 0.0f, 0.0f);
		vMax = VECTOR3(0.0f, 0.0f, 0.0f);
	}
};

// --------------------------------------------------------------------    // -- 2021.2.4
//
// �C�x���g
//
// --------------------------------------------------------------------
// �C�x���g���
enum EVENTKIND
{
	eEvtKindNone = 0,
	eEvtChangeMap = 1,
	eEvtMoveMap = 2
};

struct EVENTMAP
{
	CBBox*    m_pBBox;       // �o�E���f�B���O�{�b�N�X
	int       m_nEvtCycle;   // �C�x���g�T�C�N�� 0:�C�x���g���s���Ă��Ȃ� 1:�C�x���g���s���i�g�p���Ă��Ȃ��j 2:�C�x���g�I���`�F�b�N�i�ڐG�I��������j
	EVENTKIND m_nEvtKind;    // �C�x���g�̎�� eEvtChangeMap:�}�b�v�ړ�, eEvtMoveMap:�ړ��}�b�v�̐���
	int       m_nEvtNo;      // �C�x���g�ԍ� eEvtChangeMap�̂Ƃ��͈ړ���̃}�b�v�ԍ��AeEvtMoveMap�̂Ƃ��͈ړ��}�b�v�̗v�f�ԍ�
	int       m_nEvtOpe1;    // eEvtMoveMap�̂݁B1:�ړ��}�b�v�̈ړ�Active�̎w��B2:�ړ��}�b�v�̈ړ��t���O�̎w��
	int       m_nEvtOpe2;    // eEvtMoveMap�̂݁B�ړ�Active�̂Ƃ���0:��\�� 1:�\��  �ړ��t���O�̂Ƃ��́@0:�ړ������@1:���s�ړ��@2:��]�@3:�g��k��
	int       m_nEvtKeyPush; // 0:�o�E���f�B���O�{�b�N�X�ɐڐG����ƃC�x���g�����A 1:�o�E���f�B���O�{�b�N�X�ɐڐG���āAENTER�L�[�������ƃC�x���g����

	EVENTMAP()    // �R���X�g���N�^
	{
		m_pBBox = NULL;
		m_nEvtCycle = 0;
		m_nEvtKind = eEvtKindNone;
		m_nEvtNo = 0;
		m_nEvtOpe1 = 0;
		m_nEvtOpe2 = 0;
		m_nEvtKeyPush = 0;
	}
	~EVENTMAP()    // �f�X�g���N�^
	{
		// ���̒��Ń��b�V����CBBox��DELETE�����Ă͂Ȃ�Ȃ�
	}
};

//======================================================================
// �}�b�v�@�v���V�[�W���N���X
//
// (����)�}�b�v�@�v���V�[�W���N���X�́A��{�v���V�[�W���N���X���p�����Ă��Ȃ�
// 
//======================================================================
class CMapProc
{
protected:
	CGameMain*							m_pGMain;

	DWORD								m_dwMapNo;
	BOOL								m_bActive;

	std::vector<STAGEMAP>				m_SkyMap;			// ��̃}�b�v
	std::vector<STAGEMAP>				m_StageMap;			// �X�e�[�W�̃}�b�v
	CCollision*							m_pColMesh;			// �R���W�������b�V��
	std::vector<MOVEMAP>				m_MoveMap;			// �ړ��}�b�v  *���̃v���O�������ł́A�g�p���Ȃ�
	std::vector<NAVIGATIONMAP>			m_NavMap;			// �i�r�Q�[�V�����}�b�v
	std::vector<EVENTMAP>				m_EventMap;			// �C�x���g�}�b�v    // -- 2021.2.4
	int									m_nEvtIdx;			// �C�x���g�v�f�ԍ�  // -- 2021.2.4

public:
	void Update();
	void Render();
	void Render2();
	void DestroyAll();
	void LoadMap1();
	void LoadMap2();
	void LoadMap3();
	void LoadMap6();

	void UpdateMoveMap();
	int  SetEvent(VECTOR3 vMin, VECTOR3 vMax, MATRIX4X4 mWorld, EVENTKIND nEvtKind, int nEvtNo, int nEvtOpe1 = 0, int nEvtOpe2 = 0, int nEvtKeyPush = 0);    // -- 2021.2.4
	void UpdateEvent();                                                                           // -- 2021.2.4
	void RunEvent(EVENTMAP& EventMap);                                                             // -- 2021.2.4

	BOOL Hitcheck(CBaseObj* pObj, VECTOR3* pHit, VECTOR3* pNormal);
	BOOL Hitcheck(VECTOR3 vNow, VECTOR3 vOld, VECTOR3* pHit, VECTOR3* pNormal);
	BOOL Hitcheck(VECTOR3 vNow, VECTOR3 vOld, float fRadius, VECTOR3* pHit, VECTOR3* pNormal);    // -- 2020.12.11

	int  isCollisionMoveGravity(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius = 0.2f);
	int  isCollisionMove(MATRIX4X4* pWorld, MATRIX4X4 mWorldOld, float fRadius = 0.2f);

	// �A�N�Z�X�֐�
	std::vector<NAVIGATIONMAP> GetNavMap() { return m_NavMap; }
	DWORD GetMapNo() { return m_dwMapNo; }

	// �R���X�g���N�^�i�v���V�[�W���̏������j
	CMapProc(CGameMain*	m_pGMain);
	~CMapProc();
};