#pragma once

#include "Scene.h"
#include "Utility.h"

// ステージまたはモード分岐を選択するシーンクラス
class SelectScene : public Scene
{
public:
	enum {
		select_salt,
		select_sauce,

		select_Num,
	};

public:
	SelectScene();
	virtual ~SelectScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;

	void ChangeAttack();
	bool Getflag() { return c; }

private:
	static const int Salt_Y = 270;
	static const int Sauce_Y = 320;
	int y = 0;
	bool c;
	int NowSelect;
};