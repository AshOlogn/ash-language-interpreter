# Makefile for compiling the interpreter and cleaning binaries 

source = main.cpp \
	errors.cpp \
	exceptions.cpp \
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
	functioneval.cpp \
	memberaccesseval.cpp \
	casteval.cpp \
	arrayeval.cpp \
	evaluator.cpp \
	statementnode.cpp \
	typehandler.cpp \
	symboltable.cpp \
	executor.cpp \
	utils.cpp \
	array.cpp \
	parser.cpp
	
include = token.h \
	errors.h \
	exceptions.h \
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
	memberaccesseval.h \
	functioneval.h \
	casteval.h \
	arrayeval.h \
	evaluator.h \
	statementnode.h \
	typehandler.h \
	symboltable.h \
	executor.h \
	function.h \
	array.h \
	utils.h \
	parser.h

bin/main: $(addprefix src/, $(source)) $(addprefix include/, $(include))
	g++ -std=c++11 -Iinclude -o bin/ash $(addprefix src/, $(source))
      
.PHONY: clean
clean:
	@echo cleaning...
	rm bin/ash