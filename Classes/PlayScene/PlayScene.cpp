#include "PlayScene\PlayScene.h"
#include "PlayScene\StoneLayer.h"
#include "PlayScene\UILayer.h"
#include "PlayScene\TimerLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;


// Json ����� ���� ����
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/pointer.h"

using namespace rapidjson;
using namespace std;


CPlayScene::CPlayScene() : m_PlayerColor(Stone::Emptied), 
						   m_Turn(Stone::Black)  {}
CPlayScene::~CPlayScene() {
	DataSave();
}


void CPlayScene::DataLoad() {
	ifstream ifs("data/optiondata.json");
	IStreamWrapper isw(ifs);
	Document d;
	d.ParseStream(isw);
	assert(d.HasMember("BGM"));
	assert(d.HasMember("Effect"));
	CCLOG("DataLoad, d[BGM].IsBool() : %s", d["BGM"].IsBool() ? "true" : "false");
}
void CPlayScene::DataSave() {
	Document doc;
	Pointer("/BGM").Set(doc, m_UiLayer->getBGMState());
	Pointer("/Effect").Set(doc, m_UiLayer->getEffectState());
	
	// ����ϴ� �κ�
	ofstream ofs("data/optiondata.json");
	OStreamWrapper osw(ofs);
	Writer<OStreamWrapper> writer(osw);
	doc.Accept(writer);
	// rapidjson Ʃ�丮��
	// https://www.slideshare.net/PaulKim81/rapid-json-tutorial?ref=http://jacking.tistory.com/1431
	// http://rapidjson.org/md_doc_pointer.html Json Pointer�� �߰� �ϴ� �κ�
	// http://rapidjson.org/md_doc_stream.html  �� ��� �κ�

//#include "rapidjson\stringbuffer.h"
//#include "rapidjson\prettywriter.h"
	//StringBuffer buffer;
	//PrettyWriter<StringBuffer> writer(buffer);		// �� �ٲ� ����
	//doc.Accept(writer);
	//Writer<StringBuffer> w(buffer);					// �� �ٲ� ����
	//doc.Accept(w);
	//auto os = std::ofstream("data/test1", std::ofstream::out);
	//os << buffer.GetString() << std::endl;
}

bool CPlayScene::init() {
	if (!Scene::init())
		return false;

	DataLoad();

	// ȭ�� ��ȯ ȿ��
	LayerColor* layer = LayerColor::create(Color4B(0, 0, 0, 0));
	this->addChild(layer, 10);
	FadeOut* fout = FadeOut::create(0.5f);
	layer->runAction(fout);

	// ���콺 �̺�Ʈ �߰�
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true); // Ŭ�� ��Ȯ�� ���
	listener->onTouchBegan = CC_CALLBACK_2(CPlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CPlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CPlayScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// ���̾� �߰�
	m_StoneLayer = CStoneLayer::create();
	this->addChild(m_StoneLayer, 1);
	m_TimerLayer = CTimerLayer::create();
	this->addChild(m_TimerLayer, 2);
	// UI�� ���� ���� ���δ�.
	m_UiLayer = CUILayer::create();
	this->addChild(m_UiLayer, 3);
	
	// 0.1�ʿ� �ѹ��� ���� ������Ʈ�� ���۽�Ų��.
	this->schedule(schedule_selector(CPlayScene::gameupdate), 0.1f);
	
	return true;
}

void CPlayScene::gameupdate(const float dt) {
	// ���� ����� �˻�
	if (m_UiLayer->getReStart()) {
		m_UiLayer->ResetUILayer();
		m_TimerLayer->resetTimerLayer();
		m_StoneLayer->initStone();
		m_Turn = Stone::Black;
	}

	if (!m_UiLayer->getPause()) {
		// ������ �����ߴ��� �˻��Ѵ�.
		if (m_TimerLayer->getGameStart()) {
			m_PlayerColor = static_cast<Stone>(m_UiLayer->getPlayerColor());
			m_ComputerColor = m_PlayerColor == Stone::Black ? Stone::White : Stone::Black;

			// �÷��̾� ��
			if (m_PlayerColor == m_Turn) {
				// ���ѽð� ǥ��
				try {
					m_TimerLayer->setTimerVisible(true);
					m_TimerLayer->UpdateTimer(dt);
				}
				catch (int e) {
					if (e == GameState::TimeOver) {
						m_UiLayer->GaemResult(GameState::TimeOver);
						CCLOG("************ Time Over *********");
					}
				}
			}
			// ��ǻ�� ��
			else if (m_Turn == m_ComputerColor) {
				try {
					// AI �ൿ �Լ� ȣ��
					m_StoneLayer->CreateStone(m_ComputerColor);
					EffectSound();
					m_Turn = m_ComputerColor == Stone::Black ? Stone::White : Stone::Black;
					// Ÿ�̸� �ʱ�ȭ
					m_TimerLayer->resetTimer();
				}
				// �¸�ó��
				catch (int e) {
					if (e == GameState::Win)
						EndGame();
					else if (e == 1) {
						CCLOG("***************** Computer Fail");
					}
				}
			}
		}
		// ���� ���� �� ������
		else if (!m_TimerLayer->getGameStart()) {
			// ������ ī��Ʈ �ٿ��� ���̰� �ϰ�
			m_TimerLayer->setCountDownVisible(true);

			// ī��Ʈ �ٿ��� ������Ʈ�ؼ� �ð��� �帣�� �Ѵ�.
			if (m_TimerLayer->UpdateCountDown(dt)) {
				// ī��Ʈ �ٿ��� ������ ������ �δ� ���ѽð��� ���̰� �ϰ�
				// ī��Ʈ �ٿ��� �����.
				m_TimerLayer->setTimerVisible(true);
				m_TimerLayer->setCountDownVisible(false);
			}
		}
	}
}

void CPlayScene::EffectSound() {
	if (m_UiLayer->getEffectState()) {
		SimpleAudioEngine::getInstance()->playEffect("ui-sound8.wav");
	}
}

void CPlayScene::EndGame() {
	m_UiLayer->GaemResult(m_Turn);
}

bool CPlayScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	return true;
}
void CPlayScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {}
void CPlayScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	try {
		// ������ �����߰ų�, �������� �ʾ����� ���� ����
		if (m_UiLayer->getPause() || !m_TimerLayer->getGameStart())
			throw (int)GameState::error;
		// ���� ����� ���� �������� Ʋ���� ���� ����
		if (m_PlayerColor != m_Turn)
			throw (int)GameState::error;

		// Ŭ���� ��ǥ�� ������ �������� �ٵϵ� ���� �Լ��� ȣ����
		m_StoneLayer->PosCalculation(touch->getLocation(), m_PlayerColor);
		EffectSound();
		// ���� �����ϸ� ������ �ݴ������ ���� �ٲ�
		m_Turn = m_PlayerColor == Stone::Black ? Stone::White : Stone::Black;
	}
	catch (int e) {
		// ����, ���� ��, ���� ���� ��, ���� �¸�
		if (e == GameState::Win)
			EndGame();
		else if (e == 1){
			CCLOG("*****************onTouchEnded Fail");
		}
	}
}