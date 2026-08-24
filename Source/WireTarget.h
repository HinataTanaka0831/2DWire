#pragma once
#include "Object2D.h"

// プレイヤーがワイヤーを射出して引っ掛けることができる支点（建物・支柱等）オブジェクト
class WireTarget : public Object2D
{
public:
	// ワイヤーターゲットの生成と配置
	// 入力: filename(テクスチャパス), initPos(ワールド座標) / 出力: なし / 副作用: WireTarget2Dタグの設定
	WireTarget(std::string filename, VECTOR initPos);
	virtual ~WireTarget();

	void Update() override;
	void Draw() override;
};