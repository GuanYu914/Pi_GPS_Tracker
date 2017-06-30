#include <mcheck.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gps.h>
#include <mysql/mysql.h>
//Macros
#define HOST_NAME             "140.128.13.39"
#define DB_USER               "Bike_manager"
#define DB_USER_PWSD          "manager"
#define DB_NAME               "PU_Bike"
#define HOST_PORT             7777
//Instruction Macros
#define TABLE_NAME            "GPS_Info"
#define COL_NAME_1            "Latitude"
#define COL_NAME_2            "Longitude"
#define SEARCH_COL_NAME       "No_CAR"
#define SEARCH_COL_NAME_VALUE "\"PU_001\""
//Functions
void   DoubleToString(double, char *);
void   gps_Location(void);
void   mysql_Connect(void);
void   mysql_InstructionQuery(char *);
void   mysql_InstructionMerge_1(char *, char *, char *, char *, char *, char *, char *);
void   SIGINT_Handler(int);
//Global Objects
MYSQL  mysql;
//Global variable
loc_t  gps_data;
static volatile int keepRunning = 1;
char   lat[10];
char   lon[10];
char   newInstruction[50];
double loc_arr[2];


int main(void)
{
	//trace memory usage
	mtrace();
	//initialize gps
	gps_init();
	//initialize signal handler
	signal(SIGINT, SIGINT_Handler);
	//connect to specific mysql
	mysql_Connect();
	//get real location and transfer info to mysql table
	while(keepRunning)
	{
		gps_Location();
		DoubleToString(loc_arr[0], lat);
		DoubleToString(loc_arr[1], lon);
		mysql_InstructionMerge_1(TABLE_NAME, COL_NAME_1, lat, COL_NAME_2, lon, SEARCH_COL_NAME, SEARCH_COL_NAME_VALUE);
		mysql_InstructionQuery(newInstruction);
	}

	return EXIT_SUCCESS;
}

void SIGINT_Handler(int signum)
{
	keepRunning = 0;
	printf("Process %d got signal %d\n", getpid(), signum);
	printf("Kill process. \n");
	muntrace();
	kill(getpid(), signum);
	exit(EXIT_FAILURE);
}

void DoubleToString(double data, char *output)
{
	sprintf(output, "%lf", data);
}
void gps_Location(void)
{
	gps_location(&gps_data);
	loc_arr[0] = gps_data.latitude;
	loc_arr[1] = gps_data.longitude;
}

void mysql_Connect(void)
{
	if(mysql_init(&mysql) == NULL)
	{
		printf("ERROR: Failed to initate MySQL !\n");
	}
	if(!mysql_real_connect(&mysql,HOST_NAME,DB_USER,DB_USER_PWSD,DB_NAME,HOST_PORT,NULL,0))
	{
		printf("ERROR: Failed to connect MySQL: %s\n",mysql_error(&mysql));
	}else
	{
		printf("MySQL Server Ver is %s.\n",mysql_get_server_info(&mysql));
	}
}

void mysql_InstructionQuery(char *instruction)
{
	if(mysql_query(&mysql, instruction))
	{
		printf("ERROR: Failed to send mysql instruction: %s\n",mysql_error(&mysql));
	}else
	{
		printf("SUCES: Send mysql instruction successfully.\n");	
	}		
}

void mysql_InstructionMerge_1(char *table_name, char *col_name_1, char *col_value_1, char *col_name_2, char *col_value_2, char *search_col, char *search_col_name)
{
	/*This function only support to "UPDATE TABLE_NAME COL_NAME = , COL_NAME = WHERE COL_NAME = " instruction.*/

	strncpy(newInstruction, "UPDATE ", 8);
	strncat(newInstruction, table_name, strlen(table_name) + 1);
	strncat(newInstruction, " SET ", 5);
	strncat(newInstruction, col_name_1, strlen(col_name_1) + 1);
	strncat(newInstruction, " = ", 4);
	strncat(newInstruction, col_value_1, strlen(col_value_1) + 1);
	strncat(newInstruction, ", ", 4);
	strncat(newInstruction, col_name_2, strlen(col_name_2) + 1);
	strncat(newInstruction, " = ", 4);
	strncat(newInstruction, col_value_2, strlen(col_value_2) + 1);
	strncat(newInstruction, " WHERE ", 8);
	strncat(newInstruction, search_col, strlen(search_col) + 1);
	strncat(newInstruction, " = ", 4);
	strncat(newInstruction, search_col_name, strlen(search_col_name) + 1);	
}
