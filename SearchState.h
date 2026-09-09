#pragma once
#include "EnemyState.h"
class SearchState :
    public EnemyState
{
    void Update(Enemy* enemy) override;
    const char* GetStateName() override
    {
        return "SEARCH...";
    }
};

