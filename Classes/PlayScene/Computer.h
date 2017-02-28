#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "cocos2d.h"

class CComputer {
public:
	CComputer();
	~CComputer();
	cocos2d::Vec2 ComputerTurn(cocos2d::Node* node);
	void CalculationScore(cocos2d::Node* node);

private:
	int previous_color[4];			// ���� ���� �����ϴ� ���� 2�� ����ٴ� �ǹ�
	int count1;						// ********** ����׿� ����
};
#endif
