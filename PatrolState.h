#pragma once
#include "EnemyState.h"
class PatrolState :
    public EnemyState
{
public:
    void Update(Enemy* enemy) override;
    const char* GetStateName() override
    {
        return "PATROL";
    }
};

