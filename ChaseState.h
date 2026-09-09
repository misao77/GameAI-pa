#pragma once
#include "EnemyState.h"
class ChaseState :
    public EnemyState
{
public:
    void Update(Enemy* enemy) override;
    const char* GetStateName() override
    {
        return "CHASE";
    }
};

