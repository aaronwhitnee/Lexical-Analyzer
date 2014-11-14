P1.out : Project1.o SetLimits.o
	g++ Project1.o SetLimits.o -o P1.out -g

SetLimits.o : SetLimits.c SetLimits.h
	g++ -c SetLimits.c -g

Project1.o : Project1.cpp LexicalAnalyzer.hpp table.h
	g++ -c Project1.cpp -g

run :
	./P1.out test1.in

clean :
	rm -f core.* *.o P1.out *.tgz