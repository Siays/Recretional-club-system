#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#pragma warning(disable:4996)

struct PW {
	char password[21];
	int pwResetPin;
};

typedef struct {
	char ID[7];
	char name[31];
	struct PW pw;
	char position[21];
	char email[31];
	char status[9];
	int annualLeaveCount;
}Staff;

typedef struct {
	char ID[7];
	int sDay, sMonth, sYear;
	int totalDay;
	int eDay, eMonth, eYear;
	char reason[51];
	char leaveStatus[9];
}Leave;

void dateValidation(int currentDay, int currentMonth, int currentYear, int* checkValidation);
void checkALCount(char ID[], int totalDay, int* checkValidation);
void updateLeaveEndDate(int totalDay, int startDay, int startMonth, int startYear, int* endDay, int* endMonth, int* endYear);
void staffIDValidation(char userInput[], int* checkValidation);
void red();
void yellow();
void green();
void resetColor();


void main() {
	/*
	Staff staff, editedStaff;
	FILE* readStaff;
	FILE* modifyStaff;
	readStaff = fopen("staff.txt","r");
	modifyStaff = fopen("temp_staff.txt", "w");


	char targetID[7];
	printf("Enter staff ID to modify: ");
	rewind(stdin);
	gets(targetID);
	while (!feof(readStaff)) {
		fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
		editedStaff = staff;
		if (strcmp(editedStaff.ID, targetID) == 0) {
			printf("Record found\n");
			printf("ID: %s\nName: %-31s\nPosition: %-21s\nEmail: %-31s\nStatus: %s\n\n", editedStaff.ID, editedStaff.name, editedStaff.position, editedStaff.email, editedStaff.status);
			char newName[31];
			printf("Enter new staff name: ");
			rewind(stdin);
			gets(newName);
			strcpy(editedStaff.name, newName);
		}
		fprintf(modifyStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", editedStaff.ID, editedStaff.name, editedStaff.pw.password, editedStaff.pw.pwResetPin, editedStaff.position, editedStaff.email, editedStaff.status, editedStaff.annualLeaveCount);
	}
	system("pause");

	fclose(readStaff);
	fclose(modifyStaff);

	remove("staff.txt");
	rename("temp_staff.txt", "staff.txt");*/
	Leave leave;
	int checkValidation;

	do {
		checkValidation = 0;
		printf("Enter the staff id who is applying for leave: ");
		rewind(stdin);
		fgets(leave.ID, 7, stdin);
		staffIDValidation(leave.ID, &checkValidation);
	} while (checkValidation != 0);

	do {
		checkValidation = 0;
		printf("Enter leave date(numberic input): DD MM YYYY\n");
		rewind(stdin);
		scanf("%d %d %d", &leave.sDay, &leave.sMonth, &leave.sYear);
		dateValidation(leave.sDay, leave.sMonth, leave.sYear, &checkValidation);
	} while (checkValidation != 0);

	do {
		checkValidation = 0;
		printf("How many day(s) of leave: ");
		rewind(stdin);
		scanf("%d", &leave.totalDay);
		checkALCount(leave.ID, leave.totalDay, &checkValidation);
	} while (checkValidation != 0);

	updateLeaveEndDate(leave.totalDay, leave.sDay, leave.sMonth, leave.sYear, &leave.eDay, &leave.eMonth, &leave.eYear);
	printf("Stated the reason in 1 line (maximum 50 words, words come after length of 50 will be truncated.)\n");
	fgets(leave.reason, 51, stdin);
	for (int i = 0; i < strlen(leave.reason); i++) {
		if (leave.reason[i] == '\n') {
			leave.reason[i] = '\0';
		}
	}

	red();
	printf("Leave apply status will be auto assigned to pending until review by person in charged.\n");
	printf("DISCLAIMER: pending status doesn't mean approved, thus staff shouldn't absent from work unless\n");
	printf("the leave is confirmed to be approved.");
	resetColor();
	strcpy(leave.leaveStatus, "Pending");

	//confirm apply leave?
	char confirm;

}

void dateValidation(int currentDay, int currentMonth, int currentYear, int* checkValidation) {
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysYear = time.tm_year + 1900;
	int sysDay = time.tm_mday;
	if (currentYear < sysYear) {
		yellow();
		printf("Illogical input of year\n");
		resetColor();
		++* checkValidation;
		return;
	}

	if (currentYear > sysYear) {
		yellow();
		printf("Able to apply leave for the current year only\n");
		resetColor();
		++* checkValidation;
		return;
	}

	if (currentYear == sysYear && currentMonth < sysMonth) {
		yellow();
		printf("Illogical input of month\n");
		resetColor();
		++* checkValidation;
		return;
	}

	if (!(currentMonth >= 1 || currentMonth <= 12)) {
		yellow();
		printf("Illogical input of month\n");
		resetColor();
		++* checkValidation;
		return;
	}

	if ((currentYear == sysYear && currentMonth == sysMonth) && currentDay < sysDay) {
		yellow();
		printf("Illogical input of day\n");
		resetColor();
		++* checkValidation;
		return;
	}


	if (currentMonth == 1 || currentMonth == 3 || currentMonth == 5 || currentMonth == 7 || currentMonth == 8 || currentMonth == 10 || currentMonth == 12) {
		if (currentDay > 31) {
			++* checkValidation;
			yellow();
			printf("Illogical input, the last day of the month is 31\n");
			resetColor();
		}
		if (currentDay < 1) {
			++* checkValidation;
			yellow();
			printf("Illogical input of day\n");
			resetColor();
		}
	}
	if (currentMonth == 4 || currentMonth == 6 || currentMonth == 9 || currentMonth == 11) {
		if (currentDay > 30) {
			++* checkValidation;
			yellow();
			printf("Illogical input, the last day of the month is 30\n");
			resetColor();
		}
		if (currentDay < 1) {
			++* checkValidation;
			yellow();
			printf("Illogical input of day\n");
			resetColor();
		}
	}

	if (currentMonth == 2) {
		if (((currentYear % 4 == 0) && ((currentYear % 400 == 0) || (currentYear % 100 != 0)))) {
			if (currentDay > 29) {
				++* checkValidation;
				yellow();
				printf("Illogical input, the last day of the month is 29\n");
				resetColor();
			}
			if (currentDay < 1) {
				++* checkValidation;
				yellow();
				printf("Illogical input of day\n");
				resetColor();
			}
		}
		else {
			if (currentDay > 28) {
				++* checkValidation;
				yellow();
				printf("Illogical input, the last day of the month is 28\n");
				resetColor();
			}
			if (currentDay < 1) {
				++* checkValidation;
				yellow();
				printf("Illogical input of day\n");
				resetColor();
			}
		}

	}
}

void checkALCount(char ID[], int totalDay, int* checkValidation) {
	FILE* readStaff;
	Staff staff;
	readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	while (!feof(readStaff)) {
		fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
		if (strcmp(staff.ID, ID) == 0) {
			if (staff.annualLeaveCount < totalDay) {
				if (staff.annualLeaveCount > 0) {
					++* checkValidation;
					yellow();
					printf("\nInsufficient annual leave count to apply the leave.\n");
					printf("Current annual leave count: %d\n", staff.annualLeaveCount);
					printf("Days of leave trying to apply: %d\n", totalDay);
					resetColor();
					break;
				}
				else {
					*checkValidation = -1;
					yellow();
					printf("Current annual leave count: %d\n", staff.annualLeaveCount);
					printf("The following staff is not eligible to allow anymore leave.\n");
					resetColor();
					break;
				}
			}
			else
				continue;
		}
	}
	fclose(readStaff);
}

void updateLeaveEndDate(int totalDay, int startDay, int startMonth, int startYear, int* endDay, int* endMonth, int* endYear) {
	*endDay = startDay + totalDay;
	*endMonth = startMonth;
	*endYear = startYear;
	if (startMonth == 1 || startMonth == 3 || startMonth == 5 || startMonth == 7 || startMonth == 8 || startMonth == 10 || startMonth == 12) {
		if (*endDay > 31) {
			*endDay = *endDay - 31;
			++* endMonth;
		}

	}
	if (startMonth == 4 || startMonth == 6 || startMonth == 9 || startMonth == 11) {
		if (*endDay > 30) {
			*endDay = *endDay - 30;
			++* endMonth;
		}

	}

	if (startMonth == 2) {
		if (((startYear % 4 == 0) && ((startYear % 400 == 0) || (startYear % 100 != 0)))) {
			if (*endDay > 29) {
				*endDay = *endDay - 29;
				++* endMonth;
			}

		}
		else {
			if (*endDay > 28) {
				*endDay = *endDay - 28;
				++* endMonth;
			}
		}
	}

	if (*endMonth > 12) {
		*endMonth = *endMonth - 12;
		++* endYear;
	}
}
void staffIDValidation(char userInput[], int* checkValidation) {
	for (int i = 0; i < 2; i++) {
		if (userInput[0] == 's' || userInput[1] == 't') {
			userInput[0] = toupper(userInput[0]);
			userInput[1] = toupper(userInput[1]);
		}

		else if (userInput[0] != 'S' || userInput[1] != 'T') {
			++* checkValidation;
			yellow();
			printf("Expect ID start with 'ST'\a\n");
			resetColor();
			system("pause");
			break;
		}
		else
			break;
	}


}

void red() {
	printf("\033[0;31m");

}

void yellow() {

	printf("\033[0;33m");
}

void green() {
	printf("\033[0;32m");
}

void resetColor() {
	printf("\033[0m");
}