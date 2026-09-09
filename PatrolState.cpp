#include "PatrolState.h"
#include "Enemy.h"

void PatrolState::Update(Enemy* enemy)
{
    enemy->UpdatePatrol();
}