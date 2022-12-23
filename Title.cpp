//=============================================================================
//		‚R‚cƒQ[ƒ€ƒvƒƒOƒ‰ƒ€
//		‚b‚R‚c‚`‚ƒ‚”‚P‚P‚Q                             ver 3.1        2022.2.16
//		ƒ^ƒCƒgƒ‹EƒQ[ƒ€ƒNƒŠƒ„[EƒQ[ƒ€ƒI[ƒo[‚Ìˆ—
//																Title.cpp
//=============================================================================
#include  "Title.h"
#include  "Map.h"
#include  "Playchar.h"


//============================================================================
//
// ƒ^ƒCƒgƒ‹ƒvƒƒV[ƒWƒƒ‚ÌƒRƒ“ƒXƒgƒ‰ƒNƒ^
//
// ---------------------------------------------------------------------------
CTitleProc::CTitleProc(CGameMain* pGMain)
{
	m_pGMain = pGMain;

	m_pImageTitle = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/TITLE.png")); // ƒCƒ[ƒW‚Ìƒ[ƒh
	m_pImageClear = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/CLEAR.png")); // ƒCƒ[ƒW‚Ìƒ[ƒh
	m_pImageClearScore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/ClearScore.png")); // ƒCƒ[ƒW‚Ìƒ[ƒh
	m_pImageOver  = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/OVER.png")); // ƒCƒ[ƒW‚Ìƒ[ƒh
	m_pSprite = new CSprite(m_pGMain->m_pShader); // ƒXƒvƒ‰ƒCƒg‚Ì¶¬

	m_dwCntTime = 0;
}
// ---------------------------------------------------------------------------
//
// ƒ^ƒCƒgƒ‹ƒ^ƒCƒgƒ‹ƒvƒƒV[ƒWƒƒ‚ÌƒfƒXƒgƒ‰ƒNƒ^
//
// ---------------------------------------------------------------------------
CTitleProc::~CTitleProc()
{

	SAFE_DELETE(m_pImageTitle);
	SAFE_DELETE(m_pImageClear);
	SAFE_DELETE(m_pImageClearScore);
	SAFE_DELETE(m_pImageOver);
	SAFE_DELETE(m_pSprite);

}
//------------------------------------------------------------------------
//
//	ƒ^ƒCƒgƒ‹‚Ì•\Ž¦	
//
//------------------------------------------------------------------------
void CTitleProc::Title()
{

	// ƒtƒ‹ƒXƒNƒŠ[ƒ“‚É‚·‚é(‚È‚º‚©1“x‚¾‚¯‚¾‚Æ–ß‚Á‚Ä‚µ‚Ü‚¤‚Ì‚ÅAÄ“xƒtƒ‹ƒXƒNƒŠ[ƒ“‚É‚·‚é)
	//m_pGMain->ChangeScreenMode(1);

	// •\Ž¦
	m_pSprite->Draw(m_pImageTitle, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	// ƒtƒHƒ“ƒg–¼@"_T(‚c‚eŠ¨’à—¬")C_T("‚l‚r ‚oƒSƒVƒbƒN")C_T("‚l‚r ƒSƒVƒbƒN")C_T("HGP‘n‰pŠpÎß¯Ìß‘Ì")C_T("HGP‘n‰pŠpºÞ¼¯¸UB")
	m_pGMain->m_pFont->Draw(253, 53, _T("‚R‚cƒAƒNƒVƒ‡ƒ“ƒQ[ƒ€"), 90, RGB(0, 0, 0), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));
	m_pGMain->m_pFont->Draw(250, 50, _T("‚R‚cƒAƒNƒVƒ‡ƒ“ƒQ[ƒ€"), 90, RGB(0, 0, 255), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));

	m_pGMain->m_pFont->Draw(WINDOW_WIDTH - 200, 20, _T("C3DAct112"), 30, RGB(0, 0, 192), 1.0f, _T("HGP‘n‰pŠpºÞ¼¯¸UB"));
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH-250, 130, _T("DirectX11‘Î‰ž"), 30, RGB(0, 0, 64), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 60, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) ||  m_dwCntTime > 600 )	//
	{
		// ƒTƒEƒ“ƒh‚ÌÄ¶
		m_pGMain->m_pBgm1->Play(AUDIO_LOOP);

		// Å‰‚Íƒ}ƒbƒv‚P‚Ìƒ[ƒh
		m_pGMain->m_pMapProc->LoadMap1();

		m_pGMain->m_dwGameStatus = GAMEMAIN;

		m_dwCntTime = 0;

	}

}
//------------------------------------------------------------------------
//
//	ƒQ[ƒ€ƒNƒŠƒ„[‚Ì•\Ž¦	
//
//------------------------------------------------------------------------
void CTitleProc::GameClear()
{

	// •\Ž¦
	m_pSprite->Draw(m_pImageClear, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGMain->m_pFont->Draw(353, 83, _T("ƒQ[ƒ€ƒNƒŠƒ„["), 90, RGB(0, 0, 0), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));
	m_pGMain->m_pFont->Draw(350, 80, _T("ƒQ[ƒ€ƒNƒŠƒ„["), 90, RGB(255, 0, 0), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));

	// ƒXƒRƒA‚Ì•\Ž¦
	m_pSprite->Draw(m_pImageClearScore, WINDOW_WIDTH / 2 - 128, WINDOW_HEIGHT / 2 - 108, 0, 0, 256, 256);
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 128 + 60, WINDOW_HEIGHT / 2 - 108 + 17, _T("Score"), 36, RGB(255, 255, 255), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));
	TCHAR str[256];
	_stprintf(str, _T("%06d"), m_pGMain->m_nScore);
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 128 + 50, WINDOW_HEIGHT / 2 - 108 + 140, str, 36, RGB(255, 255, 255), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 120, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) ||  m_dwCntTime > 600 )	//
	{
		m_pGMain->m_pBgm1->Stop();

		m_pGMain->m_dwGameStatus = GAMEEND;

		m_dwCntTime = 0;

	}

}
//------------------------------------------------------------------------
//
//	ƒQ[ƒ€ƒI[ƒo[‚Ì•\Ž¦	
//
//------------------------------------------------------------------------
void CTitleProc::GameOver()
{

	// •\Ž¦
	m_pSprite->Draw(m_pImageOver, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGMain->m_pFont->Draw(353, 83, _T("ƒQ[ƒ€ƒI[ƒo["), 90, RGB(0, 0, 0), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));
	m_pGMain->m_pFont->Draw(350, 80, _T("ƒQ[ƒ€ƒI[ƒo["), 90, RGB(255, 0, 0), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 120, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP‘n‰pŠpÎß¯Ìß‘Ì"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) ||  m_dwCntTime > 600 )	//
	{
		m_pGMain->m_pBgm1->Stop();

		m_pGMain->m_dwGameStatus = GAMEEND;

		m_dwCntTime = 0;

	}

}