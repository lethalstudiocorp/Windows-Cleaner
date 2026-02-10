#include <windows.h>
#include <lmcons.h>
#include <iostream>
#include <string>

int deletedFiles = 0;
int deletedDirs  = 0;

bool DeleteFolderRecursive(const std::string& path) {
    if (path.size() < 10) return false; // защита от корня типа C:\

    std::string searchPath = path + "\\*";
    WIN32_FIND_DATAA ffd;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &ffd);

    if (hFind == INVALID_HANDLE_VALUE) return false;

    do {
        std::string name = ffd.cFileName;
        if (name == "." || name == "..") continue;

        std::string fullPath = path + "\\" + name;

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            DeleteFolderRecursive(fullPath);
            if (RemoveDirectoryA(fullPath.c_str()))
                deletedDirs++;
        } else {
            SetFileAttributesA(fullPath.c_str(), FILE_ATTRIBUTE_NORMAL);
            if (DeleteFileA(fullPath.c_str()))
                deletedFiles++;
        }

    } while (FindNextFileA(hFind, &ffd));

    FindClose(hFind);
    return true;
}

std::string GetUsername() {
    char name[UNLEN + 1];
    DWORD len = UNLEN + 1;
    GetUserNameA(name, &len);
    return std::string(name);
}

void PrintStats() {
    std::cout << "\nDeleted files: " << deletedFiles << "\n";
    std::cout << "Deleted folders: " << deletedDirs << "\n\n";
}

int main() {
    std::string username = GetUsername();

    std::string userTemp   = "C:\\Users\\" + username + "\\AppData\\Local\\Temp";
    std::string systemTemp = "C:\\Windows\\Temp";
    std::string prefetch   = "C:\\Windows\\Prefetch";

    while (true) {
        deletedFiles = 0;
        deletedDirs = 0;

        std::cout << "\n=== CLEANER v1.0 | Lethal Studio ===\n";
        std::cout << "1. Clean User TEMP\n";
        std::cout << "2. Clean System TEMP\n";
        std::cout << "3. Clean Prefetch\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose: ";

        int input;
        std::cin >> input;

        if (!std::cin) break;

        switch (input) {
            case 1:
                DeleteFolderRecursive(userTemp);
                PrintStats();
                break;
            case 2:
                DeleteFolderRecursive(systemTemp);
                PrintStats();
                break;
            case 3:
                DeleteFolderRecursive(prefetch);
                PrintStats();
                break;
            case 4:
                return 0;
            default:
                std::cout << "Unknown command.\n";
        }
    }
}
