//=============================================================================
//
// ロードファイル処理 [File.cpp]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#include "main.h"
#include "File.h"


//******************************************************************************
// マクロ定義
//******************************************************************************


//******************************************************************************
// 構造体定義
//******************************************************************************


//******************************************************************************
// プロトタイプ宣言
//******************************************************************************


//******************************************************************************
// グローバル変数
//******************************************************************************
// エネミーステータス構造体
ENEMYSTATUS	EnemyStatus[StatusType_Max];
// 弾タイプ構造体
BULLETTYPE BulletType[BulletType_Max];


//******************************************************************************
// エネミーステータスを読み込む
//******************************************************************************
HRESULT LoadEnemyStatus(void)
{
	int i = 0;
	FILE *fp;
	char FileName[] = _T("data\\EnemyFile.csv");
	// fgets取った一列文字
	char Line[512];
	// ","までの文字
	char *Result = NULL;
	// 何行目の数字
	int Column = 0;
	// エネミーステータスのタイプ番号
	int Type_No = 0;

	// 指定ルートのファイルを読み出しモードでオープンする
	fp = fopen(FileName, "r");
	if (fp == NULL)
	{
		// オープン失敗
		sprintf(Line, "Load EnemyFile Failed！");
		MessageBox(0, Line, "Error", 0);
		return E_FAIL;
	}
#if _DEBUG
	else
	{
		// オープン成功
		OutputDebugString("Load EnemyFile Success!\n");
	}
#endif

	// 一列文字を読み出す
	while (fgets(Line, sizeof(Line), fp) != NULL && Type_No < StatusType_Max)
	{
		// 最初の文字は"/"なら、次の列を読み込む
		if (Line[0] == '/')
		{
			continue;
		}

		// 最初から一つ目の","までの文字
		Result = strtok(Line, ",");

		while (Result != NULL)
		{
			switch (Column)
			{
				// 出現時間
			case 0: EnemyStatus[Type_No].AppearCount = atoi(Result); break;
				// 種類
			case 1: EnemyStatus[Type_No].EnemyType = atoi(Result); break;
				// 難易度
			case 2: EnemyStatus[Type_No].EnemyDifficulty = atoi(Result); break;
				// x座標
			case 3: EnemyStatus[Type_No].Pos.x = (float)atof(Result); break;
				// y座標
			case 4: EnemyStatus[Type_No].Pos.y = (float)atof(Result); break;
				// 移動パターン
			case 5: EnemyStatus[Type_No].MovePattern = atoi(Result); break;
				// 最大体力
			case 6: EnemyStatus[Type_No].HP_Max = (float)atof(Result); break;
				// 移動速度
			case 7: EnemyStatus[Type_No].Speed = (float)atof(Result); break;
				// 発射時間
			case 8: EnemyStatus[Type_No].ShotTime = atoi(Result); break;
				// 弾幕種類
			case 9: EnemyStatus[Type_No].BarrageType = atoi(Result); break;
				// 弾種類
			case 10:EnemyStatus[Type_No].BulletType = atoi(Result); break;
				// 弾のスピード
			case 11:EnemyStatus[Type_No].BulletSpeed = (float)atof(Result); break;
				// 待機時間
			case 12:EnemyStatus[Type_No].WaitCount = atoi(Result); break;
				// 退場時間
			case 13:EnemyStatus[Type_No].ExitCount = atoi(Result); break;
				// ドロップアイテム
			case 14:
				while (i < EnemyItem_Max)
				{
					EnemyStatus[Type_No].Item[i] = atoi(Result);
					i++;
					if (i != EnemyItem_Max)
					{
						Result = strtok(NULL, ",");
					}
				}
				break;
			default:
				break;
			}
			// 次の数字を読み出す
			Column++;
			Result = strtok(NULL, ",");
		}
		// 一行目に戻る
		Column = 0;
		i = 0;
		// 読み込むタイプ番号は次に指す
		Type_No++;
	}

	// ファイルを閉じる
	fclose(fp);

	return S_OK;
}

//******************************************************************************
// エネミーステータスの情報を取得する
//******************************************************************************
ENEMYSTATUS *GetEnemyStatus(int StatusType_No)
{
	return &EnemyStatus[StatusType_No];
}

//******************************************************************************
// 弾タイプを読み込む
//******************************************************************************
HRESULT LoadBulletType(void)
{
	FILE *fp;
	char FileName[] = _T("data\\BulletFile.csv");
	char Line[256];
	char *Result = NULL;
	int Column = 0;
	int Type_No = 0;

	fp = fopen(FileName, "r");
	if (fp == NULL)
	{
		// オープン失敗
		sprintf(Line, "Load BulletFile Failed！");
		MessageBox(0, Line, "Error", 0);
		return E_FAIL;
	}
#ifdef _DEBUG
	else
	{
		// オープン成功
		OutputDebugString("Load BulletFile Success!\n");
	}
#endif

	// 一列文字を読み出す
	while (fgets(Line, sizeof(Line), fp) != NULL && Type_No < BulletType_Max)
	{
		// 最初の文字は"/"なら、次の列を読み込む
		if (Line[0] == '/')
		{
			continue;
		}

		// 最初から一つ目の","までの文字
		Result = strtok(Line, ",");

		while (Result != NULL)
		{
			switch (Column)
			{
				// 弾の横幅
			case 0: BulletType[Type_No].Width = atoi(Result); break;
				// 弾の縦幅
			case 1: BulletType[Type_No].Height = atoi(Result); break;
				// 中心から頂点までの角度
			case 2: BulletType[Type_No].BaseAngle = (float)atof(Result); break;
				// 半径
			case 3: BulletType[Type_No].Radius = (float)atof(Result); break;
				// 当たり判定の半径
			case 4: BulletType[Type_No].HitRadius = (float)atof(Result); break;
				// テクスチャの回転角度
			case 5: BulletType[Type_No].Rot_z = DegreeToRadian((float)atof(Result)); break;
				// テクスチャ内分割数（横)
			case 6: BulletType[Type_No].Devide_X = atoi(Result); break;
			default:
				break;
			}
			// 次の数字を読み出す
			Column++;
			Result = strtok(NULL, ",");
		}
		// 一行目に戻る
		Column = 0;
		// 読み込むタイプ番号は次に指す
		Type_No++;
	}

	// ファイルを閉じる
	fclose(fp);

	return S_OK;
}

//******************************************************************************
// 弾タイプの情報を取得する
//******************************************************************************
BULLETTYPE *GetBulletType(int Type_No)
{
	return &BulletType[Type_No];
}

