OBJS = LogManager.o LogEntry.o LogFile.o LogIndex.o
CC = g++
DEBUG = -g
BOOSTFLAGS = -I -lboost_system
CFLAGS = -Wall -Wno-unused-function -c $(DEBUG) -std=c++11
LFLAGS = -Wall $(DEBUG)

logserver : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o logserver -lboost_system -lboost_filesystem

LogManager.o : LogManager.h utils.h
	$(CC) $(CFLAGS) LogManager.cc

LogEntry.o : LogEntry.h
	$(CC) $(CFLAGS) LogEntry.cc

LogIndex.o : LogIndex.h
	$(CC) $(CFLAGS) LogIndex.cc

LogFile.o : LogFile.h
	$(CC) $(CFLAGS) LogFile.cc

clean:
	\rm -f *.o logserver *.log logServerIndex
