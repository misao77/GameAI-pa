#include "AttackState.h"
#include "Enemy.h"

void AttackState::Update(Enemy* enemy)
{
	enemy->UpdateAttack();
}
