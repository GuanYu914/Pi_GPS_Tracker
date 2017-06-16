RM=/bin/rm
CC=/usr/bin/gcc
OUTPUT_PATH=../build
CFLAGS=-g -Wall -O3 
CFLAGS_MySQL=-lmysqlclient
CFLAGS_GPS=-lgps -lm  

all: BUILD

BUILD:
	${CC} ${CFLAGS} -o ${OUTPUT_PATH}/GPS_tracker GPS_tracker.c ${CFLAGS_GPS} ${CFLAGS_MySQL}
	${CC} ${CFLAGS} -o ${OUTPUT_PATH}/simpleGPS simpleGPS.c ${CFLAGS_GPS}
