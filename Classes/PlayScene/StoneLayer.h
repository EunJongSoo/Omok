#ifndef _STONE_LAYER_H_
#define _STONE_LAYER_H_

#include "cocos2d.h"
#include "StoneSprite.h"
#include "enumHeader.h"

class CComputer;

class CStoneLayer : public cocos2d::Layer {
public:
	CStoneLayer();
	virtual ~CStoneLayer();
	virtual bool init();
	CREATE_FUNC(CStoneLayer);

	void initStoneLayer();
	void createStone(const Stone s);
	void createStone(const cocos2d::Vec2 &pos, const Stone s);
	void calculationPosition(const cocos2d::Vec2 &pos, const Stone s);
	void calculationBoardSize(const cocos2d::Size &basesize, const float scale);
	CStoneSprite* getSprite(const int x, const int y);
	CStoneSprite* searchAroundSprite(const float x, const float y, const int dir, const int depth);

public:
	enum Direction {
		up, down, left, right,
		upper_left, upper_right, lower_left, lower_right, dir_count
	};
	enum Min_MaxXY {
		max_x, min_x, max_y, min_y
	};

	std::vector<cocos2d::Label*> labelvec;
	int min_max_xy_position[4];							// Min_MaxXY ������ ����

private:
	GameState checkWinColor();
	void checkBoard(const Stone s);
	void checkStone(const int x, const int y);
	void checkWidth();
	void checkHeight();
	void checkSlash();
	void checkBackSlash();
	void activeAroundStone(const cocos2d::Vec2 &pos);

private:
	const int board_size_x;
	const int board_size_y;
	float margin_pixel;			// �ٵ��� ���� �ȼ�
	float interval_pixel;			// �ٵ��� �� ���� �ȼ�

	CComputer* computer;				// ��ǻ�� �������
	float scale;						// ��ü �̹��� �����ϰ�
	int check_count;					// ********���� üũ, ����� ������, �Լ��� ���ڷ� �ִ°� ������?
	Stone check_stone_color;			// ���� ���� ���� ********** ��**********
	bool check_win;						// �¸� üũ
	cocos2d::Vec2 str_position;			// �ٵ��� ������
	cocos2d::Vec2 end_position;			// �ĵ��� ����
	
	int count;							// ************* ����׿�
};
#endif 