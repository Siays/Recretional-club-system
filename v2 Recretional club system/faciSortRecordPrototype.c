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

void main() {
	Facility facility[100];
	FILE* readFaci;
	readFaci = fopen("facilities.txt", "r");
	int totalCount = 0;
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility[totalCount].ID, facility[totalCount].type, facility[totalCount].description, facility[totalCount].venue, &facility[totalCount].maxUser, facility[totalCount].status);
		++totalCount;
	}
	fclose(readFaci);
	Facility tempFaci;
	//sort the record according to alphabetic order first
	for (int i = 0; i < totalCount; i++) {
		for (int j = i + 1; j < totalCount; j++) {
			if (strcmp(facility[i].ID, facility[j].ID) > 0) {
				tempFaci = facility[i];
				facility[i] = facility[j];
				facility[j] = tempFaci;
			}
		}
	}
	for (int i = 0; i < totalCount; i++) {
		printf("%s\n", facility[i].ID);
	}
	//sort the record according to the ID sequence 
	for (int i = 0; i < totalCount; i++) {
		for (int j = i + 1; j < totalCount; j++) {
			if ((facility[i].ID[0] == facility[j].ID[0]) && (facility[i].ID[1] == facility[j].ID[1]) && (extractCurrentIDNo(facility[i].ID) > extractCurrentIDNo(facility[j].ID))) {
				tempFaci = facility[i];
				facility[i] = facility[j];
				facility[j] = tempFaci;
			}
		}
	}


	for (int i = 0; i < totalCount; i++) {
		printf("%s\n", facility[i].ID);
	}
	/*
	Facility facility[5];
	strcpy(facility[0].ID, "BT11");
	strcpy(facility[1].ID, "BT10");
	strcpy(facility[2].ID, "BT2");
	strcpy(facility[3].ID, "BT1");
	strcpy(facility[4].ID, "BT21");

	qsort(facility, 5, sizeof(Facility), compareID);

	for (int i = 0; i < 5; i++) {
		printf("%s", facility[i].ID);
	}*/

	/*
	char a = '1';
	char b = '0';
	char test[3] = { a,b };
	printf("%s", test);
	int testing = atoi(test);
	printf("%d", testing);*/
	//char test[4] = "BT1";
	//printf("%zu",strlen(test));
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