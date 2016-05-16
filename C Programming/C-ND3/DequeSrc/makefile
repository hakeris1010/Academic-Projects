CC= gcc
LC= ar
CFLAGS= -std=c99 -g

INCLUDE=  -I"include/"
LIBCLUDE= -L"lib/"

LIBOBJ_1= src/Impl1/deque1.o src/Impl1/arraystack.o
LIBOBJ_2= 
TESTOBJ=  test/main.o

NAME_1= Deque1337_1
NAME_2= Deque1337_2
LIBNAME_1= lib/lib$(NAME_1).a
LIBNAME_2= lib/lib$(NAME_2).a

all: exec1 exec2

exec1: $(NAME_1)
exec2: $(NAME_2)
lib1: $(LIBNAME_1)
lib2: $(LIBNAME_2)

$(NAME_1): $(LIBNAME_1) $(TESTOBJ)
	$(CC) $(LIBCLUDE) $(TESTOBJ) $(LIBNAME_1) -o $@

$(NAME_2): $(LIBNAME_2) $(TESTOBJ)
	$(CC) $(LIBCLUDE) $(TESTOBJ) $(LIBNAME_2) -o $@

$(LIBNAME_1): $(LIBOBJ_1)
	$(LC) rvs $(LIBNAME_1) $(LIBOBJ_1)

$(LIBNAME_2): $(LIBOBJ_2)
	$(LC) rvs $(LIBNAME_2) $(LIBOBJ_2)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

#-----------------------------------------------------

clean: clean_o clean_exe_lib
clean_o:
	rm -f *.o */*.o */*/*.o
clean_exe_lib:
	rm -f *.exe *.a */*.exe */*.a

fresh: clean all
fresh_lib1: clean lib1
fresh_lib2: clean lib2
fresh_exec1: clean exec1
fresh_exec2: clean exec2
