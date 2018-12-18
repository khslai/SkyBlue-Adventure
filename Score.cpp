//=============================================================================
//
// スコア処理 [Score.cpp]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#include "main.h"
#include "Score.h"
#include "Player.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DigitMax (10)
#define ScoreMax (9999999999)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeScoreVertex(int ScoreDigit, int ScoreNum);
void SetScoreTexture(int ScoreDigit, int ScoreNum);
void SetScoreVertex(int ScoreDigit);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9 ScoreTexture = NULL;
SCORE Score[DigitMax];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(int InitState)
{
	int ScoreDigit = 0;

	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		Score[ScoreDigit].Pos = D3DXVECTOR3((float)ScorePos_X - ScoreDigit * Texture_Score_Width, ScorePos_Y, 0.0f);
		Score[ScoreDigit].Number = 0;
		// 頂点情報の作成
		MakeScoreVertex(ScoreDigit, Score[ScoreDigit].Number);
	}

	if (InitState == FirstInit)
	{
		// テクスチャの読み込み
		if (SafeLoad(Texture_Score, &ScoreTexture, "Score") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	// テクスチャの開放
	SafeRelease(ScoreTexture);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	PLAYER *Player = GetPlayer(0);
	int ScoreDigit = 0;
	int Scoretemp = Player->Score;

	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		Score[ScoreDigit].Number = Scoretemp % 10;
		Scoretemp /= 10;
		//Score[ScoreDigit].Number = Scoretemp / (int)pow(10, DigitMax - ScoreDigit - 1);
		//Scoretemp = Scoretemp % (int)pow(10, DigitMax - ScoreDigit - 1);
		// テクスチャ座標を設定
		SetScoreTexture(ScoreDigit, Score[ScoreDigit].Number);
	}

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	int ScoreDigit = 0;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_2D);

	for (ScoreDigit = 0; ScoreDigit < DigitMax; ScoreDigit++)
	{
		// テクスチャの設定
		Device->SetTexture(0, ScoreTexture);

		// ポリゴンの描画
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SCORE, Score[ScoreDigit].VertexWk, sizeof(VERTEX_2D));
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeScoreVertex(int ScoreDigit, int ScoreNum)
{
	// 頂点座標の設定	
	SetScoreVertex(ScoreDigit);

	// rhwの設定
	Score[ScoreDigit].VertexWk[0].rhw = 1.0f;
	Score[ScoreDigit].VertexWk[1].rhw = 1.0f;
	Score[ScoreDigit].VertexWk[2].rhw = 1.0f;
	Score[ScoreDigit].VertexWk[3].rhw = 1.0f;

	// 反射光の設定
	Score[ScoreDigit].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Score[ScoreDigit].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Score[ScoreDigit].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	Score[ScoreDigit].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	SetScoreTexture(ScoreDigit, ScoreNum);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetScoreTexture(int ScoreDigit, int ScoreNum)
{
	Score[ScoreDigit].VertexWk[0].tex = D3DXVECTOR2(ScoreNum * 0.1f, 0.0f);
	Score[ScoreDigit].VertexWk[1].tex = D3DXVECTOR2((ScoreNum + 1) * 0.1f, 0.0f);
	Score[ScoreDigit].VertexWk[2].tex = D3DXVECTOR2(ScoreNum * 0.1f, 1.0f);
	Score[ScoreDigit].VertexWk[3].tex = D3DXVECTOR2((ScoreNum + 1) * 0.1f, 1.0f);

	return;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetScoreVertex(int ScoreDigit)
{
	Score[ScoreDigit].VertexWk[0].vtx =
		D3DXVECTOR3(Score[ScoreDigit].Pos.x, Score[ScoreDigit].Pos.y, 0.0f);
	Score[ScoreDigit].VertexWk[1].vtx =
		D3DXVECTOR3(Score[ScoreDigit].Pos.x + Texture_Score_Width, Score[ScoreDigit].Pos.y, 0.0f);
	Score[ScoreDigit].VertexWk[2].vtx =
		D3DXVECTOR3(Score[ScoreDigit].Pos.x, Score[ScoreDigit].Pos.y + Texture_Score_Height, 0.0f);
	Score[ScoreDigit].VertexWk[3].vtx =
		D3DXVECTOR3(Score[ScoreDigit].Pos.x + Texture_Score_Width, Score[ScoreDigit].Pos.y + Texture_Score_Height, 0.0f);

	return;
}

//=============================================================================
// スコアの計算
//=============================================================================
void AddScore(int Score)
{
	PLAYER *Player = GetPlayer(0);

	Player->Score += Score;
	Player->Score = Player->Score > ScoreMax ? (int)ScoreMax : Player->Score;

	return;
}
