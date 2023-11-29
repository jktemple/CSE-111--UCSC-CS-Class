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

# Supress exceptions so C++ does not create unnecessary branches
CFLAGS = -Wall -Wextra -Iinclude -fno-exceptions
TCFLAGS = --coverage 
TCLIBS = -lgtest_main -lgtest -lpthread
# C++ 20 via GCC 10 as 11 lcov is mildly broken
CC = g++-10 -std=c++20

# Assignment No.
a = 3

HEADERS = $(wildcard include/*.h)

SRC = src
OBJ = obj
TST = tests

COMFILES = $(wildcard comp/*.cc)
SRCFILES = $(wildcard $(SRC)/*.cc)
TSTFILES = $(wildcard $(TST)/*.cc)
SRCOBJS = $(addprefix $(OBJ)/, $(notdir $(patsubst $(SRC)/%.cc, %.o, $(SRCFILES))))
TSTOBJS = $(addprefix $(OBJ)/, $(notdir $(patsubst $(TST)/%.cc, %.o, $(TSTFILES))))

all: bounds

bounds: $(SRCOBJS) $(TSTOBJS)
	$(CC) $(CFLAGS) $(TCFLAGS) -o $@ $^ $(TCLIBS) 2>&1 | tee cc.out
	@cat cc.out >> make.out

test: bounds
	@-./bounds 
	@lcov --rc lcov_branch_coverage=1 -q --capture --directory obj --directory src --output-file coverage.info --no-external
	@genhtml coverage.info --branch-coverage --output-directory coverage

$(OBJ)/%.o: $(SRC)/%.cc $(HEADERS)
	$(CC) $(CFLAGS) $(TCFLAGS) -c $< -o $@ 2>&1 | tee cc.out
	@cat cc.out >> make.out

$(OBJ)/%.o: $(TST)/%.cc $(HEADERS)
	$(CC) $(CFLAGS) $(TCFLAGS) -c $< -o $@ 2>&1 | tee cc.out
	@cat cc.out >> make.out

clean:
	@rm -rf compliance bounds make.out cc.out valgrind.out coverage.info obj/* coverage/*

compliance: $(SRCFILES) $(COMFILES)
	@echo ""
	@echo "#### Checking compliance:"
	@echo ""
	$(CC) -o $@ $^ $(CFLAGS) 

submit: clean compliance clean
	@echo ""
	@echo "#### Only these file are submitted:"
	@echo ""
	@tar czvf ~/CSE111-Assignment$(a).tar.gz \
				--exclude=include/Containable.h \
				--exclude=include/Point.h \
				src include/*.h tests/* 
	@echo ""
	@echo "##### Confirming submission is good..."
	@./confirm.sh $(a)
	@echo ""
	@echo "#### If that failed, fix the problem and try again."
	@echo ""
	@echo "Finally, don't forget to upload ~/CSE111-Assignment$(a).tar.gz to Canvas!"
	@echo ""

grade: 
	@./grade.sh $a
