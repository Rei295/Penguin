//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//		タイトル　ヘッダファイル							Title.h
//=============================================================================
#pragma once

#include "GameMain.h"


// --------------------------------------------------------------------
// タイトル　プロシージャクラス
// --------------------------------------------------------------------
class CTitleProc
{
public:
	CGameMain*		m_pGMain;

	CSpriteImage*   m_pImageTitle;
	CSpriteImage*   m_pImageClear;
	CSpriteImage*   m_pImageClearScore;
	CSpriteImage*   m_pImageOver;
	CSprite*        m_pSprite;

	DWORD           m_dwCntTime;


public:
	void Title();
	void GameClear();
	void GameOver();
	CTitleProc(CGameMain* pGMain);	// コンストラクタ
	~CTitleProc();

};
