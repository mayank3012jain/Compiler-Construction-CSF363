stage1exe: driver.c parser.c lexer.c lexerDef.h lexer.h parser.h parserDef.h
	gcc driver.c parser.c lexer.c -o stage1exe

stage2ast: driver.c parser.c lexer.c ast.c lexerDef.h lexer.h parser.h parserDef.h astDef.h ast.h 
	gcc driver.c parser.c lexer.c ast.c -o stage2ast

stage2astdebug: driver.c parser.c lexer.c ast.c lexerDef.h lexer.h parser.h parserDef.h astDef.h ast.h 
	gcc -g driver.c parser.c lexer.c ast.c -o stage2ast