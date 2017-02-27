#include "Computer.h"
#include "StoneLayer.h"
#include <time.h>

using namespace cocos2d;
using namespace std;

CComputer::CComputer() {
	// ********** ����׿� ����
	// ********** ����׿� ����
	// ********** ����׿� ����
	// ********** ����׿� ����
	count1 = 0;
	srand((unsigned)time(NULL));
}
CComputer::~CComputer() {}

#define MAXVAL(i, j) i = j + 2 < 18 ? j + 2 : 18
#define MINVAL(i, j) i = j - 2 > 0 ? j - 2 : 0

// ������ ����ȭ �ؼ� �켱������ �� ���ϱ� ������ �ϱ�
// ������ ����ȭ �ؼ� �켱������ �� ���ϱ� ������ �ϱ�
// ������ ����ȭ �ؼ� �켱������ �� ���ϱ� ������ �ϱ�

// ��ǻ�Ͱ� ���� �����ϸ� ���� ������
// ��ǻ�Ͱ� ���� �����ϸ� ���� ������
// ��ǻ�Ͱ� ���� �����ϸ� ���� ������
// ��ǻ�Ͱ� ���� �����ϸ� ���� ������
// ��ǻ�Ͱ� ���� �����ϸ� ���� ������
// ��ǻ�Ͱ� ���� �����ϸ� ���� ������
// ��ǻ�Ͱ� ���� �����ϸ� ���� ������

void CComputer::CalculationScore(Node* node) {
	CStoneLayer* stonelayer = dynamic_cast<CStoneLayer*>(node);
	
	int maxX = MAXVAL(maxX, stonelayer->m_iXY[CStoneLayer::Min_MaxXY::eMaxX]);
	int minX = MINVAL(minX, stonelayer->m_iXY[CStoneLayer::Min_MaxXY::eMinX]);
	int maxY = MAXVAL(maxY, stonelayer->m_iXY[CStoneLayer::Min_MaxXY::eMaxY]);
	int minY = MINVAL(minY, stonelayer->m_iXY[CStoneLayer::Min_MaxXY::eMinY]);

	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			CStoneSprite* sprite = stonelayer->getSprite(x, y);
			int maxscore[2] = { 0, };
			if (sprite->getActive()) {
				for (int dir = 0; dir < CStoneLayer::Direction::DirCount; ++dir) {

					int score[2] = { 0, };
					for (int depth = 1; depth <= 4; ++depth) {
						sprite = stonelayer->SearcharoundSprite(x, y, dir, depth);
						if (sprite == nullptr) break;
						int color = sprite->getStoneType();
						PreviousColor[depth - 1] = color;

						// ���� ����.
						if (color == Stone::Emptied) {
							if (depth != 1) {
								// 2�� ���� ����°�?
								if (color == PreviousColor[depth - 2]) break;
								score[PreviousColor[depth - 2]] += (depth - 2);
							}
						}
						// ���� �ִ�. 1~4
						else if (depth == 1) {
							score[color] += 1;
						}
						else {
							if (PreviousColor[depth - 1] == PreviousColor[depth - 2]) {
								score[color] += (depth*1.8f);
							}
							else if (depth > 2 && PreviousColor[depth - 1] == PreviousColor[depth - 3]) {
								score[color] += 1;
							}
							else if (PreviousColor[depth - 1] != Stone::Emptied) {
								score[color] -= depth;
								break;
							}
						}
					}
					for (int i = 0; i < 4; ++i) {
						PreviousColor[i] = Stone::Emptied;
					}
					maxscore[0] += score[0];
					maxscore[1] += score[1];
				}
				int iPointMaxScore = (maxscore[Black] > maxscore[White] ? maxscore[Black] : maxscore[White]);
				vector<Label*> labelvector = stonelayer->labelvec;
				Label* label = labelvector.at(19 * y + x);
				char str[4];
				sprintf_s(str, sizeof(str), "%d", iPointMaxScore);
				label->setString(str);
				label->setColor(Color3B(250, 50, 50));
				label->setVisible(true);
				sprite = stonelayer->getSprite(x, y);
				sprite->setScore(iPointMaxScore);
				++count1;
			}
		}
	}
	CCLOG("ComputerTurn count1 : %d", count1);
}

Vec2 CComputer::ComputerTurn(Node* node) {
	CStoneLayer* stonelayer = dynamic_cast<CStoneLayer*>(node);
	// ��ü �ٵ��� 19*19�� ���ؼ� ������ ����ϰ�
	// ���� ������ ���� �ڸ��� ������ �Ѵ�.
	int maxX = MAXVAL(maxX, stonelayer->m_iXY[CStoneLayer::Min_MaxXY::eMaxX]);
	int minX = MINVAL(minX, stonelayer->m_iXY[CStoneLayer::Min_MaxXY::eMinX]);
	int maxY = MAXVAL(maxY, stonelayer->m_iXY[CStoneLayer::Min_MaxXY::eMaxY]);
	int minY = MINVAL(minY, stonelayer->m_iXY[CStoneLayer::Min_MaxXY::eMinY]);
	
	vector<Vec2> vector;
	int iBestPointScore = 0;
	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			CStoneSprite* sprite = stonelayer->getSprite(x, y);
			if (sprite->getActive()) {
				if (sprite->getScore() > iBestPointScore) {
					iBestPointScore = sprite->getScore();
					vector.clear();
					vector.push_back(Vec2(x, y));
				}
				else if (sprite->getScore() == iBestPointScore) {
					vector.push_back(Vec2(x, y));
				}
			}
		}
	}
	if (vector.size() == 0) vector.push_back(Vec2(9, 9));
	return vector[rand() % vector.size()];
}

// 1.  ù��° ���� �����.		* �˻� ����, ���� ����
// 1.1 �ι�° ���� �����.		* �˻� ����, ���� ����
// 1.2 �ι�° ���� �������̴�.	* �߰� �˻�, ���� ����
// 1.3 �ι�° ���� ����̴�.		* �߰� �˻�, ���� ����

// 2.  ù��° ���� �������̴�.	* ���� ����
// 2.1 �ι�° ���� �����.		* �߰� �˻�, ���� ���� ����
// 2.2 �ι�° ���� �������̴�.	* �߰� �˻�, ���� ����
// 2.3 �ι�° ���� ����̴�.		* �˻� ����

// 3.  ù��° ���� ����̴�.		* ���� ����
// 3.1 �ι�° ���� �����.		* �߰� �˻�, ���� ���� ����
// 3.2 �ι�° ���� �������̴�.	* �˻� ����
// 3.3 �ι�° ���� ����̴�.		* �߰� �˻�, ���� ����

// ������� 1���� ���ϰ�
// ���� ������ 2���� ���Ѵ�.
// �������� ���� ������ ����.

//	else if (depth == 2) {
//		if (PreviousColor[depth - 2] == Stone::Emptied) ++score[color];
//		else if (PreviousColor[depth - 2] == PreviousColor[depth - 1]) ++score[color] *= 2;
//		else {
//			score[color] -= 2;
//			break;
//		}

//		

//	}
//	else if (depth == 3) {
//		// ���� ����
//		if (PreviousColor[1] == color) {
//			// 2�� �Ǵ� 3�� �����̴�.
//			iTmpScore += 2;
//			if (PreviousColor[0] == color) {
//				iTmpScore += 3;
//			}
//		}
//		// ���� �ٸ���
//		else {
//			// ���� ���� �����.
//			if (PreviousColor[1] == Stone::Emptied) {
//				// ���� ���� ������� ù���� ���� ����.
//				if (PreviousColor[0] == color) {
//					iTmpScore += 2;
//				}
//				// �������� ����� ù���� ���� �ٸ���. �˻��ߴ�
//				else {
//					// �ٸ� ���� ������ ������ ������ ��´�.
//					iTmpScore -= 1;
//					break;
//				}
//			}
//			// ���� ���� �ٸ��� ���� �ʾҴٴ� ���� �ٸ� �� �˻��ߴ�
//			else {
//				// �ٸ� ���� ���� ������ ������ ������ ��´�.
//				iTmpScore -= 1;
//				break;
//			}
//		}
//	}
//}



//// �߰� �˻�
//// ����°, �׹�° ���� �˻�
//if (b) {
//	for (int depth = 3; depth <= 4; ++depth) {
//		sprite = stonelayer->SearcharoundSprite(x, y, dir, depth);
//		if (sprite == nullptr) break;
//		int color = sprite->getStoneType();

//		// ���� �ִ�.
//		if (color != Stone::Emptied) {
//			if (depth == 3) {
//				// ���� ����
//				if (PreviousColor[1] == color) {
//					// 2�� �Ǵ� 3�� �����̴�.
//					iTmpScore += 2;
//					if (PreviousColor[0] == color) {
//						iTmpScore += 3;
//					}
//				}
//				// ���� �ٸ���
//				else {
//					// ���� ���� �����.
//					if (PreviousColor[1] == Stone::Emptied) {
//						// ���� ���� ������� ù���� ���� ����.
//						if (PreviousColor[0] == color) {
//							iTmpScore += 2;
//						}
//						// �������� ����� ù���� ���� �ٸ���. �˻��ߴ�
//						else {
//							// �ٸ� ���� ������ ������ ������ ��´�.
//							iTmpScore -= 1;
//							break;
//						}
//					}
//					// ���� ���� �ٸ��� ���� �ʾҴٴ� ���� �ٸ� �� �˻��ߴ�
//					else {
//						// �ٸ� ���� ���� ������ ������ ������ ��´�.
//						iTmpScore -= 1;
//						break;
//					}
//				}
//			}
//			// �߰��� ���� �޶����� �ִٸ� �˻� �ߴܵǱ� ������
//			// ù��°, �ι�° ���� ����° ���� �ٸ��� ����.
//			else if (depth == 4) {
//				// ���� ����
//				// ���� ������ ������ �����ش�.
//				if (PreviousColor[2] == color) {
//					iTmpScore += 2;
//					if (PreviousColor[1] == color) {
//						iTmpScore += 3;
//					}
//					if (PreviousColor[0] == color) {
//						iTmpScore += 4;
//					}
//				}
//				// ���� �ٸ���
//				// �����ų� ����ų�
//				else {
//					if (PreviousColor[2] == Stone::Emptied) {
//						// �����̶� ������ ���Ѵ�.
//						if (PreviousColor[1] == color) {
//							iTmpScore += 2;
//						}
//						else {
//							// �ٸ����� ������ ������ ������ ��´�.
//							iTmpScore -= 1;
//						}
//					}
//					// �����ٸ� ������ ��´�.
//					else {
//						iTmpScore -= 1;
//					}
//				}
//			}
//		}
//	}
//}

// �ְ������� �Ǵ�
// iPointMaxScore = iTmpScore >= iPointMaxScore ? iTmpScore : iPointMaxScore;
// ��� ���� ������ �Ǵ�
