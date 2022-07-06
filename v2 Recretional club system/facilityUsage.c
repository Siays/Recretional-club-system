#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#pragma warning(disable: 4996)

typedef struct {
	char bID[11];//B220700001
	BookingDate tDate;//today date
	BookingDate bDate;//booking date
	BookingTime sTime; //from
	BookingTime eTime; //to
	char memID[8];
	char faciID[5];
	int noOfPeople;
}FacilityUsageBooking;

typedef struct {
	char wID[11];//W220703001
	BookingDate tDate;
	BookingTime sTime;
	BookingTime eTime;
	char memID[8];
	char faciID[5];
	int noOfPeople;
}FacilityUsageWalkIn;

typedef struct {
	char faciType[16];
	int count;
}FacilityTypeUsageCount;


void displayAllUsageRec();
void searchUsageRecByMonth();
void searchUsageReByMonthRange();
void searchUsageReBySpecifiedDate();
void facilityPopularity();

//helper functions
void checkValidMonthRange(int sMonth, int eMonth, int* checkValidation);
void validateDateForFaciUsageRec(int year, int month, int day, int* checkValidation);


void facilityUsageModuleMain() {
	int menuSelection;
	do {
		system("cls");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t\t\Facility Usage Module\n");
		printf("\t\t\t---------------------------------------------------\n");
		printf("\t\t\t0. Exit the Module\n");
		printf("\t\t\t1. Display All Usage Record(both booked and walked in)\n");
		printf("\t\t\t2. Search Usage Record By a Month\n");
		printf("\t\t\t3. Search Usage Record Within a Range of Month\n");
		printf("\t\t\t4. Search Usage Record With Specified Date\n");
		printf("\t\t\t5. View Facility Popularity\n");
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
			displayAllUsageRec();
			break;

		case 2:
			searchUsageRecByMonth();
			break;

		case 3:
			searchUsageReByMonthRange();
			break;

		case 4:
			searchUsageReBySpecifiedDate();
			break;

		case 5:
			facilityPopularity();
			break;

		default:
			red();
			printf("\t\t\tInvalid selection!\a\n");
			resetColor();
			system("pause");
		}
	} while (menuSelection != 0);
}


void displayAllUsageRec() {
	FacilityUsageBooking bkRec;
	FacilityUsageWalkIn wkInRec;
	FILE* readWalkInRec = fopen("WalkinFacilityUsage.bin","rb");
	FILE* readBookingRec = fopen("BookingFacilityUsage.bin","rb");
	if (readWalkInRec == NULL) {
		red();
		printf("\aFailed to open file: WalkinFacilityUsage.bin\n");
		resetColor();
		system("pause");
		return;
	}
	if (readBookingRec == NULL) {
		red();
		printf("\aFailed to open file: BookingFacilityUsage.bin\n");
		resetColor();
		system("pause");
		return;
	}
	system("cls");
	printf("All facility usage\n");
	printf("Walk in (Green Color)\n");
	while (fread(&wkInRec, sizeof(FacilityUsageWalkIn), 1, readWalkInRec) != 0) {
		green();
		printf("Walk in ID: %s\nWalk in made on: %02d-%02d-%d\nUsage time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", wkInRec.wID, wkInRec.tDate.day, wkInRec.tDate.month, wkInRec.tDate.year, wkInRec.sTime.hour, wkInRec.sTime.minute, wkInRec.eTime.hour, wkInRec.eTime.minute, wkInRec.memID, wkInRec.faciID, wkInRec.noOfPeople);
		resetColor();
	}
	printf("Booked(Yellow Color)\n");
	while (fread(&bkRec, sizeof(Booking), 1, readBookingRec) != 0) {
		yellow();
		printf("Booking ID: %s\nBooking made on: %02d-%02d-%d\nDate booking for: %02d-%02d-%d\nBooking time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", bkRec.bID, bkRec.tDate.day, bkRec.tDate.month, bkRec.tDate.year, bkRec.bDate.day, bkRec.bDate.month, bkRec.bDate.year, bkRec.sTime.hour, bkRec.sTime.minute, bkRec.eTime.hour, bkRec.eTime.minute, bkRec.memID, bkRec.faciID, bkRec.noOfPeople);
		resetColor();
	}
	fclose(readWalkInRec);
	fclose(readBookingRec);
	system("pause");
}

void searchUsageRecByMonth() {
	system("cls");
	int checkValidation;
	int month, year;
	do{
		checkValidation = 0;
		printf("Enter the year you wish to view: ");
		rewind(stdin);
		scanf("%d", &year);
		if (year < 2022) {
			++checkValidation;
			red();
			printf("\aThe club start operating since year 2022\n");
			printf("Impossible to have record before year 2022\n");
			resetColor();
			system("pause");
		}
		if(checkValidation == 0){
		printf("Enter the month you wish to view: ");
		rewind(stdin);
		scanf("%d", &month);
			if (!(month >= 1 && month <= 12)) {
				++checkValidation;
				red();
				printf("\aIloogical month inputted\n");
				resetColor();
				system("pause");
			}
		}
	} while (checkValidation !=0);

	FacilityUsageBooking bkRec;
	FacilityUsageWalkIn wkInRec;
	FILE* readWalkInRec = fopen("WalkinFacilityUsage.bin", "rb");
	FILE* readBookingRec = fopen("BookingFacilityUsage.bin", "rb");
	if (readWalkInRec == NULL) {
		red();
		printf("\aFailed to open file: WalkinFacilityUsage.bin\n");
		resetColor();
		system("pause");
		return;
	}
	if (readBookingRec == NULL) {
		red();
		printf("\aFailed to open file: BookingFacilityUsage.bin\n");
		resetColor();
		system("pause");
		return;
	}

	system("cls");
	printf("Displaying all record for year %d month %d\n",year,month);
	printf("Walk in (Green Color)\n");
	while (fread(&wkInRec, sizeof(FacilityUsageWalkIn), 1, readWalkInRec) != 0) {
		//or use day directly
		char extractYear[3] = { wkInRec.wID[1],wkInRec.wID[2] };
		int convertYear = atoi(extractYear);
		char extractMonth[3] = { wkInRec.wID[3],wkInRec.wID[4] };
		int convertMonth = atoi(extractMonth);
		if (convertYear == (year%1000) && convertMonth == month) {
			green();
			printf("Walk in ID: %s\nWalk in made on: %02d-%02d-%d\nUsage time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", wkInRec.wID, wkInRec.tDate.day, wkInRec.tDate.month, wkInRec.tDate.year, wkInRec.sTime.hour, wkInRec.sTime.minute, wkInRec.eTime.hour, wkInRec.eTime.minute, wkInRec.memID, wkInRec.faciID, wkInRec.noOfPeople);
			resetColor();
		}
	}
	printf("Booked (Yellow Color)\n");
	while (fread(&bkRec, sizeof(Booking),1,readBookingRec) != 0) {
		char extractYear[3] = { bkRec.bID[1],bkRec.bID[2] };
		int convertYear = atoi(extractYear);
		char extractMonth[3] = { bkRec.bID[3],bkRec.bID[4] };
		int convertMonth = atoi(extractMonth);
		if (convertYear == (year % 1000) && convertMonth == month) {
			yellow();
			printf("Booking ID: %s\nBooking made on: %02d-%02d-%d\nDate booking for: %02d-%02d-%d\nBooking time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", bkRec.bID, bkRec.tDate.day, bkRec.tDate.month, bkRec.tDate.year, bkRec.bDate.day, bkRec.bDate.month, bkRec.bDate.year, bkRec.sTime.hour, bkRec.sTime.minute, bkRec.eTime.hour, bkRec.eTime.minute, bkRec.memID, bkRec.faciID, bkRec.noOfPeople);
			resetColor();
		}
	}
	fclose(readWalkInRec);
	fclose(readBookingRec);
	system("pause");

}

void searchUsageReByMonthRange() {

	int checkValidation;
	int sMonth, eMonth, year;
	do {
		system("cls");
		checkValidation = 0;
		printf("Enter the year you wish to view: ");
		rewind(stdin);
		scanf("%d", &year);
		if (year < 2022) {
			++checkValidation;
			red();
			printf("\aThe club start operating since year 2022\n");
			printf("Impossible to have record before year 2022\n");
			resetColor();
			system("pause");
		}
		if (checkValidation == 0) {
			printf("Enter the month you wish to view\n");
			printf("From month: ");
			rewind(stdin);
			scanf("%d", &sMonth);
			printf("To month: ");
			rewind(stdin);
			scanf("%d", &eMonth);
			checkValidMonthRange(sMonth, eMonth, &checkValidation);
		}
	} while (checkValidation != 0);

	FacilityUsageBooking bkRec;
	FacilityUsageWalkIn wkInRec;
	FILE* readWalkInRec = fopen("WalkinFacilityUsage.bin", "rb");
	FILE* readBookingRec = fopen("BookingFacilityUsage.bin", "rb");
	if (readWalkInRec == NULL) {
		red();
		printf("\aFailed to open file: WalkinFacilityUsage.bin\n");
		resetColor();
		system("pause");
		return;
	}
	if (readBookingRec == NULL) {
		red();
		printf("\aFailed to open file: BookingFacilityUsage.bin\n");
		resetColor();
		system("pause");
		return;
	}

	system("cls");
	printf("Displaying all record for year %d from month %d to month %d\n", year, sMonth,eMonth);
	printf("Walk in (Green Color)\n");
	while (fread(&wkInRec, sizeof(FacilityUsageWalkIn), 1, readWalkInRec) != 0) {
		char extractYear[3] = { wkInRec.wID[1],wkInRec.wID[2] };
		int convertYear = atoi(extractYear);
		char extractMonth[3] = { wkInRec.wID[3],wkInRec.wID[4] };
		int convertMonth = atoi(extractMonth);
		if (convertYear == (year % 1000) && (convertMonth >= sMonth && convertMonth <= eMonth)) {
			green();
			printf("Walk in ID: %s\nWalk in made on: %02d-%02d-%d\nUsage time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", wkInRec.wID, wkInRec.tDate.day, wkInRec.tDate.month, wkInRec.tDate.year, wkInRec.sTime.hour, wkInRec.sTime.minute, wkInRec.eTime.hour, wkInRec.eTime.minute, wkInRec.memID, wkInRec.faciID, wkInRec.noOfPeople);
			resetColor();
		}
	}
	printf("Booked (Yellow Color)\n");
	while (fread(&bkRec, sizeof(Booking), 1, readBookingRec) != 0) {
		char extractYear[3] = { bkRec.bID[1],bkRec.bID[2] };
		int convertYear = atoi(extractYear);
		char extractMonth[3] = { bkRec.bID[3],bkRec.bID[4] };
		int convertMonth = atoi(extractMonth);
		if (convertYear == (year % 1000) && (convertMonth >= sMonth && convertMonth <= eMonth)) {
			yellow();
			printf("Booking ID: %s\nBooking made on: %02d-%02d-%d\nDate booking for: %02d-%02d-%d\nBooking time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", bkRec.bID, bkRec.tDate.day, bkRec.tDate.month, bkRec.tDate.year, bkRec.bDate.day, bkRec.bDate.month, bkRec.bDate.year, bkRec.sTime.hour, bkRec.sTime.minute, bkRec.eTime.hour, bkRec.eTime.minute, bkRec.memID, bkRec.faciID, bkRec.noOfPeople);
			resetColor();
		}
	}
	fclose(readWalkInRec);
	fclose(readBookingRec);
	system("pause");

}

void searchUsageReBySpecifiedDate() {
	int checkValidation;
	int year, month, day;
	do {
		system("cls");
		checkValidation = 0;
		printf("Enter the date you wish to view\n");
		printf("Year: ");
		rewind(stdin);
		scanf("%d", &year);
		
		printf("Month: ");
		rewind(stdin);
		scanf("%d", &month);

		printf("Day: ");
		rewind(stdin);
		scanf("%d", &day);
		validateDateForFaciUsageRec(day,month,year,&checkValidation);
		
	} while (checkValidation != 0);


	FacilityUsageBooking bkRec;
	FacilityUsageWalkIn wkInRec;
	FILE* readWalkInRec = fopen("WalkinFacilityUsage.bin", "rb");
	FILE* readBookingRec = fopen("BookingFacilityUsage.bin", "rb");
	if (readWalkInRec == NULL) {
		red();
		printf("\aFailed to open file: WalkinFacilityUsage.bin\n");
		resetColor();
		system("pause");
		return;
	}
	if (readBookingRec == NULL) {
		red();
		printf("\aFailed to open file: BookingFacilityUsage.bin\n");
		resetColor();
		system("pause");
		return;
	}

	system("cls");
	printf("Displaying all record for the date: year %d month %d day %d\n", year, month, day);
	printf("Walk in (Green Color)\n");
	while (fread(&wkInRec, sizeof(FacilityUsageWalkIn), 1, readWalkInRec) != 0) {
		if (wkInRec.tDate.year == year && wkInRec.tDate.month == month && wkInRec.tDate.year) {
			green();
			printf("Walk in ID: %s\nWalk in made on: %02d-%02d-%d\nUsage time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", wkInRec.wID, wkInRec.tDate.day, wkInRec.tDate.month, wkInRec.tDate.year, wkInRec.sTime.hour, wkInRec.sTime.minute, wkInRec.eTime.hour, wkInRec.eTime.minute, wkInRec.memID, wkInRec.faciID, wkInRec.noOfPeople);
			resetColor();
		}
	}
	printf("Booked (Yellow Color)\n");
	while (fread(&bkRec, sizeof(Booking), 1, readBookingRec) != 0) {
		if (bkRec.bDate.year == year && bkRec.bDate.month == month && bkRec.bDate.year) {
			yellow();
			printf("Booking ID: %s\nBooking made on: %02d-%02d-%d\nDate booking for: %02d-%02d-%d\nBooking time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", bkRec.bID, bkRec.tDate.day, bkRec.tDate.month, bkRec.tDate.year, bkRec.bDate.day, bkRec.bDate.month, bkRec.bDate.year, bkRec.sTime.hour, bkRec.sTime.minute, bkRec.eTime.hour, bkRec.eTime.minute, bkRec.memID, bkRec.faciID, bkRec.noOfPeople);
			resetColor();
		}
	}
	fclose(readWalkInRec);
	fclose(readBookingRec);
	system("pause");
}

void facilityPopularity(){
	system("cls");
	FILE* readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("\aFailed to open file: facilities.txt\n");
		resetColor();
		system("pause");
		return;
	}
	int totalFaciCount = 0;
	Facility* facility = malloc(100 * sizeof * facility);
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility[totalFaciCount].ID, facility[totalFaciCount].type, facility[totalFaciCount].description, facility[totalFaciCount].venue, &facility[totalFaciCount].maxUser, facility[totalFaciCount].status);
		++totalFaciCount;
	}
	fclose(readFaci);

	//create each type of different facility type and initialize them to 0 
	FacilityTypeUsageCount* faciTypeRec = malloc(50 * sizeof * faciTypeRec);
	int totalTypeCount = 0;
	for (int i = 0; i < totalFaciCount; i++) {
		if (i != totalFaciCount - 1) {
			if (strcmp(facility[i].type, facility[i + 1].type) != 0) {
				strcpy(faciTypeRec[totalTypeCount].faciType, facility[i].type);
				faciTypeRec[totalTypeCount].count = 0;
				++totalTypeCount;
			}
			else {
				continue;
			}
		}
		else {
			for (int j = 0; j < totalTypeCount; j++) {
				if (strcmp(facility[i].type, faciTypeRec[j].faciType) == 0) {
					break;
				}
				if (j == (totalTypeCount - 1) && strcmp(facility[i].type, faciTypeRec[j].faciType) != 0) {
					strcpy(faciTypeRec[totalTypeCount].faciType, facility[i].type);
					faciTypeRec[totalTypeCount].count = 0;
					++totalTypeCount;
					break;
				}
			}
		}
	}

	/*
	printf("total type count: %d\n",totalTypeCount);
	for (int i = 0; i < totalTypeCount; i++) {
		printf("Type: %s\tCount: %d\n", faciTypeRec[i].faciType, faciTypeRec[i].count);
	}
	system("pause");*/

	FILE* readWalkInRec = fopen("WalkinFacilityUsage.bin","r");
	if (readWalkInRec == NULL) {
		red();
		printf("\aFailed to open file: WalkinFacilityUsage.bin");
		resetColor();
		system("pause");
		free(facility);
		free(faciTypeRec);
		return;
	}
	WalkIn wkIn;
	while (fread(&wkIn, sizeof(WalkIn), 1, readWalkInRec) != 0) {
		for (int i = 0; i < totalFaciCount; i++) {
			if (strcmp(wkIn.faciID, facility[i].ID) == 0) {
				for (int j = 0; j < totalTypeCount; j++) {
					if (strcmp(facility[i].type, faciTypeRec[j].faciType) == 0) {
						++faciTypeRec[j].count;
						break;
					}
				}
				break;
			}
		}
	}
	fclose(readWalkInRec);

	/*
	printf("total type count: %d\n",totalTypeCount);
	for (int i = 0; i < totalTypeCount; i++) {
		printf("Type: %s\tCount: %d\n", faciTypeRec[i].faciType, faciTypeRec[i].count);
	}
	system("pause");*/

	FILE* readBkRec = fopen("BookingFacilityUsage.bin", "r");
	if (readBkRec == NULL) {
		red();
		printf("\aFailed to open file: BookingFacilityUsage.bin");
		resetColor();
		system("pause");
		free(facility);
		free(faciTypeRec);
		return;
	}
	Booking bk;
	while (fread(&bk, sizeof(Booking), 1, readBkRec) != 0) {
		for (int i = 0; i < totalFaciCount; i++) {
			if (strcmp(bk.faciID, facility[i].ID) == 0) {
				for (int j = 0; j < totalTypeCount; j++) {
					if (strcmp(facility[i].type, faciTypeRec[j].faciType) == 0) {
						++faciTypeRec[j].count;
						break;
					}
				}
				break;
			}
		}
	}
	fclose(readBkRec);

	printf("Facility Type\t\tUsage Count\n");
	printf("--------------------------------------------\n");
	for (int i = 0; i < totalTypeCount; i++) {
		printf("%-16s\t%4d\n", faciTypeRec[i].faciType, faciTypeRec[i].count);
	}
	printf("--------------------------------------------\n");
	int mostUsage, leastUsage;
	//get the count of most usage and least usage
	for (int i = 0; i < totalTypeCount; i++) {
		if (i != totalTypeCount - 1) {
			if (faciTypeRec[i].count > faciTypeRec[i + 1].count) {
				mostUsage = faciTypeRec[i].count;
			}
			else if (faciTypeRec[i].count < faciTypeRec[i + 1].count) {
				leastUsage = faciTypeRec[i].count;
			}
			else {
				continue;
			}
		}
		else {
			if (faciTypeRec[i].count > mostUsage) {
				mostUsage = faciTypeRec[i].count;
			}
			if (faciTypeRec[i].count < leastUsage) {
				leastUsage = faciTypeRec[i].count;
			}
		}
	}

	//display the facility with most usaage and least usage
	printf("\n\n");
	for (int i = 0; i < totalTypeCount; i++) {
		if (faciTypeRec[i].count == mostUsage) {
			printf("Facility type: %s has the usage count of %d being the most popular facility type\n",faciTypeRec[i].faciType, faciTypeRec[i].count);
		}
		else if (faciTypeRec[i].count == leastUsage) {
			printf("Facility type: %s has the usage count of %d being the least popular facility type\n", faciTypeRec[i].faciType, faciTypeRec[i].count);
		}
		else {
			continue;
		}
	}
	system("pause");
	free(faciTypeRec);
	free(facility);
}

//helper functions
void checkValidMonthRange(int sMonth,int eMonth, int *checkValidation) {
	if (sMonth < 1 || sMonth > 12 || eMonth < 1 || eMonth > 12) {
		++* checkValidation;
		red();
		printf("\aIllogical month inputted\n");
		resetColor();
		system("pause");
		return;
	}

	if (eMonth <= sMonth) {
		++* checkValidation;
		red();
		printf("\aSecond input of month must be greater than the first month\n");
		resetColor();
		system("pause");
		return;
	}

}
void validateDateForFaciUsageRec(int year, int month,int day, int *checkValidation) {
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysYear = time.tm_year + 1900;
	int sysDay = time.tm_mday;
	if (year < 2022) {
		++*checkValidation;
		red();
		printf("\aThe club start operating since year 2022\n");
		printf("Impossible to have record before year 2022\n");
		resetColor();
		system("pause");
		return;
	}

	if (!(month >= 1 && month <= 12)) {
		red();
		printf("\aIllogical input of month\n");
		resetColor();
		system("pause");
		++* checkValidation;
		return;
	}

	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
		if (day > 31) {
			++* checkValidation;
			red();
			printf("\aIllogical input, the last day of the month is 31\n");
			resetColor();
			system("pause");
			return;
		}
		if (day < 1) {
			++* checkValidation;
			red();
			printf("\aIllogical input of day\n");
			resetColor();
			system("pause");
			return;
		}
	}
	if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (day > 30) {
			++* checkValidation;
			red();
			printf("\aIllogical input, the last day of the month is 30\n");
			resetColor();
			system("pause");
			return;
		}
		if (day < 1) {
			++* checkValidation;
			red();
			printf("Illogical input of day\n");
			resetColor();
			system("pause");
			return;
		}
	}

	if (month == 2) {
		if (((year % 4 == 0) && ((year % 400 == 0) || (year % 100 != 0)))) {
			if (day > 29) {
				++* checkValidation;
				red();
				printf("\aIllogical input, the last day of the month is 29\n");
				resetColor();
				system("pause");
				return;
			}
			if (day < 1) {
				++* checkValidation;
				red();
				printf("\aIllogical input of day\n");
				resetColor();
				system("pause");
				return;
			}
		}
		else {
			if (day > 28) {
				++* checkValidation;
				red();
				printf("\aIllogical input, the last day of the month is 28\n");
				resetColor();
				system("pause");
				return;
			}
			if (day < 1) {
				++* checkValidation;
				red();
				printf("\aIllogical input of day\n");
				resetColor();
				system("pause");
				return;
			}
		}

	}

}