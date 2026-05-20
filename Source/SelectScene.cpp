#include "SelectScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "Bullet.h"
#include "Enemy.h"

SelectScene::SelectScene()
	:Scene()  // 基底クラスのコンストラクタを呼び出す
{
	
}

SelectScene::~SelectScene()
{

}

void SelectScene::Initialize()
{

}

void SelectScene::Update()
{
	// Sキーが押されたら下に下がる
	if (InputManager::CheckDownKey(KEY_INPUT_S))
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		NowSelect = (NowSelect + 1) % select_Num;
	}
	// Wキーが押されたら上に上がる
	if (InputManager::CheckDownKey(KEY_INPUT_W))
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		NowSelect = (NowSelect + (select_Num - 1)) % select_Num;
	}
	// エンターキーが押されたら画面の切り替え処理
	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);


			switch (NowSelect)
			{
			case select_salt:   // 
				Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);
				c = false;
				break;

			

			case select_sauce:  //
				Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);
				c = true;
				break;

			}

		

	}


	// 基底クラスの更新処理を呼び出す
	Scene::Update();
}

void SelectScene::Draw()
{
	// 背景の表示
	DrawGraph(0, 0, grHandle, false);
	// 四角形の表示
	DrawBox(Utility::SCREEN_WIDTH / 2 - 150, 50, Utility::SCREEN_WIDTH / 2 + 150, 200, GetColor(255, 255, 255), false);
	// 文字のフォントサイズ変更
	SetFontSize(20);
	// 文字列の表示
	DrawString(Utility::SCREEN_WIDTH / 2 - 110, Utility::SCREEN_HEIGHT / 2 - 150, "　　　串揚げは\n\n　塩派？　ソース派？", GetColor(255, 255, 255));

	DrawBox(Utility::SCREEN_WIDTH / 2 - 130, 370, Utility::SCREEN_WIDTH / 2 + 120, 250, GetColor(255, 255, 255), false);

	DrawString(Utility::SCREEN_WIDTH / 2 - 60, Salt_Y, "　塩　", GetColor(255, 255, 255));

	DrawString(Utility::SCREEN_WIDTH / 2 - 60, Sauce_Y, "　ソース　", GetColor(255, 255, 255));


		// 現在選択されている選択肢のY座標を変数に設定する
		switch (NowSelect)
		{
		case select_salt:  // 選択肢（塩）が選択されている場合は塩のY座標を設定する
			y = Salt_Y;
			break;

		case select_sauce:  // 選択肢（ソース）が選択されている場合はソースのY座標を設定する
			y = Sauce_Y;
			break;
		}

	

	// 文字列の表示
	DrawString(Utility::SCREEN_WIDTH / 2 - 70, y, "◇", GetColor(255, 255, 255));

	// 基底クラスの描画処理を呼び出す
	Scene::Draw();
}

void SelectScene::Finalize()
{

}


void SelectScene::ChangeAttack()
{

		//敵情報を取得
		auto pBullet = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Bullet2D);

		Bullet* cBullet = dynamic_cast<Bullet*>(pBullet);

		if (cBullet != nullptr)
		{

			cBullet->CalcCollision();


         }

}


