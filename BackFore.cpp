//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２　　　　　　                 ver 3.0        2021.1.11
//		前景・背景							                   BackFore.cpp
//=============================================================================
#include  "BackFore.h"
#include  "Playchar.h"


//============================================================================
//
// 前景・背景プロシージャのコンストラクタ
//
// ---------------------------------------------------------------------------
CBackForeProc::CBackForeProc(CGameMain* pGMain)
{
	m_pGMain		= pGMain;
	m_pImageForegrd = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/foregrd3.png")); // スプライトイメージのロード
	m_pSprite       = new CSprite(m_pGMain->m_pShader); // スプライトの生成

}
// ---------------------------------------------------------------------------
//
// 前景・背景プロシージャのデストラクタ
//
// ---------------------------------------------------------------------------
CBackForeProc::~CBackForeProc()
{

	SAFE_DELETE(m_pImageForegrd);
	SAFE_DELETE(m_pSprite);

}
//------------------------------------------------------------------------
//
//	前景・背景プロシージャの更新	
//
//------------------------------------------------------------------------
void CBackForeProc::Update()
{
	const int nScoreMax = 100;   // スコアバーの表示上、スコアの最大値を100点と仮定する
	float h = 0;
	TCHAR str[256] = { 0 };
	int   DestX, DestY;

	// ステータスバーの表示
	h = (float)m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() / m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp();    // -- 2022.2.16
	if (h < 0) h = 0;

	DestX = 10;
	DestY = 10;
	m_pSprite->Draw(m_pImageForegrd, DestX, DestY, 0, 0, 213, 31);
	m_pSprite->Draw(m_pImageForegrd, DestX + 59, DestY + 6, 59, 32, (DWORD)(144 * h), 6);

	// ＨＰとＰＣ残数の表示
	_stprintf_s(str, _T("%d"), m_pGMain->m_pPcProc->GetNum());
	m_pGMain->m_pFont->Draw(DestX + 6, DestY + 15, str, 16, RGB(255, 0, 0));
	_stprintf_s(str, _T("%06d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp());
	m_pGMain->m_pFont->Draw(DestX + 26, DestY + 16, str, 12, RGB(0, 0, 0));

	// --------------------------------------------------------------------------------
	// スコア枠の表示
	DestX = WINDOW_WIDTH - 213 - 10;
	DestY = 10;
	m_pSprite->Draw(m_pImageForegrd, DestX, DestY, 0, 112, 213, 28);
	// スコアバーの表示
	h = (float)m_pGMain->m_nScore / nScoreMax;
	if (h < 0) h = 0;
	if (h > 1) h = 1;
	m_pSprite->Draw(m_pImageForegrd, DestX + 59, DestY + 6, 59, 142, (DWORD)(144 * h), 6);
	// スコア数値の表示
	_stprintf(str, _T("%06d"), m_pGMain->m_nScore);
	m_pGMain->m_pFont->Draw(DestX + 10, DestY + 17, str, 11, RGB(0, 0, 0));

};