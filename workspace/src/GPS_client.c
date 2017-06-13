#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gps.h>
#include <mysql/mysql.h>
//Macro
#define HOST_NAME    "140.128.13.39"
#define DB_USER      "Bike_manager"
#define DB_USER_PWSD "manager"
#define DB_NAME      "PU_Bike"
#define HOST_PORT    7777
//Function
char *   DoubleToString(double);
double * get_Location(void);
void     mysql_Connect(void);
void     mysql_InstructionQuery(char *);
char *   mysql_InstructionMerge(char *, int, char *, int, char *);
//Global Objects
MYSQL mysql;
loc_t gps_data;
//Global Variable
double loc_arr[2];

int main(void)
{
	gps_init();
	mysql_Connect();
	while(1)
	{
		double lat = get_Location()[0];
		double lon = get_Location()[1];
		printf("latitude:  %lf\r\nlongitude: %lf\r\n", lat, lon);	
		mysql_InstructionMerge(DoubleToString(lat), 0, DoubleToString(lon), 0,  "INSERT INTO GPS_Info VALUES()");
		break;
	}
	mysql_close(&mysql);	
	return EXIT_SUCCESS;
}

char * DoubleToString(double x)
{
	char s[20] = "";
	sprintf(s, "%lf", x);
	return s;
}

double * get_Location(void)
{
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
		printf("MySQL Server Ver is %s\n",mysql_get_server_info(&mysql));
	}
}

void mysql_InstructionQuery(char *instruction)
{
	if(mysql_query(&mysql, instruction))
	{
		printf("ERROR: Failed to send mysql instruction: %s\n",mysql_error(&mysql));
	}else
	{
		printf("SUCES: Successfully send mysql instructions\n");	
	}		
}

char * mysql_InstructionMerge(char *substr1, int pos1, char *substr2, int pos2, char *Instruction)
{
	//char newInstruction[100];
	//newInstruction = strcat(Instruction, substr1);
	//newInstruction = strcat(newInstruction, substr2);
	//printf("%s\n", newInstruction);
	return "test";		
}
