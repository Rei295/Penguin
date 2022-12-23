//-----------------------------------------------------------------------------
//
//  Direct3Dを利用するためのライブラリ                 ver 3.0        2021.1.11
// 
//	①　Direct3Dの初期化	
//	②　テクスチャーサンプラーとブレンドステートの作成
//	③　イメージファイルを読み込んでテクスチャを作成する関数
//	④　コンパイル済みシェーダーの読み込みをする関数
//	⑤　レンダーターゲットの設定をする関数
//
//
//																Direct3D.h
//
//-----------------------------------------------------------------------------

#pragma once

//警告非表示
#pragma warning(disable : 4005)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)
#pragma warning(disable : 4838)


//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dCompiler.h>

#include <d2d1.h>
#include <wincodec.h>

#include "MyMath.h"

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
//#pragma comment(lib, "d2d1.lib")
//#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//マクロ定義
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(p){ if(p){ delete[] (p);   (p)=NULL;}}

//マクロ
#define ALIGN16 _declspec(align(16))

#define MAX_BONES 255

// FBX ローダー用 -------------- // -- 2017.7.17 --------------------------
// モデルデータ用
#define ANIMATION_MAX 50		// 最大アニメーション数

// テクスチャ
#define TEXTURE_MAX 32			// 1モデルにつきテクスチャ32枚まで

// モデルデータにつきバウンディングボックス30個まで
#define BOUNDINGBOX_MAX 30
// -------------------------------------------------------------------------

// アンチエイリアス ------------ // -- 2018.8.3 ---------------------------
// アンチエイリアスをかけないとき
#define SAMPLE_COUNT    1
#define SAMPLE_QUALITY  0

// アンチエイリアスをかけるとき
//#define SAMPLE_COUNT    4
//#define SAMPLE_QUALITY  0.5


// アニメーション再生情報構造体
struct ANIMATION_STATUS {
	bool   playAnim;    // 再生するか
	int    animNum;     // 再生中のアニメーション番号
	int    animNumOld;  // 前のフレームのアニメーション番号
	int    endFrame;    // 再生中のアニメーションの最終フレーム
	int    animFrame;   // 現在のフレーム数
	bool   endFlag;     // 最終フレームに達したか
	bool   animLock;    // アニメーションをロックする
	ANIMATION_STATUS()
	{
		playAnim = false;
		animNum = 0;
		animNumOld = 0;
		endFrame = 0;
		animFrame = 0;
		endFlag = false;
		animLock = false;
	}
	// アニメーション番号の設定
	void SetNum(int num)
	{
		if (animLock) return;
		if (animNum != num) endFlag = false;
		animNum = num;
	}
	// アニメーションの最終フレームに達したか
	bool isEnd(int num)
	{
		if (animNum == num && endFlag) return true;
		else  return false;
	}
};


// Direct3Dクラス
class CDirect3D
{
public:
	// メンバ変数
	HWND  m_hWnd;
	DWORD m_dwWindowWidth;
	DWORD m_dwWindowHeight;

	// Dx11
	ID3D11Device*           m_pDevice;
	ID3D11DeviceContext*    m_pDeviceContext;
	IDXGISwapChain*         m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D*        m_pBackBuffer_DSTex;

	// ブレンドステート
	ID3D11BlendState*         m_pBlendStateNormal;
	ID3D11BlendState*         m_pBlendStateTrapen;
	ID3D11BlendState*         m_pBlendStateAdd;

	//テクスチャーのサンプラー
	ID3D11SamplerState*       m_pSampleLinear;
	ID3D11SamplerState*       m_pSampleBorder;

	// COM オブジェクト(CLSID_WICImagingFactory)
	IWICImagingFactory*       m_pFactory;    // -- 2018.12.26

	// ラスタライザステート
	ID3D11RasterizerState*    m_pRStateL;  // 左回りを表面とする
	ID3D11RasterizerState*    m_pRStateLW;  // 左回りを表面とする。ワイヤーフレーム表示

	ID3D11RenderTargetView*   m_pTarget_TexRTV;   // 現在のレンダーターゲット   // -- 2019.4.19
	ID3D11DepthStencilView*   m_pTarget_DSTexDSV; // 現在のレンダーターゲット   // -- 2019.4.19

	// ---------------------------------------------------------------
	// メソッド
	CDirect3D();
	~CDirect3D();
	HRESULT InitD3D(HWND, DWORD, DWORD );
	HRESULT InitBlendState();
	HRESULT InitSampler();
	HRESULT ReadCso(const TCHAR* csoName, BYTE** ppByteArray, DWORD* pCsoSize);
	HRESULT CreateTextureFromFile(const TCHAR* TName, ID3D11Texture2D** ppTexture2D, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels = 1);
	HRESULT MakeNoMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, ID3D11Texture2D**	ppTexture2D);
	HRESULT MakeMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, const UINT& MipLevels, ID3D11Texture2D**	ppTexture2D);
	HRESULT CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels = 1);
	HRESULT CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, UINT MipLevels = 1);

	void SetRenderTarget(ID3D11RenderTargetView* pTexRTV, ID3D11DepthStencilView* pDSTexDSV);  // -- 2019.4.19
	void ClearRenderTarget(float ClearColor[]);                                                // -- 2019.4.19
	void SetZBuffer(BOOL bZBuf);                                                               // -- 2019.4.19

	// --------------------------------------------------
	void DestroyD3D();

};
