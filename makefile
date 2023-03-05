PROGRAM=freedom
FLAGS=-std=c++20 -O3 -fsanitize=address,leak,undefined -Wall -Wextra -fuse-ld=gold

$(PROGRAM): main.o prog.o
	g++ $(FLAGS) *.o -o $(PROGRAM)

main.o: main.cpp
	g++ $(FLAGS) -c main.cpp

prog.o: src/*.cpp include/*.h
	g++ $(FLAGS) -c src/*.cpp

clean:
	rm *.o $(PROGRAM)
