#include "Serv.h"
#include "numberCounting.h"
#include <iostream>
#include <string>
#include <malloc.h>
#define DEBUGMODE 1 //1�϶� �ڵ� 0�϶� �ļ�, ���� ��

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
	//*********************************����*********************************
	WSADATA wsa;
	char buf[bufsize];

	std::cout << "************************��� �����***********************" << std::endl;


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cout << "WSAStartup ����" << std::endl;
		return 0;
	}

	int server_socket, client_socket, client_addr_size;
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == server_socket) {
		std::cout << "���� ��� ����" << std::endl;
		return 0;
	}

	SOCKADDR_IN server_addr, client_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(1111); //4777
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (SOCKET_ERROR == bind(server_socket, (const sockaddr*)&server_addr, sizeof(server_addr))) {
		std::cout << "��Ʈ ���ε� ����" << std::endl;
		return 0;
	}

	if (SOCKET_ERROR == listen(server_socket, 5)) {
		std::cout << "������ ����" << std::endl;
		return 0;
	}

	client_addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*) & client_addr, &client_addr_size);

	if (SOCKET_ERROR == client_socket) {
		std::cout << "accept ����" << std::endl;
		return 0;
	}

	std::cout << "���� ���ӿϷ�" << std::endl;
	std::cout << "IP : " << inet_ntoa(client_addr.sin_addr) << " Port : " << client_addr.sin_port << std::endl;

	//*******************************���*******************************

	char temp[bufsize] = {};
	char temp2[bufsize] = {};
	int Count = 0;
	int i;
	int* goal = nullptr;//��ǥ�� ������
	int goalNum = 0, numNow = 0;//��ǥ�� ����, ���� ����
	int myScore = 0, yourScore = 0;
	bool finish = false;
	string inString;
	batFirst* first = nullptr;
	batSecond* second = nullptr;
	int test, whatTest;
	cout << "������ �������� �ļ����� �Է��ϼ��� (ex:����,�ļ�) ";
	cin >> inString;
	cout << "�׽�Ʈ �ΰ���? testVector ���� 1, �̻�� �� 0" << endl;
	cin >> test;
	if (test == 1)
	{
		cout << "���� ������ �Ұ����� ���ÿ� \n" << "1���� ���� ����(���� ��� ����), \n2���� ��ǥ�� 8�� ��Ȳ,(�ļ��� ���) ";
		cout << "\n3���� ��ǥ�� �ߺ�(�ļ��� ��밡��), \n4���� ��ǥ���� ���� ���� �ٸ�,(������ ���) \n5���� ������ �ι� ���, \n6���� ���� ���µ� ���" << endl;
		cin >> whatTest;
	}
	else if (test == 0) { whatTest = 0; }
	if (inString == "����")
	{
		memset(temp, 0, bufsize); //temp�ʱ�ȭ
		memset(tcp_temp, 0, bufsize); //temp�ʱ�ȭ

		int Count{};
		recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //���⼭ ��ǥ�� ����
		decoding(temp, tcp_temp);
		while (temp[Count++] != -1);  //goalNum���
		//Count--;
		goalNum = Count - 2;

		Check_goal(temp, goalNum); // �� �� �޾Ҵ��� üũ

		goal = new int[goalNum]; //2�� ���־ �����Ⱚ 2�� ����

		cout << "���� ��ǥ���� ";
		for (int i = 0; i < goalNum; i++) {
			goal[i] = temp[i + 1];
			cout << goal[i] << " ";
		} //temp2������ �ٽ� goal�� �ٲ���
		cout << endl;

		first = new batFirst(whatTest, 0, 0);
		first->getTarget(goal, goalNum);// ��ǥ�� �����ʿ�

		goal = first->setTarget();//��ǥ�� ���� �� ��ȯ �� goalNum���� �ʿ�
		goalNum = first->getGoalNum();

		memset(temp, 0, bufsize); //temp�ʱ�ȭ
		for (int i = 0; i < goalNum; i++) {
			swap(temp3[i + 1], temp3[i + 2]);
			temp3[i + 1] = goal[i];
		}
		encoding(tcp_temp, temp3);
		send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //Ŭ���̾�Ʈ�� ����

		cout << "���� ��ǥ���� ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}
		cout << endl;

		while (finish == false)
		{
			numNow = first->tx(); //ó�� ���۽� ���� ����
			memset(temp2, 0, bufsize); //temp2�ʱ�ȭ
			temp4[1] = numNow;
			encoding(tcp_temp, temp4);
			send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //numNow ����

			cout << "���� �� : " << numNow << endl;

			finish = judgeFin(first, second, goalNum); //���� �� ������ �������� ����
			if (finish == true)break;

			memset(temp, 0, bufsize); //temp�ʱ�ȭ
			recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //Ŭ���̾�Ʈ�� �� ����
			decoding(temp, tcp_temp);
			numNow = temp[1];

			Check_numNow(temp); // �� �� �޾Ҵ��� üũ

			first->rx(numNow); //���⿡ ����� ���� ��� ���� �Ǵ� ����
			cout << "�ļ� ��: " << numNow << endl; //�ļ����� ���� ���� �޾Ҵ��� ���
			finish = judgeFin(first, second, goalNum);
			if (finish == true)break;
		}
		finish = false;
		cout << "*******************************1���� ���� *******************************" << endl;
		myScore = first->returnScore()[0]; // �� ���� ��ȯ
		yourScore = first->returnScore()[1]; // �� ���� ��ȯ
		int totalScore = myScore + yourScore;
		cout << "�߰� ���� ���� ��: ��� ���� " << myScore << " : " << yourScore << endl;
		second = new batSecond(whatTest, myScore, yourScore);
		delete first; //ó���÷����� �������� ptr
		first = nullptr;
		for (i = 0; i < goalNum; i++)
		{
			goal[i] = NULL;
		}
		//delete[]goal;//���� ���Ǵ� ��ǥ�� ����
		goalNum = 0;
		//���ο� ���� ���ο� ��ǥ�� ����
		goal = second->setTarget();
		//��ǥ�� ���� �� �����ʿ� ����,goal �� goalNum�����ؾߵ�/////////////
		//------------
		goalNum = second->getGoalNum();
		memset(temp, 0, bufsize); //temp�ʱ�ȭ
		memset(temp3, 0, bufsize);
		temp3[0] = 101, temp3[1] = -1;
		for (int i = 0; i < goalNum; i++) {
			swap(temp3[i + 1], temp3[i + 2]);
			temp3[i + 1] = goal[i];
		}
		encoding(tcp_temp, temp3);


		send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //goal ����

		cout << "���� ��ǥ���� ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}

		memset(temp, 0, bufsize); //temp�ʱ�ȭ
		Set_Clear(temp3);
		memset(tcp_temp, 0, sizeof(tcp_temp));
		recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //goal ����
		decoding(temp, tcp_temp);

		int Count1 = 0;
		while (temp[Count1++] != -1);
		goalNum = Count1 - 2;

		Check_goal(temp, goalNum); // �� �� �޾Ҵ��� üũ

		goal = new int[goalNum];

		for (int i = 0; i < goalNum; i++) {
			goal[i] = (int)temp[i + 1];
		} //temp������ �ٽ� goal�� �ٲ���


		second->getTarget(goal, goalNum);

		cout << "���� ��ǥ���� ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}
		cout << endl;
		while (finish == false)
		{
			Set_Clear(temp3);
			memset(temp2, 0, bufsize); //temp2�ʱ�ȭ
			recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //Ŭ���̾�Ʈ �� ����
			decoding(temp2, tcp_temp);
			numNow = temp2[1];

			Check_numNow(temp2); // �� �� �޾Ҵ��� üũ

			cout << "���� �� : " << numNow << endl;

			second->rx(numNow);
			cout << "�ļ� ��: " << numNow << endl; //�ļ����� ���� ���� �޾Ҵ��� ���
			finish = judgeFin(first, second, goalNum + totalScore);
			if (finish == true)break;
			numNow = second->tx(); //ó�� ���۽� ���� ����

			Set_Clear(temp3);
			memset(temp, 0, bufsize); //temp�ʱ�ȭ
			temp4[1] = numNow;
			encoding(tcp_temp, temp4);
			send(client_socket, tcp_temp, sizeof(tcp_temp), 0);
			cout << "���� �� : " << numNow << endl;

			finish = judgeFin(first, second, goalNum + totalScore); //���� �� ������ �������� ����
			if (finish == true)break;
		}
		finish = false;
		myScore = second->returnScore()[0]; // �� ���� ��ȯ
		yourScore = second->returnScore()[1]; // �� ���� ��ȯ
		cout << "������ ��: ��� ���� " << myScore << " : " << yourScore << endl;
		
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

	/////////////////////////////////////�ļ�
	else if (inString == "�ļ�")
	{
		second = new batSecond(whatTest, 0, 0);
		goal = second->setTarget();
		goalNum = second->getGoalNum();

		memset(temp, 0, bufsize); //temp�ʱ�ȭ
		Set_Clear(temp3);
		cout << goalNum << endl;
		for (int i = 0; i < goalNum; i++) {
			swap(temp3[i + 1], temp3[i + 2]);
			temp3[i + 1] = goal[i];
		}
		encoding(tcp_temp, temp3);
		send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //goal ����


		cout << "���� ��ǥ���� ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}
		memset(temp, 0, bufsize); //temp�ʱ�ȭ
		recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //goal ����
		decoding(temp, tcp_temp);

		int Count = 0;
		while (temp[Count++] != -1);  //goalNum���
		//Count--;
		goalNum = Count - 2;

		Check_goal(temp, goalNum); // �� �� �޾Ҵ��� üũ

		goal = new int[goalNum];

		for (int i = 0; i < goalNum; i++) {
			goal[i] = temp[i + 1];
		} //temp2������ �ٽ� goal�� �ٲ���

		second->getTarget(goal, goalNum);
		cout << "���� ��ǥ���� ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}
		//1����
		while (finish == false)
		{
			memset(temp2, 0, bufsize); //temp�ʱ�ȭ
			recv(client_socket, tcp_temp, sizeof(tcp_temp), 0); //������ �� ����
			decoding(temp2, tcp_temp);
			numNow = temp2[1];

			Check_numNow(temp2); // �� �� �޾Ҵ��� üũ

			cout << "���� �� : " << numNow << endl;

			second->rx(numNow); //���⿡ ����� ���� ��� ���� �Ǵ� ����
			cout << "�ļ� ��: " << numNow << endl; //�ļ����� ���� ���� �޾Ҵ��� ���
			finish = judgeFin(first, second, goalNum);
			if (finish == true)break;

			numNow = second->tx(); //ó�� ���۽� ���� ����
			memset(temp, 0, bufsize); //temp�ʱ�ȭ
			temp4[1] = numNow;
			encoding(tcp_temp, temp4);
			send(client_socket, tcp_temp, sizeof(tcp_temp), 0);

			cout << "���� �� : " << numNow << endl;
			//���� �� ������ �������� ����
			finish = judgeFin(first, second, goalNum);
			if (finish == true)break;
		}
		finish = false;
		//1���� ����*********************************
		myScore = second->returnScore()[0]; // �� ���� ��ȯ
		yourScore = second->returnScore()[1]; // �� ���� ��ȯ
		int totalScore = myScore + yourScore;
		cout << "�߰� ���� ���� ��: ��� ���� " << myScore << " : " << yourScore << endl;
		first = new batFirst(whatTest, myScore, yourScore);//������ ����
		delete second;//ó���÷����� �ļ� ���� ptr
		second = nullptr;
		for (i = 0; i < goalNum; i++)
		{
			goal[i] = NULL;
		}
		//delete[]goal;
		goalNum = 0;

		//2���� ����
		memset(temp, 0, bufsize); //send_buf�ʱ�ȭ
		Set_Clear(temp3);
		recv(client_socket, tcp_temp, sizeof(tcp_temp), 0);
		decoding(temp, tcp_temp);
		int Count2 = 0;
		while (temp[Count2++] != -1);
		goalNum = Count2 - 2;

		Check_goal(temp, goalNum); // �� �� �޾Ҵ��� üũ

		goal = new int[goalNum];

		cout << "���� ��ǥ���� ";
		for (int i = 0; i < goalNum; i++) {
			goal[i] = temp[i + 1];
			cout << goal[i] << " ";
		} //temp2������ �ٽ� goal�� �ٲ���
		cout << endl;

		first->getTarget(goal, goalNum);// ��ǥ�� �����ʿ�

		goal = first->setTarget();//��ǥ�� ���� �� ��ȯ �� goalNum���� �ʿ�
		goalNum = first->getGoalNum();

		cout << goalNum << " : ";
		Set_Clear(temp3);
		memset(temp, 0, bufsize); //temp�ʱ�ȭ
		for (int i = 1; i <= goalNum; i++) {
			temp3[i + 1] = temp3[i];
			temp3[i] = goal[i - 1];
		}
		encoding(tcp_temp, temp3);
		send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //goal ����


		cout << "���� ��ǥ���� ";
		for (i = 0; i < goalNum; i++)
		{
			cout << goal[i] << " ";
		}
		cout << endl;
		while (finish == false)
		{
			numNow = first->tx(); //ó�� ���۽� ���� ����
			memset(temp2, 0, bufsize); //temp�ʱ�ȭ
			temp4[1] = numNow;
			encoding(tcp_temp, temp4);
			send(client_socket, tcp_temp, sizeof(tcp_temp), 0); //numNow ����

			cout << "�ļ� �� : " << numNow << endl;

			finish = judgeFin(first, second, goalNum + totalScore);
			if (finish == true)break;

			memset(temp, 0, bufsize); //temp�ʱ�ȭ
			recv(client_socket, tcp_temp, sizeof(tcp_temp), 0);
			decoding(temp, tcp_temp);
			numNow = temp[1];

			Check_numNow(temp); // �� �� �޾Ҵ��� üũ

			cout << "���� �� : " << numNow << endl;

			first->rx(numNow); //���⿡ ����� ���� ��� ���� �Ǵ� ����
			finish = judgeFin(first, second, goalNum + totalScore);
			if (finish == true)break;
		}
		finish = false;
		myScore = first->returnScore()[0]; // �� ���� ��ȯ
		yourScore = first->returnScore()[1]; // �� ���� ��ȯ
		cout << "������ ��: ��� ���� " << myScore << " : " << yourScore << endl;
		
		memset(temp, 0, bufsize); //temp�ʱ�ȭ
		memset(tcp_temp, 0, bufsize); //temp�ʱ�ȭ
		recv(client_socket, tcp_temp, sizeof(tcp_temp), 0);
		decoding(temp, tcp_temp);

		Check_win(temp);

	}
}

bool judgeFin(batFirst* first, batSecond* second, int goalNum)
{
	if (second == nullptr)
	{
		if (first->returnScore()[0] + first->returnScore()[1] >= (goalNum))//�� ���ھ��� ���� ��ǥ ���� �Ѿ�� 1�� ���� ��
		{
			cout << "��������" << first->returnScore()[0] << ":" << first->returnScore()[1];
			return true;
		}
	}
	else if (first == nullptr)
	{
		if (second->returnScore()[0] + second->returnScore()[1] >= (goalNum))//�� ���ھ��� ���� ��ǥ ���� �Ѿ�� 1�� ���� ��
		{
			cout << "�ļ�����" << second->returnScore()[0] << ":" << second->returnScore()[1];
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
		//cout << "��Ģ�� �°� ��ŵǾ����ϴ�" << endl;
	}
	else {
		cout << "��Ģ�� ���� �ʰ� ��ŵǾ����ϴ�" << endl;
	}
}

void Check_numNow(char* temp) {
	if (temp[0] == 102 && temp[2] == -1) {
		//cout << "��Ģ�� �°� ��ŵǾ����ϴ�" << endl;
	}
	else {
		cout << "��Ģ�� ���� �ʰ� ��ŵǾ����ϴ�" << endl;
	}
}

void Check_win(char* temp) {
	if (temp[0] == 103 && temp[1] == -1) {
		cout << "����� �����ϴ�." << endl;
	}
	else if (temp[0] == 104 && temp[1] == -1) {
		cout << "����� �̰���ϴ�" << endl;
	}
	else if (temp[0] == 105 && temp[1] == -1) {
		cout << "�����ϴ�." << endl;
	}
	else {
		cout << "����";
	}
}