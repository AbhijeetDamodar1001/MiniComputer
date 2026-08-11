CC = gcc

TARGET = simulator

OBJ = main.o compiler.o memory.o processor.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

main.o: main.c
	$(CC) -c main.c

compiler.o: compiler.c
	$(CC) -c compiler.c

memory.o: memory.c
	$(CC) -c memory.c

processor.o: processor.c
	$(CC) -c processor.c

clean:
	del *.o $(TARGET).exe
