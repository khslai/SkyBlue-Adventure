//=============================================================================
//
// タイトルヘッダー [Title.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
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

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// タイトル構造体
typedef struct
{
	VERTEX_2D				VertexWk[Num_Vertex];		// 頂点情報格納ワーク
	D3DXVECTOR3				Pos;						// 座標
}TITLE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitTitle(int InitState);
// 終了処理
void UninitTitle(void);
// 更新処理
void UpdateTitle(void);
// 描画処理
void DrawTitle(void);
// メニュー画面描画処理
void DrawTitleMenu(void);
// タイトル選択肢の情報を取得する
SELECT *GetTitleSelect(void);

#endif
