//=============================================================================
//
// 画面遷移ヘッダー [Transition.h]
// Author：TH_GP11_GP11B341_35_頼凱興
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

typedef struct
{
	VERTEX_2D	Pixel[TextureDevideNum][TextureDevideNum][NUM_VERTEX];	// 頂点情報格納ワーク
	int			Count;													// 計算カウント
	int			Alpha;													// アルファ値
	float		DissappearRadius;										// 中心から消失の半径
	bool		FadeInOver;												// フェイドイン終了かのフラグ
	bool		Use;													// 使用フラグ
}TRANSITION;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTransition(int InitState);
void UninitTransition(void);
void UpdateTransition(void);
void DrawTransition(void);
void SetTransition(void);
TRANSITION *GetTransition(void);


#endif
