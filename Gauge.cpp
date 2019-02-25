//=============================================================================
//
// �Q�[�W���� [Gauge.cpp]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#include "main.h"
#include "Gauge.h"
#include "Score.h"
#include "Player.h"
#include "Boss.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �Q�[�W�e�N�X�`���̍ő吔
#define GaugeTexture_Max	(3)

// �Q�[�W�̎��
enum
{
	PlayerHP,			// �v���C���[��HP
	PlayerPower,		// �v���C���[��Power
	BossHP,				// �{�X��HP
	PlayerHPBox,		// �v���C���[HP�Q�[�W�̘g
	PlayerPowerBox,		// �v���C���[Power�Q�[�W�̘g
	BossHPBox,			// �{�XHP�Q�[�W�̘g
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ���_�̍쐬
HRESULT MakeGaugeVertex(GAUGE *GaugePtr);
HRESULT MakeSymbolVertex(void);
// ���_���W�̐ݒ�
void SetGaugeVertex(GAUGE *GaugePtr);
void SetSymbolVertex(void);
// �e�N�X�`�����W�̐ݒ�
void SetGaugeTexture(GAUGE *GaugePtr);
void SetSymbolTexture(void);
// �F�A�����x�̐ݒ�
void SetGaugeDiffuse(GAUGE *GaugePtr, int Color);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 GaugeBoxTexture[GaugeTexture_Max] = { NULL };
LPDIRECT3DTEXTURE9 GaugeTexture[GaugeTexture_Max] = { NULL };
LPDIRECT3DTEXTURE9 BombSymbolTexture = NULL;
// �Q�[�W�\����
GAUGE PlayerHPGauge;
GAUGE PlayerHPGaugeBox;
GAUGE PlayerPowerGauge;
GAUGE PlayerPowerGaugeBox;
GAUGE PlayerBombSymbol[PlayerBomb_Max];
GAUGE BossHPGauge;
GAUGE BossHPGaugeBox;
// �Q�[�W�̃p�[�Z���g
float GaugePercent_PlayerHP = 1.0f;
float GaugePercent_PlayerPower = PlayerStartPower / PlayerPower_Max;
float GaugePercent_BossHP = 1.0f;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGauge(int InitState)
{
	PlayerHPGauge.Type = PlayerHP;
	PlayerHPGaugeBox.Type = PlayerHPBox;
	PlayerPowerGauge.Type = PlayerPower;
	PlayerPowerGaugeBox.Type = PlayerPowerBox;
	BossHPGauge.Type = BossHP;
	BossHPGaugeBox.Type = BossHPBox;

	GaugePercent_PlayerHP = 1.0f;
	GaugePercent_PlayerPower = PlayerStartPower / PlayerPower_Max;
	GaugePercent_BossHP = 1.0f;

	// ���_���̍쐬
	MakeGaugeVertex(&PlayerHPGauge);
	MakeGaugeVertex(&PlayerHPGaugeBox);
	MakeGaugeVertex(&PlayerPowerGauge);
	MakeGaugeVertex(&PlayerPowerGaugeBox);
	MakeGaugeVertex(&BossHPGauge);
	MakeGaugeVertex(&BossHPGaugeBox);
	MakeSymbolVertex();

	if (InitState == FirstInit)
	{
		// �e�N�X�`���̓ǂݍ���
		if (SafeLoad(Texture_Gauge_Player, &GaugeTexture[PlayerHP], "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GaugeBox_PlayerHP, &GaugeBoxTexture[PlayerHP], "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Gauge_Player, &GaugeTexture[PlayerPower], "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GaugeBox_PlayerPower, &GaugeBoxTexture[PlayerPower], "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_BombSymbol, &BombSymbolTexture, "Player") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_Gauge_Boss, &GaugeTexture[BossHP], "Boss") == false)
		{
			return E_FAIL;
		}
		if (SafeLoad(Texture_GaugeBox_Boss, &GaugeBoxTexture[BossHP], "Boss") == false)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGauge(void)
{
	for (int i = 0; i < GaugeTexture_Max; i++)
	{
		SafeRelease(GaugeTexture[i]);
		SafeRelease(GaugeBoxTexture[i]);
	}
	SafeRelease(BombSymbolTexture);

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGauge(void)
{
	int GameCount = GetGameCount();
	static int TimeCount = 0;
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();

	// �v���C���[HP�Q�[�W
	// �ΐF
	if (GaugePercent_PlayerHP >= 0.6f)
	{
		SetGaugeDiffuse(&PlayerHPGauge, Green);
		StopSound(SE_HPAlarm);
	}
	// ���F
	else if (GaugePercent_PlayerHP > 0.2f && GaugePercent_PlayerHP < 0.6f)
	{
		SetGaugeDiffuse(&PlayerHPGauge, Yellow);
		StopSound(SE_HPAlarm);
	}
	// �ԐF
	else
	{
		if (GameCount >= GameClearCount)
		{
			StopSound(SE_HPAlarm);
		}
		else
		{
			PlaySound(SE_HPAlarm, E_DS8_FLAG_LOOP, false);
		}
		TimeCount++;
		if (TimeCount < 10)
		{
			SetGaugeDiffuse(&PlayerHPGauge, Red);
		}
		else if (TimeCount >= 10 && TimeCount < 20)
		{
			SetGaugeDiffuse(&PlayerHPGauge, Red_AlphaHalf);
		}
		else if (TimeCount >= 20)
		{
			TimeCount = 0;
		}
	}

	SetGaugeTexture(&PlayerHPGauge);
	SetGaugeVertex(&PlayerHPGauge);

	SetGaugeTexture(&PlayerHPGaugeBox);
	SetGaugeVertex(&PlayerHPGaugeBox);

	// �v���C���[�p���[�Q�[�W
	if (GaugePercent_PlayerPower < 1.0f)
	{
		SetGaugeDiffuse(&PlayerPowerGauge, SkyBlue);
	}
	else
	{
		SetGaugeDiffuse(&PlayerPowerGauge, PowerGaugeGradation);
	}

	SetGaugeTexture(&PlayerPowerGauge);
	SetGaugeVertex(&PlayerPowerGauge);

	SetGaugeTexture(&PlayerPowerGaugeBox);
	SetGaugeVertex(&PlayerPowerGaugeBox);

	// �v���C���[�{��
	SetSymbolTexture();

	// �{�XHP�Q�[�W
	if (Boss->Exist == true)
	{
		if (GaugePercent_BossHP >= 0.6f)
		{
			SetGaugeDiffuse(&BossHPGauge, Green);
		}
		else if (GaugePercent_BossHP > 0.2f && GaugePercent_BossHP < 0.6f)
		{
			SetGaugeDiffuse(&BossHPGauge, Yellow);
		}
		else
		{
			SetGaugeDiffuse(&BossHPGauge, Red);
		}

		SetGaugeTexture(&BossHPGauge);
		SetGaugeVertex(&BossHPGauge);

		SetGaugeTexture(&BossHPGaugeBox);
		SetGaugeVertex(&BossHPGaugeBox);

	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();

	Device->SetFVF(FVF_VERTEX_2D);

	Device->SetTexture(0, GaugeTexture[PlayerHP]);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PlayerHPGauge.VertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, GaugeBoxTexture[PlayerHP]);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PlayerHPGaugeBox.VertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, GaugeTexture[PlayerPower]);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PlayerPowerGauge.VertexWk, sizeof(VERTEX_2D));

	Device->SetTexture(0, GaugeBoxTexture[PlayerPower]);
	Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PlayerPowerGaugeBox.VertexWk, sizeof(VERTEX_2D));

	for (int i = 0; i < PlayerBomb_Max; i++)
	{
		Device->SetTexture(0, BombSymbolTexture);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, PlayerBombSymbol[i].VertexWk, sizeof(VERTEX_2D));
	}

	if (Boss->Exist == true && Boss->State != BossDisappear)
	{
		Device->SetTexture(0, GaugeBoxTexture[BossHP]);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BossHPGaugeBox.VertexWk, sizeof(VERTEX_2D));

		Device->SetTexture(0, GaugeTexture[BossHP]);
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, Num_Polygon, BossHPGauge.VertexWk, sizeof(VERTEX_2D));
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeGaugeVertex(GAUGE *GaugePtr)
{
	// ���_���W�̐ݒ�	
	SetGaugeVertex(GaugePtr);

	// rhw�̐ݒ�
	GaugePtr->VertexWk[0].rhw = 1.0f;
	GaugePtr->VertexWk[1].rhw = 1.0f;
	GaugePtr->VertexWk[2].rhw = 1.0f;
	GaugePtr->VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	GaugePtr->VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GaugePtr->VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GaugePtr->VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	GaugePtr->VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetGaugeTexture(GaugePtr);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetGaugeTexture(GAUGE *GaugePtr)
{
	GaugePtr->VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	GaugePtr->VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	GaugePtr->VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	GaugePtr->VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return;
}



//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetGaugeVertex(GAUGE *GaugePtr)
{
	int GameStage = GetGameStage();
	float PlayerHPPercent = 0.0f;
	float PlayerPowerPercent = 0.0f;
	float BossHPPercent = 0.0f;
	PLAYER *Player = GetPlayer(0);
	BOSS *Boss = GetBoss();

	switch (GaugePtr->Type)
	{
	case PlayerHP:
		// ���݃v���C���[HP�̊���
		PlayerHPPercent = (float)Player->HP / (float)PlayerHP_Max;

		// 1�t���C���O��HP %�͌��݂�葽����΁A����������
		if (GaugePercent_PlayerHP > PlayerHPPercent)
		{
			GaugePercent_PlayerHP -= 0.01f;
			// �Q�[�W�Œ�
			if (GaugePercent_PlayerHP <= PlayerHPPercent)
			{
				GaugePercent_PlayerHP = PlayerHPPercent;
			}
			// �Q�[�W��0�ɂȂ�����A�Q�[���I�[�o�[
			if (GaugePercent_PlayerHP <= 0.0f)
			{
				GaugePercent_PlayerHP = 0.0f;
				SetGameStage(Stage_GameOver);
			}
		}
		// 1�t���C���O��HP %�͌��݂�菭�Ȃ���΁A���������₷
		else if (GaugePercent_PlayerHP < PlayerHPPercent)
		{
			GaugePercent_PlayerHP += 0.01f;
			// �Q�[�W�Œ�
			if (GaugePercent_PlayerHP >= PlayerHPPercent)
			{
				GaugePercent_PlayerHP = PlayerHPPercent;
			}
			//�@100����������A100���ɂȂ�
			if (GaugePercent_PlayerHP >= 1.0f)
			{
				GaugePercent_PlayerHP = 1.0f;
			}
		}
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X, PlayerHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X + Texture_Gauge_Player_Width * GaugePercent_PlayerHP, PlayerHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X, PlayerHPGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X + Texture_Gauge_Player_Width * GaugePercent_PlayerHP, PlayerHPGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		break;
	case PlayerHPBox:
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X, PlayerHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X + Texture_Gauge_Player_Width, PlayerHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X, PlayerHPGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(PlayerHPGauge_Pos_X + Texture_Gauge_Player_Width, PlayerHPGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		break;
	case PlayerPower:
		// ���݃v���C���[Power�̊���
		PlayerPowerPercent = (float)Player->Power / (float)PlayerPower_Max;

		// 1�t���C���O��Power %�͌��݂�葽����΁A����������
		if (GaugePercent_PlayerPower > PlayerPowerPercent)
		{
			GaugePercent_PlayerPower -= 0.01f;
			// �Q�[�W�Œ�
			if (GaugePercent_PlayerPower <= PlayerPowerPercent)
			{
				GaugePercent_PlayerPower = PlayerPowerPercent;
			}
			//�@0%��菬�����A0�ɂȂ�
			if (GaugePercent_PlayerPower <= 0.0f)
			{
				GaugePercent_PlayerPower = 0.0f;
			}
		}
		// 1�t���C���O��Power %�͌��݂�菭�Ȃ���΁A���������₷
		else if (GaugePercent_PlayerPower < PlayerPowerPercent)
		{
			GaugePercent_PlayerPower += 0.01f;
			// �Q�[�W�Œ�
			if (GaugePercent_PlayerPower >= PlayerPowerPercent)
			{
				GaugePercent_PlayerPower = PlayerPowerPercent;
			}
			//�@100����������A100���ɂȂ�
			if (GaugePercent_PlayerPower >= 1.0f)
			{
				GaugePercent_PlayerPower = 1.0f;
			}
		}
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X, PlayerPowerGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X + Texture_Gauge_Player_Width * GaugePercent_PlayerPower, PlayerPowerGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X, PlayerPowerGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X + Texture_Gauge_Player_Width * GaugePercent_PlayerPower, PlayerPowerGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		break;
	case PlayerPowerBox:
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X, PlayerPowerGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X + Texture_Gauge_Player_Width, PlayerPowerGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X, PlayerPowerGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(PlayerPowerGauge_Pos_X + Texture_Gauge_Player_Width, PlayerPowerGauge_Pos_Y + Texture_Gauge_Player_Height, 0);
		break;
	case BossHP:
		// ���݃{�XHP�̊���
		BossHPPercent = (float)Boss->HP / (float)Boss->HP_Max;

		// 1�t���C���O��HP %�͌��݂�葽����΁A����������
		if (GaugePercent_BossHP > BossHPPercent && Boss->State != HPRecover && Boss->State != BossDisappear)
		{
			GaugePercent_BossHP -= 0.01f;
			// �Q�[�W�Œ�
			if (GaugePercent_BossHP <= BossHPPercent)
			{
				GaugePercent_BossHP = BossHPPercent;
			}
			// �Q�[�W��0�ɂȂ�����A�{�X�͎��̒i�K�����
			if (GaugePercent_BossHP <= 0.0f)
			{
				PlaySound(SE_BossHPZero, E_DS8_FLAG_NONE, true);
				GaugePercent_BossHP = 0.0f;
				Boss->State = HPZero;
				// �c�@�������
				if (Boss->Life == 0)
				{
					if (GameStage == Stage_Game)
					{
						// ���̂͒��{�X
						if (Boss->MiddleBossOver == false)
						{
							// ���{�X�I��
							Boss->MiddleBossOver = true;
							Boss->State = MiddleBossOver;
							GaugePercent_BossHP = 1.0f;
						}
						// ���̂̓��X�{�X
						else if (Boss->MiddleBossOver == true)
						{
							// ���X�{�X�I��
							Boss->State = LastBossOver;
							GaugePercent_BossHP = 1.0f;
						}
						// // ���̂̓��X�{�X
						//if (Boss->Phase == LastBoss_LastPhase)
						//{
						//	 ���X�{�X�I��
						//	Boss->State = LastBossOver;
						//	GaugePercent_BossHP = 1.0f;
						//}
						//else if (Boss->Phase == MiddleBoss_LastPhase)
						//{
						//	// ���{�X�I��
						//	Boss->State = MiddleBossOver;
						//	GaugePercent_BossHP = 1.0f;
						//}
					}
					else if (GameStage == Stage_BossPratice)
					{
						// �{�X���K���[�h�I��
						Boss->State = LastBossOver;
					}
				}
			}
		}
		// HP��
		else if (Boss->State == HPRecover)
		{
			GaugePercent_BossHP += 0.01f;
			// 100%�ɂȂ�����A���̒i�K�����
			if (GaugePercent_BossHP >= 1.0f)
			{
				GaugePercent_BossHP = 1.0f;
				Boss->State = NextPhase;
			}
		}
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(BossHPGauge_Pos_X, BossHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(BossHPGauge_Pos_X + Texture_Gauge_Boss_Width, BossHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(BossHPGauge_Pos_X, BossHPGauge_Pos_Y - Texture_Gauge_Boss_Height * GaugePercent_BossHP, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(BossHPGauge_Pos_X + Texture_Gauge_Boss_Width, BossHPGauge_Pos_Y - Texture_Gauge_Boss_Height * GaugePercent_BossHP, 0);
		break;
	case BossHPBox:
		GaugePtr->VertexWk[0].vtx = D3DXVECTOR3(BossHPGauge_Pos_X, BossHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[1].vtx = D3DXVECTOR3(BossHPGauge_Pos_X + Texture_Gauge_Boss_Width, BossHPGauge_Pos_Y, 0);
		GaugePtr->VertexWk[2].vtx = D3DXVECTOR3(BossHPGauge_Pos_X, BossHPGauge_Pos_Y - Texture_Gauge_Boss_Height, 0);
		GaugePtr->VertexWk[3].vtx = D3DXVECTOR3(BossHPGauge_Pos_X + Texture_Gauge_Boss_Width, BossHPGauge_Pos_Y - Texture_Gauge_Boss_Height, 0);
		break;
	default:
		break;
	}

	return;
}

//=============================================================================
// �F�A�����x�̐ݒ�
//=============================================================================
void SetGaugeDiffuse(GAUGE *GaugePtr, int Color)
{
	switch (Color)
	{
	case Green:
		GaugePtr->VertexWk[0].diffuse = GREEN(255);
		GaugePtr->VertexWk[1].diffuse = GREEN(255);
		GaugePtr->VertexWk[2].diffuse = GREEN(255);
		GaugePtr->VertexWk[3].diffuse = GREEN(255);
		break;
	case Yellow:
		GaugePtr->VertexWk[0].diffuse = YELLOW(255);
		GaugePtr->VertexWk[1].diffuse = YELLOW(255);
		GaugePtr->VertexWk[2].diffuse = YELLOW(255);
		GaugePtr->VertexWk[3].diffuse = YELLOW(255);
		break;
	case Red:
		GaugePtr->VertexWk[0].diffuse = RED(255);
		GaugePtr->VertexWk[1].diffuse = RED(255);
		GaugePtr->VertexWk[2].diffuse = RED(255);
		GaugePtr->VertexWk[3].diffuse = RED(255);
		break;
	case Red_AlphaHalf:
		GaugePtr->VertexWk[0].diffuse = RED(128);
		GaugePtr->VertexWk[1].diffuse = RED(128);
		GaugePtr->VertexWk[2].diffuse = RED(128);
		GaugePtr->VertexWk[3].diffuse = RED(128);
		break;
	case SkyBlue:
		GaugePtr->VertexWk[0].diffuse = SKYBLUE(255);
		GaugePtr->VertexWk[1].diffuse = SKYBLUE(255);
		GaugePtr->VertexWk[2].diffuse = SKYBLUE(255);
		GaugePtr->VertexWk[3].diffuse = SKYBLUE(255);
		break;
	case PowerGaugeGradation:
		GaugePtr->VertexWk[0].diffuse = SKYBLUE(255);
		GaugePtr->VertexWk[1].diffuse = BLUE(255);
		GaugePtr->VertexWk[2].diffuse = SKYBLUE(255);
		GaugePtr->VertexWk[3].diffuse = BLUE(255);
	default:
		break;
	}

	return;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeSymbolVertex(void)
{
	int i = 0;

	// ���_���W�̐ݒ�	
	SetSymbolTexture();

	// rhw�̐ݒ�
	for (i = 0; i < PlayerBomb_Max; i++)
	{
		PlayerBombSymbol[i].VertexWk[0].rhw = 1.0f;
		PlayerBombSymbol[i].VertexWk[1].rhw = 1.0f;
		PlayerBombSymbol[i].VertexWk[2].rhw = 1.0f;
		PlayerBombSymbol[i].VertexWk[3].rhw = 1.0f;
	}

	// ���ˌ��̐ݒ�
	for (i = 0; i < PlayerBomb_Max; i++)
	{
		PlayerBombSymbol[i].VertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		PlayerBombSymbol[i].VertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		PlayerBombSymbol[i].VertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		PlayerBombSymbol[i].VertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// �e�N�X�`�����W�̐ݒ�
	SetSymbolVertex();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetSymbolTexture(void)
{
	PLAYER *Player = GetPlayer(0);

	for (int i = 0; i < PlayerBomb_Max; i++)
	{
		if (i < Player->BombNum)
		{
			// �{������
			PlayerBombSymbol[i].VertexWk[0].tex = D3DXVECTOR2(0.5f, 0.0f);
			PlayerBombSymbol[i].VertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			PlayerBombSymbol[i].VertexWk[2].tex = D3DXVECTOR2(0.5f, 1.0f);
			PlayerBombSymbol[i].VertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else
		{
			// �{���Ȃ�
			PlayerBombSymbol[i].VertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			PlayerBombSymbol[i].VertexWk[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			PlayerBombSymbol[i].VertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			PlayerBombSymbol[i].VertexWk[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		}
	}

	return;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetSymbolVertex(void)
{
	for (int i = 0; i < PlayerBomb_Max; i++)
	{
		PlayerBombSymbol[i].VertexWk[0].vtx =
			D3DXVECTOR3((float)PlayerBombSymbol_Pos_X + i * Texture_BombSymbol_Width, PlayerBombSymbol_Pos_Y, 0);
		PlayerBombSymbol[i].VertexWk[1].vtx =
			D3DXVECTOR3((float)PlayerBombSymbol_Pos_X + (i + 1) * Texture_BombSymbol_Width, PlayerBombSymbol_Pos_Y, 0);
		PlayerBombSymbol[i].VertexWk[2].vtx =
			D3DXVECTOR3((float)PlayerBombSymbol_Pos_X + i * Texture_BombSymbol_Width, PlayerBombSymbol_Pos_Y + Texture_BombSymbol_Height, 0);
		PlayerBombSymbol[i].VertexWk[3].vtx =
			D3DXVECTOR3((float)PlayerBombSymbol_Pos_X + (i + 1) * Texture_BombSymbol_Width, PlayerBombSymbol_Pos_Y + Texture_BombSymbol_Height, 0);
	}

	return;
}
