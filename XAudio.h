// =========================================================================================
//
//  ＸＡｕｄｉｏ２　
//																   ver 2.8        2019.2.25
// =========================================================================================

#pragma once

#define _WIN32_DCOM
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <tchar.h>

//マクロ定義
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(p){ if(p){ delete[] (p);   (p)=NULL;}}

#define AUDIO_LOOP         1          // ループ再生
#define AUDIO_SOURCE_MAX  10          // 同時に再生する同一オーディオソースの最大数

// 
//  XAudio マスタークラス	
//  アプリに一つ必要
// 
class CXAudio
{
public:
	// メンバ変数
	HWND                    m_hWnd;
	IXAudio2*               m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;

	// メソッド
	HRESULT     InitAudio(HWND);

	CXAudio();
	~CXAudio();

};
// 
//  XAudio ソースボイスクラス	
//  一つのサウンド（ＷＡＶ）に一つ必要
// 
class CXAudioSource
{
public:
	CXAudio*				m_pXAudio;			// XAudio マスタークラスアドレス	
	BOOL					m_bWav;				// ソースがＷａｖｅファイルか　TRUE:XAudio(WAV)　FALSE:MCI(MP3やMID) 
	TCHAR                   m_szAliasName[256]; // Mciの別名が入る

	DWORD                   m_dwSourceIndex;					//	オーディオソースインデックス
	DWORD                   m_dwSourceNum;						//	オーディオソースの個数	
	IXAudio2SourceVoice*    m_pSourceVoice[AUDIO_SOURCE_MAX];  // オーディオソース
	BYTE*                   m_pWavBuffer[AUDIO_SOURCE_MAX];    // 波形データ（フォーマット等を含まない、純粋に波形データのみ）
	DWORD                   m_dwWavSize[AUDIO_SOURCE_MAX];     // 波形データのサイズ

	HRESULT      Load(TCHAR* szFileName, DWORD dwNum=1);
	HRESULT      LoadAudio(TCHAR* szFileName, DWORD dwNum);
	HRESULT      LoadAudioSub(TCHAR* szFileName, DWORD dwIndex);
	HRESULT      LoadMci(TCHAR* szFileName);
	void         Play(int loop=0);
	void         PlayAudio(int loop=0);
	void         PlayMci(int loop=0);
	void         Stop();
	void         StopAudio();
	void         StopMci();
	void         Volume(float fVol);
	void         VolumeAudio(float fVol);
	void         VolumeMci(int nVol);
	
	CXAudioSource(CXAudio*	pXAudio);
	CXAudioSource(CXAudio*	pXAudio, TCHAR* szFileName, DWORD dwNum=1);
	~CXAudioSource();

};