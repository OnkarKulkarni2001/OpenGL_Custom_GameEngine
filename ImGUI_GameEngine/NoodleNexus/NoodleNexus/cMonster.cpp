#include "cMonster.h"

void cMonster::Attack(cMonster* EnemyMonster)
{
	this->pSword->Swish(EnemyMonster);
}