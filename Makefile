stage1exe: driver.c parser.c lexer.c lexerDef.h lexer.h parser.h parserDef.h
	gcc driver.c parser.c lexer.c -o stage1exe