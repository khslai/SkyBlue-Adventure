//=============================================================================
//
// ���[�h�t�@�C������ [File.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
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
// �G�l�~�[�X�e�[�^�X�\����
ENEMYSTATUS	EnemyStatus[StatusType_Max];
// �e�^�C�v�\����
BULLETTYPE BulletType[BulletType_Max];


//******************************************************************************
// �G�l�~�[�X�e�[�^�X��ǂݍ���
//******************************************************************************
HRESULT LoadEnemyStatus(void)
{
	int i = 0;
	FILE *fp;
	char FileName[] = _T("data\\EnemyFile.csv");
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
#if _DEBUG
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
				// �o������
			case 0: EnemyStatus[Type_No].AppearCount = atoi(Result); break;
				// ���
			case 1: EnemyStatus[Type_No].EnemyType = atoi(Result); break;
				// ��Փx
			case 2: EnemyStatus[Type_No].EnemyDifficulty = atoi(Result); break;
				// x���W
			case 3: EnemyStatus[Type_No].Pos.x = (float)atof(Result); break;
				// y���W
			case 4: EnemyStatus[Type_No].Pos.y = (float)atof(Result); break;
				// �ړ��p�^�[��
			case 5: EnemyStatus[Type_No].MovePattern = atoi(Result); break;
				// �ő�̗�
			case 6: EnemyStatus[Type_No].HP_Max = (float)atof(Result); break;
				// �ړ����x
			case 7: EnemyStatus[Type_No].Speed = (float)atof(Result); break;
				// ���ˎ���
			case 8: EnemyStatus[Type_No].ShotTime = atoi(Result); break;
				// �e�����
			case 9: EnemyStatus[Type_No].BarrageType = atoi(Result); break;
				// �e���
			case 10:EnemyStatus[Type_No].BulletType = atoi(Result); break;
				// �e�̃X�s�[�h
			case 11:EnemyStatus[Type_No].BulletSpeed = (float)atof(Result); break;
				// �ҋ@����
			case 12:EnemyStatus[Type_No].WaitCount = atoi(Result); break;
				// �ޏꎞ��
			case 13:EnemyStatus[Type_No].ExitCount = atoi(Result); break;
				// �h���b�v�A�C�e��
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
// �G�l�~�[�X�e�[�^�X�̏����擾����
//******************************************************************************
ENEMYSTATUS *GetEnemyStatus(int StatusType_No)
{
	return &EnemyStatus[StatusType_No];
}

//******************************************************************************
// �e�^�C�v��ǂݍ���
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
				// �e�̉���
			case 0: BulletType[Type_No].Width = atoi(Result); break;
				// �e�̏c��
			case 1: BulletType[Type_No].Height = atoi(Result); break;
				// ���S���璸�_�܂ł̊p�x
			case 2: BulletType[Type_No].BaseAngle = (float)atof(Result); break;
				// ���a
			case 3: BulletType[Type_No].Radius = (float)atof(Result); break;
				// �����蔻��̔��a
			case 4: BulletType[Type_No].HitRadius = (float)atof(Result); break;
				// �e�N�X�`���̉�]�p�x
			case 5: BulletType[Type_No].Rot_z = DegreeToRadian((float)atof(Result)); break;
				// �e�N�X�`�����������i��)
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
// �e�^�C�v�̏����擾����
//******************************************************************************
BULLETTYPE *GetBulletType(int Type_No)
{
	return &BulletType[Type_No];
}

