//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q�@�@�@�@�@�@                 ver 3.0        2021.1.11
//		�O�i�E�w�i							                   BackFore.cpp
//=============================================================================
#include  "BackFore.h"
#include  "Playchar.h"


//============================================================================
//
// �O�i�E�w�i�v���V�[�W���̃R���X�g���N�^
//
// ---------------------------------------------------------------------------
CBackForeProc::CBackForeProc(CGameMain* pGMain)
{
	m_pGMain		= pGMain;
	m_pImageForegrd = new CSpriteImage(m_pGMain->m_pShader, _T("Data/Image/foregrd3.png")); // �X�v���C�g�C���[�W�̃��[�h
	m_pSprite       = new CSprite(m_pGMain->m_pShader); // �X�v���C�g�̐���

}
// ---------------------------------------------------------------------------
//
// �O�i�E�w�i�v���V�[�W���̃f�X�g���N�^
//
// ---------------------------------------------------------------------------
CBackForeProc::~CBackForeProc()
{

	SAFE_DELETE(m_pImageForegrd);
	SAFE_DELETE(m_pSprite);

}
//------------------------------------------------------------------------
//
//	�O�i�E�w�i�v���V�[�W���̍X�V	
//
//------------------------------------------------------------------------
void CBackForeProc::Update()
{
	const int nScoreMax = 100;   // �X�R�A�o�[�̕\����A�X�R�A�̍ő�l��100�_�Ɖ��肷��
	float h = 0;
	TCHAR str[256] = { 0 };
	int   DestX, DestY;

	// �X�e�[�^�X�o�[�̕\��
	h = (float)m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp() / m_pGMain->m_pPcProc->GetPcObjPtr()->GetMaxHp();    // -- 2022.2.16
	if (h < 0) h = 0;

	DestX = 10;
	DestY = 10;
	m_pSprite->Draw(m_pImageForegrd, DestX, DestY, 0, 0, 213, 31);
	m_pSprite->Draw(m_pImageForegrd, DestX + 59, DestY + 6, 59, 32, (DWORD)(144 * h), 6);

	// �g�o�Ƃo�b�c���̕\��
	_stprintf_s(str, _T("%d"), m_pGMain->m_pPcProc->GetNum());
	m_pGMain->m_pFont->Draw(DestX + 6, DestY + 15, str, 16, RGB(255, 0, 0));
	_stprintf_s(str, _T("%06d"), m_pGMain->m_pPcProc->GetPcObjPtr()->GetHp());
	m_pGMain->m_pFont->Draw(DestX + 26, DestY + 16, str, 12, RGB(0, 0, 0));

	// --------------------------------------------------------------------------------
	// �X�R�A�g�̕\��
	DestX = WINDOW_WIDTH - 213 - 10;
	DestY = 10;
	m_pSprite->Draw(m_pImageForegrd, DestX, DestY, 0, 112, 213, 28);
	// �X�R�A�o�[�̕\��
	h = (float)m_pGMain->m_nScore / nScoreMax;
	if (h < 0) h = 0;
	if (h > 1) h = 1;
	m_pSprite->Draw(m_pImageForegrd, DestX + 59, DestY + 6, 59, 142, (DWORD)(144 * h), 6);
	// �X�R�A���l�̕\��
	_stprintf(str, _T("%06d"), m_pGMain->m_nScore);
	m_pGMain->m_pFont->Draw(DestX + 10, DestY + 17, str, 11, RGB(0, 0, 0));

};