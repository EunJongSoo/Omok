#define DEBUG_MODE 0
#define BOARD_SIZE_X 19
#define BOARD_SIZE_Y 19
#define MARGIN_PIXEL 11.0f
#define INTERVAL_PIXEL 21.0f

#include "StoneLayer.h"
#include "Computer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CStoneLayer::CStoneLayer() : board_size_x(BOARD_SIZE_X), board_size_y(BOARD_SIZE_Y), 
								margin_pixel(MARGIN_PIXEL), interval_pixel(INTERVAL_PIXEL), 
									min_max_xy_position{ 15, 4, 15, 4 }, count(0), scale(1.0f){}
CStoneLayer::~CStoneLayer() {
	CC_SAFE_DELETE(computer);
}

bool CStoneLayer::init() {
	if (!Layer::init())
		return false;
	computer = new CComputer();
	return true;
}

void CStoneLayer::calculationBoardSize(const Size &basesize, const float _scale) {
	scale = _scale;
	Size win_size = CCDirector::getInstance()->getWinSize();
	Size size = basesize * scale;
	str_position = Vec2(win_size.width / 2 - size.width / 2,
		win_size.height/2 - size.height / 2);
	end_position = Vec2(win_size.width /2 + size.width / 2,
		win_size.height/2 + size.height / 2);
	
	margin_pixel = margin_pixel * scale;
	interval_pixel = interval_pixel * scale;
	initStoneLayer();
}

void CStoneLayer::stopAllActionVector() {
	int size = my_struct_vector.size();
	for (int i = 0; i < size; ++i) {
		eraseMyStructVector(0);
	}
}

void CStoneLayer::initStoneLayer() {
	check_win = false;
	min_max_xy_position[0] = 10;
	min_max_xy_position[1] = 8;
	min_max_xy_position[2] = 10;
	min_max_xy_position[3] = 8;
	if(this->getChildrenCount() > 0)
		this->removeAllChildren();
#ifndef DEBUG_MODE == 1
	labelvec.clear();
#endif
	float pos_x = 0;
	float pos_y = 0;
	int pos = 0, y = 0, x = 0;
	// �ٵϵ��� ����� �߰���Ų��.
	for (y = 0; y < board_size_y; ++y) {
		for (x = 0; x < board_size_x; ++x) {
			CStoneSprite* sprite = CStoneSprite::create();
			pos_x = x * interval_pixel + margin_pixel + str_position.x;
			pos_y = y * interval_pixel + margin_pixel + str_position.y;
			sprite->setPosition(pos_x, pos_y);
			pos = board_size_y * y + x;
			this->addChild(sprite, 1, pos);
			sprite->setPos(pos);
#ifndef DEBUG_MODE == 1
			Label* label = Label::create();
			label->setPosition(pos_x, pos_y);
			label->setSystemFontSize(25);
			label->setString("0");
			label->setVisible(false);
			this->addChild(label, 10);
			labelvec.push_back(label);
#endif
		}
	}
}

void CStoneLayer::calculationPosition(const Vec2 &pos, const Stone s) {
	// �ٵϵ��� ������ ��ġ�� �ٵ��� �ٱ����̸� ���� ����
	if (pos < str_position || pos > end_position) {
#ifndef DEBUG_MODE == 1
		CCLOG("************* positionCalculation, Fail");
#endif
		throw GameState::error;
	}
	// ��ǥ ���
	int x = round((pos.x - margin_pixel - str_position.x) / (interval_pixel));
	int y = round((pos.y - margin_pixel - str_position.y) / (interval_pixel));
	this->createStone(Vec2(x, y), s);
}

void CStoneLayer::createStone(const Stone s) {
	computer->calculationScore(this);
	createStone(computer->computerTurn(this), s);
}

void CStoneLayer::createStone(const Vec2 &pos, const Stone s) {
	// �ش� ��ǥ�� ���� �ҷ��´�.
	CStoneSprite* pSprite = getSprite(pos.x, pos.y);

	// �ҷ��� ���� ������ Ȯ���ؼ� ���� ������ ���� ����
	if (pSprite->getStoneType() != Stone::emptied) {
		throw GameState::error;
	}
	// �ٵϵ� �̹����� �������ش�.
	pSprite->initSprite(scale, s);
	SimpleAudioEngine::getInstance()->playEffect("sound/effect/ui-sound8.wav");
	// ������ ������ ��������� �ʿ�����Ƿ� ��Ȱ��ȭ �����ش�.
	pSprite->setActive(false);
	// �ִ�, �ּ� ��ǥ�� ����
	if (min_max_xy_position[max_x] < pos.x) min_max_xy_position[max_x] = pos.x;
	if (min_max_xy_position[min_x] > pos.x) min_max_xy_position[min_x] = pos.x;
	if (min_max_xy_position[max_y] < pos.y) min_max_xy_position[max_y] = pos.y;
	if (min_max_xy_position[min_y] > pos.y) min_max_xy_position[min_y] = pos.y;
	// ���� ���θ� �˻��Ѵ�.
	checkBoard(s);
	// 3~4���� ���� �����̰� �Ѵ�.
	BlinkingStoneSprite();
	// �ֺ� 2ĭ�� Ȱ��ȭ ��Ų��.
	activeAroundStone(pos);
#ifndef DEBUG_MODE == 1
	computer->calculationScore(this);
#endif
}

void CStoneLayer::activeAroundStone(const Vec2 &pos) {
	for (int dir = 0; dir < Direction::dir_count; ++dir) {
		for (int depth = 1; depth <= 2; ++depth) {
			CStoneSprite* sprite = searchAroundSprite(pos.x, pos.y, dir, depth);
			if (sprite != nullptr && sprite->getStoneType() == Stone::emptied)
				sprite->setActive(true);
		}
	}
}

CStoneSprite* CStoneLayer::getSprite(const int x, const int y) {
	return dynamic_cast<CStoneSprite*>(this->getChildren().at(board_size_y * y + x));
}

CStoneSprite* CStoneLayer::searchAroundSprite(const float x, const float y, const int dir, const int depth) {
	int ix = 0, iy = 0;

	switch (dir) {
	case Direction::up: ix = 0, iy = 1; break;
	case Direction::down: ix = 0, iy = -1; break;
	case Direction::left: ix = -1, iy = 0; break;
	case Direction::right: ix = 1, iy = 0; break;
	case Direction::upper_left: ix = -1, iy = 1; break;
	case Direction::upper_right: ix = 1, iy = 1; break;
	case Direction::lower_left: ix = -1, iy = -1; break;
	case Direction::lower_right: ix = 1, iy = -1; break;
	default: ix = 0, iy = 0; break;
	}

	int tx = ix * depth + x;
	int ty = iy * depth + y;
	// ��ǥ ���� 0���� �۰ų� �ٵ��� ���� ũ�� �˻����� �ʴ´�.
	if (tx < 0 || tx > board_size_x || ty < 0 || ty > board_size_y) return nullptr;

	return this->getSprite(tx, ty);
}

void CStoneLayer::checkBoard(const Stone s) {
	check_count = 0;
	check_stone_color = s;
	bool check[2] = { 0, 0 };
	if (min_max_xy_position[max_x] - min_max_xy_position[min_x] >= 3) {
		checkWidth();
		check[0] = true;
	}
	if (min_max_xy_position[max_y] - min_max_xy_position[min_y] >= 3) {
		checkHeight();
		check[1] = true;
	}
	if (check[0] && check[1]) {
		checkSlash();
		checkBackSlash();
	}
#ifndef DEBUG_MODE == 1
	CCLOG("count : %d\n", count);
	if (check_stone_color == Stone::black)
		CCLOG("CheckBoard Complete Color : Black\n");
	else if (check_stone_color == Stone::white)
		CCLOG("CheckBoard Complete Color : White\n");
#endif
}

GameState CStoneLayer::checkWinColor() {
	return check_stone_color == Stone::black ? GameState::black_win : GameState::white_win;
}

void CStoneLayer::checkStone(const int x, const int y, const Direction dir) {
	CStoneSprite* sprite = this->getSprite(x, y);
	// 0 : ���� ��, 1 : ������, 2 : ù��° ������
	Stone tmp_color[3];
	tmp_color[0] = sprite->getStoneType();
	if (tmp_color[0] == check_stone_color) {
		/* enum Direction {
		up, down, right, left, upper_left, lower_right, lower_left, upper_right, dir_count	};*/
		++check_count;
		if (check_count > 2 && check_count < 6) {
			tmp_color[1] = searchAroundSprite(x, y, dir, 1)->getStoneType();
			// �̹� ���� ���� ���� �޶���Ѵ�.
			// ����° ���� �׹�° ��, �׹�° �ټ���° ��
			if (tmp_color[0] != tmp_color[1]) {
				tmp_color[2] = searchAroundSprite(x, y, dir + 1, check_count)->getStoneType();
				// ���� �� �ڰ� ������ �ʾƾ� �Ѵ�.
				if (tmp_color[1] == Stone::emptied || tmp_color[2] == Stone::emptied) {
					addVector(x, y, check_count + 2, dir);
				}
			}
		}
		if (check_count == 5)	check_win = true;
		else if (check_count > 5) check_win = false;
	}
	else {
		check_count = 0;
	}
#ifndef DEBUG_MODE == 1
	++count;
#endif
}

void CStoneLayer::addVector(const int x, const int y, const int count, const Direction dir) {
	// ���� �յڸ� �����ؼ� ��� �����Ѵ�.
	MyStruct* my_struct = new MyStruct(count);
	CStoneSprite* sprite;
	my_struct->sprite[0] = searchAroundSprite(x, y, dir, 1);
	for (int i = 0; i < count-1; ++i) {
		sprite = searchAroundSprite(x, y, dir + 1, i);
		my_struct->sprite[i+1] = sprite;
		my_struct->sum_pos += sprite->getPos();
	}
	overlapCheckVector(my_struct);
}

void CStoneLayer::overlapCheckVector(MyStruct* mystr) {
	bool same_check = false;
	for (int i = 0; i < my_struct_vector.size(); ++i) {
		// ���� ������ �ִ��� �˻��Ѵ�.
		if (my_struct_vector.at(i)->sum_pos == mystr->sum_pos) {
			// ���� �ϳ��� ���Ѵ�. ���� ��ġ�� ���� ���� ���ٸ� �ٸ� �����̹Ƿ� �˻縦 �ߴ��Ѵ�.
			for (int j = 0; j < my_struct_vector.at(i)->size; ++j) {
				if (my_struct_vector.at(i)->sprite[j]->getPos() == mystr->sprite[j]->getPos()) {
					same_check = true;
				}
				else {
					same_check = false;
					break;
				}
			}
			// ������ ����� �־ �Լ��� ��������
			if (same_check) return;
		}
	}
	my_struct_vector.push_back(mystr);
}

void CStoneLayer::BlinkingStoneSprite() {
	int forsize = my_struct_vector.size();
	int size = 0;
	int i = 0;
	for (int j = 0; j < forsize; ++j) {
		size = my_struct_vector.at(i)->size;
		// ù��° ���� ������ ���� �ϳ��� ������� ��
		if (my_struct_vector.at(i)->sprite[0]->getStoneType() == Stone::emptied ||
			my_struct_vector.at(i)->sprite[size - 1]->getStoneType() == Stone::emptied) {
			size = my_struct_vector.at(i)->size;
			if (size == 5 || size == 6) {
				if (my_struct_vector.at(i)->sprite[1]->getStoneType()
					== my_struct_vector.at(i)->sprite[0]->getStoneType()
					|| my_struct_vector.at(i)->sprite[1]->getStoneType()
					== my_struct_vector.at(i)->sprite[size-1]->getStoneType()) {
					eraseMyStructVector(i);
				}
				else {
					++i;
				}
			}
			else {
				++i;
			}
		}
		else eraseMyStructVector(i);
	}
	for (int j = 0; j < my_struct_vector.size(); ++j) {
		runActionBlinkingSprite(j);
	}
}

void CStoneLayer::runActionBlinkingSprite(int i) {
	for (int k = 1; k < my_struct_vector.at(i)->size - 1; ++k) {
		my_struct_vector.at(i)->sprite[k]->runActionBlinkingSprite();
	}
}

void CStoneLayer::eraseMyStructVector(int i) {
	int size = my_struct_vector.at(i)->size;
	for (int k = 1; k < size - 1; ++k) {
		my_struct_vector.at(i)->sprite[k]->stopBlinkingSprite();
	}
	my_struct_vector.erase(my_struct_vector.begin() + i);
}

void CStoneLayer::checkWidth() {
	// ���� �˻� �¿��� ��� �˻�, ������ ���� ���� �̵�
	for (int y = min_max_xy_position[min_y]; y <= min_max_xy_position[max_y]; ++y) {
		for (int x = min_max_xy_position[min_x]; x <= min_max_xy_position[max_x]; ++x) {
			// width : left, height : down, slash : lower_left, back_slash : upper_left
			checkStone(x, y, Direction::right);
#ifndef DEBUG_MODE == 1
			CCLOG("********** checkWidth x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
	}
	if (check_win) throw checkWinColor();
}

void CStoneLayer::checkHeight() {
	// ���� �˻� �ؿ��� ���� �˻�, ������ ���� ������ �̵�
	for (int x = min_max_xy_position[min_x]; x <= min_max_xy_position[max_x]; ++x) {
		for (int y = min_max_xy_position[min_y]; y <= min_max_xy_position[max_y]; ++y) {
			// width : left, height : down, slash : lower_left, back_slash : upper_left
			checkStone(x, y, Direction::up);
#ifndef DEBUG_MODE == 1
			CCLOG("********** checkHeight x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
	}
	if (check_win) throw checkWinColor();
}

void CStoneLayer::checkSlash() {
	// �밢�� �˻� ������ ������ ���� �Ʒ� �������� �˻�
	for (int ix = min_max_xy_position[min_x], iy = min_max_xy_position[max_y];
		ix <= min_max_xy_position[max_x] && iy >= min_max_xy_position[min_y];
		ix == min_max_xy_position[max_x] ? --iy : ++ix) {
		for (int y = iy, x = ix; x >= min_max_xy_position[min_x] && y >= min_max_xy_position[min_y]; --y, --x) {
			// width : left, height : down, slash : upper_right, back_slash : upper_left
			checkStone(x, y, Direction::lower_left);
#ifndef DEBUG_MODE == 1
			CCLOG("********** CheckSlash x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
	}
	if (check_win) throw checkWinColor();
}

void CStoneLayer::checkBackSlash() {
	// ���밢�� �˻� ������ �Ʒ����� ���� �� �������� �˻�
	for (int ix = min_max_xy_position[min_x], iy = min_max_xy_position[min_y];
		ix <= min_max_xy_position[max_x] && iy <= min_max_xy_position[max_y];
		ix == min_max_xy_position[max_x] ? ++iy : ++ix) {
		for (int y = iy, x = ix; x >= min_max_xy_position[min_x] && y <= min_max_xy_position[max_y]; ++y, --x) {
			// width : left, height : down, slash : lower_left, back_slash : upper_left
			checkStone(x, y, Direction::upper_left);
#ifndef DEBUG_MODE == 1
			CCLOG("********** checkBackSlash x:%d, y:%d, color:%d, count : %d", x, y, check_stone_color, check_count);
#endif
		}
		check_count = 0;
	}
	if (check_win) throw checkWinColor();
}