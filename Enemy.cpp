#include "Enemy.h"
#include "time.h"
#include "Stage.h"
#include "Player.h"

namespace
{
	const int ENEMY_SIZE = 48; //敵のサイズ 32*32
	const Point ENEMY_START_POS = { 20 * ENEMY_SIZE, 10 * ENEMY_SIZE }; //敵の初期位置
	const DIR INIT_ENEMY_DIR = { LEFT };
	const int ENEMY_DRAW_SIZE = 32; //敵の描画サイズ
	const int animFrame[4]{ 0, 1, 2, 1 };
	const float ANIM_INTERVAL = 0.2f;
}


Enemy::Enemy()
	: GameObject()
{
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	////GetRand(数値)
	////3秒に1回向きをランダムに変える
	////static float dir_timer = 3.0f;
	//static float prog_timer = 0.5f;
	//float dt = Time::DeltaTime();
	////dir_timer = dir_timer - dt;
	//prog_timer = prog_timer - dt;
	//*if (dir_timer < 0.0f)
	//{
	//	dir_ = (DIR)(GetRand(3));
	//	dir_timer = 3.0f + dir_timer;
	//}*/
	//Point newPos = pos_;
	//if (prog_timer < 0.0f)
	//{
	//	switch (dir_)
	//	{
	//	case UP:
	//		newPos.y -= ENEMY_DRAW_SIZE;
	//		break;
	//	case DOWN:
	//		newPos.y += ENEMY_DRAW_SIZE;
	//		break;
	//	case LEFT:
	//		newPos.x -= ENEMY_DRAW_SIZE;
	//		break;
	//	case RIGHT:
	//		newPos.x += ENEMY_DRAW_SIZE;
	//		break;
	//	default:
	//		break;
	//	}
	//	int mapX = newPos.x / CHA_SIZE;
	//	int mapY = newPos.y / CHA_SIZE;
	//	bool canMove = false;
	//	//int mapValue = FindGameObject<Stage>()->GetMap(newPos.x / CHA_SIZE, newPos.y / CHA_SIZE);
	//	//Stage* stage = FindGameObject<Stage>();
	//	//int mapValue = stage->GetMap(newPos.x / CHA_SIZE, newPos.y / CHA_SIZE);
	//	//移動先がステージの外に出ないようにする
	//	if (mapX >= 0 && mapX < STAGE_WIDTH && mapY >= 0 && mapY < STAGE_HEIGHT)
	//	{
	//		if (FindGameObject<Stage>()->GetMap(mapX, mapY) == 0)
	//		{
	//			canMove = true;
	//		}
	//	}
	//	if (canMove)
	//	{
	//		pos_ = newPos;
	//	}
	//	else {
	//		switch (dir_)
	//		{
	//		case UP:    dir_ = RIGHT; break; // 上に行けなくなったら右を向く
	//		case RIGHT: dir_ = DOWN;  break; // 右に行けなくなったら下を向く
	//		case DOWN:  dir_ = LEFT;  break; // 下に行けなくなったら左を向く
	//		case LEFT:  dir_ = UP;    break; // 左に行けなくなったら上を向く
	//		}
	//	}
	//	prog_timer = 0.5f + prog_timer;
	//}

	static float prog_timer = 0.5f;
	float dt = Time::DeltaTime();
	prog_timer = prog_timer - dt;

	if (prog_timer < 0.0f)
	{
		Stage* stage = FindGameObject<Stage>();
		Player* player = FindGameObject<Player>();
		if (!player) return;

		Point playerPos = player->GetPos();

		if (CheckVision(playerPos))
		{
			isChasing_ = true;
			isSearching_ = false;
		}

		if (isChasing_ && !CheckVision(playerPos))
		{
			isChasing_ = false;
			isSearching_ = true;
			searchTimer_ = 3.0f;
		}

		// 2. モードに合わせて移動（関数を呼び分けるだけ！）
		if (isChasing_)
		{
			MoveChasing(playerPos, stage);
		}
		else if (isSearching_)
		{
			searchTimer_ -= prog_timer;

			if (searchTimer_ <= 0.0f)
			{
				isSearching_ = false;
			}
		}
		else
		{
			MovePatrolling(stage);
		}
		/*else
		{
			MovePatrolling(stage);
		}*/

		prog_timer = 0.5f + prog_timer;
	}

}


		//	//方向の定義を整理
		//	DIR leftDir = UP;
		//	switch (dir_)
		//	{
		//	case UP:    leftDir = LEFT;  break;
		//	case DOWN:  leftDir = RIGHT; break;
		//	case LEFT:  leftDir = DOWN;  break;
		//	case RIGHT: leftDir = UP;    break;
		//	}
		//	// 今の場所から見た「前」と「左」
		//	Point frontPos = pos_;
		//	Point leftPos = pos_;
		//	switch (dir_)
		//	{
		//	case UP:    frontPos.y -= ENEMY_DRAW_SIZE; break;
		//	case DOWN:  frontPos.y += ENEMY_DRAW_SIZE; break;
		//	case LEFT:  frontPos.x -= ENEMY_DRAW_SIZE; break;
		//	case RIGHT: frontPos.x += ENEMY_DRAW_SIZE; break;
		//	}
		//	switch (leftDir)
		//	{
		//	case UP:    leftPos.y -= ENEMY_DRAW_SIZE; break;
		//	case DOWN:  leftPos.y += ENEMY_DRAW_SIZE; break;
		//	case LEFT:  leftPos.x -= ENEMY_DRAW_SIZE; break;
		//	case RIGHT: leftPos.x += ENEMY_DRAW_SIZE; break;
		//	}
		//	// 壁かどうかを判定する
		//	auto isWall = [&](Point p) {
		//		int mapX = p.x / CHA_SIZE;
		//		int mapY = p.y / CHA_SIZE;
		//		if (mapX < 0 || mapX >= STAGE_WIDTH || mapY < 0 || mapY >= STAGE_HEIGHT) return true;
		//		return stage->GetMap(mapX, mapY) != 0;
		//		};
		//	bool frontIsWall = isWall(frontPos);
		//	bool leftIsWall = isWall(leftPos);
		//	//「今歩いてきた後ろの左側」に壁があったかをチェック
		//	Point backLeftPos = pos_;
		//	switch (dir_)
		//	{
		//	case UP:    backLeftPos.x -= ENEMY_DRAW_SIZE; break; // 上に進んでいる時の左
		//	case DOWN:  backLeftPos.x += ENEMY_DRAW_SIZE; break; // 下に進んでいる時の左
		//	case LEFT:  backLeftPos.y += ENEMY_DRAW_SIZE; break; // 左に進んでいる時の左
		//	case RIGHT: backLeftPos.y -= ENEMY_DRAW_SIZE; break; // 右に進んでいる時の左
		//	}
		//	bool wallExistedOnLeft = isWall(backLeftPos);
		//	//アルゴリズムの適用
		//	if (wallExistedOnLeft && !leftIsWall)
		//	{
		//		//さっきまで左に壁があったのに今なくなったという角の時だけ左に曲がる
		//		dir_ = leftDir;
		//		pos_ = leftPos;
		//	}
		//	else if (!frontIsWall)
		//	{
		//		//前が空いているなら、基本はまっすぐ進む
		//		pos_ = frontPos;
		//	}
		//	else
		//	{
		//		//前が壁なら、その場で時計回りに90度右を向く
		//		switch (dir_)
		//		{
		//		case UP:    dir_ = RIGHT; break;
		//		case RIGHT: dir_ = DOWN;  break;
		//		case DOWN:  leftDir = LEFT;  dir_ = LEFT;  break; // 安全のため明確に
		//		case LEFT:  dir_ = UP;    break;
		//		}
		//	}
		//	prog_timer = 0.5f + prog_timer;
		//}

	//①プレイヤーも壁から外に出ないようにする
	//②パンダを壁沿いにぐるぐる回るようにする
	//（元の移動処理はコメントにしておく）

	void Enemy::Draw()
	{
		static float animTimer = ANIM_INTERVAL;
		static int frame = 0;
		int nowFrame = animFrame[frame];

		Rect iRect[4] = {
			{  nowFrame * ENEMY_SIZE, 3 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
			{  nowFrame * ENEMY_SIZE, 0 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
			{  nowFrame * ENEMY_SIZE, 1 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
			{  nowFrame * ENEMY_SIZE, 2 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE}
		};
		DrawBox(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
			GetColor(255, 255, 0), FALSE, 2);
		DrawRectExtendGraph(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
			iRect[dir_].x, iRect[dir_].y, iRect[dir_].w, iRect[dir_].h, hImage_, TRUE);
		if (animTimer < 0) {
			frame = (++frame) % 4;
			animTimer = ANIM_INTERVAL + animTimer;
		}
		animTimer = animTimer - Time::DeltaTime();

		//パンダの視界やで
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		unsigned int visionColor = GetColor(0, 162, 232);
		int s = ENEMY_DRAW_SIZE; // 32マス

		// パンダの正面（UP）を基準とした、視界の相対座標（前方マス、左右マス）
		// { 前方に何マス目か, 左右に何マス目か }
		struct Offset { int forward; int side; };
		const Offset visionOffsets[] = {
			{1,  0},                                // 1マス前（中央）
			{2, -1}, {2,  0}, {2,  1},              // 2マス前（左・中・右）
			{3, -2}, {3, -1}, {3,  0}, {3,  1}, {3,  2} // 3マス前（5マス幅）
		};

		// 配列をループで回して一気に描画
		for (const auto& offset : visionOffsets)
		{
			int targetX = pos_.x;
			int targetY = pos_.y;

			// パンダの向きに合わせて、前（forward）と横（side）の方向を変換する
			switch (dir_)
			{
			case UP:
				targetX += offset.side * s;
				targetY -= offset.forward * s;
				break;
			case DOWN:
				targetX -= offset.side * s; // 左右反転
				targetY += offset.forward * s;
				break;
			case LEFT:
				targetX -= offset.forward * s;
				targetY += offset.side * s;
				break;
			case RIGHT:
				targetX += offset.forward * s;
				targetY -= offset.side * s;
				break;
			}

			// 計算した座標に1マス分の四角を描画
			DrawBox(targetX, targetY, targetX + s, targetY + s, visionColor, TRUE);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// --- アニメーションタイマー等の処理 ---
		if (animTimer < 0) {}

	}

	bool Enemy::CheckVision(Point playerPos)
	{
		struct Offset { int forward; int side; };
		const Offset visionOffsets[] = {
			{1,  0},
			{2, -1}, {2,  0}, {2,  1},
			{3, -2}, {3, -1}, {3,  0}, {3,  1}, {3,  2}
		};

		for (const auto& offset : visionOffsets)
		{
			int vX = pos_.x; int vY = pos_.y;
			switch (dir_)
			{
			case UP:    vX += offset.side * s; vY -= offset.forward * s; break;
			case DOWN:  vX -= offset.side * s; vY += offset.forward * s; break;
			case LEFT:  vX -= offset.forward * s; vY += offset.side * s; break;
			case RIGHT: vX += offset.forward * s; vY -= offset.side * s; break;
			}
			if (vX == playerPos.x && vY == playerPos.y) return true; // 発見！
		}
		return false;//見つからなかった
	}

	void Enemy::MoveChasing(Point playerPos, Stage * stage)
	{
		int diffX = playerPos.x - pos_.x;
		int diffY = playerPos.y - pos_.y;

		auto isWall = [&](Point p) {
			int mapX = p.x / CHA_SIZE; int mapY = p.y / CHA_SIZE;
			if (mapX < 0 || mapX >= STAGE_WIDTH || mapY < 0 || mapY >= STAGE_HEIGHT) return true;
			return stage->GetMap(mapX, mapY) != 0;
			};

		Point nextPos = pos_;
		DIR nextDir = dir_;

		if (abs(diffX) > abs(diffY)) {
			if (diffX > 0) { nextDir = RIGHT; nextPos.x += s; }
			else { nextDir = LEFT;  nextPos.x -= s; }
			if (isWall(nextPos)) {
				nextPos = pos_;
				if (diffY > 0) { nextDir = DOWN; nextPos.y += s; }
				else { nextDir = UP;   nextPos.y -= s; }
			}
		}
		else {
			if (diffY > 0) { nextDir = DOWN; nextPos.y += s; }
			else { nextDir = UP;   nextPos.y -= s; }
			if (isWall(nextPos)) {
				nextPos = pos_;
				if (diffX > 0) { nextDir = RIGHT; nextPos.x += s; }
				else { nextDir = LEFT;  nextPos.x -= s; }
			}
		}
		dir_ = nextDir;
		if (!isWall(nextPos)) pos_ = nextPos;
	}

	void Enemy::MovePatrolling(Stage * stage)
	{
		auto isWall = [&](Point p) {
			int mapX = p.x / CHA_SIZE; int mapY = p.y / CHA_SIZE;
			if (mapX < 0 || mapX >= STAGE_WIDTH || mapY < 0 || mapY >= STAGE_HEIGHT) return true;
			return stage->GetMap(mapX, mapY) != 0;
			};

		DIR leftDir = UP;
		switch (dir_) {
		case UP: leftDir = LEFT; break; case DOWN: leftDir = RIGHT; break;
		case LEFT: leftDir = DOWN; break; case RIGHT: leftDir = UP; break;
		}

		Point frontPos = pos_; Point leftPos = pos_;
		switch (dir_) {
		case UP: frontPos.y -= s; break; case DOWN: frontPos.y += s; break;
		case LEFT: frontPos.x -= s; break; case RIGHT: frontPos.x += s; break;
		}
		switch (leftDir) {
		case UP: leftPos.y -= s; break; case DOWN: leftPos.y += s; break;
		case LEFT: leftPos.x -= s; break; case RIGHT: leftPos.x += s; break;
		}

		bool frontIsWall = isWall(frontPos);
		bool leftIsWall = isWall(leftPos);

		Point backLeftPos = pos_;
		switch (dir_) {
		case UP: backLeftPos.x -= s; break; case DOWN: backLeftPos.x += s; break;
		case LEFT: backLeftPos.y += s; break; case RIGHT: backLeftPos.y -= s; break;
		}
		bool wallExistedOnLeft = isWall(backLeftPos);

		if (wallExistedOnLeft && !leftIsWall) { dir_ = leftDir; pos_ = leftPos; }
		else if (!frontIsWall) { pos_ = frontPos; }
		else {
			switch (dir_) {
			case UP: dir_ = RIGHT; break; case RIGHT: dir_ = DOWN; break;
			case DOWN: dir_ = LEFT; break; case LEFT: dir_ = UP; break;
			}
		}
	}