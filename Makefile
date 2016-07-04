
CC = gcc

OFLAGS = -g -O3 -fpic -Wall
LFLAGS = -g
CFLAGS = -I/usr/include/php5 -I/usr/include/php5/Zend -I/usr/include/php5/main -I/usr/include/php5/regex -I/usr/include/php5/ext \
	 -I/usr/include/php5/TSRM

CFLAGS += $(OFLAGS)

OBJS =  mod_alarm.o

mod_alarm.so: mod_alarm.o
	@echo "linking"; \
	gcc -shared -rdynamic -Wall -O3 -o mod_alarm.so mod_alarm.o

mod_alarm.o: mod_alarm.c
	@echo "compiling mod_alarm.c"; \
	$(CC) $(CFLAGS) -lstdc++ -c -o mod_alarm.o mod_alarm.c
