#ifndef _cMonster_HG_
#define _cMonster_HG_

#include "cSword.h"

class cMonster
{
public:
	cSword* pSword;
	void Attack(cMonster *EnemyMonster);
	float health;
};

#endif 
