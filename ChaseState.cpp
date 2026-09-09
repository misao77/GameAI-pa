#include "ChaseState.h"
#include "Enemy.h"

void ChaseState::Update(Enemy* enemy)
{
	enemy->UpdateChase();

}
