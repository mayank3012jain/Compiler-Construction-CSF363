stage1exe: driver.c parser.c lexer.c lexerDef.h lexer.h parser.h parserDef.h
	gcc driver.c parser.c lexer.c -o stage1exe

stage2ast: driver.c parser.c lexer.c ast.c lexerDef.h lexer.h parser.h parserDef.h astDef.h ast.h 
	gcc driver.c parser.c lexer.c ast.c -o stage2ast

stage2astdebug: driver.c parser.c lexer.c ast.c lexerDef.h lexer.h parser.h parserDef.h astDef.h ast.h 
	gcc -g driver.c parser.c lexer.c ast.c -o stage2ast

stage2symboldebug: driver.c parser.c lexer.c ast.c symbolTable.c semanticAnalyser.c lexerDef.h lexer.h parser.h parserDef.h astDef.h symbolTableDef.h ast.h symbolTable.h
	gcc -g driver.c parser.c lexer.c ast.c symbolTable.c semanticAnalyser.c -o stage2symbol

stage2codedebug: driver.c parser.c lexer.c ast.c symbolTable.c semanticAnalyser.c codeGenStack.c exprnCodeGen.c lexerDef.h lexer.h parser.h parserDef.h astDef.h symbolTableDef.h ast.h symbolTable.h codeGen.h
	gcc -g driver.c parser.c lexer.c ast.c symbolTable.c semanticAnalyser.c codeGenStack.c exprnCodeGen.c -o stage2code

stage2code_debug: driver.c parser.c lexer.c ast.c symbolTable.c semanticAnalyser.c code_gen.c exprn_code_gen.c lexerDef.h lexer.h parser.h parserDef.h astDef.h symbolTableDef.h ast.h symbolTable.h codeGen.h
	gcc -g driver.c parser.c lexer.c ast.c symbolTable.c semanticAnalyser.c code_gen.c exprn_code_gen.c -o stage2code

stage2codeOLD: driver.c parser.c lexer.c ast.c symbolTable.c semanticAnalyser.c code_gen_old.c exprn_code_gen.c lexerDef.h lexer.h parser.h parserDef.h astDef.h symbolTableDef.h ast.h symbolTable.h codeGen.h
	gcc -g driver.c parser.c lexer.c ast.c symbolTable.c semanticAnalyser.c code_gen_old.c exprn_code_gen.c -o stage2code