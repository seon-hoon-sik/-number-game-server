#include<iostream>
#include"numberCounting.h"
#include<cmath>
using namespace std;

// ������
referee::referee(int judgeWho) {
	firstCheck = judgeWho;
}
referee::~referee()
{
	delete[] arrayCheck;
	delete rFlag;
}

void referee::yourChanceCheck(int myScore, int yourScore) {
	if (myScore == 0 && yourScore == 0) {
		yourFlag = 0;
	}
	if (myScore == yourScore) {
		// ������ ���� ��� �ϵ� ���� �ʴ´�.
	}
	else if (myScore > yourScore) {
		if (yourUseChance == 0) { yourFlag = 1; }
	}
	if (yourUseChance == 1) { yourFlag = 2; };
}

/*�� �ʱ� �츮�� ���� myFlag = 0
	�� �츮���� ���� ���� �� �츮���� ���� ��� myFlag = 1
	�� �츮���� ������ ��ٸ� myFlag = 2*/
void referee::myChanceCheck(int myScore, int yourScore) {
	if (myScore == 0 && yourScore == 0) {
		myFlag = 0;
	}
	if (myScore == yourScore) {
		// ������ ���� ��� �ϵ� ���� �ʴ´�.
	}
	else if (myScore < yourScore) {
		if (myUseChance == 0) { myFlag = 1; }
	}
	if (myUseChance == 1) { myFlag = 2; }

}

// ����
   /*�� �ʱ� ������ 0:0�̸� flag = 0
	�� �ʱ⼼���� ������ �ߴٸ� flag = 1�� ����*/
	// ����
	  /*�� ������� ���� ��ǥ ���� 31 ~ 99���� Ȯ�� ����
		�� ������� ���� ��ǥ �� ������ 1 ~ 7������ Ȯ�� ����
		�� ������� ó���� ���� ���� 1 ~ 5������ Ȯ�� ����
		�� �츮���� ������� firstCheck = 1, �츮���� �ļ���� firstCheck = 0*/
		// �ļ�
		  /*�� ������� ���� ��ǥ ���� �츮�� ���� ��ǥ���� ���ԵǴ��� Ȯ��
			�� ������� �ʱ� ������ ���� 1 ~ 5������ Ȯ��*/
void referee::gameCheck(int& myScore, int& yourScore, int goal[], int& goalNum, int& rxcondNum, int& rxNum, int& txcondNum, int& txNum) {
	// �ʱ� �� ����
	int i, j;
	if (flag == 0) {
		// ���� �ʱ� �� ����
		if (firstCheck) {
			if (goalNum < 1 || goalNum>7) { cout << "������� ���� ��ǥ�� ���� ����(1~7��)�� �ƴմϴ�." << endl; }//��ǥ�� ó���� ó�� �Ұ���, �ߺ� ��ã��
			for (i = 0; i < goalNum; i++) {
				if (goal[i] < 31 || goal[i]>99) { cout << "������� ���� ��ǥ�� ����(31~99)�� �ƴմϴ�." << endl; }
			}
			if (settingNum == 2) {
				if (rxcondNum < 1 || rxcondNum>5) {
					cout << "������� ó���� ���� ���ڰ� ����(1~5)�� ������ϴ�." << endl;
				}
				flag++;
			}
			settingNum++;
		}
		// �ļ� �ʱ� �� ����
		else if (!firstCheck) {
			//0��°
			if (secondCheck == 0) {
				arrayCheck = new int[goalNum];
				arrayCheckSize = goalNum;
				for (i = 0; i < arrayCheckSize; i++) { //
					arrayCheck[i] = goal[i];//
					cout << arrayCheck[i] << " ";
				} //
				secondCheck++;
			}
			//1��°
			else if (secondCheck == 1) {
				if (goalNum < 0 || goalNum>7) { cout << "������� ���� ��ǥ�� ���� ����(1~7��)�� �ƴմϴ�." << endl; }
				if (settingNum == 0)
				{
					for (i = 0; i < goalNum; i++) {
						for (j = 0; j < arrayCheckSize; j++) {
							if (goal[i] == arrayCheck[j]) { numCheck++; }
						}
					}
				}
				if (numCheck != goalNum) {
					cout << "������� ���� ���� �츮���� ���� ���� �ٸ��ϴ�." << endl;
					numCheck = goalNum;//2��°�� ��üũ ������
				}
				//2��°
				if (settingNum == 1) {
					if (rxNum < 1 || rxNum>5) {
						cout << "������� �ʱ⿡ ������ ���� ����(1~5)�� ������ϴ�." << endl;
					}
					flag++;
				}
				settingNum++;
			}
		}
	}
	// �ʱ� �� ���� �� �۾�
	else if (flag == 1) {
		yourChanceCheck(myScore, yourScore);
		myChanceCheck(myScore, yourScore);

		//////////////////////////////////////////////// ������� ��Ģ�� �������� ���� ����/////////////////////////////////////////////
		if (yourFlag == 0) {
			if ((abs(rxcondNum - txcondNum) > 1) && ((rxNum - txNum) > 0)) {//��밡 ���� ���� �� Ŭ�� = ����������
				cout << "������� ��Ģ�� �����ϴ�.(�츮���� ���Ѱ����� ���� 2�̻� ���̰� ��)" << endl;
			}
			if (rxcondNum <= 0 || rxcondNum >= 6) { cout << "������� ��Ģ�� �����ϴ�. 0���� 6�̻��� ���� ���½��ϴ�." << endl; } // �߰�
		}
		// ������� ������ �� �� �ִٴ� �ǹ�
		else if (yourFlag == 1) {
			if ((abs(rxcondNum - txcondNum) > 1) && ((rxNum - txNum) > 0)) {
				yourUseChance++;
				cout << "��밡 ������ ����Ͽ����ϴ�." << endl;
			}
		}
		// ������� ������ ��ٴ� �ǹ�
		else if (yourFlag == 2) {//����������
			if ((abs(rxcondNum - txcondNum) > 1) && ((rxNum - txNum) > 0)) {
				cout << "������� ��Ģ�� �����ϴ�.(������ ����ߴµ� �츮���� ���Ѱ����� ���� 2�̻� ���̰� ��)" << endl;
				cout << rxNum << " " << rxcondNum << " " << txcondNum << endl;//ms
			}
			if (rxcondNum <= 0 || rxcondNum >= 6) { cout << "������� ��Ģ�� �����ϴ�. 0���� 6�̻��� ���� ���½��ϴ�." << endl; } // �߰�
		}

		//////////////////////////////////////////////// �츮���� ���� üũ ���� /////////////////////////////////////////////
		if (myFlag == 0) {
			;
		}
		else if (myFlag == 1) {
			if (abs(rxcondNum - txcondNum) > 1) {
				cout << "������ ����ϴ�." << endl;
				myUseChance++;
			}
		}
		else if (myFlag == 2) {
			;
		}
	}
}
int* referee::returnFlag()
{
	rFlag = new int[2];
	rFlag[0] = myFlag;
	rFlag[1] = yourFlag;
	return rFlag;
}