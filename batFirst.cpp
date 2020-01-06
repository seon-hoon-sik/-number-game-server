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
	goalNum = targetSize; // 유효성 판단후 저장
	goal = new int[goalNum];
	for (i = 0; i < goalNum; i++)
	{
		goal[i] = target[i];
	}
	firstReferee->gameCheck(ourScore, theirScore, goal, goalNum, rxcondNum, rxNum, txcondNum, txNum);
}
int* batFirst::setTarget() //목표값 배열과 사이즈 지정 필수
{
	first.initialChanceFunction(ourScore, theirScore);
	int i, j;
	if (testVector == 0)
	{
		goal = first.firstTargetAlgo(goal, goalNum); ///ptr이랑 배열 크기 연결 
	}
	else if (testVector == 1)
	{
		goal = first.randFirstTargetAlgo(goal, goalNum);
	}
	else
	{
		goal = first.testFirstTargetAlgo(goal, goalNum, testVector);
	}
	/////////////////////////////// target 거르는 알고리즘///////////////////////

	//배열크기인 goalNum은 callbyreference로 연결되었음
	// member variable인 goalNum의 숫자도 수정
	int tmp;//순서 정렬용 임시 변수

	//순서대로 정렬
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
	//보낼 숫자에 대한 알고리즘 위치
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
	//새로운 txNum에 대한 지정 필요
	txNum = rxNum + txcondNum;
	saveScore();//변한 점수 저장
	firstReferee->gameCheck(ourScore, theirScore, goal, goalNum, rxcondNum, rxNum, txcondNum, txNum);
	return txNum;
}
void batFirst::rx(int receiveNum)
{
	rxcondNum = receiveNum - txNum;
	if (rxcondNum <= 5 && rxcondNum >= 1) // 최소조건 확립 확인
	{
		rxNum = receiveNum;
	}
	saveScore();//점수 변화있는지 확인
	firstReferee->gameCheck(ourScore, theirScore, goal, goalNum, rxcondNum, rxNum, txcondNum, txNum);
}
void batFirst::saveScore()
{
	for (int i = idx_goal; i < goalNum; i++)//동시에 넘어간경우도 적용
	{
		if ((txNum >= goal[idx_goal]) && (txNum > rxNum))//보냈을때 저장
		{
			ourScore += 1;
			idx_goal += 1;
		}
		else if ((rxNum >= goal[idx_goal]) && (rxNum > txNum))//받았을때 저장
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
	theirScore = 0;//ms반복돌리기위한 초기화
}