//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//		敵キャラクター　ヘッダファイル							Enemy.h
//=============================================================================
#pragma once
#include "GameMain.h"
#include "Map.h"

// 敵のＩＤと発生数
// 
//   ID    :敵のＩＤ。１から順番に振る
//   MAX   :敵オブジェクトの数。（同時発生数）
//   ALLMAX:敵の最大出現数。（復活も含める）
// 

#define  ENM_RS_ID           2
#define  ENM_RS_MAX          100
#define  ENM_RS_ALLMAX       100

//======================================================================
// 敵ＲＳ　オブジェクトクラス
//======================================================================
class CEnmProc;
class CEnmObj : public CBaseObj
{
protected:
	VECTOR3				m_vTarget;	// ターゲット
	NAVIGATIONMAP		m_Nav;		// ナビゲーションマップ

public:
	BOOL Start() override;
	void Update() override;
	void UpdateNormal();
	void UpdateDamage();
	void UpdateDead();
	void SetNav(NAVIGATIONMAP  Nav) { m_Nav = Nav; }
	CEnmObj(CBaseProc* pProc);	// コンストラクタ        // -- 2022.2.16
	virtual	~CEnmObj();	// デストラクタ
};

//======================================================================
// 敵キャラクター全体のメインプロシージャクラス
//======================================================================
class CEnmProc : public CBaseProc
{
public:
	// アクセス関数
	CEnmObj* GetEnmObjPtr() { return (CEnmObj*)GetObjArrayPtr()[1]; }

public:
	void Update()override;
	void ResetEnmNum(CBaseProc* pProc = NULL);
	int  CountActiveEnm(CBaseProc* pProc = NULL, int nNum = 0);
	CEnmProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CEnmProc() { ; }	// デストラクタ

};
