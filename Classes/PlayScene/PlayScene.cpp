#define DEBUG_MODE 0

#include "PlayScene\PlayScene.h"
#include "PlayScene\StoneLayer.h"
#include "PlayScene\UILayer.h"
#include "PlayScene\TimerLayer.h"
#include "DataManager.h"

using namespace cocos2d;

CPlayScene::CPlayScene() : player_stone_color(Stone::emptied), now_turn(Stone::black) {}
CPlayScene::~CPlayScene() {
	this->saveData();
	CC_SAFE_DELETE(data_manager);
}

bool CPlayScene::init() {
	if (!Scene::init())
		return false;

	// ȭ�� ��ȯ ȿ��
	LayerColor* layer = LayerColor::create(Color4B(0, 0, 0, 0));
	this->addChild(layer, 10);
	FadeOut* fadeout = FadeOut::create(0.5f);
	layer->runAction(fadeout);

	// ���콺 �̺�Ʈ �߰�
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true); // Ŭ�� ��Ȯ�� ���
	listener->onTouchBegan = CC_CALLBACK_2(CPlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CPlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CPlayScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// ���̾� �߰�
	stone_layer = CStoneLayer::create();
	this->addChild(stone_layer, 1);
	timer_layer = CTimerLayer::create();
	timer_layer->setRunActionrFuncs(std::bind(&CPlayScene::runActionGameUpdate, this),
									std::bind(&CPlayScene::runActionTimeOver, this));
	this->addChild(timer_layer, 2);
	// UI�� ���� ���� ���δ�.
	ui_layer = CUILayer::create();
	ui_layer->setRunActionrCountDownFunc(std::bind(&CPlayScene::runActionCountDown, this));
	ui_layer->setRunActionrPauseFunc(std::bind(&CPlayScene::runActionPause, this));
	ui_layer->setRunActionrRestartFunc(std::bind(&CPlayScene::runActionRestart, this));
	this->addChild(ui_layer, 3);


	data_manager = new CDataManager();
	Document d = data_manager->loadData();
	/*if (d. == NULL)
		return true;*/
	//ui_layer->setSoundOption(d["BGM"].IsBool(), d["Effect"].IsBool());

	return true;
}

void CPlayScene::runActionGameUpdate() {
	this->schedule(schedule_selector(CPlayScene::gameUpdate), 1.0f);
}

void CPlayScene::unscheduleGameUpdate() {
	this->unschedule(schedule_selector(CPlayScene::gameUpdate));
}

void CPlayScene::runActionTimeOver() {
	this->endGame(GameState::time_over);
}

void CPlayScene::runActionCountDown() {
	computer_stone_color = oppositionColor(player_stone_color = ui_layer->getPlayerColor());
	timer_layer->runActionCountDown();
}

void CPlayScene::runActionPause() {
	//djfaklsd
	this->unscheduleGameUpdate();
}

void CPlayScene::runActionRestart() {
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
		if (e == GameState::black_win && e == GameState::white_win)
			endGame(e);
#ifdef DEBUG_MODE == 1
		else if (e == GameState::error) {
			CCLOG("************ Computer Fail");
#endif
		}
	}
}

void CPlayScene::gameUpdate(const float dt) {
	/*if (ui_layer->getSeleteColor()) {
		ui_layer->setSeleteColor(false);
		timer_layer->runActionCountDown();
	}*/

	if (!ui_layer->getPause()) {
		//if (timer_layer->getGameStartCheck()) {
			if (now_turn == computer_stone_color) {
				runActionComputer();	// ��ǻ�� ��
				timer_layer->runActionTimer();
			}
		//}
		/*if (timer_layer->getTimeOverCheck()) {
			endGame(GameState::time_over);
		}*/
	}
}

Stone CPlayScene::oppositionColor(const Stone &s) const {
	return s == Stone::black ? Stone::white : Stone::black;
}

void CPlayScene::saveData() const {
	data_manager->saveData("/BGM", ui_layer->getBGMState());
	data_manager->saveData("/Effect", ui_layer->getEffectState());
	data_manager->outputData();
}

void CPlayScene::endGame(GameState s) {
	this->unscheduleGameUpdate();
	ui_layer->createGaemResult(s);
}

bool CPlayScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	return true;
}
void CPlayScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {}
void CPlayScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	try {
		// ������ �����߰ų�, �������� �ʾ����� ���� ����
		//if (ui_layer->getPause() || !timer_layer->getGameStartCheck()) throw GameState::error;
		// ���� ����� ���� �������� Ʋ���� ���� ����	
		if (player_stone_color != now_turn)	throw GameState::error;

		// Ŭ���� ��ǥ�� ������ �������� �ٵϵ� ���� �Լ��� ȣ����
		stone_layer->positionCalculation(touch->getLocation(), player_stone_color);
		// ���� �����ϸ� ������ �ݴ������ ���� �ٲ�
		now_turn = oppositionColor(player_stone_color);
		timer_layer->unscheduleTimer();
		runActionGameUpdate();
	}
	catch (GameState e) {
		// ����, ���� ��, ���� ���� ��, ���� �¸�
#ifdef DEBUG_MODE == 1
		if (e == GameState::error)
			CCLOG("************ onTouchEnded Fail");
#endif			
		if(e == GameState::black_win && e == GameState::white_win)
			endGame(e);

	}
}