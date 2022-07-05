#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "staff.c"
#pragma warning(disable: 4996)
typedef struct {
	char ID[5];
	char type[16];
	char description[31];
	char venue[16];
	int maxUser;
	char status[12];
}Facility;

int compareID(const void* a, const void* b);
int extractCurrentIDNo(char input[]);
void membershipEndDate(int duration, int sDay, int sMonth, int sYear, int* eDay, int* eMonth, int* eYear);

void main() {
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysDay = time.tm_mday;
	int sysYear = time.tm_year + 1900;
	int hour = time.tm_hour;
	int minute = time.tm_min;
	int testMin = 06;


	printf("%d",hour);
	printf("%d",minute);

	if()

	/*
	int eDay, eMonth, eYear, duration = 18;
	membershipEndDate(12, 29, 2, 2024, &eDay, &eMonth, &eYear);
	printf("%02d-%02d-%d",eDay,eMonth,eYear);
	
	
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysYear = time.tm_year + 1900;
	int sysDay = time.tm_mday;
	char tempDay[3], tempMonth[3], tempYear[5];
	strcpy(tempDay,itoa(sysDay, tempDay, 10));
	strcpy(tempMonth, itoa(sysMonth, tempMonth, 10));
	strcpy(tempYear, itoa(sysYear, tempYear,10));
	printf("%02d %02d %d\n",sysDay, sysMonth, sysYear);
	printf("%s %s %s\n", tempDay,tempMonth,tempYear);
	*/
	/*
	char test[5] = "0001";
	int testing = atoi(test);

	printf("%d", testing);*/
}


int compareID(const void* a, const void* b) {
	return (strcmp((char*)a, (char*)b));
}

int extractCurrentIDNo(char input[]) {
	//extract the ID sequence 

	int sequence = 0;
	if (strlen(input) == 3) {
		//assume input[2] = '1' , in asscii table value of '1' = 49,'0' = 48 ,
		//using '1' - '0' to get the actual digit
		sequence = input[2] - '0'; // convert string type to int , for comparison purpose.
	}

	if (strlen(input) == 4) {
		char temp[3] = { input[2],input[3] };
		sequence = atoi(temp);
	}

	return sequence;
}

void membershipEndDate(int duration, int sDay, int sMonth, int sYear, int* eDay, int* eMonth, int* eYear) {
	*eDay = sDay;
	*eMonth = sMonth;
	*eYear = sYear;

	*eMonth += duration;
	if (*eMonth > 12 && *eMonth < 25) {
		*eMonth -= 12;
		++* eYear;
	}
	if (*eMonth >= 25 && *eMonth < 37) {
		*eMonth -= 24;
		*eYear += 2;
	}
	if (*eMonth >= 37 && *eMonth < 49) {
		*eMonth -= 36;
		*eYear += 3;
	}

	if ((sMonth == 3 || sMonth == 5) && (duration == 6 || duration == 18)) {
		if (sDay == 31) {
			*eDay = 30;
		}
	}

	if (sMonth == 8 && (duration == 6 || duration == 18)) {
		if (((*eYear % 4 == 0) && ((*eYear % 400 == 0) || (*eYear % 100 != 0)))) {
			if (sDay >= 29 && sDay <= 31) {
				*eDay = 29;
			}
		}
		else {
			if (sDay >= 29 && sDay <= 31) {
				*eDay = 28;
			}
		}
	}

	if (sMonth == 2) {
		if (((sYear % 4 == 0) && ((sYear % 400 == 0) || (sYear % 100 != 0)))) {
			if (sDay == 29) {//if start day is on Feb 29, then set end day to become 28
				//since the maximum duration allowed to apply is 3 years, it is impossible to
				//meet another leap year.
				*eDay = 28;
			}
		}
	}
}