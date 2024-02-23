#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include "KCSApiWapper.h"
#include "pugixml.hpp"

#include <iostream>
#include <fstream>  // 이 헤더를 추가해 주세요
#include <windows.h>

// INI 파일 경로
const char* iniFilePath = ".\\config.ini";


// 주기적으로 실행될 작업을 수행하는 함수
void performTask(const char* logFilePath) {
    // Your periodic task goes here
    std::cout << "#################1" << std::endl;
    std::cout << logFilePath << std::endl;
    std::ofstream logFile(logFilePath, std::ios::out | std::ios::app);

     // 파일이 열리는지 확인
    if (!logFile.is_open()) {
        std::cerr << "Failed to open or create the log file." << std::endl;
        return;
    }


    logFile << "Service is running." << std::endl;
    logFile.close();
}

// 서비스 메인 함수
void serviceMain(int argc, char** argv) {
    // INI 파일에서 로그 파일 경로 읽어오기
    std::cout << "################# JASPER SERVICE MAIN #################" << std::endl;
    char logFilePath[MAX_PATH];
    GetPrivateProfileString("Settings", "LogFilePath", "", logFilePath, MAX_PATH, iniFilePath);
    std::cout << "#################" << std::endl;
    std::cout << logFilePath << std::endl;
    std::cout << "#################" << std::endl;
    

    // 서비스가 시작되면 주기적으로 작업을 수행
    while (true) {
        performTask(logFilePath);

        // 주기적으로 실행될 간격 (여기서는 10초로 설정)
        Sleep(10000);
    }
}

// 서비스 제어 핸들러 함수
void serviceControlHandler(DWORD control) {
    // 서비스 제어 명령을 처리하는 로직을 추가할 수 있음
}

// 서비스 진입점
int main() {
    // 서비스 이름
    const char* serviceName = "D_JASPER";
    std::cout << "################# JASTER DEMON START #################" << std::endl;


    std::cout << "################# JASTER SERVICE LOAD #################" << std::endl;

    // 서비스 시작시에 호출되는 함수를 지정
    SERVICE_TABLE_ENTRY serviceTable[] = {
        { (LPSTR)serviceName, (LPSERVICE_MAIN_FUNCTION)serviceMain },
        { NULL, NULL }
    };

    std::cout << "################# JASTER SERVICE LOAD END #################" << std::endl;

    // 서비스를 시작
    if (!StartServiceCtrlDispatcher(serviceTable)) {
        std::cout << serviceTable << std::endl;
        std::cerr << "Failed to start service. Error code: " << GetLastError() << std::endl;
        return GetLastError();
    }

    return 0;
}

// int main2() {

//     const char* userId = "jasper";
//     const char* docNo = "VC123456789001";


//     std::cout << "start" << std::endl;
//     //  함수타입 정의
//     typedef char* (*Func_RcpnDocCscl)(char*, char*, char*, char*);
//     typedef char* (*Func_RcpnDocReqApre)(char*, char*, char*, char*);
//     typedef char* (*Func_RcpnDocLstCscl)(char*, char*);
//     typedef char* (*Func_RcpnDocLstReqApre)(char*, char*);
//     typedef char* (*Func_TrsmDocCscl)(char*, char*, char*, char*, char*);
//     typedef char* (*Func_TrsmDocReqApre)(char*, char*, char*, char*, char*, char*);
//     typedef char* (*Func_NoticeInspectionObject)(char*, char*, char*, char*, long);
//     typedef char* (*Func_GetSrvrInfo)(const char*, const char*);
//     typedef char* (*Func_LoginSecuMdle)(const char*, const char*);
//     typedef int (*Func_LogoutSecuMdle)();
//     // 호출 함수 정의
//     Func_RcpnDocCscl RcpnDocCscl = NULL;
//     Func_RcpnDocReqApre RcpnDocReqApre = NULL;
//     Func_RcpnDocLstCscl RcpnDocLstCscl = NULL;
//     Func_RcpnDocLstReqApre RcpnDocLstReqApre = NULL;
//     Func_TrsmDocCscl TrsmDocCscl = NULL;
//     Func_TrsmDocReqApre TrsmDocReqApre = NULL;
//     Func_GetSrvrInfo GetSrvrInfo = NULL;
//     Func_LoginSecuMdle LoginSecuMdle = NULL;
//     Func_LogoutSecuMdle LogoutSecuMdle = NULL;

//     initLib();


//     HINSTANCE hDll = LoadLibrary(_T("./KCSAPI4.dll"));
//     std::cout << hDll << std::endl;
//     if (hDll != NULL) {
//         std::cout << "DLL loaded successfully." << std::endl;
//         // DLL이 로드되었다면 이후 작업 수행


//          std::cout << "========== hDll IN  ===========" << std::endl;
//         RcpnDocCscl = (Func_RcpnDocCscl)GetProcAddress(hDll, "RcpnDocCscl");
//         RcpnDocReqApre = (Func_RcpnDocReqApre)GetProcAddress(hDll, "RcpnDocReqApre");
//         RcpnDocLstCscl = (Func_RcpnDocLstCscl)GetProcAddress(hDll, "RcpnDocLstCscl");
//         RcpnDocLstReqApre = (Func_RcpnDocLstReqApre)GetProcAddress(hDll, "RcpnDocLstReqApre ");
//         TrsmDocCscl = (Func_TrsmDocCscl)GetProcAddress(hDll, "TrsmDocCscl");
//         TrsmDocReqApre = (Func_TrsmDocReqApre)GetProcAddress(hDll, "TrsmDocReqApre");
//         GetSrvrInfo = (Func_GetSrvrInfo)GetProcAddress(hDll, "GetSrvrInfo");
//         LoginSecuMdle = (Func_LoginSecuMdle)GetProcAddress(hDll, "LoginSecuMdle");
//         LogoutSecuMdle = (Func_LogoutSecuMdle)GetProcAddress(hDll, "LogoutSecuMdle");
//         std::cout << "========== hDll END  ===========" << std::endl;
//         // 함수 호출 후 DLL 로드를 해제한다.


//         char* sReturn2;
//         char* sReturn;
        
//         sReturn = GetSrvrInfo(userId,docNo);
//         sReturn2 = LoginSecuMdle(userId, docNo);
//         // sReturn = TrsmDocCscl("testUser", "VC123456789001", "GOVCBR929", "123451600001U", "C:\\KCSAPI4\\MetaDoc.xml");
//         // sReturn = TrsmDocReqApre("testUser", "VC123456789001", "VC123456789002", "GOVCBR929", "123451600001U", "C:\\KCSAPI4\\MetaDoc.xml");
//         // sReturn = RcpnDocLstCscl("testUser", "VC123456789001");
//         // sReturn = RcpnDocCscl("testUser", "VC123456789001", "GOVCBR929", "123451600001U");
//         // sReturn = RcpnDocLstReqApre("testUser", "VC123456789001");
//         // sReturn = RcpnDocReqApre("testUser", "VC123456789001", "GOVCBR929", "123451600001U");
//         // sReturn = LoginSecuMdle ("testUser", "VC123456789001");
//         // LoginSecuMdle();


//         std::cout << sReturn << std::endl;
//         std::cout << sReturn2 << std::endl;

        
//         std::cout << "========== logout ===========" << std::endl;
//         LogoutSecuMdle();
//         FreeLibrary(hDll);  // DLL 언로드
//     } else {
//         DWORD error = GetLastError();
//         std::cout << "Failed to load DLL. Error code: " << error << std::endl;
//     }

//     while(1){
        
//     }
    
//     std::cout << hDll << std::endl;
   


//     std::cout << "========== End ===========" << std::endl;
//     return 0;
// }