#include "StoneLayer.h"
#include "Computer.h"

using namespace cocos2d;

const float fMarginPixel = 11.0;
const float fIntervalPixel = 21.0;

const int sizeX = 19;
const int sizeY = 19;

CStoneLayer::CStoneLayer() : m_bCheckWin(0) {}
CStoneLayer::~CStoneLayer() {
	// ********** �������� �̰� �´��� Ȯ�� �ʿ���
	// ********** �������� �̰� �´��� Ȯ�� �ʿ���
	// ********** �������� �̰� �´��� Ȯ�� �ʿ���
	CC_SAFE_DELETE(m_Computer);
}

bool CStoneLayer::init() {
	if (!Layer::init())
		return false;
	count = 0;
	// ��ǻ�� �����Ҵ�
	m_Computer = new CComputer;
	// �ٵ����� �����Ѵ�.
	initStone();
	return true;
}

void CStoneLayer::initStone() {
	m_bCheckWin = false;
	for (int i = 0; i < 2; ++i) {
		m_iXY[i] = 10;
		m_iXY[i+1] = 8;
	}
	this->removeAllChildren();
	labelvec.clear();
	CreateBoard();
	// �ٵϵ��� ���� �߰��ϸ�, ��ǥ���� Tag�� �ٿ��д�.
	// �ʱ� ������ �ٵϵ��� ���̴�.
	for (int y = 0; y < sizeY; ++y) {
		for (int x = 0; x < sizeX; ++x) {
			CStoneSprite* sprite = CStoneSprite::create();
			this->addChild(sprite, 1, sizeY * y + x);

			// ****** ����׿� ������ �������� ���̺�	
			// ****** ����׿� ������ �������� ���̺�	
			// ****** ����׿� ������ �������� ���̺�	
			float posX = x * m_fIntervalPixel + m_fMarginPixel + m_StrPos.x;
			float posY = y * m_fIntervalPixel + m_fMarginPixel + m_StrPos.y;

			Label* label = Label::create();
			label->setPosition(posX, posY);
			label->setSystemFontSize(25);
			label->setString("0");
			label->setVisible(false);
			this->addChild(label, 10);
			labelvec.push_back(label);
		}
	}
}

void CStoneLayer::CreateBoard() {
	Sprite* base = Sprite::create("Base.jpg");

	// ȭ�� ũ�⸦ �������� ����
	Size winsize = Director::getInstance()->getWinSize();
	
	// �ٵ��� �̹����� ũ�⸦ �����ͼ� ����
	Size basesize = base->getContentSize();

	// ���� ȭ���� ���� �Ǵ� ������ ũ�⿡ �°� �ٵ����� �ø��� ���ؼ�
	// ȭ���� ����, ���� ���� �ٵ��� ũ��� ������ ������ ����
	float widthscale = winsize.width / basesize.width;
	float heightscale = winsize.height / basesize.height;
	
	// ������ ���� ���� ������
	m_fScale = widthscale < heightscale ? widthscale : heightscale;

	// ������ ���� �ٵ��� ũ�⸦ ������
	basesize = basesize * m_fScale;

	// �ٵ��� �ȼ� ���� ������ ���缭 ������
	m_fMarginPixel = fMarginPixel * m_fScale;
	m_fIntervalPixel = fIntervalPixel * m_fScale;

	// ��Ŀ ��ġ�� �������� �������ְ� ��ġ�� ȭ���� �߾����� ��Ƽ� �߰��Ѵ�.
	base->setAnchorPoint(Vec2(0.5, 0.5));
	base->setScale(m_fScale);
	base->setPosition(winsize / 2);
	addChild(base, 0);

	// �ٵ��� ��������, ������ ���� �����صд�.
	// �ٵϵ��� �Ѷ� ����Ѵ�.
	m_StrPos = Vec2(base->getPosition().x - basesize.width / 2,
		base->getPosition().y - basesize.height / 2);
	m_EndPos = Vec2(base->getPosition().x + basesize.width / 2,
		base->getPosition().y + basesize.height / 2);
}

void CStoneLayer::PosCalculation(const Vec2 &pos, int s) {
	// �ٵϵ��� ������ ��ġ�� �ٵ��� �ٱ����̸� ���� ����
	if (pos < m_StrPos || pos > m_EndPos) {
		CCLOG("*************StoneLayer.cpp PosCalculation, Fail");
		throw (int)GameState::error;
	}

	// ��ǥ ���
	int x = round((pos.x - m_fMarginPixel - m_StrPos.x) / m_fIntervalPixel);
	int y = round((pos.y - m_fMarginPixel - m_StrPos.y) / m_fIntervalPixel);
	this->CreateStone(Vec2(x,y), s);
}

void CStoneLayer::CreateStone(int s) {
	m_Computer->CalculationScore(this);
	CreateStone(m_Computer->ComputerTurn(this), s);
}

void CStoneLayer::CreateStone(const Vec2 &pos, int s) {
	// �ش� ��ǥ�� ���� �ҷ��´�.
	CStoneSprite* pSprite = getSprite(pos.x, pos.y);

	// �ҷ��� ���� ������ Ȯ���ؼ� ���� ������ ���� ����
	if (pSprite->getStoneType() != Stone::Emptied) {
		CCLOG("******* StoneLayer.cpp CreateStone, Fail");
		throw (int)GameState::error;
	}

	// ����ִٸ� ��ǥ ������ ���� ��ġ�� ����ؼ�
	float posX = pos.x * m_fIntervalPixel + m_fMarginPixel + m_StrPos.x;
	float posY = pos.y * m_fIntervalPixel + m_fMarginPixel + m_StrPos.y;

	// �ٵϵ��� �߰��Ѵ�.
	pSprite->initSprite(Vec2(posX, posY), m_fScale, s);

	// ������ ������ ��������� �ʿ�����Ƿ� ��Ȱ��ȭ �����ش�.
	pSprite->setActive(false);
	
	// �ִ�, �ּ� ��ǥ�� ����
	if (m_iXY[eMaxX] < pos.x) m_iXY[eMaxX] = pos.x;
	if (m_iXY[eMinX] > pos.x) m_iXY[eMinX] = pos.x;
	if (m_iXY[eMaxY] < pos.y) m_iXY[eMaxY] = pos.y;
	if (m_iXY[eMinY] > pos.y) m_iXY[eMinY] = pos.y;

	// ���� ���θ� �˻��Ѵ�.
	CheckBoard(s);

	// �ֺ� 2ĭ�� Ȱ��ȭ ��Ų��.
	for (int dir = 0; dir < DirCount; ++dir){
		for (int depth = 1; depth <= 2; ++depth) {
			CStoneSprite* sprite = SearcharoundSprite(pos.x, pos.y, dir, depth);
			if (sprite != nullptr && sprite->getStoneType() == Stone::Emptied) {
				sprite->setActive(true);
			}
		}
	}
	
	// ****** ��������� ���ؼ� ���� �� �Լ� ȣ����
	// ��ǻ�� ���������� ����ϸ� ȿ������
	// ****** ��������� ���ؼ� ���� �� �Լ� ȣ����
	// ��ǻ�� ���������� ����ϸ� ȿ������
	m_Computer->CalculationScore(this);
}

CStoneSprite* CStoneLayer::getSprite(int x, int y) const {
	return dynamic_cast<CStoneSprite*>(this->getChildByTag(sizeY * y + x));
}

CStoneSprite* CStoneLayer::SearcharoundSprite(int x, int y, int dir, int depth) const {
	int ix = 0, iy = 0;

	switch (dir) {
	case Direction::Up: ix = 0, iy = 1; break;
	case Direction::Down: ix = 0, iy = -1; break;
	case Direction::Left: ix = -1, iy = 0; break;
	case Direction::Right: ix = 1, iy = 0; break;
	case Direction::UpperLeft: ix = -1, iy = 1; break;
	case Direction::UpperRight: ix = 1, iy = 1; break;
	case Direction::LowerLeft: ix = -1, iy = -1; break;
	case Direction::LowerRight: ix = 1, iy = -1; break;
	default: ix = 0, iy = 0; break;
	}

	int tx = ix * depth + x;
	int ty = iy * depth + y;
	// ��ǥ ���� 0���� �۰ų� �ٵ��� ���� ũ�� �˻����� �ʴ´�.
	if (tx < 0 || tx > sizeX || ty < 0 || ty > sizeY) return nullptr;

	return this->getSprite(tx, ty);
}

void CStoneLayer::CheckBoard(const int s) {
	m_iCheckCount = 0;
	m_iCheckColor = s;
	bool check[2] = { 0, 0 };
	if (m_iXY[eMaxX] - m_iXY[eMinX] > 3) {
		CheckWidth();
		check[0] = true;
	}
	if (m_iXY[eMaxY] - m_iXY[eMinY] > 3) {
		CheckHeight();
		check[1] = true;
	}
	if (check[0] && check[1]) {
		CheckSlash();
		CheckBackSlash();
	}

	// ************* ����׿�
	// ************* ����׿�
	// ************* ����׿�
	CCLOG("count : %d\n", count);
	if (m_iCheckColor == 0)
		CCLOG("CheckBoard Complete Color : Black\n");
	else if (m_iCheckColor == 1)
		CCLOG("CheckBoard Complete Color : White\n");
}

void CStoneLayer::CheckStone(const int x, const int y) {
	
	// ************ ����׿� ��� Ƚ�� ��Ƹ���
	// ************ ����׿� ��� Ƚ�� ��Ƹ���
	// ************ ����׿� ��� Ƚ�� ��Ƹ���
	++count;


	CStoneSprite* sprite = this->getSprite(x, y);
	if (sprite->getStoneType() == m_iCheckColor) {
		if (++m_iCheckCount == 5) {		
			m_bCheckWin = true;
		}
		else if (m_iCheckCount > 5) {
			m_bCheckWin = false;
		}
	}
	else m_iCheckCount = 0;
}

void CStoneLayer::CheckWidth() {
	// ���� �˻� �¿��� ��� �˻�, ������ ���� ���� �̵�
	for (int y = m_iXY[eMinY]; y <= m_iXY[eMaxY]; ++y) {
		for (int x = m_iXY[eMinX]; x <= m_iXY[eMaxX]; ++x) {
			CheckStone(x, y);
			//CCLOG("*****CheckWidth***** color : %d, count : %d", m_iCheckColor, m_iCheckCount);
		}
		m_iCheckCount = 0;
		//CCLOG("\n");
	}
	if (m_bCheckWin) throw (int)GameState::Win;
}

void CStoneLayer::CheckHeight() {
	// ���� �˻� �ؿ��� ���� �˻�, ������ ���� ������ �̵�
	for (int x = m_iXY[eMinX]; x <= m_iXY[eMaxX]; ++x) {
		for (int y = m_iXY[eMinY]; y <= m_iXY[eMaxY]; ++y) {
			CheckStone(x, y);
			//CCLOG("*****CheckHeight***** color : %d, count : %d", m_iCheckColor, m_iCheckCount);
		}
		m_iCheckCount = 0;
		//CCLOG("\n");
	}
	if (m_bCheckWin) throw (int)GameState::Win;
}

void CStoneLayer::CheckSlash() {
	// �밢�� �˻� ���������� ������ �Ʒ� �������� �˻� 0, 19 ~ 19, 0����
	for (int ix = m_iXY[eMinX], iy = m_iXY[eMaxY];
		ix <= m_iXY[eMaxX] && iy >= m_iXY[eMinY];
		ix == m_iXY[eMaxX] ? --iy : ++ix) {
		for (int y = iy, x = ix; x >= m_iXY[eMinX] && y >= m_iXY[eMinY]; --y, --x) {
			CheckStone(x, y);
			CCLOG("CheckSlash x:%d, y:%d, color:%d, count : %d", x, y, m_iCheckColor, m_iCheckCount);
		}
		m_iCheckCount = 0;
		CCLOG("\n");
	}
	if (m_bCheckWin) throw (int)GameState::Win;
}

void CStoneLayer::CheckBackSlash() {
	// ����˻簡 ����� �ȵ� 6���� �Ǿ�� �¸���
	for (int ix = m_iXY[eMinX], iy = m_iXY[eMinY];
		ix <= m_iXY[eMaxX] && iy <= m_iXY[eMaxY];
		ix == m_iXY[eMaxX] ? ++iy : ++ix) {
		for (int y = iy, x = ix; x >= m_iXY[eMinX] && y <= m_iXY[eMaxY]; ++y, --x) {
			CheckStone(x, y);
			//CCLOG("CheckBackSlash x:%d, y:%d, color:%d, count : %d", x, y, m_iCheckColor, m_iCheckCount);
		}
		m_iCheckCount = 0;
		//CCLOG("\n");
	}
	if (m_bCheckWin) throw (int)GameState::Win;
}