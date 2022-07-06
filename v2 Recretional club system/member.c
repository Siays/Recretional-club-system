#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#pragma warning(disable: 4996)
typedef struct {
	int day;
	int month;
	int year;
} Date;

typedef struct {
	char ID[8];
	char name[41];
	char gender;
	char IC[15];
	char contactNo[13];
	int regMonths; //6 months , 12 months , 24 , 36 months
	Date startDate;
	Date endDate;
	char status[9]; //active / expired;
}Member;

typedef struct {
	char ID[8];
	char contactNo[13];
	char reason[51];
	int noOfOffend;
	char reportDate[26];
	char reportStatus[9]; //pending , denied , accepted
}BlackListMember;


//display
//search
//modify
//add
//blacklist << offend? reason? 3 times acc will force to change to expired, and if wish to reapply, additional charge
//report  display blacklist, display how many customer >1 year etc.
//updateMembershipstatus
void memberModuleMain(char loginID[]);
void addMember();
void displayMember();
void searchMemberMenu();
void searchMemberByID();
void searchMemberByIC();
void modifyMemberMenu();
void modifyMemberName();
void modifyMemberContact();
void modifyMemberMembershipDuration();
void reportMember();
void reviewReportedMember();
void renewMembership();
//helper functions
void autoNewMemID(char input[]);
void memIDValidation(char input[], int* checkValidation);
void addMemICValidation(char input[], char gender,int* checkValidation);
void checkDuplicateIC(char input[], int* checkValidation);
void memContactValidation(char input[], int* checkValidation);
void checkDuplicateContact(char input[], int* checkValidation);
void durationOption(int selection, int* duration);
void membershipEndDate(int duration, int sDay, int sMonth, int sYear, int* eDay, int* eMonth, int* eYear);
void memICValidation(char input[], int* checkValidation);
void updateMemStatus();

//modify , Name, contact, membership dur , 
void memberModuleMain(char loginID[]) {
	system("cls");
	Staff staff;
	FILE* readStaff;
	readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	while (!feof(readStaff)) {
		fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
		if (strcmp(staff.ID, loginID) == 0) {
			break;
		}
	}
	fclose(readStaff);


	int menuSelection;
	do {
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\Member Module\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit the Module\n");
		printf("\t\t\t1. Display All Member Details\n");
		printf("\t\t\t2. Search Member Details\n");
		printf("\t\t\t3. Modify Member Details\n");
		printf("\t\t\t4. Add New Member\n");
		printf("\t\t\t5. Renew Membership\n");
		printf("\t\t\t6. Report Member Misbehavior\n");
		printf("\t\t\t7. Review Reported Member\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\tYour selection(numeric input only): ");
		rewind(stdin);//incase user input non-numeric input. the system will go into infinite loop.
		scanf("%d", &menuSelection);

		switch (menuSelection) {
		case 0:
			printf("\n\t\t\tExit to main menu.\n");
			system("pause");
			break;

		case 1:
			displayMember();
			break;

		case 2:
			searchMemberMenu();
			break;

		case 3:
			if (strcmp(staff.position, "CR Manager") == 0 || strcmp(staff.position, "CR Personnel") == 0) {
				modifyMemberMenu();
			}
			else {
				red();
				printf("\t\t\tYou are not allowed to access this function.\a\n");
				resetColor();
				system("pause");
			}
			break;

		case 4:
			if (strcmp(staff.position, "Facility Manager") == 0 || strcmp(staff.position, "Facility Personnel") == 0 || strcmp(staff.position,"Customer Service")) {
				addMember();
			}
			else {
				red();
				printf("\t\t\tYou are not allowed to access this function.\a\n");
				resetColor();
				system("pause");
			}
			break;

		case 5:
			if (strcmp(staff.position, "CR Manager") == 0 || strcmp(staff.position,"CR Personnel") || strcmp(staff.position,"Customer Service")) {
				renewMembership();
			}
			else {
				red();
				printf("\t\t\tYou are not allowed to access this function.\a\n");
				resetColor();
				system("pause");
			}
			break;

		case 6:
			reportMember();
			break;

		case 7:
			if (strcmp(staff.position, "CR Manager") == 0 || strcmp(staff.position, "CR Personnel")) {
				reviewReportedMember();
			}
			else {
				red();
				printf("\t\t\tYou are not allowed to access this function.\a\n");
				resetColor();
				system("pause");
			}
			break;

		default:
			red();
			printf("\t\t\tInvalid selection!\a\n");
			resetColor();
			system("pause");
		}
	} while (menuSelection != 0);
}

void addMember() {
	Member mem;
	int checkValidation;
	
	do {
		system("cls");
		checkValidation = 0;
		autoNewMemID(mem.ID);
		memIDValidation(mem.ID,&checkValidation);
	} while (checkValidation != 0);

	printf("New Member ID: %s\n", mem.ID);
	
	printf("Enter new member full name: ");
	rewind(stdin);
	fgets(mem.name,41,stdin);
	deleteNewLine(mem.name);

	do {
		printf("\nEnter new member gender(M or F only): ");
		rewind(stdin);
		scanf("%c", &mem.gender);
		if (mem.gender != 'M' && mem.gender != 'F') {
			red();
			printf("Please enter 'M' or 'F' only, which is the gender recognized by Malaysia law\n");
			resetColor();
		}
	} while (mem.gender != 'M' && mem.gender != 'F');

	do {
		printf("\nEnter new member Identity Card ID(with '-'(dash)): ");
		checkValidation = 0;
		rewind(stdin);
		fgets(mem.IC, 15, stdin);
		deleteNewLine(mem.IC);
		addMemICValidation(mem.IC, mem.gender, &checkValidation);
		if (checkValidation == 0) {
			checkDuplicateIC(mem.IC, &checkValidation);
		}
	} while (checkValidation != 0);

	do {
		checkValidation = 0;
		printf("\nPersonal phone number with with '-'(dash) and without country code (+60)\n");
		printf("Enter '-' if member doesn't has a personal phone contact\n");
		printf("Enter new member contact: ");
		rewind(stdin);
		fgets(mem.contactNo,13,stdin);
		deleteNewLine(mem.contactNo);
		if(strlen(mem.contactNo) > 1){
			memContactValidation(mem.contactNo,&checkValidation);
			if (checkValidation == 0) {
				checkDuplicateContact(mem.contactNo, & checkValidation);
			}
		}
		if (strlen(mem.contactNo) == 1 && mem.contactNo == '-') {
			break;
		}
	} while (checkValidation != 0);

	int durationOpt;
	do {
		printf("\nMembership duration\n1. 6 months\n2. 12 months\n3. 18 months\n4. 24 months\n5. 36 months\n");
		printf("Enter the membership duration apply: ");
		rewind(stdin);
		scanf("%d", &durationOpt);
		if (!(durationOpt >= 1 && durationOpt <= 5)) {
			red();
			printf("\aPlease key in the number assigned to the duration of membership\n");
			resetColor();
		}
	} while (!(durationOpt >= 1 && durationOpt <=5));
	durationOption(durationOpt, &mem.regMonths);

	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysDay = time.tm_mday;
	int sysYear = time.tm_year + 1900;

	mem.startDate.day = sysDay;
	mem.startDate.month = sysMonth;
	mem.startDate.year = sysYear;

	membershipEndDate(mem.regMonths, mem.startDate.day,mem.startDate.month,mem.startDate.year, &mem.endDate.day,&mem.endDate.month,&mem.endDate.year);

	strcpy(mem.status,"Active");

	system("cls");
	printf("New member details\n");
	printf("Member ID: %s\nMember Name: %s\nMember Gender: %c\nMember IC: %s\nContact No: %s\nMembership duration applied: %d Months\nStart Date: %02d-%02d-%02d\nExpiry Date: %02d-%02d-%02d\nStatus: %s",mem.ID,mem.name,mem.gender,mem.IC,mem.contactNo,mem.regMonths,mem.startDate.day,mem.startDate.month,mem.startDate.year,mem.endDate.day,mem.endDate.month,mem.endDate.year,mem.status);
	char confirm;
	printf("\n\nConfirm to add the new member?\n");
	printf("Press 'Y' to add, other key to cancel\n");
	confirm = getch();
	confirm = toupper(confirm);

	if (confirm == 'Y') {
		FILE* appendMember = fopen("member.txt","a");
		if (appendMember == NULL) {
			red();
			printf("Failed to open file");
			resetColor();
			return;
		}
		fprintf(appendMember, "%s|%s|%c|%s|%s|%d|%d-%d-%d|%d-%d-%d|%s\n", mem.ID, mem.name, mem.gender, mem.IC, mem.contactNo, mem.regMonths, mem.startDate.day, mem.startDate.month, mem.startDate.year, mem.endDate.day, mem.endDate.month, mem.endDate.year, mem.status);
		fclose(appendMember);
		red();
		printf("\aNew member added\n");
		resetColor();
	}
	else {
		red();
		printf("\aFailed to add the new member\n");
		resetColor();
	}
	system("pause");
}
void displayMember() {
	system("cls");
	Member mem;
	FILE* readMem = fopen("member.txt", "r");
	if (readMem == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	printf("All member details\n");

	printf("Mem.ID\tName\t\t\t\tSex\tIC\t\tContact \tMem.Duration\tS.Date\t\tE.Date\t         Status\n");
	printf("-------------------------------------------------------------------------------------------------------------------------------------\n");
	while (!feof(readMem)) {
		fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
		printf("%-7s\t%-25s\t%3c\t%-14s\t%-12s\t%d Months\t%d-%d-%d\t%d-%d-%d\t%s\n", mem.ID, mem.name, mem.gender, mem.IC, mem.contactNo, mem.regMonths, mem.startDate.day, mem.startDate.month, mem.startDate.year, mem.endDate.day, mem.endDate.month, mem.endDate.year, mem.status);
	}
	printf("-------------------------------------------------------------------------------------------------------------------------------------\n");
	fclose(readMem);
	printf("\nPlease proceed to search function if you wish to see the specified member's full name if it is too long to be displayed.\n\n");
	system("pause");
}
void searchMemberMenu() {
	int menuSelection;
	do {
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\Search Member\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit search\n");
		printf("\t\t\t1. Search By ID\n");
		printf("\t\t\t2. Search By IC\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\tYour selection(numeric input only): ");
		rewind(stdin);//incase user input non-numeric input. the system will go into infinite loop.
		scanf("%d", &menuSelection);
		switch (menuSelection) {
		case 0:
			return;

		case 1:
			searchMemberByID();
			break;

		case 2:
			searchMemberByIC();
			break;

		default:
			red();
			printf("Invalid selection\a\n");
			resetColor();
			system("pause");
		}
	} while (menuSelection != 0);
}
void searchMemberByID() {
	int checkValidation;
	char ID[8];
	do {
		system("cls");
		checkValidation = 0;
		printf("Search member by ID\n");
		printf("Enter the member ID: ");
		rewind(stdin);
		fgets(ID, 8, stdin);
		deleteNewLine(ID);
		memIDValidation(ID, &checkValidation);
	} while (checkValidation != 0);
	
	Member mem;
	FILE* readMem = fopen("member.txt", "r");
	if (readMem == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	int recordFound = 0;
	printf("\n\n");
	while (!feof(readMem)) {
		fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
		if(strcmp(mem.ID, ID) == 0){
			++recordFound;
			printf("Member ID: %s\nMember Name: %s\nMember Gender: %c\nMember IC: %s\nContact No: %s\nMembership duration applied: %d Months\nStart Date: %02d-%02d-%02d\nExpiry Date: %02d-%02d-%02d\nStatus: %s\n\n", mem.ID, mem.name, mem.gender, mem.IC, mem.contactNo, mem.regMonths, mem.startDate.day, mem.startDate.month, mem.startDate.year, mem.endDate.day, mem.endDate.month, mem.endDate.year, mem.status);
			break;
		}
	}
	fclose(readMem);

	if (recordFound == 0) {
		red();
		printf("\aThe member ID doesn't exists.\n");
		resetColor();
	}

	system("pause");
}
void searchMemberByIC() {
	int checkValidation;
	char IC[15];
	do {
		system("cls");
		checkValidation = 0;
		printf("Search member by IC\n");
		printf("Enter the member IC: ");
		rewind(stdin);
		fgets(IC, 15, stdin);
		deleteNewLine(IC);
		memICValidation(IC, &checkValidation);
	} while (checkValidation != 0);

	Member mem;
	FILE* readMem = fopen("member.txt", "r");
	if (readMem == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	int recordFound = 0;
	printf("\n\n");
	while (!feof(readMem)) {
		fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
		if (strcmp(mem.IC, IC) == 0) {
			++recordFound;
			printf("Member ID: %s\nMember Name: %s\nMember Gender: %c\nMember IC: %s\nContact No: %s\nMembership duration applied: %d Months\nStart Date: %02d-%02d-%02d\nExpiry Date: %02d-%02d-%02d\nStatus: %s\n\n", mem.ID, mem.name, mem.gender, mem.IC, mem.contactNo, mem.regMonths, mem.startDate.day, mem.startDate.month, mem.startDate.year, mem.endDate.day, mem.endDate.month, mem.endDate.year, mem.status);
			break;
		}
	}
	fclose(readMem);

	if (recordFound == 0) {
		red();
		printf("\aThe member IC doesn't exists.\n");
		resetColor();
	}

	system("pause");
}
void modifyMemberMenu() {
	int menuSelection;
	do {
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\Modify Member\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit search\n");
		printf("\t\t\t1. Modify Member Name\n");
		printf("\t\t\t2. Modify Member Contact\n");
		printf("\t\t\t3. Modify Member Membership Duration\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\tYour selection(numeric input only): ");
		rewind(stdin);//incase user input non-numeric input. the system will go into infinite loop.
		scanf("%d", &menuSelection);
		switch (menuSelection) {
		case 0:
			return;

		case 1:
			modifyMemberName();
			break;

		case 2:
			modifyMemberContact();
			break;

		case 3:
			modifyMemberMembershipDuration();
			break;

		default:
			red();
			printf("Invalid selection\a\n");
			resetColor();
			system("pause");
		}
	} while (menuSelection != 0);
}
void modifyMemberName() {
	Member updateMem, mem;
	int opt, checkValidation, recordFound = 0;
	do{
		system("cls");
		printf("Modify Member Name\n");
		printf("0. Exit modification\n1. Specifize member using ID\n2. Specifize member using IC\n");
		printf("Your selection: ");
		rewind(stdin);
		scanf("%d", &opt);
		if (opt == 0) {
			return;
		}
		if (!(opt >= 1 && opt <= 2)) {
			red();
			printf("\aInvalid selection\n");
			resetColor();
			system("pause");
		}
	} while (!(opt>=1 && opt<= 2));

	if (opt == 1) {
		char ID[8];
		do{
			checkValidation = 0;
			printf("\nEnter the member ID you wish to modify: ");
			rewind(stdin);
			fgets(ID,8,stdin);
			deleteNewLine(ID);
			memIDValidation(ID, &checkValidation);
		} while (checkValidation != 0);

		FILE* readMem = fopen("member.txt","r");
		if (readMem == NULL) {
			red();
			printf("Failed to open file");
			resetColor();
			return;
		}
		while (!feof(readMem)) {
			fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", updateMem.ID, updateMem.name, &updateMem.gender, updateMem.IC, updateMem.contactNo, &updateMem.regMonths, &updateMem.startDate.day, &updateMem.startDate.month, &updateMem.startDate.year, &updateMem.endDate.day, &updateMem.endDate.month, &updateMem.endDate.year, updateMem.status);
			if (strcmp(updateMem.ID, ID) == 0) {
				++recordFound;
				break;
			}
				
		}
		fclose(readMem);

		if (recordFound == 0) {
			red();
			printf("\aThe ID you looking for doesn't exists\n");
			resetColor();
			system("pause");
			return;
		}
	}
	if (opt == 2) {
		char IC[15];
		do {
			checkValidation = 0;
			printf("\nEnter the member IC you wish to modify: ");
			rewind(stdin);
			fgets(IC, 15, stdin);
			deleteNewLine(IC);
			memICValidation(IC, &checkValidation);
		} while (checkValidation != 0);

		FILE* readMem = fopen("member.txt", "r");
		if (readMem == NULL) {
			red();
			printf("Failed to open file");
			resetColor();
			return;
		}
		while (!feof(readMem)) {
			fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", updateMem.ID, updateMem.name, &updateMem.gender, updateMem.IC, updateMem.contactNo, &updateMem.regMonths, &updateMem.startDate.day, &updateMem.startDate.month, &updateMem.startDate.year, &updateMem.endDate.day, &updateMem.endDate.month, &updateMem.endDate.year, updateMem.status);
			if (strcmp(updateMem.IC, IC) == 0) {
				++recordFound;
				break;
			}

		}
		fclose(readMem);
		if (recordFound == 0) {
			red();
			printf("\aThe IC you looking for doesn't exists\n");
			resetColor();
			system("pause");
			return;
		}
	}

	system("cls");
	printf("Member details\n");
	printf("Member ID: %s\nMember Name: %s\nMember Gender: %c\nMember IC: %s\nContact No: %s\nMembership duration applied: %d Months\nStart Date: %02d-%02d-%02d\nExpiry Date: %02d-%02d-%02d\nStatus: %s", updateMem.ID, updateMem.name, updateMem.gender, updateMem.IC, updateMem.contactNo, updateMem.regMonths, updateMem.startDate.day, updateMem.startDate.month, updateMem.startDate.year, updateMem.endDate.day, updateMem.endDate.month, updateMem.endDate.year, updateMem.status);
	
	char newName[41];
	
	printf("\n\nEnter new name [enter '0' to exit]: ");
	rewind(stdin);
	fgets(newName,41,stdin);
	deleteNewLine(newName);
	if (newName[0] == '0') {
		red();
		printf("\aModification cancelled\n");
		resetColor();
		system("pause");
		return;
	}
	
	printf("\n\nNew name: %s\n",newName);
	printf("Confirm to update the member name?\n");
	printf("Press 'Y' to continue, other key to cancel modifcation\n");
	char confirm;
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		strcpy(updateMem.name,newName);
		FILE* writeMem = fopen("tempMember.txt","w");
		FILE* readMem = fopen("member.txt","r");
		if (writeMem == NULL || readMem == NULL) {
			red();
			printf("Failed to open file");
			resetColor();
			return;
		}
		while (!feof(readMem)) {
			fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
			if (strcmp(mem.ID, updateMem.ID) == 0) {
				fprintf(writeMem, "%s|%s|%c|%s|%s|%d|%d-%d-%d|%d-%d-%d|%s\n", updateMem.ID, updateMem.name, updateMem.gender, updateMem.IC, updateMem.contactNo, updateMem.regMonths, updateMem.startDate.day, updateMem.startDate.month, updateMem.startDate.year, updateMem.endDate.day, updateMem.endDate.month, updateMem.endDate.year, updateMem.status);
			}
			else {
				fprintf(writeMem, "%s|%s|%c|%s|%s|%d|%d-%d-%d|%d-%d-%d|%s\n", mem.ID, mem.name, mem.gender, mem.IC, mem.contactNo, mem.regMonths, mem.startDate.day, mem.startDate.month, mem.startDate.year, mem.endDate.day, mem.endDate.month, mem.endDate.year, mem.status);
			}
		}
		fclose(readMem);
		fclose(writeMem);
		remove("member.txt");
		rename("tempMember.txt","member.txt");
		red();
		printf("\aModification succeed\n");
		resetColor();
	}
	else {
		red();
		printf("\aModification cancelled\n");
		resetColor();
	}

	system("pause");
}
void modifyMemberContact() {
	Member updateMem, mem;
	int opt, checkValidation, recordFound = 0;
	do {
		system("cls");
		printf("Modify Member Contact\n");
		printf("0. Exit modification\n1. Specifize member using ID\n2. Specifize member using IC\n");
		printf("Your selection: ");
		rewind(stdin);
		scanf("%d", &opt);
		if (opt == 0) {
			return;
		}
		if (!(opt >= 1 && opt <= 2)) {
			red();
			printf("\aInvalid selection\n");
			resetColor();
			system("pause");
		}
	} while (!(opt >= 1 && opt <= 2));

	if (opt == 1) {
		char ID[8];
		do {
			checkValidation = 0;
			printf("\nEnter the member ID you wish to modify: ");
			rewind(stdin);
			fgets(ID, 8, stdin);
			deleteNewLine(ID);
			memIDValidation(ID, &checkValidation);
		} while (checkValidation != 0);

		FILE* readMem = fopen("member.txt", "r");
		if (readMem == NULL) {
			red();
			printf("Failed to open file");
			resetColor();
			return;
		}
		while (!feof(readMem)) {
			fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", updateMem.ID, updateMem.name, &updateMem.gender, updateMem.IC, updateMem.contactNo, &updateMem.regMonths, &updateMem.startDate.day, &updateMem.startDate.month, &updateMem.startDate.year, &updateMem.endDate.day, &updateMem.endDate.month, &updateMem.endDate.year, updateMem.status);
			if (strcmp(updateMem.ID, ID) == 0) {
				++recordFound;
				break;
			}

		}
		fclose(readMem);

		if (recordFound == 0) {
			red();
			printf("\aThe ID you looking for doesn't exists\n");
			resetColor();
			system("pause");
			return;
		}
	}
	if (opt == 2) {
		char IC[15];
		do {
			checkValidation = 0;
			printf("\nEnter the member IC you wish to modify: ");
			rewind(stdin);
			fgets(IC, 15, stdin);
			deleteNewLine(IC);
			memICValidation(IC, &checkValidation);
		} while (checkValidation != 0);

		FILE* readMem = fopen("member.txt", "r");
		if (readMem == NULL) {
			red();
			printf("Failed to open file");
			resetColor();
			return;
		}
		while (!feof(readMem)) {
			fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", updateMem.ID, updateMem.name, &updateMem.gender, updateMem.IC, updateMem.contactNo, &updateMem.regMonths, &updateMem.startDate.day, &updateMem.startDate.month, &updateMem.startDate.year, &updateMem.endDate.day, &updateMem.endDate.month, &updateMem.endDate.year, updateMem.status);
			if (strcmp(updateMem.IC, IC) == 0) {
				++recordFound;
				break;
			}

		}
		fclose(readMem);
		if (recordFound == 0) {
			red();
			printf("\aThe IC you looking for doesn't exists\n");
			resetColor();
			system("pause");
			return;
		}
	}

	system("cls");
	printf("Member details\n");
	printf("Member ID: %s\nMember Name: %s\nMember Gender: %c\nMember IC: %s\nContact No: %s\nMembership duration applied: %d Months\nStart Date: %02d-%02d-%02d\nExpiry Date: %02d-%02d-%02d\nStatus: %s", updateMem.ID, updateMem.name, updateMem.gender, updateMem.IC, updateMem.contactNo, updateMem.regMonths, updateMem.startDate.day, updateMem.startDate.month, updateMem.startDate.year, updateMem.endDate.day, updateMem.endDate.month, updateMem.endDate.year, updateMem.status);

	char newContact[13];
	do{
		checkValidation = 0;
		printf("\n\nEnter new contact [enter '-1' to exit]: ");
		rewind(stdin);
		fgets(newContact, 13, stdin);
		deleteNewLine(newContact);
		if (strcmp(newContact,"-1") == 0) {
			red();
			printf("\aModification cancelled\n");
			resetColor();
			system("pause");
			return;
		}
		memContactValidation(newContact, &checkValidation);
		if (checkValidation == 0) {
			checkDuplicateContact(newContact,&checkValidation);
		}
	} while (checkValidation != 0);
	printf("\n\nNew contact: %s\n", newContact);
	printf("Confirm to update the member contact?\n");
	printf("Press 'Y' to continue, other key to cancel modifcation\n");
	char confirm;
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		strcpy(updateMem.contactNo, newContact);
		FILE* writeMem = fopen("tempMember.txt", "w");
		FILE* readMem = fopen("member.txt", "r");
		if (writeMem == NULL || readMem == NULL) {
			red();
			printf("Failed to open file");
			resetColor();
			return;
		}
		while (!feof(readMem)) {
			fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
			if (strcmp(mem.ID, updateMem.ID) == 0) {
				fprintf(writeMem, "%s|%s|%c|%s|%s|%d|%d-%d-%d|%d-%d-%d|%s\n", updateMem.ID, updateMem.name, updateMem.gender, updateMem.IC, updateMem.contactNo, updateMem.regMonths, updateMem.startDate.day, updateMem.startDate.month, updateMem.startDate.year, updateMem.endDate.day, updateMem.endDate.month, updateMem.endDate.year, updateMem.status);
			}
			else {
				fprintf(writeMem, "%s|%s|%c|%s|%s|%d|%d-%d-%d|%d-%d-%d|%s\n", mem.ID, mem.name, mem.gender, mem.IC, mem.contactNo, mem.regMonths, mem.startDate.day, mem.startDate.month, mem.startDate.year, mem.endDate.day, mem.endDate.month, mem.endDate.year, mem.status);
			}
		}
		fclose(readMem);
		fclose(writeMem);
		remove("member.txt");
		rename("tempMember.txt", "member.txt");
		red();
		printf("\aModification succeed\n");
		resetColor();
	}
	else {
		red();
		printf("\aModification cancelled\n");
		resetColor();
	}

	system("pause");
}
void modifyMemberMembershipDuration() {
	Member updateMem, mem;
	int opt, checkValidation, recordFound = 0;
	do {
		system("cls");
		printf("Modify Member Membership Duration\n");
		printf("0. Exit modification\n1. Specifize member using ID\n2. Specifize member using IC\n");
		printf("Your selection: ");
		rewind(stdin);
		scanf("%d", &opt);
		if (opt == 0) {
			return;
		}
		if (!(opt >= 1 && opt <= 2)) {
			red();
			printf("\aInvalid selection\n");
			resetColor();
			system("pause");
		}
	} while (!(opt >= 1 && opt <= 2));

	if (opt == 1) {
		char ID[8];
		do {
			checkValidation = 0;
			printf("\nEnter the member ID you wish to modify: ");
			rewind(stdin);
			fgets(ID, 8, stdin);
			deleteNewLine(ID);
			memIDValidation(ID, &checkValidation);
		} while (checkValidation != 0);

		FILE* readMem = fopen("member.txt", "r");
		if (readMem == NULL) {
			red();
			printf("Failed to open file");
			resetColor();
			return;
		}
		while (!feof(readMem)) {
			fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", updateMem.ID, updateMem.name, &updateMem.gender, updateMem.IC, updateMem.contactNo, &updateMem.regMonths, &updateMem.startDate.day, &updateMem.startDate.month, &updateMem.startDate.year, &updateMem.endDate.day, &updateMem.endDate.month, &updateMem.endDate.year, updateMem.status);
			if (strcmp(updateMem.ID, ID) == 0) {
				++recordFound;
				break;
			}

		}
		fclose(readMem);

		if (recordFound == 0) {
			red();
			printf("\aThe ID you looking for doesn't exists\n");
			resetColor();
			system("pause");
			return;
		}
	}
	if (opt == 2) {
		char IC[15];
		do {
			checkValidation = 0;
			printf("\nEnter the member IC you wish to modify: ");
			rewind(stdin);
			fgets(IC, 15, stdin);
			deleteNewLine(IC);
			memICValidation(IC, &checkValidation);
		} while (checkValidation != 0);

		FILE* readMem = fopen("member.txt", "r");
		if (readMem == NULL) {
			red();
			printf("Failed to open file");
			resetColor();
			return;
		}
		while (!feof(readMem)) {
			fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", updateMem.ID, updateMem.name, &updateMem.gender, updateMem.IC, updateMem.contactNo, &updateMem.regMonths, &updateMem.startDate.day, &updateMem.startDate.month, &updateMem.startDate.year, &updateMem.endDate.day, &updateMem.endDate.month, &updateMem.endDate.year, updateMem.status);
			if (strcmp(updateMem.IC, IC) == 0) {
				++recordFound;
				break;
			}

		}
		fclose(readMem);
		if (recordFound == 0) {
			red();
			printf("\aThe IC you looking for doesn't exists\n");
			resetColor();
			system("pause");
			return;
		}
	}

	system("cls");
	printf("Member details\n");
	printf("Member ID: %s\nMember Name: %s\nMember Gender: %c\nMember IC: %s\nContact No: %s\nMembership duration applied: %d Months\nStart Date: %02d-%02d-%02d\nExpiry Date: %02d-%02d-%02d\nStatus: %s", updateMem.ID, updateMem.name, updateMem.gender, updateMem.IC, updateMem.contactNo, updateMem.regMonths, updateMem.startDate.day, updateMem.startDate.month, updateMem.startDate.year, updateMem.endDate.day, updateMem.endDate.month, updateMem.endDate.year, updateMem.status);

	int newDuration;
	do {
		printf("\n\nMembership duration\n1. 6 months\n2. 12 months\n3. 18 months\n4. 24 months\n5. 36 months\n");
		printf("Enter the membership duration apply: ");
		rewind(stdin);
		scanf("%d", &newDuration);
		if (!(newDuration >= 1 && newDuration <= 5)) {
			red();
			printf("\aPlease key in the number assigned to the duration of membership\n");
			resetColor();
		}
	} while (!(newDuration >= 1 && newDuration <= 5));
	printf("\n\nNew Duration: %d months\n", newDuration);
	printf("Confirm to update the member membership duration?\n");
	printf("Press 'Y' to continue, other key to cancel modifcation\n");
	char confirm;
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		durationOption(newDuration, &updateMem.regMonths);
		membershipEndDate(updateMem.regMonths, updateMem.startDate.day, updateMem.startDate.month, updateMem.startDate.year, &updateMem.endDate.day, &updateMem.endDate.month, &updateMem.endDate.year);
		FILE* writeMem = fopen("tempMember.txt", "w");
		FILE* readMem = fopen("member.txt", "r");
		if (writeMem == NULL || readMem == NULL) {
			red();
			printf("Failed to open file");
			resetColor();
			return;
		}
		while (!feof(readMem)) {
			fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
			if (strcmp(mem.ID, updateMem.ID) == 0) {
				fprintf(writeMem, "%s|%s|%c|%s|%s|%d|%d-%d-%d|%d-%d-%d|%s\n", updateMem.ID, updateMem.name, updateMem.gender, updateMem.IC, updateMem.contactNo, updateMem.regMonths, updateMem.startDate.day, updateMem.startDate.month, updateMem.startDate.year, updateMem.endDate.day, updateMem.endDate.month, updateMem.endDate.year, updateMem.status);
			}
			else {
				fprintf(writeMem, "%s|%s|%c|%s|%s|%d|%d-%d-%d|%d-%d-%d|%s\n", mem.ID, mem.name, mem.gender, mem.IC, mem.contactNo, mem.regMonths, mem.startDate.day, mem.startDate.month, mem.startDate.year, mem.endDate.day, mem.endDate.month, mem.endDate.year, mem.status);
			}
		}
		fclose(readMem);
		fclose(writeMem);
		remove("member.txt");
		rename("tempMember.txt", "member.txt");
		red();
		printf("\aModification succeed\n");
		resetColor();
	}
	else {
		red();
		printf("\aModification cancelled\n");
		resetColor();
	}

	system("pause");
}
void renewMembership() {
	int checkValidation;
	char ID[8];
	do{
		system("cls");
		checkValidation = 0;
		printf("Renew Membership\n");
		printf("Enter the member ID who wish to renew membership: ");
		rewind(stdin);
		fgets(ID,8,stdin);
		deleteNewLine(ID);
		memIDValidation(ID, &checkValidation);
	} while (checkValidation != 0);

	int recordFound = 0;
	Member mem,renewMem;
	FILE* readMem = fopen("member.txt","r");
	if (readMem == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	while (!feof(readMem)) {
		fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", renewMem.ID, renewMem.name, &renewMem.gender, renewMem.IC, renewMem.contactNo, &renewMem.regMonths, &renewMem.startDate.day, &renewMem.startDate.month, &renewMem.startDate.year, &renewMem.endDate.day, &renewMem.endDate.month, &renewMem.endDate.year, renewMem.status);
		if (strcmp(renewMem.ID, ID) == 0) {
			printf("\nMember ID: %s\nMember Name: %s\nMember Gender: %c\nMember IC: %s\nContact No: %s\nMembership duration applied: %d Months \nStart Date: %02d-%02d-%02d\nExpiry Date: %02d-%02d-%02d\nStatus: %s\n", renewMem.ID, renewMem.name, renewMem.gender, renewMem.IC, renewMem.contactNo, renewMem.regMonths, renewMem.startDate.day, renewMem.startDate.month, renewMem.startDate.year, renewMem.endDate.day, renewMem.endDate.month, renewMem.endDate.year, renewMem.status);
			if(strcmp(renewMem.status,"Expired") == 0) {
				++recordFound;
				break;
			}
			if (strcmp(renewMem.status, "BLed") == 0) {
				++recordFound;
				red();
				printf("\aBlacklisted member detected\n");
				resetColor();
				printf("Please inform the member different rate will be charged upon their\n");
				printf("member renewal due to their previous records\n");
				system("pause");
				break;
			}
			if (strcmp(renewMem.status,"Active") == 0) {
				red();
				printf("\aThe member's membership hasn't expired yet\n");
				resetColor();
				system("pause");
				fclose(readMem);
				return;
			}
		}
	}
	fclose(readMem);
	if (recordFound == 0) {
		red();
		printf("\aThe member ID you looking for doesn't exists\n");
		resetColor();
		system("pause");
		return;
	}

	int durationOpt;
	do {
		printf("\nMembership duration\n1. 6 months\n2. 12 months\n3. 18 months\n4. 24 months\n5. 36 months\n");
		printf("Enter the membership duration apply: ");
		rewind(stdin);
		scanf("%d", &durationOpt);
		if (!(durationOpt >= 1 && durationOpt <= 5)) {
			red();
			printf("\aPlease key in the number assigned to the duration of membership\n");
			resetColor();
		}
	} while (!(durationOpt >= 1 && durationOpt <= 5));
	durationOption(durationOpt, &renewMem.regMonths);

	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysDay = time.tm_mday;
	int sysYear = time.tm_year + 1900;

	renewMem.startDate.day = sysDay;
	renewMem.startDate.month = sysMonth;
	renewMem.startDate.year = sysYear;

	membershipEndDate(renewMem.regMonths, renewMem.startDate.day, renewMem.startDate.month, renewMem.startDate.year, &renewMem.endDate.day, &renewMem.endDate.month, &renewMem.endDate.year);
	if (strcmp(renewMem.status, "BLed") == 0) {
		red();
		printf("Please info the member that, if they continously accumulated another 2 times of misbehaviour\n");
		printf("Their membership will be cancelled(without refund) and will be banned by the club.\n");
		resetColor();
		system("pause");
	}
	strcpy(renewMem.status, "Active");

	system("cls");
	printf("Member details\n");
	printf("Member ID: %s\nMember Name: %s\nMember Gender: %c\nMember IC: %s\nContact No: %s\nMembership duration applied: %d Months\nStart Date: %02d-%02d-%02d\nExpiry Date: %02d-%02d-%02d\nStatus: %s", renewMem.ID, renewMem.name, renewMem.gender, renewMem.IC, renewMem.contactNo, renewMem.regMonths, renewMem.startDate.day, renewMem.startDate.month, renewMem.startDate.year, renewMem.endDate.day, renewMem.endDate.month, renewMem.endDate.year, renewMem.status);
	char confirm;
	printf("\n\nConfirm to renew the member?\n");
	printf("Press 'Y' to renew, other key to cancel\n");
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm != 'Y') {
		red();
		printf("\aMembership renewal cancelled\n");
		resetColor();
		system("pause");
		return;
	}

	readMem = fopen("member.txt", "r");
	FILE *writeMem = fopen("tempMember.txt","w");
	if (readMem == NULL || writeMem == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	while (!feof(readMem)) {
		fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
		if (strcmp(mem.ID, renewMem.ID) == 0) {
			fprintf(writeMem, "%s|%s|%c|%s|%s|%d|%d-%d-%d|%d-%d-%d|%s\n", renewMem.ID, renewMem.name, renewMem.gender, renewMem.IC, renewMem.contactNo, renewMem.regMonths, mem.startDate.day, renewMem.startDate.month, renewMem.startDate.year, renewMem.endDate.day, renewMem.endDate.month, renewMem.endDate.year, renewMem.status);
		}
		else {
			fprintf(writeMem, "%s|%s|%c|%s|%s|%d|%d-%d-%d|%d-%d-%d|%s\n", mem.ID, mem.name, mem.gender, mem.IC, mem.contactNo, mem.regMonths, mem.startDate.day, mem.startDate.month, mem.startDate.year, mem.endDate.day, mem.endDate.month, mem.endDate.year, mem.status);
		}
	}
	fclose(readMem);
	fclose(writeMem);
	remove("member.txt");
	rename("tempMember.txt", "member.txt");
	red();
	printf("\Membership renewal succeed\n");
	resetColor();

	system("pause");
}
void reportMember() {	
	Member mem;
	BlackListMember reportMem, matchRec;
	char ID[8];
	int checkValidation, recordFound = 0;
	do{
		system("cls");
		checkValidation = 0;
		printf("Report member misbehavior\n");
		printf("Enter the member you wish to report: ");
		rewind(stdin);
		fgets(ID,8,stdin);
		deleteNewLine(ID);
		memIDValidation(ID, &checkValidation);
	} while (checkValidation != 0);

	FILE* readMem = fopen("member.txt", "r");
	if (readMem == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	while (!feof(readMem)) {
		fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
		if (strcmp(mem.ID, ID) == 0) {
			++recordFound;
			break;
		}
	}
	fclose(readMem);
	if (recordFound == 0) {
		red();
		printf("\aThe ID you looking for doesn't exists\n");
		resetColor();
		system("pause");
		return;
	}
	
	int offendCount = 0;
	FILE* readResRec = fopen("memberReportedRes.txt","r");
	if (readResRec == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	while (!feof(readResRec)) {
		fscanf(readResRec, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^\n]\n", matchRec.ID, matchRec.contactNo, matchRec.reason, &matchRec.noOfOffend, matchRec.reportDate, matchRec.reportStatus);
		if((strcmp(matchRec.ID, mem.ID) == 0) && strcmp(matchRec.reportStatus,"Accepted")==0){
			++offendCount;
		}
	}
	fclose(readResRec);

	system("cls");
	printf("Member details\n");
	printf("Member ID: %s\nMember Name: %s\nMember Gender: %c\nMember IC: %s\nContact No: %s\nMembership duration applied: %d Months\nStart Date: %02d-%02d-%02d\nExpiry Date: %02d-%02d-%02d\nStatus: %s", mem.ID, mem.name, mem.gender, mem.IC, mem.contactNo, mem.regMonths, mem.startDate.day, mem.startDate.month, mem.startDate.year, mem.endDate.day, mem.endDate.month, mem.endDate.year, mem.status);

	printf("\n\nReport the member above?\n");
	printf("Press 'Y' to continue, other key to exit\n");
	char confirm;
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm != 'Y') {
		red();
		printf("\aReport cancelled\n");
		resetColor();
		system("pause");
		return;
	}

	strcpy(reportMem.ID, mem.ID);
	strcpy(reportMem.contactNo, mem.contactNo);
	strcpy(reportMem.reportStatus, "Pending");
	reportMem.noOfOffend = offendCount;//change through record, if ID match && status == accpeted

	time_t t;
	time(&t);
	strcpy(reportMem.reportDate, ctime(&t));
	deleteNewLine(reportMem.reportDate);

	printf("Reason of report [Max 50 input]: ");
	rewind(stdin);
	fgets(reportMem.reason, 51, stdin);
	deleteNewLine(reportMem.reason);

	system("cls");
	printf("Report details\n");
	printf("Member ID: %s\nContact: %s\nReason: %s\nCurrent offend: %d\nReport Date: %s\nReport Status: %s\n", reportMem.ID,reportMem.contactNo,reportMem.reason,reportMem.noOfOffend,reportMem.reportDate,reportMem.reportStatus);
	printf("\n\nConfirm to submit the report?\n");
	printf("Press 'Y' to submit, other key to exit\n");
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm != 'Y') {
		red();
		printf("\aReport cancelled\n");
		resetColor();
		system("pause");
		return;
	}

	FILE* appendRec = fopen("memberReported.txt","a");
	if (appendRec == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	fprintf(appendRec, "%s|%s|%s|%d|%s|%s\n", reportMem.ID, reportMem.contactNo, reportMem.reason, reportMem.noOfOffend, reportMem.reportDate, reportMem.reportStatus);
	fclose(appendRec);

	red();
	printf("\n\aReport submitted\n");
	resetColor();
	system("pause");
}
void reviewReportedMember() {
	BlackListMember *mem = malloc(50 *sizeof *mem);
	BlackListMember* matchID = malloc(10 * sizeof * matchID);
	BlackListMember responseMem;
	FILE* readReport = fopen("memberReported.txt","r");
	int size;
	if (readReport == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		free(mem);
		free(matchID);
	}
	else {
		fseek(readReport, 0, SEEK_END);
		size = ftell(readReport);
		fclose(readReport);
	}
	if (size == 0) {
		red();
		printf("\aNo report currently\n");
		resetColor();
		free(matchID);
		free(mem);
		system("pause");
		return;
	}

	readReport = fopen("memberReported.txt", "r");
	int totalReportCount = 0;
	while (!feof(readReport)) {
		fscanf(readReport, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^\n]\n", mem[totalReportCount].ID, mem[totalReportCount].contactNo, mem[totalReportCount].reason,&mem[totalReportCount].noOfOffend, mem[totalReportCount].reportDate, mem[totalReportCount].reportStatus);
		++totalReportCount;
	}
	fclose(readReport);
	for(int i=0;i<totalReportCount;i++){
		printf("ID: %s\nContact: %s\nReason: %s\nCurrent Offend Count: %d\nDate Reported: %s\nReport Status: %s\n\n", mem[i].ID, mem[i].contactNo, mem[i].reason, mem[i].noOfOffend, mem[i].reportDate, mem[i].reportStatus);
	}
	
	int checkValidation, matchIDCount = 0;
	char memID[8];
	do {
		checkValidation = 0;
		printf("\n\nEnter the member ID to response [Enter '0' to exit]: ");
		rewind(stdin);
		fgets(memID,8,stdin);
		deleteNewLine(memID);
		if (memID[0] == '0') {
			red();
			printf("\aExitted\n");
			resetColor();
			free(mem);
			free(matchID);
			return;
		}

		memIDValidation(memID, &checkValidation);
		if (checkValidation == 0) {
			for (int i = 0; i < totalReportCount; i++) {
				if (strcmp(mem[i].ID, memID) == 0) {
					matchID[matchIDCount] = mem[i];
					++matchIDCount;
				}
			}
		
			if (matchIDCount == 0) {
			red();
			printf("\aThe ID you looking for isn't in the report record\n");
			resetColor();
			++checkValidation;
		}
		}
	} while (checkValidation != 0);


	int noOfRecord;
	do {
	system("cls");
	printf("Record Match\n");
	for (int i = 0; i < matchIDCount; i++) {
		printf("Record Number: %d\n",i+1);
		printf("ID: %s\nContact: %s\nReason: %s\nCurrent Offend Count: %d\nDate Reported: %s\nReport Status: %s\n\n",matchID[i].ID, matchID[i].contactNo,matchID[i].reason,matchID[i].noOfOffend,matchID[i].reportDate,matchID[i].reportStatus);
	}
	printf("Enter the record number to response[0 to exit]: ");
	rewind(stdin);
	scanf("%d", &noOfRecord);
	if (noOfRecord == 0){
		red();
		printf("\aExitted\n");
		resetColor();
		system("pause");
		free(mem);
		free(matchID);
		return;
	}
	if (!(noOfRecord >= 1 && noOfRecord <= matchIDCount)) {
		red();
		printf("\aInvalid selection\n");
		resetColor();
		system("pause");
	}
	} while (!(noOfRecord>=1 && noOfRecord <= matchIDCount));


	for (int i = 0; i < matchIDCount; i++) {
		if (i == noOfRecord - 1) {
			responseMem = matchID[i];
			break;
		}
	}


	int updateStatusOpt;
	do{
	printf("Update response\n");
	printf("1. Accepted\n2. Denied\n");
	printf("Enter the response[0 to exit]: ");
	scanf("%d",&updateStatusOpt);
	if (updateStatusOpt == 0) {
		red();
		printf("\aExitted\n");
		resetColor();
		system("pause");
		free(mem);
		free(matchID);
		return;
	}
	} while (!(updateStatusOpt >= 1 && updateStatusOpt <= 2));

	if (updateStatusOpt == 1) {
		strcpy(responseMem.reportStatus,"Accepted");
		++responseMem.noOfOffend;
	}
	else {
		strcpy(responseMem.reportStatus, "Denied");
	}

	printf("\nUpdated status: %s\n", responseMem.reportStatus);
	printf("Confirm to response the report?\n");
	printf("Press 'Y' to confirm, other key to exit\n");
	char confirm;
	confirm = getch();
	confirm = toupper(confirm);	
	if (confirm != 'Y') {
		red();
		printf("\aResponse made cancelled\n");
		resetColor();
		system("pause");
		free(matchID);
		free(mem);
		return;
	}

	FILE* writeReport = fopen("memberReported.txt","w");
	FILE* appendResponse = fopen("memberReportedRes.txt","a");
	if (writeReport == NULL || appendResponse == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		free(matchID);
		free(mem);
		return;
	}
	for (int i = 0; i < totalReportCount; i++) {
		if ((strcmp(mem[i].ID, responseMem.ID) == 0) && (strcmp(mem[i].reportDate, responseMem.reportDate) == 0)){
			fprintf(appendResponse,"%s|%s|%s|%d|%s|%s\n", responseMem.ID, responseMem.contactNo, responseMem.reason, responseMem.noOfOffend, responseMem.reportDate, responseMem.reportStatus);
		}
		else {
			fprintf(writeReport, "%s|%s|%s|%d|%s|%s\n", mem[i].ID, mem[i].contactNo, mem[i].reason, mem[i].noOfOffend, mem[i].reportDate, mem[i].reportStatus);
		}
	}
	fclose(writeReport);
	fclose(appendResponse);
	red();
	printf("\aResponse successfully made\n");
	resetColor();
	system("pause");
	free(mem);
	free(matchID);
}

//helper functions
void autoNewMemID(char input[]) {
	Member mem;
	FILE* readMem = fopen("member.txt", "r");
	if (readMem == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	while (!feof(readMem)) {
		fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
	}
	fclose(readMem);

	char extractSeq[5] = { mem.ID[3],mem.ID[4],mem.ID[5],mem.ID[6] };
	int currentSeq = atoi(extractSeq);
	int newSeq = currentSeq + 1;

	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysYear = time.tm_year + 1900;
	int extractYear = sysYear % 1000;//get the remainder, which 2022 % 1000 = 22

	sprintf(input, "M%d%04d", extractYear, newSeq);
}
void memIDValidation(char input[],int*checkValidation) {
	if (strlen(input) != 7) {
		++*checkValidation;
		red();
		printf("\aExpected member ID length of 7\n");
		resetColor();
		system("pause");
		return;
	}
	
	if (input[0] != 77 && input[0] != 109) {
		++* checkValidation;
		red();
		printf("\aInvalid member ID inputted\n");
		printf("Member ID must start with 'M' as the first input\n");
		resetColor();	
		system("pause");
		return;
	}
	for (int i = 1; i < strlen(input);i++) {
		if (!(input[i] >= 48 && input[i] <= 57)) {
			++* checkValidation;
			red();
			printf("\aInvalid member ID inputed\n");
			printf("Please refers to the Member ID data convention\n");
			resetColor();
			system("pause");
			return;
		}
	}

	if (input[0] == 109) {
		input[0] -= 32;
		return;
	}
}
void addMemICValidation(char input[], char gender,int*checkValidation) {
	if (strlen(input) != 14) {
		++* checkValidation;
		red();
		printf("\aExpect length of 14\n");
		resetColor();
		system("pause");
		return;
	}

	if (input[6] != '-' && input[9] != '-') {
		++* checkValidation;
		red();
		printf("\aMake sure the '-'(dash) is on correct position\n");
		resetColor();
		system("pause");
		return;
	}

	if (gender == 'M') {
		if (input[13] != 49 && input[13] != 51 && input[13] != 53 && input[13] != 55 && input[13] != 57) {
			++* checkValidation;
			red();
			printf("\aPlease correct input on position 14\n");
			printf("Male's IC must end with 1/3/5/7/9 only\n");
			resetColor();
			system("pause");
			return;
		}
	}
	else {
		if (input[13] != 48 && input[13] != 50 && input[13] != 52 && input[13] != 54 && input[13] != 56) {
			++* checkValidation;
			red();
			printf("\aPlease correct input on position 15\n");
			printf("Female's IC must end with 2/4/6/8/0 only\n");
			resetColor();
			system("pause");
			return;
		}
	}

	char tempMonth[3] = { input[2],input[3]};
	char tempDay[3] = {input[4],input[5]};
	int month = atoi(tempMonth);
	int day = atoi(tempDay);
	if (!(month >= 1 && month <= 12)) {
		++* checkValidation;
		red();
		printf("\aPlease input logical month on input position 3 and 4\n");
		resetColor();
		system("pause");
		return;
	}
	else {
		if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
			if (!(day >= 1 && day <= 31)) {
				++* checkValidation;
				red();
				printf("\aPlease input logical day on input position 5 and 6\n");
				resetColor();
				system("pause");
				return;
			}
		}
		
		if (month == 4 || month == 6 || month == 9 || month == 11) {
			if (!(day >= 1 && day <= 30)) {
				++* checkValidation;
				red();
				printf("\aPlease input logical day on input position 5 and 6\n");
				resetColor();
				system("pause");
				return;
			}
		}
	
		if (month == 2) {
			//due to Malaysian IC year born is being truncated, unable to detect if 
			// the year is leap year or it isn't
			if (!(day >= 1 && day <= 29)) {
				++* checkValidation;
				red();
				printf("\aPlease input logical day on input position 5 and 6\n");
				resetColor();
				system("pause");
				return;	
			}
		}
	}

	/*
	//YYMMDD, MM = pos 2,3, pos 2 = 0/1
	if (!(input[2] >= 48 && input[2] <= 49)) {
		++* checkValidation;
		red();
		printf("\aInput doesn't match the Malaysia IC data convention\n");
		resetColor();
		system("pause");
		return;
	}
	//DD = pos 4,5     pos 4 = 0/1/2/3
	if (!(input[4] >= 48 && input[4] <= 51)) {
		++* checkValidation;
		red();
		printf("\aInput doesn't match the Malaysia IC data convention\n");
		resetColor();
		system("pause");
		return;
	}*/

	for (int i = 0; i < strlen(input); i++) {
		if (i == 6 || i == 9) {
			continue;
		}
		else {
			if (!(input[i] >= 48 && input[i] <= 57)) {
				++* checkValidation;
				red();
				printf("\aInput doesn't match the Malaysia IC data convention\n");
				resetColor();
				system("pause");
				return;
			}
		}
	}
}
void checkDuplicateIC(char input[], int *checkValidation) {
	Member mem;
	FILE* readMem = fopen("member.txt", "r");
	if (readMem == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	while (!feof(readMem)) {
		fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
		if (strcmp(mem.IC, input) == 0) {
			++* checkValidation;
			red();
			printf("\aDuplicate IC detected\n");
			printf("IC must be unique\n");
			resetColor();
			system("pause");
			fclose(readMem);
			return;
		}
	}
	fclose(readMem);
}
void memContactValidation(char input[], int* checkValidation) {
	if (!(strlen(input) >= 11 && strlen(input) <= 12)) {
		++* checkValidation;
		red();
		printf("\aExpect length of 11 or 12\n");
		resetColor();
		system("pause");
		return;
	}

	if (input[0] != 48) {
		++* checkValidation;
		red();
		printf("\aPersonal contact number must start with '0'\n");
		resetColor();
		system("pause");
		return;
	}

	if (input[3] != '-') {
		++* checkValidation;
		red();
		printf("\aMake sure the '-'(dash) is on correct position\n");
		resetColor();
		system("pause");
		return;
	}

	for (int i = 0; i < strlen(input); i++) {
		if (i == 3) {
			continue;
		}
		else {
			if (!(input[i] >= 48 && input[i] <= 57)) {
				if (!(input[i] >= 48 && input[i] <= 57)) {
					++* checkValidation;
					red();
					printf("\aIncorrect Malaysia personal phone number convention\n");
					resetColor();
					return;
				}
			}
		}
	}
}
void checkDuplicateContact(char input[], int* checkValidation) {
	Member mem;
	FILE* readMem = fopen("member.txt", "r");
	if (readMem == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		return;
	}
	while (!feof(readMem)) {
		fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
		if (strcmp(mem.contactNo, input) == 0) {
			++* checkValidation;
			red();
			printf("\aDuplicate contact number detected\n");
			printf("Contact number must be unique\n");
			resetColor();
			system("pause");
			fclose(readMem);
			return;
		}
	}
	fclose(readMem);
}
void durationOption(int selection, int* duration) {
	if (selection == 1) {
		*duration = 6;
		return;
	}
	if (selection == 2) {
		*duration = 12;
		return;
	}
	if (selection == 3) {
		*duration = 18;
		return;
	}
	if (selection == 4) {
		*duration = 24;
		return;
	}
	if (selection == 5) {
		*duration = 36;
		return;
	}

}
void membershipEndDate(int duration,int sDay, int sMonth, int sYear,int *eDay,int *eMonth,int *eYear) {
	*eDay = sDay;
	*eMonth = sMonth;
	*eYear = sYear;
	
	*eMonth += duration;
	if (*eMonth > 12 && *eMonth < 25) {
		*eMonth -= 12;
		++*eYear;
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
			*eDay = 30;//september & november only has 30 days
		}
	}

	if (sMonth == 8 && (duration == 6 || duration == 18)) {
		if (((*eYear % 4 == 0) && ((*eYear % 400 == 0) || (*eYear % 100 != 0)))) {
			if (sDay >= 29 && sDay <= 31) {
				*eDay = 29;//if it is a leap year
			}
		}
		else {
			if (sDay >= 29 && sDay <= 31) {
				*eDay = 28;//not a leap year
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
void memICValidation(char input[], int *checkValidation) {
	if (strlen(input) != 14) {
		++* checkValidation;
		red();
		printf("\aExpect length of 14\n");
		resetColor();
		system("pause");
		return;
	}

	if (input[6] != '-' && input[9] != '-') {
		++* checkValidation;
		red();
		printf("\aMake sure the '-'(dash) is on correct position\n");
		resetColor();
		system("pause");
		return;
	}

	char tempMonth[3] = { input[2],input[3] };
	char tempDay[3] = { input[4],input[5] };
	int month = atoi(tempMonth);
	int day = atoi(tempDay);
	if (!(month >= 1 && month <= 12)) {
		++* checkValidation;
		red();
		printf("\aPlease input logical month on input position 3 and 4\n");
		resetColor();
		system("pause");
		return;
	}
	else {
		if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
			if (!(day >= 1 && day <= 31)) {
				++* checkValidation;
				red();
				printf("\aPlease input logical day on input position 5 and 6\n");
				resetColor();
				system("pause");
				return;
			}
		}

		if (month == 4 || month == 6 || month == 9 || month == 11) {
			if (!(day >= 1 && day <= 30)) {
				++* checkValidation;
				red();
				printf("\aPlease input logical day on input position 5 and 6\n");
				resetColor();
				system("pause");
				return;
			}
		}

		if (month == 2) {
			//due to Malaysian IC year born is being truncated, unable to detect if 
			// the year is leap year or it isn't
			if (!(day >= 1 && day <= 29)) {
				++* checkValidation;
				red();
				printf("\aPlease input logical day on input position 5 and 6\n");
				resetColor();
				system("pause");
				return;
			}
		}
	}

	for (int i = 0; i < strlen(input); i++) {
		if (i == 6 || i == 9) {
			continue;
		}
		else {
			if (!(input[i] >= 48 && input[i] <= 57)) {
				++* checkValidation;
				red();
				printf("\aInput doesn't match the Malaysia IC data convention\n");
				resetColor();
				system("pause");
				return;
			}
		}
	}

}
void updateMemStatus() {
	Member* mem = malloc(100 * sizeof * mem);
	BlackListMember *rec = malloc(50 * sizeof * rec);
	FILE* readMem = fopen("member.txt","r");
	FILE* readReportedRes = fopen("memberReportedRes.txt","r");
	if (readMem == NULL || readReportedRes == NULL) {
		red();
		printf("File to open file");
		resetColor();
		free(mem);
		free(rec);
		return;
	}
	int totalMemRec = 0, totalReportRes = 0;
	while (!feof(readMem)) {
		fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem[totalMemRec].ID, mem[totalMemRec].name, &mem[totalMemRec].gender, mem[totalMemRec].IC, mem[totalMemRec].contactNo, &mem[totalMemRec].regMonths, &mem[totalMemRec].startDate.day, &mem[totalMemRec].startDate.month, &mem[totalMemRec].startDate.year, &mem[totalMemRec].endDate.day, &mem[totalMemRec].endDate.month, &mem[totalMemRec].endDate.year, mem[totalMemRec].status);
		++totalMemRec;
	}
	fclose(readMem);
	while (!feof(readReportedRes)) {
		fscanf(readReportedRes, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^\n]\n", rec[totalReportRes].ID, rec[totalReportRes].contactNo, rec[totalReportRes].reason, &rec[totalReportRes].noOfOffend, rec[totalReportRes].reportDate, rec[totalReportRes].reportStatus);
		++totalReportRes;
	}
	fclose(readReportedRes);
	for (int i = 0; i < totalReportRes;i++) {
		for (int j = 0; j < totalMemRec; j++) {
			if ((strcmp(rec[i].ID, mem[j].ID) == 0) && rec[i].noOfOffend == 3) {
				strcpy(mem[j].status,"BLed");
			}
			if ((strcmp(rec[i].ID, mem[j].ID) == 0) && rec[i].noOfOffend == 5) {
				strcpy(mem[j].status, "Banned");
			}
		}
	}
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysYear = time.tm_year + 1900;

	for (int i = 0; i < totalMemRec; i++) {
		if ((sysMonth > mem[i].endDate.month && sysYear == mem[i].endDate.year) || (sysYear > mem[i].endDate.year) && (strcmp(mem[i].status, "Active") == 0)) {
			strcpy(mem[i].status,"Expired");
		}
	}
	
	FILE* writeMem = fopen("member.txt","w");
	if (writeMem == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		free(mem);
		free(rec);
		return;
	}
	for (int i = 0; i < totalMemRec; i++) {
		fprintf(writeMem, "%s|%s|%c|%s|%s|%d|%d-%d-%d|%d-%d-%d|%s\n", mem[i].ID, mem[i].name, mem[i].gender, mem[i].IC, mem[i].contactNo, mem[i].regMonths, mem[i].startDate.day, mem[i].startDate.month, mem[i].startDate.year, mem[i].endDate.day, mem[i].endDate.month, mem[i].endDate.year, mem[i].status);
	}
	fclose(writeMem);

	free(mem);
	free(rec);
}
