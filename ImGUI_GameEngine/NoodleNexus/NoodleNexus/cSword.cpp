#include "cSword.h"

void cSword::Swish(cMonster* pEnemyMonster)
{
	pEnemyMonster->health *= 0.25f;
}
