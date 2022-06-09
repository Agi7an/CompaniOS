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
#include <algorithm>

namespace fs = std::filesystem;
using namespace std;

string PATH = "D:/TCS/fourthSem/OS Lab/Package/CompaniOS/Paths/";

vector<string> modes;

int getModeCount()
{
    int modeCount = 0;
    for (const auto &f : fs::directory_iterator(PATH))
    {
        modeCount++;
    }

    return modeCount;
}

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

void setUpMode(string modeName)
{
    char c;
    ifstream file;
    string line, substr, path = PATH;
    path = path + modeName + ".txt";
    vector<string> values;
    int j = 0;
    file.open(path, ios::in);
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
            pid[j] = runProcess(apps[j], args[j]);
            j++;
        }
    }

    cout << "\nEnter 'e' to exit: ";
    cin >> c;

    for (int i = 0; i < n; i++)
    {
        terminateProcess(pid[i], 0);
    }

    delete[] pid;
    delete[] apps;
    delete[] args;

    return;
}

int addMode()
{
    string modeName, newModePath;
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
        newModePath = PATH + modeName;
        newModePath += ".txt";

        // Check if mode already exists
        for (const auto &f : fs::directory_iterator(PATH))
        {
            if (newModePath == f.path())
            {
                cout << "\nFILE ALREADY EXISTS!\n\n";
                exists = true;
            }
        }
    } while (exists);

    modes.push_back(modeName);
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
    string modePath, modeName;
    bool exists = false;

    cout << "Enter the name of the mode to be edited: ";
    cin >> modePath;
    modeName = modePath;
    if (modePath == "exit")
    {
        return -1;
    }
    modePath = PATH + modePath + ".txt";

    // Check if mode already exists
    for (const auto &f : fs::directory_iterator(PATH))
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
    string del_modePath, del_modeName;
    bool deleted = false;

    cout << "Enter the name of the mode to be removed: ";
    cin >> del_modePath;
    del_modeName = del_modePath;
    if (del_modePath == "exit")
    {
        return -1;
    }
    del_modePath = PATH + del_modePath + ".txt";

    // Check if mode already exists
    for (const auto &f : fs::directory_iterator(PATH))
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
        modes.erase(find(modes.begin(), modes.end(), del_modeName));
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
    string choice;
    char username[UNLEN + 1];
    int noOfModes = getModeCount();

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

    wprintf(L"\n\n\x1b[32mHey %s!\x1b[0m\n\n", username);
    for (const auto &f : fs::directory_iterator(PATH))
    {
        char line[100];
        string modeName = f.path().u8string();
        size_t start = modeName.find("s/");
        size_t len = modeName.find(".txt");
        start += 2;
        len -= start;
        modeName = modeName.substr(start, len);
        transform(modeName.begin(), modeName.end(), modeName.begin(), ::tolower);
        modes.push_back(modeName);
    }

    do
    {
        wprintf(L"\n\x1b[1;44;97mMENU:\x1b[0m\n\t\x1b[1;46;97mMODES:\x1b[0m\n");
        for (int i = 0; i < modes.size(); i++)
        {
            // cout << "\t\t+ " << modes[i] << "\n";
            wprintf(L"\t\t\x1b[46;97m+ %s\x1b[0m\n\n", modes[i].c_str());
        }
        // cout << "\t" << "- Edit Modes\n" << "\t" << "- Exit\n\nEnter your choice: ";
        wprintf(L"\t\x1b[44;97m- Edit Modes\x1b[0m\n\t\x1b[44;97m- Exit\x1b[0m\n\n");
        cout << "Enter your choice: ";
        cin >> choice;
        transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
        if (find(modes.begin(), modes.end(), choice) != modes.end())
        {
            setUpMode(choice);
        }
        else if (choice == "Edit Modes" || choice == "edit modes" || choice == "Edit" || choice == "edit")
        {
            edit();
        }
        else if (choice != "Exit" && choice != "exit")
        {
            cout << "Invalid Option!\n";
        }
        else
        {
            break;
        }
    } while (choice != "Exit" && choice != "exit");

    wprintf(L"\n\n\x1b[101;93m-------------------\x1b[0m\x1b[1;4;101;93mTHANK YOU\x1b[0m\x1b[101;93m-------------------\x1b[0m\n\n");

    return 0;
}