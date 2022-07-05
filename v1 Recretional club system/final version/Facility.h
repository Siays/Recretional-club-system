#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#pragma warning (disable :4996)

// facility module
int FaciModule();
void DisplayFacilities(Facilities faci[], int count);
void SearchFacilities(Facilities faci[], int count);
void SearchFaciByDes(Facilities faci[], int count);
void SearchFaciByType(Facilities faci[], int count);
void SearchFaciByID(Facilities faci[], int count);
int ModifyFacilities(Facilities faci[], int count);
int ModifyFaciByDes(Facilities faci[], int count);
int ModifyFaciByID(Facilities faci[], int count);
int AddFacilities(Facilities faci[], int count, int* updateCount);
int DeleteFacilities(Facilities faci[], int count, int* updateCount);
void FacilityReport(Facilities faci[], int count);
void FaciUnderMaintain(Facilities faci[], int count);
void OperatingFaci(Facilities faci[], int count);
void AllFaciReport(Facilities faci[], int count);
void FaciDescInputConvert(char userInput[]);
void FaciTypeInputConvert(char userInput[]);
void FaciIDValidation(char userInput[], int* patternCount);
void ModifyFaciMaxUser(Facilities faci[], int count, char searchFaci[]);
void ModifyFaciStatus(Facilities faci[], int count, char searchFaci[]);
void ModifyFaciVenue(Facilities faci[], int count, char searchFaci[]);
void readFaci(Facilities faci[], int* faciCount);
// end of facility module 
void red();
void yellow();
void green();
void resetColor();


int FaciModule() {
	Facilities faci[300];
	int faciCount = 0;
	readFaci(faci,&faciCount);

	int selection;


	do {
		system("cls");
		printf("\n\n\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                    FACILITY MODULE\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                   0. Exit Module\n");
		printf("\t\t\t\t                   1. Display All Facility\n");
		printf("\t\t\t\t                   2. Search Facility\n");
		printf("\t\t\t\t                   3. Modify Facility\n");
		printf("\t\t\t\t                   4. Add Facility\n");
		printf("\t\t\t\t                   5. Delete Facility\n");
		printf("\t\t\t\t                   6. View Report\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\nSelection --> ");
		rewind(stdin);
		scanf("%d", &selection);

		if (selection == 0)
			break;

		switch (selection) {
		case 1:
			system("cls");
			DisplayFacilities(faci, faciCount);
			break;

		case 2:
			system("cls");
			SearchFacilities(faci, faciCount);
			break;

		case 3:
			system("cls");
			ModifyFacilities(faci, faciCount);
			break;

		case 4:
			system("cls");
			AddFacilities(faci, faciCount, &faciCount);
			break;

		case 5:
			system("cls");
			DeleteFacilities(faci, faciCount, &faciCount);
			break;

		case 6:
			system("cls");
			FacilityReport(faci, faciCount);
			break;

		default:
			system("cls");
			yellow();
			printf("Invalid selection.\a\n\n");
			resetColor();
			system("pause");
		}
	} while (selection != 0);

	return 0;
}

void readFaci(Facilities faci[], int* faciCount) {
	
	FILE* readFaci;

	readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		printf("Error. Unable to open file.");
		return -1;
		//exit the function if the file failed to open
	}

	for (int i = 0; i < 300; i++) {
		//while i is less than 100,and not end of file then keep reading the file
		//else if i = 99 or end of file, the loop will be break.
		//this is to track how many records currently have in the txt file
		if (!feof(readFaci)) {
			//header
			//Facilities ID|Types|Description|Venue|Maximum allowable users|Status
			fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, &faci[i].maxUsers, faci[i].status);
			++*faciCount;
		}
		else
			//break the loop once all finish reading records. 
			break;
	}
	fclose(readFaci);

	//read the facility module first, so the system is able to compare if the
	// facility ID is exist when doing booking or modify
}
void DisplayFacilities(Facilities faci[], int count) {
	printf("Display all facilities details\n\n");

	printf("Code\tType\t\tDescription\t\t\tVenue\t\tMax user\tCurrent status\n");
	for (int i = 0; i < count; i++) {
		printf("%s\t%-12s\t%-27s\t%-13s\t%8d\t%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
	}
	system("pause");
}

void SearchFacilities(Facilities faci[], int count) {

	int selection;
	do {
		system("cls");
		printf("\n\n\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                    SEARCH FACILITY\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                   0. Exit Function\n");
		printf("\t\t\t\t                   1. Search By Description\n");
		printf("\t\t\t\t                   2. Search By Type\n");
		printf("\t\t\t\t                   3. Search By ID\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\nSelection --> ");
		rewind(stdin);
		scanf("%d", &selection);


		switch (selection) {
		case 0:
			break;

		case 1:
			system("cls");
			SearchFaciByDes(faci, count);
			system("pause");
			break;

		case 2:
			system("cls");
			SearchFaciByType(faci, count);
			system("pause");
			break;

		case 3:
			system("cls");
			SearchFaciByID(faci, count);
			system("pause");
			break;

		default:
			yellow();
			printf("\aInvalid selection.\n");
			resetColor();
			system("pause");
		}
	} while (selection != 0);
}

void SearchFaciByDes(Facilities faci[], int count) {
	char moreRec;
	char searchFaci[31];
	//reset record found = 0 whenever user continue to read another record
	//this is to help user identify the facility searched exist or not.
	do {
		int recordFound = 0;
		system("cls");
		printf("Search by facilities description\n");
		printf("Enter facilities description: "); //search by facilities description
		rewind(stdin);
		gets(searchFaci);
		// this function is to convert the user input to the facilities.txt data pattern
		// refer to the function for more details explanation.
		FaciDescInputConvert(searchFaci);

		for (int i = 0; i < count; i++) {
			if (strcmp(searchFaci, faci[i].details.description) == 0) {
				++recordFound;
				printf("\nRecord found\n");
				printf("ID: %s\nType: %s\nDescription: %s\nVenue: %s\nMax.user allowed: %d\nCurrent status: %s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
			}
		}

		if (recordFound == 0) {
			yellow();
			printf("\n\aNO RECORD FOUND!\n\n");
			resetColor();
			printf("Please make sure your input is an existing facility.\n");
		}

		printf("\nCheck other record?Y = Yes / N = No\n");
		printf("Your selection: ");
		rewind(stdin);
		scanf("%c", &moreRec);
		moreRec = toupper(moreRec);
	} while (moreRec == 'Y');
}

void SearchFaciByType(Facilities faci[], int count) {
	char moreRec;
	char searchFaci[31];
	//reset record found = 0 whenever user continue to read another record
	//this is to help user identify the facility searched exist or not.
	do {
		int recordFound = 0;
		system("cls");
		printf("Search by facilities type\n");
		printf("Enter facilities type: "); //search by facilities description
		rewind(stdin);
		gets(searchFaci);
		// this function is to convert the user input to the facilities.txt data pattern
		// refer to the function for more details explanation.
		FaciTypeInputConvert(searchFaci);

		printf("Code\tType\t\tDescription\t\t\tVenue\t\tMax user\tCurrent status\n");

		for (int i = 0; i < count; i++) {
			if (strcmp(searchFaci, faci[i].details.type) == 0) {
				++recordFound;
				printf("%s\t%-12s\t%-27s\t%-13s\t%8d\t%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
			}
		}

		if (recordFound == 0) {
			yellow();
			printf("\n\a===========================================No record found============================================\n");
			resetColor();
			printf("Please make sure your input is an existing facility type.\n");
		}

		printf("\nCheck other record?Y = Yes / N = No\n");
		printf("Your selection: ");
		rewind(stdin);
		scanf("%c", &moreRec);
		moreRec = toupper(moreRec);
	} while (moreRec == 'Y');
}

void SearchFaciByID(Facilities faci[], int count) {
	char moreRec;
	char searchFaci[31];
	int recordFound = 0, invalidPtrnCnt = 0;
	//reset record found = 0 whenever user continue to read another record
	//this is to help user identify the facility searched exist or not.
	do {
		system("cls");
		do{
		invalidPtrnCnt = 0;
		printf("Search by facility ID\n");
		printf("Enter facilities ID: "); //search by facilities description
		rewind(stdin);
		gets(searchFaci);
		// this function is to convert the user input to the facilities.txt data pattern
		// refer to the function for more details explanation.
		FaciIDValidation(searchFaci, &invalidPtrnCnt);
		if (invalidPtrnCnt > 0) {
			red();
			printf("Invalid ID pattern\n");
			resetColor();
		}

		} while (invalidPtrnCnt > 0);


		for (int i = 0; i < count; i++) {
			if (strcmp(searchFaci, faci[i].ID) == 0) {
				++recordFound;
				printf("\nRecord found\n");
				printf("ID: %s\nType: %s\nDescription: %s\nVenue: %s\nMax.user allowed: %d\nCurrent status: %s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
			}
		}

		if (recordFound == 0) {
			yellow();
			printf("\n\aNO RECORD FOUND!\n\n");
			resetColor();
			printf("Please make sure your input is an existing facility.\n");
		}

		printf("\nCheck other record?Y = Yes / N = No\n");
		printf("Your selection: ");
		rewind(stdin);
		scanf("%c", &moreRec);
		moreRec = toupper(moreRec);
	} while (moreRec == 'Y');
}

int ModifyFacilities(Facilities faci[], int count) {
	int selection;
	do {
		system("cls");
		printf("\n\n\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                    MODIFY FACILITY\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                   0. Exit Function\n");
		printf("\t\t\t\t                   1. Search By Description To Modify\n");
		printf("\t\t\t\t                   2. Search By ID To Modify\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\nSelection --> ");
		rewind(stdin);
		scanf("%d", &selection);


		switch (selection) {
		case 0:
			break;

		case 1:
			system("cls");
			ModifyFaciByDes(faci, count);
			system("pause");
			break;

		case 2:
			system("cls");
			ModifyFaciByID(faci, count);
			system("pause");
			break;

		default:
			yellow();
			printf("\aInvalid selection.\n");
			resetColor();
			system("pause");
		}
	} while (selection != 0);
}

int ModifyFaciByDes(Facilities faci[], int count) {
	char searchFaci[31], moreRec;
	int recordFound = 0;
	// moreRec to get user confirmation to continue edit other record or to exit this function
	// if user input inexisting facility, recordFound remain 0 
	// and prompt user the input is inexisting facility

	printf("Modify Facility\n");
	printf("Enter facilities description: ");//search by facilities description
	rewind(stdin);
	gets(searchFaci);
	FaciDescInputConvert(searchFaci);

	for (int i = 0; i < count; i++) {
		if (strcmp(searchFaci, faci[i].details.description) == 0) {
			printf("\nRecord found\n");
			printf("ID:%s\nType:%s\nDescription:%s\nVenue:%s\nMaximum user allowed:%d\nCurrent status:%s\n\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
			++recordFound;
		}
	}

	int selection;
	//selection of which field to modify (in menu form) 
	char cont;
	//cont is to check if user want to proceed to another modification.

	if (recordFound > 0) {
		do {
		printf("\n\nWhat would you like to modify?\n");
			
		do{
			printf("0. Exit\n1. Maximum allowed people\n2. Facilities Status\n3. Facilities Venue\n");
			printf("Selection: ");
			rewind(stdin);// incase user input character 
			//the system will go into infinite loop
			scanf("%d", &selection);
		} while (selection < 0 || selection > 3);
			switch (selection) {
			case 0:
				break;

			case 1:
				ModifyFaciMaxUser(faci, count, searchFaci);
				break;

			case 2:
				ModifyFaciStatus(faci, count, searchFaci);
				break;

			case 3:
				ModifyFaciVenue(faci, count, searchFaci);
				break;

			default:
				yellow();
				printf("\aInvalid selection!\n");
				resetColor();
			}
			if (selection == 0)
				break;
			// if the selection 0 (to exit) , the rest line shouldn't be executed. 
			printf("\nContinue to modify %s? Y = yes / N = No\n", searchFaci);
			printf("Selection: ");
			rewind(stdin);
			scanf("%c", &cont);
			cont = toupper(cont);
		} while (cont == 'Y');
	}

	else {
		yellow();
		printf("\n\aNO RECORD FOUND!\n\n");
		resetColor();
		printf("Please make sure your input is an existing facility.\n");
	}


	FILE* writePtr;
	writePtr = fopen("facilities.txt", "w");
	if (writePtr == NULL) {
		printf("Error. Unable to open file.\n");
		return -1;
	}
	for (int i = 0; i < count; i++) {
		fprintf(writePtr, "%s|%s|%s|%s|%d|%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
	}
	fclose(writePtr);
	return 0;
}

int ModifyFaciByID(Facilities faci[], int count) {
	char searchFaci[31];
	int recordFound = 0;
	// if user input inexisting facility, recordFound remain 0 
	// and prompt user the input is inexisting facility
	int invalidPtrnCnt = 0;// for id validation

	system("cls");
	do {
		invalidPtrnCnt = 0;
		printf("Search by facility ID\n");
		printf("Enter facilities ID: "); //search by facilities description
		rewind(stdin);
		gets(searchFaci);
		// this function is to convert the user input to the facilities.txt data pattern
		// refer to the function for more details explanation.
		FaciIDValidation(searchFaci, &invalidPtrnCnt);
		if (invalidPtrnCnt > 0) {
			red();
			printf("Invalid ID pattern\n");
			resetColor();
		}

	} while (invalidPtrnCnt > 0);


	for (int i = 0; i < count; i++) {
		if (strcmp(searchFaci, faci[i].ID) == 0) {
			++recordFound;
			printf("\nRecord found\n");
			printf("ID: %s\nType: %s\nDescription: %s\nVenue: %s\nMax.user allowed: %d\nCurrent status: %s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
		}
	}

	if (recordFound == 0) {
		yellow();
		printf("\n\aNO RECORD FOUND!\n\n");
		resetColor();
		printf("Please make sure your input is an existing facility.\n");
	}
	
		int selection;
		//selection of which field to modify (in menu form) 
		char cont;
		//cont is to check if user want to proceed to another modification.

		if (recordFound > 0) {
			printf("\n\nWhat would you like to modify?\n");
			do {
				printf("0. Exit\n1. Maximum allowed people\n2. Facilities Status\n3. Facilities Venue\n");
				printf("Selection: ");
				rewind(stdin);// incase user input character 
				//the system will go into infinite loop
				scanf("%d", &selection);

				switch (selection) {
				case 0:
					break;

				case 1:
					ModifyFaciMaxUser(faci, count, searchFaci);
					break;

				case 2:
					ModifyFaciStatus(faci, count, searchFaci);
					break;

				case 3:
					ModifyFaciVenue(faci, count, searchFaci);

				default:
					yellow();
					printf("\aInvalid selection!\n");
					resetColor();
				}
				if (selection == 0)
					break;
				// if the selection 0 (to exit) , the rest line shouldn't be executed. 
				printf("\nContinue to modify %s? Y = yes / N = No\n", searchFaci);
				printf("Selection: ");
				rewind(stdin);
				scanf("%c", &cont);
				cont = toupper(cont);
			} while (cont == 'Y');
		}

		else {
			yellow();
			printf("\n\aNO RECORD FOUND!\n\n");
			resetColor();
			printf("Please make sure your input is an existing facility.\n");
		}


		FILE* writePtr;
		writePtr = fopen("facilities.txt", "w");
		if (writePtr == NULL) {
			printf("Error. Unable to open file.\n");
			return -1;
		}
		for (int i = 0; i < count; i++) {
			fprintf(writePtr, "%s|%s|%s|%s|%d|%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
		}
		fclose(writePtr);

	return 0;
}

void ModifyFaciMaxUser(Facilities faci[], int count, char searchFaci[]) {
	int tempNewMaxUser; // temporary new maximum people allowed to use a facility.
	// the temporary variable holds the value until the user confirm their modification.
	char confirmation;
	//confirmation is to provide user escapability on their modification.


	for (int i = 0; i < count; i++) {
		if (strcmp(searchFaci, faci[i].details.description) == 0) {
			//disclaimer message.
			printf("\nBefore proceed to modify, please be reminded that the\n");
			red();//important message in red color 
		// to catch user attention.
			printf("new update must strictly follow the SOPs.\n\n");
			resetColor();
			printf("Current maximum user allowed: %d\n", faci[i].maxUsers);
			printf("Confirm to change the maximum user allowed? Y = yes / N = No\n");
			rewind(stdin);
			scanf("%c", &confirmation);
			confirmation = toupper(confirmation);
			if (confirmation == 'Y') {
				do {
					printf("Enter new maximum user allowed: ");
					scanf("%d", &faci[i].maxUsers); //temporary hold the new value

					if (faci[i].maxUsers <= 0) {
						yellow();
						printf("\nInvalid input!\n");
						resetColor();
					}
				} while (faci[i].maxUsers <= 0);
				//upon user confirmation, the new value will replace the old value.
				system("cls");
				printf("Updated maximum user allowed: %d\n", faci[i].maxUsers);
				break;
			}
			else {
				system("cls");
				printf("\nNo change has been made.\n");
				break;
			}
		}
		else if (strcmp(searchFaci, faci[i].ID) == 0) {
			//disclaimer message.
			printf("\nBefore proceed to modify, please be reminded that the\n");
			red();//important message in red color 
		// to catch user attention.
			printf("new update must strictly follow the SOPs.\n\n");
			resetColor();
			printf("Current maximum user allowed: %d\n", faci[i].maxUsers);
			printf("Confirm to change the maximum user allowed? Y = yes / N = No\n");
			rewind(stdin);
			scanf("%c", &confirmation);
			confirmation = toupper(confirmation);
			if (confirmation == 'Y') {
				do {
					printf("Enter new maximum user allowed: ");
					scanf("%d", &faci[i].maxUsers); //temporary hold the new value

					if (faci[i].maxUsers <= 0) {
						yellow();
						printf("\nInvalid input!\n");
						resetColor();
					}
				} while (faci[i].maxUsers <= 0);
				//upon user confirmation, the new value will replace the old value.
				system("cls");
				printf("Updated maximum user allowed: %d\n", faci[i].maxUsers);
				break;
			}

			else {
				system("cls");
				printf("\nNo change has been made.\n");
				break;
			}
		}
	}
}

void ModifyFaciStatus(Facilities faci[], int count, char searchFaci[]) {
	char confirmation;
	//confirmation is to provide user escapability on their modification.
	for (int i = 0; i < count; i++) {
		if (strcmp(searchFaci, faci[i].details.description) == 0) {
			printf("\nCurrent status: %s\n", faci[i].status);

			printf("Confirm to change the stauts? Y = yes / N = No\n");
			printf("Confirmation: ");
			rewind(stdin);
			scanf("%c", &confirmation);
			confirmation = toupper(confirmation);

			if (confirmation == 'Y') {
				//upon user confirmation, if the status currently is Operating, then it will be replace
				// by Maintaining or vice versa. 
				if (strcmp(faci[i].status, "Operating") == 0)
					strcpy(faci[i].status, "Maintaining");
				else if (strcmp(faci[i].status, "Maintaining") == 0)
					strcpy(faci[i].status, "Operating");
				system("cls");
				printf("Updated status: %s\n", faci[i].status);
			}

			else {
				system("cls");
				printf("\nNo change has been made.\n");
			}
		}
		else if (strcmp(searchFaci, faci[i].ID) == 0) {
			printf("\nCurrent status: %s\n", faci[i].status);

			printf("Confirm to change the stauts? Y = yes / N = No\n");
			printf("Confirmation: ");
			rewind(stdin);
			scanf("%c", &confirmation);
			confirmation = toupper(confirmation);

			if (confirmation == 'Y') {
				//upon user confirmation, if the status currently is Operating, then it will be replace
				// by Maintaining or vice versa. 
				if (strcmp(faci[i].status, "Operating") == 0)
					strcpy(faci[i].status, "Maintaining");
				else if (strcmp(faci[i].status, "Maintaining") == 0)
					strcpy(faci[i].status, "Operating");
				system("cls");
				printf("Updated status: %s\n", faci[i].status);
			}

			else {
				system("cls");
				printf("\nNo change has been made.\n");
			}
		}
	}
}

void ModifyFaciVenue(Facilities faci[], int count, char searchFaci[]) {

	char confirmation;
	//confirmation is to provide user escapability on their modification.
	for (int i = 0; i < count; i++) {
		if (strcmp(searchFaci, faci[i].details.description) == 0) {
			printf("\nCurrent venue: %s\n", faci[i].details.venue);

			printf("Confirm to change the venue? Y = yes / N = No\n");
			printf("Confirmation: ");
			rewind(stdin);
			scanf("%c", &confirmation);
			confirmation = toupper(confirmation);

			if (confirmation == 'Y') {
				printf("Enter the new venue: ");
				rewind(stdin);
				gets(faci[i].details.venue);
				FaciDescInputConvert(faci[i].details.venue);

				//faci description data pattern can be similar to venue input, thus 
				//this function can be use to convert user input.
				system("cls");
				printf("Updated venue: %s\n", faci[i].details.venue);
			}

			else {
				system("cls");
				printf("\nNo change has been made.\n");
			}
		}
		else if (strcmp(searchFaci, faci[i].ID) == 0) {
			printf("\nCurrent venue: %s\n", faci[i].details.venue);

			printf("Confirm to change the venue? Y = yes / N = No\n");
			printf("Confirmation: ");
			rewind(stdin);
			scanf("%c", &confirmation);
			confirmation = toupper(confirmation);

			if (confirmation == 'Y') {
				printf("Enter the new venue: ");
				rewind(stdin);
				gets(faci[i].details.venue);
				FaciDescInputConvert(faci[i].details.venue);
				//faci description data pattern can be similar to venue input, thus 
				//this function can be use to convert user input.
				system("cls");
				printf("Updated venue: %s\n", faci[i].details.venue);
			}

			else {
				system("cls");
				printf("\nNo change has been made.\n");
			}
		}
	}

}

int AddFacilities(Facilities faci[], int count, int* updateCount) {
	char addFaci[21];
	int matchCount = 0;
	char a, b;
	//a and b temporary variable to hold ID
	//matchCount is to use in assigning ID
	//for instance, if 4 karaoke rooms were found, then count become 4
	// and if any new karaoke room is added, the ID will start from KA5

	printf("Enter facility type to add: ");
	rewind(stdin);
	gets(addFaci);
	FaciTypeInputConvert(addFaci);

	printf("\nCurrent records:\n");
	printf("Code\tType\t\tDescription\t\t\tVenue\t\tMax user\tCurrent status\n");
	for (int i = 0; i < count; i++) {
		if (strcmp(addFaci, faci[i].details.type) == 0) {
			++matchCount; //add 1 everytime a record is matched.
			a = faci[i].ID[0];//extract the matched facility type of its ID's first character
			b = faci[i].ID[1];//extract the matched facility type of its ID's seoncd character
			// the extration is for automated records usage, which will be clearer later. 
			printf("%s\t%-12s\t%-27s\t%-13s\t%8d\t%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
		}

		if (i == (count - 1) && matchCount == 0) {
			//if the loop go through all record and didn't found any match record.
			//then print no record found.
			yellow();
			printf("\n===========================================No record found============================================\n");
			resetColor();
		}
	}


	char confirmation;
	char tempIDHolder[5], extractDes[21], tempDesHolder[21];
	char manualOrAuto;
	int duplicate = 0;//this is to check first duplicate facility ID / facility description is match
	int incorrectPtrn = 0; // this is to check if user input ID according to data pattern
	int sepcialCase = 0; // this will decide the system will go into auto or manual record

	printf("\n\nConfirm to add new record? Y = Yes / N = No\n");
	printf("Confirmation: ");
	rewind(stdin);
	scanf("%c", &confirmation);
	confirmation = toupper(confirmation);

	if (confirmation == 'Y') {
		// if user want to add new facilities, and any record found previously, process
		// to this condition
		if (matchCount > 0) {
			// form a string which a is the first character of ID
			// and b is the second chacater of ID, for automized ID
			char extractID[3] = { a,b };
			//combine the extractedID and form new facility ID

			sprintf(tempIDHolder, "%s%d", extractID, matchCount + 1);
			for (int i = 0; i < count; i++) {
				if (strcmp(tempIDHolder, faci[i].ID) == 0) {
					++sepcialCase;
				}
			}

			if (sepcialCase > 0) {
				yellow();
				printf("\nSpecial case detected:\n");
				printf("The type of facility inputted contain previously deleted facilities.\n");
				resetColor();
				printf("You will be directed to manual input.\n");
				system("pause");
				manualOrAuto = 'M';
			}
			else {
				red();
				printf("\nRecord will be automatic generate\n");
				printf("If you wish to modify some information\n");
				printf("Please proceed to modification function in the facility main menu\n");
				resetColor();
				system("pause");
				manualOrAuto = 'A';
			}

			if (manualOrAuto == 'A') {
				for (int i = 0; i < count; i++) {
					if (strcmp(addFaci, faci[i].details.type) == 0) {
						strcpy(faci[count].ID, tempIDHolder);
						strcpy(faci[count].details.type, faci[i].details.type);
						for (int j = 0; faci[i].details.description[j] != '\0'; j++) {
							//read each of the character in facilities description.
							//as long as the character is not a digit, then append
							//it to extractDes, else if a digit is met, terminate the 
							//process
							if (isdigit(faci[i].details.description[j]) == 0)
								extractDes[j] = faci[i].details.description[j];
							else
								extractDes[j] = '\0';
							//combine the extracted string together with the match count
							//to form new facilities description (automize the record)
							//For ex: 
							//currently : Badminton Court 4
							//if new record added: Badminton Court 5
						}
						sprintf(tempDesHolder, "%s%d", extractDes, matchCount + 1);
						strcpy(faci[count].details.description, tempDesHolder);
						strcpy(faci[count].details.venue, faci[i].details.venue);
						faci[count].maxUsers = faci[i].maxUsers;
					}
					//setting the facility status to operating whenever a new record is added.
					strcpy(faci[count].status, "Operating");
				}
			}
			else if (manualOrAuto == 'M') {
				printf("\nPlease enter the information according to the ""mental club"" data convention\n");
				printf("Contact the IT team if you have any difficulty in performing this task.\n");
				printf("Ms. Gila: 011-2159 7762 / 012-623 2158\n");
				printf("Mr. Siao: 017-8006625 / 012-317 4275\n");
				printf("Mr. OK: 018-776 1312\n\n");
				char faciID[5];
				do {
					duplicate = 0;
					do {
						incorrectPtrn = 0;
						printf("Enter facilites ID: ");
						rewind(stdin);
						gets(faciID);
						FaciIDValidation(faciID, &incorrectPtrn);

						if (incorrectPtrn > 0) {
							yellow();
							printf("\aInvalid input!\nPlease refer to mental club data convention.\n");
							resetColor();
						}
					} while (incorrectPtrn > 0);
					strcpy(faci[count].ID, faciID);
					//after checking the ID pattern, then proceed to checking id any duplicate ID.
					for (int i = 0; i < count; i++) {
						if (strcmp(faci[count].ID, faci[i].ID) == 0)
							++duplicate;
					}

					if (duplicate > 0) {
						yellow();
						printf("\aInvalid input!\nID already exists.\nDuplicate is not allowed.\n");
						resetColor();
					}
				} while (duplicate > 0);


				do {
					duplicate = 0;
					printf("Enter facilities description: ");
					rewind(stdin);
					gets(faci[count].details.description);
					FaciDescInputConvert(faci[count].details.description);
					//after checking the input pattern, then proceed to checking id any duplicate facility description.
					for (int i = 0; i < count; i++) {
						if (strcmp(faci[count].details.description, faci[i].details.description) == 0)
							++duplicate;
					}

					if (duplicate > 0) {
						yellow();
						printf("\aInvalid input!\n Facilty description already exists.\nDuplicate is not allowed.\n");
						resetColor();
					}
				} while (duplicate > 0);

				// only allow user to manually key in facility ID and facility description
				// the rest should according to the record found.
				for (int i = 0; i < count; i++) {
					if (strcmp(addFaci, faci[i].details.type) == 0) {
						strcpy(faci[count].details.type, addFaci);
						strcpy(faci[count].details.venue, faci[i].details.venue);
						faci[count].maxUsers = faci[i].maxUsers;
					}
				}
				//set the default facilities status as operating whenever
				//a new record is added.
				strcpy(faci[count].status, "Operating");
			}
		}
		//if user want to add new facilities which doesn't exisits(referring to facility type) in the record,
		//user then need to key in everything manually 
		else {
			printf("\nPlease enter the information according to the ""mental club"" data convention\n");
			printf("Contact the IT team if you have any difficulty in performing this task.\n");
			printf("Ms. Gila: 011-2159 7762 / 012-623 2158\n");
			printf("Mr. Siao: 017-8006625 / 012-317 4275\n");
			printf("Mr. OK: 018-776 1312\n\n");

			do {
				duplicate = 0;
				do {
					incorrectPtrn = 0;
					red();
					printf("Please enter 2 alphabets of facilites ID which you think is\n");
					printf("meaningful to represent the new facility type.\n");
					printf("Sequence will be automatically assigned.\n");
					resetColor();
					printf("\nEnter 2 alphabets of facility ID: ");
					rewind(stdin);
					gets(faci[count].ID);
					// if user key in is not exactly 2 characters
					// back to the loop
					if (!(strlen(faci[count].ID) == 2))
						++incorrectPtrn;
					// 2 Uppercase characters + digit, digit will be auto assigned as 1
					// so user is required to input 2 characters only.
					// Since user is inputting a new type of facility,
					// so the facility ID must start from 1
					for (int i = 0; i < 3; i++) {
						// since the id is make up of 2 alphabet and 1 digit
						// thus maximum length is 3, thus set i < 3
						if (!(faci[count].ID[0] >= 'A' && faci[count].ID[0] <= 'z'))
							//if the first character is not an alphabet
							++incorrectPtrn;

						else if (!(faci[count].ID[1] >= 'A' && faci[count].ID[1] <= 'z'))
							//if the second character is not an alphabet
							++incorrectPtrn;

						else if (faci[count].ID[0] >= 'a' && faci[count].ID[0] <= 'z')
							//if the first character is an alphabet but in lowercase
							//convert it to uppercase
							faci[count].ID[0] = faci[count].ID[0] - 32;

						else if (faci[count].ID[1] >= 'a' && faci[count].ID[1] <= 'z')
							//if the second character is an alphabet but in lowercase
							//convert it to uppercase
							faci[count].ID[1] = faci[count].ID[1] - 32;

						faci[count].ID[2] = '1'; //assign 1 to the ID
						faci[count].ID[3] = '\0';//assign end of string to ID
						// Reason: every new type of facility's ID must start from 1
					}

					if (incorrectPtrn > 0) {
						yellow();
						printf("\aInvalid input!\nPlease refer to mental club data convention.\n");
						resetColor();
					}
				} while (incorrectPtrn > 0);
				for (int i = 0; i < count; i++) {
					if (strcmp(faci[count].ID, faci[i].ID) == 0)
						++duplicate;
				}

				if (duplicate > 0) {
					yellow();
					printf("\aInvalid input!\nID already exists.\nDuplicate is not allowed.\n");
					resetColor();
				}
			} while (duplicate > 0);

			//since above will check the user input for facility type, 
			// hence no need checking on this
			strcpy(faci[count].details.type, addFaci);

			do {
				//unable to restrict user input on this, since this will be vary according
				//to facility type, however will still prevent the user from inputting the same as the
				//existing record

				char tempFaciDesc[3] = " 1"; //this is used to append behind the user input
				//use only in this part, thus declare inside here

				red();
				printf("\nEnter new facilities description without its sequence\n");
				printf("Sequence will be automatically assigned.\n");
				resetColor();
				printf("\nEnter new facilities description: ");
				rewind(stdin);
				gets(faci[count].details.description);
				FaciDescInputConvert(faci[count].details.description);

				// as it is a new type of facility it must always start with sequence of 1.
				strcat(faci[count].details.description, tempFaciDesc);

				for (int i = 0; i < count; i++) {
					if (strcmp(faci[count].details.description, faci[i].details.description) == 0)
						++duplicate;
				}
				if (duplicate > 0) {
					yellow();
					printf("\aInvalid input!\n Facilty description already exists.\nDuplicate is not allowed.\n");
					resetColor();
				}
			} while (duplicate > 0);

			//impossible to restrict user input on this as well, as venue can be duplicated,
			//and also possible for new venue to be used.
			printf("Enter new facilities venue: ");
			rewind(stdin);
			gets(faci[count].details.venue);

			do {
			printf("Enter new facilities max. user allowed: ");
			
			scanf("%d", &faci[count].maxUsers);

			if (faci[count].maxUsers <= 0) {
				yellow();
				printf("\nInvalid input!\n");
				resetColor();
			}
			} while (faci[count].maxUsers <= 0);

			//set the default facilities status as operating whenever
			//a new record is added.
			strcpy(faci[count].status, "Operating");
		}
		*updateCount = *updateCount + 1;
	}
	else
		printf("\nNo new record have been added.\n");

	// proceed to write file only if user confirm to add new record
	if (confirmation == 'Y') {
		FILE* writePtr;
		writePtr = fopen("facilities.txt", "w");
		if (writePtr == NULL) {
			printf("Error. Unable to open file.\n");
			return -1;
		}

		for (int i = 0; i < count + 1; i++) {
			fprintf(writePtr, "%s|%s|%s|%s|%d|%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
		}
		fclose(writePtr);
	}

	return 0;

}

int DeleteFacilities(Facilities faci[], int count, int* updateCount) {
	char faciType[21], recordFound = 0;
	yellow();
	printf("WARNING: Delete facility\n");
	resetColor();
	printf("Enter facility type: ");
	rewind(stdin);
	gets(faciType);
	FaciTypeInputConvert(faciType);

	printf("\nCurrent records\n");
	printf("Code\tType\t\tDescription\t\t\tVenue\t\tMax user\tCurrent status\n");
	for (int i = 0; i < count; i++) {
		if (strcmp(faciType, faci[i].details.type) == 0) {
			++recordFound;
			printf("%s\t%-12s\t%-27s\t%-13s\t%8d\t%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
		}
	}

	if (recordFound == 0) {
		yellow();
		printf("\n===========================================No record found============================================\n");
		resetColor();
	}

	else {
		char faciID[5];
		int incorrectPtrn = 0;
		int inexistID = 1;  // this is to check if the user input is existing

		//this loop is to check if the user input is according to the 
		//data convention, and also convert their input 
		// to data convention pattern
		do {
			incorrectPtrn = 0;

			printf("\nEnter the facilities ID to delete: ");
			rewind(stdin);
			gets(faciID);
			FaciIDValidation(faciID, &incorrectPtrn);
			//this loop is to check if the user input is according to the 
			//data convention, and also convert their input 
			// to data convention pattern

			if (incorrectPtrn > 0) {
				yellow();
				printf("\aInvalid input!\nPlease refer to mental club data convention.\n");
				resetColor();
			}
			//if data pattern is correct, then will go into another check.
			else {

				//this loop is to check if user input is match with the
				//facilty type's ID
				// for instance , if the user key in badminton in the facility type
				// ID are start with BT, if user key in anything other than BT 
				// will be rejected
				for (int i = 0; i < count; i++) {
					if (strcmp(faciType, faci[i].details.type) == 0) {
						if (faciID[0] != faci[i].ID[0]) //compare the first chacater
							++incorrectPtrn;
						else if (faciID[1] != faci[i].ID[1])//compare the second character
							++incorrectPtrn;
					}

					if (strcmp(faciID, faci[i].ID) == 0) {
						--inexistID;
					}

				}

				if (incorrectPtrn > 0) {
					yellow();
					printf("\aInvalid input.\n");
					printf("You are deleting something different from your search\n");
					resetColor();
				}
				else if (inexistID > 0) {
					yellow();
					printf("\aInexist ID detected.\n");
					resetColor();
				}
			}
		} while (incorrectPtrn > 0 || inexistID > 0);

		printf("\n\nPlease be informed that deleting the record would affect the facility availability directly,\n");
		red();
		printf("please refrain from doing so without instruction from the authority.\n");
		resetColor();

		char confirmation;
		printf("\nContinue to delete? Y = Yes / N = No\n");
		printf("Your confirmation: ");
		rewind(stdin);
		scanf("%c", &confirmation);
		confirmation = toupper(confirmation);

		int matchCount = 0;
		if (confirmation == 'Y') {
			for (int i = 0; i < count; i++) {
				if (strcmp(faciID, faci[i].ID) == 0) {
					for (int j = i; j < count; j++) {
						faci[j] = faci[j + 1];
					}
				}

			}

			--count;
			*updateCount = *updateCount - 1;
		}

		else {
			printf("\nNo change has been made.\n");
		}
	}
	FILE* writePtr;
	writePtr = fopen("facilities.txt", "w");
	if (writePtr == NULL) {
		printf("Error. Unable to open file.\n");
		return -1;
	}

	for (int i = 0; i < count; i++) {
		fprintf(writePtr, "%s|%s|%s|%s|%d|%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
	}
	fclose(writePtr);

	return 0;
}

void FacilityReport(Facilities faci[], int count) {
	int selection;


	do {
		system("cls");
		printf("\n\n\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                    VIEW REPORT\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                   0. Back to facility main menu\n");
		printf("\t\t\t\t                   1. View all facilities\n");
		printf("\t\t\t\t                   2. View operating facilities\n");
		printf("\t\t\t\t                   3. View maintaining facilities\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\nSelection --> ");
		scanf("%d", &selection);

		switch (selection) {
		case 0:
			break;

		case 1:
			system("cls");
			AllFaciReport(faci, count);
			break;

		case 2:
			system("cls");
			OperatingFaci(faci, count);
			break;

		case 3:
			system("cls");
			FaciUnderMaintain(faci, count);
			break;

		default:
			system("cls");
			yellow();
			printf("\aInvalid selction.\n");
			resetColor();
		}
	} while (selection != 0);

}

void FaciUnderMaintain(Facilities faci[], int count) {
	int recFound = 0;
	SYSTEMTIME t;
	GetLocalTime(&t);

	printf("Facilites under maintaince\n");
	printf("Code\tType\t\tDescription\t\t\tVenue\t\tMax user\tCurrent status\n");
	for (int i = 0; i < count; i++) {
		if (strcmp(faci[i].status, "Maintaining") == 0) {
			++recFound;
			printf("%s\t%-12s\t%-27s\t%-13s\t%8d\t%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
		}
	}
	red();
	printf("\n%d Facilities are under maintainance as of %d/%d/%d\n", recFound, t.wDay, t.wMonth, t.wYear);
	resetColor();
	system("pause");
}

void OperatingFaci(Facilities faci[], int count) {
	int recFound = 0;
	SYSTEMTIME t;
	GetLocalTime(&t);

	printf("Operating facilities\n");
	printf("Code\tType\t\tDescription\t\t\tVenue\t\tMax user\tCurrent status\n");
	for (int i = 0; i < count; i++) {
		if (strcmp(faci[i].status, "Operating") == 0) {
			++recFound;
			printf("%s\t%-12s\t%-27s\t%-13s\t%8d\t%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
		}
	}

	green();
	printf("\n%d Facilities are operating as of %d/%d/%d\n", recFound, t.wDay, t.wMonth, t.wYear);
	resetColor();
	system("pause");
}

void AllFaciReport(Facilities faci[], int count) {
	SYSTEMTIME t;
	GetLocalTime(&t);

	printf("\t\t\t\t\tRecord as of %d/%d/%d\n\n", t.wDay, t.wMonth, t.wYear);
	printf("\t\t\t\t\t    Facilities\n");
	printf("Code\tType\t\tDescription\t\t\tVenue\t\tMax user\tCurrent status\n");
	for (int i = 0; i < count; i++) {
		if (strcmp(faci[i].status, "Operating") == 0) {
			green();
			printf("%s\t%-12s\t%-27s\t%-13s\t%8d\t%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
			resetColor();
		}
		else {
			red();
			printf("%s\t%-12s\t%-27s\t%-13s\t%8d\t%s\n", faci[i].ID, faci[i].details.type, faci[i].details.description, faci[i].details.venue, faci[i].maxUsers, faci[i].status);
			resetColor();
		}

	}
	system("pause");
}

void FaciDescInputConvert(char userInput[]) {
	//whenever user inputting facility description will enter into this validation.
	//Convert user input to data pattern accroding to facilities.txt
	// 3 scenario :
	// 1. if user input all UPPERCASE INPUT
	// 2. if user input all lowercase input
	// 3. if user input mixed uppercase and lowercase and is not according to the data pattern
	for (int i = 0; userInput[i] != '\0'; i++) {
		if (i == 0 && userInput[0] >= 'a' && userInput[0] <= 'z') {
			// change the first character to upper if user input lowercase first character
			userInput[i] = userInput[i] - 32;
		}
		else if (userInput[i - 1] == ' ' && isdigit(userInput[i]) == 0 && (userInput[i] >= 'a' && userInput[i] <= 'z'))
			// as long as the character come after space is not digit,
			// change lowercase alphabet to upper case as long as it is behind a space
			userInput[i] = userInput[i] - 32;


		else if (userInput[i] >= 'A' && userInput[i] <= 'Z') {
			userInput[i] = userInput[i] + 32;// convert all the input to lowercase

			if (i == 0 && userInput[0] >= 'a' && userInput[0] <= 'z') {
				// change the first character to upper
				userInput[i] = userInput[i] - 32;
			}
			else if (userInput[i - 1] == ' ' && isdigit(userInput[i]) == 0 && (userInput[i] >= 'a' && userInput[i] <= 'z'))
				// as long as the character come after space is not digit,
				// change lowercase alphabet to upper case as long as it is behind a space
				userInput[i] = userInput[i] - 32;
		}

		else if (i == 0 && userInput[0] >= 'A' && userInput[0] <= 'Z' && (userInput[(i - 1)] == ' ' && userInput[i] >= 'A' && userInput[i] <= 'Z') && (userInput[(i - 1)] == ' ' && isdigit(userInput[i]) != 0))
			//break the loop if user input extractly same as the record
			break;
	}

}

void FaciTypeInputConvert(char userInput[]) {
	//whenever user inputting facility description will enter into this validation.
	//Convert user input to data pattern accroding to facilities.txt
	// 3 scenario :
	// 1. if user input all UPPERCASE INPUT
	// 2. if user input all lowercase input
	// 3. if user input mixed uppercase and lowercase and is not according to the data pattern
	for (int i = 0; userInput[i] != '\0'; i++) {
		if (i == 0 && userInput[0] >= 'a' && userInput[0] <= 'z') {
			// change the first character to upper if user input lowercase first character
			userInput[i] = userInput[i] - 32;
		}
		else if (userInput[i - 1] == ' ' && isdigit(userInput[i]) == 0 && (userInput[i] >= 'a' && userInput[i] <= 'z'))
			// as long as the character come after space is not digit,
			// change lowercase alphabet to upper case as long as it is behind a space
			userInput[i] = userInput[i] - 32;


		else if (userInput[i] >= 'A' && userInput[i] <= 'Z') {
			userInput[i] = userInput[i] + 32;// convert all the input to lowercase

			if (i == 0 && userInput[0] >= 'a' && userInput[0] <= 'z') {
				// change the first character to upper
				userInput[i] = userInput[i] - 32;
			}
			else if (userInput[i - 1] == ' ' && isdigit(userInput[i]) == 0 && (userInput[i] >= 'a' && userInput[i] <= 'z'))
				// as long as the character come after space is not digit,
				// change lowercase alphabet to upper case as long as it is behind a space
				userInput[i] = userInput[i] - 32;
		}

		else if (i == 0 && userInput[0] >= 'A' && userInput[0] <= 'Z' && (userInput[(i - 1)] == ' ' && userInput[i] >= 'A' && userInput[i] <= 'Z') && (userInput[(i - 1)] == ' ' && isdigit(userInput[i]) != 0))
			//break the loop if user input extractly same as the record
			break;
	}

}

void FaciIDValidation(char userInput[], int* patternCount) {
	for (int i = 0; userInput[i] != '\0'; i++) {
		if ((!(userInput[0] >= 'A' && userInput[0] <= 'z') || !(userInput[1] >= 'A' && userInput[1] <= 'z') || isdigit(userInput[2]) == 0) || isdigit(userInput[3] == 0))
			++* patternCount;
		else if (userInput[0] >= 'a' && userInput[0] <= 'z')
			userInput[0] = userInput[0] - 32;
		else if (userInput[1] >= 'a' && userInput[1] <= 'z')
			userInput[1] = userInput[1] - 32;
	}
}

// end of facility module

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