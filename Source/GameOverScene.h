#pragma once
#include "Scene.h"


class GameOverScene : public Scene
{


public:
	GameOverScene();
	virtual ~GameOverScene();

	// ‰Šú‰»
	virtual void Initialize() override;
	// XV
	virtual void Update() override;
	// •`‰æ
	virtual void Draw() override;
	// I—¹ˆ—
	virtual void Finalize() override;

};