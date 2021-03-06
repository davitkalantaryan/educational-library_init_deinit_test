
// #define TRY_REORDER

#define CLASS_NAME_EXT(_nm)		f1_##_nm
#define FILE_NUMBER				"1. "

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#define EXPORT_FROM_LIB __declspec(dllexport)
#else
#include <pthread.h>
#include <dlfcn.h>
#define GetCurrentThreadId  pthread_self
#define EXPORT_FROM_LIB
#endif

#ifdef TRY_REORDER
#define DUMMY_ON_MAIN()					\
	void Dummy(){printf("this=%p,m_nVal=%d\n",static_cast<void*>(this),m_nVal++);} \
	int m_nVal;	\
	CLASS_NAME_EXT(TestClassMain)():m_nVal(10){printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);}
#define CALL_DUMMY_ON_MAIN()	g_testClassMain.Dummy();
#else
#define DUMMY_ON_MAIN()		\
	CLASS_NAME_EXT(TestClassMain)(){printf( FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);}
#define CALL_DUMMY_ON_MAIN()
#endif

class CLASS_NAME_EXT(TestClassMain) {
public:
	DUMMY_ON_MAIN()
	~CLASS_NAME_EXT(TestClassMain)() {printf(FILE_NUMBER "thread=%d,%s\n", static_cast<int>(GetCurrentThreadId()), __FUNCTION__);}

};

#ifdef TRY_REORDER
extern CLASS_NAME_EXT(TestClassMain) g_testClassMain;
#endif


extern "C" {

#ifdef _MSC_VER

#pragma section(".CRT$XCB",read)
#define INITIALIZER_RAW_(f,p)										\
        static void f(void);										\
        __declspec(allocate(".CRT$XCB")) void (*f##_)(void) = f;	\
        __pragma(comment(linker,"/include:" p #f "_"))				\
        static void f(void)

#if defined(_WIN64) || defined(_M_ARM)
#define PRAGMA_INITIALIZER(f) INITIALIZER_RAW_(f,"")
#else
#define PRAGMA_INITIALIZER(f) INITIALIZER_RAW_(f,"_")
#endif

#elif defined(__GNUC__)  // #ifdef _MSC_VER
#define PRAGMA_INITIALIZER(f)								\
static void  __attribute__ ((__constructor__)) f (void);	\
static void f (void)
#else // below is the case for solaris cc, for this I have not tested
#define PRAGMA_INITIALIZER(f)	\
static void f (void);			\
__pragma init  (client_init)	\
static void f (void)
#endif

} // extern "C"{


class CLASS_NAME_EXT(TestClass1) {
public:
	CLASS_NAME_EXT(TestClass1)(){ CALL_DUMMY_ON_MAIN() printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);}
	~CLASS_NAME_EXT(TestClass1)(){printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);}
}static s_testClass1;

class CLASS_NAME_EXT(TestClass2) {
public:
	CLASS_NAME_EXT(TestClass2)(){ CALL_DUMMY_ON_MAIN() printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);}
	~CLASS_NAME_EXT(TestClass2)() {printf(FILE_NUMBER "thread=%d,%s\n", static_cast<int>(GetCurrentThreadId()), __FUNCTION__);}
}static s_testClass2;

EXPORT_FROM_LIB int nonmain(void)
{
	printf("Hello world from %s\n", __FUNCTION__);
	return 0;
}


#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE a_hinstDLL, DWORD a_fdwReason, LPVOID a_lpvReserved)
{
	//printf(FILE_NUMBER "thread=%d,%s,", static_cast<int>(GetCurrentThreadId()), __FUNCTION__);

	switch(a_fdwReason){
	case DLL_PROCESS_ATTACH:
		printf(FILE_NUMBER "thread=%d,%s,", static_cast<int>(GetCurrentThreadId()), __FUNCTION__);
		printf("DLL_PROCESS_ATTACH. ");
		printf(a_lpvReserved ? "Static load (-l compiler option is used)\n" : "Dynamic load (LoadLibrary) called\n");
		break;
	case DLL_PROCESS_DETACH:
		printf(FILE_NUMBER "thread=%d,%s,", static_cast<int>(GetCurrentThreadId()), __FUNCTION__);
		printf("DLL_PROCESS_DETACH. ");
		printf(a_lpvReserved ? "Process terminating\n" : "Dynamic unload (FreeLibrary) called\n");
		break;
	case DLL_THREAD_ATTACH:
		//printf("DLL_THREAD_ATTACH.\n");
		break;
	case DLL_THREAD_DETACH:
		//printf("DLL_THREAD_DETACH.\n");
		break;
	default:
		//printf("default:\n"); // never will happen
		break;
	}
	return TRUE;
}
#endif // #ifdef _WIN32


extern "C" {

static void AtExitCleanupFunction1(void)
{
	printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);
}

static void AtExitCleanupFunction2(void)
{
	printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);
}

PRAGMA_INITIALIZER(CLASS_NAME_EXT(prgma_initializaer11))
{
	printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);
	atexit(AtExitCleanupFunction1);
}

PRAGMA_INITIALIZER(CLASS_NAME_EXT(prgma_initializaer12))
{
	printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);
	atexit(AtExitCleanupFunction2);
}

}

class CLASS_NAME_EXT(TestClass3) {
public:
	CLASS_NAME_EXT(TestClass3)(){printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);}
	~CLASS_NAME_EXT(TestClass3)() {printf(FILE_NUMBER "thread=%d,%s\n", static_cast<int>(GetCurrentThreadId()), __FUNCTION__);}
}static s_testClass3;

CLASS_NAME_EXT(TestClassMain) g_testClassMain;
