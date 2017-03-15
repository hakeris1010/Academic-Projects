CC = gcc

CFLAGS= -g
LDFLAGS= 

ifeq ($(OS),Windows_NT)
    LDFLAGS += -lWs2_32
endif

SOURCES_SERVER= src/server.c \
                src/gryltools/grylthread.c \
                src/gryltools/gsrvsocks.c

SOURCES_CLIENT= src/client.c

SERVNAME= server
CLINAME= client

BUILD_DIR= build

all: $(SERVNAME) $(CLINAME)

.c.o:
	$(CC) $(CFLAGS) -c $*.c -o $*.o

$(SERVNAME): $(SOURCES_SERVER:.c=.o)
	$(CC) -o $(BUILD_DIR)/$@ $^ $(LDFLAGS)

$(CLINAME): $(SOURCES_CLIENT:.c=.o)
	$(CC) -o $(BUILD_DIR)/$@ $^ $(LDFLAGS)

clean:
	$(RM) *.o */*.o */*/*.o */*/*/*.o

clean_all: clean
	$(RM) $(BUILD_DIR)/$(SERVNAME) $(BUILD_DIR)/$(CLINAME)
