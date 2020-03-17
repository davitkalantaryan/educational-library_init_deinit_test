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
	  	  [3] [".CRT$XCU"](https://docs.microsoft.com/en-us/cpp/c-runtime-library/crt-initialization?view=vs-2019)  
	  b) In the case of GCC following possible initialization routine sequence should be considered  
	      [1] global constructors  
	  	  [2]  __attribute__ ((__constructor__))  
 2.  In which thread library cleanup routine called. In the case of 
