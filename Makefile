OBJS = log_manager.o
CC = g++
DEBUG = -g
BOOSTFLAGS = -I -lboost_system
CFLAGS = -Wall -c $(DEBUG) -std=c++11
LFLAGS = -Wall $(DEBUG)

logserver : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o logserver -lboost_system -lboost_filesystem

log_manager.o : log_manager.h log_manager.cc utils.h
	$(CC) $(CFLAGS) log_manager.cc

log_server.o : log_server.cc log_manager.h
	$(CC) $(CFLAGS) log_server.cc

clean:
	\rm -f *.o logserver *.log logServerIndex
