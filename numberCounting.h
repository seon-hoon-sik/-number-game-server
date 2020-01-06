#include<string>
using namespace std;
#ifndef numCounting_H
#define numCounting_H

class referee {
public:
	// ���� �ʱ�ȭ
	referee(int judgeWho);
	~referee();
	/*�� �ʱ� ��� ���� yourFlag = 0
	  �� �츮���� �̱�� ���� �� ������ ���� ��� yourFlag = 1
	  �� ������ ������ ��ٸ� yourFlag = 2*/
	void yourChanceCheck(int myScore, int yourScore);
	/*�� �ʱ� �츮�� ���� myFlag = 0
	  �� �츮���� ���� ���� �� �츮���� ���� ��� myFlag = 1
	  �� �츮���� ������ ��ٸ� myFlag = 2*/
	void myChanceCheck(int myScore, int yourScore);
	// ����
	/*�� �ʱ� ������ 0:0�̸� flag = 0
	  �� �ʱ⼼���� ������ �ߴٸ� flag = 1�� ����*/
	  // ���� & �ļ�
	  /*�� ������� ���� ��ǥ ���� 31 ~ 99���� Ȯ��
	   �� ������� ���� ��ǥ �� ������ 1 ~ 7������ Ȯ��
	   �� ������� ó���� ���� ���� 1 ~ 5������ Ȯ��
	   �� �츮���� ������� firstCheck = 1, �츮���� �ļ���� firstCheck = 0*/
	   // �ļ�
	  /*�� ������� ���� ��ǥ ���� �츮�� ���� ��ǥ���� ���ԵǴ��� Ȯ��
		  �� ������� �ʱ� ������ ���� 1 ~ 5������ Ȯ��*/
		  //void yourCheck(int myScore, int yourScore, int goal[], int goalNum, int rxcondNum);
	void gameCheck(int& myScore, int& yourScore, int goal[], int& goalNum, int& rxcondNum, int& rxNum, int& txcondNum, int& txNum);
	int* returnFlag();
private:
	int flag = 0, myFlag = 0, yourFlag = 0; // flag -> �ʱ� ����, myFlag & yourFlag�� ������ �Ǵ��� ����
	int firstCheck; // �츮�� ��������(= 1) �ļ�����(= 0) �Ǻ����ִ� ����
	int settingNum = 0;
	int yourUseChance = 0;
	int myUseChance = 0;
	int* rFlag = nullptr;
	int* arrayCheck = nullptr; //üũ
	int arrayCheckSize; // üũ
	int numCheck = 0; // üũ
	int secondCheck = 0; // üũ
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
	int* testFirstTargetAlgo(int* targets, int& targetSize, int forTest); //testvector �� �˰���
	int* testSecondTargetAlgo(int& targetSize, int forTest);
	int* randFirstTargetAlgo(int* targets, int& targetSize);//���� �˰���
	int* randSecondTargetAlgo(int& targetSize);
	int calcNumFirst(int rxCond, int rx);//������ �˰���
	int calcNumSecond(int rxCond, int rx);//�ļ��� �˰���
	int calcTest(int rxCond, int rx, int forTest, int myScore, int yourScore);//testvector
	int calcRandom(int rxCond, int rx);//������ �˰���
	int chanceFunction(int goal, int rxNum, int rxcondNum, int myFlag);
	void initialChanceFunction(int myScore, int yourScore);
};
class batFirst {
public:
	//�ڵ�
	~batFirst();
	batFirst(int forTest, int myScore, int yourScore);
	// forTest�� 0���� �⺻����, 1���� ���� ����, 2���� ��ǥ�� 8�� ��Ȳ, 3���� ��ǥ�� �ߺ�, 4���� ��ǥ���� ���� ���� �ٸ�, 5���� ������ �ι� ���, 6���� ���� ���µ� ���
	void getTarget(int target[], const int targetSize);//���� ��ǥ�� ȹ��
	int* setTarget(); //��ǥ�� ��ȯ
	int tx(); // ������ ��ȯ�ϴ� �߽�
	void rx(int receiveNum); //���� �� ����
	void clearScore();
	int getGoalNum();// ��ǥ�� ���� ��ȯ
	int* returnScore();
private:
	commonPlay first;
	referee* firstReferee;
	int rxNum = 0, txNum = 0, goalNum = 0;
	int idx_goal = 0, testVector;
	int ourScore = 0, theirScore = 0;
	int txcondNum = 0, rxcondNum = 0;//+-1�� 1���� 5������ ���ڸ� ���ϴ����� ���� ����
	int* goal = nullptr;// �����ͷ� ���� �Ͽ� �迭�� ���Ŀ� �����Ҵ�
	//���� �ڵ�
	int* score = nullptr;
	int randNum = 0;
	int ourFlag, theirFlag;
	void saveScore(); //��������
};
////////////////////////////////////////////////////////////////////
//�ļ� Ŭ����
class batSecond {
public:
	batSecond(int forTest, int myScore, int yourScore);
	// forTest�� 0���� �⺻����, 1���� ���� ����, 2���� ��ǥ�� 8�� ��Ȳ, 3���� ��ǥ�� �ߺ�, 4���� ��ǥ���� ���� ���� �ٸ�, 5���� ������ �ι� ���, 6���� ���� ���µ� ���
	//7�� ������ 0���� 5�ʰ�, 8�� ��ǥ�� 100�ʰ� 30�̸�// 
	~batSecond();
	void getTarget(int target[], const int targetSize);//���� ��ǥ�� ȹ��
	int* setTarget(); //��ǥ�� ���� �� ��ȯ
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
	int* goal = nullptr;// �����ͷ� ���� �Ͽ� �迭�� ���Ŀ� �����Ҵ�
	int rxNum = 0, txNum = 0;
	int idx_goal = 0;
	int ourScore = 0, theirScore = 0;
	int txcondNum = 0, rxcondNum = 0;//+-1�� 1���� 5������ ���ڸ� ���ϴ����� ���� ����
	int randNum = 0, testVector;
	int ourFlag, theirFlag;
	int* score = nullptr;
	void saveScore();
};
#endif