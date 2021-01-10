#include<iostream>
#include<boost/asio.hpp>
using namespace std;
using namespace boost;
using namespace asio;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

int main()
{
	io_service io_service; // I/O 이벤트를 OS에서 디스패치하는 객체
	ip::tcp::endpoint endpoint(ip::address::from_string(SERVER_IP), PORT_NUMBER); // 접속할 서버의 IP(char*)와 포트 설정

	system::error_code connect_error; // 에러 클래스
	ip::tcp::socket socket(io_service); // 클라이언트의 소켓(정보)
	socket.connect(endpoint, connect_error); // connect : endpoint 서버에 접속 시작(connect_error의 값을 조사하여 실패 여부 파악)

	if (connect_error) // 에러 처리
	{
		cout << "연결 실패 error No: " << connect_error.value() << ", Message : " << connect_error.message() << endl;
		getchar();
	}
	else
	{
		cout << "서버에 연결 성공" << endl; // 소켓에 연결(connect) 또는 데이터를 받거나 전송될 땐(write) 반드시 error_code 인자가 수반된다.
	}

	for (int i = 0; i < 7; ++i)
	{
		//데이터 발신
		char szMessage[128] = { 0, };
		sprintf_s(szMessage, 128 - 1, "%d - Send Message", i); // szMessage에 "i - Send Message" 값을 대입
		int nMsgLen = strnlen_s(szMessage, 128 - 1); // 길이도 계산

		system::error_code ignored_error;
		socket.write_some(buffer(szMessage, nMsgLen), ignored_error); // szMessage 값을 nMsgLen 길이 만큼 데이터 전송
		
		cout << "서버에 보낸 메시지 : " << szMessage << endl;

		// 데이터 수신
		std::array<char, 128> buf;
		buf.assign(0);
		system::error_code error;
		size_t len = socket.read_some(buffer(buf), error); // buf에 서버에서 보낸 데이터를 읽음(len은 읽은 데이터 길이)

		if (error)
		{
			if (error == error::eof)
			{
				cout << "서버와 연결이 끊어졌습니다." << endl;
			}
			else
			{
				cout << "error No: " << error.value() << " error Message : " << error.message().c_str() << endl;
			}
			break;
		}

		cout << "서버로부터 받은 메시지 : " << &buf[0] << endl; // 서버로부터 수신한 메시지 출력
	}

	if (socket.is_open()) // for 문(전송작업)이 끝났을 경우
	{
		socket.close(); // 소켓 닫기
	}
}