//=============================================================================
//
// ロードファイルヘッダー [File.h]
// Author：TH_GP11_GP11B341_35_頼凱興
//
//=============================================================================
#ifndef FILE_H
#define FILE_H


/*******************************************************************************
* マクロ定義
*******************************************************************************/



/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3		Pos;					// 初期座標
	float			Speed;					// 移動スピード
	float			BulletSpeed;			// 弾のスピード
	float			HP_Max;					// 体力
	int				AppearCount;			// カウンタ
	int				MovePattern;			// 移動パターン
	int				EnemyType;				// 敵の種類
	int				EnemyDifficulty;		// どんな難易度で出現する
	int				ShotTime;				// 弾幕開始時間
	int				BarrageType;			// 弾幕の種類
	int				BulletType;				// 弾の種類
	int				WaitCount;				// 停滞時間
	int				ExitCount;				// 退場時間
	int				Item[EnemyItem_Max];	// アイテム
}ENEMYSTATUS;

typedef struct
{
	int				Width;					// 横幅
	int				Height;					// 縦幅
	int				Devide_X;				// テクスチャ内分割数（横)
	float			Radius;					// 画像中心から頂点までの距離
	float			HitRadius;				// 当たり判定の円の半径
	float			BaseAngle;				// 画像中心から頂点までの角度
	float			Rot_z;					// 画像の回転角度
}BULLETTYPE;

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT LoadEnemyStatus(void);
ENEMYSTATUS *GetEnemyStatus(int StatusType_No);
HRESULT LoadBulletType(void);
BULLETTYPE *GetBulletType(int Type_No);

#endif
