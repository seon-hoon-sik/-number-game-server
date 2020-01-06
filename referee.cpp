#include<iostream>
#include"numberCounting.h"
#include<cmath>
using namespace std;

// 생성자
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
		// 동점일 때는 어떠한 일도 하지 않는다.
	}
	else if (myScore > yourScore) {
		if (yourUseChance == 0) { yourFlag = 1; }
	}
	if (yourUseChance == 1) { yourFlag = 2; };
}

/*ㅁ 초기 우리팀 찬스 myFlag = 0
	ㅁ 우리팀이 지고 있을 때 우리팀의 찬스 허용 myFlag = 1
	ㅁ 우리팀이 찬스를 썼다면 myFlag = 2*/
void referee::myChanceCheck(int myScore, int yourScore) {
	if (myScore == 0 && yourScore == 0) {
		myFlag = 0;
	}
	if (myScore == yourScore) {
		// 동점일 때는 어떠한 일도 하지 않는다.
	}
	else if (myScore < yourScore) {
		if (myUseChance == 0) { myFlag = 1; }
	}
	if (myUseChance == 1) { myFlag = 2; }

}

// 공통
   /*ㅁ 초기 점수가 0:0이면 flag = 0
	ㅁ 초기세팅을 마무리 했다면 flag = 1로 세팅*/
	// 선수
	  /*ㅁ 상대측이 보낸 목표 값이 31 ~ 99인지 확인 ㅇㅋ
		ㅁ 상대측이 보낸 목표 값 개수가 1 ~ 7개인지 확인 ㅇㅋ
		ㅁ 상대측이 처음에 더한 값이 1 ~ 5값인지 확인 ㅇㅋ
		ㅁ 우리팀이 선수라면 firstCheck = 1, 우리팀이 후수라면 firstCheck = 0*/
		// 후수
		  /*ㅁ 상대측이 보낸 목표 값이 우리가 보낸 목표값에 포함되는지 확인
			ㅁ 상대측이 초기 시작한 값이 1 ~ 5값인지 확인*/
void referee::gameCheck(int& myScore, int& yourScore, int goal[], int& goalNum, int& rxcondNum, int& rxNum, int& txcondNum, int& txNum) {
	// 초기 값 세팅
	int i, j;
	if (flag == 0) {
		// 선수 초기 값 세팅
		if (firstCheck) {
			if (goalNum < 1 || goalNum>7) { cout << "상대팀이 보낸 목표값 개수 범위(1~7개)가 아닙니다." << endl; }//목표값 처음에 처리 불가능, 중복 못찾음
			for (i = 0; i < goalNum; i++) {
				if (goal[i] < 31 || goal[i]>99) { cout << "상대팀이 보낸 목표값 범위(31~99)가 아닙니다." << endl; }
			}
			if (settingNum == 2) {
				if (rxcondNum < 1 || rxcondNum>5) {
					cout << "상대팀이 처음에 더한 숫자가 범위(1~5)를 벗어났습니다." << endl;
				}
				flag++;
			}
			settingNum++;
		}
		// 후수 초기 값 세팅
		else if (!firstCheck) {
			//0번째
			if (secondCheck == 0) {
				arrayCheck = new int[goalNum];
				arrayCheckSize = goalNum;
				for (i = 0; i < arrayCheckSize; i++) { //
					arrayCheck[i] = goal[i];//
					cout << arrayCheck[i] << " ";
				} //
				secondCheck++;
			}
			//1번째
			else if (secondCheck == 1) {
				if (goalNum < 0 || goalNum>7) { cout << "상대팀이 보낸 목표값 개수 범위(1~7개)가 아닙니다." << endl; }
				if (settingNum == 0)
				{
					for (i = 0; i < goalNum; i++) {
						for (j = 0; j < arrayCheckSize; j++) {
							if (goal[i] == arrayCheck[j]) { numCheck++; }
						}
					}
				}
				if (numCheck != goalNum) {
					cout << "상대팀이 보낸 값은 우리팀이 보낸 값과 다릅니다." << endl;
					numCheck = goalNum;//2번째때 재체크 방지용
				}
				//2번째
				if (settingNum == 1) {
					if (rxNum < 1 || rxNum>5) {
						cout << "상대팀이 초기에 설정한 값이 범위(1~5)를 벗어났습니다." << endl;
					}
					flag++;
				}
				settingNum++;
			}
		}
	}
	// 초기 값 세팅 후 작업
	else if (flag == 1) {
		yourChanceCheck(myScore, yourScore);
		myChanceCheck(myScore, yourScore);

		//////////////////////////////////////////////// 상대팀이 규칙을 어겼는지에 대한 여부/////////////////////////////////////////////
		if (yourFlag == 0) {
			if ((abs(rxcondNum - txcondNum) > 1) && ((rxNum - txNum) > 0)) {//상대가 보낸 값이 더 클때 = 수신했을때
				cout << "상대팀이 규칙을 어겼습니다.(우리팀이 더한값과의 차가 2이상 차이가 남)" << endl;
			}
			if (rxcondNum <= 0 || rxcondNum >= 6) { cout << "상대팀이 규칙을 어겼습니다. 0이하 6이상의 값을 보냈습니다." << endl; } // 추가
		}
		// 상대팀이 찬스를 쓸 수 있다는 의미
		else if (yourFlag == 1) {
			if ((abs(rxcondNum - txcondNum) > 1) && ((rxNum - txNum) > 0)) {
				yourUseChance++;
				cout << "상대가 찬스를 사용하였습니다." << endl;
			}
		}
		// 상대팀이 찬스를 썼다는 의미
		else if (yourFlag == 2) {//수신했을때
			if ((abs(rxcondNum - txcondNum) > 1) && ((rxNum - txNum) > 0)) {
				cout << "상대팀이 규칙을 어겼습니다.(찬스를 사용했는데 우리팀이 더한값과의 차와 2이상 차이가 남)" << endl;
				cout << rxNum << " " << rxcondNum << " " << txcondNum << endl;//ms
			}
			if (rxcondNum <= 0 || rxcondNum >= 6) { cout << "상대팀이 규칙을 어겼습니다. 0이하 6이상의 값을 보냈습니다." << endl; } // 추가
		}

		//////////////////////////////////////////////// 우리팀의 찬스 체크 여부 /////////////////////////////////////////////
		if (myFlag == 0) {
			;
		}
		else if (myFlag == 1) {
			if (abs(rxcondNum - txcondNum) > 1) {
				cout << "찬스를 썼습니다." << endl;
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