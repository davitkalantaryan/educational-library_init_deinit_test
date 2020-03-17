#
# file:		linux.Makefile
# created on:	2020 Mar 17
# created by:	D. Kalantaryan (davit.kalantaryan@desy.de)
#

BinTargetName		= bin_library_init_deinit_test
DllTargetName		= dll_library_init_deinit_test

mkfile_path		:= $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir		:= $(shell dirname $(mkfile_path))
ObjectsDirBase		= $(mkfile_dir)/.objects
SourceDirBase		= $(mkfile_dir)
targetDir		= $(mkfile_dir)
binTargetFilePath	= $(targetDir)/$(BinTargetName)
dllTargetFilePath	= $(targetDir)/lib$(DllTargetName).so

CC			= gcc
CPPC			= g++
LINKER			= g++
DLL_LFLAGS		= -Wl,-E -pie -shared
CPPPARAMS		=
DLL_CPPPARAMS		= -fPIC
LDPARAMS		=
LIBS			=


BinObjects	=						\
	$(ObjectsDirBase)/main_library_init_deinit_test.o

DllObjects	=						\
	$(ObjectsDirBase)/entry_library_init_deinit_test.o	\
	$(ObjectsDirBase)/second_library_init_deinit_test.o

all: $(BinTargetName)

dll: $(DllTargetName)

$(ObjectsDirBase)/%.o: $(SourceDirBase)/%.cpp
	mkdir -p $(@D)
	$(CPPC) $(CPPPARAMS) -o $@ -c $<

# cxx I reserved for library
$(ObjectsDirBase)/%.o: $(SourceDirBase)/%.cxx
	mkdir -p $(@D)
	$(CPPC) $(CPPPARAMS) $(DLL_CPPPARAMS) -o $@ -c $<

$(BinTargetName): $(BinObjects) $(DllTargetName)
	mkdir -p $(targetDir)
	@#ld $(LDPARAMS) -T $< -o $(TARGET_FILE_PATH) $(OBJECTS)
	$(LINKER) $(BinObjects) $(LIBS) $(LFLAGS) -ldl -pthread -o $(binTargetFilePath)

$(DllTargetName): $(DllObjects)
	@mkdir -p $(targetDir)
	$(LINKER) $(DllObjects) $(LIBS) $(LFLAGS) $(DLL_LFLAGS) -o $(dllTargetFilePath)

# .PHONY means always out of date
.PHONY: clean

clean:
	rm -f $(DllObjects) $(BinObjects) $(dllTargetFilePath) $(binTargetFilePath)
