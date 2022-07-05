#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>w
#pragma warning(disable:4996)

typedef struct {
	int day;
	int month;
	int year;
}BookingDate;

typedef struct {
	int hour;
	int minute;
}BookingTime;

typedef struct{
	char bID[11];//B220700001
	BookingDate tDate;//today date
	BookingDate bDate;//booking date
	BookingTime sTime; //from
	BookingTime eTime; //to
	char memID[8];
	char faciID[5];
	int noOfPeople;
}Booking;

typedef struct {
	char wID[11];//W220703001
	BookingDate tDate;
	BookingTime sTime;
	BookingTime eTime;
	char memID[8];
	char faciID[5];
	int noOfPeople;
}WalkIn;
//0800 - 2200 

void makeBooking();
void walkIn();
void displayWalkIn();
void displayBooking();
void searchBooking();
void searchWalkIn();
void searchRecByMemID();
void modifyBooking();
void modifyWalkIn();

//helper functions
void autoGenerateWalkInID(char input[]);
void autoGenerateBookingID(char input[], int year, int month);
void validateBookingTime(int duration, int sHour, int sMin, int* eHour, int* eMin, int* checkValidation);
void validateBookingID(char input[], int* checkValidation);
void validateWalkInID(char input[], int* checkValidation);
void validateBetweenDate(char memID[], int bookingForMonth, int bookingForDay, int* checkValidation);
void rangeBeforeTheDate(int dayRange, int bkMonth, int bkDay, int* outMonth, int* outDay);
void rangeAfterTheDate(int dayRange, int bkMonth, int bkDay, int* outMonth, int* outDay);
void updateFacilityStatus();
void verifyFacilityStatus();

void bookingModuleMain(){
	//display available facility
	// display all booking
	//search booking
	//modify booking
	//make booking via booking / walk in
	//delete booking
	//display facilities populairty
	//display 
	//displayBooking();
	//displayWalkIn();
	//verifyFacilityStatus();
	//updateFacilityStatus();
	
	//makeBooking();
	
	//walkIn();
	//searchBooking();
}

void makeBooking() {
	Booking bk;
	
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysDay = time.tm_mday;
	int sysYear = time.tm_year + 1900;

	bk.tDate.day = sysDay;
	bk.tDate.month = sysMonth;
	bk.tDate.year = sysYear;
	int checkValidation;

	do{
		system("cls");
		printf("Add new booking\n");
		checkValidation = 0;
		printf("Enter the date booking for [in DD MM YYYY format] (must at least 1 day in advance)\n");
		printf("DD: ");
		rewind(stdin);
		scanf("%d",&bk.bDate.day);
		printf("MM: ");
		rewind(stdin);
		scanf("%d", &bk.bDate.month);
		printf("YYYY: ");
		rewind(stdin);
		scanf("%d", &bk.bDate.year);
		validateDate(bk.bDate.day, bk.bDate.month,bk.bDate.year,&checkValidation);
		if (sysDay == bk.bDate.day && sysMonth == bk.bDate.month && sysYear == bk.bDate.year) {
			++checkValidation;
			red();
			printf("Booking must be made 1 day in advance\n");
			resetColor();
			system("pause");
		}
	} while (checkValidation != 0);

	autoGenerateBookingID(bk.bID, bk.tDate.year, bk.tDate.month);

	int duration;
	do {
		checkValidation = 0;
		printf("\nEnter time in 24 hours format\n");
		printf("Booking hours[08 - 22] [-1 to exit]: ");
		rewind(stdin);
		scanf("%d",&bk.sTime.hour);
		if (bk.sTime.hour == -1) {
			red();
			printf("\aBooking cancelled\n");
			resetColor();
			system("pause");
			return;
		}


		printf("Minutes[00 - 59]: ");
		rewind(stdin);
		scanf("%d",&bk.sTime.minute);
		do{
			printf("Enter usage duration[at least 1 hour]\n");
			printf("Hour(s): ");
			rewind(stdin);
			scanf("%d",&duration);
		} while (duration <= 0);

		validateBookingTime(duration, bk.sTime.hour,bk.sTime.minute,&bk.eTime.hour,&bk.eTime.minute, &checkValidation);
	}while(checkValidation !=0);

	
	int recordFound, invalidStatus = 0;
	Member mem;
	
	do {
		FILE* readMem = fopen("member.txt", "r");
		if (readMem == NULL) {
			red();
			printf("Failed to open file: member.txt");
			resetColor();
			system("pause");
			return;
		}
		checkValidation = 0;
		recordFound = 0;
		printf("Enter member ID who is booking: ");
		rewind(stdin);
		fgets(bk.memID,8,stdin);
		deleteNewLine(bk.memID);
		memIDValidation(bk.memID, &checkValidation);
		if (checkValidation == 0) {
			while (!feof(readMem)) {
				fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
				if (strcmp(mem.ID, bk.memID) == 0) {
					if (strcmp(mem.status, "Active") == 0) {
						++recordFound;
						break;
					}
					if (strcmp(mem.status, "Expired") == 0) {
						++recordFound;
						++invalidStatus;
						break;
					}
					if (strcmp(mem.status, "BLed") == 0 || strcmp(mem.status, "Banned") == 0) {
						++recordFound;
						--invalidStatus;
						break;
					}
				}
			}
			if (recordFound == 0) {
				++checkValidation;
				red();
				printf("\aNo matching member ID found\n");
				resetColor();
				system("pause");
			}
		}
		fclose(readMem);
		if (checkValidation == 0) {
			validateBetweenDate(bk.memID,bk.bDate.month, bk.bDate.day,&checkValidation);
			if (checkValidation != 0) {
				red();
				printf("\aThere is already a booking made around the date\n");
				resetColor();
				system("pause");
				return;
			}
		}
	} while (checkValidation !=0);
	
	if (invalidStatus == 1) {
		red();
		printf("\aThe member's status is expired\n");
		printf("Please inform the member to renew his/her membership to use the club's facility\n");
		resetColor();
		system("pause");
		return;
	}
	if (invalidStatus == -1) {
		red();
		printf("\aThe member is either being blacklisted or banned by the club\n");
		resetColor();
		system("pause");
		return;
	}


	Facility facility;
	Facility* matchFaciType = malloc(100 *sizeof *matchFaciType);
	char faciType[16];
	do {	
		FILE* readFaci = fopen("facilities.txt", "r");
		if (readFaci == NULL) {
			red();
			printf("Failed to open file: facility.txt");
			resetColor();
			system("pause");
			free(matchFaciType);
			return;
		}
		checkValidation = 0;
		recordFound = 0;
		printf("Enter facility type booking: ");
		rewind(stdin);
		fgets(faciType,16,stdin);
		deleteNewLine(faciType);
		validateFaciType(faciType, &checkValidation);
		if (checkValidation == 0) {
			while (!feof(readFaci)) {
				fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
				if (strcmp(facility.type, faciType) == 0) {
					matchFaciType[recordFound] = facility;
					++recordFound;
				}
			}
			if (recordFound == 0) {
				++checkValidation;
				red();
				printf("\aNo matching facility type found\n");
				resetColor();
				system("pause");
			}
		}	
		fclose(readFaci);
	} while (checkValidation != 0);
	

	int noOfRecord;
	
	do{
		checkValidation = 0;
		do{
			for (int i = 0; i < recordFound; i++) {
				printf("No of record: %d\n",i+1);
				printf("Facility ID: %s\nFacility Type: %s\nFaclity Description: %s\nVenue: %s\nMax.User Allowed: %d\nStatus: %s\n\n", matchFaciType[i].ID, matchFaciType[i].type, matchFaciType[i].description, matchFaciType[i].venue, matchFaciType[i].maxUser, matchFaciType[i].status);
			}
			printf("Key in the no of record to book[0 to exit]: ");
			rewind(stdin);
			scanf("%d",&noOfRecord);
			if (noOfRecord == 0) {
				red();
				printf("\aBooking cancelled\n");
				resetColor();
				system("pause");
				free(matchFaciType);
				return;
			}
		} while (!(noOfRecord >= 1 && noOfRecord <= recordFound));

		for (int i = 0; i < recordFound; i++) {
			if (i == noOfRecord - 1) {
				facility = matchFaciType[i];
				break;
			}
		}

		Booking bkRec;
		FILE* readBkRec = fopen("booking.bin", "rb");
		if (readBkRec == NULL) {
			red();
			printf("\aFailed to open file: booking.bin\n");
			resetColor();
			system("pause");
			free(matchFaciType);
			return;
		}
		while (fread(&bkRec, sizeof(Booking), 1, readBkRec) != 0) {
			if (strcmp(bkRec.faciID, facility.ID) == 0 && (bk.sTime.hour >= bkRec.sTime.hour && bk.sTime.hour <= bkRec.eTime.hour) && (bk.sTime.minute >= bkRec.sTime.minute && bk.sTime.minute <= bkRec.eTime.minute)) {
				++checkValidation;
				red();
				printf("\aPlease proceed to other facility ID as the selected facility will not be available at the timing\n");
				resetColor();
				system("pause");
			}
		}
		fclose(readBkRec);
		if (checkValidation == 0) {
			strcpy(bk.faciID,facility.ID);
		}
	} while (checkValidation != 0);

	
	do{
		checkValidation = 0;
		printf("Enter the number of people coming for that day: \n");
		rewind(stdin);
		scanf("%d", &bk.noOfPeople);
		if (bk.noOfPeople <= 0) {
			++checkValidation;
			red();
			printf("\aInvalid number of people\n");
			resetColor();
			system("pause");
		}
		if (bk.noOfPeople > facility.maxUser) {
			++checkValidation;
			red();
			printf("\aMaximum user allowed is %d\n",facility.maxUser);
			resetColor();
			system("pause");	
		}	
	} while (checkValidation != 0);

	system("cls");
	printf("New booking details\n");
	printf("Booking ID: %s\nBooking made on: %02d-%02d-%d\nDate booking for: %02d-%02d-%d\nBooking time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n",bk.bID, bk.tDate.day, bk.tDate.month, bk.tDate.year,bk.bDate.day,bk.bDate.month,bk.bDate.year,bk.sTime.hour,bk.sTime.minute,bk.eTime.hour,bk.eTime.minute,bk.memID,bk.faciID,bk.noOfPeople);
	printf("\n\nConfirm to add the booking?\n");
	printf("\nPress 'Y' to add the new booking, other key to exit\n");
	char confirm;
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm != 'Y') {
		red();
		printf("\aBooking cancelled\n");
		resetColor();
		free(matchFaciType);
		system("pause");
		return;
	}

	FILE* writeRec = fopen("booking.bin","ab");
	if (writeRec == NULL) {
		red();
		printf("\aFailed to open file: booking.bin\n");
		resetColor();
		system("pause");
		free(matchFaciType);
		return;
	}
	fwrite(&bk,sizeof(Booking),1,writeRec);
	fclose(writeRec);
	red();
	printf("\aBooking succeed\n");
	resetColor();
	system("pause");
	free(matchFaciType);
}

void walkIn() {
	system("cls");
	WalkIn wkIn;
	autoGenerateWalkInID(wkIn.wID);
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysDay = time.tm_mday;
	int sysYear = time.tm_year + 1900;
	int sysHour = time.tm_hour;
	int sysMinute = time.tm_min;
	
	if (sysHour == 21) {
		red();
		printf("\aClub is closing at 22:00(10P.M.)\n");
		resetColor();
		system("pause");
		return;
	}
	wkIn.tDate.day = sysDay;
	wkIn.tDate.month = sysMonth;
	wkIn.tDate.year = sysYear;

	wkIn.sTime.hour = sysHour;
	wkIn.sTime.minute = sysMinute;
	
	int checkValidation,duration;

	do {
		checkValidation = 0;
		do {
			printf("Enter usage duration[at least 1 hour]\n");
			printf("Hour(s): ");
			rewind(stdin);
			scanf("%d", &duration);
		} while (duration <= 0);
		validateBookingTime(duration, wkIn.sTime.hour, wkIn.sTime.minute, &wkIn.eTime.hour, &wkIn.eTime.minute, &checkValidation);
	} while (checkValidation != 0);


	int recordFound, invalidStatus = 0;
	Member mem;

	do {
		FILE* readMem = fopen("member.txt", "r");
		if (readMem == NULL) {
			red();
			printf("Failed to open file: member.txt");
			resetColor();
			system("pause");
			return;
		}
		checkValidation = 0;
		recordFound = 0;
		printf("Enter member ID who is booking: ");
		rewind(stdin);
		fgets(wkIn.memID, 8, stdin);
		deleteNewLine(wkIn.memID);
		memIDValidation(wkIn.memID, &checkValidation);
		if (checkValidation == 0) {
			while (!feof(readMem)) {
				fscanf(readMem, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^\n]\n", mem.ID, mem.name, &mem.gender, mem.IC, mem.contactNo, &mem.regMonths, &mem.startDate.day, &mem.startDate.month, &mem.startDate.year, &mem.endDate.day, &mem.endDate.month, &mem.endDate.year, mem.status);
				if (strcmp(mem.ID, wkIn.memID) == 0) {
					if (strcmp(mem.status, "Active") == 0) {
						++recordFound;
						break;
					}
					if (strcmp(mem.status, "Expired") == 0) {
						++recordFound;
						++invalidStatus;
						break;
					}
					if (strcmp(mem.status, "BLed") == 0 || strcmp(mem.status, "Banned") == 0) {
						++recordFound;
						--invalidStatus;
						break;
					}
				}
			}
			if (recordFound == 0) {
				++checkValidation;
				red();
				printf("\aNo matching member ID found\n");
				resetColor();
				system("pause");
			}
		}
		fclose(readMem);
	} while (checkValidation != 0);
	if (invalidStatus == 1) {
		red();
		printf("\aThe member's status is expired\n");
		printf("Please inform the member to renew his/her membership to use the club's facility\n");
		resetColor();
		system("pause");
		return;
	}
	if (invalidStatus == -1) {
		red();
		printf("\aThe member is either being blacklisted or banned by the club\n");
		resetColor();
		system("pause");
		return;
	}

	Facility facility;
	Facility* matchFaciType = malloc(100 * sizeof * matchFaciType);
	char faciType[16];
	do {
		FILE* readFaci = fopen("facilities.txt", "r");
		if (readFaci == NULL) {
			red();
			printf("Failed to open file: facility.txt");
			resetColor();
			system("pause");
			free(matchFaciType);
			return;
		}

		checkValidation = 0;
		recordFound = 0;
		printf("Enter facility type booking: ");
		rewind(stdin);
		fgets(faciType, 16, stdin);
		deleteNewLine(faciType);
		validateFaciType(faciType, &checkValidation);
		if (checkValidation == 0) {
			while (!feof(readFaci)) {
				fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility.ID, facility.type, facility.description, facility.venue, &facility.maxUser, facility.status);
				if (strcmp(facility.type, faciType) == 0) {
					if(strcmp(facility.status,"Operating") == 0){
					matchFaciType[recordFound] = facility;
					++recordFound;
					}
				}
			}
			if (recordFound == 0) {
				++checkValidation;
				red();
				printf("\aNo matching facility type found or all the facility specified are in use\n");
				resetColor();
				system("pause");
			}
		}
		fclose(readFaci);
	} while (checkValidation != 0);


	int noOfRecord;
	do {
			for (int i = 0; i < recordFound; i++) {
				printf("No of record: %d\n", i + 1);
				printf("Facility ID: %s\nFacility Type: %s\nFaclity Description: %s\nVenue: %s\nMax.User Allowed: %d\nStatus: %s\n\n", matchFaciType[i].ID, matchFaciType[i].type, matchFaciType[i].description, matchFaciType[i].venue, matchFaciType[i].maxUser, matchFaciType[i].status);
			}
			printf("Key in the no of record to book[0 to exit]: ");
			rewind(stdin);
			scanf("%d", &noOfRecord);
			if (noOfRecord == 0) {
				red();
				printf("\aBooking cancelled\n");
				resetColor();
				system("pause");
				free(matchFaciType);
				return;
			}
		} while (!(noOfRecord >= 1 && noOfRecord <= recordFound));
	for (int i = 0; i < recordFound; i++) {
			if (i == noOfRecord - 1) {
				facility = matchFaciType[i];
				break;
			}
		}

	strcpy(wkIn.faciID, facility.ID);

	do {
		checkValidation = 0;
		printf("Enter the number of people coming for that day: \n");
		rewind(stdin);
		scanf("%d", &wkIn.noOfPeople);
		if (wkIn.noOfPeople <= 0) {
			++checkValidation;
			red();
			printf("\aInvalid number of people\n");
			resetColor();
			system("pause");
		}
		if (wkIn.noOfPeople > facility.maxUser) {
			++checkValidation;
			red();
			printf("\aMaximum user allowed is %d\n", facility.maxUser);
			resetColor();
			system("pause");
		}
	} while (checkValidation != 0);

	system("cls");
	printf("New booking details\n");
	printf("Walk in ID: %s\nWalk in on: %02d-%02d-%d\nUsage time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", wkIn.wID, wkIn.tDate.day, wkIn.tDate.month, wkIn.tDate.year, wkIn.sTime.hour, wkIn.sTime.minute, wkIn.eTime.hour, wkIn.eTime.minute, wkIn.memID, wkIn.faciID, wkIn.noOfPeople);
	printf("\n\nConfirm to add the walk in?\n");
	printf("\nPress 'Y' to add the walk in, other key to exit\n");
	char confirm;
	confirm = getch();
	confirm = toupper(confirm);
	if (confirm != 'Y') {
		red();
		printf("\Walk in cancelled\n");
		resetColor();
		free(matchFaciType);
		system("pause");
		return;
	}

	FILE* writeRec = fopen("walkin.bin", "ab");
	if (writeRec == NULL) {
		red();
		printf("\aFailed to open file: walkin.bin\n");
		resetColor();
		system("pause");
		free(matchFaciType);
		return;
	}
	fwrite(&wkIn, sizeof(WalkIn), 1, writeRec);
	fclose(writeRec);
	red();
	printf("\Walk in recorded\n");
	resetColor();
	system("pause");
	free(matchFaciType);
}

void displayBooking() {
	system("cls");
	Booking bk;
	FILE* readBooking = fopen("booking.bin", "rb");
	if (readBooking == NULL) {
		red();
		printf("\aFailed to open file: booking.bin\n");
		resetColor();
		system("pause");
		return;
	}
	while (fread(&bk, sizeof(Booking), 1, readBooking) != 0) {
		printf("Booking ID: %s\nBooking made on: %02d-%02d-%d\nDate booking for: %02d-%02d-%d\nBooking time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", bk.bID, bk.tDate.day, bk.tDate.month, bk.tDate.year, bk.bDate.day, bk.bDate.month, bk.bDate.year, bk.sTime.hour, bk.sTime.minute, bk.eTime.hour, bk.eTime.minute, bk.memID, bk.faciID, bk.noOfPeople);
	}
	fclose(readBooking);
	system("pause");
}

void displayWalkIn() {
	system("cls");
	WalkIn wkIn;
	FILE* readWkIn = fopen("walkin.bin", "rb");
	if (readWkIn == NULL) {
		red();
		printf("\aFailed to open file: walkin.bin\n");
		resetColor();
		system("pause");
		return;
	}
	while (fread(&wkIn, sizeof(WalkIn), 1, readWkIn) != 0) {
		printf("Walk in ID: %s\nWalk in made on: %02d-%02d-%d\nUsage time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", wkIn.wID, wkIn.tDate.day, wkIn.tDate.month, wkIn.tDate.year, wkIn.sTime.hour, wkIn.sTime.minute, wkIn.eTime.hour, wkIn.eTime.minute, wkIn.memID, wkIn.faciID, wkIn.noOfPeople);
	}
	fclose(readWkIn);
	system("pause");
}

void searchBooking() {
	system("cls");
	int searchOpt;
	do{
		printf("Search Record By Booking ID\n");
		printf("0. Exit\n");
		printf("1. Search current existing booking\n");
		printf("2. Search booking historic record\n");
		printf("Key in the number as assigned: ");
		rewind(stdin);
		scanf("%d", &searchOpt);
		if (searchOpt == 0) {
			red();
			printf("Exitted");
			resetColor();
			system("pause");
			return;
		}
	} while (!(searchOpt >= 1 && searchOpt <= 2));
	char bkID[11];
	int checkValidation;
	do{
		checkValidation = 0;
		printf("Enter the Booking ID seaching for: ");
		rewind(stdin);
		fgets(bkID, 11, stdin);
		deleteNewLine(bkID);
		validateBookingID(bkID, &checkValidation);
	} while (checkValidation != 0);
	
	int recordFound = 0;

	if(searchOpt == 1){
	Booking bk;
	FILE* readBooking = fopen("booking.bin", "rb");
	if (readBooking == NULL) {
		red();
		printf("\aFailed to open file\n");
		resetColor();
		system("pause");
		return;
	}
	while (fread(&bk, sizeof(Booking), 1, readBooking) != 0) {
		if(strcmp(bk.bID,bkID)==0){
			++recordFound;
			printf("Booking ID: %s\nBooking made on: %02d-%02d-%d\nDate booking for: %02d-%02d-%d\nBooking time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", bk.bID, bk.tDate.day, bk.tDate.month, bk.tDate.year, bk.bDate.day, bk.bDate.month, bk.bDate.year, bk.sTime.hour, bk.sTime.minute, bk.eTime.hour, bk.eTime.minute, bk.memID, bk.faciID, bk.noOfPeople);
		}
	}
	fclose(readBooking);
	if (recordFound == 0) {
		red();
		printf("\aNo matching booking ID found\n");
		resetColor();
	}
	}

	if(searchOpt == 2) {
		Booking bk;
		FILE* readBooking = fopen("BookingFacilityUsage.bin", "rb");
		if (readBooking == NULL) {
			red();
			printf("\aFailed to open file\n");
			resetColor();
			system("pause");
			return;
		}
		while (fread(&bk, sizeof(Booking), 1, readBooking) != 0) {
			if (strcmp(bk.bID, bkID) == 0) {
				++recordFound;
				printf("Booking ID: %s\nBooking made on: %02d-%02d-%d\nDate booking for: %02d-%02d-%d\nBooking time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", bk.bID, bk.tDate.day, bk.tDate.month, bk.tDate.year, bk.bDate.day, bk.bDate.month, bk.bDate.year, bk.sTime.hour, bk.sTime.minute, bk.eTime.hour, bk.eTime.minute, bk.memID, bk.faciID, bk.noOfPeople);
			}
		}
		fclose(readBooking);
		if (recordFound == 0) {
			red();
			printf("\aNo matching booking ID found\n");
			resetColor();
		}
	}

	system("pause");
}

void searchWalkIn() {
	system("cls");
	int searchOpt;
	do {
		printf("Search Record By Walk In ID\n");
		printf("0. Exit\n");
		printf("1. Search current existing walk in\n");
		printf("2. Search walk in historic record\n");
		printf("Key in the number as assigned: ");
		rewind(stdin);
		scanf("%d", &searchOpt);
		if (searchOpt == 0) {
			red();
			printf("Exitted");
			resetColor();
			system("pause");
			return;
		}
	} while (!(searchOpt >= 1 && searchOpt <= 2));
	char wkInID[11];
	int checkValidation;
	do {
		checkValidation = 0;
		printf("Enter the Booking ID seaching for: ");
		rewind(stdin);
		fgets(wkInID, 11, stdin);
		deleteNewLine(wkInID);
		validateWalkInID(wkInID, &checkValidation);
	} while (checkValidation != 0);

	int recordFound = 0;

	if (searchOpt == 1) {
		WalkIn wkIn;
		FILE* readWkIn = fopen("walkin.bin", "rb");
		if (readWkIn == NULL) {
			red();
			printf("\aFailed to open file\n");
			resetColor();
			system("pause");
			return;
		}
		while (fread(&wkIn, sizeof(WalkIn), 1, readWkIn) != 0) {
			if (strcmp(wkIn.wID, wkInID) == 0) {
				++recordFound;
				printf("Walk in ID: %s\nWalk in made on: %02d-%02d-%d\nUsage time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", wkIn.wID, wkIn.tDate.day, wkIn.tDate.month, wkIn.tDate.year, wkIn.sTime.hour, wkIn.sTime.minute, wkIn.eTime.hour, wkIn.eTime.minute, wkIn.memID, wkIn.faciID, wkIn.noOfPeople);
			}
		}
		fclose(readWkIn);
		if (recordFound == 0) {
			red();
			printf("\aNo matching booking ID found\n");
			resetColor();
		}
	}

	if (searchOpt == 2) {
		WalkIn wkIn;
		FILE* readWkIn = fopen("WalkinFacilityUsage.bin", "rb");
		if (readWkIn == NULL) {
			red();
			printf("\aFailed to open file\n");
			resetColor();
			system("pause");
			return;
		}
		while (fread(&wkIn, sizeof(WalkIn), 1, readWkIn) != 0) {
			if (strcmp(wkIn.wID, wkInID) == 0) {
				++recordFound;
				printf("Walk in ID: %s\nWalk in made on: %02d-%02d-%d\nUsage time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", wkIn.wID, wkIn.tDate.day, wkIn.tDate.month, wkIn.tDate.year, wkIn.sTime.hour, wkIn.sTime.minute, wkIn.eTime.hour, wkIn.eTime.minute, wkIn.memID, wkIn.faciID, wkIn.noOfPeople);
			}
		}
		fclose(readWkIn);
		if (recordFound == 0) {
			red();
			printf("\aNo matching booking ID found\n");
			resetColor();
		}
	}

	system("pause");

}

void searchRecByMemID() {
	system("cls");
	int searchOpt;
	do {
		printf("Search Record By Member ID\n");
		printf("0. Exit\n1. Existing Booking\n 2. Existing Walk In\n 3. Booking Historic Record\n 4. Walk In Historic Record\n");
		printf("Please key in the digit as assigned: ");
		rewind(stdin);
		scanf("%d", &searchOpt);
		if (searchOpt == 0) {
			red();
			printf("\aExitted\n");
			resetColor();
			system("pause");
			return;
		}

	} while (searchOpt >= 1 && searchOpt <= 4);

	int checkValidation;
	char memID[8];
	do {
		printf("Enter the member ID you wish to search: ");
		rewind(stdin);
		fgets(memID, 8, stdin);
		deleteNewLine(memID);
		memIDValidation(memID,&checkValidation);
	} while (checkValidation != 0);

	if (searchOpt == 1) {	
		FILE* readBk = fopen("booking.bin","rb");
		if (readBk == NULL) {
			red();
			printf("\aFailed to open file: booking.bin\n");
			resetColor();
			system("pause");
			return;
		}
		Booking rec;
		Booking* bk = malloc(100 * sizeof * bk);
		int matchRec = 0;
		while (fread(&rec,sizeof(Booking),1,readBk) != 0) {
			if(strcmp(rec.memID,memID) == 0){
				bk[matchRec] = rec;
				++matchRec;
			}
		}
		fclose(readBk);

		for (int i = 0; i < matchRec; i++) {
			printf("Booking ID: %s\nBooking made on: %02d-%02d-%d\nDate booking for: %02d-%02d-%d\nBooking time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", bk[i].bID, bk[i].tDate.day, bk[i].tDate.month, bk[i].tDate.year, bk[i].bDate.day, bk[i].bDate.month, bk[i].bDate.year, bk[i].sTime.hour, bk[i].sTime.minute, bk[i].eTime.hour, bk[i].eTime.minute, bk[i].memID, bk[i].faciID, bk[i].noOfPeople);
		}
		free(bk);
	}

	if (searchOpt == 2) {
		FILE* readWkin = fopen("walkin.bin", "rb");
		if (readWkin == NULL) {
			red();
			printf("\aFailed to open file: walkin.bin\n");
			resetColor();
			system("pause");
			return;
		}
		WalkIn rec;
		WalkIn* wkIn = malloc(100 * sizeof * wkIn);
		int matchRec = 0;
		while (fread(&rec, sizeof(Booking), 1, readWkin) != 0) {
			if (strcmp(rec.memID, memID) == 0) {
				wkIn[matchRec] = rec;
				++matchRec;
			}
		}
		fclose(readWkin);

		for (int i = 0; i < matchRec; i++) {
			printf("Walk in ID: %s\nWalk in made on: %02d-%02d-%d\nUsage time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", wkIn[i].wID, wkIn[i].tDate.day, wkIn[i].tDate.month, wkIn[i].tDate.year, wkIn[i].sTime.hour, wkIn[i].sTime.minute, wkIn[i].eTime.hour, wkIn[i].eTime.minute, wkIn[i].memID, wkIn[i].faciID, wkIn[i].noOfPeople);
		}
		free(wkIn);
	}

	if (searchOpt == 3) {
		FILE* readBk = fopen("BookingFacilityUsage.bin", "rb");
		if (readBk == NULL) {
			red();
			printf("\aFailed to open file: BookingFacilityUsage.bin\n");
			resetColor();
			system("pause");
			return;
		}
		Booking rec;
		Booking* bk = malloc(100 * sizeof * bk);
		int matchRec = 0;
		while (fread(&rec, sizeof(Booking), 1, readBk) != 0) {
			if (strcmp(rec.memID, memID) == 0) {
				bk[matchRec] = rec;
				++matchRec;
			}
		}
		fclose(readBk);

		for (int i = 0; i < matchRec; i++) {
			printf("Booking ID: %s\nBooking made on: %02d-%02d-%d\nDate booking for: %02d-%02d-%d\nBooking time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", bk[i].bID, bk[i].tDate.day, bk[i].tDate.month, bk[i].tDate.year, bk[i].bDate.day, bk[i].bDate.month, bk[i].bDate.year, bk[i].sTime.hour, bk[i].sTime.minute, bk[i].eTime.hour, bk[i].eTime.minute, bk[i].memID, bk[i].faciID, bk[i].noOfPeople);
		}
		free(bk);
	}

	if (searchOpt == 4) {
		FILE* readWkin = fopen("WalkinFacilityUsage.bin", "rb");
		if (readWkin == NULL) {
			red();
			printf("\aFailed to open file: WalkinFacilityUsage.bin\n");
			resetColor();
			system("pause");
			return;
		}
		WalkIn rec;
		WalkIn* wkIn = malloc(100 * sizeof * wkIn);
		int matchRec = 0;
		while (fread(&rec, sizeof(Booking), 1, readWkin) != 0) {
			if (strcmp(rec.memID, memID) == 0) {
				wkIn[matchRec] = rec;
				++matchRec;
			}
		}
		fclose(readWkin);

		for (int i = 0; i < matchRec; i++) {
			printf("Walk in ID: %s\nWalk in made on: %02d-%02d-%d\nUsage time from: %02d:%02d\nTo: %02d:%02d\nBooking By Member ID: %s\nFacility ID: %s\nNo Of People: %d\n\n", wkIn[i].wID, wkIn[i].tDate.day, wkIn[i].tDate.month, wkIn[i].tDate.year, wkIn[i].sTime.hour, wkIn[i].sTime.minute, wkIn[i].eTime.hour, wkIn[i].eTime.minute, wkIn[i].memID, wkIn[i].faciID, wkIn[i].noOfPeople);
		}
		free(wkIn);
	}
	system("pause");
}

void modifyBooking() {
	char bkID[11];
	int checkValidation;
	do {
		system("cls");
		checkValidation = 0;
		printf("Enter the booking ID you wish to modify: ");
		rewind(stdin);
		fgets(bkID, 11,stdin);
		deleteNewLine(bkID);
		validateBookingID(bkID, &checkValidation);
	} while (checkValidation != 0);


}

void modifyWalkIn() {

}
//helper functions
void autoGenerateWalkInID(char input[]) {
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysDay = time.tm_mday;
	int sysYear = time.tm_year + 1900;

	int extractYear = sysYear % 1000;
	char yy[3];
	sprintf(yy,"%02d",extractYear);
	

	char mm[3];
	sprintf(mm,"%02d",sysMonth);
	
	char dd[3]; 
	sprintf(dd,"%02d",sysDay);
		
	char todayWalkInID[8];
	sprintf(todayWalkInID, "W%s%s%s", yy, mm, dd);

	int recordFound = 0;
	WalkIn bk;
	char newID;
	FILE* readWalkIn = fopen("walkin.bin","rb");
	if (readWalkIn == NULL) {
		red();
		printf("\aFailed to open file: walkIn.bin\n");
		resetColor();
		system("pause");
		return;
	}
	while (fread(&bk, sizeof(WalkIn), 1, readWalkIn) != 0){
		char extractWalkingIDDate[8] = {bk.wID[0], bk.wID[1] ,bk.wID[2] ,bk.wID[3] ,bk.wID[4] ,bk.wID[5] ,bk.wID[6]};
		if (strcmp(extractWalkingIDDate, todayWalkInID) == 0) {
			++recordFound;
		}
	}
	fclose(readWalkIn);

	if (recordFound == 0) {
		sprintf(input, "%s001", todayWalkInID);
		return;
	}
	else{
		char extractSeq[5] = {bk.wID[7],bk.wID[8] ,bk.wID[9] ,bk.wID[10] };
		int currentSeq = atoi(extractSeq);
		int newSeq = currentSeq + 1;	
		sprintf(input,"%s%03d", todayWalkInID,newSeq);
		return;
	}
	
}
void autoGenerateBookingID(char input[], int year, int month) {
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysDay = time.tm_mday;
	int sysYear = time.tm_year + 1900;

	int extractYear = year % 1000;
	char yy[3];
	sprintf(yy, "%02d", extractYear);


	char mm[3];
	sprintf(mm, "%02d", month);


	char dateBookingID[8];
	sprintf(dateBookingID, "B%s%s", yy, mm);

	int recordFound = 0;
	Booking bk;
	char newID;
	FILE* readBooking = fopen("booking.bin", "rb");
	if (readBooking == NULL) {
		red();
		printf("\aFailed to open file\n");
		resetColor();
		system("pause");
		return;
	}
	while (fread(&bk, sizeof(Booking), 1, readBooking) != 0) {
		char extractBookingIDDate[8] = { bk.bID[0], bk.bID[1] ,bk.bID[2] ,bk.bID[3] ,bk.bID[4]};
		if (strcmp(extractBookingIDDate, dateBookingID) == 0) {
			++recordFound;
		}
	}
	fclose(readBooking);

	if (recordFound == 0) {
		sprintf(input, "%s00001", dateBookingID);
		return;
	}
	else {
		char extractSeq[6] = { bk.bID[6],bk.bID[7],bk.bID[8] ,bk.bID[9] ,bk.bID[10] };
		int currentSeq = atoi(extractSeq);
		int newSeq = currentSeq + 1;
		sprintf(input, "%s%05d", dateBookingID, newSeq);
		return;
	}

}
void validateBookingTime(int duration, int sHour, int sMin, int *eHour, int *eMin, int* checkValidation) {
	*eHour = sHour + duration;
	*eMin = sMin;

	if (sHour < 8 || sHour >= 22) {
		++*checkValidation;
		red();
		printf("\aClub open from 0800 to 2200 only\n");
		resetColor();
		system("pause");
		return;
	}

	if (sMin < 0 || sMin > 59) {
		++* checkValidation;
		red();
		printf("\aIllogical minutes inputted\n");
		resetColor();
		system("pause");
		return;
	}

	if (*eHour > 22) {
		++*checkValidation;
		red();
		printf("\aClub close at 2200\n");
		resetColor();
		system("pause");
		return;
	}

	if (*eHour == 22 && *eMin > 0) {
		++* checkValidation;
		red();
		printf("\aClub close at 2200\n");
		resetColor();
		system("pause");
		return;
	}
}
void validateBookingID(char input[], int* checkValidation) {
	if (strlen(input) != 10) {
		++* checkValidation;
		red();
		printf("\aExpected length of 10\n");
		resetColor();
		system("pause");
		return;
	}

	if (input[0] != 'b' && input[0] != 'B') {
		++* checkValidation;
		red();
		printf("\aInput on position 1 must be B\n");
		resetColor();
		system("pause");
		return;
	}

	if (input[0] == 'b') {
		input[0] = toupper(input[0]);
	}
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysYear = time.tm_year + 1900;
	int sysDay = time.tm_mday;

	char yy[3] = { input[1],input[2] };
	int year = atoi(yy);
	if (year != (sysYear % 1000)) {
		++* checkValidation;
		red();
		printf("\aPlease enter current year only on input position 2 and 3\n");
		resetColor();
		system("pause");
		return;
	}

	char mm[3] = { input[3],input[4]};
	int month = atoi(mm);
	if (month < sysMonth || month > 12) {
		++* checkValidation;
		red();
		printf("\aIllogical month input on input position 4 and 5\n");
		resetColor();
		system("pause");
		return;
	}

	/*
	char dd[3] = { input[5],input[6] };
	int day = atoi(dd);
		
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
			printf("\aIllogical input of day\n");
			resetColor();
			system("pause");
			return;
		}
	}
	if (month == 2) {
		if (((sysYear % 4 == 0) && ((sysYear % 400 == 0) || (sysYear % 100 != 0)))) {
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

	}*/

	for (int i = 5; i < strlen(input); i++) {
		if (!(input[i] >= 48 && input[i] <= 57)) {
			++*checkValidation;
			red();
			printf("\aExpect numeric input only on input position 6 - 10\n");
			resetColor();
			system("pause");
			return;
		}

	}
}
void validateWalkInID(char input[], int* checkValidation) {
	if (strlen(input) != 10) {
		++* checkValidation;
		red();
		printf("\aExpected length of 10\n");
		resetColor();
		system("pause");
		return;
	}

	if (input[0] != 'W' && input[0] != 'w') {
		++* checkValidation;
		red();
		printf("\aInput on position 1 must be B\n");
		resetColor();
		system("pause");
		return;
	}

	if (input[0] == 'w') {
		input[0] = toupper(input[0]);
	}
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysYear = time.tm_year + 1900;
	int sysDay = time.tm_mday;

	char yy[3] = { input[1],input[2] };
	int year = atoi(yy);
	if (year != (sysYear % 1000)) {
		++* checkValidation;
		red();
		printf("\aPlease enter current year only on input position 2 and 3\n");
		resetColor();
		system("pause");
		return;
	}

	char mm[3] = { input[3],input[4] };
	int month = atoi(mm);
	if (month < sysMonth || month > 12) {
		++* checkValidation;
		red();
		printf("\aIllogical month input on input position 4 and 5\n");
		resetColor();
		system("pause");
		return;
	}

	char dd[3] = { input[5],input[6] };
	int day = atoi(dd);

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
			printf("\aIllogical input of day\n");
			resetColor();
			system("pause");
			return;
		}
	}
	if (month == 2) {
		if (((sysYear % 4 == 0) && ((sysYear % 400 == 0) || (sysYear % 100 != 0)))) {
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

	for (int i = 7; i < strlen(input); i++) {
		if (!(input[i] >= 48 && input[i] <= 57)) {
			++* checkValidation;
			red();
			printf("\aExpect numeric input only on input position 8 - 10\n");
			resetColor();
			system("pause");
			return;
		}

	}
}
void validateBetweenDate(char memID[], int bookingForMonth, int bookingForDay, int *checkValidation) {
	int dayRange = 5;
	Booking bkRec;
	FILE* readBkRec = fopen("booking.bin", "rb");
	if (readBkRec == NULL) {
		red();
		printf("\aFailed to open file: booking.bin\n");
		resetColor();
		return;
	}
	int beforeMonth, beforeDay, afterMonth, afterDay;
	while (fread(&bkRec, sizeof(Booking),1,readBkRec) != 0) {
		if (strcmp(bkRec.memID, memID) == 0) {
			rangeBeforeTheDate(dayRange, bkRec.bDate.month, bkRec.bDate.day, &beforeMonth, &beforeDay);
			rangeAfterTheDate(dayRange,bkRec.bDate.month,bkRec.bDate.day,&afterMonth, &afterDay);
			if(beforeMonth < bkRec.bDate.month){
				if (beforeMonth == 1 || beforeMonth == 3 || beforeMonth == 5 || beforeMonth == 7 || beforeMonth == 10 || beforeMonth == 12) {
					if ((bookingForDay >= beforeDay && bookingForDay <= 31) || (bookingForDay >= 1 && bookingForDay <= bkRec.bDate.day) || (bookingForDay > bkRec.bDate.day && bookingForDay <= afterDay)) {
						++* checkValidation;
					}
				}
				if (beforeMonth == 4 || beforeMonth == 6 || beforeMonth == 9 || beforeMonth == 11) {
					if ((bookingForDay >= beforeDay && bookingForDay <= 30) || (bookingForDay >= 1 && bookingForDay <= bkRec.bDate.day) || (bookingForDay > bkRec.bDate.day && bookingForDay <= afterDay)) {
						++* checkValidation;
					}
				}
				if (beforeMonth == 2) {
					time_t t = time(NULL);
					struct tm time = *localtime(&t);
					int sysYear = time.tm_year + 1900;
					if (((sysYear % 4 == 0) && ((sysYear % 400 == 0) || (sysYear % 100 != 0)))) {
						if ((bookingForDay >= beforeDay && bookingForDay <= 29) || (bookingForDay >= 1 && bookingForDay <= bkRec.bDate.day) || (bookingForDay > bkRec.bDate.day && bookingForDay <= afterDay)) {
							++* checkValidation;
						}
					}
					else {
						if ((bookingForDay >= beforeDay && bookingForDay <= 28) || (bookingForDay >= 1 && bookingForDay <= bkRec.bDate.day) || (bookingForDay > bkRec.bDate.day && bookingForDay <= afterDay)) {
							++* checkValidation;
						}
					}
				}
			
			}
			if(afterMonth > bkRec.bDate.month){
				if (afterMonth == 3 || afterMonth == 5 || afterMonth == 7 || afterMonth == 10 || afterMonth == 12) {
					if ((bookingForDay >= beforeDay && bookingForDay <= bkRec.bDate.day) || (bookingForDay > bkRec.bDate.day && bookingForDay <= 30) || (bookingForDay >= 1 && bookingForDay <= afterDay)) {
						++* checkValidation;
					}
				}
				if (afterMonth == 4 || afterMonth == 6 || afterMonth == 8 || afterMonth == 9 ||afterMonth == 11) {
					if ((bookingForDay >= beforeDay && bookingForDay <= bkRec.bDate.day) || (bookingForDay > bkRec.bDate.day && bookingForDay <= 31) || (bookingForDay >= 1 && bookingForDay <= afterDay)) {
						++* checkValidation;
					}
				}
				if (afterMonth == 2) {
					time_t t = time(NULL);
					struct tm time = *localtime(&t);
					int sysYear = time.tm_year + 1900;
					if (((sysYear % 4 == 0) && ((sysYear % 400 == 0) || (sysYear % 100 != 0)))) {
						if ((bookingForDay >= beforeDay && bookingForDay <= bkRec.bDate.day) || (bookingForDay > bkRec.bDate.day && bookingForDay <= 29) || (bookingForDay >= 1 && bookingForDay <= afterDay)) {
							++* checkValidation;
						}
					}
					else {
						if ((bookingForDay >= beforeDay && bookingForDay <= bkRec.bDate.day) || (bookingForDay > bkRec.bDate.day && bookingForDay <= 28) || (bookingForDay >= 1 && bookingForDay <= afterDay)) {
							++* checkValidation;
						}
					}
				}
			}
			if(beforeMonth == afterMonth) {
				if ((bookingForDay >= beforeDay && bookingForDay <= bkRec.bDate.day) || (bookingForDay > bkRec.bDate.day && bookingForDay <= afterDay)) {
					++* checkValidation;
				}
			}
		break;
		}	
	}
	fclose(readBkRec);
}
void updateFacilityStatus() {
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysDay = time.tm_mday;
	int sysYear = time.tm_year + 1900;
	int sysHour = time.tm_hour;
	int sysMinute = time.tm_min;
	Facility *facility = malloc(100 *sizeof *facility);
	
	FILE* readFaci;
	int totalFaciCount = 0;
	readFaci = fopen("facilities.txt","r");
	if (readFaci == NULL) {
		red();
		printf("\aFailed to open file: facilities.txt\n");
		resetColor();
		system("pause");
		free(facility);
		return;
	}
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility[totalFaciCount].ID, facility[totalFaciCount].type, facility[totalFaciCount].description, facility[totalFaciCount].venue, &facility[totalFaciCount].maxUser, facility[totalFaciCount].status);
		++totalFaciCount;
	}
	fclose(readFaci);

	//try array
	FILE* readWkIn = fopen("walkin.bin","rb");
	if (readWkIn == NULL) {
		red();
		printf("\aFailed to open file: walkin.bin\n");
		resetColor();
		free(facility);
		return;
	}
	WalkIn *wkIn = malloc(100 *sizeof *wkIn);
	int totalWalkInRec = 0;
	while (fread(&wkIn[totalWalkInRec], sizeof(WalkIn), 1, readWkIn) != 0) {
		++totalWalkInRec;
	}
	fclose(readWkIn);

	
	for (int i = 0; i < totalFaciCount; i++) {
		for(int j=0;j<totalWalkInRec;j++){
			if (strcmp(facility[i].ID,wkIn[j].faciID) == 0) {
				if (wkIn[j].tDate.day == sysDay && wkIn[j].tDate.month == sysMonth && wkIn[j].tDate.year == sysYear) {
					if ((sysHour >= wkIn[j].sTime.hour && sysHour <= wkIn[j].eTime.hour) && strcmp(facility[i].status,"Operating") == 0){
						//(sysHour <= wkIn[j].eTime.hour && sysMinute <= wkIn[j].eTime.minute) && 
						strcpy(facility[i].status,"In Use");
					}
				}
			}
			if (strcmp(wkIn[j].faciID, facility[i].ID) == 0) {
				if (sysDay == wkIn[j].tDate.day && sysMonth == wkIn[j].tDate.month && sysYear == wkIn[j].tDate.year) {
					if ((sysHour >= wkIn[j].eTime.hour && sysMinute > wkIn[j].eTime.minute) && strcmp(facility[i].status, "In Use") == 0) {
						strcpy(facility[i].status, "Operating");
					}
				}
			}
		}
	}

	FILE* writeWalkInRec = fopen("walkin.bin", "wb");
	FILE* appendRec = fopen("WalkinFacilityUsage.bin", "ab");
	if (writeWalkInRec == NULL) {
		red();
		printf("\aFailed to open file: walkin.bin\n");
		resetColor();
		system("pause");
		free(facility);
		free(wkIn);
		return;
	}
	if (appendRec == NULL) {
		red();
		printf("\aFailed to open file: WalkinFacilityUsage.bin\n");
		resetColor();
		system("pause");
		free(facility);
		free(wkIn);
		return;
	}
	for (int i = 0; i < totalWalkInRec; i++) {	
		
		if (sysYear > wkIn[i].tDate.year || sysMonth > wkIn[i].tDate.month || (sysMonth == wkIn[i].tDate.month && sysDay > wkIn[i].tDate.day)) {
			fwrite(&wkIn[i], sizeof(WalkIn),1,appendRec);
		}
		else {
			fwrite(&wkIn[i],sizeof(WalkIn), 1, writeWalkInRec);
		}
	}
	fclose(writeWalkInRec);
	fclose(appendRec);
	free(wkIn);
	/*
	for (int i = 0; i < totalFaciCount; i++) {
		printf("faci ID: %s faci Status: %s\n", facility[i].ID, facility[i].status);
	}
	system("pause");*/
	Booking *bk = malloc(100 *sizeof *bk);
	FILE* readBk = fopen("booking.bin", "rb");
	if (readWkIn == NULL) {
		red();
		printf("\aFailed to open file: booking.bin\n");
		resetColor();
		free(facility);
		return;
	}
	int totalBkCount = 0;
	while (fread(&bk[totalBkCount], sizeof(Booking), 1, readBk) != 0) {
		++totalBkCount;
	}
	fclose(readBk);

	for (int i = 0; i < totalFaciCount; i++) {
		for(int j=0;j<totalBkCount;j++){
		if (strcmp(bk[j].faciID, facility[i].ID) == 0 && sysDay == bk[j].bDate.day && sysMonth == bk[j].bDate.month && sysYear == bk[j].bDate.year && sysHour >= bk[j].sTime.hour && sysHour <= bk[j].eTime.hour && strcmp(facility[i].status, "Operating") == 0) {
				strcpy(facility[i].status, "In Use");
		}
		if (strcmp(bk[j].faciID, facility[i].ID) == 0 && sysDay == bk[j].bDate.day && sysMonth == bk[j].bDate.month && sysYear == bk[j].bDate.year && (sysHour >= bk[j].eTime.hour && sysMinute > bk[j].eTime.minute) && strcmp(facility[i].status, "In Use") == 0) {
				strcpy(facility[i].status, "Operating");
			}
		}
	}
	
	FILE* writeBkRec = fopen("booking.bin", "wb");
	appendRec = fopen("BookingFacilityUsage.bin", "ab");
	if (writeWalkInRec == NULL) {
		red();
		printf("\aFailed to open file: booking.bin\n");
		resetColor();
		system("pause");
		free(facility);
		free(bk);
		return;
	}
	if (appendRec == NULL) {
		red();
		printf("\aFailed to open file: BookingFacilityUsage.bin\n");
		resetColor();
		system("pause");
		free(facility);
		free(bk);
		return;
	}
	for (int i = 0; i < totalWalkInRec; i++) {
		if (sysYear > bk[i].tDate.year || sysMonth > bk[i].tDate.month || (sysMonth == bk[i].tDate.month && sysDay > bk[i].tDate.day)) {
			fwrite(&bk[i], sizeof(Booking), 1, appendRec);
		}
		else {
			fwrite(&bk[i], sizeof(Booking), 1, writeBkRec);
		}
	}
	fclose(writeBkRec);
	fclose(appendRec);
	free(bk);

	//facility status doesn't change
	FILE* writeFaci = fopen("facilities.txt", "w");
	if (writeFaci == NULL) {
		red();
		printf("\aFailed to open file: facilities.txt\n");
		resetColor();
		system("pause");
		free(facility);
		return;
	}
	for (int i = 0; i < totalFaciCount; i++) {
		fprintf(writeFaci,"%s|%s|%s|%s|%d|%s\n", facility[i].ID, facility[i].type, facility[i].description, facility[i].venue, facility[i].maxUser, facility[i].status);
	}
	fclose(writeFaci);
	free(facility);
}
//function change facility status from "in use" to "operating" if couldn't find record in booking / walkin bin
void verifyFacilityStatus() {
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysMonth = time.tm_mon + 1;
	int sysDay = time.tm_mday;
	int sysYear = time.tm_year + 1900;
	int sysHour = time.tm_hour;
	int sysMinute = time.tm_min;

	FILE* readFaci = fopen("facilities.txt", "r");
	if (readFaci == NULL) {
		red();
		printf("\aFailed to open file: facilities.txt\n");
		resetColor();
		system("pause");
		return;
	}
	Facility* facility = malloc(100 * sizeof * facility);
	int totalFaciCount = 0;
	while (!feof(readFaci)) {
		fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n", facility[totalFaciCount].ID, facility[totalFaciCount].type, facility[totalFaciCount].description, facility[totalFaciCount].venue, &facility[totalFaciCount].maxUser, facility[totalFaciCount].status);
		++totalFaciCount;
	}
	fclose(readFaci);

	FILE* readWkIn = fopen("walkin.bin", "rb");
	if (readWkIn == NULL) {
		red();
		printf("\aFailed to open file: walkin.bin\n");
		resetColor();
		system("pause");
		free(facility);
		return;
	}
	int totalWkInCount = 0;
	WalkIn* wkIn = malloc(100 * sizeof * wkIn);
	while (fread(&wkIn[totalWkInCount], sizeof(WalkIn), 1, readWkIn) != 0) {
		++totalWkInCount;
	}
	fclose(readWkIn);

	FILE* readBk = fopen("booking.bin", "rb");
	if (readBk == NULL) {
		red();
		printf("\aFailed to open file: booking.bin\n");
		resetColor();
		system("pause");
		free(facility);
		return;
	}
	int totalBkCount = 0;
	Booking* bk = malloc(100 * sizeof * bk);
	while (fread(&bk[totalBkCount], sizeof(Booking), 1, readBk) != 0) {
		++totalBkCount;
	}
	fclose(readBk);

	for (int i = 0; i < totalFaciCount; i++) {
		if (strcmp(facility[i].status, "In Use") == 0) {
			if(totalWkInCount > 0){
				for (int j = 0; j < totalWkInCount; j++) {
					if (strcmp(facility[i].ID, wkIn[j].faciID) == 0) {
						break;
					}
					if (j == (totalWkInCount - 1) && strcmp(facility[i].ID, wkIn[j].faciID) != 0) {		
						if (totalBkCount > 0) {
							for (int k = 0; k < totalBkCount; k++) {
								if (sysYear > bk[k].bDate.year || sysMonth > bk[k].bDate.month || (sysMonth == bk[k].bDate.month && sysDay >= bk[k].bDate.day)) {
									if (strcmp(facility[i].ID, bk[k].faciID) == 0) {
										break;
									}
									else if (k == (totalBkCount - 1) && strcmp(facility[i].ID, bk[k].faciID) != 0) {
										strcpy(facility[i].status, "Operating");
									}
								}
							}
						}
						else {
							strcpy(facility[i].status, "Operating");
						}
					}
				}
			}
			else {
				if (totalBkCount > 0) {
					for (int k = 0; k < totalBkCount; k++) {
						if (sysYear > bk[k].bDate.year || sysMonth > bk[k].bDate.month || (sysMonth == bk[k].bDate.month && sysDay >= bk[k].bDate.day)) {
							if (strcmp(facility[i].ID, bk[k].faciID) == 0) {
								break;
							}
							else if (k == (totalBkCount - 1) && strcmp(facility[i].ID, bk[k].faciID) != 0) {
								strcpy(facility[i].status, "Operating");
							}
						}
					}
				}
				else {
					strcpy(facility[i].status, "Operating");
				}
			}
		}
	}
	
	free(wkIn);
	free(bk);
	/*
	for (int i = 0; i < totalFaciCount; i++) {
		for (int j = 0; j < totalBkCount; j++) {
			if (strcmp(facility[i].status, "In Use") == 0) {
				if(sysYear > bk[j].bDate.year || sysMonth > bk[j].bDate.month || (sysMonth == bk[j].bDate.month && sysDay >= bk[j].bDate.day)) {
					if (strcmp(facility[i].ID, bk[j].faciID) == 0) {
						break;
					}
					else if (j == (totalBkCount - 1) && strcmp(facility[i].ID, bk[j].faciID) != 0) {
						strcpy(facility[i].status, "Operating");
					}
				}
			}
		}
	}*/
	

	FILE* writeFaci = fopen("facilities.txt","w");
	for (int i = 0; i < totalFaciCount; i++) {
		fprintf(writeFaci,"%s|%s|%s|%s|%d|%s\n",facility[i].ID,facility[i].type,facility[i].description,facility[i].venue,facility[i].maxUser,facility[i].status);
	}
	fclose(writeFaci);

	free(facility);
}


//helper function to validate between Date
void rangeBeforeTheDate(int dayRange, int bkMonth, int bkDay, int* outMonth, int* outDay){
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysYear = time.tm_year + 1900;
	*outDay = bkDay - dayRange;
	*outMonth = bkMonth;
	
	if (*outDay < 1) {
		--* outMonth;
		if (*outMonth == 1 || *outMonth == 3 || *outMonth == 5 || *outMonth == 7 || *outMonth == 8 || *outMonth == 10 || *outMonth == 12) {
			*outDay = 31 + *outDay;
		}
		if (*outMonth == 4 || *outMonth == 6 || *outMonth == 9 || *outMonth == 11) {
			*outDay = 30 + *outDay;
		}
		if (*outMonth == 2) {
			if (((sysYear % 4 == 0) && ((sysYear % 400 == 0) || (sysYear % 100 != 0)))) {
				*outDay = 29 + *outDay;
			}
			else {
				*outDay = 28 + *outDay;
			}
		}

	}
}
void rangeAfterTheDate(int dayRange, int bkMonth, int bkDay, int* outMonth, int* outDay) {
	time_t t = time(NULL);
	struct tm time = *localtime(&t);
	int sysYear = time.tm_year + 1900;
	*outDay = bkDay + dayRange;
	*outMonth = bkMonth;

	if (*outMonth == 1 || *outMonth == 3 || *outMonth == 5 || *outMonth == 7 || *outMonth == 8 || *outMonth == 10 || *outMonth == 12) {
		if (*outDay > 31) {
			*outDay -= 31;
			++* outMonth;
		}
	}
	if (*outMonth == 4 || *outMonth == 6 || *outMonth == 9 || *outMonth == 11) {
		if (*outDay > 30) {
			*outDay -= 30;
			++* outMonth;
		}

	}

	if (*outMonth == 2) {
		if (((sysYear % 4 == 0) && ((sysYear % 400 == 0) || (sysYear % 100 != 0)))) {
			if (*outDay > 29) {
				*outDay -= 29;
				++* outMonth;
			}
		}
		else {
			if (*outDay > 28) {
				*outDay -= 28;
				++* outMonth;
			}

		}

	}
	if (*outMonth == 12 && *outDay > 31) {
		*outDay = 31;
	}
}
