#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable :4996)



//function declaration
void memberMenu(int* chooseFunc);

void addMember();

void searchMember();
void searchMemberMenu(int* searchChoice);
void searchMemberName();
void searchMemberPhoneNum();
void searchMemberId();

void displayAllMember();

void modifyMember();
void modifyMemberMenu(int* choice);
void modifyMemberAction(int choiceModify);


void delMember();

void memberReport();
void memActive();
void memDeactice();
void memWithdraw();

//function validation
void checkPhoneNum(int* check1, int* check01, char phoneNum[13]);

void memberSystem() {

	int chooseFunc;

	do {

		memberMenu(&chooseFunc);

		switch (chooseFunc) {
		case 0:
			printf("Thank you for using.\n");
			system("pause");
			break;

		case 1:
			system("cls");
			addMember();
			break;

		case 2:
			system("cls");
			searchMember();
			system("pause");
			break;

		case 3:
			system("cls");
			displayAllMember();
			system("pause");
			break;

		case 4:
			system("cls");
			modifyMember();
			system("pause");
			break;

		case 5:
			system("cls");
			delMember();
			system("pause");
			break;

		case 6:
			system("cls");
			memberReport();
			system("pause");
			break;
		}
	} while (chooseFunc != 0);


}


void memberMenu(int* chooseFunc) {

	system("CLS");

	printf("\t\t\t\t\t\t\t  ---------------------------------------------------------\n");
	printf("\t\t\t\t\t\t\t                      MEMBER MENU\n");
	printf("\t\t\t\t\t\t\t  ---------------------------------------------------------\n");
	printf("\t\t\t\t\t\t\t                     0. Exit to main page\n");
	printf("\t\t\t\t\t\t\t                     1. Add member\n");
	printf("\t\t\t\t\t\t\t                     2. Search member\n");
	printf("\t\t\t\t\t\t\t                     3. Display member\n");
	printf("\t\t\t\t\t\t\t                     4. Modify member\n");
	printf("\t\t\t\t\t\t\t                     5. Delete member\n");
	printf("\t\t\t\t\t\t\t                     6. Report (monthly)\n");
	printf("\t\t\t\t\t\t\t  ---------------------------------------------------------\n");

	do {

		printf("Enter your choice --> ");
		rewind(stdin);
		scanf("%d", &*chooseFunc);


		if (*chooseFunc < 0 || *chooseFunc > 7) {
			red();
			printf("Error, please enter choice between 0~7.\n\n");
			resetColor();
		}
	} while (*chooseFunc < 0 || *chooseFunc > 7);


}


void addMember() {

	struct member info[1000];
	int countRecord = 0;
	int count = 0; // this count is to identify how many record currently have
	char answer, choice;

	FILE* fp;
	fp = fopen("member.txt", "a");

	if (fp == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}



	FILE* fptr;
	fptr = fopen("member.txt", "r");

	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			++count;
		}

		else
			break;
	}

	fclose(fptr);



	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                      ADD MEMBER\n");
	printf("\t\t\t\t---------------------------------------------------------\n\n");

	do {
		printf("Enter new member name :  ");
		rewind(stdin);
		gets(info[count].name);


		int check1 = 0, check01 = 0;//this count is to check if the new ID match any exisiting ID


		do {
			check1 = 0;
			check01 = 0;

			printf("Enter new member phone number : ");
			rewind(stdin);
			scanf("%s", &info[count].phoneNum);

			checkPhoneNum(&check1, &check01, info[count].phoneNum);


		} while (check1 > 0 || check01 > 0);


		int check2 = 0;

		do {
			check2 = 0;

			printf("Enter new member id : ");
			rewind(stdin);
			gets(info[count].id);

			for (int i = 0; i < count; i++) {
				if (strcmp(info[count].id, info[i].id) == 0)
					++check2;
			}

			if (check2 > 0) {
				red();
				printf("ID already exists.\nPlease insert a new ID!\n\n");
				resetColor();
			}


		} while (check2 > 0);



		do {
			printf("Enter member gender (Male - M / Female - F) : ");
			rewind(stdin);
			scanf("%c", &info[count].gender);

			if (info[count].gender != 'M' && info[count].gender != 'm' && info[count].gender != 'f' && info[count].gender != 'F') {
				red();
				printf("Error : Invalid Input\n");
				resetColor();
			}

		} while (info[count].gender != 'm' && info[count].gender != 'M' && info[count].gender != 'F' && info[count].gender != 'f');




		int check3 = 0;

		do {
			check3 = 0;

			printf("Enter member email :  ");
			rewind(stdin);
			gets(info[count].email);

			for (int i = 0; i < count; i++) {
				if (strcmp(info[count].email, info[i].email) == 0)
					++check3;
			}

			if (check3 > 0) {
				red();
				printf("This email already been register.\nPlease insert a new email!\n\n");
				resetColor();
			}

		} while (check3 > 0);



		do {
			printf("Enter member registration day :");
			rewind(stdin);
			scanf("%d", &info[count].time.regDay);

			if (info[count].time.regDay < 1 || info[count].time.regDay >31) {
				red();
				printf("Invalid input.\nPlease enter a day between 1-31\n");
				resetColor();
			}

		} while (info[count].time.regDay < 1 || info[count].time.regDay >31);


		do {
			printf("Enter member registration month :");
			rewind(stdin);
			scanf("%d", &info[count].time.regMonth);

			if (info[count].time.regMonth < 1 || info[count].time.regMonth >12) {
				red();
				printf("Invalid input.\nPlease enter a month between 1-12\n");
				resetColor();
			}

		} while (info[count].time.regMonth < 1 || info[count].time.regMonth >12);

		printf("Enter member registration year :");
		rewind(stdin);
		scanf("%d", &info[count].time.regYear);

		strcpy(info[count].status, "active");

		fprintf(fp, "%s|%c|%s|%s|%s|%d|%d|%d|%s\n", info[count].name, info[count].gender, info[count].phoneNum, info[count].id, info[count].email, info[count].time.regDay, info[count].time.regMonth, info[count].time.regYear, info[count].status);
		countRecord++;

		printf("\n");
		printf("Do you want to add anymore new member ?\n");
		printf("Enter 'Y' to add more :");
		rewind(stdin);
		scanf("%c", &answer);

		printf("\n\n");


		printf("%d record are successfull add into member.txt file.\n", countRecord);

	} while (answer == 'y' || answer == 'Y');


	fclose(fp);
	fclose(fptr);

}

void searchMember() {

	int searchChoice, selection;

	struct member info[1000];
	char nameSearch[30], idSearch[10], phoneNumSearch[13], answer;


	FILE* fptr, * fp;
	fptr = fopen("member.txt", "r");
	fp = fopen("member.txt", "a");

	if (fptr == NULL || fp == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}


	do {
		searchMemberMenu(&searchChoice);


		switch (searchChoice) {
		case 1:
			searchMemberName();

			break;

		case 2:
			searchMemberPhoneNum();
			break;

		case 3:
			searchMemberId();
			break;

		case 4:
			printf("Thank you.\n");
			return;
		}

		printf("\nDo you want to search anymore member?\nEnter 'Y' to search more  :");
		rewind(stdin);
		scanf("%c", &answer);
		system("CLS");
	} while (answer == 'y' || answer == 'Y');

	fclose(fp);
	fclose(fptr);

}

void searchMemberMenu(int* searchChoice) {



	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                    SEARCH MEMBER\n");
	printf("\t\t\t\t---------------------------------------------------------\n\n");

	do {
		printf("Which category of data you hope to search by?\n\n");
		printf("1. Member Name\n");
		printf("2. Member Phone Number\n");
		printf("3. Member ID\n");
		printf("4. Cancel\n\n");


		printf("Your Choice : ");
		rewind(stdin);
		scanf("%d", &*searchChoice);


		if (*searchChoice < 1 || *searchChoice > 4) {

			red();
			printf("Invalid input.");
			printf("Please reenter your choice between the range(1~4).\n\n");
			resetColor();
			printf("**************************************************************\n\n");

		}

	} while (*searchChoice != 1 && *searchChoice != 2 && *searchChoice != 3 && *searchChoice != 4);

}

void searchMemberName() {

	struct member info[1000];
	char nameSearch[30];
	int recCount = 0;

	FILE* fptr;
	fptr = fopen("member.txt", "r");


	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	printf("Enter member name to search :");
	rewind(stdin);
	scanf("%[^\n]", &nameSearch);

	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			if (strcmp(info[i].name, nameSearch) == 0) {
				++recCount;
				printf("\nYour result:\n\n");

				printf("\nMember Name\t\tMember Gender\t  Member Phone Number\t Member ID\t Member Email\t Member Registration Date\tMember Status\n");
				printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
				printf("%-27s %-13c %-22s %-15s %-22s %d-%d-%-20d %s\n\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);

			}

		}

		else {
			break;
		}
	}

	if (recCount == 0) {
		printf("0 record been found.\n");
	}
	else {
		printf("%d records been found.\n", recCount);
	}

	yellow();
	printf("\nALERT :\n");
	printf("\nPlease try again if the result that you want haven't came out, it means that your member name is invalid\n");
	resetColor();
	fclose(fptr);
}

void searchMemberPhoneNum() {

	struct member info[1000];
	char phoneNumSearch[30];
	int recCount = 0;

	FILE* fptr;
	fptr = fopen("member.txt", "r");

	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}


	printf("Enter member phone number to search :");
	rewind(stdin);
	scanf("%[^\n]", &phoneNumSearch);
	if (strlen(phoneNumSearch) < 11 || strlen(phoneNumSearch) > 12)
	{
		red();
		printf("The contact number is invalid. Please Enter Again.\n\n");
		resetColor();
	}

	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			if (strcmp(info[i].phoneNum, phoneNumSearch) == 0) {
				++recCount;
				printf("\nYour result:\n\n");

				printf("\nMember Name\t\tMember Gender\t  Member Phone Number\t Member ID\t Member Email\t Member Registration Date\tMember Status\n");
				printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
				printf("%-27s %-13c %-22s %-15s %-22s %d-%d-%-20d %s\n\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);

			}

		}

		else {
			break;
		}

	}

	if (recCount == 0) {
		printf("0 record been found.\n");
	}
	else {
		printf("%d records been found.\n", recCount);
	}

	yellow();
	printf("\nALERT :\n");
	printf("\nPlease try again if the result that you want haven't came out, it means that your member phone number is invalid\n");
	resetColor();
	fclose(fptr);
}

void searchMemberId() {

	struct member info[1000];
	char idSearch[30];
	int recCount = 0;

	FILE* fptr;
	fptr = fopen("member.txt", "r");

	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	printf("Enter member id to search :");
	rewind(stdin);
	scanf("%[^\n]", &idSearch);
	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			if (strcmp(info[i].id, idSearch) == 0) {
				++recCount;
				printf("\nYour result:\n\n");
				printf("\nMember Name\t\tMember Gender\t  Member Phone Number\t Member ID\t Member Email\t Member Registration Date\tMember Status\n");
				printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
				printf("%-27s %-13c %-22s %-15s %-22s %d-%d-%-20d %s\n\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);

			}
		}
		else {
			break;
		}

	}

	if (recCount == 0) {
		printf("0 record been found.\n");
	}
	else {
		printf("%d records been found.\n", recCount);
	}

	yellow();
	printf("\nALERT :\n");
	printf("\nPlease try again if the result that you want haven't came out, it means that your member id is invalid\n");
	resetColor();
	fclose(fptr);

}

void displayAllMember() {
	struct member info[1000];
	int recCount = 0;

	FILE* fptr;
	fptr = fopen("member.txt", "r");

	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	printf("\nYour result:\n\n");
	printf("\nMember Name\t\tMember Gender\t  Member Phone Number\t Member ID\t Member Email\t Member Registration Date\tMember Status\n");
	printf("------------------------------------------------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			++recCount;
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			printf("%-27s %-13c %-22s %-15s %-22s %d-%d-%-20d %s\n\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);

		}

		else
			break;
	}


	printf("%d records been found.\n", recCount);

	fclose(fptr);
}

void modifyMember() {
	int choiceModify;

	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                  MODIFY MEMBER MENU\n");
	printf("\t\t\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t                 1. Modify member name\n");
	printf("\t\t\t\t                 2. Modify member gender\n");
	printf("\t\t\t\t                 3. Modify member phone number\n");
	printf("\t\t\t\t                 4. Modify member email\n");
	printf("\t\t\t\t                 5. Modify member status\n");
	printf("\t\t\t\t---------------------------------------------------------\n");

	do {
		printf("Enter your choice : ");
		scanf("%d", &choiceModify);


		if (choiceModify < 1 || choiceModify > 5) {
			red();
			printf("Error, please enter choice between 1~5.\n\n");
			resetColor();
		}


	} while (choiceModify != 1 && choiceModify != 2 && choiceModify != 3 && choiceModify != 4 && choiceModify != 5);

	modifyMemberAction(choiceModify);
}

void modifyMemberAction(int choiceModify) {


	char idSearch[30], answer;
	struct member info[1000];
	int recordCount = 0;
	int statusSelection = 0;

	int check1 = 0, check01 = 0, count = 0;//this count is to check if the new ID match any exisiting ID

	FILE* fptr;
	fptr = fopen("member.txt", "r");


	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			++recordCount;
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);

		}
		else
			break;
	}
	fclose(fptr);


	do {

		printf("Enter member id to search :");
		rewind(stdin);
		gets(idSearch);
		for (int i = 0; i < recordCount; i++) {
			if (strcmp(info[i].id, idSearch) == 0) {
				printf("\nYour result:\n\n");
				printf("\nMember Name\t\tMember Gender\t  Member Phone Number\t Member ID\t Member Email\t Member Registration Date\tMember Status\n");
				printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
				printf("%-27s %-13c %-22s %-15s %-22s %d-%d-%-20d %s\n\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);

				printf("Is this the member you want to modify?\nYour answer ('Y' for yes)  : ");
				rewind(stdin);
				scanf("%c", &answer);
				if (answer == 'Y' || answer == 'y') {

					switch (choiceModify) {
					case 1:

						printf("Enter a new name for the member : ");
						rewind(stdin);
						gets(info[i].name);
						printf("\nYour change has been save successfully.\n");
						break;

					case 2:

						do {
							printf("Enter member gender (Male - M / Female - F) : ");
							rewind(stdin);
							scanf("%c", &info[i].gender);

							if (info[i].gender != 'M' && info[i].gender != 'm' && info[i].gender != 'f' && info[i].gender != 'F') {
								red();
								printf("Error : Invalid Input\n");
								resetColor();
							}

						} while (info[i].gender != 'm' && info[i].gender != 'M' && info[i].gender != 'F' && info[i].gender != 'f');

						printf("\nYour change has been save successfully.\n");
						break;

					case 3:
						do {
							check1 = 0;
							check01 = 0;
							char tempPhoneNum[15];
							printf("Enter a new phone number for the member : ");
							rewind(stdin);
							scanf("%s", &tempPhoneNum);

							checkPhoneNum(&check1, &check01, tempPhoneNum);
							strcpy(info[i].phoneNum, tempPhoneNum);
						} while (check1 > 0 || check01 > 0);
						printf("\nYour change has been save successfully.\n");
						break;

					case 4:

						do {

							check1 = 0;

							printf("Enter a new email for the member : ");
							rewind(stdin);
							scanf("%s", &info[i].email);

							for (int j = 0; j < recordCount; j++) {
								if (strcmp(info[i].email, info[j].email) == 0)
									++check1;
							}

							if (check1 > 0) {
								red();
								printf("This email already been register.\nPlease insert a new email!\n\n");
								resetColor();
							}


						} while (check1 > 0);
						printf("\nYour change has been save successfully.\n");
						break;

					case 5:

						do {
							printf("Enter a new status for the member : \n");
							printf("\t 1 for active\n");
							printf("\t 2 for deactive\n");
							printf("\t 3 for withdraw\n\n");
							printf("Your selection :\n");
							scanf("%d", &statusSelection);

							switch (statusSelection) {
							case 1:
								strcpy(info[i].status, "active");
								break;

							case 2:
								strcpy(info[i].status, "deactive");
								break;

							case 3:
								strcpy(info[i].status, "withdraw");
								break;
							}

						} while (statusSelection != 1 && statusSelection != 2 && statusSelection != 3);


						printf("\nYour change has been save successfully.\n");
						break;
					}



				}

			}

		}


		fptr = fopen("member.txt", "w");
		if (fptr == NULL) {
			printf("Error!! Member.txt file can't open!!");
			return;
		}
		for (int i = 0; i < recordCount; i++) {
			fprintf(fptr, "%s|%c|%s|%s|%s|%d|%d|%d|%s\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);
		}

		yellow();
		printf("\nALERT :\n");
		printf("\nPlease try again if the result that you want haven't came out, it means that your member id is invalid.\n\n");
		resetColor();

		printf("Do you one to modify member again?\n Your choice ('Y' for yes):");
		rewind(stdin);
		scanf("%c", &answer);
		printf("\n\n");

	} while (answer == 'y' || answer == 'Y');

	fclose(fptr);

}

void delMember() {


	FILE* fptr1, * fptr2;
	struct member info[1000];
	char idDel[20], ans, answer;
	int a = 0, recCount = 0;


	fptr1 = fopen("member.txt", "r");
	fptr2 = fopen("temp.txt", "w");

	if (fptr1 == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	if (fptr2 == NULL) {
		printf("Error!! temporary.txt file can't open!!");
		return;
	}

	do {
		system("CLS");

		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                  Delete Member\n");
		printf("\t\t\t\t---------------------------------------------------------\n\n");



		printf("Enter member id to search the member you want to delete :");
		rewind(stdin);
		scanf("%[^\n]", &idDel);
		for (int i = 0; i < 1000; i++) {
			while (!feof(fptr1)) {
				fscanf(fptr1, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
				if (strcmp(info[i].id, idDel) == 0) {
					++recCount;
					printf("\nYour result:\n\n");

					printf("\nMember Name\t\tMember Gender\t  Member Phone Number\t Member ID\t Member Email\t Member Registration Date\tMember Status\n");
					printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
					printf("%-27s %-13c %-22s %-15s %-22s %d-%d-%-20d %s\n\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);

					printf("Is this the member you want to delete?\nYour answer('Y' for yes):");
					rewind(stdin);
					scanf("%c", &ans);
					if (ans == 'Y' || ans == 'y') {
						printf("\nThe member has been deleted successfully.\n");
					}
					else {
						return;
					}

				}
				else {
					fprintf(fptr2, "%s|%c|%s|%s|%s|%d|%d|%d|%s\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);
				}
			}
		}



		yellow();
		printf("\nALERT :\n");
		printf("\nPlease try again if the result that you want haven't came out, it means that your member id is invalid\n");
		resetColor();

		printf("\nDo you want to delete anymore member?\nEnter 'Y' to delete more  :");
		rewind(stdin);
		scanf("%c", &answer);


	} while (answer == 'y' || answer == 'Y');



	if (recCount == 0) {
		printf("0 record been delete.\n");
	}
	else {
		printf("%d records been delete.\n", recCount);
	}


	fclose(fptr1);
	fclose(fptr2);
	remove("member.txt");  		// remove the original file 
	rename("temp.txt", "member.txt");


}

void memberReport() {

	int selection;

	do {
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                        Report\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                  1. Member active\n");
		printf("\t\t\t\t                  2. Member deactive\n");
		printf("\t\t\t\t                  3. Member withdraw\n");
		printf("\t\t\t\t---------------------------------------------------------\n\n");
		printf("\nYour selection : ");
		scanf("%d", &selection);

		switch (selection) {
		case 1:
			system("cls");
			memActive();
			break;

		case 2:
			system("cls");
			memDeactice();
			break;

		case 3:
			system("cls");
			memWithdraw();
			break;


		}
	} while (selection != 1 && selection != 2 && selection != 3);

}

void memActive() {


	struct member info[1000];
	int recFound = 0, recCount = 0;

	FILE* fptr;
	fptr = fopen("member.txt", "r");


	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	printf("Member who active:\n");
	printf("\nMember Name\t\tMember Gender\t  Member Phone Number\t Member ID\t Member Email\t Member Registration Date\tMember Status\n");
	printf("------------------------------------------------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			if (strcmp(info[i].status, "active") == 0) {
				++recCount;
				green();
				printf("%-27s %-13c %-22s %-15s %-22s %d-%d-%-20d %s\n\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);
				resetColor();
			}


		}

		else
			break;
	}

	printf("\n%d members are under active.\n\n", recCount);
	fclose(fptr);
}

void memDeactice() {

	struct member info[1000];
	int recFound = 0, recCount = 0;

	FILE* fptr;
	fptr = fopen("member.txt", "r");


	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	printf("Member who deactive:\n");
	printf("\nMember Name\t\tMember Gender\t  Member Phone Number\t Member ID\t Member Email\t Member Registration Date\tMember Status\n");
	printf("------------------------------------------------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			if (strcmp(info[i].status, "deactive") == 0) {
				++recCount;
				yellow();
				printf("%-27s %-13c %-22s %-15s %-22s %d-%d-%-20d %s\n\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);
				resetColor();
			}


		}

		else
			break;
	}

	printf("\n%d members are under deactive.\n\n", recCount);
	fclose(fptr);
}

void memWithdraw() {

	struct member info[1000];
	int recFound = 0, recCount = 0;

	FILE* fptr;
	fptr = fopen("member.txt", "r");


	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	printf("Member who active:\n");
	printf("\nMember Name\t\tMember Gender\t  Member Phone Number\t Member ID\t Member Email\t Member Registration Date\tMember Status\n");
	printf("------------------------------------------------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			if (strcmp(info[i].status, "withdraw") == 0) {
				++recCount;
				red();
				printf("%-27s %-13c %-22s %-15s %-22s %d-%d-%-20d %s\n\n", info[i].name, info[i].gender, info[i].phoneNum, info[i].id, info[i].email, info[i].time.regDay, info[i].time.regMonth, info[i].time.regYear, info[i].status);
				resetColor();
			}


		}

		else
			break;
	}

	printf("\n%d members are under withdraw.\n\n", recCount);
	fclose(fptr);
}

void checkPhoneNum(int* check1, int* check01, char phoneNum[13]) {

	struct member info[1000];
	int count = 0;

	FILE* fptr;
	fptr = fopen("member.txt", "r");

	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			++count;
		}

		else
			break;
	}

	fclose(fptr);

	for (int i = 0; i < count; i++) { // go through all the ID records in file
		if (strcmp(phoneNum, info[i].phoneNum) == 0) {// if any of the ID match the user input
			++* check1; //check become 1
		}
	}

	if (*check1 > 0) {
		red();
		printf("This phone number already exists.\nPlease insert a new phone number!\n\n");
		resetColor();
	}// if check become 1, print the following messaage


	if (strlen(phoneNum) < 11 || strlen(phoneNum) > 12)
	{
		++* check01;
		red();
		printf("The contact number is invalid. Please Enter Again.\n\n");
		resetColor();
	}

}
