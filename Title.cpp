//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//		�^�C�g���E�Q�[���N�����[�E�Q�[���I�[�o�[�̏���
//																Title.cpp
//=============================================================================
#include  "Title.h"
#include  "Map.h"
#include  "Playchar.h"


//============================================================================
//
// �^�C�g���v���V�[�W���̃R���X�g���N�^
//
// ---------------------------------------------------------------------------
CTitleProc::CTitleProc(CGameMain* pGMain)
{
	m_pGMain = pGMain;

	m_pImageTitle = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/TITLE.png")); // �C���[�W�̃��[�h
	m_pImageClear = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/CLEAR.png")); // �C���[�W�̃��[�h
	m_pImageClearScore = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/ClearScore.png")); // �C���[�W�̃��[�h
	m_pImageOver  = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/OVER.png")); // �C���[�W�̃��[�h
	m_pSprite = new CSprite(m_pGMain->m_pShader); // �X�v���C�g�̐���

	m_dwCntTime = 0;
}
// ---------------------------------------------------------------------------
//
// �^�C�g���^�C�g���v���V�[�W���̃f�X�g���N�^
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
//	�^�C�g���̕\��	
//
//------------------------------------------------------------------------
void CTitleProc::Title()
{

	// �t���X�N���[���ɂ���(�Ȃ���1�x�������Ɩ߂��Ă��܂��̂ŁA�ēx�t���X�N���[���ɂ���)
	//m_pGMain->ChangeScreenMode(1);

	// �\��
	m_pSprite->Draw(m_pImageTitle, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);

	// �t�H���g���@"_T(�c�e������")�C_T("�l�r �o�S�V�b�N")�C_T("�l�r �S�V�b�N")�C_T("HGP�n�p�p�߯�ߑ�")�C_T("HGP�n�p�p�޼��UB")
	m_pGMain->m_pFont->Draw(253, 53, _T("�R�c�A�N�V�����Q�[��"), 90, RGB(0, 0, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	m_pGMain->m_pFont->Draw(250, 50, _T("�R�c�A�N�V�����Q�[��"), 90, RGB(0, 0, 255), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));

	m_pGMain->m_pFont->Draw(WINDOW_WIDTH - 200, 20, _T("C3DAct112"), 30, RGB(0, 0, 192), 1.0f, _T("HGP�n�p�p�޼��UB"));
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH-250, 130, _T("DirectX11�Ή�"), 30, RGB(0, 0, 64), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 60, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) ||  m_dwCntTime > 600 )	//
	{
		// �T�E���h�̍Đ�
		m_pGMain->m_pBgm1->Play(AUDIO_LOOP);

		// �ŏ��̓}�b�v�P�̃��[�h
		m_pGMain->m_pMapProc->LoadMap1();

		m_pGMain->m_dwGameStatus = GAMEMAIN;

		m_dwCntTime = 0;

	}

}
//------------------------------------------------------------------------
//
//	�Q�[���N�����[�̕\��	
//
//------------------------------------------------------------------------
void CTitleProc::GameClear()
{

	// �\��
	m_pSprite->Draw(m_pImageClear, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGMain->m_pFont->Draw(353, 83, _T("�Q�[���N�����["), 90, RGB(0, 0, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	m_pGMain->m_pFont->Draw(350, 80, _T("�Q�[���N�����["), 90, RGB(255, 0, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));

	// �X�R�A�̕\��
	m_pSprite->Draw(m_pImageClearScore, WINDOW_WIDTH / 2 - 128, WINDOW_HEIGHT / 2 - 108, 0, 0, 256, 256);
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 128 + 60, WINDOW_HEIGHT / 2 - 108 + 17, _T("Score"), 36, RGB(255, 255, 255), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	TCHAR str[256];
	_stprintf(str, _T("%06d"), m_pGMain->m_nScore);
	m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 128 + 50, WINDOW_HEIGHT / 2 - 108 + 140, str, 36, RGB(255, 255, 255), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 120, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
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
//	�Q�[���I�[�o�[�̕\��	
//
//------------------------------------------------------------------------
void CTitleProc::GameOver()
{

	// �\��
	m_pSprite->Draw(m_pImageOver, 0, 0, 0, 0, m_pImageTitle->m_dwImageWidth, m_pImageTitle->m_dwImageHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pGMain->m_pFont->Draw(353, 83, _T("�Q�[���I�[�o�["), 90, RGB(0, 0, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	m_pGMain->m_pFont->Draw(350, 80, _T("�Q�[���I�[�o�["), 90, RGB(255, 0, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));

	m_dwCntTime++;
	if (m_dwCntTime % 10 > 5)
	{
		m_pGMain->m_pFont->Draw(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 120, _T("HIT SPACE KEY!!"), 36, RGB(0, 255, 0), 1.0f, _T("HGP�n�p�p�߯�ߑ�"));
	}

	if (m_pGMain->m_pDI->CheckKey(KD_TRG, DIK_SPACE) || m_pGMain->m_pDI->CheckJoy(KD_DAT, DIJ_A) || m_pGMain->m_pDI->CheckMouse(KD_DAT, DIM_LBUTTON) ||  m_dwCntTime > 600 )	//
	{
		m_pGMain->m_pBgm1->Stop();

		m_pGMain->m_dwGameStatus = GAMEEND;

		m_dwCntTime = 0;

	}

}