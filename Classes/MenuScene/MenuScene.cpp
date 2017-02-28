#include "MenuScene.h"
#include "PlayScene\PlayScene.h"

using namespace cocos2d;
using namespace ui;

Scene* CMenuScene::createScene() {
	Scene* scene = Scene::create();
	Layer* layer = CMenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool CMenuScene::init() {
	if (!Layer::init())
		return false;
	// cocos studio���� �۾��� ��ư�� �ҷ��´�.
	auto rootNode = CSLoader::createNode("MenuLayer.csb");
	// MenuScene Layer�� �߰���Ų��.
	addChild(rootNode, 0, 0);

	// Node���� STARTBUTTON�� ã�Ƽ� ������ �����Ѵ�.
	Button* start_button = (Button*)rootNode->getChildByName("STARTBUTTON");
	Button* exit_button = (Button*)rootNode->getChildByName("STARTBUTTON");
	// ��ư�� ���������� �̺�Ʈ�� �߰��Ѵ�.
	start_button->addTouchEventListener(CC_CALLBACK_2(CMenuScene::onTouch, this));

	return true;
}


// �̺�Ʈ�� �ൿ�� �������ش�.
void CMenuScene::onTouch(Ref* sender, Widget::TouchEventType type) {
	switch (type) {
	case Widget::TouchEventType::BEGAN:
		CCLOG("BEGAN");
		break;
	case Widget::TouchEventType::MOVED:
		CCLOG("MOVED");
		break;
	case Widget::TouchEventType::ENDED:
		CCLOG("ENDED");
		CallFunc* call = CallFunc::create(CC_CALLBACK_0(CMenuScene::replaceScene, this));
		FadeOut* fadeout = FadeOut::create(0.5f);
		Sequence* seq = Sequence::create(fadeout, call, NULL);
		Node* node = this->getChildByTag(0);
		node->runAction(seq);
		break;
	}
}

void CMenuScene::replaceScene() {
	CPlayScene* scene = CPlayScene::create();
	Director::getInstance()->replaceScene(scene);
}