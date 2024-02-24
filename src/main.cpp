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

void SimulateTabKeyPress() {
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;

    // Press Tab key
    input.ki.wVk = VK_TAB;
    input.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &input, sizeof(INPUT));

    // Release Tab key
    input.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &input, sizeof(INPUT));
}

void SendTextInput(const char* text) {
    INPUT inputs[2];
    memset(inputs, 0, sizeof(inputs));

    // 키 다운
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 0; // 가상 키 코드는 사용하지 않음
    inputs[0].ki.dwFlags = KEYEVENTF_UNICODE; // 유니코드 문자열 사용
    inputs[0].ki.wScan = text[0]; // 첫 번째 문자

    // 키 업
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 0;
    inputs[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP; // 키를 눌렀다가 떼기
    inputs[1].ki.wScan = text[0];

    // 입력 보내기
    SendInput(2, inputs, sizeof(INPUT));
}

void SendArrowKeyInput(WORD arrowKeyCode) {
    INPUT inputs[2];
    memset(inputs, 0, sizeof(inputs));

    // 키 다운
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = arrowKeyCode; // 방향키의 가상 키 코드
    inputs[0].ki.dwFlags = 0; // 키를 누름

    // 키 업
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = arrowKeyCode;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 키를 떼기

    // 입력 보내기
    SendInput(2, inputs, sizeof(INPUT));
}


void SetFocusToWindowWithTitle(const wchar_t* windowTitle) {
    Sleep(1000);
    HWND windowHandle = FindWindowW(NULL, windowTitle);
    if (windowHandle != NULL) {
        // Bring the window to the foreground
        ShowWindow(windowHandle, SW_SHOWNORMAL);
        SetForegroundWindow(windowHandle);
        SimulateTabKeyPress();
        SimulateTabKeyPress();
        SimulateTabKeyPress();
        SimulateTabKeyPress();
        SimulateTabKeyPress();
        SendArrowKeyInput(VK_DOWN);
        SimulateTabKeyPress();
        SimulateTabKeyPress();
        SimulateTabKeyPress();

         // 입력할 텍스트
        const char* textToType = "jasper002!";

        // // 텍스트 입력
        for (int i = 0; textToType[i] != '\0'; ++i) {
            SendTextInput(&textToType[i]);
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 입력 간격
        }
        //  // 마지막으로 Enter 키 입력
        // const char* enterKey = "\r";
        // SendTextInput(enterKey);
        
        
    } else {
        std::wcout << L"Window with title \"" << windowTitle << L"\" not found." << std::endl;
    }
}



bool authentication(){
    const wchar_t* windowTitle = L"인증서 선택";
    // SetFocusToWindowWithTitle 함수를 비동기적으로 호출

    //인증 요청  
    std::thread focusThread([windowTitle]() {
        SetFocusToWindowWithTitle(windowTitle);
    });
    sReturn = LoginSecuMdle(userId,docNo);
    focusThread.join();
    LogToFile(sReturn);

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


