#pragma once

#include <string>
#include <vector>

class SoundManager
{
public:          // enum, struct などの定義
	// BGM の種類
	enum SOUND_BGM
	{
		BGM_TITLE = 0,  // タイトル画面
		BGM_GAME,      // ゲーム画面
		BGM_RESULT,   // リザルト画面
	};

	// SE の種類
	enum SOUND_SE
	{
		SE_DECIDE = 0,  // 決定音
	};

public:
	// コンストラクタ
	SoundManager();
	// デストラクタ
	~SoundManager();

	// 初期化
	void Initialize();
	// 終了処理
	void Finalize();

public:        // サウンド再生系

	// BGM 再生
	// bgm ... 鳴らしたいBGMの種類
	// isTop ... 最初から再生するかどうか
	// bool isTop = true -> デフォルト引数と呼ばれるかきかた。関数を呼び出す際に渡さなくても認識される
	void PlayBGM(SOUND_BGM bgm, bool isTop = true);

	// SE再生
	// se ... 鳴らしたいSEの種類
	void PlaySE(SOUND_SE se);

	// BGMの読み込み
	void LoadBGM(SOUND_BGM bgm, std::string filename);

	// SEの読み込み
	void LoadSE(SOUND_SE se, std::string filename);

	// BGMの停止
	void StopBGM();

private:    // メンバ変数
	SOUND_BGM mnNowPlayingBgm;       // 現在再生されているBGMの種類
	SOUND_SE mnNowPlayingSe;         // 現在再生されているSEの種類

	std::vector <std::pair<SOUND_BGM, int>> mnBgmHandleList;  // 読み込んだBGMハンドルのリスト
	std::vector <std::pair<SOUND_SE, int>> mnSeHandleList;   // 読み込んだSEハンドルのリスト
};