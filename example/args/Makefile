CXX			:= clang++
CXX_FLAGS	:= -Wall -Werror -g -std=c++11
EX_NAME		:= cpp

SRC			:= ../../src
INCLUDE		:= ../../include

OUTS		:= test.exe

MODNAME		:= args

SRCS		:= $(wildcard *.$(EX_NAME)) $(wildcard $(SRC)/$(MODNAME)/*.$(EX_NAME))
OBJS		:= $(patsubst %.$(EX_NAME), %.o, $(SRCS))

ifeq ($(OS),Windows_NT)
# PLATFORM = "Windows"
RM			:= del /s /q /f 
else
#  PLATFORM = "Unix-Like"
RM			:= rm -f
endif

.PHONY: all clean
all: $(OUTS)
	@echo Done

$(OUTS): $(OBJS)
	@echo Linking $@
	@$(CXX) $(CXX_FLAGS) $^ -o $@

%.o: %.cpp
	@echo Compiling $@
	@$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $^ -o $@

clean:
	@echo remove $(OUTS)
	@$(RM) $(OUTS)
	@echo remove *.o
	@$(RM) *.o
	

test:
	echo $(OBJS)