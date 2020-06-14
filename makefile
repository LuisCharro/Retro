# Game Make

#To check the rules use: make -p 

#make CC=clang++ C=clang

#make DEBUG=1

############################################################################################################
### MACROS
############################################################################################################

# $(1) : Compiler
# $(2) : Object file to generate
# $(3) : Source file
# $(4) : Additional dependencies
# $(5) : Compiler flags

#file.o : file.c file.h
#	comp -c -o file.o file.c -Wall -pedantic 

define COMPILE_RULE
$(2) : $(3) $(4)
	$(1) -c -o $(2) $(3) $(5)
endef
#               $(1)  $(2)             $(3) $(4)             $(5)
#$(call COMPILE_RULE,$(CC),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(CCFLAGS) $(INCLUDE))

# $(eval $(call COMPILE_RULE, g++, fichero.o, fichero.c, fichero.h,-Wall -pedantic))
# $(eval $(call COMPILE_RULE, g++, fichero.o, fichero.c,,-Wall -pedantic))

# $(1) : Source file
define C2O
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1))))
endef

# $(1) : Header file
define C2H
$(patsubst %.c,%.h,$(patsubst %.cpp,%.hpp,$(1)))
endef

############################################################################################################
### CONFIG
############################################################################################################

APP := game

#Compiler
C       := gcc
CC      := g++
CFLAGS  := -Wall -pedantic
CCFLAGS := -Wall -pedantic -std=c++17

#Main folders
MKDIR      := mkdir -p
SRC        := src
OBJ        := obj

#LIBS
LIBDIR  := lib
LIBS    := -L/opt/X11/lib -lX11 -L/opt/X11/lib -lXext
INCLUDE := -I/opt/X11/include
INCDIRS := -I$(SRC) -I$(SRC)/$(LIBDIR)

ifdef DEBUG
	CCFLAGS += -g
else
	CCFLAGS += -O3
endif

#Files
ALLCPPS := $(shell find $(SRC) -type f -iname *.cpp)
ALLCS   := $(shell find $(SRC) -type f -iname *.c)
ALLOBJ  := $(foreach F,$(ALLCPPS) $(ALLCS),$(call C2O,$(F)))

#Folders
SUBDIRS    := $(shell find $(SRC) -type d)
OBJSUBDIRS := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIRS))

#To avoid continous generation (when no dependeces) -> Objective phony --> wihtout output file
.PHONY: info clean

#App it will be generaed from: all obj folder(s) and main
$(APP) : $(OBJSUBDIRS) $(ALLOBJ)
	$(CC) -o $(APP) $(ALLOBJ) $(LIBS) $(INCLUDE)

#Gereration of rules for each unit of translation
$(foreach F,$(ALLCS),$(eval $(call COMPILE_RULE,$(C),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(CFLAGS) $(INCLUDE) $(INCDIRS))))
$(foreach F,$(ALLCPPS),$(eval $(call COMPILE_RULE,$(CC),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(CCFLAGS) $(INCLUDE) $(INCDIRS))))

info:	
	$(info $(SUBDIRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCS))
	$(info $(ALLCPPS))
	$(info $(ALLOBJ))

clean:
	rm -f -r ./$(OBJ)
	rm -f game

#No dependences
$(OBJSUBDIRS) :
	$(MKDIR) $(OBJSUBDIRS)

#make clean; make DEBUG=1
#cgdb ./game
#lldb ./game

# Stops execution when we do a wrong usage of pointers/Heap usage
#-fsanitize=address -fno-ommit-frame-pointer

# -f noexception compilacion final

# git add src/ -f