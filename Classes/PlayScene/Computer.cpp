#define DEBUG_MODE 0;
#define MAXVAL(i, j) i = j + 2 < 18 ? j + 2 : 18
#define MINVAL(i, j) i = j - 2 > 0 ? j - 2 : 0

#include "Computer.h"
#include "StoneLayer.h"
#include <time.h>

using namespace cocos2d;
using namespace std;

CComputer::CComputer() : count1(0) {
	srand((unsigned)time(NULL));
}

CComputer::~CComputer() {}

Vec2 CComputer::computerTurn(Node* node) const {
	CStoneLayer* stonelayer = dynamic_cast<CStoneLayer*>(node);
	// ��ü �ٵ��� 19*19�� ���ؼ� ������ ����ϰ�
	// ���� ������ ���� �ڸ��� ������ �Ѵ�.
	int maxX = MAXVAL(maxX, stonelayer->min_max_xy_position[CStoneLayer::Min_MaxXY::max_x]);
	int minX = MINVAL(minX, stonelayer->min_max_xy_position[CStoneLayer::Min_MaxXY::min_x]);
	int maxY = MAXVAL(maxY, stonelayer->min_max_xy_position[CStoneLayer::Min_MaxXY::max_y]);
	int minY = MINVAL(minY, stonelayer->min_max_xy_position[CStoneLayer::Min_MaxXY::min_y]);

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

void CComputer::calculationScore(Node* node) {
	CStoneLayer* stone_layer = dynamic_cast<CStoneLayer*>(node);
	
	int max_x = MAXVAL(max_x, stone_layer->min_max_xy_position[CStoneLayer::Min_MaxXY::max_x]);
	int min_x = MINVAL(min_x, stone_layer->min_max_xy_position[CStoneLayer::Min_MaxXY::min_x]);
	int max_y = MAXVAL(max_y, stone_layer->min_max_xy_position[CStoneLayer::Min_MaxXY::max_y]);
	int min_y = MINVAL(min_y, stone_layer->min_max_xy_position[CStoneLayer::Min_MaxXY::min_y]);

	
	int point_max_score = 0;
	CStoneSprite* sprite;
	for (int y = min_y; y <= max_y; ++y) {
		for (int x = min_x; x <= max_x; ++x) {
			sprite = stone_layer->getSprite(x, y);
			if (sprite->getActive()) {
				point_max_score = calculationPointMaxScore(stone_layer, x, y);
				sprite->setScore(point_max_score);
#ifdef DEBUG_MODE == 1
				vector<Label*> labelvector = stone_layer->labelvec;
				Label* label = labelvector.at(19 * y + x);
				char str[4];
				sprintf_s(str, sizeof(str), "%d", point_max_score);
				label->setString(str);
				label->setColor(Color3B(250, 50, 50));
				label->setVisible(true);
				++count1;
#endif
			}
		}
	}
#ifdef DEBUG_MODE == 1
	CCLOG("ComputerTurn count1 : %d", count1);
#endif
}

int CComputer::calculationPointMaxScore(CStoneLayer* stone_layer, int x, int y) {
	CStoneSprite* sprite;
	int max_score[2] = { 0, };
	int score[2] = { 0, };
	Stone previous_color[4] = { Stone::emptied, };
	for (int dir = 0; dir < CStoneLayer::Direction::dir_count; ++dir) {
		for (int depth = 1; depth <= 4; ++depth) {
			sprite = stone_layer->searchAroundSprite(x, y, dir, depth);
			if (sprite == nullptr) break;
			Stone color = sprite->getStoneType();
			previous_color[depth - 1] = color;

			// ���� ����.
			if (color == Stone::emptied) {
				if (depth != 1) {
					// 2�� ���� ����°�?
					if (color == previous_color[depth - 2]) break;
					score[previous_color[depth - 2]] += (depth - 2);
				}
			}
			// ���� �ִ�. 1~4
			else if (depth == 1) {
				score[color] += 1;
			}
			else {
				if (previous_color[depth - 1] == previous_color[depth - 2]) {
					score[color] += (depth*1.8f);
				}
				else if (depth > 2 && previous_color[depth - 1] == previous_color[depth - 3]) {
					score[color] += 1;
				}
				else if (previous_color[depth - 1] != Stone::emptied) {
					score[color] -= depth;
					break;
				}
			}
		}
		max_score[Stone::black] += score[Stone::black];
		max_score[Stone::white] += score[Stone::white];
		for (int i = 0; i < 4; ++i) {
			previous_color[i] = Stone::emptied;
		}
		score[Stone::black] = 0;
		score[Stone::white] = 0;
	}
	return (max_score[Stone::black] > max_score[Stone::white] ? max_score[Stone::black] : max_score[Stone::white]);
}

// ���� ��� �ڵ� �ּ�ó��
// ���� ��� �ڵ� �ּ�ó��
// ���� ��� �ڵ� �ּ�ó��
// ���� ��� �ڵ� �ּ�ó��

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
