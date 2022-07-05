#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#pragma warning(disable: 4996)

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

typedef struct {
	char ID[7];
	int sDay, sMonth, sYear;
	int totalDay;
	int eDay, eMonth, eYear;
	char reason[51];
	char leaveStatus[9];
	char rejectReason[51];
}RejectLeave;

void displayStaffDetails();
void searchStaffMenu();
void searchStaffByID();
void searchStaffByName();
void modifyStaffMenu();
void modifyStaffByID();
void modifyStaffByName();
void modifyStaffName(char staffName[]);
void modifyStaffPW(char staffPW[]);
void modifyStaffPWRP(int* staffPWRP);
void modifyStaffPosition(char staffPosition[]);
void modifyStaffEmail(char staffEmail[]);
void modifyStaffAL(int* staffAL);
void addStaff();
void changeStaffStatus();
void applyLeave();
void reviewLeave();


void staffIDValidation(char userInput[], int* checkValidation);
void staffNameValidation(char userInput[], int* checkValidation);
void staffPWValidation(char userInput[], int* checkValidation);
void staffEmailValidation(char userInput[], int* checkValidation);
void autoStaffIDGenerate(char input[], int currentStaffCount);
void positionList(char position[]);
void checkDuplicatedEmail(char input[], int* checkValidation);
void checkIDExist(char userInput[], int* checkValidation);
void checkIDAlreadyPendingLeave(char input[], int* checkValidation);
void validateDate(int currentDay, int currentMonth, int currentYear, int* checkValidation);
void checkALCount(char ID[], int totalDay, int* checkValidation);
void updateLeaveEndDate(int totalDay, int startDay, int startMonth, int startYear, int* endDay, int* endMonth, int* endYear);
void updateLeaveStatus(int response, char status[]);
void updateALCount(char ID[], int totalDay);
void checkLeaveApplicationEmpty(char input[], int* checkEmpty);
void validateEndOfLeaveDate(int* currentDay, int* currentMonth, int* currentYear);


//void staffWriteFile();
void red();
void yellow();
void green();
void resetColor();
void updateStaffLeaveStatus();
int countTotalLeaveRecord();

//apply leave
//on leave
//modify leave
void staffModule() {
	/*
	Staff staff;

	int staffCount = 0;

	FILE* readStaff;
	readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		printf("Failed to open file.");
		return -1;
	}
	while (!feof(readStaff)) {
		fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.staffID, staff.staffName, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email,staff.status, &staff.annualLeaveCount);
		++staffCount;
	}*/
	//updateStaffLeaveStatus();

	int menuSelection;

	do {
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\tStaff Module\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit the Module\n");
		printf("\t\t\t1. Display All Staff Details\n");
		printf("\t\t\t2. Search Staff Details\n");
		printf("\t\t\t3. Modify Staff Details\n");
		printf("\t\t\t4. Add New Staff\n");
		printf("\t\t\t5. Change staff status\n");
		printf("\t\t\t6. Apply leave\n");
		printf("\t\t\t7. Review leave\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\tYour selection(numeric input only): ");
		rewind(stdin);//incase user input non-numeric input. the system will go into infinite loop.
		scanf("%d", &menuSelection);

		switch (menuSelection) {
		case 0:
			printf("\nThank you for using our system.\n");
			break;

		case 1:
			displayStaffDetails();
			break;

		case 2:
			searchStaffMenu();
			break;

		case 3:
			modifyStaffMenu();
			break;

		case 4:
			addStaff();
			break;

		case 5:
			changeStaffStatus();
			break;

		case 6:
			system("cls");
			applyLeave();
			break;

		case 7:
			system("cls");
			reviewLeave();
			break;

		default:
			yellow();
			printf("\t\t\tInvalid selection!\a\n");
			resetColor();
			system("pause");
		}
	} while (menuSelection != 0);

	//fclose(readStaff);
}

void displayStaffDetails() {
	Staff staff;
	FILE* readStaff;
	readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	system("cls");
	printf("Display All Staff Information\n");
	printf("\nID\tName\t\t\t\tPosition\t\tEmail\t\t\t\tStatus");
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	while (!feof(readStaff)) {
		fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
		printf("%s\t%-31s\t%-21s\t%-31s\t%s\n", staff.ID, staff.name, staff.position, staff.email, staff.status);
	}
	printf("-----------------------------------------------------------------------------------------------------------------\n");
	system("pause");

	fclose(readStaff);
}

void searchStaffMenu() {
	/*Staff staff;
	FILE* readStaff;
	readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		printf("Failed to open file.");
		return -1;
	}*/

	int menuSelection;

	do {
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\tSearch Staff Information\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit the function\n");
		printf("\t\t\t1. Search Staff By ID\n");
		printf("\t\t\t2. Search Staff By Name\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\tYour selection(numeric input only): ");
		rewind(stdin);
		scanf("%d", &menuSelection);


		switch (menuSelection) {
		case 0:
			break;

		case 1:
			searchStaffByID();
			system("pause");
			break;

		case 2:
			searchStaffByName();
			system("pause");
			break;

		default:
			yellow();
			printf("\t\t\tInvalid selection!\a\n");
			resetColor();
			system("pause");
		}

	} while (menuSelection != 0);
}

void searchStaffByID() {

	char searchByID[7], cont;
	int checkValidation;

	do {
		Staff staff;
		FILE* readStaff;
		readStaff = fopen("staff.txt", "r");
		if (readStaff == NULL) {
			yellow();
			printf("Failed to open file.");
			resetColor();
			return;
		}

		do {
			system("cls");
			checkValidation = 0;
			printf("Enter the staff ID you wish to search: ");
			rewind(stdin);
			fgets(searchByID, 7, stdin);
			staffIDValidation(searchByID, &checkValidation);
		} while (checkValidation != 0);

		printf("\nStaff Information\n");
		while (!feof(readStaff)) {
			fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
			if (strcmp(searchByID, staff.ID) == 0) {
				printf("ID: %s\nName: %s\nPosition: %s\nEmail: %s\nStatus: %s\n", staff.ID, staff.name, staff.position, staff.email, staff.status);
				break; //once record found, then break the while loop
			}
			else if (feof(readStaff) && strcmp(searchByID, staff.ID) != 0) {
				//else if file finished read, and no record found.
				yellow();
				printf("No record found.\n");
				resetColor();
			}
		}

		printf("\nContinue to search other staff by ID?\n");
		printf("Your selection (Y=yes, any other key to exit)\n");
		rewind(stdin);
		cont = getch();
		cont = toupper(cont);

		fclose(readStaff);
	} while (cont == 'Y');
}

void searchStaffByName() {
	char searchByName[31], cont;
	int checkValidation;

	do {
		Staff staff;
		FILE* readStaff;
		readStaff = fopen("staff.txt", "r");
		if (readStaff == NULL) {
			yellow();
			printf("Failed to open file.");
			resetColor();
			return;
		}

		do {
			system("cls");
			checkValidation = 0;
			printf("Enter the staff name you wish to search: ");
			rewind(stdin);
			fgets(searchByName, 31, stdin);
			staffNameValidation(searchByName, &checkValidation);
		} while (checkValidation != 0);

		printf("\nStaff Information\n");
		while (!feof(readStaff)) {
			fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
			if (strcmp(searchByName, staff.name) == 0) {
				printf("ID: %s\nName: %s\nPosition: %s\nEmail: %s\nStatus: %s\n", staff.ID, staff.name, staff.position, staff.email, staff.status);
				break; //once record found, then break the while loop
			}
			else if (feof(readStaff) && strcmp(searchByName, staff.name) != 0) {
				//else if file finished read, and no record found.
				yellow();
				printf("No record found.\n");
				resetColor();
			}
		}

		printf("\nContinue to search other staff by name?\n");
		printf("Your selection (Y=yes, any other key to exit)\n");
		rewind(stdin);
		cont = getch();
		cont = toupper(cont);

		fclose(readStaff);
	} while (cont == 'Y');

}

void modifyStaffMenu() {

	int menuSelection;

	do {
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\Modify Staff Information\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit the function\n");
		printf("\t\t\t1. Modify Staff By ID\n");
		printf("\t\t\t2. Modify Staff By Name\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\tYour selection(numeric input only): ");
		rewind(stdin);
		scanf("%d", &menuSelection);


		switch (menuSelection) {
		case 0:
			break;

		case 1:
			modifyStaffByID();
			system("pause");
			break;

		case 2:
			modifyStaffByName();
			system("pause");
			break;

		default:
			yellow();
			printf("\t\t\tInvalid selection!\a\n");
			resetColor();
			system("pause");
		}

	} while (menuSelection != 0);

}

void modifyStaffByID() {
	char modifyByID[7], cont, confirm;
	int checkValidation;

	do {
		Staff staff, modifyStaff;
		FILE* readStaff;
		FILE* writeStaff;
		readStaff = fopen("staff.txt", "r");
		writeStaff = fopen("tempStaff.txt", "w");
		if (readStaff == NULL || writeStaff == NULL) {
			yellow();
			printf("Failed to open file.");
			resetColor();
			return;
		}

		do {
			system("cls");
			checkValidation = 0;
			printf("Enter the staff ID you wish to modify: ");
			rewind(stdin);
			fgets(modifyByID, 7, stdin);
			staffIDValidation(modifyByID, &checkValidation);
		} while (checkValidation != 0);

		//this is needed to prevent the programme run into the else if statement, which 
		//stated no record found in the switch case.
		int exitModification = 0;
		printf("\nStaff Information\n");
		while (!feof(readStaff)) {
			fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
			modifyStaff = staff;
			if (strcmp(modifyByID, modifyStaff.ID) == 0) {
				printf("ID: %s\nName: %s\nPosition: %s\nEmail: %s\nStatus: %s\n", modifyStaff.ID, modifyStaff.name, modifyStaff.position, modifyStaff.email, modifyStaff.status);
				printf("\nConfirm to change the staff information? [Y=yes, other key to exit]: ");
				rewind(stdin);
				confirm = getch();
				confirm = toupper(confirm);
				if (confirm == 'Y') {
					int menuSelection;
					do {
						exitModification = 0;
						system("cls");
						printf("\t\t\t---------------------------------------------------\n");
						printf("\t\t\t\t\Modify Staff Information\n");
						printf("\t\t\t---------------------------------------------------\n");
						printf("\t\t\t0. Exit the modification\n");
						printf("\t\t\t1. Modify Name\n");
						printf("\t\t\t2. Modify Password\n");
						printf("\t\t\t3. Modify Password Reset Pin\n");
						printf("\t\t\t4. Modify Position\n");
						printf("\t\t\t5. Modify Email\n");
						printf("\t\t\t6. Modify Annual Leave Count\n");
						printf("\t\t\t---------------------------------------------------\n");
						printf("\t\t\tYour selection(numeric input only): ");
						rewind(stdin);
						scanf("%d", &menuSelection);
						switch (menuSelection) {
						case 0:
							exitModification = 1;
							break;

						case 1:
							modifyStaffName(modifyStaff.name);
							system("pause");
							break;

						case 2:
							modifyStaffPW(modifyStaff.pw.password);
							system("pause");
							break;

						case 3:
							modifyStaffPWRP(&modifyStaff.pw.pwResetPin);
							system("pause");
							break;

						case 4:
							modifyStaffPosition(modifyStaff.position);
							system("pause");
							break;

						case 5:
							modifyStaffEmail(modifyStaff.email);
							system("pause");
							break;

						case 6:
							modifyStaffAL(&modifyStaff.annualLeaveCount);
							system("pause");
							break;

						default:
							yellow();
							printf("Invalid selection");
							resetColor();
						}
					} while (menuSelection != 0);
					//break;
				}

				else {
					red();
					printf("\nModification cancelled.\a\n");
					resetColor();
					++exitModification;
					//break;
				}
			}
			else if (feof(readStaff) && strcmp(modifyByID, staff.ID) != 0 && exitModification == 0) {
				//else if file finished read, and no record found.
				yellow();
				printf("No record found.\n");
				resetColor();
			}
			fprintf(writeStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", modifyStaff.ID, modifyStaff.name, modifyStaff.pw.password, modifyStaff.pw.pwResetPin, modifyStaff.position, modifyStaff.email, modifyStaff.status, modifyStaff.annualLeaveCount);
		}

		fclose(writeStaff);
		fclose(readStaff);
		remove("staff.txt");
		rename("tempStaff.txt", "staff.txt");

		printf("\nContinue to modify other staff by ID?\n");
		printf("Your selection (Y=yes, any other key to exit)\n");
		rewind(stdin);
		cont = getch();
		cont = toupper(cont);
	} while (cont == 'Y');
}

void modifyStaffByName() {
	char modifyByName[31], cont, confirm;
	int checkValidation;

	do {
		Staff staff, modifyStaff;
		FILE* readStaff;
		FILE* writeStaff;
		readStaff = fopen("staff.txt", "r");
		writeStaff = fopen("tempStaff.txt", "w");
		if (readStaff == NULL || writeStaff == NULL) {
			yellow();
			printf("Failed to open file.");
			resetColor();
			return;
		}

		do {
			system("cls");
			checkValidation = 0;
			printf("Enter the staff name you wish to modify: ");
			rewind(stdin);
			fgets(modifyByName, 31, stdin);
			staffNameValidation(modifyByName, &checkValidation);
		} while (checkValidation != 0);

		//this is needed to prevent the programme run into the else if statement, which 
		//stated no record found in the switch case.
		int exitModification = 0;
		printf("\nStaff Information\n");
		while (!feof(readStaff)) {
			fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
			modifyStaff = staff;
			if (strcmp(modifyByName, modifyStaff.name) == 0) {
				printf("ID: %s\nName: %s\nPosition: %s\nEmail: %s\nStatus: %s\n", modifyStaff.ID, modifyStaff.name, modifyStaff.position, modifyStaff.email, modifyStaff.status);
				printf("\nConfirm to change the staff information? [Y=yes, other key to exit]: ");
				rewind(stdin);
				confirm = getch();
				confirm = toupper(confirm);
				if (confirm == 'Y') {
					int menuSelection;
					do {
						exitModification = 0;
						system("cls");
						printf("\t\t\t---------------------------------------------------\n");
						printf("\t\t\t\t\Modify Staff Information\n");
						printf("\t\t\t---------------------------------------------------\n");
						printf("\t\t\t0. Exit the modification\n");
						printf("\t\t\t1. Modify Name\n");
						printf("\t\t\t2. Modify Password\n");
						printf("\t\t\t3. Modify Password Reset Pin\n");
						printf("\t\t\t4. Modify Position\n");
						printf("\t\t\t5. Modify Email\n");
						printf("\t\t\t6. Modify Annual Leave Count\n");
						printf("\t\t\t---------------------------------------------------\n");
						printf("\t\t\tYour selection(numeric input only): ");
						rewind(stdin);
						scanf("%d", &menuSelection);
						switch (menuSelection) {
						case 0:
							exitModification = 1;
							break;

						case 1:
							modifyStaffName(modifyStaff.name);
							system("pause");
							break;

						case 2:
							modifyStaffPW(modifyStaff.pw.password);
							system("pause");
							break;

						case 3:
							modifyStaffPWRP(&modifyStaff.pw.pwResetPin);
							system("pause");
							break;

						case 4:
							modifyStaffPosition(modifyStaff.position);
							system("pause");
							break;

						case 5:
							modifyStaffEmail(modifyStaff.email);
							system("pause");
							break;

						case 6:
							modifyStaffAL(&modifyStaff.annualLeaveCount);
							system("pause");
							break;

						default:
							yellow();
							printf("Invalid selection");
							resetColor();
						}
					} while (menuSelection != 0);
					//break;
				}

				else {
					red();
					printf("\nModification cancelled.\a\n");
					resetColor();
					//break;
				}
			}
			else if (feof(readStaff) && strcmp(modifyByName, staff.ID) != 0 && exitModification == 0) {
				//else if file finished read, and no record found.
				yellow();
				printf("No record found.\n");
				resetColor();
			}
			fprintf(writeStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", modifyStaff.ID, modifyStaff.name, modifyStaff.pw.password, modifyStaff.pw.pwResetPin, modifyStaff.position, modifyStaff.email, modifyStaff.status, modifyStaff.annualLeaveCount);
		}

		fclose(writeStaff);
		fclose(readStaff);
		remove("staff.txt");
		rename("tempStaff.txt", "staff.txt");

		printf("\nContinue to modify other staff by name?\n");
		printf("Your selection (Y=yes, any other key to exit)\n");
		rewind(stdin);
		cont = getch();
		cont = toupper(cont);
	} while (cont == 'Y');
}

void modifyStaffName(char staffName[]) {
	char tempName[31], confirm;
	int checkValidation;
	do {
		checkValidation = 0;
		printf("\nEnter new staff name: ");
		rewind(stdin);
		fgets(tempName, 31, stdin);
		staffNameValidation(tempName, &checkValidation);
	} while (checkValidation != 0);

	red();
	printf("Current name: %s", staffName);
	printf("\n\t\t|");
	printf("\n\t\t|");
	printf("\n\t\tv\n");
	printf("New name: %s\n", tempName);
	resetColor();
	printf("confirm to change the new name? [Y=yes, any other key to exit]: ");
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		strcpy(staffName, tempName);
		red();
		printf("Modification successed\n");
		resetColor();
	}
	else {
		red();
		printf("\nModification cancelled\a\n");
		resetColor();
	}
}

void modifyStaffPW(char staffPW[]) {
	char tempPW[21], confirm;
	int checkValidation;
	do {
		checkValidation = 0;
		yellow();
		printf("\nMaximum length of 20");
		printf("\nInput after length of 20 will be ignored.\n");
		resetColor();
		printf("\nEnter new staff password: ");
		rewind(stdin);
		fgets(tempPW, 21, stdin);
		staffPWValidation(tempPW, &checkValidation);
	} while (checkValidation != 0);

	red();
	printf("Current password: %s", staffPW);
	printf("\n\t\t|");
	printf("\n\t\t|");
	printf("\n\t\tv\n");
	printf("New password: %s\n", tempPW);
	resetColor();

	printf("\nConfirm to change the new password? [Y=yes, any other key to exit]: ");
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		strcpy(staffPW, tempPW);
		red();
		printf("Modification successed\n");
		resetColor();
	}
	else {
		red();
		printf("\nModification cancelled\a\n");
		resetColor();
	}
}

void modifyStaffPWRP(int* staffPWRP) {
	char confirm;
	int tempPWRP;
	do {
		yellow();
		printf("\nMaximum length of 6 NUMERIC INPUT.");
		printf("\nAvoid 0 as the leading digit otherwise 0 will be truncated.\n");
		resetColor();
		printf("\nEnter new staff PWRP(Password Reset Pin): ");
		rewind(stdin);
		scanf("%d", &tempPWRP);
	} while (!(tempPWRP >= 1 && tempPWRP <= 999999));

	red();
	printf("Current PWRP: %d", *staffPWRP);
	printf("\n\t\t|");
	printf("\n\t\t|");
	printf("\n\t\tv\n");
	printf("New PWRP: %d\n", tempPWRP);
	resetColor();

	printf("\nConfirm to change the new PWRP? [Y=yes, any other key to exit]: ");
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		*staffPWRP = tempPWRP;
		red();
		printf("Modification successed\n");
		resetColor();
	}
	else {
		red();
		printf("\nModification cancelled\a\n");
		resetColor();
	}
}

void modifyStaffPosition(char staffPosition[]) {
	char confirm, tempPosition[21];
	int positionSelection;
	do {
		printf("New position: \n0.Cancel modification\n1. CEO\n2. COO\n3. IT Manager\n4. IT Technician");
		printf("\n5. HR Manager\n6. HR Personnel\n7. Customer Service\n");
		printf("Key in the number assigned to the position: ");
		rewind(stdin);
		scanf("%d", &positionSelection);
	} while (!(positionSelection >= 0 && positionSelection <= 7));
	switch (positionSelection) {
	case 0:
		red();
		printf("\nModification cancelled\a\n");
		resetColor();
		return;

	case 1:
		strcpy(tempPosition, "CEO");
		break;

	case 2:
		strcpy(tempPosition, "COO");
		break;

	case 3:
		strcpy(tempPosition, "IT Manager");
		break;

	case 4:
		strcpy(tempPosition, "IT Technician");
		break;

	case 5:
		strcpy(tempPosition, "HR Manager");
		break;

	case 6:
		strcpy(tempPosition, "HR Personnel");
		break;

	case 7:
		strcpy(tempPosition, "Customer Service");
		break;
	}
	red();
	printf("Current Position: %s", staffPosition);
	printf("\n\t\t|");
	printf("\n\t\t|");
	printf("\n\t\tv\n");
	printf("New Position: %s\n", tempPosition);
	resetColor();

	printf("\nConfirm to change the new position? [Y=yes, any other key to exit]: ");
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		strcpy(staffPosition, tempPosition);
		red();
		printf("Modification successed\n");
		resetColor();
	}
	else {
		red();
		printf("\nModification cancelled\a\n");
		resetColor();
	}
}

void modifyStaffEmail(char staffEmail[]) {
	char tempEmail[31], confirm;
	int checkValidation;
	do {
		checkValidation = 0;
		printf("\nEnter new staff email: ");
		rewind(stdin);
		fgets(tempEmail, 31, stdin);
		staffEmailValidation(tempEmail, &checkValidation);
	} while (checkValidation != 0);
	red();
	printf("Current email: %s", staffEmail);
	printf("\n\t\t|");
	printf("\n\t\t|");
	printf("\n\t\tv\n");
	printf("New email: %s\n", tempEmail);
	resetColor();

	printf("\nConfirm to change the new email? [Y=yes, any other key to exit]: ");
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		strcpy(staffEmail, tempEmail);
		red();
		printf("Modification successed\n");
		resetColor();
	}
	else {
		red();
		printf("\nModification cancelled\a\n");
		resetColor();
	}
}

void modifyStaffAL(int* staffAL) {
	char confirm;
	int tempALcount;

	printf("The staff current annual leave count: %d", *staffAL);
	red();
	printf("PLEASE REFRAIN FROM MODIFY THIS DATA WITHOUT A PROPER REASON OR INSTRUCTION.");
	resetColor();
	printf("\nTotal day to add (numeric input): ");
	rewind(stdin);//incase user input alphabetic input may cause infinite loop
	scanf("%d", &tempALcount);

	printf("\nConfirm to update the annual leave count? [Y=yes, any other key to exit]: ");
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		*staffAL += tempALcount;
		red();
		printf("Modification successed\n");
		resetColor();
	}
	else {
		red();
		printf("\nModification cancelled\a\n");
		resetColor();
	}
}

void addStaff() {
	system("cls");
	Staff staff;
	FILE* readStaff;
	int countRecord = 0;

	readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	while (!feof(readStaff)) {
		fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
		++countRecord;
	}
	fclose(readStaff);



	char confirm;
	printf("\nConfirm to add new staff? [Y=yes, any other key to exit]: \n");
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);



	if (confirm == 'Y') {
		Staff newStaff;
		red();
		printf("Adding new staff\n");
		printf("Staff ID will be auto generated.\n");
		resetColor();
		//id
		char autoStaffID[7];
		autoStaffIDGenerate(autoStaffID, countRecord + 1);
		strcpy(newStaff.ID, autoStaffID);

		//name
		int checkValidation;
		do {
			checkValidation = 0;
			printf("\nEnter new staff name\n");
			printf("Maximum length of 30\n");
			printf("Name: ");
			rewind(stdin);
			fgets(newStaff.name, 31, stdin);
			staffNameValidation(newStaff.name, &checkValidation);
		} while (checkValidation != 0);

		//password
		do {
			checkValidation = 0;
			printf("\nEnter new staff password\n");
			printf("Maximum length of 20\n");
			printf("Password: ");
			rewind(stdin);
			fgets(newStaff.pw.password, 21, stdin);
			staffPWValidation(newStaff.pw.password, &checkValidation);
		} while (checkValidation != 0);

		//PWRP
		red();
		printf("\nPassword Reset Pin will be auto generated base on the new staff's ID sequence of entry\n\n");
		resetColor();
		newStaff.pw.pwResetPin = countRecord + 1;

		//position
		printf("New staff position\n");
		positionList(newStaff.position);

		//email		
		do {
			checkValidation = 0;
			printf("\nEnter new staff email\n");
			printf("Maximum length of 30\n");
			printf("Email: ");
			rewind(stdin);
			fgets(newStaff.email, 31, stdin);
			staffEmailValidation(newStaff.email, &checkValidation);
			if (checkValidation == 0)
				checkDuplicatedEmail(newStaff.email, &checkValidation);
		} while (checkValidation != 0);

		//status
		red();
		printf("\nNew staff status will be auto assigned to active.\n");
		resetColor();
		strcpy(newStaff.status, "Active");

		//annual leave count
		red();
		printf("\nNew staff annual leave count will be auto generated.\n");
		resetColor();
		newStaff.annualLeaveCount = 5;

		printf("\nNew staff details\n");
		printf("ID: %s\nName: %s\nPassword: %s\nPWRP: %d\nPosition: %s\nEmail: %s\nStatus: %s\nAnnual leave count: %d\n", newStaff.ID, newStaff.name, newStaff.pw.password, newStaff.pw.pwResetPin, newStaff.position, newStaff.email, newStaff.status, newStaff.annualLeaveCount);
		system("pause");
		FILE* appendStaff;
		appendStaff = fopen("staff.txt", "a");
		if (appendStaff == NULL) {
			yellow();
			printf("Failed to open file.");
			resetColor();
			return;
		}
		fprintf(appendStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", newStaff.ID, newStaff.name, newStaff.pw.password, newStaff.pw.pwResetPin, newStaff.position, newStaff.email, newStaff.status, newStaff.annualLeaveCount);
		fclose(appendStaff);
	}
	else {
		red();
		printf("\nNew staff addition cancelled\a\n");
		resetColor();
		system("pause");
	}
}

void changeStaffStatus() {
	system("cls");
	Staff staff, modifyStaff;
	FILE* readStaff, * writeStaff;
	char modifyByID[7], confirm;
	int checkValidation;

	readStaff = fopen("staff.txt", "r");
	writeStaff = fopen("tempStaff.txt", "w");
	if (readStaff == NULL || writeStaff == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}

	do {
		system("cls");
		checkValidation = 0;
		printf("Enter the staff ID: ");
		rewind(stdin);
		fgets(modifyByID, 7, stdin);
		staffIDValidation(modifyByID, &checkValidation);
	} while (checkValidation != 0);

	int exitModification = 0;
	printf("\nStaff Information\n");
	while (!feof(readStaff)) {
		fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
		modifyStaff = staff;
		if (strcmp(modifyByID, modifyStaff.ID) == 0) {
			printf("ID: %s\nName: %s\nPosition: %s\nEmail: %s\nStatus: %s\n", modifyStaff.ID, modifyStaff.name, modifyStaff.position, modifyStaff.email, modifyStaff.status);
			red();
			printf("\nPLEASE REFRAIN FROM MODIFY THIS DATA WITHOUT A PROPER REASON OR INSTRUCTION.");
			resetColor();
			printf("\nConfirm to change the staff status? [Y=yes, other key to exit]: ");
			rewind(stdin);
			confirm = getch();
			confirm = toupper(confirm);
			if (confirm == 'Y') {
				if (strcmp(modifyStaff.status, "Active") == 0)
					strcpy(modifyStaff.status, "Resigned");
				else
					strcpy(modifyStaff.status, "Active");
				yellow();
				printf("\nThe staff status has been changed to %s\n", modifyStaff.status);
				resetColor();
				system("pause");
			}
			else {
				++exitModification;
				red();
				printf("\nModification cancelled\a\n");
				resetColor();
				system("pause");
			}
		}

		else if (feof(readStaff) && strcmp(modifyByID, staff.ID) != 0 && exitModification == 0) {
			//else if file finished read, and no record found.
			yellow();
			printf("No record found.\n");
			resetColor();
			system("pause");
		}

		fprintf(writeStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", modifyStaff.ID, modifyStaff.name, modifyStaff.pw.password, modifyStaff.pw.pwResetPin, modifyStaff.position, modifyStaff.email, modifyStaff.status, modifyStaff.annualLeaveCount);
	}

	fclose(writeStaff);
	fclose(readStaff);
	remove("staff.txt");
	rename("tempStaff.txt", "staff.txt");
}

void applyLeave() {
	Leave leave;
	int checkValidation;
	FILE* writeRec, * writeHistory;
	writeRec = fopen("leaveApplication.txt", "a"); // one for review application
	writeHistory = fopen("leaveApplicationHistory.txt", "a"); // record for further reference if any dispute
	if (writeRec == NULL || writeHistory == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}

	printf("Leave application\n");
	do {
		checkValidation = 0;
		printf("Enter the staff id who is applying for leave: ");
		rewind(stdin);
		fgets(leave.ID, 7, stdin);
		staffIDValidation(leave.ID, &checkValidation);
		if (checkValidation == 0)
			checkIDExist(leave.ID, &checkValidation);
		checkIDAlreadyPendingLeave(leave.ID, &checkValidation);
		if (checkValidation == -1) {
			red();
			printf("A leave application is currently pending.\a\n");
			resetColor();
			system("pause");
			return;
		}
	} while (checkValidation != 0);

	do {
		checkValidation = 0;
		printf("Enter leave date(numberic input): DD MM YYYY\n");
		rewind(stdin);
		scanf("%d %d %d", &leave.sDay, &leave.sMonth, &leave.sYear);
		validateDate(leave.sDay, leave.sMonth, leave.sYear, &checkValidation);
	} while (checkValidation != 0);

	do {
		checkValidation = 0;
		printf("\nHow many day(s) of leave: ");
		rewind(stdin);
		scanf("%d", &leave.totalDay);
		checkALCount(leave.ID, leave.totalDay, &checkValidation);
		if (checkValidation == -1)
			return;
		if (leave.totalDay < 0) {
			yellow();
			printf("Iloogical day input.\a\n");
			resetColor();
		}
	} while (checkValidation != 0 || leave.totalDay < 0);

	updateLeaveEndDate(leave.totalDay, leave.sDay, leave.sMonth, leave.sYear, &leave.eDay, &leave.eMonth, &leave.eYear);
	printf("\nStated the reason in 1 line (maximum 50 words, words come after length of 50 will be truncated.)\n");
	rewind(stdin);
	fgets(leave.reason, 51, stdin);
	for (int i = 0; i < strlen(leave.reason); i++) {
		if (leave.reason[i] == '\n') {
			leave.reason[i] = '\0';
		}
	}

	red();
	printf("Leave application status will be auto assigned to pending until review by person in charged.\n");
	printf("DISCLAIMER: pending status doesn't mean approved, thus staff shouldn't absent from work unless\n");
	printf("the leave application status is approved.\n");
	resetColor();
	strcpy(leave.leaveStatus, "Pending");
	system("pause");
	system("cls");
	printf("\nLeave application detail\n");
	printf("ID: %s\nTotal days of leave applied: %d\nOn leave from: %02d-%02d-%d\n", leave.ID, leave.totalDay, leave.sDay, leave.sMonth, leave.sYear);
	printf("On leave to: %02d-%02d-%d\n", leave.eDay, leave.eMonth, leave.eYear);
	printf("Reason: %s\n", leave.reason);
	printf("Status: %s\n", leave.leaveStatus);

	char confirm;
	printf("\nConfirm to submit the leave application? [Y=yes, any other key to exit]: \n");
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		/*
		FILE* writeRec, *writeHistory;
		writeRec = fopen("leaveApplication.txt","a"); // one for review application
		writeHistory = fopen("leaveApplicationHistory.txt","a"); // record for further reference if any dispute
		if (writeRec == NULL || writeHistory == NULL) {
			yellow();
			printf("Failed to open file.");
			resetColor();
			return;
		}*/
		fprintf(writeRec, "%s|%d|%d-%d-%d|%d-%d-%d|%s|%s\n", leave.ID, leave.totalDay, leave.sDay, leave.sMonth, leave.sYear, leave.eDay, leave.eMonth, leave.eYear, leave.reason, leave.leaveStatus);
		fprintf(writeHistory, "%s|%d|%d-%d-%d|%d-%d-%d|%s|%s\n", leave.ID, leave.totalDay, leave.sDay, leave.sMonth, leave.sYear, leave.eDay, leave.eMonth, leave.eYear, leave.reason, leave.leaveStatus);
		//fclose(writeRec);
		//fclose(writeHistory);
		red();
		printf("Leave application submitted\n");
		resetColor();
		system("pause");
	}
	else {
		red();
		printf("\nLeave application cancelled\a\n");
		resetColor();
		system("pause");
	}
	fclose(writeRec);
	fclose(writeHistory);
}

void reviewLeave() {
	printf("Leave application review\n");
	FILE* readRec;
	Leave leave;
	readRec = fopen("leaveApplication.txt", "r");
	if (readRec == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	int checkFileEmpty = 0;
	while (!feof(readRec)) {
		fscanf(readRec, "%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^|]|%[^\n]\n", leave.ID, &leave.totalDay, &leave.sDay, &leave.sMonth, &leave.sYear, &leave.eDay, &leave.eMonth, &leave.eYear, leave.reason, leave.leaveStatus);
		checkLeaveApplicationEmpty(leave.ID, &checkFileEmpty);
		if (checkFileEmpty == 1) {
			system("cls");
			yellow();
			printf("No leave application currently\a\n");
			resetColor();
			system("pause");
			return;
		}
		else {
			printf("ID: %s\nTotal days of leave applied: %d\nOn leave from: %02d-%02d-%d\n", leave.ID, leave.totalDay, leave.sDay, leave.sMonth, leave.sYear);
			printf("On leave to: %02d-%02d-%d\n", leave.eDay, leave.eMonth, leave.eYear);
			printf("Reason: %s\n", leave.reason);
			printf("Status: %s\n\n", leave.leaveStatus);
		}
	}
	//fclose(readRec);

	int checkValidation;
	char ID[7];
	do {
		checkValidation = 0;
		printf("Enter the staff ID: ");
		rewind(stdin);
		fgets(ID, 7, stdin);
		staffIDValidation(ID, &checkValidation);
		if (checkValidation == 0)
			checkIDExist(ID, &checkValidation);
	} while (checkValidation != 0);

	int response;
	char tempStatus[9];
	do {
		printf("Leave application response\n");
		printf("1. Approved\n");
		printf("2. Rejected\n");
		printf("Response as the digit assigned: ");
		rewind(stdin);
		scanf("%d", &response);
		updateLeaveStatus(response, tempStatus);
	} while (!(response >= 1 && response <= 2));


	char rejectReason[51];
	if (strcmp(tempStatus, "Rejected") == 0) {
		printf("\nPlease state the reason of reject the application in 1 line (maximum 50 words, words come after length of 50 will be truncated.)\n");
		rewind(stdin);
		fgets(rejectReason, 51, stdin);
	}
	printf("\nThe application status will be update to %s\n", tempStatus);

	char confirm;
	printf("Confirm to update the status? [Y=yes, any other key to exit]: \n");
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		//remove the application that has been responded.
		Leave tempLeaveRec;
		FILE* updateLeaveRec;
		updateLeaveRec = fopen("tempLeaveApplication.txt", "w");
		//readRec = fopen("leaveApplication.txt","r");
		//readRec == NULL || 
		if (updateLeaveRec == NULL) {
			yellow();
			printf("Failed to open file.");
			resetColor();
			return;
		}
		while (!feof(readRec)) {
			fscanf(readRec, "%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^|]|%[^\n]\n", leave.ID, &leave.totalDay, &leave.sDay, &leave.sMonth, &leave.sYear, &leave.eDay, &leave.eMonth, &leave.eYear, leave.reason, leave.leaveStatus);
			tempLeaveRec = leave;
			if (strcmp(leave.ID, ID) == 0) {
				//if the status is approved
				if (strcmp(tempStatus, "Approved") == 0) {
					if (leave.totalDay == 0) //apply for 0 day consider as half day off, however still deduct 1 day of AL count
						++leave.totalDay;
					//update the AL count upon leave application have been approved
					updateALCount(ID, leave.totalDay);
					//delcare a new leave struct to store the data, which change the match ID data
					Leave approved;
					approved = leave;
					//change the status to approved
					strcpy(approved.leaveStatus, tempStatus);
					//write the approved record
					FILE* writeRec;
					writeRec = fopen("leaveApproved.txt", "a");
					fprintf(writeRec, "%s|%d|%d-%d-%d|%d-%d-%d|%s|%s\n", approved.ID, approved.totalDay, approved.sDay, approved.sMonth, approved.sYear, approved.eDay, approved.eMonth, approved.eYear, approved.reason, approved.leaveStatus);
					fclose(writeRec);
				}
				//if the status is rejected
				else {
					RejectLeave rejected;
					//assign value of leave to reject. Unable to copy the entire data, due to rejected has 1 more member, that is
					//reject reason
					strcpy(rejected.ID, leave.ID); rejected.sDay = leave.sDay; rejected.sMonth = leave.sMonth; rejected.sYear = leave.sYear;
					rejected.totalDay = leave.totalDay; rejected.eDay = leave.eDay; rejected.eMonth = leave.eMonth; rejected.eYear = leave.eYear;
					strcpy(rejected.reason, leave.reason); strcpy(rejected.leaveStatus, tempStatus); strcpy(rejected.rejectReason, rejectReason);
					FILE* writeRec;
					writeRec = fopen("leaveResponseRecord.txt", "a");
					fprintf(writeRec, "%s|%d|%d-%d-%d|%d-%d-%d|%s|%s|%s\n", rejected.ID, rejected.totalDay, rejected.sDay, rejected.sMonth, rejected.sYear, rejected.eDay, rejected.eMonth, rejected.eYear, rejected.reason, rejected.leaveStatus, rejected.rejectReason);
					fclose(writeRec);
				}
			}

			//if the ID match with user input ID, ignore the line (to delete the line which response has been made)
			if (strcmp(tempLeaveRec.ID, ID) == 0) {
				continue;
			}
			//if the ID doesn't match with user input ID, then write the record
			else {
				fprintf(updateLeaveRec, "%s|%d|%d-%d-%d|%d-%d-%d|%s|%s\n", tempLeaveRec.ID, tempLeaveRec.totalDay, tempLeaveRec.sDay, tempLeaveRec.sMonth, tempLeaveRec.sYear, tempLeaveRec.eDay, tempLeaveRec.eMonth, tempLeaveRec.eYear, tempLeaveRec.reason, tempLeaveRec.leaveStatus);
			}
		}
		yellow();
		printf("Leave status has been updated\n");
		resetColor();
		system("pause");
		fclose(readRec);
		fclose(updateLeaveRec);
		remove("leaveApplication.txt");
		rename("tempLeaveApplication.txt", "leaveApplication.txt");
	}
	else {
		red();
		printf("\nLeave application status update cancelled\a\n");
		resetColor();
		system("pause");
	}
}

/*
void staffWriteFile(Staff staff) {
	FILE* writePtr;
	writePtr = fopen("tempStaff.txt", "w");
	if (writePtr == NULL) {
		yellow();
		printf("Failed to open file.");
		return;
		resetColor();
	}

	fprintf(writePtr, "%s|%s|%s|%d|%s|%s|%s|%d", staff.ID, staff.name, staff.pw.password, staff.pw.pwResetPin, staff.position, staff.email, staff.status, staff.annualLeaveCount);

	fclose(writePtr);
}*/

void staffIDValidation(char userInput[], int* checkValidation) {
	if (strlen(userInput) != 6) {
		++* checkValidation;
		yellow();
		printf("ID must be length of 6");
		resetColor();
		return;
	}

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
			return;
		}
		else
			break;
	}


}

void staffNameValidation(char userInput[], int* checkValidation) {
	if (userInput[0] == ' ') {
		++* checkValidation;
		yellow();
		printf("Name cannot be empty.\a\n");
		resetColor();
	}

	for (int i = 0; i < strlen(userInput); i++) {
		//alphabetic input allowed only for name
		if (!(userInput[i] >= 65 && userInput[i] <= 90) && !(userInput[i] >= 97 && userInput[i] <= 122) && userInput[i] != 32 && userInput[i] != '\n') {
			//need to include userInput[i] != '\n', because in the fgets , 31 space were allocated, the unused space will be replace by '\n'
			++* checkValidation;
			yellow();
			printf("Expect only alphabetic input.\a\n");
			resetColor();
			system("pause");
			break;
		}

		else {
			//allow user input to become non-case sensitive
			//convert first character to upper;
			userInput[0] = toupper(userInput[0]);

			if (userInput[i] == ' ')
				continue;

			//convert any character after space to upper;
			else if (userInput[i - 1] == ' ' && isalpha(userInput[i]) != 0)
				userInput[i] = toupper(userInput[i]);

			else if (userInput[i - 1] != ' ' && isalpha(userInput[i]) != 0)
				userInput[i] = tolower(userInput[i]);
			//using fgets() will add '\n' at the end of user input.
			//replace the '\n' with '\0' otherwise is consider unmatch with file's data / disrupt the data structure.
			else if (userInput[i] == '\n')
				userInput[i] = '\0';
		}
	}
}

void staffPWValidation(char userInput[], int* checkValidation) {
	if (userInput[0] == ' ') {
		++* checkValidation;
		yellow();
		printf("Password cannot be empty.\a\n");
		resetColor();
	}

	for (int i = 0; i < strlen(userInput); i++) {
		if (userInput[i] == ' ') {
			++* checkValidation;
			yellow();
			printf("Password shouldn't include space\n");
			resetColor();
			break;
		}
		//using fgets() will add '\n' at the end of user input.
		//replace the '\n' with '\0' otherwise is consider unmatch with file's data / disrupt the data structure.
		if (userInput[i] == '\n') {
			userInput[i] = '\0';
		}
	}
}

void staffEmailValidation(char userInput[], int* checkValidation) {
	int checkAliasSign = 0;
	if (strlen(userInput) < 5) {
		yellow();
		printf("Illogical input\n");
		resetColor();
		++* checkValidation;
		return;
	}
	for (int i = 0; i < strlen(userInput); i++) {
		if (userInput[i] == '@') {
			++checkAliasSign;
		}
		//using fgets() will add '\n' at the end of user input.
		//replace the '\n' with '\0' otherwise is consider unmatch with file's data / disrupt the data structure.
		if (userInput[i] == '\n') {
			userInput[i] = '\0';
		}


	}
	if (checkAliasSign != 1) {
		++* checkValidation;
		yellow();
		printf("One and only one @ (Alias sign) must be included for an email\n");
		resetColor();
	}

	//46
	if (userInput[strlen(userInput) - 4] != '.' && userInput[strlen(userInput) - 3] != 'c' && userInput[strlen(userInput) - 2] != 'o' && userInput[strlen(userInput) - 1] != 'm') {
		++* checkValidation;
		yellow();
		printf("Email must end with .com\n");
		resetColor();
	}
}

void autoStaffIDGenerate(char input[], int currentStaffCount) {
	currentStaffCount + 1;
	if (currentStaffCount < 10) {
		sprintf(input, "ST000%d", currentStaffCount);
		return;
	}
	else if (currentStaffCount >= 10 && currentStaffCount < 100) {
		sprintf(input, "ST00%d", currentStaffCount);
		return;
	}
	else if (currentStaffCount >= 100 && currentStaffCount < 1000) {
		sprintf(input, "ST0%d", currentStaffCount);
		return;
	}
	else {
		sprintf(input, "ST%d", currentStaffCount);
		return;
	}
}

void positionList(char position[]) {
	int positionSelection;
	do {
		printf("1. CEO\n2. COO\n3. IT Manager\n4. IT Technician");
		printf("\n5. HR Manager\n6. HR Personnel\n7. Customer Service\n");
		printf("Key in the number assigned to the position: ");
		rewind(stdin);
		scanf("%d", &positionSelection);
	} while (!(positionSelection >= 0 && positionSelection <= 7));
	switch (positionSelection) {
	case 0:
		red();
		printf("\nModification cancelled\a\n");
		resetColor();
		return;

	case 1:
		strcpy(position, "CEO");
		break;

	case 2:
		strcpy(position, "COO");
		break;

	case 3:
		strcpy(position, "IT Manager");
		break;

	case 4:
		strcpy(position, "IT Technician");
		break;

	case 5:
		strcpy(position, "HR Manager");
		break;

	case 6:
		strcpy(position, "HR Personnel");
		break;

	case 7:
		strcpy(position, "Customer Service");
		break;
	}
}

void checkDuplicatedEmail(char input[], int* checkValidation) {
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
		if (strcmp(staff.email, input) == 0) {
			++* checkValidation;
			yellow();
			printf("Duplicate email detected. Email must be unique.");
			resetColor();
			break;
		}
	}
	fclose(readStaff);
}

void checkIDExist(char userInput[], int* checkValidation) {
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
		if (strcmp(staff.ID, userInput) == 0) {
			break;
		}
		else if (feof(readStaff) && strcmp(staff.ID, userInput) != 0) {
			++* checkValidation;
			yellow();
			printf("Invalid staff ID\n");
			resetColor();
		}
	}
	fclose(readStaff);
}

void checkIDAlreadyPendingLeave(char input[], int* checkValidation) {
	FILE* readRec;
	Leave leave;
	readRec = fopen("leaveApplication.txt", "r");
	if (readRec == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	while (!feof(readRec)) {
		fscanf(readRec, "%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^|]|%[^\n]\n", leave.ID, &leave.totalDay, &leave.sDay, &leave.sMonth, &leave.sYear, &leave.eDay, &leave.eMonth, &leave.eYear, leave.reason, leave.leaveStatus);
		if (strcmp(input, leave.ID) == 0) {
			*checkValidation = -1;
			return;
		}
	}
}

void validateDate(int currentDay, int currentMonth, int currentYear, int* checkValidation) {
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
					printf("The following staff is not eligible to apply any more leave.\n");
					resetColor();
					system("pause");
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

void updateLeaveStatus(int response, char status[]) {
	if (response == 1) {
		strcpy(status, "Approved");
		return;
	}
	else if (response == 2) {
		strcpy(status, "Rejected");
		return;
	}
	else {
		yellow();
		printf("Invalid response\n");
		resetColor();
		return;
	}
}
//upon leave being approved, used on review leave

void updateALCount(char ID[], int totalDay) {
	FILE* readStaff, * writeStaff;
	Staff staff, modifyStaff;
	readStaff = fopen("staff.txt", "r");
	writeStaff = fopen("tempStaff.txt", "w");
	if (readStaff == NULL || writeStaff == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	while (!feof(readStaff)) {
		fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
		modifyStaff = staff;
		if (strcmp(modifyStaff.ID, ID) == 0) {
			modifyStaff.annualLeaveCount = modifyStaff.annualLeaveCount - totalDay;
			fprintf(writeStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", modifyStaff.ID, modifyStaff.name, modifyStaff.pw.password, modifyStaff.pw.pwResetPin, modifyStaff.position, modifyStaff.email, modifyStaff.status, modifyStaff.annualLeaveCount);
		}
		else {
			fprintf(writeStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", modifyStaff.ID, modifyStaff.name, modifyStaff.pw.password, modifyStaff.pw.pwResetPin, modifyStaff.position, modifyStaff.email, modifyStaff.status, modifyStaff.annualLeaveCount);
		}
	}
	fclose(readStaff);
	fclose(writeStaff);
	remove("staff.txt");
	rename("tempStaff.txt", "staff.txt");
}
//used on review leave
void checkLeaveApplicationEmpty(char input[], int* checkEmpty) {
	//since the first data entry is leave applicant ID, if the first data entry doesn't meet the data requirement
	//the file is most probably empty
	for (int i = 0; i < 2; i++) {
		if (input[0] == 's' || input[1] == 't') {
			input[0] = toupper(input[0]);
			input[1] = toupper(input[1]);
		}

		else if (input[0] != 'S' || input[1] != 'T') {
			++* checkEmpty;
			break;
		}
		else
			break;
	}

}
//end of leave end date + 1, this function is to ensure after the date +1 remain logical date
void validateEndOfLeaveDate(int* currentDay, int* currentMonth, int* currentYear) {
	*currentDay += 1;
	if (*currentMonth == 1 || *currentMonth == 3 || *currentMonth == 5 || *currentMonth == 7 || *currentMonth == 8 || *currentMonth == 10 || *currentMonth == 12) {
		if (*currentDay > 31) {
			*currentDay -= 31;
			++* currentMonth;
		}
	}
	if (*currentMonth == 4 || *currentMonth == 6 || *currentMonth == 9 || *currentMonth == 11) {
		if (*currentDay > 30) {
			*currentDay -= 30;
			++* currentMonth;
		}

	}

	if (*currentMonth == 2) {
		if (((*currentYear % 4 == 0) && ((*currentYear % 400 == 0) || (*currentYear % 100 != 0)))) {
			if (*currentDay > 29) {
				*currentDay -= 29;
				++* currentMonth;
			}
		}
		else {
			if (*currentDay > 28) {
				*currentDay -= 28;
				++* currentMonth;
			}

		}

	}
	if (*currentMonth > 12) {
		*currentMonth -= 12;
		++* currentYear;
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

//if the sysdate match with the leave date applied, turn the staff status to on leave or back to active
//this function must be run first whenever programme started.
void updateStaffLeaveStatus() {
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysYear = time.tm_year + 1900;
	int sysDay = time.tm_mday;
	FILE* readStaff, * readApprovedRec, * writeStaff, * writeApprovedRec;
	Staff staff, modifyStaff[100];
	Leave approvedLeave[50], tempRec;

	int totalLeaveRecord = countTotalLeaveRecord(); int totalStaffRecord = countTotalStaffRecord();
	readApprovedRec = fopen("leaveApproved.txt", "r");
	readStaff = fopen("staff.txt", "r");
	writeStaff = fopen("tempStaff.txt", "w");
	writeApprovedRec = fopen("tempLeaveApproved.txt", "w");
	if (readStaff == NULL || writeStaff == NULL || readApprovedRec == NULL || writeApprovedRec == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}

	for (int i = 0; i < totalStaffRecord; i++) {
		if (readStaff != EOF) {
			fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", modifyStaff[i].ID, modifyStaff[i].name, modifyStaff[i].pw.password, &modifyStaff[i].pw.pwResetPin, modifyStaff[i].position, modifyStaff[i].email, modifyStaff[i].status, &modifyStaff[i].annualLeaveCount);
		}
		else
			break;
	}
	for (int i = 0; i < totalLeaveRecord; i++) {
		if (readApprovedRec != EOF)
			fscanf(readApprovedRec, "%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^|]|%[^\n]\n", approvedLeave[i].ID, &approvedLeave[i].totalDay, &approvedLeave[i].sDay, &approvedLeave[i].sMonth, &approvedLeave[i].sYear, &approvedLeave[i].eDay, &approvedLeave[i].eMonth, &approvedLeave[i].eYear, approvedLeave[i].reason, approvedLeave[i].leaveStatus);

		else
			break;
	}
	/*
	for (int i = 0; i < totalStaffRecord; i++) {
		printf("%s\n", modifyStaff[i].status);
		printf("%d\n",modifyStaff[i].annualLeaveCount);
		system("pause");
	}*/

	int endOfLeave;
	for (int i = 0; i < totalStaffRecord; i++) {
		endOfLeave = 0;
		for (int j = 0; j < totalLeaveRecord; j++) {
			if (strcmp(modifyStaff[i].ID, approvedLeave[j].ID) == 0) {
				tempRec = approvedLeave[j];

				printf("%d-%d-%d\n", tempRec.eDay, tempRec.eMonth, tempRec.eYear);
				if (approvedLeave[j].sDay == sysDay && approvedLeave[j].sMonth == sysMonth && approvedLeave[j].sYear == sysYear && strcmp(modifyStaff[i].status, "Active") == 0) {
					strcpy(modifyStaff[i].status, "On leave");
				}
				//insert the temp record to check if end day + 1 is match with the sysDay, if match then change status from on leave to active.
				//must use temp record to test the +1 since the original data shouldn't be twisted
				validateEndOfLeaveDate(&tempRec.eDay, &tempRec.eMonth, &tempRec.eYear);
				printf("%d-%d-%d\n", tempRec.eDay, tempRec.eMonth, tempRec.eYear);
				printf("%d-%d-%d\n", approvedLeave[j].eDay, approvedLeave[j].eMonth, approvedLeave[j].eYear);
				system("pause");
				if (tempRec.eDay == sysDay && tempRec.eMonth == sysMonth && tempRec.eYear == sysYear && strcmp(modifyStaff[i].status, "On leave") == 0) {
					++endOfLeave;
					strcpy(modifyStaff[i].status, "Active");
				}
				if (approvedLeave[j].eDay < sysDay && approvedLeave[j].eMonth == sysMonth && approvedLeave[j].eYear == sysYear) {
					++endOfLeave;
				}

				if (endOfLeave != 0) {
					continue;
				}
				else {
					//tempRec = approvedLeave[j];
					//fprintf(writeApprovedRec, "%s|%d|%d-%d-%d|%d-%d-%d|%s|%s\n", tempRec.ID, tempRec.totalDay, tempRec.sDay, tempRec.sMonth, tempRec.sYear, tempRec.eDay, tempRec.eMonth, tempRec.eYear, tempRec.reason, tempRec.leaveStatus);
					fprintf(writeApprovedRec, "%s|%d|%d-%d-%d|%d-%d-%d|%s|%s\n", approvedLeave[j].ID, approvedLeave[j].totalDay, approvedLeave[j].sDay, approvedLeave[j].sMonth, approvedLeave[j].sYear, approvedLeave[j].eDay, approvedLeave[j].eMonth, approvedLeave[j].eYear, approvedLeave[j].reason, approvedLeave[j].leaveStatus);
				}
			}
		}
	}
	printf("-------------------------\n");
	for (int i = 0; i < totalStaffRecord; i++) {
		printf("%d %s\n", i + 1, modifyStaff[i].status);
		fprintf(writeStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", modifyStaff[i].ID, modifyStaff[i].name, modifyStaff[i].pw.password, modifyStaff[i].pw.pwResetPin, modifyStaff[i].position, modifyStaff[i].email, modifyStaff[i].status, modifyStaff[i].annualLeaveCount);

	}

	fclose(readApprovedRec);
	fclose(readStaff);
	fclose(writeStaff);
	fclose(writeApprovedRec);
	remove("staff.txt");
	rename("tempStaff.txt", "staff.txt");
	remove("leaveApproved.txt");
	rename("tempLeaveApproved.txt", "leaveApproved.txt");
}
//helper function to updateStaffLeaveStatus
int countTotalLeaveRecord() {
	int totalRec = 0;
	Leave approvedLeave;
	FILE* readApprovedRec;
	readApprovedRec = fopen("leaveApproved.txt", "r");
	if (readApprovedRec == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	while (!feof(readApprovedRec)) {
		fscanf(readApprovedRec, "%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^|]|%[^\n]\n", approvedLeave.ID, &approvedLeave.totalDay, &approvedLeave.sDay, &approvedLeave.sMonth, &approvedLeave.sYear, &approvedLeave.eDay, &approvedLeave.eMonth, &approvedLeave.eYear, approvedLeave.reason, approvedLeave.leaveStatus);
		++totalRec;
	}
	fclose(readApprovedRec);
	return totalRec;
}
//helper function to updateStaffLeaveStatus
int countTotalStaffRecord() {
	int totalRec = 0;
	Staff staff;
	FILE* readStaff;
	readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	while (!feof(readStaff)) {
		fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
		++totalRec;
	}
	fclose(readStaff);
	return totalRec;
}