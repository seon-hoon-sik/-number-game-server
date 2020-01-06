#include<iostream>
#include"numberCounting.h"
using namespace std;

batSecond::batSecond(int forTest, int myScore, int yourScore) //�����Ǵ� ������
{
	testVector = forTest;
	this->ourScore = myScore;
	this->theirScore = yourScore;
	secondReferee = new referee(0);//0�� �ļ�
}
batSecond::~batSecond()
{
	delete secondReferee;
	delete[] score;
}
int* batSecond::setTarget() {
	if (testVector == 0)
	{
		second.initialChanceFunction(ourScore, theirScore);
		goal = second.secondTargetAlgo(goalNum); ///ptr�̶� �迭 ũ�� ���� 
	}
	else if (testVector == 1)
	{
		goal = second.randSecondTargetAlgo(goalNum);
	}
	else
	{
		goal = second.testSecondTargetAlgo(goalNum, testVector);
	}
	secondReferee->gameCheck(ourScore, theirScore, goal, goalNum, rxcondNum, rxNum, txcondNum, txNum);
	return goal;
}

void batSecond::getTarget(int target[], const int targetSize)
{
	int i, j;
	int tmp;
	goalNum = targetSize;
	goal = new int[targetSize]; // �´��� Ȯ�������ϱ� ����
	goal = target;
	for (i = 0; i < goalNum - 1; i++)//��ǥ�� ���� �ڵ�
	{
		for (j = i + 1; j < goalNum; j++)
		{
			if (goal[i] > goal[j])
			{
				tmp = goal[j];
				goal[j] = goal[i];
				goal[i] = tmp;
			}
		}
	}
	secondReferee->gameCheck(ourScore, theirScore, goal, goalNum, rxcondNum, rxNum, txcondNum, txNum);
}

int batSecond::tx()
{
	ourFlag = secondReferee->returnFlag()[0];
	//���� ���ڿ� ���� �˰��� ��ġ
	if (testVector == 0)
	{
		txcondNum = second.chanceFunction(goal[idx_goal], rxNum, rxcondNum, ourFlag);
		if (txcondNum == 0) { txcondNum = second.calcNumFirst(rxcondNum, rxNum); }
	}
	else if (testVector == 1)
	{
		txcondNum = second.chanceFunction(goal[idx_goal], rxNum, rxcondNum, ourFlag);
		if (txcondNum == 0) { txcondNum = second.calcRandom(rxcondNum, rxNum); }
	}
	else
	{
		txcondNum = second.calcTest(rxcondNum, rxNum, testVector, ourScore, theirScore);
	}
	//���ο� txNum�� ���� ���� �ʿ�
	txNum = txcondNum + rxNum;// +-1�� ���� ���� ����
	saveScore();//���� ���� ����
	secondReferee->gameCheck(ourScore, theirScore, goal, goalNum, rxcondNum, rxNum, txcondNum, txNum);

	return txNum;
}

void batSecond::rx(int receiveNum)
{
	rxcondNum = receiveNum - txNum;
	rxNum = receiveNum;
	saveScore();//���� ���� ����
	secondReferee->gameCheck(ourScore, theirScore, goal, goalNum, rxcondNum, rxNum, txcondNum, txNum);

}

void batSecond::saveScore()
{
	int i;
	for (i = idx_goal; i < goalNum; i++)//���ÿ� �Ѿ��쵵 ����
	{
		if (txNum >= goal[idx_goal])
		{
			ourScore += 1;
			idx_goal += 1;
		}
		else if (rxNum >= goal[idx_goal])
		{
			theirScore += 1;
			idx_goal += 1;
		}
	}
}

int batSecond::getGoalNum()
{
	return goalNum;
}
int* batSecond::returnScore()
{
	score = new int[2];
	score[0] = ourScore;
	score[1] = theirScore;
	return score;
}
void batSecond::clearScore()
{
	delete[]score;
	ourScore = 0;
	theirScore = 0;//ms�ݺ����������� �ʱ�ȭ
}