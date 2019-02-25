//=============================================================================
//
// 背景ヘッダー [BackGround.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
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


//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 背景構造体
typedef	struct
{
	VERTEX_2D		VertexWk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;					// 座標
	bool			InShake;				// 今は揺れるか
}BACKGROUND;

// ゲームクリア構造体
typedef struct
{
	VERTEX_2D		Pixel[GameClearVertexDevide][Num_Vertex];		// 頂点情報格納ワーク
	int				Alpha;											// アルファ値
	bool			FadeInOver;										// フェイドイン終了か
	bool			AnimationOver;									// 演出終わる
}GAMECLEAR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitBG(int InitState);
// 終了処理
void UninitBG(void);
// 更新処理
void UpdateBG(void);
// 描画処理
void DrawBG(void);
// プレイヤーUIの描画処理
void DrawInformation(void);
// 背景が揺れる演出の設置
void SetBGShake(bool State);
// 背景の状態を取得する
int GetBGState(void);
// 背景のフェイドアウト状態を取得する
bool GetBGFadeout(void);
// ゲームクリアの情報を取得する
GAMECLEAR *GetGameClear(void);

#endif
