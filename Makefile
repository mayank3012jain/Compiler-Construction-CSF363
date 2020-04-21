compiler: driver.c parser.c lexer.c ast.c symbolTable.c semanticAnalyser.c code_gen.c exprn_code_gen.c lexerDef.h lexer.h parser.h parserDef.h astDef.h symbolTableDef.h ast.h symbolTable.h codeGen.h
	gcc -g driver.c parser.c lexer.c ast.c symbolTable.c semanticAnalyser.c code_gen.c exprn_code_gen.c -o compiler

