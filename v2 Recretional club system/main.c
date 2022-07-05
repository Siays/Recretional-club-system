#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "staff.c"
#include "facility.c"
#include "member.c"
#include "booking.c"
#pragma warning(disable: 4996)
typedef struct {
	char ID[7], position[21], email[31], loginStatus[8], date[26];
}loginSucceed;

void loginRequest(char loginIDHolder[]);
void trackNoOfAttempts(int noOfAttempts, int validID);
void recordSucceedLogin(Staff staff[], int matchArrayPos);
//helper functions
void verifyLoginIDStaffStatus(char status[], int* checkStaffStatus);
void validatePwResetPin(char input[], int* checkValidation);
//end of helper functions

// add member module
void main() {
	bookingModuleMain();
	/*
	char loginIDHolder[7];
	loginRequest(loginIDHolder);

	int menuSelection;

	do {
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\tMain Module\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit the Module\n");
		printf("\t\t\t1. Staff Module\n");
		printf("\t\t\t2. Facility Module\n");
		printf("\t\t\t3. Member Module\n");
		printf("\t\t\t4. Bookinwg Module\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\tYour selection(numeric input only): ");
		rewind(stdin);//incase user input non-numeric input. the system will go into infinite loop.
		scanf("%d", &menuSelection);

		switch (menuSelection) {
		case 0:
			printf("\nThank you for using our system.\n");
			break;

		case 1:
			staffModuleMain(loginIDHolder);
			break;
		
		case 2:
			facilityModuleMain(loginIDHolder);
			break;

		case 3:
			memberModuleMain(loginIDHolder);
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
	} while (menuSelection != 0); */
}

void loginRequest(char loginIDHolder[]) {
	updateStaffLeaveStatus();
	updateMemStatus();
	int staffRecordCount = countTotalStaffRecord();
	int matchArrayPos;
	FILE* readStaff;
	Staff staff[100];
	readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	//paste the data into staff array
	for (int i = 0; i < staffRecordCount; i++) {
		if (!feof(readStaff))
			fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff[i].ID, staff[i].name, staff[i].pw.password, &staff[i].pw.pwResetPin, staff[i].position, staff[i].email, staff[i].status, &staff[i].annualLeaveCount);

		else
			break;
	}
	fclose(readStaff);
	
	char loginID[7];
	int checkValidation, checkStaffStatus, noOfAttempt = 0, validLogin = 0; int reenterLoginIDReq, validID = 0;

	do {
		do {
			do {
				reenterLoginIDReq = 0;
				validID = 0;
				system("cls");
				trackNoOfAttempts(noOfAttempt, validID);
				red();
				printf("\tIf you has reached 3 fault attempts with a valid ID, you will be forced to reset password\n");
				printf("\tIf you has reached 5 fault attempts, the system will be forced to shut down\n");
				resetColor();
				printf("\t\t________________________________________________\n");
				printf("\t        \t\tLogin required\t\t\t \n");
				printf("\t\t________________________________________________\n");
				checkValidation = 0;
				checkStaffStatus = 0; //will ++ the variable if user trying to login using staff status that is on leave or resigned
				printf("\t\tEnter your staff ID: ");
				rewind(stdin);
				fgets(loginID, 7, stdin);
				staffIDValidation(loginID, &checkValidation); // check ID input according to format
				if (checkValidation == 0) {
					checkIDExist(loginID, &checkValidation); // check ID exisit in record
				}
				if (checkValidation == 0) {// if ID is exist, then pin the position of array the data found.
					
					for (int i = 0; i < staffRecordCount; i++) {
						if (strcmp(staff[i].ID, loginID) == 0) {
							matchArrayPos = i;
						}
					}
					verifyLoginIDStaffStatus(staff[matchArrayPos].status, &checkStaffStatus);
					if(checkStaffStatus == 0){
						++validID;
					}
				}
				if (checkValidation != 0 || checkStaffStatus != 0) {
					++noOfAttempt;
					trackNoOfAttempts(noOfAttempt,validID);
				}
				if (noOfAttempt == 3 && validID > 0)
				{break; }
			} while (checkValidation != 0 || checkStaffStatus != 0);

			char loginPW[21], ch;
			int resetVariable = 0, pwLengthCount = 0, invalidPW;
			do {
				invalidPW = 0;
				checkValidation = 0;
				printf("\t\tEnter you password: ");
				rewind(stdin);
				//this loop is to display the password input in asterick form
				while (1) {
					if (resetVariable > 0) { //if user come back to this loop
						pwLengthCount = 0; //reset the input as empty, this condition occur when user last attempt failed, which is line ch == 13
					}
					resetVariable = 0;
					rewind(stdin);
					ch = getch();
					if (ch == 13) {//if user key in "enter"
						loginPW[pwLengthCount] = '\0'; //assign end of string at where
						//user key in "enter"
						++resetVariable;//once user confirm their input , assign the variable to >0
						break;
					}
					else if (ch == 8) {//if user key in "backspace"
						if (pwLengthCount > 0) {//and the length of password is >0
							pwLengthCount--;// -- the length
							printf("\b \b");//delete the previos input
						}
						else
							continue;//else of the length is 0, skip user backspace command
					}
					else if (ch == 9 || ch == 32) //if user input space / tab
						continue;//ignore the line

					else if (pwLengthCount == 20) {
						red();//if user input hit the maximum password length, which is 20
						printf("\n\t\tMaximum password range!\n");
						printf("\t\tRe-enter password\n");
						resetColor();//reset the password legth= 0; OR re-enter password
						pwLengthCount = 0;
					}

					else {
						loginPW[pwLengthCount] = ch;//otherwise then assign whatever use
						pwLengthCount++;//input into password, and ++ the length
						printf("*");//output the password in asterick
					}

				}
				staffPWValidation(loginPW, &checkValidation); // check if password input is in valid form
				//if password is in valid form, check if the password input match the record password
				if (checkValidation == 0) {
					if (strcmp(staff[matchArrayPos].pw.password, loginPW) != 0) {
						++invalidPW;
					}
				}
				if (invalidPW > 0 && noOfAttempt < 3) {
					char confirm;
					yellow();
					printf("\n\n\t\tInvalid password inputted\n\n");
					resetColor();
					++noOfAttempt;
					++checkValidation;
					//if no of attempt more than 3, then directly go to reset password instead of execute this block of codes
					//current issue: repeat direct to reset password msg 2 times.
					if(noOfAttempt < 3){
					printf("\tAre you sure %s is your login ID?\n", staff[matchArrayPos].ID);
					printf("\tNumber of fault attempt(s) will remain if you choose to re-enter ID\n");
					printf("\tPress 'Y' if it is the correct login ID[Y=yes, other key to re-enter ID]: ");
					confirm = getch();
					confirm = toupper(confirm);
					printf("\n\n");
					if (confirm != 'Y') {
						++reenterLoginIDReq;
						break;
					}
					}
				}
				if (noOfAttempt >= 3 && validID > 0)
				{break; }
			} while (checkValidation != 0);
		} while (reenterLoginIDReq > 0);
		//if user enter all the information correctly
		if (checkValidation == 0) {
			++validLogin; //the user then can break the do while loop
		}
		if (validLogin > 0) {
			break;
		}
	} while (noOfAttempt < 3);
	
	if (noOfAttempt >= 3 && validID > 0) {	
		int resetVariable = 0, pwLengthCount = 0, invalidPW;
		char passwordResetPin[7], ch; //get the password in char first so able to display user input in asterick form
		int pwResetPin;
		do{	
		system("cls");	
		trackNoOfAttempts(noOfAttempt, validID);
		printf("\tIf you has reached 5 fault attempts, the system will be forced to shut down\n");
		checkValidation = 0;
		printf("\t\t________________________________________________\n");
		printf("\a\n\t\t\t\tReset password\n");
		printf("\t\t________________________________________________\n");
		printf("\t\tLogin ID: %s\n", staff[matchArrayPos].ID);
		printf("\t\tEnter you password reset pin(only digit and maximum length of 6): ");
		rewind(stdin);
		while (1) {
			if (resetVariable > 0) { //if user come back to this loop
				pwLengthCount = 0; //reset the input as empty, this condition occur when user last attempt failed, which is line ch == 13
			}
			resetVariable = 0;
			rewind(stdin);
			ch = getch();
			if (ch == 13) {//if user key in "enter"
				passwordResetPin[pwLengthCount] = '\0'; //assign end of string at where
				//user key in "enter"
				++resetVariable;//once user confirm their input , assign the variable to >0
				break;
			}
			else if (ch == 8) {//if user key in "backspace"
				if (pwLengthCount > 0) {//and the length of password is >0
					pwLengthCount--;// -- the length
					printf("\b \b");//delete the previos input
				}
				else
					continue;//else of the length is 0, skip user backspace command
			}
			else if (ch == 9 || ch == 32) //if user input space / tab
				continue;//ignore the line

			else if (pwLengthCount == 20) {
				red();//if user input hit the maximum password length, which is 20
				printf("\n\t\tMaximum password range!\n");
				printf("\t\tRe-enter password\n");
				resetColor();//reset the password legth= 0; OR re-enter password
				pwLengthCount = 0;
			}

			else {
				passwordResetPin[pwLengthCount] = ch;//otherwise then assign whatever use
				pwLengthCount++;//input into password, and ++ the length
				printf("*");//output the password in asterick
			}
		}
		validatePwResetPin(passwordResetPin, &checkValidation);
		if (checkValidation != 0) {
			++noOfAttempt;
		}
		else{
			pwResetPin = atoi(passwordResetPin);
				if (staff[matchArrayPos].pw.pwResetPin == pwResetPin) {
					break;
				}
				else {
					++noOfAttempt;
					++checkValidation;
				}
			}
		} while (checkValidation > 0);
	
		char tempPassword[21], confirm;;
		
		do{
		system("cls");
		checkValidation = 0;
		printf("Enter your new password [maximum length of 20]: ");
		rewind(stdin);
		fgets(tempPassword,21,stdin); 	
		staffPWValidation(tempPassword, &checkValidation);
		printf("\nNew Password: %s\n", tempPassword);
		printf("Confirm the new password? [Y=yes, any other key to exit]: ");
		rewind(stdin);
		confirm = getch();
		confirm = toupper(confirm);
		if(confirm != 'Y'){
			++checkValidation;
		}
		} while (checkValidation != 0);
		
		strcpy(staff[matchArrayPos].pw.password, tempPassword);
		FILE* writeStaff;
		writeStaff = fopen("staff.txt","w");
		for (int i = 0; i < staffRecordCount; i++) {
			fprintf(writeStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", staff[i].ID, staff[i].name, staff[i].pw.password, staff[i].pw.pwResetPin, staff[i].position, staff[i].email, staff[i].status, staff[i].annualLeaveCount);
		}
		fclose(writeStaff);
	}

	recordSucceedLogin(staff, matchArrayPos);
	strcpy(loginIDHolder, loginID);
}

void trackNoOfAttempts(int noOfAttempts, int validID) {
	if (noOfAttempts == 5) {
		red();
		printf("\tCurrently fault attempt count(s): %d \n", noOfAttempts);
		printf("\a\tYou are most likely an unauthorized personnel.\n");
		printf("\tSystem will be forced to shut down\n");
		resetColor();
		system("pause");
		exit(-1);
	}
	
	if ((noOfAttempts > 0 && noOfAttempts < 3) || noOfAttempts == 4) {
		red();
		printf("\tCurrently fault attempt count(s): %d \n", noOfAttempts);
		resetColor();
		return;
	}
	
	if (noOfAttempts >= 3 && validID != 1) {
		red();
		printf("\tCurrently fault attempt count(s): %d \n", noOfAttempts);
		resetColor();
		return;
	}

	if (noOfAttempts >= 3 && validID == 1) {
		red();
		printf("\tCurrently fault attempt count(s): %d \n", noOfAttempts);
		printf("\a\tYou will be directed to password reset\n");
		resetColor();
		system("pause");
		return;
	}
}

void recordSucceedLogin(Staff staff[], int matchArrayPos) {
	loginSucceed loginRec;
	time_t t;
	time(&t);
	FILE* writeRecSucceedLogin;
	writeRecSucceedLogin = fopen("staffLoginRecord.txt","a");
	if (writeRecSucceedLogin == NULL) {
		yellow();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	
	strcpy(loginRec.ID, staff[matchArrayPos].ID);
	strcpy(loginRec.position, staff[matchArrayPos].position);
	strcpy(loginRec.email, staff[matchArrayPos].email);
	strcpy(loginRec.loginStatus,"Succeed");
	strcpy(loginRec.date, ctime(&t));

	fprintf(writeRecSucceedLogin, "%s|%s|%s|%s|%s", loginRec.ID, loginRec.position, loginRec.email, loginRec.loginStatus, loginRec.date);
	fclose(writeRecSucceedLogin);
}

//helper functions
void verifyLoginIDStaffStatus(char status[], int* checkStaffStatus) {
	if (strcmp(status, "On leave") == 0) {
		++* checkStaffStatus;
		yellow();
		printf("You are trying to login an ID that is currently on leave.\a\n");
		resetColor();
		system("pause");
		return;
	}
	if (strcmp(status, "Resigned") == 0) {
		++* checkStaffStatus;
		yellow();
		printf("You are trying to login an ID that had resigned.\a\n");
		resetColor();
		system("pause");
		return;
	}
}

void validatePwResetPin(char input[], int*checkValidation) {
	if (strlen(input) < 1) {
		++*checkValidation;
		red();
		printf("\nCannot be empty\n");
		resetColor();
		system("pause");
		return;
	}

	if (strlen(input) > 6) {
		++*checkValidation;
		red();
		printf("\nMaximum length is 6\n");
		resetColor();
		system("pause");
		return;
	}
	if (input[0] == '0') {
		++*checkValidation;
		red();
		printf("\nLeading 0 is denied\n");
		resetColor();
		system("pause");
		return;
	}

	for (int i = 0; i < strlen(input); i++) {
		if (!(input[i] >= '0' && input[i] <= '9')) {
			++*checkValidation;
			red();
			printf("\nOnly numeric input is allowed\n");
			resetColor();
			system("pause");
			return;
		}
	
	}
}
