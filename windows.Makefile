#
# file:			windows.Makefile
# created on:	2020 Mar 17
# created by:	D. Kalantaryan (davit.kalantaryan@desy.de)
#
# purpose:		To compile 2 projects - lib and bin to work with this lib
#

TargetMachine	= x86

BinTargetName	= bin_library_init_deinit_test
DllTargetName	= dll_library_init_deinit_test
ObjectsDirBase	= .objects\win

CC				= cl 
CPPC           	= cl -Zc:__cplusplus
LINKER        	= link
DEFINES       	= -DUNICODE -D_UNICODE -DUSETIME_HASH=\"be759ec2\" 
#CFLAGS        	= /MTd -nologo -Zc:wchar_t -FS -Zc:strictStrings -Zi -MDd -W3 -w44456 -w44457 -w44458 /$(TargetName).vc.pdb $(DEFINES)
#CXXFLAGS      	= -nologo -Zc:wchar_t -FS -Zc:rvalueCast -Zc:inline -Zc:strictStrings -Zc:throwingNew -Zc:referenceBinding -Zc:__cplusplus -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS /W3 -Zi -MDd -W3 -w34100 -w34189 -w44996 -w44456 -w44457 -w44458 -wd4577 -wd4467 -EHsc /$(TargetName).vc.pdb $(DEFINES)
CFLAGS			= /MTd
CXXFLAGS		= $(CFLAGS)
LIBS			=

InitCurDir		= $(MAKEDIR)
BinObjectsDir	= $(ObjectsDirBase)\$(BinTargetName)
DllObjectsDir	= $(ObjectsDirBase)\$(BinTargetName)

BinObjects			=					\
	main_library_init_deinit_test.obj	

DllObjects			=					\
	entry_library_init_deinit_test.obj

CurObjectsDir = $(DllObjectsDir)

.cpp.obj:
	if not exist $(CurObjectsDir)\$(@D) mkdir $(CurObjectsDir)\$(@D)
	$(CPPC) /c $(CXXFLAGS) /Fo$(BinObjectsDir)\ $*.cpp

.cxx.obj:
	if not exist $(CurObjectsDir)\$(@D) mkdir $(CurObjectsDir)\$(@D)
	$(CPPC) /c $(CXXFLAGS) /Fo$(BinObjectsDir)\ $*.cxx
  
all: $(BinTargetName)

dll: $(DllTargetName)

clean:
	if exist $(ObjectsDirBase) rmdir /q /s $(ObjectsDirBase)
	@if exist $(BinTargetName).exe del $(BinTargetName).exe
	@if exist $(DllTargetName).dll del $(DllTargetName).dll
	@if exist *.ilk del /s /q *.ilk
	@if exist *.pdb del /s /q *.pdb
	@echo "clean done!"
	
$(BinTargetName): $(BinObjects) $(DllTargetName)
	@::pushd $(BinObjectsDir)
	@cd $(BinObjectsDir)
	@::let us display this, so skip @
	$(LINKER) $(LFLAGS) $(BinObjects) $(LIBS) /SUBSYSTEM:CONSOLE Advapi32.lib /OUT:$(BinTargetName).exe
	@cd $(InitCurDir)
	@::popd 
	@::copy /y $(ObjectsDir)\$(TargetName).exe .
	@::copy /y $(TargetName).exe ..\.
	@copy /y $(BinObjectsDir)\$(BinTargetName).exe .

$(DllTargetName): $(DllObjects) dllObjDir
	@cd $(DllObjectsDir)
	@::let us display this, so skip @
	$(LINKER) $(LFLAGS) $(DllObjects) $(LIBS) /DLL /SUBSYSTEM:CONSOLE Advapi32.lib /OUT:$(DllTargetName).dll
	@cd $(InitCurDir)
	@copy /y $(DllObjectsDir)\$(DllTargetName).dll .


binObjDir:
	set CurObjectsDir=$(BinObjectsDir) 

dllObjDir:
	set CurObjectsDir=$(DllObjectsDir)

tests:
	@echo InitCurDir=$(ThisDir)
	@echo MAKEDIR=$(MAKEDIR)
	@echo MAKEFLAGS=$(MAKEFLAGS)
	@echo MAKE=$(MAKE)
	@echo PATH=$(PATH)
	