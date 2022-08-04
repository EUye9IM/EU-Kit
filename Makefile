CXX			:= g++
CXX_FLAGS	:= -Wall -Werror -O2 -std=c++11
EX_NAME		:= cpp

SRC			:= src
INCLUDE		:= include

OUTS		:= EUkitlib.a logclib.a inilib.a

ifeq ($(OS),Windows_NT)
# PLATFORM = "Windows"
RM			:= del /s /q /f 
FIND_FILE	= $(shell dir /b /s /a-d "$(1)\*.$(2)")
else
#  PLATFORM = "Unix-Like"
RM			:= rm -f
FIND_FILE	= $(shell find $(1) -type f | grep ".$(2)$$" --color=never)
endif

GET_OBJS	= $(patsubst %.$(EX_NAME), %.o,$(call FIND_FILE,$(1),$(EX_NAME)))
ALL_OBJ		= $(call GET_OBJS,$(SRC))

.PHONY: all clean
all: $(OUTS)
	@echo Done

EUkitlib.a: $(ALL_OBJ)
	@echo Linking $@
	@ar cr $@ $^

logclib.a: $(call GET_OBJS,$(SRC)/logc)
	@echo Linking $@
	@ar cr $@ $^

inilib.a: $(call GET_OBJS,$(SRC)/ini)
	@echo Linking $@
	@ar cr $@ $^

%.o: %.cpp
	@echo Compiling $@
	@$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $< -o $@

clean:
	@echo remove $(ALL_OBJ)
	@$(RM) $(ALL_OBJ)
	@echo remove $(OUTS)
	@$(RM) $(OUTS)
