RM=/bin/rm
CC=/usr/bin/gcc
OUTPUT_PATH=../build
CFLAGS=-g -Wall -O3 -I /usr/local/include -L /usr/local/lib/mysql -l mysqlclient
MAKE=/usr/bin/make

all: GPS_client
GPS_client:
	${CC} ${CFLAGS} -o ${OUTPUT_PATH}/GPS_client GPS_client.c -lgps -lm
