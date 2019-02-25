//=============================================================================
//
// Mainヘッダー [main.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_


//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止

#include <time.h>
#include <Windows.h>
#include <stdio.h>
#include <d3dx9.h>
#include <tchar.h>


#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include <dsound.h>
#include <dinput.h>
#include <mmsystem.h>

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dsound.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// UI
#define Screen_Width		(1024)					// ウインドウの幅
#define Screen_Height		(768)					// ウインドウの高さ
#define ScreenCenter_X		(Screen_Width / 2)		// ウインドウの中心座標
#define ScreenCenter_Y		(Screen_Height / 2)		
#define MoveRange_Y			(90)					// 移動可能の範囲のY座標
// ゲーム流れ
#define GameStartCount		(150)					// ゲームスタートの時間
#define GameClearCount		(9300)					// ゲームクリアの時間
// ロードファイル
#define StatusType_Max		(145)					// エネミーステータス資料の最大数
#define BulletType_Max		(11)					// バレットステータス資料の最大数
#define EnemyBulletType_Max	(6)						// 弾種類の最大数
#define PlayerBulletType_A	(EnemyBulletType_Max)   // プレイヤーの弾のタイプ
#define PlayerBulletType_B	(PlayerBulletType_A + 1)
#define OptionBulletType_A	(PlayerBulletType_B + 1)
#define OptionBulletType_B	(OptionBulletType_A + 1)
#define BombBulletType		(EnemyBulletType_Max + 4)
// プレイヤー
#define Player_Max			(1)						// プレイヤーの最大人数
#define PlayerOption_Max	(2)						// プレイヤーオプションの最大数
#define PlayerInvCount		(120)					// プレイヤー無敵時間
#define PlayerDebutCount	(120)					// プレイヤー登場のカウント
#define PlayerDebutPos_X	(80)					// プレイヤー登場の座標
#define PlayerDebutPos_Y	(ScreenCenter_Y)
#define PlayerStartPower	(2.0f)					// プレイヤーパワーの初期状態
#define PlayerStartBomb		(3)						// プレイヤーボムの初期状態
#define PlayerHP_Max		(100.0f)				// プレイヤーHPの最大数
#define PlayerPower_Max		(3.0f)					// プレイヤーパワーの最大数
#define PlayerBomb_Max		(5)						// プレイヤーボムの最大数
#define PlayerItem_Max		(10)					// プレイヤー持つアイテムの最大数
#define PlayerBullet_Max	(200)					// プレイヤー持つ最大弾数
#define BombBullet_Max		(Enemy_Max)				// ボム発射する弾の最大数
// エネミー
#define Enemy_Max			(20)					// 画面中表示するエネミーの最大数
#define EnemyType_Max		(5)						// エネミー種類の最大数
#define EnemyBullet_Max		(1000)					// 一つのエネミー持つ最大弾数
#define EnemyItem_Max		(6)						// 一つのエネミー持つアイテムの最大数
#define Barrage_Max			(30)					// 画面中表示する弾幕の最大数
// ボス
#define BossBullet_Max		(1000)					// ボス持つ最大弾数
#define BossItem_Max		(30)					// ボス持つアイテムの最大数
#define Laser_Max			(20)					// 画面中表示するレーザーの最大数
// エフェクト
#define EffectCounter_Max	(200)					// 一回表示できるエフェクトの最大種類
#define Effect_Max			(200)					// 一回表示できるエフェクトの最大数
// アイテム
#define ItemNum_Max			(500)					// 一回表示できるアイテムの最大数
#define HPItemValue			(0.5f)					// アイテム上昇の数値
#define ScoreItemValue		(5000)
#define PowerItemValue		(0.02f)
// 選択肢ループ
#define RepeatCount (60)
#define RepeatSpeed (5)
// 計算用
#define PI					(D3DX_PI)
#define Root2				(1.414214f)
#define TextureDevideNum	(50)					// ポリゴン分割の数

// 頂点数
#define	Num_Vertex			(4)					
// ポリゴン数
#define Num_Polygon			(2)

// 色
#define WHITE(Alpha)		D3DCOLOR_RGBA(255, 255, 255, Alpha)
#define BLACK(Alpha)		D3DCOLOR_RGBA(0, 0, 0, Alpha)
#define GREEN(Alpha)		D3DCOLOR_RGBA(0, 255, 0, Alpha)
#define BLUE(Alpha)			D3DCOLOR_RGBA(0, 0, 255, Alpha)
#define YELLOW(Alpha)		D3DCOLOR_RGBA(255, 255, 0, Alpha)
#define RED(Alpha)			D3DCOLOR_RGBA(255, 0, 0, Alpha)
#define SKYBLUE(Alpha)		D3DCOLOR_RGBA(135, 206, 235, Alpha)
#define ORANGE(Alpha)		D3DCOLOR_RGBA(255, 165, 0, Alpha)
#define PURPLE(Alpha)		D3DCOLOR_RGBA(128, 0, 128, Alpha)

// メニューテクスチャ
#define Texture_BlackScreen			_T("data/Texture/Other/BlackScreen.png")
#define Texture_BlackScreen_Width	(1024)
#define Texture_BlackScreen_Height	(768)
#define Texture_SelectBox			_T("data/Texture/Other/SelectBox.png")
#define Texture_SelectBox_Width		(350)
#define Texture_SelectBox_Height	(100)

// プレイヤーオプションの座標と発射角度
const int OptionPos_x[2] = { 0,0 };
const int OptionPos_y[2] = { -50, 50 };
const int OptionPos_x_LowSpeed[2] = { 30, 30 };
const int OptionPos_y_LowSpeed[2] = { -40, 40 };

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// テクスチャのメモリをリリース
#define SafeRelease(ReleaseObject) { if(ReleaseObject != NULL) { ReleaseObject->Release(); ReleaseObject = NULL; } }
// 角度をラジアンに変換する
#define DegreeToRadian(Degree)	(Degree) * (0.017456f)

// 背景の状態
enum BGState
{
	Day,
	Afternoon,
	Night,
};

// SetStageの引数
enum StagePhase
{
	Stage_Title,
	Stage_SelectChar,
	Stage_Tutorial,
	Stage_Game,
	Stage_PraticeSelect,
	Stage_BossPratice,
	Stage_Pause,
	Stage_Result,
	Stage_GameOver,
};

// タイトルの状態
enum TitlePhase
{
	GameStart,
	StartYes,
	BossPratice,
	PraticeYes,
	ExitGame,
	ExitCheck,
	ExitYes,
	EasyMode,
	NormalMode,
	HardMode,
};

// ゲームの難易度
enum GameDifficulty
{
	Easy,
	Normal,
	Hard,
};

// 自機選ぶ画面の状態
enum SelectCharPhase
{
	SetObjectStart,
	ChooseTypeA,
	ChooseTypeB,
	ChooseCheck,
	ChooseOver,
	BackToTitle,
};

// チュートリアルの状態
enum TutorialPhase
{
	ShowTutorial1,
	ShowTutorial2,
	SetTutorialStart,
	SetTutorialOver,
	ExitTutorial,
};

// 練習モード選択の状態
enum PraticeSelectState
{
	SetBossStart,
	BossBarrage_M1,
	BossBarrage_M2,
	BossBarrage_L1,
	BossBarrage_L2,
	BossBarrage_L3,
	BossBarrage_L4,
	BossRush,
	BackToSelectChar,
};

// 初期化の状態
enum InitState
{
	FirstInit,
	ReInit,
};

// CheckRangeの引数
enum CheckType
{
	OutOfRange,
	OutOfRange_SelectChar,
	InRange,
};

// エネミーの種類
enum
{
	Enemy00,
	Enemy01,
	Enemy02,
	Enemy03,
	Enemy04,
};

// アイテムの種類
enum ItemType
{
	HPItem,
	ScoreItem,
	PowerItem,
	ItemBackGround,
	Nothing = -1,
};

// 弾幕の状態
enum BarragePhase
{
	BarrageStart,			// 発射開始
	EnemyDisappear,			// エネミーは死んだ、でも弾は残ってる
	BarrageOver,			// 全部の弾が消える、弾幕終わる
};

// テクスチャ色の引数
enum Texturecolor
{
	White,
	Red,
	Red_AlphaHalf,
	Green,
	SkyBlue,
	Yellow,
	PowerGaugeGradation,
};

// フォントの種類
enum FontType
{
	FontSize_108,
	FontSize_54,
	FontSize_40,
	FontSize_30,
	Font_Max,
};

// UIテクスチャの種類
enum UITextureType
{
	BlackScreen,
	SelectBox,
	UITexture_Max,
};

// ボス今の状態
enum BossState
{
	StandBy,
	ShotBullet,
	HPZero,
	HPRecover,
	NextPhase,
	MiddleBossOver,
	BossDisappear,
	LastBossOver,
};

// ボス今の弾幕段階
enum BossPhase
{
	MiddleBoss_Phase1,
	MiddleBoss_LastPhase,
	LastBoss_Phase1,
	LastBoss_Phase2,
	LastBoss_Phase3,
	LastBoss_LastPhase,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 上記頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3		vtx;					// 頂点座標
	float			rhw;					// テクスチャのパースペクティブコレクト用
	D3DCOLOR		diffuse;				// 反射光
	D3DXVECTOR2		tex;					// テクスチャ座標
} VERTEX_2D;

// 選択肢構造体
typedef struct
{
	VERTEX_2D		VertexWk[Num_Vertex];	// 頂点情報格納ワーク
	D3DXVECTOR3		Pos;					// 座標
	int				Phase;					// 今指す選択肢
	bool			InYes;					// 答えは"YES"か"No"か
}SELECT;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// デバイス取得関数
LPDIRECT3DDEVICE9 GetDevice(void);
// UIテクスチャ取得関数
LPDIRECT3DTEXTURE9 GetUITexture(int UITextureType);
// テキストフォント取得関数
LPD3DXFONT GetFont(int FontType);
// 描画ライン取得関数
LPD3DXLINE GetLine(void);
// GameCount取得関数
int GetGameCount(void);
// FPS取得関数
int GetFPSCount(void);
// ゲームステージ取得関数
int GetGameStage(void);
// ゲーム難易度取得関数
int GetGameDiffuculty(void);
// ゲーム難易度設定関数
void SetGameDiffuculty(int DifficultyMode);
// ゲームステージ設定関数（画面遷移）
void SetGameStage(int Stage);
// ゲームの再初期化
void ReInitGame(bool InitSound);
// 弾はスクリーンの範囲内かどうかをチェック
bool CheckRange(int CheckMode, D3DXVECTOR3 Pos, int Width, int Height);
// テクスチャ読み込む関数
bool SafeLoad(LPCSTR SrcFile, LPDIRECT3DTEXTURE9* TexturePtr, const char* ErrorSrc);
// 二つの座標の移動角度を計算する
float AngleCalculate(D3DXVECTOR3 ReferencePos, D3DXVECTOR3 ObjectPos);
// +Radian ~ -Radianランダムなラジアンを返す
float RandRadian(float Radian);

#endif