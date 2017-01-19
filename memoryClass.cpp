#include <Windows.h>
#include <TlHelp32.h>
#include <string>

class memoryFunctions
{
	private:
		HANDLE processHandle;
		LPCSTR windowName;
		DWORD processID;
		
		HANDLE createHandle()
		{
		    HWND WindowHandle = FindWindowA(0, windowName);
		    GetWindowThreadProcessId(WindowHandle, &processID);

		    return OpenProcess(PROCESS_ALL_ACCESS, false, processID);
		}

		DWORD setModuleAddress(std::string mName)
		{
		    HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID); //Get all loaded modules
		    MODULEENTRY32 mEntry;
		    mEntry.dwSize = sizeof(mEntry); //Declare size of module

		    std::wstring widestr = std::wstring(mName.begin(), mName.end());

		    while (Module32Next(hModule, &mEntry))
		    {
		    	if (!wcscmp(mEntry.szModule, widestr.c_str()))
			{
			    CloseHandle(hModule);
			    return (DWORD)mEntry.modBaseAddr;
			}
		    }
		    return 0;
		}

	public:
		void setProcessWindowName(std::string wName)
		{
		    windowName = wName.c_str(); //string to LPCSTR
		}
		HANDLE getHandle()
		{
		    return createHandle(); //this is used for reading/writing later
		}
		DWORD getModuleAddress(std::string mName)
		{
		    return setModuleAddress(mName);
		}
		
		//reading and writing improvents added 1/18/17 - makes it cleaner and easier to read when being used
		//memory reading
		template <class r>
		r read(DWORD dwAddress)
		{
	            r rData; //stores data
		    ReadProcessMemory(process, (LPVOID)dwAddress, &rData, sizeof(r), NULL);
		    return r; //returns the value of r from the address that we read
		}
		//memory writing
		template <class w>
		void write(DWORD dwAddress, w value)
		{
		    WriteProcessMemory(process, (LPVOID)dwAddress, &value, sizeof(w), NULL); //writeprocessmemory from microsoft
		}
};
