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
	enum PlaySceneLocalZ{
		back_ground_localz, board_localz, stone_layer_localz, 
		time_layer_localz, ui_layer_localz, fadelayer_localz
	};

	void createBackGround();
	float createBoard();
	void runActionGameUpdate();
	void unscheduleGameUpdate();
	void runActionTimeOver();
	void runActionCountDown();
	void runActionRestart();
	void runActionComputer();
	void gameUpdate(const float dt);
	Stone oppositionColor(const Stone &s) const;
	void saveData() const;
	void endGame(const GameState &s);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
	CStoneLayer* stone_layer;
	CTimerLayer* timer_layer;
	CUILayer* ui_layer;

	CDataManager* data_manager;
	Stone player_stone_color;		// 플레이어 색
	Stone computer_stone_color;		// 컴퓨터 색
	Stone now_turn;					// 현재 차례 색
	bool pause_check;
	bool game_start_check;
};
#endif