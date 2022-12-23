//=============================================================================
//		�R�c�Q�[���v���O����
//		�b�R�c�`�����P�P�Q                             ver 3.1        2022.2.16
//		�^�C�g���@�w�b�_�t�@�C��							Title.h
//=============================================================================
#pragma once

#include "GameMain.h"


// --------------------------------------------------------------------
// �^�C�g���@�v���V�[�W���N���X
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
	CTitleProc(CGameMain* pGMain);	// �R���X�g���N�^
	~CTitleProc();

};
