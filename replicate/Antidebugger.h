#include <stdlib.h>
#include <windows.h>
#include <winbase.h>

class Process
{
public:
    wchar_t name[256];
    BOOL is_detected;
    void IsDbgPresent();
    
private:
    int pid;
    HANDLE hSnapshot;
    PROCESSENTRY32 pEntry;
    void FindProcessID();
    void SetPrivileges();
};

// TRUE -> debugger detected
// FALSE -> debugger not detected

bool CheckTEB()
{
    int debugger_flag = 0;
    int a;
    __asm
    {
        mov eax, fs:[30h]
        push ecx
        mov ecx, [eax+2]
        mov a, ecx
        pop ecx
        
    }
    
    if(a)
        return true;
    else
        return false;
}

bool SendString()
{
    SetLastError(0);
    OutputDebugStringA(random_str(5));
    if (GetLastError() != 0)
        return true;
    else
        return false;
}

bool CheckDebugger()
{
    if (IsDebuggerPresent() == TRUE)
        return true;
    else
        return false;
}

// This method checks for a particular debuger. In this case it checks for x64dbg (http://x64dbg.com/).

bool x64dbg()
{
    LPCWSTR window = L"x64dbg";
    if (FindWindow(NULL, window) != NULL)
        return true;
    else
        return false;
}
