CC := gcc
CFLAGS := -Wall -std=c99 -g

OBJECTS := interface.o main.o
EXEC_NAME := INFO1_U2

all: exec

exec: $(OBJECTS)
	$(CC) $^ -o $(EXEC_NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean_all: clean_obj
	rm -f $(EXEC_NAME) $(EXEC_NAME).exe
clean_obj:
	rm -f $(OBJECTS)

fresh: clean_all all
