//=============================================================================
//
// 当たり判定処理 [CheckHit.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "CheckHit.h"
#include "Player.h"
#include "Enemy.h"
#include "Barrage.h"
#include "Laser.h"
#include "Score.h"
#include "Effect.h"
#include "Bomb.h"
#include "Boss.h"
#include "Item.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ゲーム難易度係数
#define DifficultyRate (0.6f)
// 攻撃する時のスコア
#define AttackScore (490)
// ボムが弾を削除する時のスコア
#define ClearBulletScore (90)
// エネミーを倒す時のスコア
#define KillEnemyScore (9990)
// ボスを倒す時のスコア
#define KillBossScore (1000000)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// CP = Cross Product(外積)
bool CheckHitCP(D3DXVECTOR3 Object, float ObjectRadius, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrePos);
// BC = バウンディングサークル
bool CheckHitBC(float CheckRange, D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrePos, float BulletSpeed);
// CR = サークル と 矩形(回転あり)
bool CheckHitCR(D3DXVECTOR3 RotateCenter, float MoveAngle, D3DXVECTOR3 Object, float ObjectRadius);
// 弾消失をチェックする
bool CheckBulletDisappear(D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, float BulletRadius);
// プレイヤーが撃たれた処理
void PlayerEvent(void);
// エネミー死亡処理
void EnemyDeadEvent(int Enemy_No);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// レーザーの判定範囲（矩形）
RECT Rect;

//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	int Barrage_No = 0;
	int Laser_No = 0;
	int Bullet_No = 0;
	int BombBullet_No = 0;
	int Enemy_No = 0;
	int Item_No = 0;
	int GameDifficulty = GetGameDiffuculty();
	static bool AddBossScore = false;
	PLAYER *Player = GetPlayer(0);
	ENEMY *Enemy = GetEnemy(Enemy_No);
	BARRAGE *Barrage = GetBarrage(Barrage_No);
	LASER *Laser = GetLaser(Laser_No);
	ITEM *Item = GetItem(Item_No);
	BOMB *Bomb = GetBomb();
	BOSS *Boss = GetBoss();

	// プレイヤーの弾とエネミー、ボス
	for (Bullet_No = 0; Bullet_No < PlayerBullet_Max; Bullet_No++)
	{
		if (Player->Bullet[Bullet_No].Use == true)
		{
			// エネミー
			if (Boss->Exist == false)
			{
				// エネミーのポインターを初期化
				Enemy_No = 0;
				Enemy = GetEnemy(Enemy_No);
				for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
				{
					if (Enemy->Exist == true)
					{
						if (CheckHitBC((Enemy->HitRadius + Player->Bullet[Bullet_No].HitRadius),
							Enemy->Pos,
							Player->Bullet[Bullet_No].Pos,
							Player->Bullet[Bullet_No].PreviousPos,
							Player->Bullet[Bullet_No].Speed) == true)
						{
							if (CheckBulletDisappear(Enemy->Pos,
								Player->Bullet[Bullet_No].Pos,
								Player->Bullet[Bullet_No].HitRadius) == true)
							{
								// 1フレイム後弾が消える
								Player->Bullet[Bullet_No].DelayDisappear = true;
							}
							else
							{
								// 今のフレイムに弾が消える
								Player->Bullet[Bullet_No].Use = false;
							}
							// スコア上昇
							AddScore(AttackScore);
							// エネミーHP減る
							if (GameDifficulty == Easy)
							{
								Enemy->HP -= Player->Bullet[Bullet_No].Damage * DifficultyRate;
							}
							else
							{
								Enemy->HP -= Player->Bullet[Bullet_No].Damage;
							}
							// 被弾エフェクト
							SetEnemyColor(Enemy_No, Red);
							// エネミー死亡
							if (Enemy->HP <= 0)
							{
								EnemyDeadEvent(Enemy_No);
							}
							break;
						}
					}
				}
			}
			// ボス
			else if (Boss->Exist == true)
			{
				if (CheckHitBC((Boss->HitRadius + Player->Bullet[Bullet_No].HitRadius),
					Boss->Pos,
					Player->Bullet[Bullet_No].Pos,
					Player->Bullet[Bullet_No].PreviousPos,
					Player->Bullet[Bullet_No].Speed) == true)
				{
					if (CheckBulletDisappear(Enemy->Pos,
						Player->Bullet[Bullet_No].Pos,
						Player->Bullet[Bullet_No].HitRadius) == true)
					{
						// 1フレイム後弾が消える
						Player->Bullet[Bullet_No].DelayDisappear = true;
					}
					else
					{
						// 今のフレイムに弾が消える
						Player->Bullet[Bullet_No].Use = false;
					}
					if (Boss->State != HPRecover)
					{
						AddScore(AttackScore);
						Boss->HP -= Player->Bullet[Bullet_No].Damage;
						if (Boss->HP <= 0)
						{
							Boss->HP = 0;
							if (AddBossScore == false)
							{
								AddScore(KillBossScore);
								AddBossScore = true;
							}
						}
						else
						{
							AddBossScore = false;
						}
					}
					SetBossColor(Red);
				}
			}
		}
	}

	// プレイヤーのボムとエネミー、弾
	if (Bomb->Use == true && Player->Type == Player_A)
	{
		if (Boss->Exist == false)
		{
			// エネミー
			// エネミーのポインターを初期化
			Enemy_No = 0;
			Enemy = GetEnemy(Enemy_No);
			for (Enemy_No = 0; Enemy_No < Enemy_Max; Enemy_No++, Enemy++)
			{
				if (Enemy->Exist == true)
				{
					if (CheckHitBC((Enemy->HitRadius + Bomb->HitRadius),
						Enemy->Pos,
						Bomb->Pos,
						Bomb->Pos,
						0.0f) == true)
					{
						Enemy->HP = 0;
						EnemyDeadEvent(Enemy_No);
					}
				}
			}
			// エネミーの弾
			// 弾幕のポインターを初期化
			Barrage_No = 0;
			Barrage = GetBarrage(Barrage_No);
			for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
			{
				if (Barrage->Phase != BarrageOver)
				{
					for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
					{
						if (Barrage->Bullet[Bullet_No].Use == true)
						{
							if (CheckHitBC((Barrage->Bullet[Bullet_No].HitRadius + Bomb->HitRadius),
								Barrage->Bullet[Bullet_No].Pos,
								Bomb->Pos,
								Bomb->Pos,
								0.0f) == true)
							{
								AddScore(ClearBulletScore);
								Barrage->Bullet[Bullet_No].Use = false;
								SetBulletDeadEffect(Barrage->Bullet[Bullet_No].Pos);
							}
						}
					}
				}
			}
		}
		else if (Boss->Exist == true)
		{
			// ボス
			if (Boss->BombDamage == false &&
				CheckHitBC((Boss->HitRadius + Bomb->HitRadius),
					Boss->Pos, Bomb->Pos, Bomb->Pos, 0) == true)
			{
				if (Player->Power <= 1.0f)
				{
					Boss->HP -= 500.0f;
				}
				else
				{
					Boss->HP -= Player->Power * 500.0f;
				}
				Boss->BombDamage = true;
				if (Boss->HP <= 0)
				{
					Boss->HP = 0;
					if (AddBossScore == false)
					{
						AddScore(KillBossScore);
						AddBossScore = true;
					}
				}
				else
				{
					AddBossScore = false;
				}
			}
			// ボスの弾
			for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
			{
				if (Boss->Bullet[Bullet_No].Use == true)
				{
					if (CheckHitBC((Boss->Bullet[Bullet_No].HitRadius + Bomb->HitRadius),
						Boss->Bullet[Bullet_No].Pos,
						Bomb->Pos,
						Bomb->Pos,
						0.0f) == true)
					{
						AddScore(ClearBulletScore);
						Boss->Bullet[Bullet_No].Use = false;
						SetBulletDeadEffect(Boss->Bullet[Bullet_No].Pos);
					}
				}
			}
			// ボスのレーザー
			Laser_No = 0;
			Laser = GetLaser(Laser_No);
			for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
			{
				if (Laser->Use == true)
				{
					if (CheckHitBC((1.0f + Bomb->HitRadius),
						Laser->Pos,
						Bomb->Pos,
						Bomb->Pos,
						0) == true)
					{
						AddScore(ClearBulletScore);
						Laser->Use = false;
						SetBulletDeadEffect(Laser->Pos);
					}
				}
			}
		}
	}

	// ボムの弾とエネミー
	if (Bomb->Use == true && Player->Type == Player_B)
	{
		for (BombBullet_No = 0; BombBullet_No < BombBullet_Max; BombBullet_No++)
		{
			if (Bomb->Bullet[BombBullet_No].Use == true)
			{
				// 弾とエネミー
				if (Boss->Exist == false)
				{
					Enemy = GetEnemy(Bomb->BulletTarget[BombBullet_No]);
					if (CheckHitBC((Bomb->Bullet[BombBullet_No].HitRadius + Enemy->HitRadius),
						Enemy->Pos,
						Bomb->Bullet[BombBullet_No].Pos,
						Bomb->Bullet[BombBullet_No].PreviousPos,
						0.0f) == true)
					{
						// 着弾効果音
						StopSound(SE_ShotBombBullet);
						PlaySound(SE_BombBulletExplosion, E_DS8_FLAG_NONE, true);
						Enemy->HP = 0;
						EnemyDeadEvent(Bomb->BulletTarget[BombBullet_No]);
						Bomb->Bullet[BombBullet_No].Use = false;
						Bomb->LockOn[BombBullet_No].Use = false;
						Bomb->Bullet[BombBullet_No].State = Explosion;
					}
				}
				// 弾とボス
				else
				{
					if (CheckHitBC((Bomb->Bullet[BombBullet_No].HitRadius + Boss->HitRadius),
						Boss->Pos,
						Bomb->Bullet[BombBullet_No].Pos,
						Bomb->Bullet[BombBullet_No].PreviousPos,
						0.0f) == true)
					{
						// 着弾効果音
						StopSound(SE_ShotBombBullet);
						PlaySound(SE_BombBulletExplosion, E_DS8_FLAG_NONE, true);
						Boss->BombDamage = true;
						if (Player->Power <= 1.0f)
						{
							Boss->HP -= 50.0f;
						}
						else
						{
							Boss->HP -= 50.0f * Player->Power;
						}
						if (Boss->HP <= 0)
						{
							Boss->HP = 0;
							if (AddBossScore == false)
							{
								AddScore(KillBossScore);
								AddBossScore = true;
							}
						}
						else
						{
							AddBossScore = false;
						}
						Bomb->Bullet[BombBullet_No].Use = false;
						Bomb->Bullet[BombBullet_No].State = Explosion;
						Bomb->LockOn[BombBullet_No].Use = false;
					}
				}
			}
			// 衝撃波と弾
			else if (Bomb->Bullet[BombBullet_No].State == Explosion)
			{
				if (Boss->Exist == false)
				{
					// エネミーの弾
					// 弾幕のポインターを初期化
					Barrage_No = 0;
					Barrage = GetBarrage(Barrage_No);
					for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
					{
						if (Barrage->Phase != BarrageOver)
						{
							for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
							{
								if (Barrage->Bullet[Bullet_No].Use == true)
								{
									if (CheckHitBC(
										(Barrage->Bullet[Bullet_No].HitRadius + Bomb->ExplosionRadius[BombBullet_No]),
										Barrage->Bullet[Bullet_No].Pos,
										Bomb->Bullet[BombBullet_No].Pos,
										Bomb->Bullet[BombBullet_No].Pos,
										0.0f) == true)
									{
										AddScore(ClearBulletScore);
										Barrage->Bullet[Bullet_No].Use = false;
										SetBulletDeadEffect(Barrage->Bullet[Bullet_No].Pos);
									}
								}
							}
						}
					}
				}
				else
				{
					// ボスの弾
					for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
					{
						if (Boss->Bullet[Bullet_No].Use == true)
						{
							if (CheckHitBC((Boss->Bullet[Bullet_No].HitRadius + Bomb->ExplosionRadius[BombBullet_No]),
								Boss->Bullet[Bullet_No].Pos,
								Bomb->Bullet[BombBullet_No].Pos,
								Bomb->Bullet[BombBullet_No].Pos,
								0.0f) == true)
							{
								AddScore(ClearBulletScore);
								Boss->Bullet[Bullet_No].Use = false;
								SetBulletDeadEffect(Boss->Bullet[Bullet_No].Pos);
							}
						}
					}
					// ボスのレーザー
					Laser_No = 0;
					Laser = GetLaser(Laser_No);
					for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
					{
						if (Laser->Use == true)
						{
							if (CheckHitBC((1.0f + Bomb->ExplosionRadius[BombBullet_No]),
								Laser->Pos,
								Bomb->Bullet[BombBullet_No].Pos,
								Bomb->Bullet[BombBullet_No].Pos,
								0.0f) == true)
							{
								AddScore(ClearBulletScore);
								Laser->Use = false;
								SetBulletDeadEffect(Laser->Pos);
							}
						}
					}
				}
			}
		}
	}

	// エネミーの弾とプレイヤー
	if (Boss->Exist == false)
	{
		// 弾幕ポインタを初期化
		Barrage_No = 0;
		Barrage = GetBarrage(Barrage_No);
		for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
		{
			if (Barrage->Phase != BarrageOver)
			{
				for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
				{
					if (Barrage->Bullet[Bullet_No].Use == true)
					{
						if (CheckHitBC((Player->HitRadius + Barrage->Bullet[Bullet_No].HitRadius),
							Player->Pos,
							Barrage->Bullet[Bullet_No].Pos,
							Barrage->Bullet[Bullet_No].PreviousPos,
							Barrage->Bullet[Bullet_No].Speed) == true)
						{
							Barrage->Bullet[Bullet_No].Use = false;
							if (Player->InBomb == false)
							{
								if (Player->InvincibleCount == 0)
								{
									PlayerEvent();
								}
							}
						}
					}
				}
			}
		}
	}
	// ボスの弾とプレイヤー
	else if (Boss->Exist == true)
	{
		for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				if (CheckHitBC((Player->HitRadius + Boss->Bullet[Bullet_No].HitRadius),
					Player->Pos,
					Boss->Bullet[Bullet_No].Pos,
					Boss->Bullet[Bullet_No].PreviousPos,
					Boss->Bullet[Bullet_No].Speed) == true)
				{
					Boss->Bullet[Bullet_No].Use = false;
					if (Player->InBomb == false)
					{
						if (Player->InvincibleCount == 0)
						{
							PlayerEvent();
						}
					}
				}
			}
		}
	}

	// レーザーとプレイヤー
	// レーザーポインタを初期化
	Laser_No = 0;
	Laser = GetLaser(Laser_No);
	for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
	{
		if (Laser->Use == true && Laser->InRotate == false)
		{
			if (CheckHitCR(Laser->RotateCenterPos, Laser->MoveAngle, Player->Pos, Player->HitRadius) == true)
			{
				if (Player->InBomb == false)
				{
					if (Player->InvincibleCount == 0)
					{
						PlayerEvent();
					}
				}
			}
		}
	}

	// アイテムとプレイヤー
	// アイテムポインタを初期化
	Item_No = 0;
	Item = GetItem(Item_No);
	for (Item_No = 0; Item_No < ItemNum_Max; Item_No++, Item++)
	{
		if (Item->Use == true)
		{
			if (CheckHitBC((Player->HitRadius + Item->Radius), Player->Pos, Item->Pos, Item->Pos, Item->Speed) == true)
			{
				Item->Use = false;
				// 効果音
				PlaySound(SE_GetItem, E_DS8_FLAG_NONE, true);
				switch (Item->Type)
				{
				case HPItem:
					Player->HP += HPItemValue;
					if (Player->HP >= PlayerHP_Max)
					{
						Player->HP = PlayerHP_Max;
					}
					break;
				case ScoreItem:
					AddScore(ScoreItemValue);
					break;
				case PowerItem:
					Player->Power += PowerItemValue;
					if (Player->Power >= PlayerPower_Max)
					{
						Player->Power = PlayerPower_Max;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	return;
}

//=============================================================================
// 当たり判定（直線と円）
//=============================================================================
bool CheckHitCP(D3DXVECTOR3 Object, float ObjectRadius, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrePos)
{
	// 当たり判定の結果
	bool Result = false;
	// Vector_A = 弾と円の中心のベクトル
	VECTOR Vector_A;
	// Vector_B = 弾と1フレイム前の弾のベクトル
	VECTOR Vector_B;
	// Vector_Bの長さを計算
	D3DXVECTOR2 temp = D3DXVECTOR2(0.0f, 0.0f);
	// Objectの中心と弾の軌跡の距離
	float Distance = 0.0f;
	// Objectの中心は弾の移動範囲内かどうか？
	bool InRange = false;


	if (Object.x >= BulletPrePos.x && Object.x < Bullet.x		// 弾は左から右
		|| Object.x <= BulletPrePos.x && Object.x > Bullet.x)	// 弾は右から左
	{
		Vector_A.x = Bullet.x - Object.x;
		Vector_A.y = Bullet.y - Object.y;
		temp = D3DXVECTOR2(Bullet.x - BulletPrePos.x, Bullet.y - BulletPrePos.y);
		// Vector_B = 単位ベクトル
		Vector_B.Length = D3DXVec2Length(&temp);
		Vector_B.x = temp.x / Vector_B.Length;
		Vector_B.y = temp.y / Vector_B.Length;
		// Vector_AとVector_Bの外積計算
		Distance = fabsf(Vector_A.x * Vector_B.y - Vector_B.x * Vector_A.y);
		Result = Distance <= ObjectRadius ? true : false;
	}

	return Result;
}

//=============================================================================
// 当たり判定（円と円）
//=============================================================================
bool CheckHitBC(float CheckRange, D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, D3DXVECTOR3 BulletPrePos, float BulletSpeed)
{
	int i = 0;
	// 当たり判定の結果
	bool Result = false;
	// 中心点間の長さを計算
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 BulletPostemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// Objectの中心と弾の中心の距離
	float Distance = 0.0f;

	// 弾のスピードは当たり範囲より大きいなら
	if (BulletSpeed > CheckRange)
	{
		// 今の弾と1フレイム前の弾の距離を記録する
		BulletPostemp = Bullet - BulletPrePos;
		// 弾の飛行距離 / 当たり範囲 = ループ回数
		for (i = 0; i < BulletSpeed / CheckRange; i++)
		{
			// 目標と弾の距離を計算する
			temp = Object - BulletPrePos;
			Distance = D3DXVec3LengthSq(&temp);
			if (Distance <= pow(CheckRange, 2))
			{
				Result = true;
				break;
			}
			else
			{
				// 弾を進んでる、進んでる距離 = 全部の飛行距離 / ループ回数
				BulletPrePos.x += BulletPostemp.x / (BulletSpeed / CheckRange);
				BulletPrePos.y += BulletPostemp.y / (BulletSpeed / CheckRange);
				Result = false;
			}
		}
	}
	else
	{
		temp = Object - Bullet;
		Distance = D3DXVec3LengthSq(&temp);
		Result = Distance <= pow(CheckRange, 2) ? true : false;
	}

	return Result;
}

//=============================================================================
// レーザーの判定範囲（矩形）を設置する
//=============================================================================
void SetLaserCheckRect(D3DXVECTOR3 Point_LeftUp, D3DXVECTOR3 Point_RightDown)
{
	Rect.left = (long)Point_LeftUp.x;
	Rect.top = (long)Point_LeftUp.y;
	Rect.right = (long)Point_RightDown.x;
	Rect.bottom = (long)Point_RightDown.y;
	return;
}

//=============================================================================
// 当たり判定（円と矩形）
//=============================================================================
bool CheckHitCR(D3DXVECTOR3 RotateCenter, float MoveAngle, D3DXVECTOR3 Object, float ObjectRadius)
{
	// 当たり判定の結果
	bool Result = false;
	// 円と矩形の最短距離の座標
	D3DXVECTOR3 ClosestPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// RotateCenterを中心で逆回転後の座標
	D3DXVECTOR3 ObjectPos_NoRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 二つの座標間の長さを計算
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// Objectの中心と弾の中心の距離
	float Distance = 0.0f;

	// レーザーの角度によって、プレイヤーの座標を逆回転させる
	ObjectPos_NoRot.x = (Object.x - RotateCenter.x) * cosf(-MoveAngle) - (Object.y - RotateCenter.y) * sinf(-MoveAngle) + RotateCenter.x;
	ObjectPos_NoRot.y = (Object.x - RotateCenter.x) * sinf(-MoveAngle) + (Object.y - RotateCenter.y) * cosf(-MoveAngle) + RotateCenter.y;

	// プレイヤーのx座標は矩形の左側より小さいなら、最短距離のx座標 = 矩形の左側
	if (ObjectPos_NoRot.x < Rect.left)
	{
		ClosestPoint.x = (float)Rect.left;
	}
	// プレイヤーのx座標は矩形の右側より大きいなら、最短距離のx座標 = 矩形の右側
	else if (ObjectPos_NoRot.x > Rect.right)
	{
		ClosestPoint.x = (float)Rect.right;
	}
	// 以上じゃなかったら、最短距離のx座標 = プレイヤーのx座標
	else
	{
		ClosestPoint.x = ObjectPos_NoRot.x;
	}

	// プレイヤーのy座標は矩形の上側より小さいなら、最短距離のy座標 = 矩形の上側
	if (ObjectPos_NoRot.y < Rect.top)
	{
		ClosestPoint.y = (float)Rect.top;
	}
	// プレイヤーのy座標は矩形の下側より大きいなら、最短距離のy座標 = 矩形の下側
	else if (ObjectPos_NoRot.y > Rect.bottom)
	{
		ClosestPoint.y = (float)Rect.bottom;
	}
	// 以上じゃなかったら、最短距離のy座標 = プレイヤーのy座標
	else
	{
		ClosestPoint.y = ObjectPos_NoRot.y;
	}

	// 距離計算
	temp = ObjectPos_NoRot - ClosestPoint;
	Distance = D3DXVec3LengthSq(&temp);
	// 当たり判定
	Result = Distance <= pow(ObjectRadius, 2) ? true : false;

	return Result;
}

//=============================================================================
// 弾消失をチェックする
//=============================================================================
bool CheckBulletDisappear(D3DXVECTOR3 Object, D3DXVECTOR3 Bullet, float BulletRadius)
{
	D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool Result = false;
	float Distance = 0.0f;

	temp = Object - Bullet;
	Distance = D3DXVec3LengthSq(&temp);
	// 当たり判定
	// 半径 * 2 = 直径
	Result = Distance >= pow((BulletRadius * 2), 2) ? true : false;

	return Result;
}

//=============================================================================
// プレイヤーが撃たれた処理
//=============================================================================
void PlayerEvent(void)
{
	int Bullet_No = 0;
	int Barrage_No = 0;
	int Laser_No = 0;
	int GameStage = GetGameStage();
	PLAYER *Player = GetPlayer(0);
	BARRAGE *Barrage = GetBarrage(Barrage_No);
	BOSS *Boss = GetBoss();
	LASER *Laser = GetLaser(Laser_No);

	if (Boss->Exist == false)
	{
		// エネミーの弾
		Barrage_No = 0;
		Barrage = GetBarrage(Barrage_No);
		for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
		{
			if (Barrage->Phase != BarrageOver)
			{
				for (Bullet_No = 0; Bullet_No < EnemyBullet_Max; Bullet_No++)
				{
					if (Barrage->Bullet[Bullet_No].Use == true)
					{
						Barrage->Bullet[Bullet_No].Use = false;
						SetBulletDeadEffect(Barrage->Bullet[Bullet_No].Pos);
					}
				}
			}
		}
	}
	else if (Boss->Exist == true)
	{
		// ボスの弾
		for (Bullet_No = 0; Bullet_No < BossBullet_Max; Bullet_No++)
		{
			if (Boss->Bullet[Bullet_No].Use == true)
			{
				Boss->Bullet[Bullet_No].Use = false;
				SetBulletDeadEffect(Boss->Bullet[Bullet_No].Pos);
			}
		}

		// ボスのレーザー
		Laser_No = 0;
		Laser = GetLaser(Laser_No);
		for (Laser_No = 0; Laser_No < Laser_Max; Laser_No++, Laser++)
		{
			if (Laser->Use == true)
			{
				Laser->Use = false;
			}
		}
	}

	// 効果音
	PlaySound(SE_PlayerBeShot, E_DS8_FLAG_NONE, true);
	// 無敵カウント加算
	Player->InvincibleCount++;
	if (GameStage == Stage_Game || GameStage == Stage_BossPratice)
	{
		// HPは2割減る
		Player->HP -= PlayerHP_Max * 0.2f;
		if (Player->HP <= 0.0f)
		{
			Player->HP = 0.0f;
		}

		// Powerは0.5減る
		Player->Power -= 0.5f;
		if (Player->Power <= 0.0f)
		{
			Player->Power = 0.0f;
		}

		// ボムたす1
		Player->BombNum++;
		if (Player->BombNum >= PlayerBomb_Max)
		{
			Player->BombNum = PlayerBomb_Max;
		}

		// プレイヤーアイテム設置
		SetPlayerItem(Player->Pos);
	}

	return;
}


//=============================================================================
// エネミー死亡処理
//=============================================================================
void EnemyDeadEvent(int Enemy_No)
{
	int Barrage_No = 0;
	ENEMY *Enemy = GetEnemy(Enemy_No);
	BARRAGE *Barrage = GetBarrage(Barrage_No);

	// 効果音
	PlaySound(SE_EnemyDead, E_DS8_FLAG_NONE, true);
	// スコア加算
	AddScore(KillEnemyScore);
	// フラグ消す
	Enemy->Exist = false;
	// 落とすアイテム設置
	SetEnemyItem(Enemy->Pos, Enemy->Item, EnemyItem_Max);
	// 死亡エフェクト設置
	SetEnemyDeadEffect(Enemy_No);
	// 弾幕状態を変わる
	for (Barrage_No = 0; Barrage_No < Barrage_Max; Barrage_No++, Barrage++)
	{
		if (Barrage->Phase != BarrageOver)
		{
			if (Barrage->Attacker_No == Enemy_No)
			{
				Barrage->Phase = EnemyDisappear;
				break;
			}
		}
	}

	return;
}