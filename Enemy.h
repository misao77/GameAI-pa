#pragma once
#include ".\Library\GameObject.h"
#include "global.h"
#include "Stage.h"

enum ENEMY_STATE
{
	STATE_PATROL,   // パトロール中
	STATE_CHASE,    // 追跡中（チェイス）
	STATE_ATTACK,   // 攻撃中
	STATE_SEARCH,   // 捜索中
};

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

private:
	bool isChasing_ = false; // 追跡中かどうか
	bool isSearching_ = false; // 捜索中
	bool isAttacking_ = false;
	float searchTimer_ = 0.0f; // 捜索時間
	int s = 32; // マスサイズ(32)
	bool CheckVision(Point playerPos); // 視界チェック
	bool CheckSerchTimeOver();
	void MoveChasing(Point playerPos, Stage* stage); // 追跡移動
	void MovePatrolling(Stage* stage); // 外周巡回
	const float ATTACK_RANGE = 32.0f;
	ENEMY_STATE state_ = STATE_PATROL; // 現在の状態変数
};