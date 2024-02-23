#include <iostream>
#include <Windows.h>
#include <tchar.h>
// 이벤트 베이스
void Log(const std::wstring& message) {
    std::wcout << message << std::endl;
}


// void LogToFile(const std::string& message) {
//     std::ofstream logFile(logFilePath, std::ios::app);
//     if (logFile.is_open()) {
//         logFile << message << std::endl;
//         logFile.close();
//     }
// }


void MonitorDirectoryChanges(const std::wstring& directoryPath) {
    while (true) {
        HANDLE hDir = CreateFileW(
            directoryPath.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS,
            NULL
        );

        if (hDir == INVALID_HANDLE_VALUE) {
            std::wcerr << L"Failed to open directory: " << directoryPath << std::endl;
            return;
        }

        BYTE buffer[4096];
        DWORD bytesReturned;
        OVERLAPPED overlapped = { 0 };

        BOOL success = ReadDirectoryChangesW(
            hDir,
            buffer,
            sizeof(buffer),
            FALSE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES |
            FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_LAST_ACCESS |
            FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY,
            &bytesReturned,
            &overlapped,
            NULL
        );

        if (!success) {
            std::wcerr << L"Failed to read directory changes." << std::endl;
            break;
        }

        FILE_NOTIFY_INFORMATION* fileInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);
        do {
            // Process change
            if (fileInfo->Action == FILE_ACTION_ADDED) {
                std::wstring fileName(fileInfo->FileName, fileInfo->FileNameLength / sizeof(wchar_t));
                std::wstring filePath = directoryPath + L"\\" + fileName;
                Log(L"File created: " + filePath);
            }

            fileInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                reinterpret_cast<BYTE*>(fileInfo) + fileInfo->NextEntryOffset
            );
        } while (fileInfo->NextEntryOffset != 0);

        CloseHandle(hDir);
    }
}

int main() {
    std::wstring directoryPath = L"C:\\JASPER\\XML";
    MonitorDirectoryChanges(directoryPath);

    return 0;
}
