#include<iostream>
#include<cstdlib>
#include<cmath>
#include"numberCounting.h"
#include<ctime>
using namespace std;

int* commonPlay::firstTargetAlgo(int* targets, int& targetSize) //선수의 목표값 수령후 알고리즘 반환
{
	int** resultNumTest;
	int maxIndex, max = 0, arrayCnt;
	int checkArr = targetSize; //arraySize=goalNumber //arrayNum=targets //testNum=goal
	//checkArr은 동적할당해제시에 사용하기 위한 것임
	goalNumber = targetSize;
	int i, j, temp;
	for (i = 0; i < goalNumber - 1; i++) { //정렬 코드
		for (j = i + 1; j < goalNumber; j++) {
			if (targets[i] > targets[j]) {
				temp = targets[i];
				targets[i] = targets[j];
				targets[j] = temp;
			}
		}
	}
	// 공차가 6인 숫자가 많은 배열 뽑기
	// 2차원 배열 동적 할당
	resultNumTest = new int* [goalNumber];//ms
	for (i = 0; i < goalNumber; i++) {
		resultNumTest[i] = new int[goalNumber];
	}
	// -1 구분값 넣기
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
	if (chanceGoalNum == 0)// 비기고 있는 경우, 목표값 설정 방식
	{
		goalNumber = maxIndex;//목표값 수 저장
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
	targetSize = goalNumber;//call by reference도 변경 시켜줘야됨 

	for (i = 0; i < checkArr; i++) {
		delete[] resultNumTest[i];
	}
	delete[] resultNumTest;
	return goal;
}

int* commonPlay::secondTargetAlgo(int& targetSize) //후수의 목표값 초기 설정 및 크기 callbyvalue로 저장
{    //////////////////////algorithm
	if (chanceGoalNum == 0)// 비기고 있는 경우, 목표값 설정 방식
	{
		targetSize = goalNumber = 3;//목표값 수 저장
		goal = new int[3];
		goal[0] = 48;
		goal[1] = 60;
		goal[2] = 72;

	}
	else if (chanceGoalNum != 1)// 다음 라운드에서 이겨야할 때 목표값 갯수 설정 방식
	{
		int i;
		goalNumber = chanceGoalNum;
		goal = new int[goalNumber];
		if (goalNumber < 6)
		{
			for (i = 0; i < goalNumber; i++)
			{
				goal[i] = 48 + 12 * i; // 갯수에 따라 차등 조정
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


int* commonPlay::testFirstTargetAlgo(int* targets, int& targetSize, int forTest) //testvector 용 알고리즘
{
	int i = 0;
	if (forTest == 2)//선수가 목표값을 8개를 보내지 않으므로 받은 값 그대로 반환
	{
		goalNumber = targetSize;
		goal = new int[goalNumber];
		for (i = 0; i < goalNumber; i++)
		{
			goal[i] = targets[i];
		}
	}
	else if (forTest == 3)//목표값 중복 반환
	{
		goalNumber = targetSize;
		goal = new int[goalNumber];
		for (i = 0; i < goalNumber; i++)
		{
			goal[i] = targets[i];
		}
		goal[goalNumber - 2] = targets[goalNumber - 1];//중복값 저장
	}
	else if (forTest == 4)//목표값이 보낸 값과 다른 상황
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
		int randTargetNum = rand() % targetSize + 1;//받은 갯수와 나눈 나머지를 랜덤목표값 갯수
	//msmsms
		randTargetNum = targetSize;//msmsms
		int i = 0, j, tmp;
		int rand_idx = 0;//두번 같은지 여부 판단
		goal = new int[randTargetNum];
		while (i < randTargetNum)
		{
			tmp = targets[(rand() % randTargetNum)];
			rand_idx = 0;
			if (i >= 1)
			{
				for (j = 0; j < i; j++)//지금까지 들어간 목표값 확인 하려면 j번째일때 j이하의 모든수에 대해서 점검
				{
					rand_idx = 0;
					if (tmp == goal[j])
					{
						rand_idx++;
					}
					if (rand_idx == 1)//1이면 중복값이 존재, 다시 전으로 돌아가서 중복여부 확인
					{
						i -= 1;
						j = i + 1;
					}
				}
				if (rand_idx == 0)//0이면 중복값이 없으므로 저장하고 넘어감
				{
					goal[i] = tmp;
				}
			}
			else { goal[0] = tmp; }
			i++;
		}
		//아마 중복수 나올 경우의 수도 있어서 확인해보길 바람
		goalNumber = randTargetNum;
		targetSize = randTargetNum;
	}
	targetSize = goalNumber;//무조건 지정
	return goal;
}
int* commonPlay::testSecondTargetAlgo(int& targetSize, int forTest)
{
	int randTargetNum, i, j;
	if (forTest == 2)//목표값 8개이상 보내는 상황
	{
		randTargetNum = 8 + rand()%3; //8개에서 10개 사이 보냄
		goal = new int[randTargetNum];
		goalNumber = randTargetNum; //공통 클래스에 저장
		// 랜덤으로 목표값 설정(상대방이 보낼 값), 중복되는 숫자 제거
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
	else if (forTest == 3)//목표값 중복 송신
	{
		randTargetNum = rand() % 6 + 2;
		goal = new int[randTargetNum];
		goalNumber = randTargetNum; //공통 클래스에 저장
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
		goalNumber = randTargetNum; //공통 클래스에 저장
		// 랜덤으로 목표값 설정(상대방이 보낼 값), 중복되는 숫자 제거
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
	targetSize = goalNumber;// 후수 클래스에 값 저장
	//don't touch default실행
	return goal;
}
int* commonPlay::randFirstTargetAlgo(int* targets, int& targetSize)//랜덤 알고리즘
{
	int randTargetNum = rand() % targetSize + 1;//받은 갯수와 나눈 나머지를 랜덤목표값 갯수
	//msmsms
	randTargetNum = targetSize;//msmsms
	int i = 0, j, tmp;
	int rand_idx=0;//두번 같은지 여부 판단
	goal = new int[randTargetNum];
	while(i<randTargetNum)
	{
		tmp = targets[(rand() % randTargetNum)];
		rand_idx=0;
		if (i >= 1)
		{
			for (j = 0; j < i; j++)//지금까지 들어간 목표값 확인 하려면 j번째일때 j이하의 모든수에 대해서 점검
			{
				rand_idx = 0;
				if (tmp == goal[j])
				{
					rand_idx++;
				}
				if (rand_idx == 1)//1이면 중복값이 존재, 다시 전으로 돌아가서 중복여부 확인
				{
					i-=1;
					j = i + 1;
				}
			}
			if (rand_idx == 0)//0이면 중복값이 없으므로 저장하고 넘어감
			{
				goal[i] = tmp;
			}
		}
		else { goal[0] = tmp; }
		i++;
	}
	//아마 중복수 나올 경우의 수도 있어서 확인해보길 바람
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
	goalNumber = randTargetNum; //공통 클래스에 저장
	// 랜덤으로 목표값 설정(상대방이 보낼 값), 중복되는 숫자 제거
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
	targetSize = randTargetNum;// 후수 클래스에 값 저장
	return goal;
}

int commonPlay::calcNumFirst(int rxCond, int rx)//선수용 알고리즘
{
	rxcondNum = rxCond;
	if (turnNum == 1)
	{
		txcondNum = (goal[0]) % 6;//goal[0]+1 에서 변화함
		if (txcondNum == 0) { txcondNum = 1; }
		hintNum = txcondNum;
		turnNum++;
	}
	else
	{
		txcondNum = rxcondNum;
		if (rxcondNum == 1) {
			if ((rx + 1 - hintNum) % 6 == 0) { txcondNum = 1; } // 1을 더해서 공차가 6인지 확인
			else if ((rx + 2 - hintNum) % 6 == 0) { txcondNum = 2; } // 2을 더해서 공차가 6인지 확인
			else { txcondNum = 1; }
		}
		else if (rxcondNum == 5) {
			if ((rx + 4 - hintNum) % 6 == 0) { txcondNum = 4; } // 4을 더해서 공차가 6인지 확인
			else if ((rx + 5 - hintNum) % 6 == 0) { txcondNum = 5; } // 5을 더해서 공차가 6인지 확인
			else { txcondNum = 4; }
		}
		else if (rxcondNum >= 2 && rxcondNum <= 4) {
			if ((rx + rxcondNum - hintNum) % 6 == 0) { ; }
			else if ((rx + rxcondNum - 1 - hintNum) % 6 == 0) { txcondNum -= 1; } // plusNum - 1 값을 더해서 공차가 6인지 확인
			else if ((rx + rxcondNum + 1 - hintNum) % 6 == 0) { txcondNum += 1; } // plusNum + 1 값을 더해서 공차가 6인지 확인
			else { txcondNum -= 1; } // 공차가 6이 될 수 없다면 최소값으로 더함
		}
	}
	if (rx + txcondNum > 99) { txcondNum = 99 - rx; }
	return txcondNum;
}
int commonPlay::calcNumSecond(int rxCond, int rx)//후수용 알고리즘
{
	rxcondNum = rxCond;
	txcondNum = rxcondNum;
	if (rxcondNum == 1) {
		if ((rx + 1) % 6 == 0) { txcondNum = 1; } // 1을 더해서 공차가 6인지 확인
		else if ((rx + 2) % 6 == 0) { txcondNum = 2; } // 2을 더해서 공차가 6인지 확인
		else { txcondNum = 1; }
	}
	else if (rxcondNum == 5) {
		if ((rx + 4) % 6 == 0) { txcondNum = 4; } // 4을 더해서 공차가 6인지 확인
		else if ((rx + 5) % 6 == 0) { txcondNum = 5; } // 5을 더해서 공차가 6인지 확인
		else { txcondNum = 4; }
	}
	else if (rxcondNum >= 2 && rxcondNum <= 4) {
		if ((rx + rxcondNum) % 6 == 0) { ; }
		else if ((rx + rxcondNum - 1) % 6 == 0) { txcondNum -= 1; } // plusNum - 1 값을 더해서 공차가 6인지 확인
		else if ((rx + rxcondNum + 1) % 6 == 0) { txcondNum += 1; } // plusNum + 1 값을 더해서 공차가 6인지 확인
		else { txcondNum -= 1; } // 공차가 6이 될 수 없다면 최소값으로 더함
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
			txcondNum = rxcondNum - (rand() % 2); // 4,5 반환
		}
		else if (rxcondNum == 1)
		{
			txcondNum = rxcondNum + (rand() % 2); // 1,2 반환
		}
		else
		{
			txcondNum = rxcondNum + (rand() % 3) - 1; //+-1과 0중에 더해서 반환
		}
		if (rx + txcondNum > 99) { txcondNum = 99 - rx; }
	}
	if ((useChance < 2)&&(forTest>=5))
	{
		if ((myScore < yourScore) && (forTest == 5))//찬스 2번이상 사용상황
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
		else if ((myScore >= yourScore) && (forTest == 6))//찬스가 없는데 사용
		{
			if ((txcondNum<4))
			{
				txcondNum = 5;
			}
			else if ((txcondNum>2))
			{
				txcondNum = 1;
			}
			useChance += 2;//2번쓴걸로 인식해서 찬스 한번만 쓰게 함
		}
	}
	return txcondNum;
}
int commonPlay::calcRandom(int rxCond, int rx)//랜덤용 알고리즘
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
			txcondNum = rxcondNum - (rand() % 2); // 4,5 반환
		}
		else if (rxcondNum == 1)
		{
			txcondNum = rxcondNum + (rand() % 2); // 1,2 반환
		}
		else
		{
			txcondNum = rxcondNum + (rand() % 3) - 1; //+-1과 0중에 더해서 반환
		}
	}
	return txcondNum;
}
// goal : 현재 목표값
// rxNum : 현재 저장된 값
// rxcondNum : 상대측이 더한값
// myFlag가 1이면 찬스를 쓸 수 있다는 의미
// 반환 값은 우리가 더할 값이 될 것임
int commonPlay::chanceFunction(int goal, int rxNum, int rxcondNum,int myFlag) {
	// 이 Branch에 접근하면 우리팀이 찬스를 쓸 수 있음
	// 찬스 알고리즘 적용
	if (myFlag == 1) {
		if ((goal - rxNum) <= 5) {
			// 찬스를 쓸수 있는 상황
			// 우리가 최종적으로 더할 값
			return goal - rxNum; // 심판 클래스를 거칠테니까 찬스를 썼다 안썼다 신호는 안줘도 될 듯해서 안넣었어
		}
		else if ((goal - rxNum) > 5) {
			// 찬스를 못쓰는 상황
			if (rxcondNum == 1) { return 1; }
			else if (rxcondNum == 5) { return 4; }
			else if (rxcondNum > 1 && rxcondNum < 5) { return rxcondNum - 1; }
		}
	}
	//이 Branch에 접근하면 찬스를 쓸 수 없음
	else if (myFlag != 1) {
		if ((goal - rxNum) <= 5) {
			return 0; // 반환값이 0이라면 기존 알고리즘 적용하면 됨
		}
	}
	return 0;
}
// 초기 세팅할 때 들어가는 함수
// 반환 값 -> 0이면 알고리즘이 목표값 개수 정하기
//         -> 0이 아니면 이 함수에서 반환하는 값이 목표값 개수임
void commonPlay::initialChanceFunction(int myScore, int yourScore) {
	int difScore;
	difScore = myScore - yourScore;
	// 우리팀이 지고 있는 경우
	/*ㅁ 상대와의 점수 격차 + 1만큼 반환<역전해야지~~~> */
	if (difScore < 0) {
		chanceGoalNum = -difScore + 1;//ms
		if (chanceGoalNum == 8)//8넘으면 어쩔려고 훈식아
		{
			chanceGoalNum = 7;
		}
	}
	// 상대팀이 지고 있는 경우
	/*ㅁ 목표값 개수는 무조건 1개만<위험위험~~~~~>*/
	else if (difScore > 0) {
		chanceGoalNum= 1;//ms
	}
	chanceGoalNum= 0;//비기고 있는 경우
}