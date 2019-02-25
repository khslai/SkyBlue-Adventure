//=============================================================================
//
// チュートリアルヘッダー [Tutorial.h]
// Author：HAL東京　ゲーム学科1年生　頼凱興 
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Texture_Tutorial1_KeyBoard	_T("data/Texture/BackGround/Tutorial1_KeyBoard.png")
#define Texture_Tutorial1_GamePad	_T("data/Texture/BackGround/Tutorial1_GamePad.png")
#define Texture_Tutorial2			_T("data/Texture/BackGround/Tutorial2.png")
#define Texture_Tutorial_Width		(1024)
#define Texture_Tutorial_Height		(768)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// 初期化処理
HRESULT InitTutorial(int InitState);
// 終了処理
void UninitTutorial(void);
// 更新処理
void UpdateTutorial(void);
// 描画処理
void DrawTutorial(void);
// メニュー画面描画処理
void DrawTutorialMenu(void);
// チュートリアルの状態を取得する
int GetTutorialPhase(void);

#endif
