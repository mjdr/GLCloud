CFLAGS := -Wall -Werror -std=c11
LN_LIBS=-lGL -lglut -lGLU -lm -L/usr/lib/ -lmariadb -lz -ldl -lm -lpthread -lssl -lcrypto

objs= \
main.o \
mat.o \
display.o \
db.o  


all: clear compile run
test: clear compile_test run_test
test_info: 
	./tests | grep Fail

compile:
	$(CC) -c $(objs:.o=.c)
	$(CC) -o gl $(objs) $(LN_LIBS)

clear:
	rm -f $(objs)
	rm -f gl
	rm -f tests
run:
	./gl
	

compile_test:
	$(CC) -c *.c
	$(CC) -o tests mat.o tests.o  -lm
	
run_test:
	clear
	./tests | grep Fail | wc -l
