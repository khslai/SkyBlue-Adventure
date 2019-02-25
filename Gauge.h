//=============================================================================
//
// ゲージヘッダー [Gauge.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Gauge_Player			_T("data/Texture/Player/Gauge.png")	
#define Texture_GaugeBox_PlayerHP		_T("data/Texture/Player/HPGaugeBox.png")	
#define Texture_GaugeBox_PlayerPower	_T("data/Texture/Player/PowerGaugeBox.png")	
#define Texture_Gauge_Player_Width		(400)
#define Texture_Gauge_Player_Height		(20)
#define Texture_BombSymbol				_T("data/Texture/Player/BombSymbol.png")	
#define Texture_BombSymbol_Width_Total	(60)
#define Texture_BombSymbol_Divide_X		(2)
#define Texture_BombSymbol_Width		(Texture_BombSymbol_Width_Total / Texture_BombSymbol_Divide_X)
#define Texture_BombSymbol_Height		(30)
#define Texture_Gauge_Boss				_T("data/Texture/Boss/Gauge.png")	
#define Texture_GaugeBox_Boss			_T("data/Texture/Boss/GaugeBox.png")	
#define Texture_Gauge_Boss_Width		(20)
#define Texture_Gauge_Boss_Height		(500)

// プレイヤーのHPゲージの座標
#define PlayerHPGauge_Pos_X				(100)		
#define PlayerHPGauge_Pos_Y				(10)
// プレイヤーのパワーゲージの座標
#define PlayerPowerGauge_Pos_X			(100)		
#define PlayerPowerGauge_Pos_Y			(PlayerHPGauge_Pos_Y + 40)
// プレイヤーのパワーゲージの座標
#define PlayerBombSymbol_Pos_X			(ScorePos_X - Texture_Score_Width_Total * 0.9)	
#define PlayerBombSymbol_Pos_Y			(PlayerPowerGauge_Pos_Y)
// ボスのHPゲージの座標
#define BossHPGauge_Pos_X				(1000)			
#define BossHPGauge_Pos_Y				(700)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// ゲージ構造体
typedef	struct
{
	VERTEX_2D	VertexWk[Num_Vertex];		// 頂点情報格納ワーク
	int			Type;						// ゲージの種類
}GAUGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitGauge(int InitState);
// 終了処理
void UninitGauge(void);
// 更新処理
void UpdateGauge(void);
// 描画処理
void DrawGauge(void);

#endif
