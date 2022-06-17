CXX			:= g++
CXX_FLAGS	:= -Wall -Werror -O2 -std=c++11
EX_NAME		:= cpp

SRC			:= src
INCLUDE		:= include

OUTS		:= EUkitlib.a logclib.a

ifeq ($(OS),Windows_NT)
# PLATFORM = "Windows"
RM			:= del /s /q /f 
# RMDIR		:= rmdir /s /q
SRCS		:= $(shell dir /b /s /a-d "$(SRC)\*.$(EX_NAME)")
else
#  PLATFORM = "Unix-Like"
RM			:= rm -f
# RMDIR		:= rm -rf
SRCS		:= $(shell find $(SRC) -type f | grep ".$(EX_NAME)$$" --color=never)
endif

OBJS		:= $(patsubst %.$(EX_NAME), %.o,$(SRCS))

.PHONY: all clean
all: $(OUTS)
	@echo Done

EUkitlib.a: $(OBJS)
	@echo Linking $@
	@ar cr $@ $^

logclib.a: $(SRC)/logc/logc.$(EX_NAME)
	@echo Linking $@
	@ar cr $@ $^

%.o: %.cpp
	@echo Compiling $@
	@$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $< -o $@

clean:
	@echo remove $(OBJS)
	@$(RM) $(OBJS)
	@echo remove $(OUTS)
	@$(RM) $(OUTS)