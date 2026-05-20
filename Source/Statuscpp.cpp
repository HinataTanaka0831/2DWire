#include "Status.h"

Status::Status(int hp, int attack)
	:mnHp(hp)
	,mnMaxHp(hp)
	,mnAttack(attack)
{

}

Status::~Status()
{

}


void Status::Damage(int damage)
{
	mnHp -= damage;
	if (mnHp<= 0)
	{
		mnHp = 0;
	}
}

