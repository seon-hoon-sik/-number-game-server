#include"numberCounting.h"
#include<iostream>
#include<cstdlib>
using namespace std;
batFirst::batFirst(int forTest, int myScore, int yourScore)
{
	testVector = forTest;
	this->ourScore = myScore;
	this->theirScore = yourScore;
	firstReferee = new referee(1);
}
batFirst::~batFirst()
{
	delete firstReferee;
	delete[] score;
}
void batFirst::getTarget(int target[], const int targetSize)
{
	int i;
	goalNum = targetSize; // ��ȿ�� �Ǵ��� ����
	goal = new int[goalNum];
	for (i = 0; i < goalNum; i++)
	{
		goal[i] = target[i];
	}
	firstReferee->gameCheck(ourScore, theirScore, goal, goalNum, rxcondNum, rxNum, txcondNum, txNum);
}
int* batFirst::setTarget() //��ǥ�� �迭�� ������ ���� �ʼ�
{
	first.initialChanceFunction(ourScore, theirScore);
	int i, j;
	if (testVector == 0)
	{
		goal = first.firstTargetAlgo(goal, goalNum); ///ptr�̶� �迭 ũ�� ���� 
	}
	else if (testVector == 1)
	{
		goal = first.randFirstTargetAlgo(goal, goalNum);
	}
	else
	{
		goal = first.testFirstTargetAlgo(goal, goalNum, testVector);
	}
	/////////////////////////////// target �Ÿ��� �˰���///////////////////////

	//�迭ũ���� goalNum�� callbyreference�� ����Ǿ���
	// member variable�� goalNum�� ���ڵ� ����
	int tmp;//���� ���Ŀ� �ӽ� ����

	//������� ����
	for (i = 0; i < goalNum - 1; i++)
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
	return goal;
}
int batFirst::tx()
{
	ourFlag = firstReferee->returnFlag()[0];
	//���� ���ڿ� ���� �˰��� ��ġ
	if (testVector == 0)
	{
		txcondNum = first.chanceFunction(goal[idx_goal], rxNum, rxcondNum, ourFlag);

		if (txcondNum == 0) { txcondNum = first.calcNumFirst(rxcondNum, rxNum); }
	}
	else if (testVector == 1)
	{
		txcondNum = first.chanceFunction(goal[idx_goal], rxNum, rxcondNum, ourFlag);
		if (txcondNum == 0) { txcondNum = first.calcRandom(rxcondNum, rxNum); }
	}
	else
	{
		txcondNum = first.calcTest(rxcondNum, rxNum, testVector, ourScore, theirScore);
	}
	//���ο� txNum�� ���� ���� �ʿ�
	txNum = rxNum + txcondNum;
	saveScore();//���� ���� ����
	firstReferee->gameCheck(ourScore, theirScore, goal, goalNum, rxcondNum, rxNum, txcondNum, txNum);
	return txNum;
}
void batFirst::rx(int receiveNum)
{
	rxcondNum = receiveNum - txNum;
	if (rxcondNum <= 5 && rxcondNum >= 1) // �ּ����� Ȯ�� Ȯ��
	{
		rxNum = receiveNum;
	}
	saveScore();//���� ��ȭ�ִ��� Ȯ��
	firstReferee->gameCheck(ourScore, theirScore, goal, goalNum, rxcondNum, rxNum, txcondNum, txNum);
}
void batFirst::saveScore()
{
	for (int i = idx_goal; i < goalNum; i++)//���ÿ� �Ѿ��쵵 ����
	{
		if ((txNum >= goal[idx_goal]) && (txNum > rxNum))//�������� ����
		{
			ourScore += 1;
			idx_goal += 1;
		}
		else if ((rxNum >= goal[idx_goal]) && (rxNum > txNum))//�޾����� ����
		{
			theirScore += 1;
			idx_goal += 1;
		}
	}
}
int batFirst::getGoalNum()
{
	return goalNum;
}
int* batFirst::returnScore()
{
	score = new int[2];
	score[0] = ourScore;
	score[1] = theirScore;
	return score;
}
void batFirst::clearScore()
{
	delete[]score;
	ourScore = 0;
	theirScore = 0;//ms�ݺ����������� �ʱ�ȭ
}