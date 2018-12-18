//=============================================================================
//
// タイトルヘッダー [Title.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_TitleBG				_T("data/Texture/BackGround/TitleBG.png")
#define Texture_TitleBG_Width		(1988)
#define Texture_TitleBG_Height		(768)
#define Texture_GameTitle			_T("data/Texture/BackGround/GameTitle.png")
#define Texture_GameTitle_Width		(850)
#define Texture_GameTitle_Height	(120)
#define Texture_Light				_T("data/Texture/BackGround/Light.png")
#define Texture_Light_Width			(1024)
#define Texture_Light_Height		(768)


typedef struct
{
	VERTEX_2D				VertexWk[NUM_VERTEX];		// 頂点情報格納ワーク
	D3DXVECTOR3				Pos;						// 座標
}TITLE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(int InitState);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void DrawTitleMenu(void);
SELECT *GetTitleSelect(void);

#endif
