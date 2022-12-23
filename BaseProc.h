//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//
//		基本プロシージャ・基本オブジェクトクラス処理
//																BaseProc.h
//=============================================================================
#pragma once
#include "GameMain.h"

// -------------------------------------------------------------------------------------------------  // -- 2021.2.4
// 
// 手に持つアイテムや武器を指定する構造体
// 
// -------------------------------------------------------------------------------------------------
struct BASEHOLDITEM
{
	int				m_nHoldObjNo;	// 手に持つアイテムのオブジェクト番号
	int				m_nPosMesh;		// アイテムを持たせるスキンメッシュの位置　メッシュ番号
	int				m_nPosBone;		// アイテムを持たせるスキンメッシュの位置　ボーン番号
	VECTOR3			m_nOffsetPos;	// 手に持つアイテムの位置オフセット
	VECTOR3			m_nOffsetRot;	// 手に持つアイテムの角度オフセット
	BASEHOLDITEM()
	{
		m_nHoldObjNo = 0;				// 手に持つアイテムのオブジェクト番号
		m_nPosMesh = 0;					// アイテムを持たせるスキンメッシュの位置　メッシュ番号
		m_nPosBone = 0;					// アイテムを持たせるスキンメッシュの位置　ボーン番号
		m_nOffsetPos = VECTOR3(0, 0, 0);	// 手に持つアイテムの位置オフセット
		m_nOffsetRot = VECTOR3(0, 0, 0);	// 手に持つアイテムの角度オフセット
	}
};

class CBBox;
class CShader;
class CGameMain;
class CBaseProc;
// --------------------------------------------------------------------------------------------------
// 基本オブジェクトクラス
// 
// ＊　オブジェクトとは、ゲーム中に登場する一つ一つの物体、例えばＰＣや、武器の１つなどを指す。
// 　　なお、オブジェクトは、各プロシージャのメンバー配列として宣言する
// 
// --------------------------------------------------------------------------------------------------
class CBaseObj
{
protected:
	CGameMain*		m_pGMain;			// ゲームメインクラス
	CBaseProc*		m_pProc;			// 親のプロシージャ                     // -- 2022.2.16
	CBaseObj*       m_pObj;             //親のオブジェ
	DWORD			m_dwStatus;			// ステータス
	DWORD			m_dwStatusSub;		// ステータスサブ
	DWORD			m_dwStatusPhase;	// ステータスフェーズ
	BOOL			m_bActive;			// TRUE:表示  FALSE:非表示
	DWORD			m_dwObjID;			// オブジェクトＩＤ
	DWORD			m_dwObjNo;			// オブジェクトＮＯ
	ANIMATION_STATUS m_AnimStatus;		// アニメーションステータス
	CBBox*			m_pBBox;			// バウンディングボックスクラス
	MATRIX4X4		m_mWorldOld;		// ワールドマトリクス（一つ前）
	MATRIX4X4		m_mWorld;			// ワールドマトリクス
	VECTOR3			m_vPosUp;			// 移動増分
	VECTOR3			m_vRotUp;			// 回転増分
	float			m_fJumpY;			// ジャンプ高さ
	float			m_fJumpTime;		// ジャンプ時間
	CBaseObj*		m_pOyaObj;			// 親オブジェクト（武器オブジェ等）
	CBaseObj*		m_pHitObj;			// 当たった相手のオブジェクト
	CBaseObj*       m_pEnmObj;          //敵オブジェクト
	VECTOR3			m_vHitPos;			// 当たった場所の座標
	VECTOR3			m_vHitNormal;		// 当たった場所の法線座標
	int             m_nCnt1;			// カウンター１
	int             m_nCnt2;			// カウンター２
	int				m_nMaxHp;			// 最大体力
	int				m_nHp;				// 体力
	int				m_nAtc;				// 攻撃力

public:
	// オブジェクトの発生処理
	virtual BOOL	Start() { return TRUE; }
	virtual BOOL	Start(VECTOR3 vPos) { return TRUE; }
	virtual BOOL	Start(VECTOR3 vPos, VECTOR3 vNormal) { return TRUE; }
	virtual BOOL	Start(MATRIX4X4 mGun, DWORD dwOwner) { return TRUE; }
	virtual BOOL	Start(VECTOR3 vStart, VECTOR3 vTarget, DWORD dwOwner) { return TRUE; }
	virtual BOOL	Start(MATRIX4X4 mMat, VECTOR3 vMin, VECTOR3 vMax, DWORD dwOwner) { return TRUE; }
	virtual BOOL	Start(MATRIX4X4 mGun, VECTOR3 vOffset, MATRIX4X4 mOwnerWorld, DWORD dwOwner) { return TRUE; }

	// あたり判定処理
	virtual BOOL    Hitcheck(CBaseObj* pOtherObj, VECTOR3 vNow, VECTOR3 vOld);
	virtual BOOL    Hitcheck(CBaseObj* pOtherObj, CBBox* pBBoxAtack);

	// 更新処理
	virtual	void    Update() { ; }

	// 目的地（vTargetX,Z）への移動処理
	// (なお、回転スピードの省略値は3.0fとする)
	virtual	BOOL    TargetMove(VECTOR3 vTarget, float fSpeedIn, float fRotSpeed = 3.0f);

	// オブジェクト同士が重ならないようにするメンバ関数
	virtual	CBaseObj*	SetKeepOffset(CBaseProc* pProj);
	// オブジェクトのmWorldと引数位置との距離を求める
	virtual	float   GetDistance(VECTOR3 vPos);    // -- 2018.8.2
	// ステータスをリセットする
	virtual	void    ResetStatus();      // -- 2019.6.8

	// アクセス関数
	BOOL	GetActive() { return m_bActive; }
	void	SetActive(BOOL bActive) { m_bActive = bActive; }
	CFbxMesh* GetMesh();											// メッシュアドレス    // -- 2022.2.16
	BASEHOLDITEM GetHoldItem();										// 手に持つアイテム    // -- 2022.2.16
	DWORD	GetObjID() { return m_dwObjID; }						// オブジェクトＩＤ    // -- 2022.2.16
	DWORD	GetObjNo() { return m_dwObjNo; }						// オブジェクトＮＯ
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

	// コンストラクタ（基本オブジェクトの初期化）
	CBaseObj(CBaseProc* pProc);
	// デストラクタ
	virtual		~CBaseObj();
};

// --------------------------------------------------------------------------------------------------
// 基本プロシージャクラス
//
//   ＊　プロシージャとは、「複数の処理を一つにまとめたもの」という意味
//       複数のオブジェクトや下位プロシージャをコントロールするクラスのこと
//
// --------------------------------------------------------------------------------------------------
class  CBaseProc
{
protected:
	CGameMain*		m_pGMain;				// ゲームメインクラス

	// 
	// 下位のプロシージャポインタ配列は、プロシージャを階層構造にして管理するときに使用する
	// オブジェクトポインタ配列は、プロシージャの中にオブジェクトを配置して管理するときに使用する
	// 一般的には、どちらか片方のみを使用すること
	// 
	std::vector<CBaseProc*>  m_pProcArray;  // 下位のプロシージャポインタ配列
	std::vector<CBaseObj*>   m_pObjArray;	// オブジェクトポインタ配列

	CFbxMesh*		m_pMesh;				// Fbxメッシュ            // -- 2022.2.16
	BASEHOLDITEM	m_HoldItem;				// 手に持つアイテム       // -- 2022.2.16
	DWORD			m_dwProcID;				// プロシージャＩＤ
	DWORD			m_dwPcProcID;			// PCのプロシージャＩＤ
	DWORD			m_dwEnmProcID;			// 敵のプロシージャＩＤ
	DWORD			m_dwFellowProcID;	    // 仲間のプロシージャＩＤ
	int				m_nMaxNum;				// 最大発生数
	int				m_nNum;					// 発生数
	int				m_nMaxwaitTime;			// 最大待ち時間
	int				m_nWaitTime;			// 現在の待ち時間

public:
	// 全体更新処理（全てのプロシージャやオブジェクトを読み下して更新処理をする）
	virtual void	UpdateAll() final;

	// 更新処理（UpdateAll()の中で起動。プロシージャの更新処理）
	virtual void    Update() { ; }

	// あたり判定（全てのプロシージャやオブジェクトを読み下して処理をする）
	virtual BOOL    Hitcheck(CBaseObj* pOtherObj, VECTOR3 vNow, VECTOR3 vOld) final;
	virtual BOOL    Hitcheck(CBaseObj* pOtherObj, CBBox* pBBoxAtack) final;

	//	プロシージャとオブジェクトを探索し全てのオブジェクトをノンアクティブにする
	virtual void	SetNonActive();

	//	発生数の設定
	virtual void	SetMaxNum() { m_nNum = m_nMaxNum; }

	//	ウェイトタイムの設定
	virtual void	SetMaxWaitTime() { m_nWaitTime = m_nMaxwaitTime; }

	// アクセス関数
	std::vector<CBaseProc*> GetProcArrayPtr() { return m_pProcArray; } // 下位のプロシージャポインタ配列のアドレスを返す
	std::vector<CBaseObj*>  GetObjArrayPtr() { return m_pObjArray; }  // オブジェクトポインタ配列のアドレスを返す
	CBaseObj* GetObj(int i=0) { return m_pObjArray[i]; } // -- 2022.2.16
	CGameMain* GetGMain() { return m_pGMain; }           // -- 2022.2.16
	CFbxMesh* GetMesh() { return m_pMesh; }              // -- 2022.2.16
	BASEHOLDITEM GetHoldItem() { return m_HoldItem; }    // -- 2022.2.16
	DWORD GetProcID() { return m_dwProcID; }             // -- 2022.2.16
	int  GetNum() { return m_nNum; }
	void AddNum(int n) { m_nNum += n; }

	// コンストラクタ（基本プロシージャの初期化）
	CBaseProc(CGameMain* pGMain)
	{
		m_pGMain = pGMain;  // ゲームメインクラス
		m_dwProcID = 0;     // プロシージャＩＤ
		m_nMaxwaitTime = 0;	// 最大待ち時間
		m_nWaitTime = 0;	// 現在の待ち時間
		m_pMesh = NULL;		// Fbxメッシュ            // -- 2022.2.16
		m_nMaxNum = 0;		// 最大発生数
		m_nNum = 0;			// 発生数
	}
	// デストラクタ（プロシージャとオブジェクトの解放）
	virtual		~CBaseProc();
};


