#the path for different kinds of sources
LIB_DEST = ./lib
OBJ_DIR = ./obj
SRC_DIR = ./src
INCLUDE_DIR = ./include/

#set the compile tool
CC = g++
CC_FLAG = -I$(INCLUDE_DIR) -Wall -std=c++11

#for compiling a static library
#r(replace) c(create) a(append) ars(ranlib)
AR = ar rc

#the path and file of different sources
#wildcard is used to auto-complete a string.
#notdir is used to remove the direct of a path.
#patsubst is used to change the last item from 
#the first pattern into the second.
SRC_CPP = $(wildcard $(SRC_DIR)/*.cpp)
CPP = $(notdir $(SRC_CPP))
OBJECT = $(patsubst %.cpp,%.o,$(CPP))
SRC_OBJ = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(CPP))
LIB = $(LIB_DEST)/libmultid.a

#default object
#all: $(LIB)
all: $(LIB)
	@echo src_obj $(SRC_CPP)
	@echo src_obj $(SRC_OBJ)
	@echo object $(OBJECT)
	@echo cppfile $(CPP)
	@echo lib $(LIB)

#inform make of the search path and file format 
vpath %.cpp $(SRC_DIR)
vpath %.o $(OBJ_DIR)
#make the object file
$(filter %.o,$(OBJECT)):%.o:%.cpp
	$(CC) -c $(CC_FLAG) $< -o $(OBJ_DIR)/$@
vpath %.o $(OBJ_DIR)
#make the static library
#problem might occur when there are directs in the OBJECT
#the dependencies do not allow use directs.
#but the command line can use direct to specific the object file.
$(LIB): $(OBJECT)
	@$(AR) $@ $(SRC_OBJ)
