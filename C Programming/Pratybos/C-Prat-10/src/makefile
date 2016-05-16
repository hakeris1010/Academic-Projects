OBJECTS= main.o f.o
CFLAGS= -Wall -O2
CC= gcc



all: $(OBJECTS)

	$(CC) $(OBJECTS) -o prog10

*.o: *.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.i *.s *.exe

rebuild: clean all
