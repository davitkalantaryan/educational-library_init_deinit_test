# Sources to demonstrate library initialization cleanup details

Example software to demonstrate specifics of initializing and cleaning libraries.  
2 compilers are discussed here   
 1.  [microsoft](https://www.cprogramming.com/visual.html)  
 2.  [gcc](https://gcc.gnu.org/) (clang is compatible with gcc, but for time being clang tests are not done (planned to do))  
  
Two projects are compiled - library and executable that loads and unloads this library to check and demonstrate following:  
 1.  What is the sequence of initializations routines calling.  
      a) In the case of Microsoft Windows following should be considered  
	      [1] global constructors  
	  	  [2] DllMain (if we have dynamic library)  
	  	  [3] [".CRT$XCU"](https://docs.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization?view=vs-2019) method  
	  b) In the case of GCC following possible initialization routine sequence should be considered  
	      [1] global constructors  
	  	  [2]  __attribute__((__constructor__))  
 2.  In which thread library cleanup routine called. In the case of 
      a) return from main thread  
	  b) exit call in the mid of execution  


## How to compile application  
### Windows  
Better to open developer command prompt. Visual studio version does not play role. From visual studio console run the command  
``` bat  
nmake /f windows.Makefile  
:: to clean the stuff  
nmake /f windows.Makefile clean  
```  
  
### Linux  
From any shell run the command  
``` bash  
make -f linux.Makefile  
# to clean the stuff  
make -f linux.Makefile clean  
```

## products  
Commands above will generate 2 files: dynamic library and executable to test this library  
  
## running test  
```  
./bin_library_init_deinit_test   # will load library and unload dynamically from main thread  
./bin_library_init_deinit_test 1 # will load library dynamically and unload lib because of exit on main return    
./bin_library_init_deinit_test 1 2 # will load library will be done from main thread and other thread dynamically will unload it  
./bin_library_init_deinit_test 1 2 # will load library will be done from main thread and library will be unloaded because of exit call from other thread  
```  
  
### conclusions on tests  
 1. If process exit triggered by not library loader thread, then initialization routine and cleanup routines will be called from different conexes  
    This is not taken into account always!!!  
 2. Initializing routines calling sequence will be listed below  
     a) For Windows  
         [1]  pragma_initializers # some hack is needed to make this first  
	     [2]  global/static constructors  
	     [3]  DllMain  
	 b) For linux inside one file if there are several '__attribute__((__constructor__))'s and several global/static constructors,   
	    then for that file first will be called '__attribute__((__constructor__))'s then constructors.  
		But !!!!!!!!!!!!!!!!  
		In the case if there are 2 files each with '__attribute__((__constructor__))'s and constrctors, then constructors in first  
		file will be called before '__attribute__((__constructor__))'s in second file.  
		So for GCC (I guess clang also, should be tested) no guaranty that '__attribute__((__constructor__))'s will be called  
		before constructors if they are all not in one source file . see [this](https://github.com/davitkalantaryan/library_init_deinit_test#linux-screenshot-on-problem)  


![Init deinit sequence](https://github.com/davitkalantaryan/library_init_deinit_test/blob/master/docs/images/windows_output_sceenshot.png)  
![sequence with 2 library source files](https://github.com/davitkalantaryan/library_init_deinit_test/blob/master/docs/images/windows_output_with_2_source_files_sceenshot.png)  

## code example to insert code in the crt initialization section 
look here microsoft [documentation](https://docs.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization?view=vs-2019)  
```cpp  
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
static void AtExitCleanupFunction(void)
{
	printf("I'll be called during this code cleanup!\n");
}

PRAGMA_INITIALIZER(pragma_initializaer))
{
	printf("I'll be called during this code initialization and my undecorated name is %s!\n",__FUNCTION__); // undecorated name is 'pragma_initializaer'
	atexit(AtExitCleanupFunction);
}
```  

## linux screenshot on problem  
![gcc unknown sequence](https://github.com/davitkalantaryan/library_init_deinit_test/blob/master/docs/images/problematic_initialization_sequence_on_linux.png)
