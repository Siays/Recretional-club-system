#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#pragma warning(disable: 4996)
typedef struct {
	char ID[5];
	char type[16];
	char description[31];
	char venue[16];
	int maxUser;
	char status[12];
}Facility;

typedef struct {
	Facility faci;
	char staffID[7];
	char addDate[26];
}NewFacilityRec;

typedef struct {
	Facility faci;
	char staffID[7];
	char deleteDate[26];
}DeleteFacilityRec;

typedef struct {
	char faciID[5];
	char type[16];
	char description[31];
	char issue[51];
	char staffID[7];
	char dateReport[26];
}FaciIssue;

typedef struct {
	FaciIssue rec;
	char response[16]; //false report    fixed    working on it   postponed
	char responseStaffID[7];
	char responseDate[26];
}FaciIssueRes;


void facilityModuleMain(char loginID[]);
void displayFaciDetails();
void searchFaciMenu();
void searchFaciByID();
void searchFaciByType();
void searchFaciByDesc();
void modifyFaciMenu();
void modifyFaciVenue(char facilityVenue[]);
void modifyFaciMaxUser(int* maxUser);
void modifyFaciStatus(char facilityStatus[]);
void addNewFacility(char loginID[]);
void addExistingTypeFaciAuto(char loginID[]);
void addExistingTypeFaciSpecifiedID(char loginID[]);
void addNewTypeFaci(char loginID[]);
void deleteFacility(char loginID[]);
void reportFaciIssue(char loginID[]);
void reviewFaciIssue(char loginID[]);
void updateFaciIssue(char loginID[]);
//helper functions
void validateFaciID(char input[], int* checkValidation);
void validateFaciType(char input[], int* checkValidation);
void validateFaciDesc(char input[], int* checkValidation);
void extractFacilityDesc(char input[]);
void sortFaciRecord();
void checkFaciIDExist(char input[], int* checkValidation);
void responseStatus(char input[], int option);



void facilityModuleMain(char loginID[]) {
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
		printf("\t\t\t\t\tFacility Module\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit the Module\n");
		printf("\t\t\t1. Display All Facility Details\n");
		printf("\t\t\t2. Search Facility Details\n");
		printf("\t\t\t3. Modify Facility Details\n");
		printf("\t\t\t4. Add New Facility\n");
		printf("\t\t\t5. Delete Facility\n");
		printf("\t\t\t6. Report Facility Issue\n");
		printf("\t\t\t7. Review Facility Issue\n");
		printf("\t\t\t8. Update Facility Issue Status\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\tYour selection(numeric input only): ");
		rewind(stdin);//incase user input non-numeric input. the system will go into infinite loop.
		scanf("%d", &menuSelection);

		switch (menuSelection) {
		case 0:
			printf("\nExit to main menu.\n");
			break;

		case 1:
			displayFaciDetails();
			break;

		case 2:
			searchFaciMenu();
			break;

		case 3:
			if (strcmp(staff.position, "Facility Manager") == 0 || strcmp(staff.position, "Facility Personnel") == 0) {
				modifyFaciMenu();
			}
			else {
				red();
				printf("\t\t\tYou are not allowed to access this function.\a\n");
				resetColor();
				system("pause");
			}
			break;

		case 4:
			if (strcmp(staff.position, "Facility Manager") == 0 || strcmp(staff.position, "Facility Personnel") == 0) {
				addNewFacility(loginID);
			}
			else {
				red();
				printf("\t\t\tYou are not allowed to access this function.\a\n");
				resetColor();
				system("pause");
			}
			break;

		case 5:
			if (strcmp(staff.position, "Facility Manager") == 0) {
				deleteFacility(loginID);
			}
			else {
				red();
				printf("\t\t\tYou are not allowed to access this function.\a\n");
				resetColor();
				system("pause");
			}
			break;

		case 6:
			reportFaciIssue(loginID);
			break;

		case 7:
			if ((strcmp(staff.position, "Facility Manager") == 0) || (strcmp(staff.position, "Facility Personnel") == 0)){
			reviewFaciIssue(loginID);
			}
			else {
				red();
				printf("\t\t\tYou are not allowed to access this function.\a\n");
				resetColor();
				system("pause");
			}
			break;

		case 8:
			if ((strcmp(staff.position, "Facility Manager") == 0) || (strcmp(staff.position, "Facility Personnel") == 0)) {
				updateFaciIssue(loginID);
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

		//displayFaciDetails();
		//searchFaciByID();
		//searchFaciByType();
		//searchFaciByDesc();
		//searchFaciMenu();
		//modifyFaciMenu();
		//addNewFacility();
		//addExistingTypeFaciAuto();
		//addExistingTypeFaciSpecifiedID();
		//addNewFacility();
		//deleteFacility();
}

void displayFaciDetails() {
	system("cls");
	Facility facility;
	FILE* readFaci;
	readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
			red();
			printf("Failed to open file.");
			resetColor();
			return;
	}
	printf("Faclity details\n");
	printf("ID\tType\t\tDescription\t\t\tVenue\t\tMax.User\tStatus\n");
	printf("--------------------------------------------------------------------------------------------------\n");
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n",facility.ID,facility.type,facility.description,facility.venue,&facility.maxUser,facility.status);
		printf("%-5s\t%-15s\t%-30s\t%s\t%5d\t\t%-12s\n", facility.ID, facility.type, facility.description, facility.venue, facility.maxUser, facility.status);
	}
	printf("--------------------------------------------------------------------------------------------------\n");
	system("pause");
	fclose(readFaci);
}

void searchFaciMenu() {
	int menuSelection;
	do {
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\Search Facility\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit search\n");
		printf("\t\t\t1. Search By ID\n");
		printf("\t\t\t2. Search By Type\n");
		printf("\t\t\t3. Search By Description\n");
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
			searchFaciByID();
			break;

		case 2:
			searchFaciByType();
			break;

		case 3:
			searchFaciByDesc();
			break;

		default:
			red();
			printf("Invalid selection\a\n");
			resetColor();
			system("pause");
		}
	} while (menuSelection != 0);
}

void searchFaciByID() {
	system("cls");
	Facility facility;
	FILE* readFaci;
	readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	char faciID[5]; 
	int validationCheck;
	do{
		validationCheck = 0;
		printf("Enter facility ID: ");
		rewind(stdin);
		fgets(faciID, 5, stdin);
		deleteNewLine(faciID); //fgets will end user input with '\n' , delete the \n and replace '\0' instead.
		validateFaciID(faciID, &validationCheck);
	} while (validationCheck > 0);

	int recordFound = 0;
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
		if (strcmp(facility.ID, faciID) == 0) {
			++recordFound;
			break;
		}
	}
	fclose(readFaci);

	if (recordFound == 0) {
		red();
		printf("\aNo matching facility ID found\n");
		resetColor();
	}
	else{
		printf("\nFacility details\n");
		printf("ID: %s\nType: %s\nDescription: %s\nVenue: %s\nMax.User: %d\nStatus: %s\n", facility.ID, facility.type, facility.description, facility.venue, facility.maxUser, facility.status);
	}
	system("pause");
}

void searchFaciByType() {
	system("cls");
	Facility facility;
	FILE* readFaci;
	readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	char faciType[16];
	int checkValidation;
	do{
		checkValidation = 0;
		printf("Enter facility type: ");
		rewind(stdin);
		fgets(faciType, 16, stdin);
		deleteNewLine(faciType);
		validateFaciType(faciType,&checkValidation);
	} while (checkValidation != 0);
	int recordFound = 0;
	printf("ID\tType\t\tDescription\t\t\tVenue\t\tMax.User\tStatus\n");
	printf("--------------------------------------------------------------------------------------------------\n");
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
		if (strcmp(facility.type, faciType) == 0) {
			++recordFound;
			printf("%-5s\t%-15s\t%-30s\t%s\t%5d\t\t%-12s\n", facility.ID, facility.type, facility.description, facility.venue, facility.maxUser, facility.status);
		}
	}

	if (recordFound == 0) {
		red();
		printf("\aNo matching facility type found\n");
		resetColor();
	}
	printf("--------------------------------------------------------------------------------------------------\n");
	system("pause");
	fclose(readFaci);
}

void searchFaciByDesc() {
	system("cls");
	Facility facility;
	FILE* readFaci;
	readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	char faciDesc[31];
	int validationCheck;
	do {
		validationCheck = 0;
		printf("Enter facility description: ");
		rewind(stdin);
		fgets(faciDesc, 31, stdin);
		deleteNewLine(faciDesc); //fgets will end user input with '\n' , delete the \n and replace '\0' instead.
		validateFaciDesc(faciDesc, &validationCheck);
	} while (validationCheck > 0);

	int recordFound = 0;
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
		if (strcmp(facility.description, faciDesc) == 0) {
			++recordFound;
			break;
		}
	}
	fclose(readFaci);

	if (recordFound == 0) {
		red();
		printf("\aNo matching facility description found\n");
		resetColor();
	}
	else {
		printf("\nFacility details\n");
		printf("ID: %s\nType: %s\nDescription: %s\nVenue: %s\nMax.User: %d\nStatus: %s\n", facility.ID, facility.type, facility.description, facility.venue, facility.maxUser, facility.status);;
	}
	system("pause");
}

void modifyFaciMenu(){
	system("cls");
	Facility facility, tempFacility;
	FILE* readFaci, *writeFaci;
	readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	printf("Faclity details\n");
	printf("ID\tType\t\tDescription\t\t\tVenue\t\tMax.User\tStatus\n");
	printf("--------------------------------------------------------------------------------------------------\n");
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
		printf("%-5s\t%-15s\t%-30s\t%s\t%5d\t\t%-12s\n", facility.ID, facility.type, facility.description, facility.venue, facility.maxUser, facility.status);
	}
	printf("--------------------------------------------------------------------------------------------------\n");
	fclose(readFaci);


	char faciID[5];
	int checkValidation;
	do{
		checkValidation=0;
		printf("\nEnter the facility ID you wish to modify: ");
		rewind(stdin);
		fgets(faciID, 5, stdin);
		deleteNewLine(faciID);
		validateFaciID(faciID, &checkValidation);
	} while (checkValidation > 0);


	readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
		if (strcmp(facility.ID, faciID) == 0) {
			tempFacility = facility;
			break;
		}
	}
	fclose(readFaci);

	system("cls");
	int menuSelection;
	do {
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\Modify Facility\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit modification\n");
		printf("\t\t\t1. Modify Facility Venue\n");
		printf("\t\t\t2. Modify Facility Max.User Allowed\n");
		printf("\t\t\t3. Modify Facility Status\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\tYour selection(numeric input only): ");
		rewind(stdin);//incase user input non-numeric input. the system will go into infinite loop.
		scanf("%d", &menuSelection);
		switch (menuSelection) {
		case 0:
			break;

		case 1:
			modifyFaciVenue(tempFacility.venue);
			break;

		case 2:
			modifyFaciMaxUser(&tempFacility.maxUser);
			break;

		case 3:
			modifyFaciStatus(tempFacility.status);
			break;

		default:
			red();
			printf("Invalid selection\a\n");
			resetColor();
			system("pause");
		}
	} while (menuSelection != 0);

	readFaci = fopen("facilities.txt", "r");
	writeFaci = fopen("tempFaci.txt","w");
	if (readFaci == NULL || writeFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
		if (strcmp(facility.ID, faciID) == 0) {
			fprintf(writeFaci,"%s|%s|%s|%s|%d|%s\n", tempFacility.ID, tempFacility.type, tempFacility.description, tempFacility.venue, tempFacility.maxUser, tempFacility.status);
		}
		else {
			fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", facility.ID, facility.type, facility.description, facility.venue, facility.maxUser, facility.status);
		}
	}
	fclose(writeFaci);
	fclose(readFaci);

	remove("facilities.txt");
	rename("tempFaci.txt","facilities.txt");
}

void modifyFaciVenue(char facilityVenue[]) {
	system("cls");

	printf("Modify Facility Venue\n");
	printf("\nCurrent available venue: \n1. Sport Complex\n2. Leisure Center\n");
	if(strcmp(facilityVenue,"Sport Complex") == 0){
		printf("Venue before modification: %s\n", facilityVenue);
		printf("Venue after modification: Leisure Center\n");
	}
	else {
		printf("Venue before modification: %s\n", facilityVenue);
		printf("Venue after modification: Sport Complex\n");
	}
	char confirm;
	printf("\nConfirm to make the modification? [Y=yes, any other key to exit]: ");
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		if (strcmp(facilityVenue, "Sport Complex") == 0) {
			strcpy(facilityVenue, "Leisure Center");
		}
		else {
			strcpy(facilityVenue, "Sport Complex");
		}
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

void modifyFaciMaxUser(int *maxUser) {
	system("cls");

	printf("Modify Facility Max.User Allowed\n");
	printf("Current Max.User Allowed: %d\n", *maxUser);
	char confirm;
	printf("\nConfirm modification is needed? [Y=yes, any other key to exit]: ");
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		int tempMaxUser;
		do{
			printf("\nEnter the new max.user allowed: ");
			rewind(stdin); //incase user input alphabet, the system will go into infinite loop
			scanf("%d", &tempMaxUser);
			if (tempMaxUser < 1) {
				red();
				printf("\aIllogical Max.User\n");
				resetColor();
			}
		} while (tempMaxUser < 1); // if 0 mean the facility is under maintenance
		*maxUser = tempMaxUser;
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

void modifyFaciStatus(char facilityStatus[]) {
	system("cls");

	printf("Modify Facility Status\n");
	if (strcmp(facilityStatus, "Operating") == 0) {
		printf("Status before modification: %s\n", facilityStatus);
		printf("Status after modification: Maintaining\n");
	}
	else {
		printf("Status before modification: %s\n", facilityStatus);
		printf("Status after modification: Operating\n");
	}
	char confirm;
	printf("\nConfirm to make the modification? [Y=yes, any other key to exit]: ");
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		if (strcmp(facilityStatus, "Operating") == 0) {
			strcpy(facilityStatus, "Maintaining");
		}
		else {
			strcpy(facilityStatus, "Operating");
		}
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

void addNewFacility(char loginID[]) {
	int selection;
	do{
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\tAdd Facility\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit\n");
		printf("\t\t\t1. Add existing type of facility(Auto)\n");
		printf("\t\t\t2. Add existing type of facility(Specified ID)\n");
		printf("\t\t\t3. Add new type of facility\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\tYour selection: ");
		rewind(stdin);
		scanf("%d", &selection);
		
		switch (selection) {
		case 0:
			break;

		case 1:
			addExistingTypeFaciAuto(loginID);
			break;
		
		case 2:
			addExistingTypeFaciSpecifiedID(loginID);
			break;

		case 3:
			addNewTypeFaci(loginID);
			break;

		default:
			red();
			printf("\t\t\t\aInvalid selection\n");
			resetColor();
			system("pause");
		}
	} while (selection != 0);
	sortFaciRecord();//sort the facilities.txt everytime a new facility is added.
}

void addExistingTypeFaciAuto(char loginID[]) {
	int checkValidation;
	char faciType[16];
	do {
		checkValidation = 0;
		system("cls");
		printf("Enter the facility type: ");
		rewind(stdin);
		fgets(faciType, 16, stdin);
		deleteNewLine(faciType);
		validateFaciType(faciType,&checkValidation);
	} while (checkValidation != 0);

	Facility facility[100];
	FILE* readFaci = fopen("facilities.txt","r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	int totalCount = 0;//number of records
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility[totalCount].ID, facility[totalCount].type, facility[totalCount].description, facility[totalCount].venue, &facility[totalCount].maxUser, facility[totalCount].status);
		++totalCount;
	}
	fclose(readFaci);

	int matchTypeCount = 0;//number of facility type that match user input
	int fillMissingSequence = 0; // if the system found missing sequence, for instance BT1 , BT3, where BT2 is missing,
	//the system will prompt user either to use the missing sequence as new ID. User will be prompted only once upon first missing sequence.
	//user should proceed to manual data adding if they wish to add specific, but not first missing sequence.
	char tempID[5];//user to hold the new ID 
	int filledMissingValue = 0;//user to check if user agree to use the missing value. 0 mean rejected or there is no missing sequence.
	Facility tempFaci, newFaci;//user to hold the last record of matching type, if user reject to user miss sequence as ID
	for (int i = 0; i < totalCount; i++) {
		if(strcmp(facility[i].type, faciType)==0){
			++matchTypeCount;
			// the function will extract the ID sequence, for instace, BT1 will extract 1. 
			if (matchTypeCount != extractCurrentIDNo(facility[i].ID)) {
				if(fillMissingSequence == 0){
					red();
					printf("\aMissing sequence detected\n");
					resetColor();
					++fillMissingSequence;
					char extractIDAlph[3] = {facility[i].ID[0],facility[i].ID[1]};
					if (matchTypeCount < 10) {
					char tempMissingID[4];
					sprintf(tempMissingID,"%s%d",extractIDAlph, matchTypeCount);
					strcpy(tempID, tempMissingID);
				}
					else {
					char tempMissingID[5];
					sprintf(tempMissingID,"%s%d",extractIDAlph, matchTypeCount);
					strcpy(tempID, tempMissingID);
				}

					printf("Use the ID: %s as the new facility ID?\n",tempID);
					printf("Press 'Y' to use it, other key to reject\n");
					char confirm;
					confirm = getch();
					confirm = toupper(confirm);
					if (confirm == 'Y') {
						++filledMissingValue;
						strcpy(newFaci.ID, tempID);
						printf("\nThe new facility ID will be: %s", newFaci.ID);
						break;
					}
				}
			}
			tempFaci = facility[i];
		}
	}
	if (matchTypeCount == 0) {
		red();
		printf("\aThe facility type you entered doesn't exist\n");
		printf("\aPlease proceed to add new type of facility\n");
		resetColor();
		system("pause");
		return;
	}
	if (filledMissingValue == 0) {//if user reject to use the missing sequence as ID, or there is no missing sequence
		int extractLastSequence = extractCurrentIDNo(tempFaci.ID);
		int newSequence = extractLastSequence + 1;
		char extractAlph[3] = {tempFaci.ID[0], tempFaci.ID[1]}; // extract the alphabets of the faci ID
		sprintf(tempID, "%s%d", extractAlph, newSequence);//concatenate the alphabets and sequence
		strcpy(newFaci.ID, tempID);
		printf("\nThe new facility ID will be: %s", newFaci.ID);
	}

	printf("\n\nThe data will be generated automatically based on previous record\n");
	printf("Please proceed to modification if any change is needed\n");
	system("pause");
	system("cls");
	strcpy(newFaci.type,tempFaci.type);
	int newEntrySequence = extractCurrentIDNo(newFaci.ID);
	extractFacilityDesc(tempFaci.description);//extract the description from previous data, but delete the sequence number.
	sprintf(newFaci.description, "%s %d", tempFaci.description, newEntrySequence);//concatenate the description and the new sequence
	strcpy(newFaci.venue, tempFaci.venue);
	newFaci.maxUser = tempFaci.maxUser;
	strcpy(newFaci.status, "Operating");

	printf("New facility details\n");
	printf("ID: %s\nType: %s\nDescription: %s\nVenue: %s\nMax.User Allowed: %d\nStatus: %s\n", newFaci.ID,newFaci.type,newFaci.description,newFaci.venue,newFaci.maxUser,newFaci.status);
	printf("\n\nAdd the new record?\n");
	printf("Press 'Y' to add it, other key to cancel\n");
	char confirm;
	confirm = getch();
	confirm = toupper(confirm);
	
	if (confirm == 'Y') {
		FILE* writeFaci = fopen("facilities.txt", "w");
		FILE* writeRec = fopen("newFacilitiesRec","a");
		NewFacilityRec newFaciRec;
		time_t t;
		time(&t);
		if (writeFaci == NULL || writeRec == NULL) {
			red();
			printf("Failed to open file.");
			resetColor();
			return;
		}
		for (int i = 0; i < totalCount; i++) {
			if (strcmp(facility[i].ID, tempFaci.ID) == 0) {
				//tempFaci will always be newFaci.ID - 1. append the new facility right after it.
				fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", facility[i].ID, facility[i].type, facility[i].description, facility[i].venue, facility[i].maxUser, facility[i].status);
				fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", newFaci.ID, newFaci.type, newFaci.description, newFaci.venue, newFaci.maxUser, newFaci.status);
				newFaciRec.faci = newFaci;
				strcpy(newFaciRec.staffID, loginID);
				strcpy(newFaciRec.addDate, ctime(&t));
				deleteNewLine(newFaciRec.addDate);
				fprintf(writeRec,"%s|%s|%s|%s|%d|%s|%s|%s\n", newFaciRec.faci.ID, newFaciRec.faci.type, newFaciRec.faci.description, newFaciRec.faci.venue, newFaciRec.faci.maxUser, newFaciRec.faci.status, newFaciRec.staffID, newFaciRec.addDate);
			}
			else {
				fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", facility[i].ID, facility[i].type, facility[i].description, facility[i].venue, facility[i].maxUser, facility[i].status);
			}
		}
		fclose(writeFaci);
		fclose(writeRec);
		red();
		printf("\n\n\aSuccessfully appended the new facility\n");
		resetColor();
	}
	else {
		red();
		printf("\n\nNo change has been made\a");
		resetColor();
	}
	system("pause");
}

void addExistingTypeFaciSpecifiedID(char loginID[]) {
	int checkValidation;
	char faciType[16];
	do {
		checkValidation = 0;
		system("cls");
		printf("Enter the facility type: ");
		rewind(stdin);
		fgets(faciType, 16, stdin);
		deleteNewLine(faciType);
		validateFaciType(faciType, &checkValidation);
	} while (checkValidation != 0);

	Facility facility[100];
	FILE* readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	int totalCount = 0;//number of records
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility[totalCount].ID, facility[totalCount].type, facility[totalCount].description, facility[totalCount].venue, &facility[totalCount].maxUser, facility[totalCount].status);
		++totalCount;
	}
	fclose(readFaci);

	Facility tempFaci;//this is to identify if user key in the appropriate ID which should match with the facility type
	int matchTypeCount = 0, missingSequence = 0;
	for (int i = 0; i < totalCount; i++) {
		if (strcmp(facility[i].type, faciType) == 0) {
			++matchTypeCount;
			if (matchTypeCount != extractCurrentIDNo(facility[i].ID)) {
				//if there is no missing sequence, ask user to go to auto generate data
				++missingSequence;
			}
			tempFaci = facility[i];
		}
	}

	if (matchTypeCount == 0) {
		red();
		printf("\aThe facility type you entered doesn't exist\n");
		printf("\aPlease proceed to add new type of facility\n");
		resetColor();
		system("pause");
		return;
	}

	if (missingSequence == 0) {
		red();
		printf("\aThere is no missing sequence detected\n");
		printf("Please proceed to auto generate data\n");
		resetColor();
		system("pause");
		return;
	}
	Facility newFaci;
	do {
		checkValidation = 0;
		printf("Enter new facility ID: ");
		rewind(stdin);
		fgets(newFaci.ID, 5, stdin);
		deleteNewLine(newFaci.ID);
		validateFaciID(newFaci.ID, &checkValidation);
		if (checkValidation == 0) {
			if ((newFaci.ID[0] != tempFaci.ID[0]) || (newFaci.ID[1] != tempFaci.ID[1])) {
				//if user input ID doesn't match with the facility type's ID inputted
				++checkValidation;
				red();
				printf("\aID inputted doesn't match the facility's type ID\n");
				resetColor();
				printf("Expected ID should start with %c%c\n", tempFaci.ID[0], tempFaci.ID[1]);
				system("pause");
			}
		}
		if (checkValidation == 0) {
			for (int i = 0; i < totalCount; i++) {
				if (strcmp(facility[i].type, faciType) == 0) {//extract the matched faciType
					if (strcmp(newFaci.ID, facility[i].ID) == 0) {//compare the ID, if duplicated found, add the validation
						++checkValidation;
						red();
						printf("\aDuplicated facility ID detected\n");
						resetColor();
						system("pause");
					}
				}
			}
		}
	} while (checkValidation != 0);

	int newFaciIDSequence = extractCurrentIDNo(newFaci.ID);
	int targetSequence;
	char extractIDAlphabets[3] = { newFaci.ID[0],newFaci.ID[1] };
	char lastIDSequenceBeforeNew[5];

	for (int i = 0; i < totalCount; i++) {
		if (strcmp(facility[i].type, faciType) == 0) {
			int currentSequence = extractCurrentIDNo(facility[i].ID);
			if (currentSequence < newFaciIDSequence) {
				targetSequence = currentSequence;
			}
			else {
				break;
			}
		}
	}
	sprintf(lastIDSequenceBeforeNew, "%s%d", extractIDAlphabets, targetSequence); //new record will append right after this ID
	
	printf("\n\nThe data will be generated automatically based on previous record\n");
	printf("Please proceed to modification if any change is needed\n");
	system("pause");
	system("cls");
	strcpy(newFaci.type, tempFaci.type);
	int newEntrySequence = extractCurrentIDNo(newFaci.ID);
	extractFacilityDesc(tempFaci.description);//extract the description from previous data, but delete the sequence number.
	sprintf(newFaci.description, "%s %d", tempFaci.description, newEntrySequence);//concatenate the description and the new sequence
	strcpy(newFaci.venue, tempFaci.venue);
	newFaci.maxUser = tempFaci.maxUser;
	strcpy(newFaci.status, "Operating");

	printf("New facility details\n");
	printf("ID: %s\nType: %s\nDescription: %s\nVenue: %s\nMax.User Allowed: %d\nStatus: %s\n", newFaci.ID, newFaci.type, newFaci.description, newFaci.venue, newFaci.maxUser, newFaci.status);
	printf("\n\nAdd the new record?\n");
	printf("Press 'Y' to add it, other key to cancel\n");
	char confirm;
	confirm = getch();
	confirm = toupper(confirm);
	
	if (confirm == 'Y') {
		FILE* writeFaci = fopen("facilities.txt", "w");
		FILE* writeRec = fopen("newFacilitiesRec", "a");
		NewFacilityRec newFaciRec;
		time_t t;
		time(&t);
		if (writeFaci == NULL || writeRec == NULL) {
			red();
			printf("Failed to open file.");
			resetColor();
			return;
		}
		for (int i = 0; i < totalCount; i++) {
			if (strcmp(facility[i].ID, lastIDSequenceBeforeNew) == 0) {
				//tempFaci will always be newFaci.ID - 1. append the new facility right after it.
				fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", facility[i].ID, facility[i].type, facility[i].description, facility[i].venue, facility[i].maxUser, facility[i].status);
				fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", newFaci.ID, newFaci.type, newFaci.description, newFaci.venue, newFaci.maxUser, newFaci.status);
				newFaciRec.faci = newFaci;
				strcpy(newFaciRec.staffID, loginID);
				strcpy(newFaciRec.addDate, ctime(&t));
				deleteNewLine(newFaciRec.addDate);
				fprintf(writeRec, "%s|%s|%s|%s|%d|%s|%s|%s\n", newFaciRec.faci.ID, newFaciRec.faci.type, newFaciRec.faci.description, newFaciRec.faci.venue, newFaciRec.faci.maxUser, newFaciRec.faci.status, newFaciRec.staffID, newFaciRec.addDate);
			}
			else {
				fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", facility[i].ID, facility[i].type, facility[i].description, facility[i].venue, facility[i].maxUser, facility[i].status);
			}
		}
		fclose(writeFaci);
		fclose(writeRec);
		red();
		printf("\n\n\aSuccessfully appended the new facility\n");
		resetColor();
	}
	else {
		red();
		printf("\n\nNo change has been made\a");
		resetColor();
	}
	system("pause");
}

void addNewTypeFaci(char loginID[]) {
	Facility facility[100];
	FILE* readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	int totalCount = 0;//number of records
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility[totalCount].ID, facility[totalCount].type, facility[totalCount].description, facility[totalCount].venue, &facility[totalCount].maxUser, facility[totalCount].status);
		++totalCount;
	}
	fclose(readFaci);
	
	Facility newFaci;
	int checkValidation;
	do {
		checkValidation = 0;
		system("cls");
		red();
		printf("Please use 2 alphabets that you think is meaningful abbreviation\n");
		printf("to represent the new type of facility\n");
		resetColor();
		printf("Enter the facility ID: ");
		rewind(stdin);
		fgets(newFaci.ID, 5, stdin);
		deleteNewLine(newFaci.ID);
		validateFaciID(newFaci.ID, &checkValidation);
		if(checkValidation == 0){
			if (newFaci.ID[2] != 49) {
			++checkValidation;
			red();
			printf("\aNew type of facility ID must start with 1\n");
			resetColor();
			system("pause");
		}
		}
		if(checkValidation == 0){
			for (int i = 0; i < totalCount; i++) {
				if ((newFaci.ID[0] == facility[i].ID[0]) && (newFaci.ID[1] == facility[i].ID[1])) {
					++checkValidation;
					red();
					printf("\aThe abbreviation already used by other facility type\n");
					printf("Consider other abbreviation\n");
					resetColor();		
					system("pause");
					break;
				}
			}
		}
	} while (checkValidation != 0);

	do {
		checkValidation = 0;
		printf("\nEnter the facility type: ");
		rewind(stdin);
		fgets(newFaci.type, 16,stdin);
		deleteNewLine(newFaci.type);
		validateFaciType(newFaci.type,&checkValidation);
	} while (checkValidation != 0);

	do {
		char tempFaciDesc[31];
		checkValidation = 0;
		printf("\nEnter the facility description: ");
		rewind(stdin);
		fgets(tempFaciDesc, 31, stdin);
		deleteNewLine(tempFaciDesc);
		validateFaciDesc(tempFaciDesc, &checkValidation);
		if (checkValidation == 0) {
			if (tempFaciDesc[strlen(tempFaciDesc) - 1] != 49) {
				red();
				printf("\aNew type of facility's description sequence must start with 1\n");
				printf("Auto correction will be made\n");
				resetColor();
				extractFacilityDesc(tempFaciDesc);
				sprintf(newFaci.description,"%s %d",tempFaciDesc,1);
				printf("Corrected facility description: %s\n", newFaci.description);
				system("pause");
			}
			else {
				strcpy(newFaci.description, tempFaciDesc);
			}
		}
	}while (checkValidation !=0);
	
	int venueSelection;
	do{
		checkValidation = 0;
		printf("\n\nCurrent available venue: \n1. Sport Complex\n2. Leisure Center\n");
		printf("Key in the assigned digit to set the new facility venue: ");
		rewind(stdin);
		scanf("%d", &venueSelection);
		if (!(venueSelection >= 1 && venueSelection <= 2)) {
			++checkValidation;
			red();
			printf("\aInvalid input! Please key in as the available option\n");
			resetColor();
		}
		if (checkValidation == 0) {
			if (venueSelection == 1) {
				strcpy(newFaci.venue,"Sport Complex");
			}
			else {
				strcpy(newFaci.venue, "Leisure Center");
			}
		}
	} while (checkValidation != 0);

	do {
		checkValidation = 0;
		printf("\nEnter Maximum user allowed for the new facility: ");
		rewind(stdin);
		scanf("%d",&newFaci.maxUser);
		if (newFaci.maxUser <= 0) {
			++checkValidation;
			red();
			printf("\aIllogical number of user inputted\n");
			resetColor();
			system("pause");
		}
	} while (checkValidation != 0);

	strcpy(newFaci.status,"Operating");

	system("cls");
	printf("New facility details\n");
	printf("ID: %s\nType: %s\nDescription: %s\nVenue: %s\nMax.User Allowed: %d\nStatus: %s\n", newFaci.ID, newFaci.type, newFaci.description, newFaci.venue, newFaci.maxUser, newFaci.status);
	printf("\n\nAdd the new record?\n");
	printf("Press 'Y' to add it, other key to cancel\n");
	char confirm;
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		FILE* writeFaci = fopen("facilities.txt","a");
		FILE* writeRec = fopen("newFacilitiesRec", "a");
		NewFacilityRec newFaciRec;
		time_t t;
		time(&t);
		if (writeFaci == NULL || writeRec == NULL) {
			red();
			printf("Failed to open file.");
			resetColor();
			return;
		}
		fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", newFaci.ID, newFaci.type, newFaci.description, newFaci.venue, newFaci.maxUser, newFaci.status);
		newFaciRec.faci = newFaci;
		strcpy(newFaciRec.staffID, loginID);
		strcpy(newFaciRec.addDate, ctime(&t));
		deleteNewLine(newFaciRec.addDate);

		fprintf(writeRec, "%s|%s|%s|%s|%d|%s|%s|%s\n", newFaciRec.faci.ID, newFaciRec.faci.type, newFaciRec.faci.description, newFaciRec.faci.venue, newFaciRec.faci.maxUser, newFaciRec.faci.status, newFaciRec.staffID, newFaciRec.addDate);
		fclose(writeFaci);
		fclose(writeRec);
		red();
		printf("\n\n\aSuccessfully appended the new facility\n");
		resetColor();
	}
	else {
		red();
		printf("\n\nNo change has been made\a");
		resetColor();
	}
	system("pause");
}

void deleteFacility(char loginID[]) {
	system("cls");
	red();
	printf("\aThis function is deleting a selected facility\n");
	printf("Please refrain from doing so without an instruction\n");
	printf("Deletion of a facility will directly delete the data and change its availbility\n");
	resetColor();

	printf("\n\nPress 'Y' to proceed, other key to exit\n");
	char confirm;
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm != 'Y') {
		printf("Returning to facility main menu\n");
		system("pause");
		return;
	}
	system("cls");
	int checkValidation;
	char tempID[5];
	do{
		system("cls");
		checkValidation = 0;
		printf("Delete facility\n");
		printf("Enter the facility ID you wish to delete: ");
		rewind(stdin);
		fgets(tempID,5,stdin);
		deleteNewLine(tempID);
		validateFaciID(tempID, &checkValidation);
		if (checkValidation == 0) {
			checkFaciIDExist(tempID,&checkValidation);
		}
	} while (checkValidation != 0);

	Facility facility;
	FILE* readFaci;
	FILE* writeFaci;
	readFaci = fopen("facilities.txt", "r");
	writeFaci = fopen("tempFaci.txt","w");
	if (readFaci == NULL || writeFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
		if (strcmp(facility.ID, tempID) == 0) {
			printf("\nFacility details\n");
			printf("ID: %s\nType: %s\nDescription: %s\nVenue: %s\nMax.User: %d\nStatus: %s\n\n", facility.ID, facility.type, facility.description, facility.venue, facility.maxUser, facility.status);
			printf("\nConfirm to delete the facility?\n");
			printf("\nPress 'Y' to delete, other key to cancel\n");
			char confirm;
			rewind(stdin);
			confirm = getch();
			confirm = toupper(confirm);
			if (confirm == 'Y') {
				DeleteFacilityRec deleteFaciRec;
				FILE* writeRec = fopen("deleteFacilitiesRec", "a");
				time_t t;
				time(&t);
				if (writeRec == NULL) {
					red();
					printf("Failed to open file.");
					resetColor();
					return;
				}
				deleteFaciRec.faci = facility;
				strcpy(deleteFaciRec.staffID, loginID);
				strcpy(deleteFaciRec.deleteDate, ctime(&t));
				deleteNewLine(deleteFaciRec.deleteDate);
				fprintf(writeRec, "%s|%s|%s|%s|%d|%s|%s|%s\n", deleteFaciRec.faci.ID, deleteFaciRec.faci.type, deleteFaciRec.faci.description, deleteFaciRec.faci.venue, deleteFaciRec.faci.maxUser, deleteFaciRec.faci.status, deleteFaciRec.staffID, deleteFaciRec.deleteDate);
				fclose(writeRec);
				red();
				printf("Deletion succeed\n");
				resetColor();

				continue;
			}
			else {
				red();
				printf("Deletion cancelled\n");
				resetColor();
				fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", facility.ID, facility.type, facility.description, facility.venue, facility.maxUser, facility.status);
			}
		}
		else {
			fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", facility.ID, facility.type, facility.description, facility.venue, facility.maxUser, facility.status);
		}
	}
	fclose(readFaci);
	fclose(writeFaci);
	system("pause");
	remove("facilities.txt");
	rename("tempFaci.txt", "facilities.txt");
}

void reportFaciIssue(char loginID[]) {
	int checkValidation = 0;
	char faciID[5];
	do{
		system("cls");
		checkValidation = 0;
		printf("Report Facility Issue\n");
		printf("Enter the facility ID that is having issue: ");
		rewind(stdin);
		fgets(faciID, 5, stdin);
		deleteNewLine(faciID);
		validateFaciID(faciID,&checkValidation);
		if (checkValidation == 0) {
			checkFaciIDExist(faciID, &checkValidation);
		}
	} while (checkValidation != 0);
	FaciIssue faciIssue;
	Facility facility;
	FILE* readFaci = fopen("facilities.txt","r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
		if (strcmp(facility.ID, faciID) == 0) {
			strcpy(faciIssue.faciID, facility.ID);
			strcpy(faciIssue.type, facility.type);
			strcpy(faciIssue.description, facility.description);
			strcpy(faciIssue.staffID, loginID);
			break;
		}
	}
	fclose(readFaci);

	printf("\nIssue to report [Max 50 words]: ");
	rewind(stdin);
	fgets(faciIssue.issue,51,stdin);
	deleteNewLine(faciIssue.issue);

	time_t t;
	time(&t);
	strcpy(faciIssue.dateReport, ctime(&t));
	deleteNewLine(faciIssue.dateReport);

	system("cls");
	printf("Report details\n");
	printf("Facility ID: %s\nFacility Type: %s\nFacility Description: %s\nIssue reported: %s\nReport by: %s\nDate report: %s\n", faciIssue.faciID, faciIssue.type,faciIssue.description,faciIssue.issue,faciIssue.staffID,faciIssue.dateReport);

	printf("\n\nConfirm to submit the report?\n");
	printf("\n\nPress 'Y' to submit the report, other key to cancel\n");
	char confirm;
	rewind(stdin);
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm == 'Y') {
		FILE* appendReport = fopen("FacilityIssue.txt","a");
		if (appendReport == NULL) {
			red();
			printf("Failed to open file.");
			resetColor();
			return;
		}
		fprintf(appendReport, "%s|%s|%s|%s|%s|%s\n", faciIssue.faciID, faciIssue.type, faciIssue.description, faciIssue.issue, faciIssue.staffID, faciIssue.dateReport);
		fclose(appendReport);

		red();
		printf("\aReported issue added\n");
		resetColor();
	}
	else {
		red();
		printf("\aReported issue cancelled\n");
		resetColor();
	}
	system("pause");
}

void reviewFaciIssue(char loginID[]) {	
	int totalRec = 0;
	FaciIssue* faciIssue = malloc(100 * sizeof * faciIssue);

	FILE* readRec = fopen("FacilityIssue.txt", "r");
	int size;
	if (readRec == NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		free(faciIssue);
		return;
	}
	else {
		fseek(readRec, 0, SEEK_END);
		size = ftell(readRec);
		fclose(readRec);
	}

	
	if (size == 0) {
		red();
		printf("\aNo issue reported currently\n");
		resetColor();
		free(faciIssue);
		system("pause");
		return;

	}
	else{
	readRec = fopen("FacilityIssue.txt", "r");
	while (!feof(readRec)) {
		fscanf(readRec, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", faciIssue[totalRec].faciID, faciIssue[totalRec].type, faciIssue[totalRec].description, faciIssue[totalRec].issue, faciIssue[totalRec].staffID, faciIssue[totalRec].dateReport);
		++totalRec;
	}
	}
	fclose(readRec);
	

	system("cls");
	printf("Review Facility Issue\n");
	printf("Report found\n");
	for (int i = 0; i < totalRec; i++) {
		printf("Facility ID: %s\nFacility Type: %s\nFacility Description: %s\nIssue reported: %s\nReport by: %s\nDate report: %s\n\n", faciIssue[i].faciID, faciIssue[i].type, faciIssue[i].description, faciIssue[i].issue, faciIssue[i].staffID, faciIssue[i].dateReport);
	}
	printf("Total %d reports found\n", totalRec);

	FaciIssue* matchID = malloc(50 * sizeof * matchID);
	int checkValidation, noOfMatchIDRec = 0, arrayPos = 0; char faciID[5];
	do {
		checkValidation = 0;
		printf("Enter the Facility ID to response: ");
		rewind(stdin);
		fgets(faciID, 5, stdin);
		deleteNewLine(faciID);
		validateFaciID(faciID,&checkValidation);
		if (checkValidation == 0) {
			for (int i = 0; i < totalRec; i++) {
				if (strcmp(faciIssue[i].faciID, faciID) == 0) {
					matchID[arrayPos] = faciIssue[i];
					++noOfMatchIDRec;
					++arrayPos;
				}
			}
			if (noOfMatchIDRec == 0) {
				red();
				printf("\aNo matching facility ID found in the list of reported issue\n");
				resetColor();
				system("pause");
				++checkValidation;
			}
		}
	} while (checkValidation != 0);
	
	system("cls");
	
	printf("Record found\n");
	for (int i = 0; i < noOfMatchIDRec; i++) {
		printf("No of Record: %d\n", i+1);
		printf("Facility ID: %s\nFacility Type: %s\nFacility Description: %s\nIssue reported: %s\nReport by: %s\nDate report: %s\n\n", matchID[i].faciID, matchID[i].type, matchID[i].description, matchID[i].issue, matchID[i].staffID, matchID[i].dateReport);
	}
	
	int recordToResponse;
	do{
	printf("\nKey in the no of record you wish to response[0 to exit]: ");
	rewind(stdin);
	scanf("%d", &recordToResponse);
	if (recordToResponse == 0) {
		free(faciIssue);
		free(matchID);
		return;
	}
	if (!(recordToResponse >= 1 && recordToResponse <= noOfMatchIDRec)) {
		red();
		printf("\aInvalid number of record inputted\n");
		resetColor();
	}
	} while (!(recordToResponse >= 1 && recordToResponse <= noOfMatchIDRec));

	system("cls");
	FaciIssueRes response;
	int responseMade = 0;
	for (int i = 0; i < noOfMatchIDRec; i++) {
		if (i == recordToResponse - 1) {
			printf("Facility ID: %s\nFacility Type: %s\nFacility Description: %s\nIssue reported: %s\nReport by: %s\nDate report: %s\n\n", matchID[i].faciID, matchID[i].type, matchID[i].description, matchID[i].issue, matchID[i].staffID, matchID[i].dateReport);
			time_t t;
			time(&t);			
			response.rec = matchID[i];
			strcpy(response.responseStaffID, loginID);
			strcpy(response.responseDate, ctime(&t));
			deleteNewLine(response.responseDate);

			int resOption;
			do{
				printf("\n\nResponse\n");
				printf("1. False Report\n2. Fixed\n3. Postponed\n4. Working on it\n");
				printf("Key in the assigned number as response: ");
				scanf("%d",&resOption);
				if (!(resOption >= 1 && resOption <= 4)) {
					red();
					printf("\aInvalid response\n");
					resetColor();
				}
			} while (!(resOption >= 1 && resOption <= 4));
			responseStatus(response.response,resOption);
			system("cls");

			printf("Facility ID: %s\nFacility Type: %s\nFacility Description: %s\nIssue reported: %s\nReport by: %s\nDate report: %s\nResponse: %s\nResponse By: %s\nDate responsed: %s\n", response.rec.faciID,response.rec.type,response.rec.description, response.rec.issue,response.rec.staffID,response.rec.dateReport, response.response, response.responseStaffID, response.responseDate);
			
			printf("\n\nConfirm to make the response?\n");
			printf("Press 'Y' to submit the response, other key to exit\n");
			char confirm;
			confirm = getch();
			confirm = toupper(confirm);
			if (confirm == 'Y') {
				++responseMade;
				FILE* appendRec = fopen("FaciIssueResponse.txt","a");
				if (appendRec == NULL) {
					red();
					printf("Failed to open file");
					resetColor();
					free(faciIssue);
					free(matchID);
					return;
				}
				fprintf(appendRec, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n", response.rec.faciID, response.rec.type, response.rec.description, response.rec.issue, response.rec.staffID, response.rec.dateReport, response.response, response.responseStaffID, response.responseDate);
				fclose(appendRec);
				red();
				printf("\aResponse successfully made.\n");
				printf("PLEASE CHANGE THE FACILITY STATUS THROUGH MODIFICATION FUNCTION IF THE ISSUE WILL AFFECT THE FACILITY FUNCTONALITY\n");
				resetColor();
				break;
			}
			else {
				red();
				printf("\aResponse cancelled\n");
				resetColor();
				break;
			}

		}
	}

	if (responseMade > 0) {
		FILE* writeReport = fopen("FacilityIssue.txt","w");
		if (writeReport == NULL) {
			red();
			printf("\aFailed to open file");
			resetColor();
			free(faciIssue);
			free(matchID);
			return;
		}
		for (int i = 0; i < totalRec; i++) {
			if ((strcmp(faciIssue[i].faciID, response.rec.faciID) == 0 ) && (strcmp(faciIssue[i].dateReport, response.rec.dateReport) == 0)) {
				continue;
			}
			else {
				fprintf(writeReport, "%s|%s|%s|%s|%s|%s\n", faciIssue[i].faciID, faciIssue[i].type, faciIssue[i].description, faciIssue[i].issue, faciIssue[i].staffID, faciIssue[i].dateReport);
			}
		}
		fclose(writeReport);
	}
	
	system("pause");
	free(faciIssue);
	free(matchID);
}

void updateFaciIssue(char loginID[]) {
	system("cls");
	FILE* readRec = fopen("FaciIssueResponse.txt","r");
	FaciIssueRes *responseRec = malloc(70 *sizeof * responseRec), rec;// this allow is to store data of which status isn't false report / solved, as this two status considered done.
	FaciIssueRes* allRec = malloc(100 *sizeof *allRec);//this data store all data regardless the status, which used to write data in the end of the function if any update made.
	int totalRec = 0;
	int arrPos = 0;
	if (readRec ==NULL) {
		red();
		printf("Failed to open file");
		resetColor();
		free(responseRec);
		free(allRec);
		return;
	}
	while (!feof(readRec)) {
		//read each of the data, if the response status is false report / fixed, then don't add the ada into the array of response sience both of the aforementioned status is considered completed
		fscanf(readRec, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", rec.rec.faciID, rec.rec.type, rec.rec.description, rec.rec.issue, rec.rec.staffID, rec.rec.dateReport, rec.response, rec.responseStaffID, rec.responseDate);
		allRec[totalRec] = rec;
		++totalRec;
		if (strcmp(rec.response, "False Report") == 0 || strcmp(rec.response, "Solved") == 0) {
			continue;
		}
		else {
			responseRec[arrPos] = rec;
			printf("Number of record: %d\n",arrPos+1);
			printf("Facility ID: %s\nFacility Type: %s\nFacility Description: %s\nIssue reported: %s\nReport by: %s\nDate report: %s\nResponse: %s\nResponse By: %s\nDate responsed: %s\n\n", rec.rec.faciID, rec.rec.type, rec.rec.description, rec.rec.issue, rec.rec.staffID, rec.rec.dateReport, rec.response, rec.responseStaffID, rec.responseDate);
			++arrPos;
		}
	}
	fclose(readRec);

	int recOption;
	do{
		printf("Enter the number of record to update the specified response status[0 to exit]: ");
		rewind(stdin);
		scanf("%d",&recOption);
		if (recOption == 0) {
			red();
			printf("\aExitted\n");
			resetColor();
			free(responseRec);
			free(allRec);
			return;
		}
		if (!(recOption >= 1 && recOption <= arrPos)) {
			red();
			printf("\aInvalid number of record inputted\n");
			resetColor();
		}
	} while (!(recOption >= 1 && recOption <= arrPos));
	system("cls");
	int updateRecord = 0;
	FaciIssueRes updatedRes;//if we directly write the responseRec[i] into the rec, record that having status of "False Report" and "Solved" will not be written.
	//thus, create another new struct to store this update, and write into file

	for (int i = 0; i < arrPos; i++) {
		if (i == recOption - 1) {
			printf("Facility ID: %s\nFacility Type: %s\nFacility Description: %s\nIssue reported: %s\nReport by: %s\nDate report: %s\nResponse: %s\nResponse By: %s\nDate responsed: %s\n\n", responseRec[i].rec.faciID, responseRec[i].rec.type, responseRec[i].rec.description, responseRec[i].rec.issue, responseRec[i].rec.staffID, responseRec[i].rec.dateReport, responseRec[i].response, responseRec[i].responseStaffID, responseRec[i].responseDate);
			printf("\n\nThis is the record to update response status?\n");
			printf("Press 'Y' to modify the status, other key to exit\n");
			char confirm;
			confirm = getch();
			confirm = toupper(confirm);
			if (confirm == 'Y') {
				++updateRecord;
				printf("Current response: %s\n\n", responseRec[i].response);

				int resOption;
				char tempRes[16];
				strcpy(tempRes,responseRec[i].response);
				do {
					printf("\n\nResponse\n");
					printf("1. False Report\n2. Fixed\n3. Postponed\n4. Working on it\n");
					printf("Key in the assigned number as response[0 to exit]: ");
					scanf("%d", &resOption);

					if (resOption == 0) {
						free(responseRec);
						free(allRec);
						return;
					}

					if (strcmp(tempRes, "Postponed") == 0 && resOption == 3) {
						red();
						printf("\aNo update required,it is the current status\n");
						printf("Key in 0 to exit\n");
						resetColor();
						resOption = -1;
					}

					if (strcmp(tempRes, "Working") == 0 && resOption == 4) {
						red();
						printf("\aNo update required,it is the current status\n");
						printf("Key in 0 to exit\n");
						resetColor();
						resOption = -1;
					}

					if (!(resOption >= 1 && resOption <= 4) && resOption != -1) {
						red();
						printf("\aInvalid response\n");
						resetColor();
					}
				} while (!(resOption >= 1 && resOption <= 4));
				responseStatus(tempRes, resOption);
				time_t t;
				time(&t);
				char tempNewResDate[26];
				strcpy(tempNewResDate,ctime(&t));
				deleteNewLine(tempNewResDate);
				char tempID[7];
				strcpy(tempID, loginID);
				printf("\n\nUpdated response status\n");
				printf("Facility ID: %s\nFacility Type: %s\nFacility Description: %s\nIssue reported: %s\nReport by: %s\nDate report: %s\nResponse: %s\nResponse By: %s\nDate responsed: %s\n", responseRec[i].rec.faciID, responseRec[i].rec.type, responseRec[i].rec.description, responseRec[i].rec.issue, responseRec[i].rec.staffID, responseRec[i].rec.dateReport, tempRes, tempID, tempNewResDate);
				printf("\n\nThe record will be updated as above\n");
				printf("Confirm to update? Press 'Y' to confirm, other key to exit\n");
				char doubleConfirm;
				doubleConfirm = getch();
				doubleConfirm = toupper(doubleConfirm);
				if (doubleConfirm == 'Y') {
					strcpy(responseRec[i].responseStaffID, tempID);
					strcpy(responseRec[i].response, tempRes);
					strcpy(responseRec[i].responseDate, tempNewResDate);
					updatedRes = responseRec[i];
					break;
				}
				else {
					--updateRecord;
					break;
				}
			}
			else{
				red();
				printf("\aStatus update cancelled\n");
				resetColor();
				break;
			}
		}
	}

	if(updateRecord > 0){
		FILE* writeRec = fopen("FaciIssueResponse.txt","w");
		if (writeRec == NULL) {
			red();
			printf("Failed to open file");
			resetColor;
			free(responseRec);
			free(allRec);
			return;
		}
		for (int i = 0; i < totalRec; i++) {
			if ((strcmp(allRec[i].rec.faciID, updatedRes.rec.faciID) == 0) && (strcmp(allRec[i].rec.dateReport, updatedRes.rec.dateReport) == 0)) {
				fprintf(writeRec, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n", updatedRes.rec.faciID, updatedRes.rec.type, updatedRes.rec.description, updatedRes.rec.issue, updatedRes.rec.staffID, updatedRes.rec.dateReport, updatedRes.response, updatedRes.responseStaffID, updatedRes.responseDate);
			}
			else {
				fprintf(writeRec, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n", allRec[i].rec.faciID, allRec[i].rec.type, allRec[i].rec.description, allRec[i].rec.issue, allRec[i].rec.staffID, allRec[i].rec.dateReport, allRec[i].response, allRec[i].responseStaffID, allRec[i].responseDate);
			}
		}
		fclose(writeRec);
		red();
		printf("\aUpdate succeed\n");
		resetColor();
	}
	else {
		red();
		printf("No change is made\n");
		resetColor();
	}
	system("pause");

	free(responseRec);
	free(allRec);
}
//helper function
void validateFaciID(char input[], int *checkValidation) {
	if (!(strlen(input) >= 3)) { //shortest facility ID is length of 3
		++* checkValidation;
		red();
		printf("\aIncorrect facility ID format inputted\n");
		resetColor();
		system("pause");
		return;
	}
	
	if (!(input[0] >= 65 && input[0] <= 90) && !(input[0] >= 97 && input[0] <= 122)) {	 //if array of 0 and 1 isn't alphabetic input
		++* checkValidation;
		red();
		printf("\aFacility ID must start with two alphabets\n");
		resetColor();
		system("pause");
		return;
	}
	if (!(input[1] >= 65 && input[1] <= 90) && !(input[1] >= 97 && input[1] <= 122)) {
		++* checkValidation;
		red();
		printf("\aFacility ID must start with two alphabets\n");
		resetColor();
		system("pause");
		return;
	}

	for (int i = 2; i < strlen(input); i++) {//array of 2 onward must be numeric input, else wrong format
		if (!(input[i] >= '0' && input[i] <= '9')) {
			++* checkValidation;
			red();
			printf("\aIncorrect facility ID format inputted\n");
			resetColor();
			system("pause");
			return;
		}
	}

	if (input[0] >= 97 && input[0] <= 122) { //if input is lowercase, convert to uppercase
		input[0] -= 32;
	}
	if (input[1] >= 97 && input[1] <= 122) {
		input[1] -= 32;
	}
}
void validateFaciType(char input[], int* checkValidation) {
	for (int i = 0; i < strlen(input); i++) {
		if (input[i] >= 48 && input[i] <= 57) {
			++* checkValidation;
			red();
			printf("\aFacility type shouldn't include number\n");
			resetColor();
			system("pause");
			return;
		}

		if (input[i - 1] == ' ' && (input[i] >= 97 && input[i] <= 122)) {
			input[i] -= 32; //convert any alphabet come after space to uppercase
		}
	}

	if (input[0] >= 97 && input[0] <= 122) { //if input is lowercase, convert to uppercase
		input[0] -= 32;
	}
}
void validateFaciDesc(char input[], int* checkValidation) {
	int numericInputCount = 0;
	if (input[0] >= 97 && input[0] <= 122) {
		input[0] -= 32;
	}
	
	for (int i = 0; i < strlen(input); i++) {
		if (input[i] >= 48 && input[i] <= 57) {
			++numericInputCount;
		}

		if (input[i - 1] == ' ' && (input[i] >= 97 && input[i] <= 122)) {
			input[i] -= 32; //convert any alphabet come after space to uppercase
		}
	}
	if (numericInputCount == 0) {
		++* checkValidation;
		red();
		printf("\aIncorrect facility description format inputted\n");
		resetColor();
		system("pause");
		return;
	}
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
		char temp[3] = {input[2],input[3]};
		sequence = atoi(temp);
	}

	return sequence;
}
void extractFacilityDesc(char input[]) {
	for (int i = 0; i < strlen(input); i++) {
		if (input[i] == ' ' && (input[i + 1] >= 48 && input[i + 1] <= 57)) {
			input[i] = '\0';
			return;
		}
	}
}
void sortFaciRecord() {
	Facility facility[100];
	FILE* readFaci;
	readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
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

	FILE* writeFaci = fopen("facilities.txt","w");
	if (writeFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	for (int i = 0; i < totalCount; i++) {
		fprintf(writeFaci, "%s|%s|%s|%s|%d|%s\n", facility[i].ID, facility[i].type, facility[i].description, facility[i].venue, facility[i].maxUser, facility[i].status);
	}
	fclose(writeFaci);
}
void checkFaciIDExist(char input[], int* checkValidation) {
	Facility facility;
	FILE* readFaci;
	readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("Failed to open file.");
		resetColor();
		return;
	}
	int recordFound = 0;
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
		if (strcmp(facility.ID, input) == 0) {
			++recordFound;
			break;
		}
	}
	fclose(readFaci);
	if (recordFound == 0) {
		++* checkValidation;
		red();
		printf("\aThe ID inputted doesn't exists\n");
		resetColor();
		system("pause");
	}
}
void responseStatus(char input[], int option) {
	if (option == 1) {
		strcpy(input,"False Report");
		return;
	}

	if (option == 2) {
		strcpy(input,"Solved");
		return;
	}

	if (option == 3) {	
		strcpy(input, "Postponed");
		return;
	}

	if (option == 4) {
		if (strcmp(input, "Working") == 0) {
			red();
			printf("\aNo update required,it is the current status\n");
			resetColor();
			return;
		}
		strcpy(input, "Working");
		return;
	}
}

