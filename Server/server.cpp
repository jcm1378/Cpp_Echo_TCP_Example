#include <iostream>
#include <string>
#include <WinSock2.h> //winSock �ַ��-> �Ӽ� -> �����Ӽ� ->��Ŀ-> �Է�-> �߰� ���Ӽ� ->ws2_32.lib  ����� �� �ҷ��� ��

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[�����߻�]" << message << '\n'; //�Է¹��� �޽����� ������� ��
	system("pause");
	exit(1); //�����߸� �׳� �������� ����
}

int main(void)
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket; //���� ���ϰ� Ŭ���̾�Ʈ ���� ��ü ����
	SOCKADDR_IN serverAddress, clientAddress; //���� ���� ��巹�� ���

	string sent;

	int serverPort = 9876; //��Ʈ ����
	char received[256]; //�Է� ����ũ�� (ä��ĥ��)

	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) //WSAStartup(MAKEWORD(����),wsaData����) != 0 �ΰɷ� ���� 1�� ��ȯ�ϸ� �۵��� �ȵǵ��� ��������
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP���ϻ��� soket(�ּ�ü�� = IPv4=AF_INET, IPv6=AF_INET6//��! �ʱ�ȭ �Ҷ��� AF(��巡�� �йи�)��� PF(���������йи�)����, ����Ÿ�� TCP (Sock_STREAM), UDP�� (SOCK_DGRAM), �������� =0�� ����)

	if (serverSocket == INVALID_SOCKET) { //VALID ��ȿ�� INVALID ��ȿ���� ������
		ShowErrorMessage("soket()"); //���� �ٷ� ����ϰ� �ؤ��� ��ŵ
	}
	memset(&serverAddress, 0, sizeof(serverAddress)); //���� ��巹�� ������ �ʱ�ȭ ���ִ� ����
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);// htonl��  l�� 4����Ʈ�� �ǹ� long
	serverAddress.sin_port = htons(serverPort); //s�� short�ǹ� 2����Ʈ ���� ����Ʈ��������

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[�������] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[���� ����] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) {//Ŭ���� ����� �Ӹ޽����� �޾Ƽ� �״�� �ٽ�����
		
		int length = recv(clientSocket, received, sizeof(received) -1, 0);
		received[length] = '\0';

		cout << "[Ŭ���� ��Ʈ �޽���] : " << received << '\n';

		cout << "[�޽��� ����]: ";
		getline(cin, sent);
		if (sent == "")continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);


		//cout << "[�޽��� ����]: " <<  received << '\n';

		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[���� ����]\n";
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

