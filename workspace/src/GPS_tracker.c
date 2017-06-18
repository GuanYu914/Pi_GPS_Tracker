#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gps.h>
#include <mysql/mysql.h>
//Macros
#define HOST_NAME       "140.128.13.39"
#define DB_USER         "Bike_manager"
#define DB_USER_PWSD    "manager"
#define DB_NAME         "PU_Bike"
#define HOST_PORT       7777
//Instruction Macros
#define TABLE_NAME            "GPS_Info"
#define COL_NAME_1            "Latitude"
#define COL_NAME_2            "Longitude"
#define SEARCH_COL_NAME       "No_CAR"
#define SEARCH_COL_NAME_VALUE "\"PU_001\""
//Functions
char   *DoubleToString(double);
char   *Substring(char *, int, int);
double *GetLocation(void);
void    mysql_Connect(void);
void    mysql_InstructionQuery(char *);
char   *mysql_InstructionMerge_1(char *, char *, char *, char *, char *, char *, char *);
//Global Objects
MYSQL mysql;
loc_t gps_data;

int main(void)
{
	gps_init();
	mysql_Connect();
	while(1)
	{
		double lat = GetLocation()[0];
		double lon = GetLocation()[1];
		mysql_InstructionQuery(mysql_InstructionMerge_1(TABLE_NAME, COL_NAME_1, DoubleToString(lat), COL_NAME_2, DoubleToString(lon), SEARCH_COL_NAME, SEARCH_COL_NAME_VALUE));
	}
	mysql_close(&mysql);	
	return EXIT_SUCCESS;
}

char *DoubleToString(double x)
{
	char *str = malloc(20 * sizeof(char));
	sprintf(str, "%lf", x);
	return str;
}

char *Substring(char *string, int pos, int len)
{
	char *ptr;
	int c;
	ptr = malloc(len+1);
	if(ptr == NULL)
	{
		exit(EXIT_FAILURE);
	}
	for(c = 0; c < len; c++ )
	{
		*(ptr + c) = *((string+pos-1)+c);	
	}
	*(ptr+c) = '\0';
	
	return ptr;
}

double *GetLocation(void)
{
	double *loc_arr = malloc(2 * sizeof(double));
	gps_location(&gps_data);
	loc_arr[0] = gps_data.latitude;
	loc_arr[1] = gps_data.longitude;
	return loc_arr;
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

char *mysql_InstructionMerge_1(char *table_name, char *col_name_1, char *col_value_1, char *col_name_2, char *col_value_2, char *search_col, char *search_col_name)
{
	/*This function only support to "UPDATE TABLE_NAME COL_NAME = , COL_NAME = WHERE COL_NAME = " instruction.*/
	
	char *newInstruction = malloc(45 * sizeof(char));

	strcpy(newInstruction, "UPDATE ");
	strcat(newInstruction, table_name);
	strcat(newInstruction, " SET ");
	strcat(newInstruction, col_name_1);
	strcat(newInstruction, " = ");
	strcat(newInstruction, col_value_1);
	strcat(newInstruction, ", ");
	strcat(newInstruction, col_name_2);
	strcat(newInstruction, " = ");
	strcat(newInstruction, col_value_2);
	strcat(newInstruction, " WHERE ");
	strcat(newInstruction, search_col);
	strcat(newInstruction, " = ");
	strcat(newInstruction, search_col_name);	
	
	return newInstruction;
}
