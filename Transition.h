//=============================================================================
//
// 画面遷移ヘッダー [Transition.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _TRANSITION_H_
#define _TRANSITION_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Transition			_T("data/Texture/Other/Transition.png")
#define Texture_Transition_Width	(1024)
#define Texture_Transition_Height	(768)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// ステージ遷移構造体
typedef struct
{
	VERTEX_2D	Pixel[TextureDevideNum][TextureDevideNum][Num_Vertex];	// 頂点情報格納ワーク
	int			Count;													// 計算カウント
	int			Alpha;													// アルファ値
	float		DissappearRadius;										// 中心から消失の半径
	bool		FadeInOver;												// フェイドイン終了かのフラグ
	bool		Use;													// 使用フラグ
}TRANSITION;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitTransition(int InitState);
// 終了処理
void UninitTransition(void);
// 更新処理
void UpdateTransition(void);
// 描画処理
void DrawTransition(void);
// ステージ遷移を設置する
void SetTransition(void);
// ステージ遷移の情報を取得する
TRANSITION *GetTransition(void);

#endif
