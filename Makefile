CC = g++
CFLAGS = -g -Wall -ldl -lGL -lOpenGL -lGLU -lglfw
OBJ = glad.c main.o shader.o target.o bolt.o fire.o

arcane: $(OBJ)
	$(CC) $(CFLAGS) -o arcane $(OBJ)

main.o: main.cpp collision.h target.h bolt.h fire.h
	$(CC) -g -c main.cpp
shader.o: shader.cpp shader.h
	$(CC) -g -c shader.cpp
bolt.o: bolt.cpp bolt.h
	$(CC) -g -c bolt.cpp
fire.o: fire.cpp fire.h
	$(CC) -g -c fire.cpp
target.o: target.cpp target.h
	$(CC) -g -c target.cpp

clean:
	rm -f *.o arcane
