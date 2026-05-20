#pragma once

class Status
{
public:
	Status(int hp, int attack);
	~Status();

	void Damage(int damage);

public:
	int GetHp() { return mnHp; }
	int GetMaxHp() { return mnMaxHp; }
	int GetAttack() { return mnAttack; }

private:
	int mnHp;
	int mnMaxHp;
	int mnAttack;
};