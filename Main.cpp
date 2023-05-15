#pragma comment(lib,"ws2_32.lib")
#include "RoboCatPCH.h"

//#if _WIN32
//int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
//{
//	UNREFERENCED_PARAMETER( hPrevInstance );
//	UNREFERENCED_PARAMETER( lpCmdLine );
//	
//}
//#else
//const char** __argv;
//int __argc;
int main(int argc, const char** argv)
{
    SocketUtil com;
    com.StaticInit();
    TCPSocketPtr listenSocket = com.CreateTCPSocket(INET);
    SocketAddressPtr sap = make_shared<SocketAddress>(INADDR_ANY, 8000);
    if (listenSocket->Bind(*sap) != NO_ERROR)
        return -1;
    if (listenSocket->Listen() != NO_ERROR)
        return -1;

    SocketAddressPtr cap = make_shared<SocketAddress>();
    auto newClient = listenSocket->Accept(*cap);

    RoboCat* RoboCatPtr = new RoboCat();
    MemoryStream* stream = new OutputMemoryStream;

    // 반복문을 통해 메모리 스트림에 RoboCat 객체를 직렬화
    //루프는 RoboCat::sDataType에서 정의된 MemberVariable들의 목록을 반복하며, 각 멤버 변수에 대한 switch 문을 실행
    for (const auto& mv : RoboCat::sDataType->GetMemberVariables())
    {
        switch (mv.GetPrimitiveType())
        {
        case EPT_Int:
        {
            uint32_t* intValue = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(RoboCatPtr) + mv.GetOffset());
            stream->Serialize(*intValue);
            break;
        }
        case EPT_Float:
        {
            float* floatValue = reinterpret_cast<float*>(reinterpret_cast<char*>(RoboCatPtr) + mv.GetOffset());
            stream->Serialize(*floatValue);
            break;
        }
        case EPT_String:
        {
            std::string* stringValue = reinterpret_cast<std::string*>(reinterpret_cast<char*>(RoboCatPtr) + mv.GetOffset());
            stream->Serialize(*stringValue);
            break;
        }
        default:
            break;
        }
    }

    newClient->Send(stream->GetBufferPtr(), stream->GetLength());
    getchar();
}

//#endif