//仲間
#pragma once
#include "GameMain.h"
#include "Playchar.h"
#include "Map.h"

//仲間のID
#define FELLOW_ID   101
//同時出現数
#define FELLOW_MAX  70
//最大出現数
#define FELLOW_ALL  70

//オブジェクトクラス
class CFellowProc;
class CFellowObj : public CBaseObj
{
protected:
	VECTOR3				m_vTarget;		// ターゲット
	NAVIGATIONMAP		m_Nav;			// ナビゲーションマップ

public:
	BOOL Start() override;
	void Update() override;
	void UpdateNormal();
	void UpdateDamage();
	void SetNav(NAVIGATIONMAP  Nav) { m_Nav = Nav; }

	CFellowObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CFellowObj();	// デストラクタ
};

//プロシージャクラス
class CFellowProc :public CBaseProc
{
public:
	// アクセス関数
	CFellowObj* GetFellowObjPtr() { return (CFellowObj*)GetObjArrayPtr()[2]; }
public:
	void Update() override;
	void ResetFellowNum(CBaseProc* pProc = NULL);
	int  CountActiveFellow(CBaseProc* pProc = NULL, int nNum = 0);
	CFellowProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CFellowProc();	// デストラクタ
};