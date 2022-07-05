#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#pragma warning (disable:4996)

void addBooking();
void deleteBooking();
void displayBooking();
void modifyBooking(Facilities faci[], int faciCount);
void searchBooking();
void bookingReport();
void checkFaciID(Facilities faci[], int faciCount, char userInput[], int* invalidCount);
void checkMemID(struct member info[], int memCount, char userInput[], int* invalidCount);

void bookingSystem() {
	Facilities faci[300];
	int faciCount = 0;
	readFaci(faci, &faciCount);

	int choose;

	do {
		choose = -1;

		printf("-----------------------------------------------------------------\n");
		printf("			Booking Menu\n");
		printf("-----------------------------------------------------------------\n");
		printf("		1) Display All Booking Record\n");
		printf("		2) Add New Booking Record\n");
		printf("		3) Delete Booking Record\n");
		printf("		4) Search Booking Record\n");
		printf("		5) Modify Booking Record\n");
		printf("		6) Generate Booking Report\n");
		printf("		0) Exit Booking Menu\n");
		printf("-----------------------------------------------------------------\n");
		printf("\tEnter your chioce >>> ");
		rewind(stdin);
		scanf("%d", &choose);

		system("CLS");

		switch (choose) {
		case 1:
			//display booking func
			displayBooking();
			break;
		case 2:
			//add booking func
			addBooking(faci,faciCount);
			break;
		case 3:
			//delete booking func
			deleteBooking();
			break;
		case 4:
			//search booking func
			searchBooking();
			break;
		case 5:
			//modify booking func
			modifyBooking(faci,faciCount);
			break;
		case 6:
			//arrival rate report func
			bookingReport();
			break;
		case 0:
			return;
		default:
			continue;
		}

		system("pause");
		system("CLS");

	} while (1);

	system("pause");
}



void addBooking(Facilities faci[], int faciCount) {
	char cont, confirm;
	int amount;

	struct member info[1000];
	int memberCount = 0;
	FILE* fptr;

	fptr = fopen("staff.txt", "r");
	if (fptr == NULL) {
		printf("Error. Unable to open file.");
		return -1;
		//exit the function if the file failed to open
	}

	for (int i = 0; i < 1000; i++) {

		if (!feof(fptr)) {
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			++memberCount;
		}
		else
			break;
	}
	fclose(fptr);
	

	struct BookingSystem booking;
	FILE* fbooking = fopen("booking.bin", "ab+"); //ab+ write + read

	if (fbooking == NULL) {
		printf("Error! booking file unable to open!\n");
		return; //not need exit(-1) to exit the program
	}

	do {
		//input the new data
		printf("Enter the Booking Id: ");
		rewind(stdin);

		CheckInBooking(booking.bookingId);

		while (scanf("%s", &booking.bookingId) != 1 || CheckInBooking(booking.bookingId) >= 0) {
			rewind(stdin);
			printf("Invalid Input\n");
			printf("Please enter again the Booking Id: ");
		}

		printf("Enter the booking date(dd/mm/yyyy): ");
		rewind(stdin);
		while (scanf("%d/%d/%d", &booking.bookDay, &booking.bookMth, &booking.bookYear) != 3 || booking.bookDay > 31 || booking.bookDay <= 0 || booking.bookMth > 12 || booking.bookMth <= 0 || booking.bookYear == 0) {
			rewind(stdin);
			printf("Invalid Input\n");
			printf("Please enter again the booking date(dd/mm/yyyy): ");
		}

		printf("Enter the Booking Time(0000): ");
		rewind(stdin);
		while (scanf("%d", &booking.bookingTime) != 1 || booking.bookingTime > 2400 || booking.bookingTime < 0000) {
			rewind(stdin);
			printf("Invalid Input\n");
			printf("Please enter again the Booking Time: ");
		}

		printf("Enter the Book Date(dd/mm/yy): ");
		rewind(stdin);
		while (scanf("%d/%d/%d", &booking.day, &booking.mth, &booking.year) != 3 || booking.day > 31 || booking.day <= 0 || booking.mth > 12 || booking.mth <= 0 || booking.year == 0) {
			rewind(stdin);
			printf("Invalid Input\n");
			printf("Please enter again the book date(dd/mm/yyyy): ");
		}

		// Need to check the record is have or not
		int invalidCount = 0; // this is to check if the user input valid member ID and facility ID
		do{
		invalidCount = 0; //need to reset 0 , otherwise once user input 1 time wrong ID will keep accumulate
		//will eventually go into infinite loop
		printf("Enter the Member Id: ");
		rewind(stdin);
		while (scanf("%s", &booking.memberId) != 1) {
			rewind(stdin);
			printf("Invalid Input\n");
			printf("Please enter again the Member Id: ");
		}
		checkMemID(info, memberCount, booking.memberId,&invalidCount);
		}while(invalidCount > 0);

		// Need to check the record is have or not
		do{
		invalidCount = 0;
		printf("Enter the Facility Id: ");
		rewind(stdin);
		while (scanf("%s", &booking.facilityId) != 1) {
			rewind(stdin);
			printf("Invalid Input\n");
			printf("Please enter again the Facility Id: ");
		}
		checkFaciID(faci, faciCount, booking.facilityId, &invalidCount);
		} while (invalidCount > 0);


		fwrite(&booking, sizeof(booking), 1, fbooking);

		printf("\nBooking Id\tBook Date\tBooking Date\tBooking Time\tMember Id\tFacility Id\n");
		printf("-----------------------------------------------------------------------------------------------\n");
		printf("%s\t\t", booking.bookingId);
		printf("%02d/%02d/%d\t", booking.day, booking.mth, booking.year);
		printf("%02d/%02d/%d\t", booking.bookDay, booking.bookMth, booking.bookYear);
		printf("%04d\t\t", booking.bookingTime);
		printf("%s\t", booking.memberId);
		printf("%s\n\n", booking.facilityId);

		//confirm to add more record or not
		printf("\nAny more record? (Y/N): ");
		rewind(stdin);
		scanf("%c", &cont);

		printf("\n");

	} while (cont == 'Y');

	fclose(fbooking);
}

void displayBooking() {

	struct BookingSystem booking;

	FILE* fbooking = fopen("booking.bin", "rb"); //ab+ write + read

	if (fbooking == NULL) {
		printf("Error! booking file unable to open!\n");
		return; //not need exit(-1) to exit the program
	}

	printf("Booking Id\tBook Date\tBooking Date\tBooking Time\tMember Id\tFacility Id\n");
	printf("-----------------------------------------------------------------------------------------------\n");

	while (fread(&booking, sizeof(booking), 1, fbooking) != 0) {

		printf("%s\t\t", booking.bookingId);
		printf("%02d/%02d/%d\t", booking.day, booking.mth, booking.year);
		printf("%02d/%02d/%d\t", booking.bookDay, booking.bookMth, booking.bookYear);
		printf("%04d\t\t", booking.bookingTime);
		printf("%s\t", booking.memberId);
		printf("%s\n\n", booking.facilityId);
	}

	fclose(fbooking);
}

void deleteBooking() {

	char search[21];
	int result;
	int amount;

	struct BookingSystem booking;

	FILE* fbooking = fopen("booking.bin", "ab+"); //ab+ write + read
	FILE* dbooking = fopen("delete booking.bin", "wb+"); //delete file pointer

	if (fbooking == NULL || dbooking == NULL) {

		if (fbooking != NULL) {
			fclose(fbooking);
		}
		if (dbooking != NULL) {
			fclose(dbooking);
		}

		printf("Error! booking file unable to open!\n");
		return; //not need exit(-1) to exit the program
	}

	printf("\tEnter Booking ID of booking record want to delete:");
	rewind(stdin);
	scanf("%[^\n]", &search);

	amount = CheckInBooking(search);

	if (amount >= 0) {

		while (fread(&booking, sizeof(booking), 1, fbooking) != 0) {

			//use strcmp to find the same booking ID
			//if same , write into the other file

			if (strcmp(search, booking.bookingId) != 0) {
				fwrite(&booking, sizeof(booking), 1, dbooking);
			}

		}
	}
	else {
		fclose(fbooking);
		fclose(dbooking);
		printf("\n\tInvalid Booking Id\n");
		return;
	}

	fclose(fbooking);
	fclose(dbooking);

	remove("booking.bin");

	rename("delete booking.bin", "booking.bin");


}

void searchBooking() {

	char search[10];
	int amount;
	int result = 0;
	char check;

	struct BookingSystem booking;

	FILE* fbooking = fopen("booking.bin", "rb"); //ab+ write + read

	if (fbooking == NULL) {
		printf("Error! booking file unable to open!");
		return; //not need exit(-1) to exit the program
	}

	//get input to search

	printf("\tEnter Booking ID want to search:");
	rewind(stdin);
	scanf("%[^\n]", &search);

	amount = CheckInBooking(search);

	if (amount >= 0) {

		printf("\nBooking Id\tBook Date\tBooking Date\tBooking Time\tMember Id\tFacility Id\n");
		printf("-----------------------------------------------------------------------------------------------\n");

		while (fread(&booking, sizeof(booking), 1, fbooking) != 0) {

			//use strcmp to find the same booking ID
			//if same , display the record

			if (strcmp(search, booking.bookingId) == 0) {

				printf("%s\t\t", booking.bookingId);
				printf("%02d/%02d/%d\t", booking.day, booking.mth, booking.year);
				printf("%02d/%02d/%d\t", booking.bookDay, booking.bookMth, booking.bookYear);
				printf("%04d\t\t", booking.bookingTime);
				printf("%s\t\t", booking.memberId);
				printf("%s\n\n", booking.facilityId);
			}
		}
	}
	else {
		fclose(fbooking);
		printf("\n\tInvalid Booking Id\n");
		return;
	}

	fclose(fbooking);
}

void modifyBooking(Facilities faci[], int faciCount) {
	struct member info[1000];
	int memberCount = 0;
	FILE* fptr;

	fptr = fopen("staff.txt", "r");
	if (fptr == NULL) {
		printf("Error. Unable to open file.");
		return -1;
		//exit the function if the file failed to open
	}

	for (int i = 0; i < 1000; i++) {

		if (!feof(fptr)) {
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear, info[i].status);
			++memberCount;
		}
		else
			break;
	}
	fclose(fptr);
	
	char search[21];
	int result;
	int choose;
	char confirm;
	int invalidCount = 0;

	struct BookingSystem tempBooking, editBooking;

	FILE* fbooking = fopen("booking.bin", "ab+"); //ab+ write + read
	FILE* nbooking = fopen("new booking.bin", "wb"); //modify file pointer

	if (fbooking == NULL || nbooking == NULL) {

		if (fbooking != NULL) {
			fclose(fbooking);
		}
		if (nbooking != NULL) {
			fclose(nbooking);
		}

		printf("Error! booking file unable to open!");
		return; //not need exit(-1) to exit the program
	}

	//get the input to search

	printf("\tEnter Booking ID of booking record want to modify:");
	rewind(stdin);
	scanf("%[^\n]", &search);

	while (fread(&tempBooking, sizeof(struct BookingSystem), 1, fbooking) != 0) {

		editBooking = tempBooking;

		//use strcmp to find the same booking ID

		if (strcmp(search, tempBooking.bookingId) == 0) {

			//menu to let user to choose the data want to modify
			printf("-----------------------------------------------------------------\n");
			printf("		Choose what you want to modify\n");
			printf("-----------------------------------------------------------------\n");
			printf("			1) Book Date\n");
			printf("			2) Booking Date\n");
			printf("			3) Booking Time\n");
			printf("			4) Member Id\n");
			printf("			5) Facility Id\n");
			printf("			0) Exit to main\n");
			printf("-----------------------------------------------------------------\n");
			printf("\tEnter your chioce >>> ");
			rewind(stdin);

			while (scanf("%d", &choose) != 1 || choose > 6 || choose < 0) {
				//validation the choose of user
				printf("Invalid choice! Please Enter again.\n");
				printf("Enter your chioce >>> ");
			}

			system("CLS");

			switch (choose) {
			case 1:
				printf("\tEnter the new booking date(dd/mm/yyyy): ");
				rewind(stdin);
				scanf("%d/%d/%d", &editBooking.bookDay, &editBooking.bookMth, &editBooking.bookYear);
				break;
			case 2:
				printf("\tEnter the new Book Date(dd/mm/yyyy): ");
				rewind(stdin);
				scanf("%d/%d/%d", &editBooking.day, &editBooking.mth, &editBooking.year);
				break;
			case 3:
				printf("\tEnter the new Booking Time: ");
				rewind(stdin);
				scanf("%d", &editBooking.bookingTime);
				break;
			case 4:
				do{
				invalidCount = 0;
				printf("\tEnter the new Member Id: ");
				rewind(stdin);
				scanf("%s", &editBooking.memberId);
				checkMemID(info, memberCount, editBooking.memberId, &invalidCount);
				} while (invalidCount > 0);
				break;
			case 5:
				do{
				invalidCount = 0;
				printf("\tEnter the new Facility Id: ");
				rewind(stdin);
				scanf("%s", &editBooking.facilityId);
				checkFaciID(faci, faciCount, editBooking.facilityId, &invalidCount);
				} while (invalidCount > 0);
				break;
			case 0:
				fclose(fbooking);
				fclose(nbooking);
				return;
				break;
			}

			printf("Booking Id\tBook Date\tBooking Date\tBooking Time\tMember Id\tFacility Id\n");
			printf("-----------------------------------------------------------------------------------------------\n");

			printf("%s\t\t", tempBooking.bookingId);
			printf("%02d/%02d/%d\t", tempBooking.day, tempBooking.mth, tempBooking.year);
			printf("%02d/%02d/%d\t", tempBooking.bookDay, tempBooking.bookMth, tempBooking.bookYear);
			printf("%04d\t\t", tempBooking.bookingTime);
			printf("%s\t", tempBooking.memberId);
			printf("%s\n", tempBooking.facilityId);

			printf("\t\t\t\t\t|\n\t\t\t\t\tV\n");

			printf("%s\t\t", editBooking.bookingId);
			printf("%02d/%02d/%d\t", editBooking.day, editBooking.mth, editBooking.year);
			printf("%02d/%02d/%d\t", editBooking.bookDay, editBooking.bookMth, editBooking.bookYear);
			printf("%04d\t\t", editBooking.bookingTime);
			printf("%s\t", editBooking.memberId);
			printf("%s\n\n", editBooking.facilityId);

			//confirm the record of want modify
			printf("\tConfirm to modify the following record?(Y for yes): ");
			rewind(stdin);
			scanf("%c", &confirm);

			if (confirm == 'Y') {
				printf("\tModify successfull\n");
				fwrite(&editBooking, sizeof(struct BookingSystem), 1, nbooking);
			}
			else {
				printf("\tModify cancelled\n");
				fclose(fbooking);
				fclose(nbooking);
				return;
			}

		}
		else {
			fwrite(&tempBooking, sizeof(struct BookingSystem), 1, nbooking);
		}
	}

	fclose(fbooking);
	fclose(nbooking);

	remove("booking.bin");

	rename("new booking.bin", "booking.bin");


}

void bookingReport() {

	struct BookingSystem booking;

	int search, numMth;
	int count = 0;
	char month[21];

	FILE* fbooking = fopen("booking.bin", "rb");

	if (fbooking == NULL) {
		printf("Error! booking file unable to open!");
		return; //not need exit(-1) to exit the program
	}

	printf("Enter the number month for report: ");
	scanf("%d", &search);

	switch (search) {
	case 1:
		strcpy(month, "January");
		numMth = 1;
		break;
	case 2:
		strcpy(month, "February");
		numMth = 2;
		break;
	case 3:
		strcpy(month, "March");
		numMth = 3;
		break;
	case 4:
		strcpy(month, "April");
		numMth = 4;
		break;
	case 5:
		strcpy(month, "May");
		numMth = 5;
		break;
	case 6:
		strcpy(month, "June");
		numMth = 6;
		break;
	case 7:
		strcpy(month, "July");
		numMth = 7;
		break;
	case 8:
		strcpy(month, "August");
		numMth = 8;
		break;
	case 9:
		strcpy(month, "September");
		numMth = 9;
		break;
	case 10:
		strcpy(month, "October");
		numMth = 10;
		break;
	case 11:
		strcpy(month, "November");
		numMth = 11;
		break;
	case 12:
		strcpy(month, "December");
		numMth = 12;
		break;
	case 0:
		fclose(fbooking);
		return;
	}

	printf("\n------------------------------------------------------------------------------------------------------\n");
	printf("\tBooking Quantities for Year 2022\n");
	printf("------------------------------------------------------------------------------------------------------\n");

	printf("\nBooking ID\tBooking Date\tBooking Time\tMember Id\tFacility Id\n");
	printf("------------------------------------------------------------------------------------------------------\n");

	while (fread(&booking, sizeof(booking), 1, fbooking) != 0) {

		if (booking.bookMth == numMth) {

			printf("%s\t\t", booking.bookingId);
			printf("%02d/%02d/%d\t", booking.bookDay, booking.bookMth, booking.bookYear);
			printf("%04d\t\t", booking.bookingTime);
			printf("%s\t", booking.memberId);
			printf("%s\n\n", booking.facilityId);

			count++;
		}
	}

	printf("Total member booking in %s = %d\n", month, count);

	fclose(fbooking);
}

void checkFaciID(Facilities faci[], int faciCount, char userInput[], int* invalidCount) {
	for (int i = 0; i < faciCount; i++) {
		if (strcmp(userInput, faci[i].ID) == 0) {
			if (strcmp(faci[i].status, "Operating") == 0) {
				break;
			}
			else {
				printf("The facility is under maintainace\n");
				++* invalidCount;
				break;
			}
		}

		else if (i == faciCount - 1 && strcmp(userInput, faci[i].ID) != 0) {
			printf("Invalid Facility ID\n");
			++* invalidCount;
		}
	}

}

void checkMemID(struct member info[], int memCount, char userInput[], int* invalidCount) {
	info[1000];
	int memberCount = 0;
	FILE* fptr;
	fptr = fopen("member.txt", "r");

	if (fptr == NULL) {
		printf("Error!! Member.txt file can't open!!");
		return;
	}

	for (int i = 0; i < 1000; i++) {
		if (!feof(fptr)) {
			++memberCount;
			fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|[^\n]\n", info[i].name, &info[i].gender, info[i].phoneNum, info[i].id, info[i].email, &info[i].time.regDay, &info[i].time.regMonth, &info[i].time.regYear);
		}

		else
			break;
	}

	fclose(fptr);

	for (int i = 0; i < memberCount; i++) {
		if (strcmp(userInput, info[i].id) == 0) {
			break;
		}

		else if (i == memberCount - 1 && strcmp(userInput, info[i].id) != 0) {
			printf("Invalid Member ID\n");
			++* invalidCount;
		}
	}

}