//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//		タイトル・ゲームクリヤー・ゲームオーバーの処理
//																Title.cpp
//=============================================================================
#include  "Title.h"
#include  "Map.h"
#include  "Playchar.h"


//============================================================================
//
// タイトルプロシージャのコンストラクタ
//
// ---------------------------------------------------------------------------
CTitleProc::CTitleProc(CGameMain* pGMain)
{
	m_pGMain = pGMain;

	m_pImageTitle = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/TITLE.png")); // イメージのロード
	m_pImageClear = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/CLEAR.png")); // イメージのロード
	m_pImageClearScore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/ClearScore.png")); // イメージのロード
	m_pImageOver  = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/OVER.png")); // イメージのロード
	m_pSprite = new CSprite(m_pGMain->m_pShader); // スプライトの生成

	m_dwCntTime = 0;
}
// ---------------------------------------------------------------------------
//
// タイトルタイトルプロシージャのデストラクタ
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
//	タイトルの表示	
//
//------------------------------------------------------------------------
void CTitleProc::Title()
{

	// フルスクリーンにする(なぜか1度だけだと戻ってしまうので、再度フルスクリーンにする)
	//m_pGMain->ChangeScreenMode(1);

	// 表示
	m_pSprite->Draw(m_pImageTitle, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	// フォント名　"_T(ＤＦ勘亭流")，_T("ＭＳ Ｐゴシック")，_T("ＭＳ ゴシック")，_T("HGP創英角ﾎﾟｯﾌﾟ体")，_T("HGP創英角ｺﾞｼｯｸUB")
	m_pGMain->m_pFont->Draw(253, 53, _T("３Ｄアクションゲーム"), 90, RGB(0, 0, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	m_pGMain->m_pFont->Draw(250, 50, _T("３Ｄアクションゲーム"), 90, RGB(0, 0, 255), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));

	m_pGMain->m_pFont->Draw(WINDOW_WIDTH - 200, 20, _T("C3DAct112"), 30, RGB(0, 0, 192), 1.0f, _T("HGP創英角ｺﾞｼｯｸUB"));
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH-250, 130, _T("DirectX11対応"), 30, RGB(0, 0, 64), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 60, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) ||  m_dwCntTime > 600 )	//
	{
		// サウンドの再生
		m_pGMain->m_pBgm1->Play(AUDIO_LOOP);

		// 最初はマップ１のロード
		m_pGMain->m_pMapProc->LoadMap1();

		m_pGMain->m_dwGameStatus = GAMEMAIN;

		m_dwCntTime = 0;

	}

}
//------------------------------------------------------------------------
//
//	ゲームクリヤーの表示	
//
//------------------------------------------------------------------------
void CTitleProc::GameClear()
{

	// 表示
	m_pSprite->Draw(m_pImageClear, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGMain->m_pFont->Draw(353, 83, _T("ゲームクリヤー"), 90, RGB(0, 0, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	m_pGMain->m_pFont->Draw(350, 80, _T("ゲームクリヤー"), 90, RGB(255, 0, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));

	// スコアの表示
	m_pSprite->Draw(m_pImageClearScore, WINDOW_WIDTH / 2 - 128, WINDOW_HEIGHT / 2 - 108, 0, 0, 256, 256);
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 128 + 60, WINDOW_HEIGHT / 2 - 108 + 17, _T("Score"), 36, RGB(255, 255, 255), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	TCHAR str[256];
	_stprintf(str, _T("%06d"), m_pGMain->m_nScore);
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 128 + 50, WINDOW_HEIGHT / 2 - 108 + 140, str, 36, RGB(255, 255, 255), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 120, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
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
//	ゲームオーバーの表示	
//
//------------------------------------------------------------------------
void CTitleProc::GameOver()
{

	// 表示
	m_pSprite->Draw(m_pImageOver, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGMain->m_pFont->Draw(353, 83, _T("ゲームオーバー"), 90, RGB(0, 0, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	m_pGMain->m_pFont->Draw(350, 80, _T("ゲームオーバー"), 90, RGB(255, 0, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 120, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP創英角ﾎﾟｯﾌﾟ体"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) ||  m_dwCntTime > 600 )	//
	{
		m_pGMain->m_pBgm1->Stop();

		m_pGMain->m_dwGameStatus = GAMEEND;

		m_dwCntTime = 0;

	}

}