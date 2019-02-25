//=============================================================================
//
// ���[�h�t�@�C������ [File.cpp]
// Author�FTH_GP11_GP11B341_35_���M��
//
//=============================================================================
#include "main.h"
#include "File.h"


//******************************************************************************
// �}�N����`
//******************************************************************************


//******************************************************************************
// �\���̒�`
//******************************************************************************



//******************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************



//******************************************************************************
// �O���[�o���ϐ�
//******************************************************************************
ENEMYSTATUS	EnemyStatus[StatusType_Max];
BULLETTYPE BulletType[BulletType_Max];


//******************************************************************************
// �O���[�o���ϐ�
//******************************************************************************
HRESULT LoadEnemyStatus(void)
{
	int i = 0;
	FILE *fp;
	char FileName[] = _T("data\\EnemyFile.csv");
	//char FileName[] = _T("data\\EnemyFileTest.csv");
	// fgets�������񕶎�
	char Line[512];
	// ","�܂ł̕���
	char *Result = NULL;
	// ���s�ڂ̐���
	int Column = 0;
	// �G�l�~�[�X�e�[�^�X�̃^�C�v�ԍ�
	int Type_No = 0;

	// �w�胋�[�g�̃t�@�C����ǂݏo�����[�h�ŃI�[�v������
	fp = fopen(FileName, "r");
	if (fp == NULL)
	{
		// �I�[�v�����s
		sprintf(Line, "Load EnemyFile Failed�I");
		MessageBox(0, Line, "Error", 0);
		return E_FAIL;
	}
#ifdef _DEBUG
	else
	{
		// �I�[�v������
		OutputDebugString("Load EnemyFile Success!\n");
	}
#endif

	// ��񕶎���ǂݏo��
	while (fgets(Line, sizeof(Line), fp) != NULL && Type_No < StatusType_Max)
	{
		// �ŏ��̕�����"/"�Ȃ�A���̗��ǂݍ���
		if (Line[0] == '/')
		{
			continue;
		}

		// �ŏ������ڂ�","�܂ł̕���
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
			// ���̐�����ǂݏo��
			Column++;
			Result = strtok(NULL, ",");
		}
		// ��s�ڂɖ߂�
		Column = 0;
		i = 0;
		// �ǂݍ��ރ^�C�v�ԍ��͎��Ɏw��
		Type_No++;
	}

	// �t�@�C�������
	fclose(fp);

	return S_OK;
}

//******************************************************************************
// �O���[�o���ϐ�
//******************************************************************************
ENEMYSTATUS *GetEnemyStatus(int StatusType_No)
{
	return &EnemyStatus[StatusType_No];
}

//******************************************************************************
// �O���[�o���ϐ�
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
		// �I�[�v�����s
		sprintf(Line, "Load BulletFile Failed�I");
		MessageBox(0, Line, "Error", 0);
		return E_FAIL;
	}
#ifdef _DEBUG
	else
	{
		// �I�[�v������
		OutputDebugString("Load BulletFile Success!\n");
	}
#endif

	// ��񕶎���ǂݏo��
	while (fgets(Line, sizeof(Line), fp) != NULL && Type_No < BulletType_Max)
	{
		// �ŏ��̕�����"/"�Ȃ�A���̗��ǂݍ���
		if (Line[0] == '/')
		{
			continue;
		}

		// �ŏ������ڂ�","�܂ł̕���
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
			// ���̐�����ǂݏo��
			Column++;
			Result = strtok(NULL, ",");
		}
		// ��s�ڂɖ߂�
		Column = 0;
		// �ǂݍ��ރ^�C�v�ԍ��͎��Ɏw��
		Type_No++;
	}

	// �t�@�C�������
	fclose(fp);

	return S_OK;
}

//******************************************************************************
// �O���[�o���ϐ�
//******************************************************************************
BULLETTYPE *GetBulletType(int Type_No)
{
	return &BulletType[Type_No];
}

