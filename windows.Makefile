TargetName		= nmake_test
ObjectsDir		= .objects

CC				= cl 
CPPC           	= cl -Zc:__cplusplus
LINKER        	= link
DEFINES       	= -DUNICODE -D_UNICODE -DUSETIME_HASH=\"be759ec2\" 
CFLAGS        	= /MTd -nologo -Zc:wchar_t -FS -Zc:strictStrings -Zi -MDd -W3 -w44456 -w44457 -w44458 /$(TargetName).vc.pdb $(DEFINES)
#CXXFLAGS      	= -nologo -Zc:wchar_t -FS -Zc:rvalueCast -Zc:inline -Zc:strictStrings -Zc:throwingNew -Zc:referenceBinding -Zc:__cplusplus -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS /W3 -Zi -MDd -W3 -w34100 -w34189 -w44996 -w44456 -w44457 -w44458 -wd4577 -wd4467 -EHsc /$(TargetName).vc.pdb $(DEFINES)
CXXFLAGS		= /MTd
LIBS			=

OBJECTS			= \
	main_nmake_test.obj	\
	nonmain_nmake_test.obj

.cpp.obj:
	if not exist $(ObjectsDir)  mkdir $(ObjectsDir)
	$(CPPC) /c $(CXXFLAGS) /Fo$(ObjectsDir)\ $*.cpp
  
all: $(TargetName)

clean:
	@echo off
	if exist $(ObjectsDir) rmdir /q /s $(ObjectsDir)
	if exist $(OBJECTS) del $(OBJECTS)
	if exist $(TargetName).exe del $(TargetName).exe
	if exist *.ilk del /s /q *.ilk
	if exist *.pdb del /s /q *.pdb
	
$(TargetName): $(OBJECTS)
	cd $(ObjectsDir)
	$(LINKER) $(LFLAGS) $(OBJECTS) $(LIBS) /SUBSYSTEM:CONSOLE Advapi32.lib /OUT:$(TargetName).exe
	::popd 
	::copy /y $(ObjectsDir)\$(TargetName).exe .
	copy /y $(TargetName).exe ..\.
	