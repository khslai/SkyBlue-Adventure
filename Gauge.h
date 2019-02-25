//=============================================================================
//
// �Q�[�W�w�b�_�[ [Gauge.h]
// Author�FHAL�����@�Q�[���w��1�N���@���M�� 
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_


//*****************************************************************************
// �}�N����`
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

// �v���C���[��HP�Q�[�W�̍��W
#define PlayerHPGauge_Pos_X				(100)		
#define PlayerHPGauge_Pos_Y				(10)
// �v���C���[�̃p���[�Q�[�W�̍��W
#define PlayerPowerGauge_Pos_X			(100)		
#define PlayerPowerGauge_Pos_Y			(PlayerHPGauge_Pos_Y + 40)
// �v���C���[�̃p���[�Q�[�W�̍��W
#define PlayerBombSymbol_Pos_X			(ScorePos_X - Texture_Score_Width_Total * 0.9)	
#define PlayerBombSymbol_Pos_Y			(PlayerPowerGauge_Pos_Y)
// �{�X��HP�Q�[�W�̍��W
#define BossHPGauge_Pos_X				(1000)			
#define BossHPGauge_Pos_Y				(700)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �Q�[�W�\����
typedef	struct
{
	VERTEX_2D	VertexWk[Num_Vertex];		// ���_���i�[���[�N
	int			Type;						// �Q�[�W�̎��
}GAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// ����������
HRESULT InitGauge(int InitState);
// �I������
void UninitGauge(void);
// �X�V����
void UpdateGauge(void);
// �`�揈��
void DrawGauge(void);

#endif
