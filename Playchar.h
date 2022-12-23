//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//		プレイキャラクター　ヘッダファイル							Playchar.h
//=============================================================================
#pragma once
#include "GameMain.h"

#define PC_ID  201;


//======================================================================
// プレイキャラクター　オブジェクトクラス
//======================================================================
class CPcObj : public CBaseObj
{
protected:
	float				m_fLocalRotY;		// ローカル軸(Y軸)    // -- 2018.8.4
	BOOL				m_bSide;			// 横移動モード       // -- 2018.8.4

	float				m_fSpeed;			// 移動速度           // -- 2019.3.5
	float				m_fRotSpeed;		// 回転速度           // -- 2019.3.5
	float				m_fJumpSpeed;		// ジャンプ速度       // -- 2019.3.5

public:
	void Update() override;
	void UpdateNormal();       // -- 2019.3.5
	void UpdateNormalMove();   // -- 2019.3.5
	MATRIX4X4 UpdateNormalMoveKeystate(DWORD DIKey);  // -- 2021.1.10
	void UpdateDamage();       // -- 2019.3.5
	void UpdateDead();         // -- 2019.3.5
	MATRIX4X4  GetLocalMatrix();			// ローカル軸マトリックスを得る
	void SetLocalRotY(float fRotY) { m_fLocalRotY = fRotY; }   // ローカル軸(Y軸)をセットする

	CPcObj(CBaseProc* pProc );	// コンストラクタ        // -- 2022.2.16
	virtual	~CPcObj();
};

//======================================================================
// プレイキャラクター　プロシージャクラス
//======================================================================
class CPcProc : public CBaseProc
{
protected:
	;

public:
	// アクセス関数
	CPcObj* GetPcObjPtr() { return (CPcObj*)GetObjArrayPtr()[0]; }

	// コンストラクタ
	CPcProc(CGameMain* pGMain);
	virtual	~CPcProc();
};

