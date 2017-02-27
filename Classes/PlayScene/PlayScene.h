#ifndef _PLAY_SCENE_H_
#define _PLAY_SCENE_H_

#include "cocos2d.h"
#include "enumHeader.h"

// Ŭ���� ���漱��
// ******* effective �׸� 31�� �̰� �³�
// ******* effective �׸� 31�� �̰� �³�
// ******* effective �׸� 31�� �̰� �³�

class CStoneLayer;
class CTimerLayer;
class CUILayer;

class sound;


class CPlayScene : public cocos2d::Scene {
private:
	CStoneLayer* m_StoneLayer;
	CTimerLayer* m_TimerLayer;
	CUILayer* m_UiLayer;

	int m_PlayerColor;
	int m_ComputerColor;
	int m_Turn;

private:
	void EndGame();
	void EffectSound();
	void gameupdate(const float dt);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	// �ɼ� ���� �� �ҷ�����
	void DataLoad();
	void DataSave();

public:
	CPlayScene();
	virtual ~CPlayScene();
	virtual bool init();
	CREATE_FUNC(CPlayScene);
};
#endif