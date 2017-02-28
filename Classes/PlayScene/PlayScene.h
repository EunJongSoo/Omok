#ifndef _PLAY_SCENE_H_
#define _PLAY_SCENE_H_

#include "cocos2d.h"
#include "enumHeader.h"

class CStoneLayer;
class CTimerLayer;
class CUILayer;
class CDataManager;

class CPlayScene : public cocos2d::Scene {
public:
	CPlayScene();
	virtual ~CPlayScene();
	virtual bool init();
	CREATE_FUNC(CPlayScene);

private:
	void runActionGameUpdate();
	void unscheduleGameUpdate();
	void runActionTimeOver();
	void runActionCountDown();
	void runActionPause();
	void runActionRestart();
	void runActionComputer();
	void gameUpdate(const float dt);
	Stone oppositionColor(const Stone &s) const;
	void saveData() const;
	void endGame(GameState s);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
	CStoneLayer* stone_layer;
	CTimerLayer* timer_layer;
	CUILayer* ui_layer;

	CDataManager* data_manager;
	Stone player_stone_color;		// �÷��̾� ��
	Stone computer_stone_color;		// ��ǻ�� ��
	Stone now_turn;					// ���� ���� ��
};
#endif