//=============================================================================
//
// 背景ヘッダー [BackGround.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define Texture_BG_Day				_T("data/Texture/BackGround/BackGround_Day.png")
#define Texture_BG_Day				_T("data/Texture/BackGround/BackGround_Day.png")
#define Texture_BG_Afternoon		_T("data/Texture/BackGround/BackGround_Afternoon.png")
#define Texture_BG_Night			_T("data/Texture/BackGround/BackGround_Night.png")
#define Texture_BG_Width			(1988)
#define Texture_BG_Height			(768)
#define Texture_GameStart			_T("data/Texture/BackGround/GameStart.png")
#define Texture_GameStart_Width		(600)
#define Texture_GameStart_Height	(100)
#define Texture_GameClear			_T("data/Texture/BackGround/GameClear.png")
#define Texture_GameClear_Width		(600)
#define Texture_GameClear_Height	(120)

#define GameClearVertexDevide		(6)		// テクスチャは六つのポリゴンを分割する



typedef	struct
{
	VERTEX_2D		VertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;					// 座標
	bool			InShake;				// 今は揺れるか
}BACKGROUND;

typedef struct
{
	VERTEX_2D		Pixel[GameClearVertexDevide][NUM_VERTEX];		// 頂点情報格納ワーク
	int				Alpha;											// アルファ値
	bool			FadeInOver;										// フェイドイン終了か
	bool			AnimationOver;									// 演出終わる
}GAMECLEAR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBG(int InitState);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
void DrawInformation(void);
void SetBGShake(bool State);
int GetBGState(void);
bool GetBGFadeout(void);
GAMECLEAR *GetGameClear(void);


#endif
