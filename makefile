a9: graph.o main.cpp
	g++ -o trains.out main.cpp graph.o

main.o: graph.o main.cpp
	g++ -o trains.out main.cpp graph.o

graph.o: graph.h graph.cpp
	g++ -c -o graph.o graph.cpp

clean: 
	rm *.o
