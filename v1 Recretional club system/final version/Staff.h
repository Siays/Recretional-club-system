#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#pragma warning (disable :4996)

void displayStaff(Staff staff[],int staffAmount );
void searchStaff(Staff staff[], int staffAmount);
void modifyStaff(Staff staff[], int staffAmount);
void ModifyStaffName(Staff staff[], int staffAmount);
void modifyStaffMenu();
void modifyStaffPhoneNum(Staff staff[], int staffAmount, char userInput[]);
void modifyStaffGender(Staff staff[], int staffAmount, char userInput[]);
void modifyStaffState(Staff staff[], int staffAmount, char userInput[]);
void modifyStaffAge(Staff staff[], int staffAmount, char userInput[]);
void modifyStaffPosition(Staff staff[], int staffAmount, char userInput[]);
void modifyStaffPW(Staff staff[], int staffAmount, char userInput[]);
void modifyStaffPWRP(Staff staff[], int staffAmount, int userInput); //PWRP stand for password recovery pin
void addNewStaff(Staff staff[], int staffAmount, char userInput[]);
void actOrDeactiveStaff(Staff staff[], int staffAmount);
void staffReport(Staff staff[], int staffAmount);
void activeStaff(Staff staff[], int staffAmount);
void resignedStaff(Staff staff[], int staffAmount);

void StaffModule() {
	Staff staff[100];
	FILE* read;
	int staffAmount = 0;
	int number;
	read = fopen("staff.txt", "r");
	if (read == NULL) {
		printf("Error. Failed to open file.");
		return;
	}

	for (int i = 0; i < 100; i++) {
		if (!feof(read)) {
			++staffAmount;
			fscanf(read, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%d|%s\n", staff[i].name, staff[i].contact, &staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, &staff[i].pwRecovery, staff[i].position, staff[i].email, &staff[i].age, staff[i].status);
		}

		else
			break;
	}
	fclose(read);


	do{
	system("cls");
	printf("\n\n\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                   STAFF MODULE\n");
	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                   0. Exit function\n");
	printf("\t\t\t\t                   1. Display all staff\n");
	printf("\t\t\t\t                   2. Search staff\n");
	printf("\t\t\t\t                   3. Modify staff\n");
	printf("\t\t\t\t                   4. Add new staff\n");
	printf("\t\t\t\t                   5. Activate/Deactivate staff\n");
	printf("\t\t\t\t                   6. Staff report\n");
	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("Enter the number --> ");
	rewind(stdin);
	scanf("%d", &number);
	switch(number){
	case 0:
		break;

	case 1:
		system("cls");
		displayStaff(staff, staffAmount);
		system("pause");
		break;
	case 2:
		system("cls");
		searchStaff(staff, staffAmount);
		system("pause");
		break;
	case 3:
		system("cls");
		modifyStaff(staff, staffAmount);
		system("pause");
		break;

	case 4:
		system("cls");
		addNewStaff(staff, staffAmount, &staffAmount);
		system("pause");
		break;

	case 5:
		system("cls");
		actOrDeactiveStaff(staff, staffAmount);
		system("pause");
		break;

	case 6:
		system("cls");
		staffReport(staff, staffAmount);
		system("pause");
		break;

	default:
		printf("Invalid.\n");
		break;
	}
	} while (number != 0);
}

//output need redesign
void displayStaff(Staff staff[], int staffAmount) {
	// OUTPUT need redeisng
	printf("\t\t\tStaff\n");
	printf("PWRP mean Password Recovery Pin\n");
	printf("========================================================================================================================================================\n");
	printf("%-20s %-12s %s %-10s %-10s %-15s %s %-20s %-25s %2s %-10s\n", "Name", "Contact", "Gender", "States", "ID", "Password", "PWRP", "  Position", "  Email", "Age", " Status");
	printf("========================================================================================================================================================\n");
	//display all staff information
	for (int i = 0; i < staffAmount; i++) {
		printf("%-20s|%-12s|%-6c|%-10s|%-10s|%-15s|%-6d|%-20s|%-23s|%-5d|%-10s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
	}
}
void searchStaff(Staff staff[],int staffAmount) {
	char id[10]; //staff use ID to search staff their imformation
	char verify;//this is to see if user want to continue to search another staff
	int noRecordFound = 0;//this is to allow user to tack if the ID they input is an existing staff or not
	int checkValid;
	do {
		noRecordFound = 0;//keep reset it to 0, otherwise it will keep accumulate

		do {
			printf("Enter id wished to modify for futher action: ");
			rewind(stdin);
			scanf("%s", id);
			checkValid = 1;
			if (!(id[0] >= 65 && id[0] <= 83) && !(id[1] >= 65 && id[1] <= 83)) {
				//refer to assci table, if the specified index of character is not A-Z , then 
				// consider it invalid
				printf("invalid input of id \n");
				checkValid = 0;

			}

		} while (checkValid == 0);

		for (int i = 0; i < staffAmount; i++) {
			
			if (strcmp(id, staff[i].ID) == 0)
			{
				//if any ID matched, it will show this
				printf("Name         : %s\n", staff[i].name);
				printf("Phone Number : %s\n", staff[i].contact);
				printf("Gender       : %c\n", staff[i].gender);
				printf("States       : %s\n", staff[i].state);
				printf("ID           : %s\n", staff[i].ID);
				printf("Password     : %s\n", staff[i].password);
				printf("Password Pin : %d\n", staff[i].pwRecovery);
				printf("Position     : %s\n", staff[i].position);
				printf("Email        : %s\n", staff[i].email);
				printf("Age          : %d\n", staff[i].age);
				printf("Status       : %s\n", staff[i].status);
				break;
			}
			//else if the system read through all records, and didnt find any match
			else if(i == staffAmount - 1 && strcmp(id, staff[i].ID) != 0)
				++noRecordFound;
		}

		//if no the found record, system will prompt the user
		if (noRecordFound > 0) {
			printf("\nNo record found.\n");
		}
		//then see if user want to search another staff
		printf("\nSearch another staff?\n");
		printf("Y = Yes: ");
		rewind(stdin);
		scanf("%c", &verify);
		verify = toupper(verify);
	} while (verify == 'Y');

}
void modifyStaff(Staff staff[], int staffAmount) {
	
	int noRecordFound = 0;
	int checkValid;
	char id[10];
		//staff input 
	do {
		printf("Enter id wished to modify for futher action: ");
		rewind(stdin);
		scanf("%s", id);
		checkValid = 1;
		if (!(id[0] >= 65 && id[0] <= 83) && !(id[1] >= 65 && id[1] <= 83)) {
			//refer to assci table, if the specified index of character is not A-Z , then 
			// consider it invalid
			printf("invalid input of id \n");
			checkValid = 0;

		}

	} while (checkValid == 0);

		for (int i = 0; i < staffAmount; i++) {
			if (strcmp(id, staff[i].ID) == 0)
			{
				printf("Name         : %s\n", staff[i].name);
				printf("Phone Number : %s\n", staff[i].contact);
				printf("Gender       : %c\n", staff[i].gender);
				printf("States       : %s\n", staff[i].state);
				printf("ID           : %s\n", staff[i].ID);
				printf("Password     : %s\n", staff[i].password);
				printf("Password Pin : %d\n", staff[i].pwRecovery);
				printf("Position     : %s\n", staff[i].position);
				printf("Email        : %s\n", staff[i].email);
				printf("Age          : %d\n", staff[i].age);
				printf("Status       : %s\n", staff[i].status);
				break;
			}
			else if (i == staffAmount - 1 && strcmp(id, staff[i].ID) != 0)
				++noRecordFound;
		}

		if (noRecordFound > 0) {
			printf("\nNo record found.\n");
		}
		else {
			int number;
			
			do {
			modifyStaffMenu();
			printf("Enter the number: ");
			rewind(stdin); //incase user key in character
			scanf("%d", &number);
			

				switch (number) {
				case 0:
					break;
				case 1:
					ModifyStaffName(staff, staffAmount, id);
					break;

				case 2:
					modifyStaffPhoneNum(staff, staffAmount, id);
					break;

				case 3:
					modifyStaffGender(staff, staffAmount, id);
					break;

				case 4:
					modifyStaffState(staff, staffAmount, id);
					break;

				case 5:
					modifyStaffAge(staff, staffAmount, id);
					break;

				case 6:
					modifyStaffPosition(staff, staffAmount, id);
					break;

				case 7:
					modifyStaffPW(staff, staffAmount, id);
					break;

				case 8:
					modifyStaffPWRP(staff,staffAmount,id);
					break;

				default:
					printf("Invalid\n");
				}
			} while (number != 0);
			
		}
}
void modifyStaffMenu()
{
	printf("\n\n\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                   MODIFY STAFF\n");
	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                   0. Exit function\n");
	printf("\t\t\t\t                   1. Staff name\n");
	printf("\t\t\t\t                   2. Staff phoneNumber\n");
	printf("\t\t\t\t                   3. Staff gender\n");
	printf("\t\t\t\t                   4. Staff states\n");
	printf("\t\t\t\t                   5. Staff age\n");
	printf("\t\t\t\t                   6. Staff password\n");
	printf("\t\t\t\t                   5. Staff password reset pin\n");
	printf("\t\t\t\t---------------------------------------------------------\n");
}
void ModifyStaffName(Staff staff[], int staffAmount, char userInput[]){
	char newName[51], verify;

	for (int i = 0; i < staffAmount; i++) {
		if (strcmp(staff[i].ID, userInput) == 0) { // if ID match 
			printf("Enter new staff name: "); //Enter new name
			rewind(stdin);
			gets(newName); //temporarily input

			printf("You really what to change staff name?\n");
			printf("verify: ");
			rewind(stdin);
			scanf("%c", &verify);
			verify = toupper(verify);
			if (verify == 'Y') {
				strcpy(staff[i].name, newName); //if user confirm want to change name 
				//the name will be replaced
				printf("New name updated.\n");
				break;
			}
			 

			else {
				printf("It can no change \n");
				break;
			}
		}
	}

	FILE* write;
	write = fopen("staff.txt", "w");
	if (write == NULL) {
		printf("Unable to open file.");
		return;
	}

	for (int i = 0; i < staffAmount; i++) {
		fprintf(write, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
	}

	fclose(write);
}
void modifyStaffPhoneNum(Staff staff[], int staffAmount, char userInput[]){
	char newContact[13], verify;

	for (int i = 0; i < staffAmount; i++) {
		if (strcmp(staff[i].ID, userInput) == 0) {
			printf("Enter new staff contact: ");
			rewind(stdin);
			gets(newContact);

			printf("You really what to change staff contact?\n");
			printf("YES/NO: ");
			rewind(stdin);
			scanf("%c", &verify);
			verify = toupper(verify);
			if (verify == 'Y') {
				strcpy(staff[i].contact, newContact);
			}
			else {
				printf("It can no change \n");
				break;
			}
			break;
		}
	}

	FILE* write;
	write = fopen("staff.txt", "w");
	if (write == NULL) {
		printf("Unable to open file.");
		return;
	}

	for (int i = 0; i < staffAmount; i++) {
		fprintf(write, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
	}

	fclose(write);

}
void modifyStaffGender(Staff staff[], int staffAmount, char userInput[]){
	char newGender, verify;

	for (int i = 0; i < staffAmount; i++) {
		if (strcmp(staff[i].ID, userInput) == 0) {
			printf("You really what to change staff gender??\n");
			printf("YES/NO: ");
			rewind(stdin);
			scanf("%c", &verify);
			verify = toupper(verify);
			if (verify == 'Y') {
				if (staff[i].gender == 'M') //if the staff gender was 'M', replace by 'F'
					staff[i].gender = 'F';
				else//else if the staff gender was F , replaced By M
					staff[i].gender = 'M';
			}
			else {
				printf("It can no change \n");//if user doesn't confirm their changes
				break;
			}
		}
	}

	FILE* write;
	write = fopen("staff.txt", "w");
	if (write == NULL) {
		printf("Unable to open file.");
		return;
	}

	for (int i = 0; i < staffAmount; i++) {
		fprintf(write, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
	}

	fclose(write);
}
void modifyStaffState(Staff staff[], int staffAmount, char userInput[]){
	char newState[21], verify;

	for (int i = 0; i < staffAmount; i++) {
		if (strcmp(staff[i].ID, userInput) == 0) {
			printf("Enter new staff state: ");
			rewind(stdin);
			gets(newState);

			printf("You really what to change staff state?\n");
			printf("YES/NO: ");
			rewind(stdin);
			scanf("%c", &verify);
			verify = toupper(verify);
			if (verify == 'Y') {
				strcpy(staff[i].state, newState);
			}
			else {
				printf("It can no change \n");
				break;
			}
			break;
		}
	}

	FILE* write;
	write = fopen("staff.txt", "w");
	if (write == NULL) {
		printf("Unable to open file.");
		return;
	}

	for (int i = 0; i < staffAmount; i++) {
		fprintf(write, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
	}

	fclose(write);

}
void modifyStaffAge(Staff staff[], int staffAmount, char userInput[]){

	int newAge;
	char verify;
	int wrong = 0;

	for (int i = 0; i < staffAmount; i++) {
		if (strcmp(staff[i].ID, userInput) == 0) {
			do {
				printf("Enter new staff age: ");
				rewind(stdin); //incase user key in digit
				scanf("%d", &newAge);
				if (newAge <= 0)
				{
					printf("invalid input of new staff age\n");
					wrong++;
				}
				else {
					wrong = 0;
				}
			} while (wrong != 0);

			printf("You really what to change staff age?\n");
			printf("YES/NO: ");
			rewind(stdin);
			scanf("%c", &verify);
			verify = toupper(verify);
			if (verify == 'Y') {
				staff[i].age = newAge;
			}
			else {
				printf("It can no change \n");
				break;
			}
		}
	}

	FILE* write;
	write = fopen("staff.txt", "w");
	if (write == NULL) {
		printf("Unable to open file.");
		return;
	}

	for (int i = 0; i < staffAmount; i++) {
		fprintf(write, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
	}

	fclose(write);
}
void modifyStaffPosition(Staff staff[], int staffAmount, char userInput[]) {
	char newPosition[21];
	char verify;

	for (int i = 0; i < staffAmount; i++) {
		if (strcmp(staff[i].ID, userInput) == 0) {
			printf("Enter new staff position: ");
			rewind(stdin);
			gets(newPosition);

			printf("You really what to change staff position?\n");
			printf("YES/NO: ");
			rewind(stdin);
			scanf("%c", &verify);
			verify = toupper(verify);
			if (verify == 'Y') {
				strcpy(staff[i].position, newPosition);
			}
			else {
				printf("It can no change \n");
				break;

			}
		}
	}

	FILE* write;
	write = fopen("staff.txt", "w");
	if (write == NULL) {
		printf("Unable to open file.");
		return;
	}
	for (int i = 0; i < staffAmount; i++) {
		fprintf(write, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
	}
}
void modifyStaffPW(Staff staff[], int staffAmount, char userInput[]) {

	char newPassword[21];
	char verify;

	for (int i = 0; i < staffAmount; i++) {
		if (strcmp(staff[i].ID, userInput) == 0) {
			do{
			printf("Note: Maximum length 20\n");
			printf("Enter new staff password: ");
			rewind(stdin);
			gets(newPassword);
			} while (strlen(newPassword) > 20);

			printf("You really what to change staff password?\n");
			printf("YES/NO: ");
			rewind(stdin);
			scanf("%c", &verify);
			verify = toupper(verify);
			if (verify == 'Y') {
				strcpy(staff[i].password, newPassword);
			}
			else{
				printf("It can no change \n");
				break;
				
			}
		}
	}

	FILE* write;
	write = fopen("staff.txt", "w");
	if (write == NULL) {
		printf("Unable to open file.");
		return;
	}

	for (int i = 0; i < staffAmount; i++) {
		fprintf(write, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
	}

	fclose(write);
}
//PWRP stand for password recovery pin
void modifyStaffPWRP(Staff staff[], int staffAmount, char userInput[]) {
	int newPin;
	char verify;

	for (int i = 0; i < staffAmount; i++) {
		if (strcmp(staff[i].ID, userInput) == 0) {
			do {
				printf("Note: avoid 0 as the leading number, maximum 6 pin\n");
				printf("Enter new staff password recovery pin: ");
				rewind(stdin); //incase user input character
				scanf("%d", &newPin);

				if (isdigit(newPin) == 0 || newPin > 999999) {
					printf("Please read the instruction clearly.\n");
				}

			} while (isdigit(newPin) ==0 || newPin > 999999);

			printf("You really what to change staff password recovery pin?\n");
			printf("YES/NO: ");
			rewind(stdin);
			scanf("%c", &verify);
			verify = toupper(verify);
			if (verify == 'Y') {
				staff[i].pwRecovery = newPin;
			}
			else {
				printf("It can no change \n");
				break;

			}
		}
	}

	FILE* write;
	write = fopen("staff.txt", "w");
	if (write == NULL) {
		printf("Unable to open file.");
		return;
	}

	for (int i = 0; i < staffAmount; i++) {
		fprintf(write, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
	}

	fclose(write);
}

void addNewStaff(Staff staff[], int staffAmount, int* updatestaffAmount) {
	char name[41], contact[13], gender, state[15], ID[7];
	char password[41];
	int pwRecovery, age;
	char position[21], email[41];
	char verify;
	int wrong = 0, checkValid;

	printf("Add new staff module\n");
	printf("Confirm to add new staff(y/n)?\n"); //if user confirm want to add staff
	printf("number: ");
	rewind(stdin);
	scanf("%c", &verify);
	verify = toupper(verify);

	if (verify == 'Y') {
		do {
			printf("Enter new name: ");
			rewind(stdin);
			gets(staff[staffAmount].name);
			checkValid = 1;
			for (int i = 0; i < strlen(staff[staffAmount].name); i++) {
				//check key in name is aplpha & space
				if (isalpha(staff[staffAmount].name[i]) == 0 && staff[staffAmount].name[i] != ' ')
				{
					printf("invalid input of staff name\n");
					checkValid = 0;
					break;
				}
			}
		} while (checkValid == 0);

		do {
			printf("Enter new staff contact: ");
			rewind(stdin);
			gets(staff[staffAmount].contact);
			checkValid = 0;
			for (int i = 0; i < strlen(staff[staffAmount].contact); i++) {
				//check key in contact just only number
				if (isdigit(staff[staffAmount].contact[i]) != 0)
				{
					checkValid = 1;
				}
				else
				{
					continue;
				}
			}
			if (checkValid == 0)
				printf("invalid input of phone number \n");
		} while (checkValid == 0);

		do {
			printf("Enter new staff gender: ");
			rewind(stdin);
			scanf("%c", &staff[staffAmount].gender);
			staff[staffAmount].gender = toupper(staff[staffAmount].gender);
			checkValid = 0;
			//check key in the gender just can F & M
			if ((staff[staffAmount].gender == 'F') || (staff[staffAmount].gender == 'M'))
			{
				checkValid = 1;
			}
			else
			{
				printf("invalid input of new staff gender \n");
			}
		} while (checkValid == 0);

		do {
			printf("Enter new staff state: ");
			rewind(stdin);
			gets(staff[staffAmount].state);
			checkValid = 1;
			for (int i = 0; i < strlen(staff[staffAmount].state); i++) {
				//check key in state just only alphabet
				if (isalpha(staff[staffAmount].state[i]) == 0)
				{
					printf("invalid input of states\n");
					checkValid = 0;
					break;
				}
			}
		} while (checkValid == 0);

		int repeat = 0; // to check if any exist ID in the record
		do {
			repeat = 0;
			printf("Enter new staff ID: ");
			rewind(stdin);
			gets(staff[staffAmount].ID);
			checkValid = 1;
			for (int i = 0; i < strlen(staff[staffAmount].ID); i++) {
				//check key in staff id have number and alphabet
				if (isalpha(staff[staffAmount].ID[i]) == 0 && isdigit(staff[staffAmount].ID[i]) == 0)
				{
					printf("invalid input of staff id \n");
					checkValid = 0;
					break;
				}
			}

			for (int i = 0; i < staffAmount; i++) {//read through every record
				if (strcmp(staff[staffAmount].ID, staff[i].ID) == 0) { //if any of the record
					//match with the user input
					++repeat;//++repeat , which cause the user unable to exit the do while loop
					printf("Same ID detected.\n");
					break;
				}
			}
		} while (checkValid == 0 || repeat != 0);

		
		
			printf("Enter new staff password: ");
			rewind(stdin);
			gets(staff[staffAmount].password);
			
		

		do {
			printf("Enter new staff reset pin: ");
			scanf("%d", &staff[staffAmount].pwRecovery);
			if (staff[staffAmount].pwRecovery <= 0)
			{
				printf("invalid input of new staff reset pin\n");
				wrong++;
			}
			else {
				wrong = 0;
			}
		} while (wrong != 0);

		do {
			printf("Enter new staff age: ");
			scanf("%d", &staff[staffAmount].age);
			//check key in ahe just only number
			if (staff[staffAmount].age <= 0)
			{
				printf("invalid input of new staff age\n");
				wrong++;
			}
			else {
				wrong = 0;
			}
		} while (wrong != 0);


		do {
			printf("Enter new staff position: ");
			rewind(stdin);
			gets(staff[staffAmount].position);
			checkValid = 1;
			for (int i = 0; i < strlen(staff[staffAmount].position); i++) {
				//check key in position just only alphabet
				if (isalpha(staff[staffAmount].position[i]) == 0)
				{
					printf("invalid input of position\n");
					checkValid = 0;
					break;
				}
			}
		} while (checkValid == 0);

		do {
			printf("Enter new staff email: ");
			rewind(stdin);
			gets(staff[staffAmount].email);
			checkValid = 1;
			for (int i = 0; i < strlen(staff[staffAmount].email); i++) {
				//it check all the key in email have @ or no
				if (i == staffAmount-1 && staff[staffAmount].email[i] != '@')
				{
					printf("invalid input of email\n");
					checkValid = 0;
					break;
				}
			}
		} while (checkValid == 0);

		

		strcpy(staff[staffAmount].status, "Active");// set the new staff status in active
		++staffAmount; //add the staffAmount for the file to add in this all new record
		++* updatestaffAmount; //update the current staff amount
	}
	else
		printf("No new record has been made.\n");

	//if user confirm want to add new staff information, then the new staff information will be record
	// the system will go into write file,to write the record
	if(verify == 'Y'){
	FILE* write;
	write = fopen("staff.txt", "w");
	if (write == NULL) {
		printf("Unable to open file.");
		return;
	}

	for (int i = 0; i < staffAmount; i++) {
		fprintf(write, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
	}

	fclose(write);
	}

	
}

// if status = active,staff is working
// else then the working is resigned
void actOrDeactiveStaff(Staff staff[], int staffAmount) {
	char staffID[10], verify;
	printf("Change staff status\n");
	printf("Confirm to delete staff?\n");
	printf("Enter the number: ");
	rewind(stdin);
	scanf("%c", &verify);
	verify = toupper(verify);
	if (verify == 'Y') {
		int checkValid;
		do {
			printf("Enter you need to  modify staff ID: ");
			rewind(stdin);
			gets(staffID);
			checkValid = 1;
			if (!(staffID[0] >= 65 && staffID[0] <= 83) && !(staffID[1] >= 65 && staffID[1] <= 83)) {
				//refer to assci table, if the specified index of character is not A-Z , then 
				// consider it invalid
				printf("invalid input of id \n");
				checkValid = 0;

			}
		} while (checkValid == 0);
		

		for (int i = 0; i < staffAmount; i++) {
			if (strcmp(staff[i].ID, staffID) == 0) {
				if (strcmp(staff[i].status, "Active") == 0) // if the staff was "Active" , will be change to Resigned
					strcpy(staff[i].status, "Resigned");
				else//else if the staff was "Resigned, change back to active
					strcpy(staff[i].status, "Active");
			}
		}

		FILE* write;
		write = fopen("staff.txt", "w");
		if (write == NULL) {
			printf("Unable to open file.");
			return;
		}

		for (int i = 0; i < staffAmount; i++) {
			fprintf(write, "%s|%s|%c|%s|%s|%s|%d|%s|%s|%d|%s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
		}

		fclose(write);
	}
	else
		printf("It can no change \n");
}

void staffReportMenu() {
	printf("\n\n\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                   REPORT\n");
	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                   0. Exit function\n");
	printf("\t\t\t\t                   1. Active Staff\n");
	printf("\t\t\t\t                   2. Resigned Staff\n");
	printf("\t\t\t\t---------------------------------------------------------\n");
}

void staffReport(Staff staff[], int staffAmount) {
	int number;
	do{
	staffReportMenu();
	printf("Enter the number: ");
	rewind(stdin);
	scanf("%d", &number);

	switch (number){
	case 0:
		break;

	case 1:
		activeStaff(staff, staffAmount);
		break;

	case 2:
		resignedStaff(staff, staffAmount);
		break;
	
	default:
		printf("Invalid.\n");
	}
	} while (number != 0);
}

void activeStaff(Staff staff[],int staffAmount) {
	printf("\t\t\tStaff\n");
	printf("========================================================================================================================================\n");
	printf("%-20s|%-12s|%s|%-10s|%-10s|%-15s|%s|%-21s|%-18s|%s|%-10s\n", "Name", "Contact", "Gender", "States", "ID", "Password", "Password Rec.Pin", "Position", "Email", "Age", "Status");
	printf("========================================================================================================================================\n");
	for (int i = 0; i < staffAmount; i++) {
		if (strcmp(staff[i].status, "Active") == 0) // if the record match "active" status, then will print it out
		{
			printf("%-20s|%-12s|%-6c|%-10s|%-10s|%-15s|%-10d|%-21s|%-18s|%-3d|%-10s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
		}
	}
}
//output show all resigned staff
void resignedStaff(Staff staff[], int staffAmount) {
	printf("\t\t\tStaff\n");
	printf("========================================================================================================================================\n");
	printf("%-20s|%-12s|%s|%-10s|%-10s|%-15s|%s|%-21s|%-18s|%s|%-10s\n", "Name", "Contact", "Gender", "States", "ID", "Password", "Password Rec.Pin", "Position", "Email", "Age", "Status");
	printf("========================================================================================================================================\n");


	for (int i = 0; i < staffAmount; i++) {
		//if the record match resigned it will print out  
		if (strcmp(staff[i].status, "Resigned") == 0)
		{

			printf("%-20s|%-12s|%-6c|%-10s|%-10s|%-15s|%-10d|%-21s|%-18s|%-3d|%-10s\n", staff[i].name, staff[i].contact, staff[i].gender, staff[i].state, staff[i].ID, staff[i].password, staff[i].pwRecovery, staff[i].position, staff[i].email, staff[i].age, staff[i].status);
		}
	}
}