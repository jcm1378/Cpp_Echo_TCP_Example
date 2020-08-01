#include <iostream>
#include <string>
#include <WinSock2.h> //winSock 솔루션-> 속성 -> 구성속성 ->링커-> 입력-> 추가 종속성 ->ws2_32.lib  등록한 뒤 불러서 씀

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[오류발생]" << message << '\n'; //입력받은 메시지가 오류라는 뜻
	system("pause");
	exit(1); //에러뜨면 그냥 꺼버리게 만듬
}

int main(void)
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket; //서버 소켓과 클라이언트 소켓 객체 생성
	SOCKADDR_IN serverAddress, clientAddress; //서버 연결 어드레스 방식

	string sent;

	int serverPort = 9876; //포트 변수
	char received[256]; //입력 버퍼크기 (채팅칠때)

	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) //WSAStartup(MAKEWORD(버전),wsaData변수) != 0 인걸로 봐서 1을 반환하면 작동이 안되도록 설정했음
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP소켓생성 soket(주소체계 = IPv4=AF_INET, IPv6=AF_INET6//단! 초기화 할때는 AF(어드래스 패밀리)대신 PF(프로토콜패밀리)쓴다, 소켓타입 TCP (Sock_STREAM), UDP는 (SOCK_DGRAM), 프로토콜 =0이 보통)

	if (serverSocket == INVALID_SOCKET) { //VALID 유효한 INVALID 유효하지 않으면
		ShowErrorMessage("soket()"); //에러 바로 재생하고 멈ㅊ게 시킵
	}
	memset(&serverAddress, 0, sizeof(serverAddress)); //서버 어드레스 변수를 초기화 해주는 과정
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);// htonl의  l이 4바이트를 의미 long
	serverAddress.sin_port = htons(serverPort); //s는 short의미 2바이트 정수 바이트형식으로

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) {//클라이 어느으 ㅣ메시지를 받아서 그대로 다시전달
		
		int length = recv(clientSocket, received, sizeof(received) -1, 0);
		received[length] = '\0';

		cout << "[클라이 언트 메시지] : " << received << '\n';

		cout << "[메시지 전송]: ";
		getline(cin, sent);
		if (sent == "")continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);


		//cout << "[메시지 전송]: " <<  received << '\n';

		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[서버 종료]\n";
			break;
		}
		// send(clientSocket, received, sizeof(received) - 1, 0);
		
	}
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;

}

