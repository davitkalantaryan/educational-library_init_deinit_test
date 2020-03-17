
// #define TRY_REORDER

#define CLASS_NAME_EXT(_nm)		f2_##_nm
#define FILE_NUMBER				"2. "

#define CLASS_NAME_EXT_MAIN(_nm)		f1_##_nm

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#else
#include <pthread.h>
#include <dlfcn.h>
#define GetCurrentThreadId  pthread_self
#endif

#ifdef TRY_REORDER
#define DUMMY_ON_MAIN()					\
	void Dummy(); \
	int m_nVal;	\
	CLASS_NAME_EXT_MAIN(TestClassMain)();
#define CALL_DUMMY_ON_MAIN()	g_testClassMain.Dummy();
#else
#define DUMMY_ON_MAIN()		\
	CLASS_NAME_EXT_MAIN(TestClassMain)();
#define CALL_DUMMY_ON_MAIN()
#endif

class CLASS_NAME_EXT_MAIN(TestClassMain) {
public:
	DUMMY_ON_MAIN()
	~CLASS_NAME_EXT_MAIN(TestClassMain)() ;

};

#ifdef TRY_REORDER
extern CLASS_NAME_EXT_MAIN(TestClassMain) g_testClassMain;
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


extern "C" {

static void AtExitCleanupFunction1(void)
{
	printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);
}

static void AtExitCleanupFunction2(void)
{
	printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);
}

PRAGMA_INITIALIZER(CLASS_NAME_EXT(prgma_initializaer1))
{
	printf(FILE_NUMBER "thread=%d,%s\n",static_cast<int>(GetCurrentThreadId()),__FUNCTION__);
	atexit(AtExitCleanupFunction1);
}

PRAGMA_INITIALIZER(CLASS_NAME_EXT(prgma_initializaer2))
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

static CLASS_NAME_EXT_MAIN(TestClassMain) g_testClassMain2;
