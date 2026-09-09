#pragma once

class Enemy;

class EnemyState
{
public:
    virtual ~EnemyState() {}

    virtual void Update(Enemy* enemy) = 0;
    virtual const char* GetStateName() = 0;
};

