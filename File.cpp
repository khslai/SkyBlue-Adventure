//=============================================================================
//
// ロードファイル処理 [File.cpp]
// Author：TH_GP11_GP11B341_35_頼凱興
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
ENEMYSTATUS	EnemyStatus[StatusType_Max];
BULLETTYPE BulletType[BulletType_Max];


//******************************************************************************
// グローバル変数
//******************************************************************************
HRESULT LoadEnemyStatus(void)
{
	int i = 0;
	FILE *fp;
	char FileName[] = _T("data\\EnemyFile.csv");
	//char FileName[] = _T("data\\EnemyFileTest.csv");
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
#ifdef _DEBUG
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
			case 0: EnemyStatus[Type_No].AppearCount = atoi(Result); break;
			case 1: EnemyStatus[Type_No].EnemyType = atoi(Result); break;
			case 2: EnemyStatus[Type_No].EnemyDifficulty = atoi(Result); break;
			case 3: EnemyStatus[Type_No].Pos.x = (float)atof(Result); break;
			case 4: EnemyStatus[Type_No].Pos.y = (float)atof(Result); break;
			case 5: EnemyStatus[Type_No].MovePattern = atoi(Result); break;
			case 6: EnemyStatus[Type_No].HP_Max = (float)atof(Result); break;
			case 7: EnemyStatus[Type_No].Speed = (float)atof(Result); break;
			case 8: EnemyStatus[Type_No].ShotTime = atoi(Result); break;
			case 9: EnemyStatus[Type_No].BarrageType = atoi(Result); break;
			case 10:EnemyStatus[Type_No].BulletType = atoi(Result); break;
			case 11:EnemyStatus[Type_No].BulletSpeed = (float)atof(Result); break;
			case 12:EnemyStatus[Type_No].WaitCount = atoi(Result); break;
			case 13:EnemyStatus[Type_No].ExitCount = atoi(Result); break;
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
// グローバル変数
//******************************************************************************
ENEMYSTATUS *GetEnemyStatus(int StatusType_No)
{
	return &EnemyStatus[StatusType_No];
}

//******************************************************************************
// グローバル変数
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
			case 0: BulletType[Type_No].Width = atoi(Result); break;
			case 1: BulletType[Type_No].Height = atoi(Result); break;
			case 2: BulletType[Type_No].BaseAngle = (float)atof(Result); break;
			case 3: BulletType[Type_No].Radius = (float)atof(Result); break;
			case 4: BulletType[Type_No].HitRadius = (float)atof(Result); break;
			case 5: BulletType[Type_No].Rot_z = DegreeToRadian((float)atof(Result)); break;
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
// グローバル変数
//******************************************************************************
BULLETTYPE *GetBulletType(int Type_No)
{
	return &BulletType[Type_No];
}

