
#define LIBRARY_NAME_BASE	"dll_library_init_deinit_test"

#include <iostream>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#define LIBRARY_PATH	LIBRARY_NAME_BASE ".dll"
#else
#endif


int main(void)
{
	HMODULE libHandle = LoadLibraryA(LIBRARY_PATH);

	if(!libHandle){
		::std::cerr << "Unable to open library " << LIBRARY_PATH << ::std::endl;
		return 0;
	}

	FreeLibrary(libHandle);
	
	return 0;
}
