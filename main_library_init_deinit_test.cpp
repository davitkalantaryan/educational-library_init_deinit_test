
#define LIBRARY_NAME_BASE	"dll_library_init_deinit_test"

#include <iostream>
#include <stdlib.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#define LIBRARY_PATH			LIBRARY_NAME_BASE ".dll"
#define THREAD_RET				DWORD 
#define THREAD_RET_AND_CALLING	DWORD WINAPI
#define proper_cast				static_cast
typedef HANDLE pthread_t;
// let's for now ignore thred return (GetThreadExitCode)
#define pthread_join(__thread,__retArg)	\
	do{ \
		WaitForSingleObjectEx((__thread),INFINITE,TRUE); \
		CloseHandle((__thread)); \
	}while(0)
#define pthread_create(__newthread,__attr,__start_routine,__arg)	\
	( *(__newthread)=CreateThread((__attr),0,ExiterThreadFunction,(__arg),0,nullptr) )
#else
#include <pthread.h>
#include <dlfcn.h>
#define LIBRARY_PATH	"lib" LIBRARY_NAME_BASE ".so"
typedef void* HMODULE;
#define LoadLibraryA(__libName) dlopen((__libName), RTLD_LAZY)
#define FreeLibrary dlclose
#define THREAD_RET				void* 
#define THREAD_RET_AND_CALLING	void*
#define proper_cast				reinterpret_cast
#if __cplusplus<=201103L
#define nullptr NULL
#endif
#endif

static HMODULE s_libHandle = proper_cast<HMODULE>(0);

static THREAD_RET_AND_CALLING ExiterThreadFunction(void* lpThreadParameter);

int main(int a_argc, char* [])
{
	if(a_argc<3){
		if (a_argc == 1) {
			::std::cout << "FreeLibrary[dlclose] will be called from main thread\n";
		}
		else {
			::std::cout << "Return from main function will be called (no FreeLibrary or dlclose)\n";
		}
	}
	else{
		if(a_argc==3){
			::std::cout << "second thread will be created to cleanup lib with FreeLibrary[dlclose]\n";
		}
		else{
			::std::cout << "second thread will be created to call exit for exiting application\n";
		}
	}

	s_libHandle = LoadLibraryA(LIBRARY_PATH);

	if(!s_libHandle){
		::std::cerr << "Unable to open library " << LIBRARY_PATH << ::std::endl;
		return 0;
	}

	if(a_argc>2){
		pthread_t threadHandle;
		pthread_create(&threadHandle,nullptr,ExiterThreadFunction,(a_argc>3)?reinterpret_cast<void*>(1):nullptr);
		pthread_join(threadHandle,nullptr);
	}
	else{
		if(a_argc<2){FreeLibrary(s_libHandle);}
	}
	
	return 0;
}

static THREAD_RET_AND_CALLING ExiterThreadFunction(void* a_pArg)
{
	if(a_pArg){
		exit(0);
	}
	FreeLibrary(s_libHandle);
	return proper_cast<THREAD_RET>(0);
}
