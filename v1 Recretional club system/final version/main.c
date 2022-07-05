#include "allStruct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include "Staff.h"
#include "Facility.h"
#include "Member.h"
#include "Booking.h"
#include "fumodule.h"
#pragma warning (disable :4996)

void loginRequest(char loginIDHolder[]);
void loginFailedRecord(Staff staff[], int staffCount, char loginID[], char password[], int invalidCount);
void resetFailedRecord(Staff staff[], int staffCount, char loginID[], int invRecoveryCount);
void immediateResetPW(Staff staff[], int staffCount, char loginID[]);
void heading();


void main() {
	char loginIDHolder[10];
	loginRequest(loginIDHolder);

	char word[] = "Welcome to Mental Club Maintanence System.";
	int a = 0;
	printf("\n\n");


	printf("\n\n%62s", "");

	for (int i = 0; i < 43; i++)
	{
		printf("%c", word[a++]);
		_sleep(40);

	}

	_sleep(1000);
	
	int selection;
	do{
	system("cls");
	printf("\033[0;30m");
	printf("\033[47m");
	heading();
	resetColor();

	
	printf("\n\n\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                    MAIN MENU\n");
	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                   0. Exit system\n");
	printf("\t\t\t\t                   1. Staff Module\n");
	printf("\t\t\t\t                   2. Facility Module\n");
	printf("\t\t\t\t                   3. Member Module\n");
	printf("\t\t\t\t                   4. Booking Module\n");
	printf("\t\t\t\t                   5. Faculity Usage Module\n");
	printf("\t\t\t\t---------------------------------------------------------\n");

	printf("\nSelection --> ");
	rewind(stdin);  //rewind here is needed in case the user input character
	// for the first time, the system will go into infinite loop
	scanf("%d",&selection);

	switch (selection) {
	case 0:
		printf("\nThank you for using our system.\n");
		break;
	
	case 1:
		system("cls");
		StaffModule();
		break;

	case 2:
		system("cls");
		FaciModule();
		break;
	case 3:
		system("cls");
		memberSystem();
		break;

	case 4:
		system("cls");
		bookingSystem();
		break;

	case 5:
		FUmodule(loginIDHolder);
		break;

	default:
		yellow();
		printf("\a\nInvalid selection.\n");
		printf("Please select as shown.\n");
		resetColor();
		system("pause"); //pause system for short while to allow
		//user to read the error message
		
	}
	} while (selection != 0);
	system("pause");
}


void heading() {

	printf("                _    _        _                                   _             ___  ___              _          _      _____  _         _     \n");
	printf("               | |  | |      | |                                 | |            |  \\/  |             | |        | |    /  __ \\| |       | |    \n");
	printf("               | |  | |  ___ | |  ___  ___   _ __ ___    ___     | |_  ___      | .  . |  ___  _ __  | |_  __ _ | |    | /  \\/| | _   _ | |__  \n");
	printf("               | |/\\| | / _ \\| | / __|/ _ \\ | '_ ` _ \\  / _ \\    | __|/ _ \\     | |\\/| | / _ \\| '_ \\ | __|/ _` || |    | |    | || | | || '_ \\ \n");
	printf("               \\  /\\  /|  __/| || (__| (_) || | | | | ||  __/    | |_| (_) |    | |  | ||  __/| | | || |_| (_| || |    | \\__/\\| || |_| || |_) |\n");
	printf("                \\/  \\/  \\___||_| \\___|\\___/ |_| |_| |_| \\___|     \\__|\\___/     \\_|  |_/ \\___||_| |_| \\__|\\__,_||_|     \\____/|_| \\__,_||_.__/ \n");


	printf("              Mental club is a very famous club among teenagers. Is a club where gather a group of people that interest in playing some extreme game.\n");
	printf("                                                                      Join us now !!!\n");
	printf("                                                                   Let's have fun together !!!\n");

	
}

void loginRequest(char loginIDHolder[]) {

	Staff staff[100];
	FILE* readPtr;
	int staffCount = 0;
	readPtr = fopen("staff.txt", "r");
	if (readPtr == NULL) {
		printf("Error. Failed to open file.");
		return;
	}

	for (int i = 0; i < 100; i++) {
		if (!feof(readPtr)) {
			++staffCount;
			fscanf(readPtr, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%d|%s\n", staff[i].name, staff[i].contact,&staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, &staff[i].pwRecovery, staff[i].position, staff[i].email, &staff[i].age, staff[i].status);
		}

		else
			break;
	}

	fclose(readPtr);

	char loginID[10], password[21], resetPWconfirm;
	int invalidCount = 0, validCount = 0, invRecoveryCount = 0, noRcdFound = 0, resignedStaff = 0;
	int pwRecovery = 0;
	char newPW[21], pwRecoveryPin[7];
	char ch;
	int pwLengthCount = 0, exceedPwMaxLength = 0, resetVariable = 0;
	do {
		resignedStaff = 0; //allow user to keep track if the ID they input is resigned or active
		loginFailedRecord(staff, staffCount, loginID, password, invalidCount);
		printf("\t\t________________________________________________\n");
		printf("\t        \t\tLogin required\t\t\t \n");
		printf("\t\t________________________________________________\n");

			printf("\t\tEnter your staff ID: ");
			rewind(stdin);
			gets(loginID);
			printf("\t\tEnter your password: ");
			rewind(stdin);

			while(1){
				if (resetVariable > 0)
					strcpy(password,"");
				resetVariable = 0;
				rewind(stdin);
				ch = getch();
				if (ch == 13) {//if user key in "enter"
					password[pwLengthCount] = '\0'; //assign end of string at where
					//user key in "enter"
					++resetVariable;
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

				else if (pwLengthCount == 20){
					red();//if user input hit the maximum password length, which is 20
					printf("\n\t\tMaximum password range!\n");
					printf("\t\tRe-enter password\n");
					resetColor();//reset the password legth= 0; OR re-enter password
					pwLengthCount = 0;
				}

				else{
					password[pwLengthCount] = ch;//otherwise then assign whatever use
					pwLengthCount++;//input into password, and ++ the length
					printf("*");//output the password in asterick
				}
			
			}
			printf("%s", password);

		for (int i = 0; i < staffCount; i++) {
			if (strcmp(loginID, staff[i].ID) == 0 && strcmp(password, staff[i].password) == 0 && strcmp(staff[i].status,"Active") == 0) {
				// if user key in the correct ID and password break the do while loop
				++validCount;
				break;
			}

			else if (strcmp(loginID, staff[i].ID) == 0 && strcmp(password, staff[i].password) == 0 && strcmp(staff[i].status, "Resigned") == 0) {
				++resignedStaff;
				++invalidCount;
				break;
			}

			else if (strcmp(loginID, staff[i].ID) == 0 && strcmp(password, staff[i].password) != 0) {
				++invalidCount;
				break;
			}

			else if (i == staffCount - 1 && strcmp(loginID, staff[i].ID) != 0 && strcmp(password, staff[i].password) != 0) {
				//if user in wrong ID or password, then add invalid Count
				++invalidCount;
			}
		}
		if (resignedStaff > 0) {
			yellow();
			printf("\n\t\tThe ID you enter is a resigned staff.\n");
			resetColor();
		}

		if (invalidCount > 0 && validCount == 0) {
			//let the user to know how many time they have been keyying in the wrong information
			yellow();
			printf("\n\t\tInvalid ID or password.\n");
			printf("\t\tThis is your %d time of attempt\n\n", invalidCount);
			resetColor();
		}

		if (invalidCount > 3) {
			if (invalidCount == 4) {
				//once the invalid count up to 4 time, ask the user to reset password
				system("cls");
				yellow();
				printf("\t\tYou have been entering the wrong ID or wrong password for more than 3 times\n");
				printf("\t\tReset password?\n");
				printf("\t\tYes = Y: ");
				resetColor();
				rewind(stdin);
				scanf("%c", &resetPWconfirm);
				resetPWconfirm = toupper(resetPWconfirm);
			}

			if (resetPWconfirm == 'Y' || invalidCount == 5) {
				// if user select to reset password OR if user have input wrong ID / Wrong password
				// for the 5th time, enter password reset.
				char loginID2[10];
				//required user to key the ID they wish to reset password.
				system("cls");
				red();
				printf("\t\tThe system will exit if you faield to key in\n");
				printf("\t\tcorrect information for more than 3 times.\n");
				resetColor();
				//if user failed to reset password for 3 time,
				// the system will directly shut down.
				do {
					resignedStaff = 0;
					noRcdFound = 0;// this is to let user know if they ID they input exisit or not
					printf("\n\n\t\t\t\tCurrent Count: %d\n", invRecoveryCount+1);//let user keep track current try count
					printf("\t\t________________________________________________\n");
					printf("\a\n\t\t\t\tReset password\n");
					printf("\t\t________________________________________________\n");
					printf("\t\tEnter you staff ID: ");
					rewind(stdin);
					gets(loginID2);
					for (int i = 0; i < staffCount; i++) {
						//if user input correct ID 
						if (strcmp(staff[i].ID, loginID2) == 0 && strcmp(staff[i].status,"Active") == 0) {
							printf("\t\tEnter your password recovery code: ");
							while (1) {
								rewind(stdin);
								ch = getch();
								if (ch == 13) {//if user key in "enter"
									pwRecoveryPin[pwLengthCount] = '\0'; //assign end of string at where
									//user key in "enter"
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
									//red();//if user input hit the maximum password length, which is 20
									printf("\n\t\tMaximum password range!\n");
									printf("\t\tRe-enter password\n");
									//resetColor();//reset the password legth= 0; OR re-enter password
									pwLengthCount = 0;
								}

								else {
									pwRecoveryPin[pwLengthCount] = ch;//otherwise then assign whatever use
									pwLengthCount++;//input into password, and ++ the length
									printf("*");//output the password in asterick
								}

							}
							pwRecovery = atoi(pwRecoveryPin); // convert a "digit" string to int.
							
							printf("%d", pwRecovery);
							//if user input correct ID and correct reset password pin
							if (pwRecovery == staff[i].pwRecovery) {
								printf("\t\tYou will be directed to reset password.\n");
								system("pause");
								invRecoveryCount = 0; // reset to 0 , so the system will not exit
								invalidCount = 0; // reset to 0 so user will be able to exit the reset password loop 
								strcpy(loginID, loginID2);
								immediateResetPW(staff, staffCount, loginID2);
								++validCount;
								break;
							}
							//else if user input correct ID but incorrect reset password pin
							else if (pwRecovery != staff[i].pwRecovery){
								++invRecoveryCount;		// continue to ++the count, which if 4 time
								//the system will close down directly.
								yellow();
								printf("\t\tInvalid recovery code.\n");
								resetColor();
								break;
							}
							
						}
						//else the loop go until last record, yet unable to find the ID as user input
						// means user input an inexist ID

						else if (strcmp(staff[i].ID, loginID2) == 0 && strcmp(staff[i].status, "Resigned") == 0) {
							++invRecoveryCount;
							++resignedStaff;
							break;
						}

						else if ((i == staffCount - 1) && strcmp(staff[i].ID, loginID2) != 0) {
							++noRcdFound;//prompt user incorrect ID
							++invRecoveryCount;// plus the count
						}
					}
					//insert failed to reset password record
					resetFailedRecord(staff, staffCount, loginID2, pwRecovery, invRecoveryCount); 

					if (resignedStaff > 0) {
						yellow();
						printf("\t\tThe ID you enter is a resigned staff.\n");
						resetColor();
					}
					
					if (noRcdFound > 0) {
						yellow();
						printf("\t\tInvalid staff ID.\n");
						resetColor();
					}

					//once the count accumulate up to 4 times
					if (invRecoveryCount == 4) {
						yellow();
						printf("\n\t\tYou are most likely an unauthorized personnel.\n");
						printf("\t\tSystem is closing.\n\n");
						resetColor();
						system("pause"); //allow the user to read the messaage
						exit(0);//exit system directly if too much false attempt
					}

					if (invRecoveryCount == 0) {
						//if the user sucessfully input the correct ID and correct reset password PIN
						//then reset the user selected ID password to 1234
						FILE* writePtr;
						writePtr = fopen("staff.txt", "w");

						for (int i = 0; i < staffCount; i++) {
							fprintf(writePtr, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age,staff[i].status);
						}
						
						fclose(writePtr);
						break;
					}
							

				} while (invRecoveryCount < 5);

			}
			else {
				//else if during invalid count == 4 time, and user refuse to reset password,
				//inform user if they wrong one more time, will auto be direct to password reset.
				red();
				printf("\n\t\tYou will be directed to reset password if you insert \n\t\ta wrong password or ID once again.\n");
				resetColor();
			}


		}
		//upon correct ID and correct password inputted,
		//break the loop
	} while (validCount == 0);

	strcpy(loginIDHolder,loginID); //this is to be used in facility usage record

	//the login will be recorded once login success.
	StaffLoginRec loginRec;
	time_t t;
	time(&t);
	strcpy(loginRec.date, ctime(&t));
	FILE* appendPtr;
	appendPtr = fopen("staffLoginRecord.txt", "a");
	if (appendPtr == NULL) {
		printf("Error. Failed to open file.");
		return;
	}
	for (int i = 0; i < staffCount; i++) {
		if (strcmp(loginID, staff[i].ID) == 0) {
			strcpy(loginRec.ID, staff[i].ID);
			strcpy(loginRec.name, staff[i].name);
			strcpy(loginRec.position, staff[i].position);
			strcpy(loginRec.status, "Login Successed");
			fprintf(appendPtr, "%s|%s|%s|%s|%s", loginRec.ID, loginRec.name, loginRec.position,loginRec.status, loginRec.date);
		}
		else
			continue;
	}
	fclose(appendPtr);

	
		
	
}

void loginFailedRecord(Staff staff[], int staffCount, char loginID[], char password[], int invalidCount) {

	StaffLoginRec loginRec;
	time_t t; // record the time
	time(&t);
	strcpy(loginRec.date, ctime(&t));
	FILE* appendPtr;
	appendPtr = fopen("staffFailedLoginRecord.txt", "a");
	if (appendPtr == NULL) {
		printf("Error. Failed to open file.");
		return;
	}


	if (invalidCount > 0 && invalidCount <= 5) {
		//when user key in the wrong ID or password
		// record it. Once it up to 5 time, then will go into reset password
		for (int i = 0; i < staffCount; i++) {
			if (strcmp(loginID, staff[i].ID) == 0 && strcmp(password,staff[i].password) != 0) {
				strcpy(loginRec.ID, staff[i].ID);
				strcpy(loginRec.name, staff[i].name);
				strcpy(loginRec.position, staff[i].position);
				strcpy(loginRec.status, "Login Failed");
				fprintf(appendPtr, "%s|%s|%s|%s|%s", loginRec.ID, loginRec.name, loginRec.position,loginRec.status, loginRec.date);
				break;
			}
			else if ((i == staffCount - 1) && strcmp(loginID, staff[i].ID) != 0) {
				strcpy(loginRec.ID, "Invalid ID");
				strcpy(loginRec.name, "NULL");
				strcpy(loginRec.position, "NULL");
				strcpy(loginRec.status, "Login Failed");
				fprintf(appendPtr, "%s|%s|%s|%s|%s", loginRec.ID, loginRec.name, loginRec.position, loginRec.status, loginRec.date);
				break;
			}

		}
		}


		fclose(appendPtr);
	}

void resetFailedRecord(Staff staff[], int staffCount, char loginID2[], int resetPWCode, int invRecoveryCount) {
	//record password failed upon user failed to key in the correct ID and correct PIN
	StaffLoginRec loginRec;
	time_t t;
	time(&t);
	strcpy(loginRec.date, ctime(&t));
	FILE* appendPtr;
	appendPtr = fopen("staffResetFailedRecord.txt", "a");
	if (appendPtr == NULL) {
		printf("Error. Failed to open file.");
		return;
	}

	if (invRecoveryCount < 5) {
		for (int i = 0; i < staffCount; i++) {
			if (strcmp(loginID2, staff[i].ID) == 0 && resetPWCode != staff[i].pwRecovery) {
				strcpy(loginRec.ID, staff[i].ID);
				strcpy(loginRec.name, staff[i].name);
				strcpy(loginRec.position, staff[i].position);
				if(invRecoveryCount < 4)
					strcpy(loginRec.status, "Reset Failed");
				else//once the count up to 5 , system will exit directly
					strcpy(loginRec.status, "System Exited");
				fprintf(appendPtr, "%s|%s|%s|%s|%s", loginRec.ID, loginRec.name, loginRec.position, loginRec.status, loginRec.date );
				break;
			}
			else if ((i == staffCount - 1) && strcmp(loginID2, staff[i].ID) != 0) {
				strcpy(loginRec.ID, "Invalid ID");
				strcpy(loginRec.name, "NULL");
				strcpy(loginRec.position, "NULL");
				if (invRecoveryCount < 4)
					strcpy(loginRec.status, "Reset Failed");
				else
					strcpy(loginRec.status, "System Exited");
				fprintf(appendPtr, "%s|%s|%s|%s|%s", loginRec.ID, loginRec.name, loginRec.position, loginRec.status, loginRec.date);
				break;
			}		
		}
	}

	fclose(appendPtr);
}

void immediateResetPW(Staff staff[], int staffCount, char loginID[]) {
	system("cls");
	char newPW[21];
	for (int i = 0; i < staffCount; i++) {
		if (strcmp(staff[i].ID, loginID) == 0) {
			do{
			red();
			printf("\t\tNOTE: password maxiumum length 20\n");
			resetColor();
			printf("\t\t________________________________________________\n");
			printf("\t        \t\Reset password required\n");
			printf("\t\t________________________________________________\n");
			printf("\t\tLogin ID: %s\n", loginID);
			printf("\t\tPlease enter a new password: ");
			rewind(stdin);
			gets(newPW);
			if (strlen(newPW) > 21) {
				printf("Please read the instruction carefully.\n");
			}
			} while (strlen(newPW) > 21);
			
			strcpy(staff[i].password, newPW);
			green();
			printf("\t\tNew password updated.\n");
			resetColor();
			FILE* writePtr;
			writePtr = fopen("staff.txt", "w");
			if (writePtr == NULL) {
				printf("Unable to open file.");
				return;
			}

			for (int i = 0; i < staffCount; i++) {
				fprintf(writePtr, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
			}

			fclose(writePtr);

			break;
		}
	}
}