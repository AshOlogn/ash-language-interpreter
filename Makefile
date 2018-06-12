# Makefile for compiling the interpreter and cleaning binaries 

source = main.cpp \
	token.cpp \
	lexer.cpp \
	parsetoken.cpp \
	parsenode.cpp \
	arithmeticeval.cpp \
	assignmenteval.cpp \
	bitlogicaleval.cpp \
	comparisoneval.cpp \
	groupeval.cpp \
	literaleval.cpp \
	unaryeval.cpp \
	evaluator.cpp \
	statementnode.cpp \
	typehandler.cpp \
	parser.cpp
	
include = token.h \
	lexer.h \
	parsetoken.h \
	parsenode.h \
	arithmeticeval.h \
	assignmenteval.h \
	bitlogicaleval.h \
	comparisoneval.h \
	groupeval.h \
	literaleval.h \
	unaryeval.h \
	evaluator.h \
	statementnode.h \
	typehandler.h \
	parser.h

bin/main: $(addprefix src/, $(source)) $(addprefix include/, $(include))
	g++ -std=c++11 -Iinclude -o bin/main $(addprefix src/, $(source))
      
.PHONY: clean
clean:
	@echo cleaning...
	rm bin/main
	  
	  
	  