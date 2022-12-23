//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//		マップ　ヘッダファイル											Map.h
//=============================================================================
#pragma once
#include "GameMain.h"

// --------------------------------------------------------------------
//
// ステージマップ
//
// --------------------------------------------------------------------
struct STAGEMAP
{
	CFbxMesh*				m_pMesh;			// スタティックメッシュへのポインタ
	CWave*					m_pWave;			// 波のメッシュへのポインタ
	MATRIX4X4				m_mWorld;			// メッシュのワールドマトリックス
	int						m_nMaterialFlag;	// マテリアルフラグ　0:通常　1:透明色　2:ディスプレイスメントマッピング 3:波

	STAGEMAP()    // コンストラクタ
	{
		Init();
	}
	~STAGEMAP()    // デストラクタ
	{
		// この中でメッシュのDELETEをしてはならない
	}
	void Init()    // 初期化メソッド
	{
		m_pMesh = NULL;
		m_pWave = NULL;
		m_mWorld = XMMatrixIdentity();
		m_nMaterialFlag = 0;
	}
};
// --------------------------------------------------------------------
//
// 移動マップ            ＊このプログラム中では、使用しない
//
// --------------------------------------------------------------------
struct MOVEMAP
{
	BOOL					m_bActive;			// 移動Active
	CFbxMesh*				m_pMesh;			// スタティックメッシュへのポインタ
	int						m_nMaterialFlag;	// マテリアルフラグ　0:通常　1:透明色　2:ディスプレイスメントマッピング
	CCollision*				m_pColMesh;			// 移動コリジョンメッシュへのポインタ
	int						m_nMoveFlag;		// 移動フラグ　0:移動無し　1:平行移動　2:回転　3:拡大縮小      // -- 2019.12.30
	VECTOR3					m_vUp;				// 移動増分
	VECTOR3					m_vMin;				// 移動最小値
	VECTOR3					m_vMax;				// 移動最大値
	int						m_nChangeFlag;		// 移動が限界値に到達して方向が入れ替わるとき　0:移動中　1:方向チェンジ
	int						m_nLoop;			// 移動を繰り返すか  0:チェンジの箇所で停止し移動フラグを0:移動無しに戻す。 1:移動を繰り返す  // -- 2021.2.4

	MOVEMAP()    // コンストラクタ
	{
		Init();
	}
	~MOVEMAP()    // デストラクタ
	{
		// この中でメッシュのDELETEをしてはならない
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
		m_nLoop = 1;           // 初期設定はループ        // -- 2021.2.4
	}
};

// --------------------------------------------------------------------
//
// ナビゲーションマップ
//
// --------------------------------------------------------------------
struct NAVIGATIONMAP
{
	VECTOR3 vMin;
	VECTOR3 vMax;
	std::vector<DWORD>  m_dwEnmID;  // 対象となる敵のＩＤ
	std::vector<DWORD>  m_dwFellowID;  // 対象となる仲間のＩＤ
	NAVIGATIONMAP()    // コンストラクタ
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
// イベント
//
// --------------------------------------------------------------------
// イベント種類
enum EVENTKIND
{
	eEvtKindNone = 0,
	eEvtChangeMap = 1,
	eEvtMoveMap = 2
};

struct EVENTMAP
{
	CBBox*    m_pBBox;       // バウンディングボックス
	int       m_nEvtCycle;   // イベントサイクル 0:イベント実行していない 1:イベント実行中（使用していない） 2:イベント終了チェック（接触終わったか）
	EVENTKIND m_nEvtKind;    // イベントの種類 eEvtChangeMap:マップ移動, eEvtMoveMap:移動マップの制御
	int       m_nEvtNo;      // イベント番号 eEvtChangeMapのときは移動先のマップ番号、eEvtMoveMapのときは移動マップの要素番号
	int       m_nEvtOpe1;    // eEvtMoveMapのみ。1:移動マップの移動Activeの指定。2:移動マップの移動フラグの指定
	int       m_nEvtOpe2;    // eEvtMoveMapのみ。移動Activeのときは0:非表示 1:表示  移動フラグのときは　0:移動無し　1:平行移動　2:回転　3:拡大縮小
	int       m_nEvtKeyPush; // 0:バウンディングボックスに接触するとイベント発動、 1:バウンディングボックスに接触して、ENTERキーを押すとイベント発動

	EVENTMAP()    // コンストラクタ
	{
		m_pBBox = NULL;
		m_nEvtCycle = 0;
		m_nEvtKind = eEvtKindNone;
		m_nEvtNo = 0;
		m_nEvtOpe1 = 0;
		m_nEvtOpe2 = 0;
		m_nEvtKeyPush = 0;
	}
	~EVENTMAP()    // デストラクタ
	{
		// この中でメッシュのCBBoxのDELETEをしてはならない
	}
};

//======================================================================
// マップ　プロシージャクラス
//
// (注意)マップ　プロシージャクラスは、基本プロシージャクラスを継承していない
// 
//======================================================================
class CMapProc
{
protected:
	CGameMain*							m_pGMain;

	DWORD								m_dwMapNo;
	BOOL								m_bActive;

	std::vector<STAGEMAP>				m_SkyMap;			// 空のマップ
	std::vector<STAGEMAP>				m_StageMap;			// ステージのマップ
	CCollision*							m_pColMesh;			// コリジョンメッシュ
	std::vector<MOVEMAP>				m_MoveMap;			// 移動マップ  *このプログラム中では、使用しない
	std::vector<NAVIGATIONMAP>			m_NavMap;			// ナビゲーションマップ
	std::vector<EVENTMAP>				m_EventMap;			// イベントマップ    // -- 2021.2.4
	int									m_nEvtIdx;			// イベント要素番号  // -- 2021.2.4

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

	// アクセス関数
	std::vector<NAVIGATIONMAP> GetNavMap() { return m_NavMap; }
	DWORD GetMapNo() { return m_dwMapNo; }

	// コンストラクタ（プロシージャの初期化）
	CMapProc(CGameMain*	m_pGMain);
	~CMapProc();
};