#include "UILayer.h"
#include "SimpleAudioEngine.h"
#include "GossiniDanceSprite.h"

using namespace cocos2d;
using namespace ui;
using namespace CocosDenshion;

CUILayer::CUILayer() : color_selete_state_check(true), menu_visible_check(false), 
bgm_state_check_check(true), effect_State_check(true) {
	RunActionrCountDown = NULL;
}
CUILayer::~CUILayer() {}

void CUILayer::setRunActionrCountDownFunc(std::function<void(void)> countdown) {
	RunActionrCountDown = countdown;
}

void CUILayer::setRunActionrRestartFunc(std::function<void(void)> restart) {
	RunActionrRestart = restart;
}

bool CUILayer::init() {
	if (!Layer::init())
		return false;
	this->createTitle();
	this->createUI();
	this->createCharacter();
	return true;
}

void CUILayer::initUILayer() {
	Button* bt1 = (Button*)this->getChildByName("black");
	Button* bt2 = (Button*)this->getChildByName("white");
	Button* bt3 = (Button*)this->getChildByName("selete");
	Button* bt4 = (Button*)this->getChildByName("option");
	Menu* menu = (Menu*)this->getChildByName("menu");
	Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
	Label* label = (Label*)this->getChildByName("resultstr");
	bt1->setVisible(true);
	bt1->setEnabled(true);
	bt2->setVisible(true);
	bt2->setEnabled(true);
	bt3->setVisible(true);
	bt4->setEnabled(true);
	bt4->setVisible(false);
	menu->setVisible(false);
	soundmenu->setVisible(false);
	label->setVisible(false);
	for (int i = 0; i < 3; ++i) {
		char str[16];
		sprintf_s(str, sizeof(str), "title_%d", i);
		Label* label = (Label*)this->getChildByName(str);
		label->setVisible(true);
	}
	this->idleAnimation();
}

void CUILayer::createGaemResult(const GameState s) {
	// 승리 색상 표현
	std::string str;
	switch (s) {
	case GameState::black_win: str = "Black Win!!"; break;
	case GameState::white_win: str = "White Win!!"; break;
	case GameState::time_over: str = "Time Over"; break;
	}

	Label* label = (Label*)this->getChildByName("resultstr");
	label->setString(str);
	label->setVisible(true);
	visibleOptionMenu();
	Button* bt4 = (Button*)this->getChildByName("option");
	bt4->setEnabled(false);
}

void CUILayer::createTitle() {
	Size winsize = CCDirector::getInstance()->getWinSize();
	float x = winsize.width / 10;
	float y = winsize.height / 10;

	Label* title_0 = Label::create("Selete Color", "font/Marker Felt.ttf", 70);
	title_0->setPosition(winsize.width / 2, y * 8);
	title_0->setColor(Color3B(255, 255, 255));
	this->addChild(title_0, UISceneLocalZ::title_localz, "title_0");

	Label* title_1 = Label::create("Black", "font/Marker Felt.ttf", 50);
	title_1->setPosition(x * 4, y * 7);
	title_1->setColor(Color3B(0, 0, 0));
	this->addChild(title_1, UISceneLocalZ::title_localz, "title_1");

	Label* title_2 = Label::create("White", "font/Marker Felt.ttf", 50);
	title_2->setPosition(x * 6, y * 7);
	title_2->setColor(Color3B(255, 255, 255));
	this->addChild(title_2, UISceneLocalZ::title_localz, "title_2");
}

void CUILayer::createUI() {
	Size winsize = CCDirector::getInstance()->getWinSize();
	float x = winsize.width / 10;
	float y = winsize.height / 10;

	cocos2d::ui::Button* bt1 = ui::Button::create("Button/grey_panel.png","Button/yellow_panel.png","etc/Black.png");
	bt1->setPosition(Vec2(x * 4, y * 5));
	bt1->addTouchEventListener(CC_CALLBACK_2(CUILayer::onTouchBlackSeleteButton, this));
	bt1->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(bt1, UISceneLocalZ::button_localz, "black");

	Button* bt2 = Button::create("Button/grey_panel.png", "Button/yellow_panel.png", "etc/White.png");
	bt2->setPosition(Vec2(x * 6, y * 5));
	bt2->addTouchEventListener(CC_CALLBACK_2(CUILayer::onTouchWhiteSeleteButton, this));
	bt2->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(bt2, UISceneLocalZ::button_localz, "white");
	
	Button* bt3 = Button::create("Button/grey_panel.png", "Button/yellow_panel.png");
	bt3->setPosition(Vec2(x * 5, y * 3));
	bt3->addTouchEventListener(CC_CALLBACK_2(CUILayer::onTouchColorCheckButton, this));
	bt3->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(bt3, UISceneLocalZ::button_localz, "selete");

	Size winSize = CCDirector::getInstance()->getWinSize();

	Button* optionbt = ui::Button::create("Button/grey_panel.png", "Button/yellow_panel.png");
	optionbt->setPosition(Vec2(winSize.width / 1.1f, winSize.height / 1.1f));
	optionbt->addTouchEventListener(CC_CALLBACK_2(CUILayer::onTouchOptionMenuButton, this));
	optionbt->setVisible(false);
	this->addChild(optionbt, UISceneLocalZ::button_localz, "option");

	Label* label = Label::create();
	label->setPosition(500, 500);
	label->setSystemFontSize(50);
	label->setColor(Color3B(255, 255, 255));
	label->setVisible(false);
	this->addChild(label, UISceneLocalZ::resultstr_localz, "resultstr");

	createOptionMenu();
	createSoundMenu();
}

void CUILayer::createCharacter() {
	Size winsize = CCDirector::getInstance()->getWinSize();
	int j = 0;
	for (int i = 0; i < 2; ++i) {
		j = i == 0 ? 1 : 9;
		CGossiniDanceSprite* sprite = CGossiniDanceSprite::create();
		sprite->setPosition(winsize.width / 10 * j, winsize.height / 10 * 7);
		char str[16];
		sprintf_s(str, sizeof(str), "character%d", i);
		this->addChild(sprite, UISceneLocalZ::animation_localz, str);
	}
}

void CUILayer::idleAnimation() {
	for (int i = 0; i < 2; ++i) {
		char str[16];
		sprintf_s(str, sizeof(str), "character%d", i);
		CGossiniDanceSprite* sprite = (CGossiniDanceSprite*)this->getChildByName(str);
		sprite->idleAnimation();
	}
}

void CUILayer::runActionDanceAnimation() {
	for (int i = 0; i < 2; ++i) {
		char str[16];
		sprintf_s(str, sizeof(str), "character%d", i);
		CGossiniDanceSprite* sprite = (CGossiniDanceSprite*)this->getChildByName(str);
		sprite->runActionAnimation((CGossiniDanceSprite::GossiniDanceState)i);
	}
}

void CUILayer::changeDanceAnimation() {
	for (int i = 0; i < 2; ++i) {
		char str[16];
		sprintf_s(str, sizeof(str), "character%d", i);
		CGossiniDanceSprite* sprite = (CGossiniDanceSprite*)this->getChildByName(str);
		sprite->changeDanceAnimation();
	}
}

void CUILayer::createOptionMenu() {
	Label* label1 = Label::create("restart", "font/Marker Felt.ttf", 30);
	label1->setColor(Color3B(0, 0, 0));
	Label* label2 = Label::create("sound", "font/Marker Felt.ttf", 30);
	label2->setColor(Color3B(0, 0, 0));
	Label* label3 = Label::create("exit", "font/Marker Felt.ttf", 30);
	label3->setColor(Color3B(0, 0, 0));

	MenuItemLabel* itemlabel1 = MenuItemLabel::create(label1, CC_CALLBACK_1(CUILayer::onTouchRestartGameButton, this));
	auto itemlabel2 = MenuItemLabel::create(label2, CC_CALLBACK_1(CUILayer::onTouchSoundMenuButton, this));
	auto itemlabel3 = MenuItemLabel::create(label3, CC_CALLBACK_1(CUILayer::onTouchExitButton, this));

	Menu* menu = Menu::create(itemlabel1, itemlabel2, itemlabel3, NULL);
	menu->alignItemsVertically();
	menu->setVisible(false);
	this->addChild(menu, UISceneLocalZ::optionmenu_localz, "menu");
}

void CUILayer::createSoundMenu() {
	auto itemimg1_1 = MenuItemImage::create("Button/BGM_1.png", "Button/BGM_2.png");
	auto itemimg1_2 = MenuItemImage::create("Button/BGM_2.png", "Button/BGM_1.png");
	auto itemimg2_1 = MenuItemImage::create("Button/Effect_1.png", "Button/Effect_2.png");
	auto itemimg2_2 = MenuItemImage::create("Button/Effect_2.png", "Button/Effect_1.png");
	MenuItemToggle* toggle1 = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(CUILayer::onTouchBGM_ON_OFF_Toggle, this), itemimg1_1, itemimg1_2, NULL);

	auto* toggle2 = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(CUILayer::onToucheffect_ON_OFF_Toggle, this), itemimg2_1, itemimg2_2, NULL);

	auto itemimg3 = MenuItemImage::create("Button/Back_1.png", "Button/Back_2.png",
		CC_CALLBACK_1(CUILayer::onTouchSoundMenuBackButton, this));

	Menu* soundmenu = Menu::create(toggle1, toggle2, itemimg3, NULL);
	soundmenu->alignItemsVertically();
	soundmenu->setVisible(false);
	this->addChild(soundmenu, UISceneLocalZ::soundmenu_localz, "soundmenu");
}

void CUILayer::visibleOptionMenu() {
	Menu* menu = (Menu*)this->getChildByName("menu");
	if (!menu_visible_check) {
		menu_visible_check = true;
		*pause_check = true;
		menu->setVisible(menu_visible_check);
	}
	else {
		*pause_check = false;
		menu_visible_check = false;
		menu->setVisible(menu_visible_check);
		Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
		soundmenu->setVisible(menu_visible_check);
	}
}

void CUILayer::visibleSoundMenu() {
	Menu* menu = (Menu*)this->getChildByName("menu");
	menu->setVisible(false);
	Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
	soundmenu->setVisible(true);
}

void CUILayer::enabledColorSeleteButton(const Stone color) {
	// 색상을 받아서 저장하고
	player_color = color;
	// 색상이 검은색이면 상태값을 flase, 아니면 true를 저장한다.
	player_color == Stone::black ? color_selete_state_check = false : color_selete_state_check = true;
	// 상태값으로 버튼1과 버튼2의 상태를 반대로 보이도록 한다.
	Button* bt1 = (Button*)this->getChildByName("black");
	bt1->setEnabled(color_selete_state_check);
	Button* bt2 = (Button*)this->getChildByName("white");
	bt2->setEnabled(!color_selete_state_check);
}

void CUILayer::onTouchBlackSeleteButton(Ref* sender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		enabledColorSeleteButton(Stone::black);
	}
}

void CUILayer::onTouchWhiteSeleteButton(Ref* sender, Widget::TouchEventType type) {
	switch (type) {
	case Widget::TouchEventType::ENDED:
		enabledColorSeleteButton(Stone::white);
	}
}

void CUILayer::onTouchColorCheckButton(Ref* sender, Widget::TouchEventType type) {
	Button* bt1 = (Button*)this->getChildByName("black");
	Button* bt2 = (Button*)this->getChildByName("white");
	// 검은돌이나 백색돌을 선택 후 버튼3을 누르면
	// 화면에 띄운 버튼을 모두 숨기고 정지를 풀어준다.
	if (!bt1->isEnabled() || !bt2->isEnabled()) {
		switch (type) {
		case Widget::TouchEventType::ENDED:
			Button* bt3 = (Button*)this->getChildByName("selete");
			bt1->setVisible(false);
			bt2->setVisible(false);
			bt3->setVisible(false);
			Button* bt4 = (Button*)this->getChildByName("option");
			bt4->setVisible(true);
			for (int i = 0; i < 3; ++i) {
				char str[16];
				sprintf_s(str, sizeof(str), "title_%d", i);
				Label* label = (Label*)this->getChildByName(str);
				label->setVisible(false);
			}
			RunActionrCountDown();
		}
	}
}

void CUILayer::onTouchOptionMenuButton(Ref* sender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		visibleOptionMenu();
	}
}

void CUILayer::onTouchRestartGameButton(Ref* sender) {
	menu_visible_check = false;
	RunActionrRestart();
}

void CUILayer::onTouchSoundMenuButton(Ref* sender) {
	visibleSoundMenu();
}

void CUILayer::onTouchExitButton(Ref* sender) {
	CCDirector::getInstance()->end();
}

void CUILayer::onTouchBGM_ON_OFF_Toggle(Ref* sender) {
	if (bgm_state_check_check) {
		bgm_state_check_check = false;
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		return;
	}
	bgm_state_check_check = true;
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void CUILayer::onToucheffect_ON_OFF_Toggle(Ref* sender) {
	if (effect_State_check) {
		effect_State_check = false;
		SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
	}
	else {
		effect_State_check = true;
		SimpleAudioEngine::getInstance()->setEffectsVolume(0);
	}
}

void CUILayer::onTouchSoundMenuBackButton(Ref* sender) {
	Menu* menu = (Menu*)this->getChildByName("menu");
	menu->setVisible(true);
	Menu* soundmenu = (Menu*)this->getChildByName("soundmenu");
	soundmenu->setVisible(false);
}

Stone CUILayer::getPlayerColor() const {
	return player_color;
}

void CUILayer::setPauseCheck(bool* b) {
	pause_check = b;
}

bool CUILayer::getBGMState() const {
	return bgm_state_check_check;
}

bool CUILayer::getEffectState() const {
	return effect_State_check;
}

void CUILayer::setSoundOption(const bool bgm, const bool effect) {
	bgm_state_check_check = bgm;
	effect_State_check = effect;
	if (bgm && !SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/bgm/01_01_trial.mp3", true);
	}
	if (!effect) {
		SimpleAudioEngine::getInstance()->preloadEffect("sound/effect/ui-sound8.wav");
		SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
	}
}