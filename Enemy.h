#pragma once
#include ".\Library\GameObject.h"
#include "global.h"
#include "Stage.h"

class EnemyState;

class Enemy :
	public GameObject
{
	int hImage_;//画像ID
	Point pos_;//位置
	DIR dir_;//移動方向
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;

	// ステートから呼び出す処理
	void UpdatePatrol();
	void UpdateChase();
	void UpdateAttack();
	void UpdateSearch();

	// ステート変更
	void ChangeState(EnemyState* newState);

	bool CheckVision(Point playerPos);
	bool CheckSerchTimeOver();

	void MoveChasing(Point playerPos, Stage* stage);
	void MovePatrolling(Stage* stage);



private:
	const float ATTACK_RANGE = 32.0f;

	
	float searchTimer_ = 0.0f;
	float prog_timer_ = 0.5f;
	int s = 32;

	EnemyState* state_;
	EnemyState* nextState_;
};