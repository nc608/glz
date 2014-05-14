ifeq ($(OS), Windows_NT)
	CC = gcc
	CPP = g++
	OUT = cvec.exe
else
	CC = clang
	CPP = clang++
	OUT = gl
endif
CFLAGS = -Wall -Wno-write-strings -g
CPPFLAGS = -Wall -Wno-write-strings -g
OBJ_DIR = obj
BIN_DIR = .
LIBS = X11 \
	   GL \
	   GLEW

#object lists
C_SOURCES	=
CPP_SOURCES	= start.cpp \
			  xwin_t.cpp \
			  rn_t.cpp

OBJS := $(patsubst %, $(OBJ_DIR)/%, $(C_SOURCES:%.c=%.o))
OBJS += $(patsubst %, $(OBJ_DIR)/%, $(CPP_SOURCES:%.cpp=%.o))
#BINS := $(patsubst %, $(BIN_DIR)/%, $(OUT))

all: BIN 

clean:
ifeq ($(OS), Windows_NT)
	del /Q $(OBJ_DIR)\*
	del /Q $(BIN_DIR)\$(OUT)
else
	rm -f $(OBJ_DIR)/*
	rm -f $(BIN_DIR)/$(OUT)
endif

#make the c files
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

#make the cpp files
$(OBJ_DIR)/%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

BIN: $(OBJS)
	$(CPP) $(CPPFLAGS) $(foreach i,$(LIBS),-l $i) $(OBJS) -o $(OUT)

