// g++ -std=c++17 main.cpp -lwininet -o CompaniOS

#include <stdio.h>
#include <windows.h>
#include <Lmcons.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <wininet.h>
#include <filesystem>
#include <wchar.h>

namespace fs = std::filesystem;
using namespace std;

BOOL terminateProcess(DWORD dwProcessId, UINT uExitCode)
{
    DWORD dwDesiredAccess = PROCESS_TERMINATE;
    BOOL bInheritHandle = false;
    HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
    BOOL result = TerminateProcess(hProcess, uExitCode);

    CloseHandle(hProcess);

    if (result)
    {
        printf("\nSuccessfully closed the application!\n");
    }
    else
    {
        printf("\nThe application(PID: %lu) was closed by the user.\n", dwProcessId);
    }

    return result;
}

int runProcess(const char *app, const char *arg = NULL)
{
    char pid[10];

    SHELLEXECUTEINFO pInfo;
    pInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    pInfo.fMask = SEE_MASK_NOCLOSEPROCESS; // Used to indicate that the process member receives a process handle.
    pInfo.hwnd = NULL;
    pInfo.lpVerb = NULL;
    pInfo.lpFile = app;
    pInfo.lpParameters = arg;
    pInfo.lpDirectory = NULL;
    pInfo.nShow = SW_SHOWMAXIMIZED;
    pInfo.hInstApp = NULL;

    ShellExecuteEx(&pInfo);

    printf("\nSuccessfully opened %s!\n", app);

    return GetProcessId(pInfo.hProcess);
}

void setCodingMode()
{
    char c;
    ifstream file;
    string line, substr;
    vector<string> values;
    int j = 0;
    file.open("D:\\TCS\\fourthSem\\OS Lab\\Package\\Rough\\Paths\\coding.txt", ios::in);
    while (getline(file, line))
    {
        stringstream ss(line);
        while (ss.good())
        {
            getline(ss, substr, ',');
            values.push_back(substr);
        }
    }
    file.close();

    int n = values.size() / 2;
    const char **apps = new const char *[n], **args = new const char *[n];
    int *pid = new int(n);

    for (int i = 0; i < values.size(); i++)
    {
        if (i % 2 == 0)
        {
            apps[j] = values[i].c_str();
        }
        else
        {
            args[j] = values[i].c_str();
            j++;
        }
    }

    cout << "App 1: " << apps[0] << " with " << args[0] << "\n";
    cout << "App 2: " << apps[1] << " with " << args[1] << "\n";

    pid[0] = runProcess(apps[0], args[0]);
    pid[1] = runProcess(apps[1], args[1]);

    cout << "\nEnter 'e' to exit: ";
    cin >> c;

    terminateProcess(pid[0], 0);
    terminateProcess(pid[1], 0);

    delete[] pid;
    delete[] apps;
    delete[] args;

    return;
}

void setGamingMode()
{
    char c;
    ifstream file;
    string line, substr;
    vector<string> values;
    int j = 0;
    file.open("D:\\TCS\\fourthSem\\OS Lab\\Package\\Rough\\Paths\\gaming.txt", ios::in);
    while (getline(file, line))
    {
        stringstream ss(line);
        while (ss.good())
        {
            getline(ss, substr, ',');
            values.push_back(substr);
        }
    }
    file.close();

    int n = values.size() / 2;
    const char **apps = new const char *[n], **args = new const char *[n];
    int *pid = new int(n);

    for (int i = 0; i < values.size(); i++)
    {
        if (i % 2 == 0)
        {
            apps[j] = values[i].c_str();
        }
        else
        {
            args[j] = values[i].c_str();
            j++;
        }
    }

    cout << "App 1: " << apps[0] << " with " << args[0] << "\n";
    cout << "App 2: " << apps[1] << " with " << args[1] << "\n";

    pid[0] = runProcess(apps[0], args[0]);
    pid[1] = runProcess(apps[1], args[1]);

    cout << "\nEnter 'e' to exit: ";
    cin >> c;

    terminateProcess(pid[0], 0);
    terminateProcess(pid[1], 0);

    delete[] pid;
    delete[] apps;
    delete[] args;

    return;
}

void setStudyingMode()
{
    char c;
    ifstream file;
    string line, substr;
    vector<string> values;
    int j = 0;

    file.open("D:\\TCS\\fourthSem\\OS Lab\\Package\\Rough\\Paths\\studying.txt", ios::in);
    while (getline(file, line))
    {
        stringstream ss(line);
        while (ss.good())
        {
            getline(ss, substr, ',');
            values.push_back(substr);
        }
    }
    file.close();

    int n = values.size() / 2;
    const char **apps = new const char *[n], **args = new const char *[n];
    int *pid = new int(n);

    for (int i = 0; i < values.size(); i++)
    {
        if (i % 2 == 0)
        {
            apps[j] = values[i].c_str();
        }
        else
        {
            args[j] = values[i].c_str();
            j++;
        }
    }

    cout << "App 1: " << apps[0] << " with " << args[0] << "\n";
    cout << "App 2: " << apps[1] << " with " << args[1] << "\n";

    pid[0] = runProcess(apps[0], args[0]);
    pid[1] = runProcess(apps[1], args[1]);

    cout << "\nEnter 'e' to exit: ";
    cin >> c;

    terminateProcess(pid[0], 0);
    terminateProcess(pid[1], 0);

    delete[] pid;
    delete[] apps;
    delete[] args;

    return;
}

int addMode()
{
    string modeName, newModePath, path = "./Paths/";
    bool exists;
    do
    {
        exists = false;
        cout << "Enter the name of the new mode: ";
        cin >> modeName;
        if (modeName == "exit")
        {
            return -1;
        }
        newModePath = path + modeName;
        newModePath += ".txt";

        // Check if mode already exists
        for (const auto &f : fs::directory_iterator(path))
        {
            if (newModePath == f.path())
            {
                cout << "\nFILE ALREADY EXISTS!\n\n";
                exists = true;
            }
        }
    } while (exists);

    string more;
    ofstream file(newModePath);

    do
    {
        string application, argument;

        cout << "Enter the path of the application(.exe): ";
        cin >> application;
        application = "\"" + application + "\",";
        cout << "Enter the argument to be passed: ";
        cin >> argument;
        if (argument != "NULL")
        {
            argument = "\"" + argument + "\"";
        }

        file << application;
        file << argument;

        cout << "Do you wish to add more applications?(Y/N) ";
        cin >> more;
        if (more == "Y" || more == "y")
        {
            file << "\n";
        }
    } while (more == "Y" || more == "y");

    file.close();

    cout << "\nThe new mode " << modeName << " has been successfully added.\n";

    return 0;
}

int editMode()
{
    string modePath, modeName, path = "./Paths/";
    bool exists = false;

    cout << "Enter the name of the mode to be edited: ";
    cin >> modePath;
    modeName = modePath;
    if (modePath == "exit")
    {
        return -1;
    }
    modePath = path + modePath + ".txt";

    // Check if mode already exists
    for (const auto &f : fs::directory_iterator(path))
    {
        if (modePath == f.path())
        {
            exists = true;
            break;
        }
    }
    if (!exists)
    {
        cout << "\n"
             << modeName << " was not found!\n";
        return -1;
    }

    string more;
    do
    {
        int c;
        cout << "\n1. Add Application\n2. Remove Application\n3. Edit Application\n\n";
        cin >> c;
        switch (c)
        {
        case 1:
        {
            ofstream file;
            file.open(modePath, ios_base::app);
            string application, argument;

            cout << "Enter the path of the application(.exe): ";
            cin >> application;
            application = "\n\"" + application + "\",";
            cout << "Enter the argument to be passed: ";
            cin >> argument;
            if (argument != "NULL")
            {
                argument = "\"" + argument + "\"";
            }

            file << application;
            file << argument;

            file.close();

            break;
        }
        case 2:
        {
            string app, arg;
            ifstream fileRead(modePath);
            string line, substr;
            int j = 0;
            vector<string> values;

            cout << "Enter the path of the application to be removed: ";
            cin >> app;
            cout << "Enter its argument: ";
            cin >> arg;
            app = "\"" + app + "\"";
            if (arg != "NULL")
            {
                arg = "\"" + arg + "\"";
            }

            while (getline(fileRead, line))
            {
                stringstream ss(line);
                while (ss.good())
                {
                    getline(ss, substr, ',');
                    values.push_back(substr);
                }
            }

            fileRead.close();
            remove(modePath.c_str());

            ofstream fileWrite(modePath);
            bool firstLine = true;
            bool found = false;

            for (int i = 0; i < values.size(); i = i + 2)
            {
                if ((values[i] == app) && (values[i + 1] == arg))
                {
                    found = true;
                    continue;
                }
                else
                {
                    if (i == 0 || firstLine)
                    {
                        values[i] += ",";
                    }
                    else
                    {
                        values[i] = "\n" + values[i] + ",";
                    }
                    fileWrite << values[i];
                    fileWrite << values[i + 1];
                    firstLine = false;
                }
            }

            if (!found)
            {
                cout << "The application " << app << " with argument " << arg << " was not found!\n";
            }

            fileWrite.close();

            break;
        }

        // Edit Application
        case 3:
        {
            string app, arg, newApp, newArg;
            ifstream fileRead(modePath);
            string line, substr;
            int j = 0;
            vector<string> values;

            cout << "Enter the path of the application to be edited: ";
            cin >> app;
            cout << "Enter its current argument: ";
            cin >> arg;
            cout << app << " will be changed to: ";
            cin >> newApp;
            cout << arg << " will be changed to: ";
            cin >> newArg;
            newApp = "\"" + newApp + "\"";
            app = "\"" + app + "\"";
            if (newArg != "NULL")
            {
                newArg = "\"" + newArg + "\"";
            }
            if (arg != "NULL")
            {
                arg = "\"" + arg + "\"";
            }

            while (getline(fileRead, line))
            {
                stringstream ss(line);
                while (ss.good())
                {
                    getline(ss, substr, ',');
                    values.push_back(substr);
                }
            }

            fileRead.close();
            remove(modePath.c_str());

            ofstream fileWrite(modePath);
            bool found = false;

            for (int i = 0; i < values.size(); i = i + 2)
            {
                if ((values[i] == app) && (values[i + 1] == arg))
                {
                    values[i] = newApp;
                    values[i + 1] = newArg;
                    found = true;
                }

                if (i == 0)
                {
                    values[i] += ",";
                }
                else
                {
                    values[i] = "\n" + values[i] + ",";
                }

                fileWrite << values[i];
                fileWrite << values[i + 1];
            }

            fileWrite.close();

            if (!found)
            {
                cout << "The application " << app << " with argument " << arg << " was not found!\n";
            }

            break;
        }
        default:
        {
            cout << "Invalid option!\n";
            break;
        }
        }
        cout << "Do you wish to make any other changes?(Y/N) ";
        cin >> more;
    } while (more == "Y" || more == "y");

    return 0;
}

int deleteMode()
{
    string del_modePath, del_modeName, path = "./Paths/";
    bool deleted = false;

    cout << "Enter the name of the mode to be removed: ";
    cin >> del_modePath;
    del_modeName = del_modePath;
    if (del_modePath == "exit")
    {
        return -1;
    }
    del_modePath = path + del_modePath + ".txt";

    // Check if mode already exists
    for (const auto &f : fs::directory_iterator(path))
    {
        if (del_modePath == f.path())
        {
            remove(del_modePath.c_str());
            deleted = true;
            break;
        }
    }
    if (!deleted)
    {
        cout << "\n"
             << del_modeName << " was not found!\n";
        return -1;
    }
    else
    {
        cout << "\nThe mode " << del_modeName << " has been successfully deleted.\n";
    }

    return 0;
}

void edit()
{
    int c = 5;
    system("cls");
    while (c < 1 || c > 4)
    {
        cout << "EDIT MODES\n1. Add Mode\n2. Edit Mode\n3. Delete Mode\n4. Exit Editor\n";
        cin >> c;
        switch (c)
        {
        case 1:
            addMode();
            break;
        case 2:
            editMode();
            break;
        case 3:
            deleteMode();
            break;
        case 4:
            return;
        default:
            cout << "Invalid option!\n";
        }
    }
    return;
}

int main()
{
    char c;
    char username[UNLEN + 1];

    // Getting the Windows username
    DWORD username_len = UNLEN + 1;
    GetUserName(username, &username_len);

    bool internet = InternetCheckConnection("https://google.com", FLAG_ICC_FORCE_CONNECTION, 0);
    if (internet)
    {
        cout << "\n\x1b[42;97mYou are connected to the internet!\x1b[0m\n";
    }
    else
    {
        cout << "\n\x1b[7;31mYou are NOT connected to the internet!\x1b[0m\n";
    }

    wprintf(L"\n\n\x1b[32mHey %s!\x1b[0m\n\n\x1b[4;36mPick a mode:\x1b[0m\n\t\x1b[97m1. Coding\n\t2. Gaming\n\t3. Studying\n\t4. Edit Modes\n\t5. Exit\x1b[0m\n", username);
    cin >> c;
    switch (c)
    {
    case '1':
        printf("\nSetting up coding mode...\n");
        setCodingMode();
        break;
    case '2':
        printf("\nSetting up gaming mode...\n");
        setGamingMode();
        break;
    case '3':
        printf("\nSetting up studying mode...\n");
        setStudyingMode();
        break;
    case '4':
        printf("\nPreparing to edit modes...\n");
        edit();
        break;
    case '5':
        return 0;
    default:
        printf("Invalid option!\n");
    };

    cout << "\n\n----------------THANK YOU----------------\n\n";

    return 0;
}