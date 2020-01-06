#include "Serv.h"
#include "numberCounting.h"
#include <iostream>
#include <string>
#include <malloc.h>
#define DEBUGMODE 1 //1일때 자동 0일때 후수, 선수 고름

using namespace std;
void Set_Clear(char* temp);
void print(char* pa);
bool judgeFin(batFirst* first, batSecond* second, int goalNum);
void decoding(char* temp, const char* input);
void encoding(char* temp, const char* input);
void Check_goal(char* temp, int num);
void Check_numNow(char* temp);
void Check_win(char* temp);

int main()
{
	char temp3[bufsize] = {};
	char temp4[bufsize] = { 102, 0, -1 };
	char tcp_temp[bufsize];
	char Win_send[bufsize] = { 103, -1 };
	char Lose_send[bufsize] = { 104, -1 };
	char Draw_send[bufsize] = { 105, -1 };
	Set_Clear(temp3);
	//*********************************서버*********************************
	WSADATA wsa;
	char buf[bufsize];

	std::cout << "************************통신 대기중***********************" << std::endl;


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cout << "WSAStartup 오류" << std::endl;
		return 0;
	}

	int server_socket, client_socket, client_addr_size;
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == server_socket) {
		std::cout << "소켓 통신 에러" << std::endl;
		return 0;
	}

	SOCKADDR_IN server_addr, client_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(1111); //4777
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (SOCKET_ERROR == bind(server_socket, (const sockaddr*)&server_addr, sizeof(server_addr))) {
		std::cout << "포트 바인딩 에러" << std::endl;
		return 0;
	}

	if (SOCKET_ERROR == listen(server_socket, 5)) {
		std::cout << "대기상태 에러" << std::endl;
		return 0;
	}

	client_addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*) & client_addr, &client_addr_size);

	if (SOCKET_ERROR == client_socket) {
		std::cout << "accept 에러" << std::endl;
		return 0;
	}

	std::cout << "수비 접속완료" << std::endl;
	std::cout << "IP : " << inet_ntoa(client_addr.sin_addr) << " Port : " << client_addr.sin_port << std::endl;

	//*******************************통신*******************************

	char temp[bufsize] = {};
	char temp2[bufsize] = {};
	int Count = 0;
	int i;
	int* goal = nullptr;//목표값 포인터
	int goalNum = 0, numNow = 0;//목표값 갯수, 현재 숫자
	int myScore = 0, yourScore = 0;
	bool finish = false;
	string inString;
	batFirst* first = nullptr;
	batSecond* second = nullptr;
	int test, whatTest;
	cout << "역할이 선수인지 후수인지 입력하세요 (ex:선수,후수) ";
	cin >> inString;
	cout << "테스트 인가요? testVector 사용시 1, 미사용 시 0" << endl;
	cin >> test;
	if (test == 1)
	{
		cout << "무슨 시험을 할것인지 고르시오 \n" << "1번은 랜덤 동작(찬스 사용 가능), \n2번은 목표값 8개 상황,(후수만 사용) ";
		cout << "\n3번은 목표값 중복(후수만 사용가능), \n4번은 목표값이 보낸 값과 다름,(선수만 사용) \n5번은 찬스를 두번 사용, \n6번은 찬스 없는데 사용" << endl;
		cin >> whatTest;
	}
	else if (test == 0) { whatTest = 0; }
	if (inString == "선수")
	{
		memset(temp, 0, bufsize); //temp초기화
		memset(tcp_temp, 0, bufsize); //temp초기화

		int Count{};
		recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //여기서 목표값 수령
		decoding(temp, tcp_temp);
		while (temp[Count++] != -1);  //goalNum계산
		//Count--;
		goalNum = Count - 2;

		Check_goal(temp, goalNum); // 값 잘 받았는지 체크

		goal = new int[goalNum]; //2씩 빼주어서 쓰레기값 2개 없앰

		cout << "받은 목표값은 ";
		for (int i = 0; i < goalNum; i++) {
			goal[i] = temp[i + 1];
			cout << goal[i] << " ";
		} //temp2받은걸 다시 goal로 바꿔줌
		cout << endl;

		first = new batFirst(whatTest, 0, 0);
		first->getTarget(goal, goalNum);// 목표값 수령필요

		goal = first->setTarget();//목표값 설정 후 반환 및 goalNum설정 필요
		goalNum = first->getGoalNum();

		memset(temp, 0, bufsize); //temp초기화
		for (int i = 0; i < goalNum; i++) {
			swap(temp3[i + 1], temp3[i + 2]);
			temp3[i + 1] = goal[i];
		}
		encoding(tcp_temp, temp3);
		send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //클라이언트로 전달

		cout << "최종 목표값은 ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}
		cout << endl;

		while (finish == false)
		{
			numNow = first->tx(); //처음 시작시 값을 보냄
			memset(temp2, 0, bufsize); //temp2초기화
			temp4[1] = numNow;
			encoding(tcp_temp, temp4);
			send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //numNow 전달

			cout << "선수 값 : " << numNow << endl;

			finish = judgeFin(first, second, goalNum); //보낸 후 게임이 끝났는지 검증
			if (finish == true)break;

			memset(temp, 0, bufsize); //temp초기화
			recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //클라이언트의 값 수령
			decoding(temp, tcp_temp);
			numNow = temp[1];

			Check_numNow(temp); // 값 잘 받았는지 체크

			first->rx(numNow); //여기에 상대의 찬스 사용 여부 판단 넣자
			cout << "후수 값: " << numNow << endl; //후수한테 무슨 값을 받았는지 출력
			finish = judgeFin(first, second, goalNum);
			if (finish == true)break;
		}
		finish = false;
		cout << "*******************************1라운드 종료 *******************************" << endl;
		myScore = first->returnScore()[0]; // 내 점수 반환
		yourScore = first->returnScore()[1]; // 님 점수 반환
		int totalScore = myScore + yourScore;
		cout << "중간 점수 집계 나: 상대 기준 " << myScore << " : " << yourScore << endl;
		second = new batSecond(whatTest, myScore, yourScore);
		delete first; //처음플레이한 선수삭제 ptr
		first = nullptr;
		for (i = 0; i < goalNum; i++)
		{
			goal[i] = NULL;
		}
		//delete[]goal;//경기로 사용되는 목표값 제거
		goalNum = 0;
		//새로운 라운드 새로운 목표값 생성
		goal = second->setTarget();
		//목표값 전송 및 수신필요 이후,goal 과 goalNum지정해야됨/////////////
		//------------
		goalNum = second->getGoalNum();
		memset(temp, 0, bufsize); //temp초기화
		memset(temp3, 0, bufsize);
		temp3[0] = 101, temp3[1] = -1;
		for (int i = 0; i < goalNum; i++) {
			swap(temp3[i + 1], temp3[i + 2]);
			temp3[i + 1] = goal[i];
		}
		encoding(tcp_temp, temp3);


		send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //goal 전달

		cout << "보낸 목표값은 ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}

		memset(temp, 0, bufsize); //temp초기화
		Set_Clear(temp3);
		memset(tcp_temp, 0, sizeof(tcp_temp));
		recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //goal 수신
		decoding(temp, tcp_temp);

		int Count1 = 0;
		while (temp[Count1++] != -1);
		goalNum = Count1 - 2;

		Check_goal(temp, goalNum); // 값 잘 받았는지 체크

		goal = new int[goalNum];

		for (int i = 0; i < goalNum; i++) {
			goal[i] = (int)temp[i + 1];
		} //temp받은걸 다시 goal로 바꿔줌


		second->getTarget(goal, goalNum);

		cout << "최종 목표값은 ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}
		cout << endl;
		while (finish == false)
		{
			Set_Clear(temp3);
			memset(temp2, 0, bufsize); //temp2초기화
			recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //클라이언트 값 수신
			decoding(temp2, tcp_temp);
			numNow = temp2[1];

			Check_numNow(temp2); // 값 잘 받았는지 체크

			cout << "선수 값 : " << numNow << endl;

			second->rx(numNow);
			cout << "후수 값: " << numNow << endl; //후수한테 무슨 값을 받았는지 출력
			finish = judgeFin(first, second, goalNum + totalScore);
			if (finish == true)break;
			numNow = second->tx(); //처음 시작시 값을 보냄

			Set_Clear(temp3);
			memset(temp, 0, bufsize); //temp초기화
			temp4[1] = numNow;
			encoding(tcp_temp, temp4);
			send(client_socket, tcp_temp, sizeof(tcp_temp), 0);
			cout << "선수 값 : " << numNow << endl;

			finish = judgeFin(first, second, goalNum + totalScore); //보낸 후 게임이 끝났는지 검증
			if (finish == true)break;
		}
		finish = false;
		myScore = second->returnScore()[0]; // 내 점수 반환
		yourScore = second->returnScore()[1]; // 님 점수 반환
		cout << "점수는 나: 상대 기준 " << myScore << " : " << yourScore << endl;
		
		memset(tcp_temp, 0, bufsize);
		if ((int)myScore > (int)yourScore) {
			encoding(tcp_temp, Win_send);
		}
		else if ((int)myScore == (int)yourScore) {
			encoding(tcp_temp, Draw_send);
		}
		else if ((int)myScore < (int)yourScore) {
			encoding(tcp_temp, Lose_send);
		}
		send(client_socket, tcp_temp, sizeof(tcp_temp), 0);
}

	/////////////////////////////////////후수
	else if (inString == "후수")
	{
		second = new batSecond(whatTest, 0, 0);
		goal = second->setTarget();
		goalNum = second->getGoalNum();

		memset(temp, 0, bufsize); //temp초기화
		Set_Clear(temp3);
		cout << goalNum << endl;
		for (int i = 0; i < goalNum; i++) {
			swap(temp3[i + 1], temp3[i + 2]);
			temp3[i + 1] = goal[i];
		}
		encoding(tcp_temp, temp3);
		send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //goal 전달


		cout << "보낸 목표값은 ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}
		memset(temp, 0, bufsize); //temp초기화
		recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //goal 수신
		decoding(temp, tcp_temp);

		int Count = 0;
		while (temp[Count++] != -1);  //goalNum계산
		//Count--;
		goalNum = Count - 2;

		Check_goal(temp, goalNum); // 값 잘 받았는지 체크

		goal = new int[goalNum];

		for (int i = 0; i < goalNum; i++) {
			goal[i] = temp[i + 1];
		} //temp2받은걸 다시 goal로 바꿔줌

		second->getTarget(goal, goalNum);
		cout << "최종 목표값은 ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}
		//1라운드
		while (finish == false)
		{
			memset(temp2, 0, bufsize); //temp초기화
			recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //선수의 값 수령
			decoding(temp2, tcp_temp);
			numNow = temp2[1];

			Check_numNow(temp2); // 값 잘 받았는지 체크

			cout << "선수 값 : " << numNow << endl;

			second->rx(numNow); //여기에 상대의 찬스 사용 여부 판단 넣자
			cout << "후수 값: " << numNow << endl; //후수한테 무슨 값을 받았는지 출력
			finish = judgeFin(first, second, goalNum);
			if (finish == true)break;

			numNow = second->tx(); //처음 시작시 값을 보냄
			memset(temp, 0, bufsize); //temp초기화
			temp4[1] = numNow;
			encoding(tcp_temp, temp4);
			send(client_socket, tcp_temp, sizeof(tcp_temp), 0);

			cout << "선수 값 : " << numNow << endl;
			//보낸 후 게임이 끝났는지 검증
			finish = judgeFin(first, second, goalNum);
			if (finish == true)break;
		}
		finish = false;
		//1라운드 종료*********************************
		myScore = second->returnScore()[0]; // 내 점수 반환
		yourScore = second->returnScore()[1]; // 님 점수 반환
		int totalScore = myScore + yourScore;
		cout << "중간 점수 집계 나: 상대 기준 " << myScore << " : " << yourScore << endl;
		first = new batFirst(whatTest, myScore, yourScore);//선수의 생성
		delete second;//처음플레이한 후수 삭제 ptr
		second = nullptr;
		for (i = 0; i < goalNum; i++)
		{
			goal[i] = NULL;
		}
		//delete[]goal;
		goalNum = 0;

		//2라운드 시작
		memset(temp, 0, bufsize); //send_buf초기화
		Set_Clear(temp3);
		recv(client_socket, tcp_temp, sizeof(tcp_temp), 0);
		decoding(temp, tcp_temp);
		int Count2 = 0;
		while (temp[Count2++] != -1);
		goalNum = Count2 - 2;

		Check_goal(temp, goalNum); // 값 잘 받았는지 체크

		goal = new int[goalNum];

		cout << "받은 목표값은 ";
		for (int i = 0; i < goalNum; i++) {
			goal[i] = temp[i + 1];
			cout << goal[i] << " ";
		} //temp2받은걸 다시 goal로 바꿔줌
		cout << endl;

		first->getTarget(goal, goalNum);// 목표값 수령필요

		goal = first->setTarget();//목표값 설정 후 반환 및 goalNum설정 필요
		goalNum = first->getGoalNum();

		cout << goalNum << " : ";
		Set_Clear(temp3);
		memset(temp, 0, bufsize); //temp초기화
		for (int i = 1; i <= goalNum; i++) {
			temp3[i + 1] = temp3[i];
			temp3[i] = goal[i - 1];
		}
		encoding(tcp_temp, temp3);
		send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //goal 전달


		cout << "최종 목표값은 ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}
		cout << endl;
		while (finish == false)
		{
			numNow = first->tx(); //처음 시작시 값을 보냄
			memset(temp2, 0, bufsize); //temp초기화
			temp4[1] = numNow;
			encoding(tcp_temp, temp4);
			send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //numNow 전달

			cout << "후수 값 : " << numNow << endl;

			finish = judgeFin(first, second, goalNum + totalScore);
			if (finish == true)break;

			memset(temp, 0, bufsize); //temp초기화
			recv(client_socket, tcp_temp, sizeof(tcp_temp), 0);
			decoding(temp, tcp_temp);
			numNow = temp[1];

			Check_numNow(temp); // 값 잘 받았는지 체크

			cout << "선수 값 : " << numNow << endl;

			first->rx(numNow); //여기에 상대의 찬스 사용 여부 판단 넣자
			finish = judgeFin(first, second, goalNum + totalScore);
			if (finish == true)break;
		}
		finish = false;
		myScore = first->returnScore()[0]; // 내 점수 반환
		yourScore = first->returnScore()[1]; // 님 점수 반환
		cout << "점수는 나: 상대 기준 " << myScore << " : " << yourScore << endl;
		
		memset(temp, 0, bufsize); //temp초기화
		memset(tcp_temp, 0, bufsize); //temp초기화
		recv(client_socket, tcp_temp, sizeof(tcp_temp), 0);
		decoding(temp, tcp_temp);

		Check_win(temp);

	}
}

bool judgeFin(batFirst* first, batSecond* second, int goalNum)
{
	if (second == nullptr)
	{
		if (first->returnScore()[0] + first->returnScore()[1] >= (goalNum))//각 스코어의 합이 목표 값이 넘어가면 1차 게임 끝
		{
			cout << "선수기준" << first->returnScore()[0] << ":" << first->returnScore()[1];
			return true;
		}
	}
	else if (first == nullptr)
	{
		if (second->returnScore()[0] + second->returnScore()[1] >= (goalNum))//각 스코어의 합이 목표 값이 넘어가면 1차 게임 끝
		{
			cout << "후수기준" << second->returnScore()[0] << ":" << second->returnScore()[1];
			return true;
		}
	}
	return false;
}

void Set_Clear(char* temp) {
	memset(temp, 0, bufsize * sizeof(char));
	temp[0] = 101;
	temp[1] = -1;
}

void print(char* pa) {
	for (int i = 0; i < 1024; i++)
		cout << (int)pa[i] << ' ';
	cout << endl;
}

void decoding(char* temp, const char* input) {
	memset(temp, 0, sizeof(temp));
	temp[0] = 100 + input[2] - 48;
	int idx = 3, now_idx = 1;
	for (; input[idx] != '-'; idx += 2, now_idx++)
		temp[now_idx] = (input[idx] - 48) * 10 + input[idx + 1] - 48;
	temp[now_idx] = -1;
}

void encoding(char* temp, const char* input) {
	memset(temp, 0, sizeof(temp));
	temp[0] = 49;
	temp[1] = 48;
	temp[2] = input[0] % 10 + 48;

	int idx = 1;
	int now_idx = 3;
	for (; input[idx] != -1; idx++) {
		temp[now_idx] = input[idx] / 10 + 48;
		temp[now_idx + 1] = input[idx] % 10 + 48;
		now_idx += 2;
	}
	temp[now_idx] = '-';
	temp[now_idx + 1] = '1';
}

void Check_goal(char* temp, int num) {
	if (temp[0] == 101 && temp[num + 1] == -1) {
		//cout << "규칙에 맞게 통신되었습니다" << endl;
	}
	else {
		cout << "규칙에 맞지 않게 통신되었습니다" << endl;
	}
}

void Check_numNow(char* temp) {
	if (temp[0] == 102 && temp[2] == -1) {
		//cout << "규칙에 맞게 통신되었습니다" << endl;
	}
	else {
		cout << "규칙에 맞지 않게 통신되었습니다" << endl;
	}
}

void Check_win(char* temp) {
	if (temp[0] == 103 && temp[1] == -1) {
		cout << "당신이 졌습니다." << endl;
	}
	else if (temp[0] == 104 && temp[1] == -1) {
		cout << "당신이 이겼습니다" << endl;
	}
	else if (temp[0] == 105 && temp[1] == -1) {
		cout << "비겼습니다." << endl;
	}
	else {
		cout << "오류";
	}
}