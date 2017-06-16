#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gps.h>
#include <mysql/mysql.h>
//Macros
#define HOST_NAME    "140.128.13.39"
#define DB_USER      "Bike_manager"
#define DB_USER_PWSD "manager"
#define DB_NAME      "PU_Bike"
#define HOST_PORT    7777
//Functions
char   *DoubleToString(double);
char   *substring(char *, int, int);
double *get_Location(void);
void    mysql_Connect(void);
void    mysql_InstructionQuery(char *);
char   *mysql_InstructionMerge_1(char *, char *, char *);
//Global Objects
MYSQL mysql;
loc_t gps_data;

int main(void)
{
	gps_init();
	mysql_Connect();
	while(1)
	{
		//double lat = get_Location()[0];
		//double lon = get_Location()[1];
		char *instruction_1 = malloc(5 * sizeof(char)); 
		instruction_1 = "INSERT INTO GPS_Info VALUES()";
		mysql_InstructionQuery(mysql_InstructionMerge_1(DoubleToString(23.111), DoubleToString(120.586), instruction_1));
		break;
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

char *substring(char *string, int pos, int len)
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

double *get_Location(void)
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

char *mysql_InstructionMerge_1(char *substr1, char *substr2, char *Instruction)
{
	/*This function only support to "INSERT INTO TABLE_NAME VALUES()" instruction.*/
	
	
	char *f, *e;
	int len = strlen(Instruction);
	int pos = len;
	
	char *str = malloc(15 * sizeof(char));
	char *newInstruction = malloc(45 * sizeof(char));
	strcpy(str, "");
	strcat(str, substr1);
	strcat(str, ", ");
	strcat(str, substr2);
	printf("%s\n", str);

	//printf("%d\n", len);
	f = substring(Instruction, 1, pos - 1);
	//printf("%s\n", f);
	e = substring(Instruction, pos, len - pos + 1);
	//printf("%s\n", e);
	strcpy(newInstruction, "");
	strcat(newInstruction, f);
	free(f);
	strcat(newInstruction, str);
	free(str);
	strcat(newInstruction, e);
	free(e);
	printf("%s\n", newInstruction);

	return newInstruction;
}
