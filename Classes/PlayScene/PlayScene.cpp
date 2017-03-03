#define DEBUG_MODE 1

#include "PlayScene\PlayScene.h"
#include "PlayScene\StoneLayer.h"
#include "PlayScene\UILayer.h"
#include "PlayScene\TimerLayer.h"
#include "DataManager.h"

using namespace cocos2d;

CPlayScene::CPlayScene() : player_stone_color(Stone::emptied), now_turn(Stone::black),
pause_check(true), game_start_check(false) {}
CPlayScene::~CPlayScene() {
	this->saveData();
	CC_SAFE_DELETE(data_manager);
}

bool CPlayScene::init() {
	if (!Scene::init())
		return false;

	// ȭ�� ��ȯ ȿ��
	LayerColor* layer = LayerColor::create(Color4B(0, 0, 0, 0));
	this->addChild(layer, PlaySceneLocalZ::fadelayer_localz);
	FadeOut* fadeout = FadeOut::create(0.5f);
	layer->runAction(fadeout);

	// ���콺 �̺�Ʈ �߰�
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true); // Ŭ�� ��Ȯ�� ���
	listener->onTouchBegan = CC_CALLBACK_2(CPlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CPlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CPlayScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	

	createBackGround();
	
	float scale = createBoard();
	Size size = this->getChildByName("base")->getContentSize();

	stone_layer = CStoneLayer::create();
	stone_layer->calculationBoardSize(size, scale);
	this->addChild(stone_layer, PlaySceneLocalZ::stone_layer_localz);
	
	timer_layer = CTimerLayer::create();
	timer_layer->setRunActionrFuncs(std::bind(&CPlayScene::runActionGameUpdate, this),
									std::bind(&CPlayScene::runActionTimeOver, this));
	timer_layer->setPauseCheck(&pause_check);
	this->addChild(timer_layer, PlaySceneLocalZ::time_layer_localz);

	ui_layer = CUILayer::create();
	ui_layer->setRunActionrCountDownFunc(std::bind(&CPlayScene::runActionCountDown, this));
	ui_layer->setRunActionrRestartFunc(std::bind(&CPlayScene::runActionRestart, this));
	ui_layer->setPauseCheck(&pause_check);
	this->addChild(ui_layer, PlaySceneLocalZ::ui_layer_localz);

	data_manager = new CDataManager();
	Document d = data_manager->loadData();
	if (!d["open"].GetBool()) {
		ui_layer->setSoundOption(true, true);
		return true;
	}
	// ȿ���� ����
	// ��ư ǥ����� ����
	ui_layer->setSoundOption(d["bgm"].GetBool(), d["effect"].GetBool());

	return true;
}

void CPlayScene::createBackGround() {
	Size winsize = CCDirector::getInstance()->getWinSize();
	Sprite* back_ground = Sprite::create("etc/bg_1.jpg");
	back_ground->setPosition(winsize / 2);
	back_ground->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(back_ground, PlaySceneLocalZ::back_ground_localz);
}

float CPlayScene::createBoard() {
	Sprite* base_sprite = Sprite::create("etc/Base.jpg");
	Size winsize = Director::getInstance()->getWinSize();
	Size basesize = base_sprite->getContentSize();

	float widthscale = winsize.width / basesize.width;
	float heightscale = winsize.height / basesize.height;
	float scale = widthscale > heightscale ? heightscale : widthscale;

	base_sprite->setAnchorPoint(Vec2(0.5, 0.5));
	base_sprite->setScale(scale);
	base_sprite->setPosition(winsize / 2);
	this->addChild(base_sprite, PlaySceneLocalZ::board_localz, "base");
	return scale;
}

void CPlayScene::runActionGameUpdate() {
	game_start_check = true;
	ui_layer->runActionDanceAnimation();
	this->schedule(schedule_selector(CPlayScene::gameUpdate), 1.5f);
}

void CPlayScene::unscheduleGameUpdate() {
	this->unschedule(schedule_selector(CPlayScene::gameUpdate));
}

void CPlayScene::runActionTimeOver() {
	this->endGame(GameState::time_over);
}

void CPlayScene::runActionCountDown() {
	pause_check = false;
	computer_stone_color = oppositionColor(player_stone_color = ui_layer->getPlayerColor());
	timer_layer->runActionCountDown();
}

void CPlayScene::runActionRestart() {
	game_start_check = false;
	ui_layer->initUILayer();
	timer_layer->initTimerLayer();
	stone_layer->initStoneLayer();
	now_turn = Stone::black;
}

void CPlayScene::runActionComputer() {
	try {
		// AI �ൿ �Լ� ȣ��
		stone_layer->createStone(computer_stone_color);
		now_turn = oppositionColor(computer_stone_color);
	}
	// �¸�ó��
	catch (GameState e) {
		if (e == GameState::black_win || e == GameState::white_win)
			endGame(e);
#ifndef DEBUG_MODE == 1
		else if (e == GameState::error) {
			CCLOG("************ Computer Fail");
		}
#endif
	}
}

void CPlayScene::gameUpdate(const float dt) {
	if (!pause_check && now_turn == computer_stone_color && game_start_check) {
		ui_layer->changeDanceAnimation();
		runActionComputer();	// ��ǻ�� ��
		timer_layer->resetTimer();
	}
}

Stone CPlayScene::oppositionColor(const Stone &s) const {
	return s == Stone::black ? Stone::white : Stone::black;
}

void CPlayScene::saveData() const {
	data_manager->saveData("/bgm", ui_layer->getBGMState());
	data_manager->saveData("/effect", ui_layer->getEffectState());
	data_manager->outputData();
}

void CPlayScene::endGame(const GameState &s) {
	stone_layer->stopAllActionVector();
	pause_check = true;
	game_start_check = false;
	this->unscheduleGameUpdate();
	timer_layer->stopTimer();
	ui_layer->createGaemResult(s);
}

bool CPlayScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	return true;
}
void CPlayScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {}
void CPlayScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	try {
		// ������ ����������, ���� ����� ���� �������� Ʋ���� ���� ����	
		if (!pause_check && now_turn == player_stone_color && game_start_check) {
			// Ŭ���� ��ǥ�� ������ �������� �ٵϵ� ���� �Լ��� ȣ����
			stone_layer->calculationPosition(touch->getLocation(), player_stone_color);
			// ���� �����ϸ� ������ �ݴ������ ���� �ٲ�
			now_turn = oppositionColor(player_stone_color);
			timer_layer->resetTimer();
			ui_layer->changeDanceAnimation();
		}
	}
	catch (GameState e) {
		// ����, ���� ��, ���� ���� ��, ���� �¸�
		if (e == GameState::black_win || e == GameState::white_win)
			endGame(e);
	}
}