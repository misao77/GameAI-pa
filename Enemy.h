#pragma once
#include ".\Library\GameObject.h"
#include "global.h"
#include "Stage.h"


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
	int s = 32; // マスサイズ(32)
	bool CheckVision(Point playerPos); // 視界チェック
	void MoveChasing(Point playerPos, Stage* stage); // 追跡移動
	void MovePatrolling(Stage* stage); // 外周巡回
};