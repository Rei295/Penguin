//=============================================================================
//		３Ｄゲームプログラム
//		Ｃ３ＤＡｃｔ１１２                             ver 3.1        2022.2.16
//
//		エフェクトの処理
//										2021.1.11					Effect.h
//=============================================================================

#pragma once
#include "GameMain.h"

#define  PARTICLE_MAX   50
#define  BILLBOARD_MAX  20
#define  BILLFIRE_MAX   500

// 
class CDirect3D;
// ========================================================================================
//
//	パーティクル
//（ポイントスプライトでパーティクルを実現）
//
// ========================================================================================
//
// パーティクル構造体
//
struct PART
{
	VECTOR3 Pos;
	VECTOR3 Dir;
	float       Speed;
	int         BirthFrame;
	PART()
	{
		ZeroMemory(this, sizeof(PART));
	}
};
//
// 頂点の構造体
//
struct PARTICLE_VERTEX
{
	VECTOR3 Pos; //位置
};
struct BILLBOARD_VERTEX
{
	VECTOR3 Pos; //位置
	VECTOR2 UV; //テクスチャー座標
};

// -----------------------------------------------------------------------------
//	パーティクルのオブジェクトクラス
//（ポイントスプライトでパーティクルを実現）
//
// ----------------------------------------------------------------------------
class CParticleObj : public CBaseObj
{
protected:
	// 
	CDirect3D*	   m_pD3D;
	CShader*       m_pShader;
	FLOAT          m_fAlpha;
	float          m_fSize;  // パーティクルのサイズ // -- 2018.8.23
	int            m_MaxParticle;
	VECTOR3        m_vEmitPos;
	PART*          m_pPartArray;
	int            m_Frame;
	int            m_FrameEnd;
	float          m_fSpeed;  // パーティクルの移動スピード
	int            m_iBarthFrame;     // パーティクルの移動開始までの最大待ち時間（ランダム）

	ID3D11Buffer*  m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（パーティクルプロシージャのテクスチャアドレスをセットするだけ）

public:	
	// 
	CParticleObj(CBaseProc*  pProc, int MaxParticle );      // -- 2022.2.16
	virtual	~CParticleObj();

	void Init(int MaxParticle);
	HRESULT SetSrc();

	BOOL Start(VECTOR3 vEmitPos, VECTOR3 vNormal = VECTOR3(0.0f,0.0f,0.0f)) override;
	void Update() override;
	void Render();
	void RenderParticle(MATRIX4X4 W, MATRIX4X4 V, MATRIX4X4 P);  // -- 2017.8.25

	void SetTexture(ID3D11ShaderResourceView* pTexture) { m_pTexture = pTexture; }

};
// ========================================================================================
//
//	パーティクルのプロシージャクラス
//
// ========================================================================================
class CParticleProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //テクスチャー（用意するのはリソースビューだけでいい）
public:
	BOOL Start(VECTOR3 vEmitPos, VECTOR3 vNormal = VECTOR3(0.0f, 0.0f, 0.0f));
	CParticleProc(CGameMain* pGMain);	// コンストラクタ
	~CParticleProc();	// デストラクタ
};


// ========================================================================================
//
//	ビルボード
//
// ========================================================================================
// ------------------------------------------------------------------------
//	ビルボードのオブジェクトクラス
//
// ------------------------------------------------------------------------
class CBillboardObj : public CBaseObj
{
protected:
	float			m_fDestWidth;
	float			m_fDestHeight;
	DWORD			m_dwSrcX;
	DWORD			m_dwSrcY;
	DWORD			m_dwSrcWidth;
	DWORD			m_dwSrcHeight;
public:

	// 
	CDirect3D*	   m_pD3D;
	CShader*       m_pShader;
	VECTOR3        m_vPos;
	VECTOR2        m_vUVOffset;
	VECTOR2        m_vUVScale;  // -- 2019.7.17
	DWORD          m_dwNumX;
	DWORD          m_dwNumY;
	DWORD          m_dwFrame;
	FLOAT          m_fAlpha;
	DWORD          m_dwImageWidth;
	DWORD          m_dwImageHeight;

	ID3D11Buffer*  m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（ビルボードプロシージャのテクスチャアドレスをセットするだけ）

public:
	// 
	CBillboardObj(CBaseProc* pProc);     // -- 2022.2.16
	virtual	~CBillboardObj();

	void Init();
	HRESULT SetSrc( float fDestWidth, float fDestHeight, DWORD dwSrcX, DWORD dwSrcY, DWORD dwSrcWidth, DWORD dwSrcHeight, DWORD dwNumX, DWORD dwNumY);
	DWORD   GetSrcWidth()  {return m_dwSrcWidth;}
	DWORD   GetSrcHeight() {return m_dwSrcHeight;}
	float   GetDestWidth()  {return m_fDestWidth;}
	float   GetDestHeight() {return m_fDestHeight;}

	BOOL Start(VECTOR3 vPos) override;
	void Update() override;
	BOOL Render();
	void RenderMesh();

};
// ========================================================================================
//
//	ビルボードのプロシージャクラス
//
// ========================================================================================
class CBillboardProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //テクスチャー（用意するのはリソースビューだけでいい）
	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
public:
	BOOL Start(VECTOR3 vPos);
	HRESULT Load(TCHAR* szFName);
	CBillboardProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CBillboardProc();	// デストラクタ
};

// ========================================================================================
//
//	ビルボード　炎
//
// ========================================================================================
// ------------------------------------------------------------------------
//	ビルボード　炎のオブジェクトクラス
//
// ------------------------------------------------------------------------
class CBillFireObj : public CBaseObj
{
protected:
	float			m_fDestWidth;
	float			m_fDestHeight;
	DWORD			m_dwSrcX;
	DWORD			m_dwSrcY;
	DWORD			m_dwSrcWidth;
	DWORD			m_dwSrcHeight;

public:
	// 
	CDirect3D*	   m_pD3D;
	CShader*       m_pShader;
	VECTOR3        m_vPos;
	VECTOR2        m_vUVOffset;
	BOOL           m_bAuto;
	DWORD          m_dwNumX;
	DWORD          m_dwNumY;
	DWORD          m_dwFrame;
	FLOAT          m_fAlpha;
	DWORD          m_dwImageWidth;
	DWORD          m_dwImageHeight;

	ID3D11Buffer*  m_pVertexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（ビルボード炎プロシージャのテクスチャアドレスをセットするだけ）

public:
	// 
	CBillFireObj(CBaseProc* pProc);                // -- 2022.2.16
	virtual	~CBillFireObj();

	void Init();
	HRESULT SetSrc(float fDestWidth, float fDestHeight, DWORD dwSrcX, DWORD dwSrcY, DWORD dwSrcWidth, DWORD dwSrcHeight, DWORD dwNumX, DWORD dwNumY);
	DWORD   GetSrcWidth() { return m_dwSrcWidth; }
	DWORD   GetSrcHeight() { return m_dwSrcHeight; }

	BOOL Start(VECTOR3 vPos) override;
	void Update() override;
	BOOL Render();

};
// ========================================================================================
//
//	ビルボード　炎のプロシージャクラス
//
// ========================================================================================
class CBillFireProc : public CBaseProc
{
protected:
	ID3D11ShaderResourceView* m_pTexture;  //テクスチャー（用意するのはリソースビューだけでいい）
	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
public:
	BOOL Start(VECTOR3 vPos);
	CBillFireObj* Start();
	HRESULT Load(TCHAR* szFName);
	CBillFireProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CBillFireProc();	// デストラクタ
};


// ========================================================================================
//
//	エフェクト全体のメインプロシージャクラス
//
// ========================================================================================
class CEffectProc : public CBaseProc
{
public:
	CParticleProc*		m_pParticleProc;
	CBillboardProc*		m_pBillProc;
	CBillFireProc*		m_pBillFireProc;

public:
	CEffectProc(CGameMain* pGMain);	// コンストラクタ
	virtual	~CEffectProc() { ; }
};

