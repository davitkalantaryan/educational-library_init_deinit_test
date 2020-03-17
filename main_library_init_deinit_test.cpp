
#define LIBRARY_NAME_BASE	"dll_library_init_deinit_test"

#include <iostream>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#define LIBRARY_PATH	LIBRARY_NAME_BASE ".dll"
#else
#include <dlfcn.h>
#define LIBRARY_PATH	"lib" LIBRARY_NAME_BASE ".so"
typedef void* HMODULE;
#define LoadLibraryA(__libName) dlopen((__libName), RTLD_LAZY)
#define FreeLibrary dlclose
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
