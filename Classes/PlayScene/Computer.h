#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "cocos2d.h"

class CComputer {
private:
	// ���� ���� �����ϴ� ���� 2�� ����ٴ� �ǹ�
	int PreviousColor[4] = { 2, 2, 2, 2 };
	
	// ********** ����׿� ����
	// ********** ����׿� ����
	// ********** ����׿� ����
	// ********** ����׿� ����
	int count1;

public:
	CComputer();
	~CComputer();
	cocos2d::Vec2 ComputerTurn(cocos2d::Node* node);
	void CalculationScore(cocos2d::Node* node);
};
#endif
