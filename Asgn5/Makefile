#######################################################################
#
# Copyright (C) 2018-2022 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

################### DO NOT MODIFY THIS FILE ###########################
#
# It is not included in the subission archive ceated by 'make submit'.
#
# If you modify it and your code relies on those modifications, your code
# will not compile in the automated test harness and will be unable to
# execute any tests.
#
#######################################################################

CFLAGS = -g -Llib -Wall -Wextra -Iinclude -fno-exceptions -pthread
VFLAGS = --track-origins=yes --leak-check=full --show-leak-kinds=all
# C++ 20 via GCC 10 as 11 lcov is mildly broken
CC = g++-10 -std=c++20 

# Assignment No.
# a = 6
# Spring 2022 only
a = 5

SRC=$(wildcard src/*.cc)HEADERS = $(wildcard include/*.h)
SRC = src
OBJ = obj
SRCFILES = $(wildcard $(SRC)/*.cc)
SRCOBJS = $(addprefix $(OBJ)/, $(notdir $(patsubst $(SRC)/%.cc, %.o, $(SRCFILES))))

all: clean radix 

unsetcov: 
	$(eval TCFLAGS := -Wall)

setcov: 
	$(eval TCFLAGS := --coverage)

radix: unsetcov $(SRCOBJS) 
	$(CC) $(CFLAGS) $(TCFLAGS) -o radix $(SRCOBJS) -lradix 2>&1 | tee cc.out
	@cat cc.out >> make.out

test: clean setcov $(SRCOBJS)
	$(CC) $(CFLAGS) $(TCFLAGS) -o test $(SRCOBJS) -lradix 
	@-./test 1024 4 2
	@lcov --rc lcov_branch_coverage=1 -q --capture --directory obj --directory src --output-file coverage.info --no-external 2>/dev/null
	@genhtml coverage.info --branch-coverage --output-directory coverage

valgrind: clean radix
	valgrind $(VFLAGS) ./radix 1024 4 2 1>/dev/null 2>valgrind.out
	@tail valgrind.out
	@echo "****************************************"
	@echo "***** See valgrind.out for details *****"
	@echo "****************************************"

$(OBJ)/%.o: $(SRC)/%.cc $(HEADERS)
	$(CC) $(CFLAGS) $(TCFLAGS) -c $< -o $@ 2>&1 | tee cc.out
	@cat cc.out >> make.out

check: radix
	@./check.sh 10000

perf: radix
	@./perf.sh 100000

grade: clean radix 
	@./grade.sh $(a) 100000

clean:
	@rm -f radix cc.out make.out obj/*

submit: clean
	@echo ""
	@echo "#### Only these file are submitted:"
	@echo ""
	@tar czvf ~/CSE111-Assignment$(a).tar.gz src/*
	@echo ""
	@echo "##### Confirming submission is good..."
	@echo ""
	@./confirm.sh $(a)
	@echo ""
	@echo "#### If that failed, fix the problem and try again."
	@echo ""
	@echo "Finally, don't forget to upload ~/CSE111-Assignment$(a).tar.gz to Canvas!"
	@echo ""
