#include <windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <filesystem>
#include <thread>
#include <tchar.h>
#include <filesystem>
namespace fs = std::filesystem;
/**
 * 분당 처리할 수 있는 시간이 있어서 해당 프로그램은 시간 베이스로 동작하도록 한다.
 * 
 * 신고서 송신은 초당 20건 이하 / 분당 300건 이하로 전송 처리 한다.
 * 문서 수신중 C901 코드를 받으면 30초 이후에 요청가능
 * 통보서 리스트가 없는 경우 업체에서 통보서 리스트 요청 후 리스트 미수신 시 30초 후에 리스트 요청한다.
*/

const std::string logFilePath = "log.txt";
const std::string monitoredFolderPath = "C:\\JASPER\\XML"; // 감시할 폴더 경로

// const char* userId = "testUser";
const char* userId = "jasper";
// const char* docNo = "VC123456789001";
const char* docNo = "VC595860327001";

HINSTANCE hDll = LoadLibrary(_T("./KCSAPI4.dll"));
char* sReturn;
const char* authValue = "C200";
bool auth = false;
int count = 0;
const int DEF_SLEEP_VALUE = 25000;
int sleepVal = DEF_SLEEP_VALUE;


//  함수타입 정의
typedef char* (*Func_RcpnDocCscl)(char*, char*, char*, char*);
typedef char* (*Func_RcpnDocReqApre)(char*, char*, char*, char*);
typedef char* (*Func_RcpnDocLstCscl)(const char*, const char*);
typedef char* (*Func_RcpnDocLstReqApre)(char*, char*);


typedef char* (*Func_TrsmDocCscl)(const char*, const char*, char*, char*, char*); //사용자ID, 사용자 문서함번호, 문서코드, 제출번호, 송신XML파일명
typedef char* (*Func_TrsmDocReqApre)(char*, char*, char*, char*, char*, char*);
typedef char* (*Func_NoticeInspectionObject)(char*, char*, char*, char*, long);
typedef char* (*Func_GetSrvrInfo)(const char*, const char*);
typedef char* (*Func_LoginSecuMdle)(const char*, const char*);
typedef int (*Func_LogoutSecuMdle)();


// 호출 함수 정의
Func_RcpnDocCscl RcpnDocCscl = NULL;
Func_RcpnDocReqApre RcpnDocReqApre = NULL;
Func_RcpnDocLstCscl RcpnDocLstCscl = NULL;
Func_RcpnDocLstReqApre RcpnDocLstReqApre = NULL;
Func_TrsmDocCscl TrsmDocCscl = NULL;
Func_TrsmDocReqApre TrsmDocReqApre = NULL;
Func_GetSrvrInfo GetSrvrInfo = NULL;
Func_LoginSecuMdle LoginSecuMdle = NULL;
Func_LogoutSecuMdle LogoutSecuMdle = NULL;



void RedirectIOToConsole() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout.clear();
}

void LogToFile(const std::string& message) {
    // 현재 시간 구하기
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    // 시간을 문자열로 변환
    char timeString[20]; // 충분한 크기로 배열 할당
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);

    // 로그 파일에 메시지와 시간 기록
    std::ofstream logFile(logFilePath, std::ios::app);
    if (logFile.is_open()) {
        logFile << "[" << timeString << "] " << message << std::endl;
        logFile.close();
    }
}

// 폴더 모니터링
void getFileList() {
    std::string directoryPath = "C:/your/directory/path";
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (fs::is_regular_file(entry)) {
            std::cout << entry.path().filename() << std::endl;
        }
    }
}

void runTimeWrite(){
    time_t rawtime;
    struct tm timeinfo;
    char buffer[80];
    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::string currentTime(buffer);
    LogToFile("Program is running. Current time: " + currentTime);
    std::cout << "Program is running. Current time: " << currentTime << std::endl;
}

void getSleepValue(){


}

void RunProgram() {
    

    while (true) {
        if(!auth){
            LoginSecuMdle(userId, docNo);
        }else{
            count++;
            runTimeWrite();
            std::cout << "Program is running. Current count: " << count << std::endl;

            // getFileList();

            sReturn = GetSrvrInfo(userId, docNo);
            std::cout << sReturn << std::endl;

            // sReturn = RcpnDocLstCscl(userId, docNo);
            // sReturn = TrsmDocCscl(userId, docNo,"GOVCBR936","AE00030000002134","C:\\KCSAPI4\\GOVCBR936_1.xml");
            std::cout << sReturn << std::endl;
        }

  


        //값이 설정되지 않았으면 기본값으로 설정
        Sleep(sleepVal);
    }
}


bool authentication(){
    //인증 요청
    sReturn = LoginSecuMdle(userId,docNo);
    if (strstr(sReturn, authValue) != NULL) {
        // sReturn이 "C200 : 정상처리"와 같은 경우
        LogToFile("sReturn is equal to comparisonValue");
        // std::cout << "sReturn is equal to comparisonValue" << std::endl;
        return true;
    } else {
        // sReturn이 "C200 : 정상처리"와 다른 경우
        LogToFile("sReturn is not equal to comparisonValue");
        // std::cout << "sReturn is not equal to comparisonValue" << std::endl;
        return false;
    }
}

void unAuthentication(){
    //인증 해제
    LogoutSecuMdle();
}

void init() {

    const char* iniFilePath = ".\\config.ini";

    // INI 파일에서 로그 파일 경로 읽어오기
    std::cout << "################# JASPER SERVICE MAIN #################" << std::endl;
    char logFilePath[MAX_PATH];
    GetPrivateProfileString("Settings", "LogFilePath", "", logFilePath, MAX_PATH, iniFilePath);

    // INI 파일에서 프로그램 SLEEP 동작시간 설정
    sleepVal = GetPrivateProfileInt("Settings", "SleepTime", DEF_SLEEP_VALUE, iniFilePath);
    std::string sleepTimeString = "SET SLEEP VALUE : "+std::to_string(sleepVal);
    LogToFile(sleepTimeString);


    std::string logPathPrefix = "SET LOG PATH : ";
    std::string fullPath = logPathPrefix + logFilePath;
    LogToFile(fullPath);

    
    LogToFile("JASPER KCSAPI4 INIT ...");
    std::cout << hDll << std::endl;
    if (hDll != NULL) {
        std::string dellLoadSuccess = "[INFO] DLL loaded successfully.";
        LogToFile(dellLoadSuccess);

        // DLL이 로드되었다면 이후 작업 수행
        RcpnDocCscl = (Func_RcpnDocCscl)GetProcAddress(hDll, "RcpnDocCscl");
        RcpnDocReqApre = (Func_RcpnDocReqApre)GetProcAddress(hDll, "RcpnDocReqApre");
        RcpnDocLstCscl = (Func_RcpnDocLstCscl)GetProcAddress(hDll, "RcpnDocLstCscl");
        RcpnDocLstReqApre = (Func_RcpnDocLstReqApre)GetProcAddress(hDll, "RcpnDocLstReqApre ");
        TrsmDocCscl = (Func_TrsmDocCscl)GetProcAddress(hDll, "TrsmDocCscl");
        TrsmDocReqApre = (Func_TrsmDocReqApre)GetProcAddress(hDll, "TrsmDocReqApre");
        GetSrvrInfo = (Func_GetSrvrInfo)GetProcAddress(hDll, "GetSrvrInfo");
        LoginSecuMdle = (Func_LoginSecuMdle)GetProcAddress(hDll, "LoginSecuMdle");
        LogoutSecuMdle = (Func_LogoutSecuMdle)GetProcAddress(hDll, "LogoutSecuMdle");
        // 함수 호출 후 DLL 로드를 해제한다.
        auth = authentication();
        std::cout << sReturn << std::endl;
        // std::cout << sReturn2 << std::endl;
        // FreeLibrary(hDll);  // DLL 언로드
    } else {
        DWORD error = GetLastError();
        std::cout << "Failed to load DLL. Error code: " << error << std::endl;
    }
    
    std::cout << hDll << std::endl;
    std::cout << "========== End ===========" << std::endl;
}

int main() {


    init();
    RedirectIOToConsole();
    RunProgram();
    FreeConsole();
 
    return 0;
}


