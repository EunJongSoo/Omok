#ifndef _STONE_SPRITE_H_
#define _STONE_SPRITE_H_

#include "cocos2d.h"
#include "enumHeader.h"

class CStoneSprite : public cocos2d::Sprite {
private:
	int m_Stone;
	int m_iScore;
	bool m_bActive;

public:
	CStoneSprite();
	virtual ~CStoneSprite();

	// ******** get, set �����ϰ� �����ϴ� ���
	// ******** get, set �����ϰ� �����ϴ� ���
	// ******** get, set �����ϰ� �����ϴ� ���
	bool getActive() const;
	void setActive(const bool b);
	int getScore() const;
	void setScore(const int score);
	int getStoneType() const;

	bool CStoneSprite::initSprite(cocos2d::Vec2 pos, const float scale, const int s);
	bool CStoneSprite::initSprite(const float x, const float y, const float scale, const int s);

	virtual bool init();
	CREATE_FUNC(CStoneSprite);
};
#endif

// ******* ������ ���� ����ü�� ���� �ְ� �޴°��� ������?
// ******* ������ ���� ����ü�� ���� �ְ� �޴°��� ������?
// ******* ������ ���� ����ü�� ���� �ְ� �޴°��� ������?
//int ix;
//int iy;
//int getXpos() const;
//int getYpos() const;
//virtual bool init(const int x, const int y);
//static CStoneSprite* create(const int x, const int y);
