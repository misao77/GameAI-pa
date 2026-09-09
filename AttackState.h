#pragma once
#include "EnemyState.h"
class AttackState :
    public EnemyState
{
    void Update(Enemy* enemy) override;
    const char* GetStateName() override
    {
        return "ATTACK!!";
    }
};

