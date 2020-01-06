#include<string>
using namespace std;
#ifndef numCounting_H
#define numCounting_H

class referee {
public:
	// 변수 초기화
	referee(int judgeWho);
	~referee();
	/*ㅁ 초기 상대 찬스 yourFlag = 0
	  ㅁ 우리팀이 이기고 있을 때 상대방의 찬스 허용 yourFlag = 1
	  ㅁ 상대방이 찬스를 썼다면 yourFlag = 2*/
	void yourChanceCheck(int myScore, int yourScore);
	/*ㅁ 초기 우리팀 찬스 myFlag = 0
	  ㅁ 우리팀이 지고 있을 때 우리팀의 찬스 허용 myFlag = 1
	  ㅁ 우리팀이 찬스를 썼다면 myFlag = 2*/
	void myChanceCheck(int myScore, int yourScore);
	// 공통
	/*ㅁ 초기 점수가 0:0이면 flag = 0
	  ㅁ 초기세팅을 마무리 했다면 flag = 1로 세팅*/
	  // 선수 & 후수
	  /*ㅁ 상대측이 보낸 목표 값이 31 ~ 99인지 확인
	   ㅁ 상대측이 보낸 목표 값 개수가 1 ~ 7개인지 확인
	   ㅁ 상대측이 처음에 더한 값이 1 ~ 5값인지 확인
	   ㅁ 우리팀이 선수라면 firstCheck = 1, 우리팀이 후수라면 firstCheck = 0*/
	   // 후수
	  /*ㅁ 상대측이 보낸 목표 값이 우리가 보낸 목표값에 포함되는지 확인
		  ㅁ 상대측이 초기 시작한 값이 1 ~ 5값인지 확인*/
		  //void yourCheck(int myScore, int yourScore, int goal[], int goalNum, int rxcondNum);
	void gameCheck(int& myScore, int& yourScore, int goal[], int& goalNum, int& rxcondNum, int& rxNum, int& txcondNum, int& txNum);
	int* returnFlag();
private:
	int flag = 0, myFlag = 0, yourFlag = 0; // flag -> 초기 세팅, myFlag & yourFlag는 찬스를 판단할 변수
	int firstCheck; // 우리가 선수인지(= 1) 후수인지(= 0) 판별해주는 변수
	int settingNum = 0;
	int yourUseChance = 0;
	int myUseChance = 0;
	int* rFlag = nullptr;
	int* arrayCheck = nullptr; //체크
	int arrayCheckSize; // 체크
	int numCheck = 0; // 체크
	int secondCheck = 0; // 체크
};

class commonPlay
{
public:
	friend class batFirst;
	friend class batSecond;
private:
	int* goal;
	int goalNumber, turnNum = 1;
	int txcondNum, rxcondNum;
	int hintNum, chanceGoalNum = 0;
	int* firstTargetAlgo(int* targets, int& targetSize);
	int* secondTargetAlgo(int& targetSize);
	int* testFirstTargetAlgo(int* targets, int& targetSize, int forTest); //testvector 용 알고리즘
	int* testSecondTargetAlgo(int& targetSize, int forTest);
	int* randFirstTargetAlgo(int* targets, int& targetSize);//랜덤 알고리즘
	int* randSecondTargetAlgo(int& targetSize);
	int calcNumFirst(int rxCond, int rx);//선수용 알고리즘
	int calcNumSecond(int rxCond, int rx);//후수용 알고리즘
	int calcTest(int rxCond, int rx, int forTest, int myScore, int yourScore);//testvector
	int calcRandom(int rxCond, int rx);//랜덤용 알고리즘
	int chanceFunction(int goal, int rxNum, int rxcondNum, int myFlag);
	void initialChanceFunction(int myScore, int yourScore);
};
class batFirst {
public:
	//자동
	~batFirst();
	batFirst(int forTest, int myScore, int yourScore);
	// forTest는 0번이 기본동작, 1번은 랜덤 동작, 2번은 목표값 8개 상황, 3번은 목표값 중복, 4번은 목표값이 보낸 값과 다름, 5번은 찬스를 두번 사용, 6번은 찬스 없는데 사용
	void getTarget(int target[], const int targetSize);//최초 목표값 획득
	int* setTarget(); //목표값 반환
	int tx(); // 보낼값 반환하는 발신
	void rx(int receiveNum); //수신 값 저장
	void clearScore();
	int getGoalNum();// 목표값 갯수 반환
	int* returnScore();
private:
	commonPlay first;
	referee* firstReferee;
	int rxNum = 0, txNum = 0, goalNum = 0;
	int idx_goal = 0, testVector;
	int ourScore = 0, theirScore = 0;
	int txcondNum = 0, rxcondNum = 0;//+-1과 1에서 5사이의 숫자를 더하는지에 대한 기준
	int* goal = nullptr;// 포인터로 선언 하여 배열을 추후에 동적할당
	//랜덤 자동
	int* score = nullptr;
	int randNum = 0;
	int ourFlag, theirFlag;
	void saveScore(); //점수저장
};
////////////////////////////////////////////////////////////////////
//후수 클래스
class batSecond {
public:
	batSecond(int forTest, int myScore, int yourScore);
	// forTest는 0번이 기본동작, 1번은 랜덤 동작, 2번은 목표값 8개 상황, 3번은 목표값 중복, 4번은 목표값이 보낸 값과 다름, 5번은 찬스를 두번 사용, 6번은 찬스 없는데 사용
	//7번 상대방이 0이하 5초과, 8번 목표값 100초과 30미만// 
	~batSecond();
	void getTarget(int target[], const int targetSize);//최초 목표값 획득
	int* setTarget(); //목표값 설정 및 반환
	int tx();
	void rx(int receiveNum);
	void clearScore();
	int getGoalNum();
	int* returnScore();
private:
	commonPlay second;
	referee* secondReferee;
	int* sendGoal = nullptr;
	int goalNum = 0;
	int* goal = nullptr;// 포인터로 선언 하여 배열을 추후에 동적할당
	int rxNum = 0, txNum = 0;
	int idx_goal = 0;
	int ourScore = 0, theirScore = 0;
	int txcondNum = 0, rxcondNum = 0;//+-1과 1에서 5사이의 숫자를 더하는지에 대한 기준
	int randNum = 0, testVector;
	int ourFlag, theirFlag;
	int* score = nullptr;
	void saveScore();
};
#endif