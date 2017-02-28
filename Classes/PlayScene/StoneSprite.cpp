#include "StoneSprite.h"

CStoneSprite::CStoneSprite() : stone(Stone::emptied), score(0), active(false) {}
CStoneSprite::~CStoneSprite() {}

bool CStoneSprite::init() {
	if (!Sprite::init()) return false;
	return true;
}

bool CStoneSprite::initSprite(const float scale, const Stone s) {
	if (stone == Stone::emptied) {
		// ������ �� �ٵϵ��� �̹����� �ҷ���
		switch (s) {
		case Stone::black:
			if (!initWithFile("etc/Black.png"))	return false;
			break;
		case Stone::white:
			if (!initWithFile("etc/White.png"))	return false;
			break;
		default:
			return false;
		}
		stone = s;
		active = false;
		setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
		setScale(0.5 * scale);
		return true;
	}
	return false;
}

int CStoneSprite::getScore() const {
	return score;
}
void CStoneSprite::setScore(const int i) {
	score = i;
}

bool CStoneSprite::getActive() const {
	return active;
}
void CStoneSprite::setActive(const bool b) {
	active = b;
}

Stone CStoneSprite::getStoneType() const {
	return stone;
}

//int CStoneSprite::getXpos() const {
//	return ix;
//}
//
//int CStoneSprite::getYpos() const {
//	return iy;
//}

//CStoneSprite* CStoneSprite::create(const int x, const int y) {
//	CStoneSprite* tmp = new CStoneSprite();
//	if (tmp && tmp->init(x, y))
//	{
//		tmp->autorelease();
//		return tmp;
//	}
//	else
//	{
//		if (tmp != nullptr) delete tmp;
//		tmp = nullptr;
//		return tmp;
//	}
//	return tmp;
//}
//
//bool CStoneSprite::init(const int x, const int y) {
//	if (!Sprite::init()) return false;
//	// ���� �ٵϵ� ��ġ ������ ��ǥ�� �Է�
//	ix = x;
//	iy = y;
//	return true;
//}