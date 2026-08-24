#include "SoundManagerh.h"
#include "DxLib.h"

SoundManager::SoundManager()
	: mnNowPlayingBgm((SOUND_BGM)-1)
	, mnNowPlayingSe((SOUND_SE)-1)
{
}

SoundManager::~SoundManager()
{
}

// 使用するBGM/SEリソースの事前読み込み
// 入力: なし / 出力: なし / 副作用: 各種音声ファイルのメモリロード
void SoundManager::Initialize()
{
	LoadBGM(SOUND_BGM::BGM_TITLE, "Resource/BGM/maou_14_shining_star.mp3");
	LoadSE(SOUND_SE::SE_DECIDE, "Resource/SE/maou_se_system49.mp3");
}

// ロード済み全サウンドハンドルの破棄
// 入力: なし / 出力: なし / 副作用: DxLibサウンドハンドルの全解放
void SoundManager::Finalize()
{
	// メモリリーク防止のため保持している全サウンドハンドルを破棄
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		DeleteSoundMem(it->second);
	}

	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		DeleteSoundMem(it->second);
	}
}

// 指定BGMのループ再生（既に同一BGMが再生中の場合は無駄な再頭出しを防止）
// 入力: bgm(対象BGM), isTop(先頭から再生するか) / 出力: なし / 副作用: BGM再生、再生中BGM番号の更新
void SoundManager::PlayBGM(SOUND_BGM bgm, bool isTop)
{
	// 同一曲が既に再生中かつ頭出し不要なら再生を継続
	if (mnNowPlayingBgm == bgm && !isTop)
	{
		return;
	}

	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == bgm)
		{
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
			mnNowPlayingBgm = bgm;
			break;
		}
	}
}

// 指定SEの単発再生
// 入力: se(対象SE) / 出力: なし / 副作用: SE再生、再生中SE番号の更新
void SoundManager::PlaySE(SOUND_SE se)
{
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		if (it->first == se)
		{
			// SEはバックグラウンド非同期再生でゲーム描画をブロックしない
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
			mnNowPlayingSe = se;
			break;
		}
	}
}

// BGM音声ファイルのロードとリスト登録（重複ロード防止）
// 入力: bgm(登録識別子), filename(ファイルパス) / 出力: なし / 副作用: リストへのハンドル追加
void SoundManager::LoadBGM(SOUND_BGM bgm, std::string filename)
{
	// 同一BGM識別子の多重ロードを防止
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == bgm)
		{
			return;
		}
	}

	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;
	}

	mnBgmHandleList.push_back(std::pair<SOUND_BGM, int>(bgm, handle));
}

// SE音声ファイルのロードとリスト登録（重複ロード防止）
// 入力: se(登録識別子), filename(ファイルパス) / 出力: なし / 副作用: リストへのハンドル追加
void SoundManager::LoadSE(SOUND_SE se, std::string filename)
{
	// 同一SE識別子の多重ロードを防止
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); it++)
	{
		if (it->first == se)
		{
			return;
		}
	}

	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;
	}

	mnSeHandleList.push_back(std::pair<SOUND_SE, int>(se, handle));
}

// 現在再生中のBGMの停止
// 入力: なし / 出力: なし / 副作用: BGM再生停止
void SoundManager::StopBGM()
{
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); it++)
	{
		if (it->first == mnNowPlayingBgm)
		{
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);
				break;
			}
		}
	}
}