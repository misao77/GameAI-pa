#include "SearchState.h"
#include "Enemy.h"

void SearchState::Update(Enemy* enemy)
{
	enemy->UpdateSearch();
}
