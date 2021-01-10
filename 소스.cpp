#include<iostream>
#include<boost/asio.hpp>
using namespace std;
using namespace boost;
using namespace asio;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

int main()
{
	io_service io_service; // I/O �̺�Ʈ�� OS���� ����ġ�ϴ� ��ü
	ip::tcp::endpoint endpoint(ip::address::from_string(SERVER_IP), PORT_NUMBER); // ������ ������ IP(char*)�� ��Ʈ ����

	system::error_code connect_error; // ���� Ŭ����
	ip::tcp::socket socket(io_service); // Ŭ���̾�Ʈ�� ����(����)
	socket.connect(endpoint, connect_error); // connect : endpoint ������ ���� ����(connect_error�� ���� �����Ͽ� ���� ���� �ľ�)

	if (connect_error) // ���� ó��
	{
		cout << "���� ���� error No: " << connect_error.value() << ", Message : " << connect_error.message() << endl;
		getchar();
	}
	else
	{
		cout << "������ ���� ����" << endl; // ���Ͽ� ����(connect) �Ǵ� �����͸� �ްų� ���۵� ��(write) �ݵ�� error_code ���ڰ� ���ݵȴ�.
	}

	for (int i = 0; i < 7; ++i)
	{
		//������ �߽�
		char szMessage[128] = { 0, };
		sprintf_s(szMessage, 128 - 1, "%d - Send Message", i); // szMessage�� "i - Send Message" ���� ����
		int nMsgLen = strnlen_s(szMessage, 128 - 1); // ���̵� ���

		system::error_code ignored_error;
		socket.write_some(buffer(szMessage, nMsgLen), ignored_error); // szMessage ���� nMsgLen ���� ��ŭ ������ ����
		
		cout << "������ ���� �޽��� : " << szMessage << endl;

		// ������ ����
		std::array<char, 128> buf;
		buf.assign(0);
		system::error_code error;
		size_t len = socket.read_some(buffer(buf), error); // buf�� �������� ���� �����͸� ����(len�� ���� ������ ����)

		if (error)
		{
			if (error == error::eof)
			{
				cout << "������ ������ ���������ϴ�." << endl;
			}
			else
			{
				cout << "error No: " << error.value() << " error Message : " << error.message().c_str() << endl;
			}
			break;
		}

		cout << "�����κ��� ���� �޽��� : " << &buf[0] << endl; // �����κ��� ������ �޽��� ���
	}

	if (socket.is_open()) // for ��(�����۾�)�� ������ ���
	{
		socket.close(); // ���� �ݱ�
	}
}