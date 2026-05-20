#include "DxLib.h"
#include "Texture.h"
#include "TextureAnimation.h"
#include "Player.h"
#include "Collision.h"
#include "SceneManager.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"

/* @note リファレンス https://dxlib.xsrv.jp/dxfunc.html
*/

// Master クラスの静的メンバ変数定義
SceneManager* Master::mpSceneManager = new SceneManager();
SoundManager* Master::mpSoundManager = new SoundManager();



/**
* @fn WinMain
* @brief Main関数
* @param[in] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
* @return int 0 正常終了／-1 エラー
* @details Main関数
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(true);

	// 画面サイズの設定
	SetGraphMode(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, 32);

	// DXライブラリ初期化
	if(DxLib_Init() == -1)
	{
		return -1;
	}

	// 初期化
	//Initialize();
	
	// サウンドマネージャーの初期化
	Master::mpSoundManager->Initialize();    // 全てのサウンドが読み込まれる
	// シーンマネージャーの初期化
	Master::mpSceneManager->Initialize();

	// 描画先画面を裏画面に設定する
	SetDrawScreen(DX_SCREEN_BACK);




	//// Textureクラスを使って描画
	//VECTOR position;
	//position.x = 640 / 2;
	//position.y = 480 / 2;
	//Texture *pTexture = new Texture("Resource/Player.bmp", position, false);


	/*// テクスチャアニメーション
	int handle = LoadGraph("Resource/fire_animation.png");
	if (handle == -1)
	{
		// 読み込めなかった場合の処理
		// 今は一旦スルー
		return 0;
	}
	// サイズ取得
	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);
	// テクスチャの分割読み込み
	int* list = new int[10];    // 分割された画像のハンドルリスト
	int success = LoadDivGraph(
		"Resource/fire_animation.png", // 読み込みたいファイル名
		10,                            // 分割総数
		10,                            // 横に何個分割されるか
		1,                             // 縦に何個分割されるか
		sizeX / 10,                    // 分割された一枚の幅
		sizeY / 1,                     // 分割された一枚の高さ
		list
	);

	// クラス化したテクスチャアニメーションを生成
	TextureAnimation* pAnimation = new TextureAnimation(
		VGet(100.0f, 100.0f, 0.0f),
		"Resource/fire_animation.png",
		10,
		10,
		1,
		5
	);

	// テクスチャアニメーション
	int handle2 = LoadGraph("Resource/freeze_animation.png");
	if (handle2 == -1)
	{
		// 読み込めなかった場合の処理
		// 今は一旦スルー
		return 0;
	}
	// サイズ取得
	
	GetGraphSize(handle2, &sizeX, &sizeY);
	// テクスチャの分割読み込み
	int success2 = LoadDivGraph(
		"Resource/fire_animation.png", // 読み込みたいファイル名
		10,                            // 分割総数
		5,                            // 横に何個分割されるか
		2,                             // 縦に何個分割されるか
		sizeX / 5,                    // 分割された一枚の幅
		sizeY / 2,                     // 分割された一枚の高さ
		list
	);

	// クラス化したテクスチャアニメーションを生成
	TextureAnimation* pAnimation2 = new TextureAnimation(
		VGet(200.0f, 200.0f, 0.0f),
		"Resource/freeze_animation.png",
		10,
		5,
		2,
		5
	);
	*/

	//Texture* pTexture = new Texture("Resource/Player.bmp", position, false)

	// ゲームのメインループ
	// ProcessMessage() == 0 -> ウィンドウの×ボタンを押されていないかどうか
	// CheckHitKey(KEY_INPUT_ESCAPE) == 0 -> エスケープキーが押されていないかどうか
	int animationCounter = 0;
	int textureCurrentNum = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面を初期化する
		ClearDrawScreen();

		int time = GetNowCount();



		 // 更新
		//Update();
		Master::mpSceneManager->Update();


		// 描画
		//Draw();
		Master::mpSceneManager->Draw();
		

		// 裏画面の内容を表画面に映す
		ScreenFlip();

		// 17ミリ秒（秒間約60フレームだった場合の1フレーム当たりの経過時間）
		// 経過するまでここで待つ
		while (GetNowCount() - time < 17)
		{
			// 待つだけなのでここでは何も処理はしない
		}

		// 削除する必要のあるオブジェクトがあれば削除する
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();

		// ループする直前にシーン遷移チェックを入れておく
		Master::mpSceneManager->ChangeSceneIfNeeded();

	}

	

	// 終了処理
	//Finalize();
	Master::mpSceneManager->Finalize();
	delete Master::mpSceneManager;
	Master::mpSoundManager->Finalize();
	delete Master::mpSoundManager;

	// DXライブラリ使用の終了
	DxLib_End();

	// ソフトの終了
	return 0;
}


