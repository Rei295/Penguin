//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//		武器　　　ヘッダファイル								Weapon.h
//=============================================================================
#pragma once

#include "GameMain.h"
#include "Effect.h"

#define  LASER_MAX          20
#define  FIREBALL_MAX       50
#define  FIREBALL_FIRE_MAX  10
#define  FLAME_MAX          100
#define  FLAME_FIRE_MAX     10
#define  BALL_MAX           200
#define  SPRAY_MAX          300
#define  HOLDITEM_MAX       15

// --------------------------------------------------------------------
//武器　レーザー　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponLaserObj : public CBaseObj
{
protected:
	VECTOR3                  m_vTarget;			// ターゲット
	VECTOR3                  m_vStart;			// 発射位置
	VECTOR3                  m_vEnd;			// 最終到達点
	DWORD                    m_dwOwner;			// 武器を発射したキャラ区分（PC:PC　ENM:敵）

public:
	BOOL Start(MATRIX4X4, DWORD dwOwner) override;
	void Update() override;
	void Render();

	// アクセス関数
	CWeaponLaserObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponLaserObj();
};
// --------------------------------------------------------------------
//武器　レーザー　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponLaserProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(MATRIX4X4, DWORD dwOwner);  //武器　レーザーの発射
	BOOL Start(MATRIX4X4 mGun, VECTOR3 vOffset, MATRIX4X4 mOwnerWorld, DWORD dwOwner);

	CWeaponLaserProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponLaserProc();
};
// --------------------------------------------------------------------
//武器　火の玉　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponFireBallObj : public CBaseObj
{
protected:
	DWORD                    m_dwOwner;
	CBillFireObj*            m_pBillFire[FIREBALL_FIRE_MAX];
public:
	BOOL Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner) override;
	void Update() override;
	void Render();

	CWeaponFireBallObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponFireBallObj();
};
// --------------------------------------------------------------------
//武器　火の玉　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponFireBallProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner);//武器　火の玉の発射

	CWeaponFireBallProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponFireBallProc();
};

// --------------------------------------------------------------------
//武器　剣　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponSwordObj : public CBaseObj
{
protected:
	DWORD           m_dwOwner;
public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner) override;
	void Update() override;
	void Render();

	CWeaponSwordObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponSwordObj();
};
// --------------------------------------------------------------------
// 武器　剣　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponSwordProc : public CBaseProc
{
public:
	BOOL Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner);	// 剣の発射
	CWeaponSwordProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponSwordProc() { ; }
};

// --------------------------------------------------------------------
//武器　火炎　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponFlameObj : public CBaseObj
{
protected:
	DWORD                    m_dwOwner;
	CBillFireObj*            m_pBillFire[FLAME_MAX];
public:
	BOOL Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner) override;
	void Update() override;
	void Render();

	// アクセス関数
	CWeaponFlameObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponFlameObj();
};
// --------------------------------------------------------------------
//武器　火炎　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponFlameProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner);//武器　火炎の発射

	CWeaponFlameProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponFlameProc();
};

// --------------------------------------------------------------------
//武器　ボール　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponBallObj : public CBaseObj
{
protected:
	DWORD                    m_dwOwner;
public:
	BOOL Start(MATRIX4X4 mStartWorld, DWORD dwOwner) override;
	void Update() override;
	void Render();

	// アクセス関数
	CWeaponBallObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponBallObj();
};
// --------------------------------------------------------------------
//武器　ボールプロシージャクラス
// --------------------------------------------------------------------
class CWeaponBallProc : public CBaseProc
{
protected:
	;
public:
	BOOL Start(MATRIX4X4 mGun, VECTOR3 vOffset, MATRIX4X4 mOwnerWorld, DWORD dwOwner);  //武器　ボールの発射

	CWeaponBallProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponBallProc();
};

// --------------------------------------------------------------------
//武器　スプレー　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponSprayObj : public CBaseObj
{
protected:
	CSprite*            m_pSprite;		// スプライト
	DWORD               m_dwOwner;
public:
	BOOL Start(MATRIX4X4 mStartWorld, DWORD dwOwner) override;
	void Update() override;
	void Render();

	// アクセス関数
	void CreateSprite(CSpriteImage*  pImage);

	CWeaponSprayObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponSprayObj();
};
// --------------------------------------------------------------------
//武器　スプレープロシージャクラス
// --------------------------------------------------------------------
class CWeaponSprayProc : public CBaseProc
{
protected:
	CSpriteImage*       m_pImage;   // スプライトイメージ
public:
	BOOL Start(MATRIX4X4 mGun, VECTOR3 vOffset, MATRIX4X4 mOwnerWorld, DWORD dwOwner);  //武器　ボールの発射

	CWeaponSprayProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponSprayProc();
};

// --------------------------------------------------------------------
//武器　手に持つアイテム　オブジェクトクラス
// --------------------------------------------------------------------
class CWeaponHoldObj : public CBaseObj
{
protected:
	int                      m_nKindNo;			// 手に持つアイテムの種類(1:剣　2:銃)
	CFbxMesh*                m_pMesh;			// Fbxメッシュ(プロシージャではなくオブジェクトにメッシュを持たせる)
	VECTOR3                  m_vOffset;			// 銃口までのオフセット（銃のみ）
public:
	void Init();
	BOOL Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem);
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset);
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax);
	CBBox* GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem);

	void Update() override;
	void Render();

	int GetKind() { return m_nKindNo; }

	CWeaponHoldObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CWeaponHoldObj();
};
// --------------------------------------------------------------------
//武器　手に持つアイテム　プロシージャクラス
// --------------------------------------------------------------------
class CWeaponHoldProc : public CBaseProc
{
protected:
	;
public:
	BOOL Disp(MATRIX4X4 mHold, BASEHOLDITEM HoldItem); // 武器　の表示
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vOffset);
	void GetInfo(MATRIX4X4 mHold, BASEHOLDITEM HoldItem, MATRIX4X4& mMat, VECTOR3& vMin, VECTOR3& vMax);
	CBBox* GetBBox(MATRIX4X4 mHold, BASEHOLDITEM HoldItem);
	int GetKind(BASEHOLDITEM HoldItem) { return ((CWeaponHoldObj*)m_pObjArray[HoldItem.m_nHoldObjNo])->GetKind(); }
	CWeaponHoldProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponHoldProc();
};

// ======================================================================
// 武器全体のメインプロシージャクラス
// ======================================================================
class CWeaponProc : public CBaseProc
{
public:
	CWeaponLaserProc*     m_pWeaponLaserProc;
	CWeaponFireBallProc*  m_pWeaponFireBallProc;
	CWeaponSwordProc*     m_pWeaponSwordProc;
	CWeaponFlameProc*     m_pWeaponFlameProc;

	CWeaponBallProc*      m_pWeaponBallProc;
	CWeaponSprayProc*     m_pWeaponSprayProc;
	CWeaponHoldProc*      m_pWeaponHoldProc;

public:
	CWeaponProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CWeaponProc() { ; }
};


