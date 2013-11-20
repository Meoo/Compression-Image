## -*- Makefile -*-
##
## User: GIZOU
## Time: 20 nov. 2013 15:48:26
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# Define the target directories.
TARGETDIR_codagedin.cpp=GNU-amd64-Windows


all: $(TARGETDIR_codagedin.cpp)/codagedin.cpp

## Target: codagedin.cpp
OBJS_codagedin.cpp =  \
	$(TARGETDIR_codagedin.cpp)/codage.o \
	$(TARGETDIR_codagedin.cpp)/codagedin.o
USERLIBS_codagedin.cpp = $(SYSLIBS_codagedin.cpp) 
DEPLIBS_codagedin.cpp =  
LDLIBS_codagedin.cpp = $(USERLIBS_codagedin.cpp)


# Link or archive
$(TARGETDIR_codagedin.cpp)/codagedin.cpp: $(TARGETDIR_codagedin.cpp) $(OBJS_codagedin.cpp) $(DEPLIBS_codagedin.cpp)
	$(LINK.cc) $(CCFLAGS_codagedin.cpp) $(CPPFLAGS_codagedin.cpp) -o $@ $(OBJS_codagedin.cpp) $(LDLIBS_codagedin.cpp)


# Compile source files into .o files
$(TARGETDIR_codagedin.cpp)/codage.o: $(TARGETDIR_codagedin.cpp) codage.cpp
	$(COMPILE.cc) $(CCFLAGS_codagedin.cpp) $(CPPFLAGS_codagedin.cpp) -o $@ codage.cpp

$(TARGETDIR_codagedin.cpp)/codagedin.o: $(TARGETDIR_codagedin.cpp) codagedin.cpp
	$(COMPILE.cc) $(CCFLAGS_codagedin.cpp) $(CPPFLAGS_codagedin.cpp) -o $@ codagedin.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_codagedin.cpp)/codagedin.cpp \
		$(TARGETDIR_codagedin.cpp)/codage.o \
		$(TARGETDIR_codagedin.cpp)/codagedin.o
	$(CCADMIN)
	rm -f -r $(TARGETDIR_codagedin.cpp)


# Create the target directory (if needed)
$(TARGETDIR_codagedin.cpp):
	mkdir -p $(TARGETDIR_codagedin.cpp)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Windows

