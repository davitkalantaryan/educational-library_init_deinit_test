
#define LIBRARY_NAME_BASE	"dll_library_init_deinit_test"

#include <iostream>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#define LIBRARY_PATH			LIBRARY_NAME_BASE ".dll"
#define THREAD_RET				DWORD 
#define THREAD_RET_AND_CALLING	DWORD WINAPI
#else
#include <dlfcn.h>
#define LIBRARY_PATH	"lib" LIBRARY_NAME_BASE ".so"
typedef void* HMODULE;
#define LoadLibraryA(__libName) dlopen((__libName), RTLD_LAZY)
#define FreeLibrary dlclose
#define THREAD_RET				void* 
#define THREAD_RET_AND_CALLING	void*
#endif

static HMODULE s_libHandle = static_cast<HMODULE>(0);

static THREAD_RET_AND_CALLING ExiterThreadFunction(void* lpThreadParameter);

int main(int a_argc, char* [])
{
	if(a_argc>1){
		::std::cout << "second thread will be created to cleanup lib\n";
	}

	s_libHandle = LoadLibraryA(LIBRARY_PATH);

	if(!s_libHandle){
		::std::cerr << "Unable to open library " << LIBRARY_PATH << ::std::endl;
		return 0;
	}

	if(a_argc>1){
		HANDLE	secondThreadHandle = CreateThread(nullptr,0,ExiterThreadFunction,nullptr,0,nullptr);
		WaitForSingleObjectEx(secondThreadHandle,INFINITE,TRUE);
		CloseHandle(secondThreadHandle);
	}
	else{
		FreeLibrary(s_libHandle);
	}
	
	return 0;
}

static THREAD_RET_AND_CALLING ExiterThreadFunction(void*)
{
	FreeLibrary(s_libHandle);
	return static_cast<THREAD_RET>(0);
}
