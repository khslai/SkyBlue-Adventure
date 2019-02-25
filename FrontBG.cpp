//=============================================================================
//
// 前景処理 [FrontBG.cpp]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#include "main.h"
#include "FrontBG.h"
#include "BackGround.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FrontBGSpeed (0.005f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeFrontBGVertex(void);
void SetFrontBGTexture(void);
void SetFrontBGDiffuse(int Alpha);
void SetFrontBGVertex(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 FrontBGTexture = NULL;
BACKGROUND FrontBG;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFrontBG(int InitState)
{
	FrontBG.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	MakeFrontBGVertex();

	if (InitState == FirstInit)
	{
		if (SafeLoad(Texture_FrontBG, &FrontBGTexture, "FrontBG") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFrontBG(void)
{
	SafeRelease(FrontBGTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFrontBG(void)
{
	bool BGFadeout = GetBGFadeout();
	int State = GetBGState();
	static int Alpha = 64;

	FrontBG.Pos.x += FrontBGSpeed;

	if (State == Afternoon && BGFadeout == true)
	{
		Alpha = Alpha <= 0 ? 0 : Alpha - 1;
	}
	else if (State == Night && BGFadeout == true)
	{
		Alpha = Alpha >= 64 ? 64 : Alpha + 1;
	}

	SetFrontBGTexture();
	SetFrontBGDiffuse(Alpha);
	SetFrontBGVertex();

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFrontBG(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	// 加算合成モードに設置する
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// テクスチャの設定
	Device->SetTexture(0, FrontBGTexture);

	// ポリゴンの描画
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_BG, FrontBG.VertexWk, sizeof(VERTEX_2D));

	// 本来の設置を戻す
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeFrontBGVertex(void)
{
	// 頂点座標の設定	
	SetFrontBGVertex();

	// rhwの設定
	FrontBG.VertexWk[0].rhw = 1.0f;
	FrontBG.VertexWk[1].rhw = 1.0f;
	FrontBG.VertexWk[2].rhw = 1.0f;
	FrontBG.VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	FrontBG.VertexWk[0].diffuse = WHITE(255);
	FrontBG.VertexWk[1].diffuse = WHITE(255);
	FrontBG.VertexWk[2].diffuse = WHITE(255);
	FrontBG.VertexWk[3].diffuse = WHITE(255);

	// テクスチャ座標の設定
	SetFrontBGTexture();

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetFrontBGTexture(void)
{
	float BGDistance_X = (float)Screen_Width / (float)Texture_FrontBG_Width;

	FrontBG.VertexWk[0].tex = D3DXVECTOR2(FrontBG.Pos.x, 0.0f);
	FrontBG.VertexWk[1].tex = D3DXVECTOR2(FrontBG.Pos.x + BGDistance_X, 0.0f);
	FrontBG.VertexWk[2].tex = D3DXVECTOR2(FrontBG.Pos.x, 1.0f);
	FrontBG.VertexWk[3].tex = D3DXVECTOR2(FrontBG.Pos.x + BGDistance_X, 1.0f);

	return;
}

//=============================================================================
// 色、透明度の設定
//=============================================================================
void SetFrontBGDiffuse(int Alpha)
{
	FrontBG.VertexWk[0].diffuse = WHITE(Alpha);
	FrontBG.VertexWk[1].diffuse = WHITE(Alpha);
	FrontBG.VertexWk[2].diffuse = WHITE(Alpha);
	FrontBG.VertexWk[3].diffuse = WHITE(Alpha);

	return;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetFrontBGVertex(void)
{
	FrontBG.VertexWk[0].vtx = D3DXVECTOR3(0, 0, 0);
	FrontBG.VertexWk[1].vtx = D3DXVECTOR3(Screen_Width, 0, 0);
	FrontBG.VertexWk[2].vtx = D3DXVECTOR3(0, Screen_Height, 0);
	FrontBG.VertexWk[3].vtx = D3DXVECTOR3(Screen_Width, Screen_Height, 0);

	return;
}
