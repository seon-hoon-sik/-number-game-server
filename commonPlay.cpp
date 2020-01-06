#include<iostream>
#include<cstdlib>
#include<cmath>
#include"numberCounting.h"
#include<ctime>
using namespace std;

int* commonPlay::firstTargetAlgo(int* targets, int& targetSize) //������ ��ǥ�� ������ �˰��� ��ȯ
{
	int** resultNumTest;
	int maxIndex, max = 0, arrayCnt;
	int checkArr = targetSize; //arraySize=goalNumber //arrayNum=targets //testNum=goal
	//checkArr�� �����Ҵ������ÿ� ����ϱ� ���� ����
	goalNumber = targetSize;
	int i, j, temp;
	for (i = 0; i < goalNumber - 1; i++) { //���� �ڵ�
		for (j = i + 1; j < goalNumber; j++) {
			if (targets[i] > targets[j]) {
				temp = targets[i];
				targets[i] = targets[j];
				targets[j] = temp;
			}
		}
	}
	// ������ 6�� ���ڰ� ���� �迭 �̱�
	// 2���� �迭 ���� �Ҵ�
	resultNumTest = new int* [goalNumber];//ms
	for (i = 0; i < goalNumber; i++) {
		resultNumTest[i] = new int[goalNumber];
	}
	// -1 ���а� �ֱ�
	for (i = 0; i < goalNumber; i++) {
		for (j = 0; j < goalNumber; j++) {
			resultNumTest[i][j] = -1;
		}
	}
	maxIndex = 1;
	for (i = 0; i < goalNumber; i++) {
		for (j = 0; j < goalNumber; j++) {
			if (i == j) {
				resultNumTest[i][j] = targets[i];
			}
			else {
				if (abs(targets[i] - targets[j]) % 6 == 0) { resultNumTest[i][j] = targets[j]; }
			}
		}
	}
	for (i = 0; i < goalNumber; i++) {
		arrayCnt = 0;
		for (j = 0; j < goalNumber; j++) {
			if (resultNumTest[i][j] != -1) {
				arrayCnt++;
			}
		}
		if (maxIndex < arrayCnt) {
			if ((resultNumTest[i][i] % 6 != 0)) {//hs
				max = i;
				maxIndex = arrayCnt;
			}
		}
	}
	if (max == 0) {  //hs
		if (targets[0] % 6 == 0) {
			max++;
		}
	}
	if (chanceGoalNum == 0)// ���� �ִ� ���, ��ǥ�� ���� ���
	{
		goalNumber = maxIndex;//��ǥ�� �� ����
	}
	else if (chanceGoalNum < goalNumber) {
		goalNumber = chanceGoalNum;
	}
	goal = new int[goalNumber];
	arrayCnt = 0;
	for (j = 0; j < checkArr; j++) {
		if (resultNumTest[max][j] != -1) {
			goal[arrayCnt] = resultNumTest[max][j];
			arrayCnt++;
		}
	}
	targetSize = goalNumber;//call by reference�� ���� ������ߵ� 

	for (i = 0; i < checkArr; i++) {
		delete[] resultNumTest[i];
	}
	delete[] resultNumTest;
	return goal;
}

int* commonPlay::secondTargetAlgo(int& targetSize) //�ļ��� ��ǥ�� �ʱ� ���� �� ũ�� callbyvalue�� ����
{    //////////////////////algorithm
	if (chanceGoalNum == 0)// ���� �ִ� ���, ��ǥ�� ���� ���
	{
		targetSize = goalNumber = 3;//��ǥ�� �� ����
		goal = new int[3];
		goal[0] = 48;
		goal[1] = 60;
		goal[2] = 72;

	}
	else if (chanceGoalNum != 1)// ���� ���忡�� �̰ܾ��� �� ��ǥ�� ���� ���� ���
	{
		int i;
		goalNumber = chanceGoalNum;
		goal = new int[goalNumber];
		if (goalNumber < 6)
		{
			for (i = 0; i < goalNumber; i++)
			{
				goal[i] = 48 + 12 * i; // ������ ���� ���� ����
			}
		}
		else if (goalNumber > 6)
		{
			for (i = 0; i < 5; i++)
			{
				goal[i] = 48 + 12*i;
			}
			for (i = 5; i < goalNumber; i++)
			{
				goal[i] = 96 + (i - 4);
			}
		}
	}
	//goal[0] = 46;
	//goal[1] = 73;
	//goal[2] = 88;
	return goal;
}


int* commonPlay::testFirstTargetAlgo(int* targets, int& targetSize, int forTest) //testvector �� �˰���
{
	int i = 0;
	if (forTest == 2)//������ ��ǥ���� 8���� ������ �����Ƿ� ���� �� �״�� ��ȯ
	{
		goalNumber = targetSize;
		goal = new int[goalNumber];
		for (i = 0; i < goalNumber; i++)
		{
			goal[i] = targets[i];
		}
	}
	else if (forTest == 3)//��ǥ�� �ߺ� ��ȯ
	{
		goalNumber = targetSize;
		goal = new int[goalNumber];
		for (i = 0; i < goalNumber; i++)
		{
			goal[i] = targets[i];
		}
		goal[goalNumber - 2] = targets[goalNumber - 1];//�ߺ��� ����
	}
	else if (forTest == 4)//��ǥ���� ���� ���� �ٸ� ��Ȳ
	{
		goalNumber = rand() % targetSize + 1;
		goal = new int[goalNumber];
		for (i = 0; i < goalNumber; i++)
		{
			goal[i] = targets[i] + 1;
			cout << goal[i] << " ";//ms
		}
	}
	else 
	{
		int randTargetNum = rand() % targetSize + 1;//���� ������ ���� �������� ������ǥ�� ����
	//msmsms
		randTargetNum = targetSize;//msmsms
		int i = 0, j, tmp;
		int rand_idx = 0;//�ι� ������ ���� �Ǵ�
		goal = new int[randTargetNum];
		while (i < randTargetNum)
		{
			tmp = targets[(rand() % randTargetNum)];
			rand_idx = 0;
			if (i >= 1)
			{
				for (j = 0; j < i; j++)//���ݱ��� �� ��ǥ�� Ȯ�� �Ϸ��� j��°�϶� j������ ������ ���ؼ� ����
				{
					rand_idx = 0;
					if (tmp == goal[j])
					{
						rand_idx++;
					}
					if (rand_idx == 1)//1�̸� �ߺ����� ����, �ٽ� ������ ���ư��� �ߺ����� Ȯ��
					{
						i -= 1;
						j = i + 1;
					}
				}
				if (rand_idx == 0)//0�̸� �ߺ����� �����Ƿ� �����ϰ� �Ѿ
				{
					goal[i] = tmp;
				}
			}
			else { goal[0] = tmp; }
			i++;
		}
		//�Ƹ� �ߺ��� ���� ����� ���� �־ Ȯ���غ��� �ٶ�
		goalNumber = randTargetNum;
		targetSize = randTargetNum;
	}
	targetSize = goalNumber;//������ ����
	return goal;
}
int* commonPlay::testSecondTargetAlgo(int& targetSize, int forTest)
{
	int randTargetNum, i, j;
	if (forTest == 2)//��ǥ�� 8���̻� ������ ��Ȳ
	{
		randTargetNum = 8 + rand()%3; //8������ 10�� ���� ����
		goal = new int[randTargetNum];
		goalNumber = randTargetNum; //���� Ŭ������ ����
		// �������� ��ǥ�� ����(������ ���� ��), �ߺ��Ǵ� ���� ����
		for (i = 0; i < randTargetNum; i++) {
			goal[i] = rand() % 69 + 31;
			for (j = 0; j < i; j++) {
				if (goal[i] == goal[j]) {
					goal[i] = rand() % 69 + 31;
					i--;
					break;
				}
			}
		}
	}
	else if (forTest == 3)//��ǥ�� �ߺ� �۽�
	{
		randTargetNum = rand() % 6 + 2;
		goal = new int[randTargetNum];
		goalNumber = randTargetNum; //���� Ŭ������ ����
		goal[0] = 41;
		goal[1] = 41;
		for (i = 2; i < randTargetNum; i++)
		{
			goal[i] = rand() % 69 + 31;
			for (j = 0; j < i; j++) {
				if (goal[i] == goal[j]) {
					goal[i] = rand() % 69 + 31;
					i--;
					break;
				}
			}
		}
	}
	else
	{
		int randTargetNum = rand() % 7 + 1;
		goal = new int[randTargetNum];
		goalNumber = randTargetNum; //���� Ŭ������ ����
		// �������� ��ǥ�� ����(������ ���� ��), �ߺ��Ǵ� ���� ����
		for (int i = 0; i < randTargetNum; i++) {
			goal[i] = rand() % 69 + 31;
			for (int j = 0; j < i; j++) {
				if (goal[i] == goal[j]) {
					goal[i] = rand() % 69 + 31;
					i--;
					break;
				}
			}
		}
	}
	targetSize = goalNumber;// �ļ� Ŭ������ �� ����
	//don't touch default����
	return goal;
}
int* commonPlay::randFirstTargetAlgo(int* targets, int& targetSize)//���� �˰���
{
	int randTargetNum = rand() % targetSize + 1;//���� ������ ���� �������� ������ǥ�� ����
	//msmsms
	randTargetNum = targetSize;//msmsms
	int i = 0, j, tmp;
	int rand_idx=0;//�ι� ������ ���� �Ǵ�
	goal = new int[randTargetNum];
	while(i<randTargetNum)
	{
		tmp = targets[(rand() % randTargetNum)];
		rand_idx=0;
		if (i >= 1)
		{
			for (j = 0; j < i; j++)//���ݱ��� �� ��ǥ�� Ȯ�� �Ϸ��� j��°�϶� j������ ������ ���ؼ� ����
			{
				rand_idx = 0;
				if (tmp == goal[j])
				{
					rand_idx++;
				}
				if (rand_idx == 1)//1�̸� �ߺ����� ����, �ٽ� ������ ���ư��� �ߺ����� Ȯ��
				{
					i-=1;
					j = i + 1;
				}
			}
			if (rand_idx == 0)//0�̸� �ߺ����� �����Ƿ� �����ϰ� �Ѿ
			{
				goal[i] = tmp;
			}
		}
		else { goal[0] = tmp; }
		i++;
	}
	//�Ƹ� �ߺ��� ���� ����� ���� �־ Ȯ���غ��� �ٶ�
	goalNumber = randTargetNum;
	targetSize = randTargetNum;
	return goal;
}
int* commonPlay::randSecondTargetAlgo(int& targetSize)
{
	int randTargetNum = rand() % 7 + 1;
	//msmsms
	randTargetNum = 7;//msmsms
	goal = new int[randTargetNum];
	goalNumber = randTargetNum; //���� Ŭ������ ����
	// �������� ��ǥ�� ����(������ ���� ��), �ߺ��Ǵ� ���� ����
	for (int i = 0; i < randTargetNum; i++) {
		goal[i] = rand() % 69 + 31;
		for (int j = 0; j < i; j++) {
			if (goal[i] == goal[j]) {
				goal[i] = rand() % 69 + 31;
				i--;
				break;
			}
		}
	}
	targetSize = randTargetNum;// �ļ� Ŭ������ �� ����
	return goal;
}

int commonPlay::calcNumFirst(int rxCond, int rx)//������ �˰���
{
	rxcondNum = rxCond;
	if (turnNum == 1)
	{
		txcondNum = (goal[0]) % 6;//goal[0]+1 ���� ��ȭ��
		if (txcondNum == 0) { txcondNum = 1; }
		hintNum = txcondNum;
		turnNum++;
	}
	else
	{
		txcondNum = rxcondNum;
		if (rxcondNum == 1) {
			if ((rx + 1 - hintNum) % 6 == 0) { txcondNum = 1; } // 1�� ���ؼ� ������ 6���� Ȯ��
			else if ((rx + 2 - hintNum) % 6 == 0) { txcondNum = 2; } // 2�� ���ؼ� ������ 6���� Ȯ��
			else { txcondNum = 1; }
		}
		else if (rxcondNum == 5) {
			if ((rx + 4 - hintNum) % 6 == 0) { txcondNum = 4; } // 4�� ���ؼ� ������ 6���� Ȯ��
			else if ((rx + 5 - hintNum) % 6 == 0) { txcondNum = 5; } // 5�� ���ؼ� ������ 6���� Ȯ��
			else { txcondNum = 4; }
		}
		else if (rxcondNum >= 2 && rxcondNum <= 4) {
			if ((rx + rxcondNum - hintNum) % 6 == 0) { ; }
			else if ((rx + rxcondNum - 1 - hintNum) % 6 == 0) { txcondNum -= 1; } // plusNum - 1 ���� ���ؼ� ������ 6���� Ȯ��
			else if ((rx + rxcondNum + 1 - hintNum) % 6 == 0) { txcondNum += 1; } // plusNum + 1 ���� ���ؼ� ������ 6���� Ȯ��
			else { txcondNum -= 1; } // ������ 6�� �� �� ���ٸ� �ּҰ����� ����
		}
	}
	if (rx + txcondNum > 99) { txcondNum = 99 - rx; }
	return txcondNum;
}
int commonPlay::calcNumSecond(int rxCond, int rx)//�ļ��� �˰���
{
	rxcondNum = rxCond;
	txcondNum = rxcondNum;
	if (rxcondNum == 1) {
		if ((rx + 1) % 6 == 0) { txcondNum = 1; } // 1�� ���ؼ� ������ 6���� Ȯ��
		else if ((rx + 2) % 6 == 0) { txcondNum = 2; } // 2�� ���ؼ� ������ 6���� Ȯ��
		else { txcondNum = 1; }
	}
	else if (rxcondNum == 5) {
		if ((rx + 4) % 6 == 0) { txcondNum = 4; } // 4�� ���ؼ� ������ 6���� Ȯ��
		else if ((rx + 5) % 6 == 0) { txcondNum = 5; } // 5�� ���ؼ� ������ 6���� Ȯ��
		else { txcondNum = 4; }
	}
	else if (rxcondNum >= 2 && rxcondNum <= 4) {
		if ((rx + rxcondNum) % 6 == 0) { ; }
		else if ((rx + rxcondNum - 1) % 6 == 0) { txcondNum -= 1; } // plusNum - 1 ���� ���ؼ� ������ 6���� Ȯ��
		else if ((rx + rxcondNum + 1) % 6 == 0) { txcondNum += 1; } // plusNum + 1 ���� ���ؼ� ������ 6���� Ȯ��
		else { txcondNum -= 1; } // ������ 6�� �� �� ���ٸ� �ּҰ����� ����
	}
	if (rx + txcondNum > 99) { txcondNum = 99 - rx; }
	return txcondNum;
}
int commonPlay::calcTest(int rxCond, int rx, int forTest,int myScore, int yourScore)
{
	int useChance = 0;
	//testvector
	rxcondNum = rxCond;
	if (rxcondNum == 0)
	{
		txcondNum = rand() % 5 + 1;
	}
	else if (rxcondNum != 0)
	{
		if (rxcondNum == 5)
		{
			txcondNum = rxcondNum - (rand() % 2); // 4,5 ��ȯ
		}
		else if (rxcondNum == 1)
		{
			txcondNum = rxcondNum + (rand() % 2); // 1,2 ��ȯ
		}
		else
		{
			txcondNum = rxcondNum + (rand() % 3) - 1; //+-1�� 0�߿� ���ؼ� ��ȯ
		}
		if (rx + txcondNum > 99) { txcondNum = 99 - rx; }
	}
	if ((useChance < 2)&&(forTest>=5))
	{
		if ((myScore < yourScore) && (forTest == 5))//���� 2���̻� ����Ȳ
		{
			if ((txcondNum < 4))
			{
				txcondNum = 5;
			}
			else if ((txcondNum > 2))
			{
				txcondNum = 1;
			}
			useChance++;
		}
		else if ((myScore >= yourScore) && (forTest == 6))//������ ���µ� ���
		{
			if ((txcondNum<4))
			{
				txcondNum = 5;
			}
			else if ((txcondNum>2))
			{
				txcondNum = 1;
			}
			useChance += 2;//2�����ɷ� �ν��ؼ� ���� �ѹ��� ���� ��
		}
	}
	return txcondNum;
}
int commonPlay::calcRandom(int rxCond, int rx)//������ �˰���
{
	rxcondNum = rxCond;
	if (rxcondNum == 0)
	{
		txcondNum = rand() % 5 + 1;
	}
	else if (rxcondNum != 0)
	{
		if (rxcondNum == 5)
		{
			txcondNum = rxcondNum - (rand() % 2); // 4,5 ��ȯ
		}
		else if (rxcondNum == 1)
		{
			txcondNum = rxcondNum + (rand() % 2); // 1,2 ��ȯ
		}
		else
		{
			txcondNum = rxcondNum + (rand() % 3) - 1; //+-1�� 0�߿� ���ؼ� ��ȯ
		}
	}
	return txcondNum;
}
// goal : ���� ��ǥ��
// rxNum : ���� ����� ��
// rxcondNum : ������� ���Ѱ�
// myFlag�� 1�̸� ������ �� �� �ִٴ� �ǹ�
// ��ȯ ���� �츮�� ���� ���� �� ����
int commonPlay::chanceFunction(int goal, int rxNum, int rxcondNum,int myFlag) {
	// �� Branch�� �����ϸ� �츮���� ������ �� �� ����
	// ���� �˰��� ����
	if (myFlag == 1) {
		if ((goal - rxNum) <= 5) {
			// ������ ���� �ִ� ��Ȳ
			// �츮�� ���������� ���� ��
			return goal - rxNum; // ���� Ŭ������ ��ĥ�״ϱ� ������ ��� �Ƚ�� ��ȣ�� ���൵ �� ���ؼ� �ȳ־���
		}
		else if ((goal - rxNum) > 5) {
			// ������ ������ ��Ȳ
			if (rxcondNum == 1) { return 1; }
			else if (rxcondNum == 5) { return 4; }
			else if (rxcondNum > 1 && rxcondNum < 5) { return rxcondNum - 1; }
		}
	}
	//�� Branch�� �����ϸ� ������ �� �� ����
	else if (myFlag != 1) {
		if ((goal - rxNum) <= 5) {
			return 0; // ��ȯ���� 0�̶�� ���� �˰��� �����ϸ� ��
		}
	}
	return 0;
}
// �ʱ� ������ �� ���� �Լ�
// ��ȯ �� -> 0�̸� �˰����� ��ǥ�� ���� ���ϱ�
//         -> 0�� �ƴϸ� �� �Լ����� ��ȯ�ϴ� ���� ��ǥ�� ������
void commonPlay::initialChanceFunction(int myScore, int yourScore) {
	int difScore;
	difScore = myScore - yourScore;
	// �츮���� ���� �ִ� ���
	/*�� ������ ���� ���� + 1��ŭ ��ȯ<�����ؾ���~~~> */
	if (difScore < 0) {
		chanceGoalNum = -difScore + 1;//ms
		if (chanceGoalNum == 8)//8������ ��¿���� �ƽľ�
		{
			chanceGoalNum = 7;
		}
	}
	// ������� ���� �ִ� ���
	/*�� ��ǥ�� ������ ������ 1����<��������~~~~~>*/
	else if (difScore > 0) {
		chanceGoalNum= 1;//ms
	}
	chanceGoalNum= 0;//���� �ִ� ���
}