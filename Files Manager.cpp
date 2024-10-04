#include <iostream>
#include <corecrt_io.h>
#include <sstream>
#include<Windows.h>
using namespace std;

void GetAllFilesAtPath(string Path);
void Menu();
void DefaultColor();
void RedColor();
void GreenColor();
void BlueColor();
bool DeleteFileOrFolder(const std::string& path);
bool MarkFileOrFolderForDeletion(const std::string& path);

int main()
{
    RedColor();
    cout << "Welcome to File Manager! Made by Baugle!\n";
    Menu();
    int number = 0;
    cin >> number;

    string Path;
    while (number != 000)
    {
        switch (number)
        {
        case 1:
            cin.get();
            cout << "Path:_________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
            getline(cin, Path);
            GetAllFilesAtPath(Path);
            cout << "Enter for continue!";
            cin.get();
            break;
        case 2:
            cin.get();
            cout << "Path:_________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
            getline(cin, Path);
            if (DeleteFileOrFolder(Path) == false)
            {
                RedColor();
                cout << "Deletion failed, there may be a folder running!\n";
                cin.get();
                DefaultColor();
            }
            else
            {
                GreenColor();
                cout << "Deleted successfully\n";
                cin.get();
                DefaultColor();
            }
            
            break;
        case 3:
            cin.get();
            cout << "Path:_________________________________\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
            getline(cin, Path);
            if (MarkFileOrFolderForDeletion(Path) == true)
            {
                GreenColor();
                cout << "Marked successfully, the system will delete this file or folder on next startup\n";
                cin.get();
                DefaultColor();
            }
            else
            {
                RedColor();
                cout << "Failed to mark, may be insufficient permissions, please use administrator mode to open this program\n";
                cin.get();
                DefaultColor();
            }
            break;
        default:
            break;
        }
        system("cls");
        Menu();
        cin >> number;
    }
   
    cin.get();
    return 0;
}
void GetAllFilesAtPath(string Path)
{
    intptr_t Handle;
    struct _finddata_t FileInfo;
    string p;
    if ((Handle = _findfirst(p.assign(Path).append("\\*").c_str(), &FileInfo)) == -1)
    {
        RedColor();
        cout << "Can not find!";
    }
    else
    {
        GreenColor();
        cout << FileInfo.name <<endl;
        while (_findnext(Handle, &FileInfo) == 0)
            cout << FileInfo.name << endl;
        _findclose(Handle);
    }
    DefaultColor();
}
void Menu()
{
    BlueColor();
    cout << endl;
    cout << "-----------------------------\n";
    cout << "Tool Number:\n";
    cout << "#1:Get all files at path.\n";
    cout << "#2:Delete files.\n";
    cout << "#3:System-level deletion (effective on next reboot).\n";
    cout << "#000:Exit\n";
    cout << "-----------------------------\n";
    cout << "Enter a number:";
    DefaultColor();
}

void DefaultColor()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//设置三色相加
}
void RedColor()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);//设置红色
}
void GreenColor()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);//设置绿色
}
void BlueColor()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);//设置蓝色
}

bool DeleteFileOrFolder(const std::string& path) {
    WIN32_FIND_DATA fileInfo;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &fileInfo);

    if (hFind == INVALID_HANDLE_VALUE) {
        // 如果无效，尝试删除单个文件
        return DeleteFile(path.c_str()) || RemoveDirectory(path.c_str());
    }

    // 遍历文件夹内的所有文件和文件夹
    do {
        std::string fileOrFolder = fileInfo.cFileName;
        if (fileOrFolder != "." && fileOrFolder != "..") {
            std::string fullPath = path + "\\" + fileOrFolder;

            if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // 递归删除文件夹
                if (!DeleteFileOrFolder(fullPath)) {
                    return false;
                }
            }
            else {
                // 删除文件
                if (!DeleteFile(fullPath.c_str())) {
                    return false;
                }
            }
        }
    } while (FindNextFile(hFind, &fileInfo));

    FindClose(hFind);

    // 最后删除文件夹本身
    return RemoveDirectory(path.c_str());
}

bool MarkFileOrFolderForDeletion(const std::string& path) {
    // 使用 MoveFileEx 将文件/文件夹标记为系统重启时删除
    return MoveFileEx(path.c_str(), nullptr, MOVEFILE_DELAY_UNTIL_REBOOT);
}