#pragma once

#include <string>
#include <vector>

// BGMおよびSEの読み込み・再生・停止・重複防止を一括管理するクラス
class SoundManager
{
public:
	// 再生対象のBGM識別子
	enum SoundBgm
	{
		BgmTitle = 0,
		BgmGame,
		BgmResult,
	};

	// 再生対象のSE識別子
	enum SoundSe
	{
		SeDecide = 0,
	};

public:
	SoundManager();
	~SoundManager();

	// 使用するBGM/SEリソースの事前読み込み
	// 入力: なし / 出力: なし / 副作用: 各種音声ファイルのメモリロード
	void Initialize();

	// ロード済み全サウンドハンドルの破棄
	// 入力: なし / 出力: なし / 副作用: DxLibサウンドハンドルの全解放
	void Finalize();

public:
	// 指定BGMのループ再生（既に同一BGMが再生中の場合は無駄な再頭出しを防止）
	// 入力: bgm(対象BGM), isTop(先頭から再生するか) / 出力: なし / 副作用: BGM再生、再生中BGM番号の更新
	void PlayBGM(SoundBgm bgm, bool isTop = true);

	// 指定SEの単発再生
	// 入力: se(対象SE) / 出力: なし / 副作用: SE再生、再生中SE番号の更新
	void PlaySE(SoundSe se);
	// BGM音声ファイルのロードとリスト登録（重複ロード防止）
	// 入力: bgm(登録識別子), fileName(ファイルパス) / 出力: なし / 副作用: リストへのハンドル追加
	void LoadBGM(SoundBgm bgm, std::string fileName);

	// SE音声ファイルのロードとリスト登録（重複ロード防止）
	// 入力: se(登録識別子), fileName(ファイルパス) / 出力: なし / 副作用: リストへのハンドル追加
	void LoadSE(SoundSe se, std::string fileName);

	// 現在再生中のBGMの停止
	// 入力: なし / 出力: なし / 副作用: BGM再生停止
	void StopBGM();

private:
	SoundBgm m_nowPlayingBgm;       // 現在再生中のBGM識別子（-1は未再生）
	SoundSe m_nowPlayingSe;         // 直近に再生されたSE識別子

	std::vector<std::pair<SoundBgm, int>> m_bgmHandleList;  // 登録済みBGMハンドル一覧
	std::vector<std::pair<SoundSe, int>> m_seHandleList;    // 登録済みSEハンドル一覧
};