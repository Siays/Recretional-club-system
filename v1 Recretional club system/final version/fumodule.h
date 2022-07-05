#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning (disable : 4996)

void FacUsageAdd(int *amount);
void FacUsageDisplay(int recNum);
void FacUsageModify();
void FacUsageDelete(int *recNum);
void FacUsageSearch();
void FacUsageReports();

int CheckInMember(char id[20]);
int CheckInStaff(char id[7]);
int CheckInFacs(char id[4]);
int CheckInBooking(char id[10]);
int CheckInFacUsage(char id[7]);

int FURecCount();
int CompareFacTime(void* x, void* y); //reverse order
int CompareMemberTime(void* x, void* y); //reverse order
void FUmodule();

void FUmodule() {
	int selection, recCount;
	
	//system("color 70");
	system("cls");
	recCount = FURecCount();

	do {
		selection = -1;
		system("cls");
		printf("\n\n\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                    FACILITY USAGE MODULE\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("\t\t\t\t                   0. Back to Main Menu\n");
		printf("\t\t\t\t                   1. Add Record\n");
		printf("\t\t\t\t                   2. Display Records\n");
		printf("\t\t\t\t                   3. Modify Record\n");
		printf("\t\t\t\t                   4. Delete Record\n");
		printf("\t\t\t\t                   5. Search Records\n");
		printf("\t\t\t\t                   6. Show Reports\n");
		printf("\t\t\t\t---------------------------------------------------------\n");
		printf("Your selection --> ");
		do {
			rewind(stdin);
			scanf("%d", &selection);

			rewind(stdin);
			switch (selection) {
			case 1:
				FacUsageAdd(&recCount);
				break;
			case 2:
				FacUsageDisplay(recCount);
				break;
			case 3:
				FacUsageModify();
				break;
			case 4:
				FacUsageDelete(&recCount);
				break;
			case 5:
				FacUsageSearch();
				break;
			case 6:
				FacUsageReports();
				break;
			case 0:
				return;
			default:
				printf("Selection is not valid. Try again.\n>\n");
			}
		} while (selection > 6 || selection < 0);

	} while (1);
}

int CompareFacTime(void* t1, void* t2) { // Comparison Between Facility Time Usages
	struct FacDuration* time1 = (struct FacDuration*)t1;
	struct FacDuration* time2 = (struct FacDuration*)t2;
	
	long tt1 = time1->total_time.hour * 3600 + time1->total_time.min * 60 + time1->total_time.sec;
	long tt2 = time2->total_time.hour * 3600 + time2->total_time.min * 60 + time2->total_time.sec;
	

	return ((tt1 < tt2) - (tt1 > tt2));

}

int CompareMemberTime(void* t1, void* t2) { //Comparison between Two Members Time Usage
	struct MemberDuration* time1 = (struct MemberDuration*)t1;
	struct MemberDuration* time2 = (struct MemberDuration*)t2;

	long tt1 = time1->total_time.hour * 3600 + time1->total_time.min * 60 + time1->total_time.sec;
	long tt2 = time2->total_time.hour * 3600 + time2->total_time.min * 60 + time2->total_time.sec;


	return ((tt1 < tt2) - (tt1 > tt2));

}

int FURecCount() { //Count Amount of Records

	FacUsageRec tempRec;
	FILE* fu_ptr;
	int amount = 0;

	fu_ptr = fopen("fac_usage_rec.bin","rb");

	while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr)) {
		amount++;
	}

	fclose(fu_ptr);
	fu_ptr = NULL;
	return amount;
};

int CheckInMember(char id[20]) { //returns index of id, -1 otherwise
	struct member info; //staff structures
	FILE* fptr;
	int amount = 0;

	fptr = fopen("member.txt", "r");

	if (!fptr) { return -2; }

	while (fscanf(fptr, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%s\n", info.name, &info.gender, info.phoneNum, info.id, info.email, &info.time.regDay, &info.time.regMonth, &info.time.regYear,info.status) != EOF) { //Should be fscanf of staff.txt
		if (strcmp(id, info.id) == 0) {
			amount++;
			fclose(fptr);
			return amount;
		}

	}

	fclose(fptr);
	return -1;
};

int CheckInStaff(char id[7]) { //returns index of id, -1 otherwise
	Staff staff; //staff structures
	FILE* fptr;
	int amount = 0;

	fptr = fopen("staff.txt", "r");

	if (!fptr) { return -2; }

	while (!feof(fptr)) { //Should be fscanf of staff.txt
		fscanf(fptr, "%[^|]|%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%d|%s\n", staff.name, staff.contact, &staff.gender, staff.state, staff.ID, staff.password, &staff.pwRecovery, staff.position, staff.email, &staff.age, staff.status);
		if (strcmp(staff.ID, id) == 0) {
			amount++;
			fclose(fptr);
			return amount;
		}
	}

	fclose(fptr);
	return -1;
};

int CheckInFacs(char id[4]) { //returns index of id, -1 otherwise
	Facilities faci[300];//staff structures
	int amount = 0;
	//read faci record, to enable to check facility ID
	FILE* readFaci;
	int faciCount = 0;//track how many facilities currently have. 

	readFaci= fopen("facilities.txt", "r");
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
			faciCount++;
		}
		else
			//break the loop once all finish reading records. 
			break;
	}

	for (int i = 0; i < faciCount; i++) {
		if (strcmp(faci[i].ID, id) == 0) {
			amount++;
			return amount;
		}
	}

	return -1;
};

int CheckInFacUsage(char id[7]) { //returns index of id, -1 or -2 otherwise

	FacUsageRec tempRec;
	FILE* fptr;
	int amount = 0;

	fptr = fopen("fac_usage_rec.bin", "rb");

	if (!fptr) { return -2; }

	while (fread(&tempRec, sizeof(FacUsageRec), 1, fptr) > 0) {
		if (strcmp(id, tempRec.facUsageId) == 0) {
			fclose(fptr);
			fptr = NULL;
			return amount;
		}

		amount++;
	}

	fclose(fptr);
	fptr = NULL;
	return -1;
};

int CheckInBooking(char id[10]) { //returns index of id, -1 or -2 otherwise

	struct BookingSystem tempRec; 
	FILE* fptr;
	int amount = 0;

	fptr = fopen("booking.bin", "rb");

	if (!fptr) { return -2; }

	while (0 > 0) { //Should be fread of booking.bin
		if (strcmp(id,tempRec.bookingId) == 0) {
			fclose(fptr);
			fptr = NULL;
			return amount;
		}

		amount++;
	}

	fclose(fptr);
	fptr = NULL;
	return -1;
};

void FacUsageAdd(int* recNum) {
	int amount = 0, facCheck, memCheck, bookCheck,staffCheck;
	FacUsageRec tempRec, defaultRec = { "-",-1,-1,-1,-1,-1,-1,"-",-1,-1,-1,"-","-","-" };
	char prompt, confirmPrompt, prebookPrompt;
	FILE* fu_ptr;

	fu_ptr = fopen("fac_usage_rec.bin", "ab");

	
	do {
		system("cls");
		tempRec = defaultRec;

		printf("Insert FacUsage ID (0 to cancel) >");
		rewind(stdin);
		gets(tempRec.facUsageId);

		while (CheckInFacUsage(tempRec.facUsageId) >= 0) {
			printf("\nThis ID is already in use.\n");
			printf("Please reinsert FacUsage ID >");
			rewind(stdin);
			gets(tempRec.facUsageId);
		};

		if (!strcmp(tempRec.facUsageId, "0")) {
			printf("\n\nAdd Cancelled.\n");
			fclose(fu_ptr);
			printf("Press enter to exit.\n");
			rewind(stdin);
			(void)getchar();
			return;
		
		}

		printf("\nInsert Starting Date (DD/MM/YYYY) >");
		rewind(stdin);
		scanf("%d/%d/%d", &tempRec.starting_date.day, &tempRec.starting_date.month, &tempRec.starting_date.year);

		while (tempRec.starting_date.day < 1 || tempRec.starting_date.month < 1 || tempRec.starting_date.year < 1950 || tempRec.starting_date.day > 31 || tempRec.starting_date.month > 12) {
			printf("Input format or value error.\n");
			printf("Please reinsert Starting Date (DD/MM/YYYY) >");
			rewind(stdin);
			scanf("%d/%d/%d", &tempRec.starting_date.day, &tempRec.starting_date.month, &tempRec.starting_date.year);

		};

		printf("\nInsert Starting Time (hr:min:ss) >");
		scanf("%d:%d:%d", &tempRec.starting_time.hour, &tempRec.starting_time.min, &tempRec.starting_time.sec);

		while (tempRec.starting_time.hour < 0 || tempRec.starting_time.min < 0 || tempRec.starting_time.sec < 0 || tempRec.starting_time.min > 60 || tempRec.starting_time.sec > 60 || tempRec.starting_time.hour > 24) {
			printf("Input format or value error.\n");
			printf("Please reinsert starting time (hr:min:ss) >");
			scanf("%d:%d:%d", &tempRec.starting_time.hour, &tempRec.starting_time.min, &tempRec.starting_time.sec);
		};

		printf("\nInsert Duration (hr:mm:ss) >");
		scanf("%d:%d:%d", &tempRec.duration.hour, &tempRec.duration.min, &tempRec.duration.sec);

		while (tempRec.duration.hour < 0 || tempRec.duration.min < 0 || tempRec.duration.sec < 0 || tempRec.duration.min > 60 || tempRec.duration.sec > 60) {
			printf("Input format or value error.\n");
			printf("Please reinsert duration (hr:min:ss) >");
			scanf("%d:%d:%d", &tempRec.duration.hour, &tempRec.duration.min, &tempRec.duration.sec);
		};
		
		printf("\nInsert Your (Staff) ID >");
		rewind(stdin);
		gets(tempRec.processing_staff);
		staffCheck = (int)(CheckInStaff(tempRec.processing_staff) >= 0);

		printf("\nInsert Member ID >");
		rewind(stdin);
		gets(tempRec.member_id);
		memCheck = (int)(CheckInMember(tempRec.member_id) >= 0);

		printf("\nInsert Facility ID >"); //note that staff , member ,facility and booking don't need validation.
		rewind(stdin);
		gets(tempRec.facility_id);
		facCheck = (int)(CheckInFacs(tempRec.facility_id) >= 0);

		printf("Is the usage prebooked? (Y for yes)\n");
		printf("=");
		rewind(stdin);
		prebookPrompt = toupper(getchar());
		if (prebookPrompt == 'Y') {
			printf("Booking ID > ");
			rewind(stdin);
			gets(tempRec.booking_id);

		}

		bookCheck = (int)(CheckInBooking(tempRec.booking_id) >= 0 || strcmp(tempRec.booking_id, "-") == 0);


		system("cls");
		printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
		printf("============================================================================================================================================\n");
		printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);


		if (!bookCheck || !memCheck || !facCheck ||!staffCheck) {
			printf("Please note that :\n");
			if (!staffCheck) {
				printf("\t Staff ID does not exist.\n");
			}
			if (!memCheck) {
				printf("\t Member ID does not exist.\n");
			}
			if (!facCheck) {
				printf("\t Facility ID does not exist.\n");
			}
			if (!bookCheck) {
				printf("\t Booking ID does not exist.\n");
			}
			

		}

		printf("\n\nWould you like to proceed?(Y for yes) =");
		rewind(stdin);
		confirmPrompt = toupper(getchar());
		if (confirmPrompt != 'Y') {
			printf("Add New FacUsage Record cancelled. %d record(s) added.\n", amount);
			fclose(fu_ptr);
			fu_ptr = NULL;
			printf("Press enter to exit.\n");
			rewind(stdin);
			(void)getchar();
			*recNum += amount;
			return;
		}
		fwrite(&tempRec,sizeof(FacUsageRec),1,fu_ptr);
		amount++;
		printf("Any other records to add? (Y for more) = ");
		rewind(stdin);
		prompt = toupper(getchar());


	} while (prompt == 'Y');

	fclose(fu_ptr);
	fu_ptr = NULL;
	printf("\n\n%d record(s) added.\n", amount);
	*recNum += amount;
	printf("Press enter to exit.\n");
	rewind(stdin);
	(void)getchar();

}

void FacUsageDisplay(int recNum) {

	FILE* fu_ptr;
	FacUsageRec tempRec;
	
	system("cls");
	fu_ptr = fopen("fac_usage_rec.bin", "rb");

	printf("\t\t\t\t\t\tFacility Usage Records\n\n");
	printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
	printf("============================================================================================================================================\n");
	
	while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr)) {
		printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
	}

	printf("\n\t\t\t<%d record(s) shown>\n",recNum);
	printf("\nPress enter to exit.\n");
	rewind(stdin);
	(void)getchar();
	fclose(fu_ptr);
}

void FacUsageModify() {
	int choice, modifyAmount = 0,columnChoice;
	char modifyChoice[7], confirmModify;

	FacUsageRec tempRec,editedRec;
	FILE* fu_ptr;
	FILE* fu_out_ptr;

	fu_ptr = fopen("fac_usage_rec.bin","rb");
	fu_out_ptr = fopen("fu_temp.bin","wb");

	if (!fu_ptr || !fu_out_ptr) {
		if (fu_ptr) {
			fclose(fu_ptr);
			fu_ptr = NULL;

		}else if (fu_out_ptr) {
			
			fclose(fu_out_ptr);
			fu_out_ptr = NULL;
		}
		remove("fu_temp.bin");
		printf("Faculty Usage Records cannot be modified currently.\n");
		printf("Please notify person-in-charge for assistance.\n");
		return;
	}

	system("cls");

	printf("Modify by ID selected.\n");
	

	printf("Insert ID Here (type 0 to cancel modify) > ");
	rewind(stdin);
	gets(modifyChoice);

		

	while (CheckInFacUsage(modifyChoice) < 0) {
		if (strcmp(toupper(modifyChoice), "0") == 0) {
			fclose(fu_ptr);
			fclose(fu_out_ptr);
			fu_ptr = NULL;
			fu_out_ptr = NULL;
			remove("fu_temp.bin");
			printf("Modify Cancelled.\n");
			printf("Press enter to continue.\n");

			rewind(stdin);
			(void)getchar();
			return;
		}

		printf("\nNo such ID Found.\n");
		printf("Please reenter ID.\n >");
		rewind(stdin);
		gets(modifyChoice);
		rewind(stdin);
	}

	while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
		editedRec = tempRec;
		if (!strcmp(tempRec.facUsageId, modifyChoice)) {
			columnChoice = -1;
			system("cls");

			printf("For %s, what would you like to modify?\n\n",modifyChoice);
			printf("[1] Starting Date (%02d/%02d/%4d)\n",tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year);
			printf("[2] Starting Time (%02d:%02d:%02d)\n",tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec);
			printf("[3] Processing Staff (%s)\n",tempRec.processing_staff);
			printf("[4] Duration (%02d:%02d:%02d)\n",tempRec.duration.hour,tempRec.duration.min,tempRec.duration.sec);
			printf("[5] Member ID (%s)\n",tempRec.member_id);
			printf("[6] Facility ID (%s)\n",tempRec.facility_id);
			printf("[7] Booking ID (%s)\n\n",tempRec.booking_id);
				
			do{
				printf(">");
				scanf("%d",&columnChoice);

				if (columnChoice < 1 || columnChoice > 7) {
					printf("Selection not Valid. Please enter again.\n");
				}
				
			} while (columnChoice < 1 || columnChoice > 7);
				
			system("cls");
			switch (columnChoice) {
				case 1:
					printf("Please enter new starting date for %s. (dd/mm/yyyy)\n", modifyChoice);
					printf("->");
					while (scanf("%d/%d/%d", &editedRec.starting_date.day, &editedRec.starting_date.month, &editedRec.starting_date.year) != 3 || editedRec.starting_date.day < 1 || editedRec.starting_date.month < 1 || editedRec.starting_date.year < 1950 || editedRec.starting_date.day > 31 || editedRec.starting_date.month > 12) {
						printf("Invalid input or format. Please try again.\n>");
					};
					break;
				case 2:
					printf("Please enter new starting time for %s. (hr:min:sec)\n", modifyChoice);
					printf("->");
					while (scanf("%d:%d:%d", &editedRec.starting_time.hour, &editedRec.starting_time.min, &editedRec.starting_time.sec) != 3 || editedRec.starting_time.hour < 0 || editedRec.starting_time.min < 0 || editedRec.starting_time.sec < 0 || editedRec.starting_time.min > 60 || editedRec.starting_time.sec > 60 || editedRec.starting_time.hour > 24) {
						printf("Invalid input or format. Please try again.\n>");
					};
					break;
				
				case 3:
					printf("Please enter new processing staff ID for %s.\n", modifyChoice);
					printf("->");
					rewind(stdin);
					gets(editedRec.processing_staff);
					break;

				case 4:
					printf("Please enter new duration of use for %s. (hr:min:sec)\n", modifyChoice);
					printf("->");
					while (scanf("%d:%d:%d", &editedRec.duration.hour, &editedRec.duration.min, &editedRec.duration.sec) != 3 || editedRec.duration.hour < 0 || editedRec.duration.min < 0 || editedRec.duration.sec < 0 || editedRec.duration.min > 60 || editedRec.duration.sec > 60) {
						printf("Invalid input or format. Try again.\n>");
					};
					break;
				case 5:
					printf("Please enter new member ID for %s.\n", modifyChoice);
					printf("->");
					rewind(stdin);
					gets(editedRec.member_id);
					break;
				case 6:
					printf("Please enter new facility ID for %s.\n", modifyChoice);
					printf("->");
					rewind(stdin);
					gets(editedRec.facility_id);
					break;
				case 7:
					printf("Please enter new booking ID for %s.\n", modifyChoice);
					printf("->");
					rewind(stdin);
					gets(editedRec.booking_id);
					break;
				
			}
			system("cls");
			printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
			printf("============================================================================================================================================\n");
			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
			printf("\t\t\t\t\t\t|\n");
			printf("\t\t\t\t\t\tv\n");
			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", editedRec.facUsageId, editedRec.starting_date.day, editedRec.starting_date.month, editedRec.starting_date.year, editedRec.starting_time.hour, editedRec.starting_time.min, editedRec.starting_time.sec, editedRec.duration.hour, editedRec.duration.min, editedRec.duration.sec, editedRec.processing_staff, editedRec.member_id, editedRec.facility_id, editedRec.booking_id);

			printf("\nModify the following record? (Y for yes)\n");
			printf(">");
				
			rewind(stdin);
			confirmModify = toupper(getchar());

			if (confirmModify != 'Y') {
				printf("Modify for %s cancelled. Press enter to continue.\n",modifyChoice);
				rewind(stdin);
				(void)getchar();
				fwrite(&tempRec, sizeof(FacUsageRec), 1, fu_out_ptr);
				continue;
			}

			printf("Modify confirmed.");
			modifyAmount++;
		}

		fwrite(&editedRec,sizeof(FacUsageRec),1,fu_out_ptr);
	};



	fclose(fu_ptr);
	fu_ptr = NULL;
	fclose(fu_out_ptr);
	fu_out_ptr = NULL;
	printf("\n\n%d record(s) modified.\n", modifyAmount);

	remove("fac_usage_rec.bin"); 

	rename("fu_temp.bin", "fac_usage_rec.bin");

	printf("Press enter to continue.\n");
	
	rewind(stdin);
	(void)getchar();

		
}

void FacUsageDelete(int* recNum) {

	FILE* fu_ptr;
	FILE* fu_out_ptr;
	FacUsageRec tempRec, deleteTarget;
	int prompt, delCount = 0;
	char finalPrompt;

	system("cls");
	fu_ptr = fopen("fac_usage_rec.bin", "rb");
	fu_out_ptr = fopen("fu_temp.bin", "wb");

	if (!fu_ptr || !fu_out_ptr) {
		if (fu_ptr) {
			fclose(fu_ptr);
			fu_ptr = NULL;
		}
		else if (fu_out_ptr) {

			fclose(fu_out_ptr);
			fu_ptr = NULL;
		}
		remove("fu_temp.bin");
		printf("Faculty Usage Records cannot be deleted currently.\n");
		printf("Please notify person-in-charge for assistance.\n");
		return;
	}
	printf("Delete Records selected.\n");
	printf("Delete:\n");
	printf("\t[1] Record with a specific ID\n");
	printf("\t[2] Records with certain values\n");
	printf("\t[0] Return to main menu\n");

	do {
		printf("> ");
		scanf("%d", &prompt);

		if (prompt > 2 || prompt < 0) {
			printf("Invalid input. Please try again.\n");
		}

	} while (prompt > 2 || prompt < 0);

	if (!prompt) {
		fclose(fu_ptr);
		fclose(fu_out_ptr);
		system("cls");
		printf("Delete Cancelled.\n");
		printf("Press enter to return...\n");
		rewind(stdin);
		(void)getchar();
		return;
	
	}

	if (prompt == 1) {

		printf("Please insert the FacUsage ID that you would like to delete.\n");
		printf(">");

		rewind(stdin);
		gets(deleteTarget.facUsageId);


		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			if (strcmp(tempRec.facUsageId, deleteTarget.facUsageId) != 0) {
				fwrite(&tempRec, sizeof(FacUsageRec), 1, fu_out_ptr);
				continue;
			}
			delCount++;
			if (delCount == 1) {
				printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
				printf("============================================================================================================================================\n");
			};
			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
		}

	}
	else if (prompt == 2) {
		int columnChoice;

		printf("\nSelect the criteria you would like to delete by.\n");
		printf("\t[1] Entire Starting Date\n");
		printf("\t[2] Starting Date Year & Month\n");
		printf("\t[3] Staff ID\n");
		printf("\t[4] Facility ID\n");
		printf("\t[5] Booking ID\n");

		do {
			printf("> ");
			rewind(stdin);
			scanf("%d", &columnChoice);
			if (columnChoice < 1 || columnChoice > 5) {
				printf("\nInvalid selection. Please try again.\n");
			}


		} while (columnChoice < 1 || columnChoice > 5);

		switch (columnChoice) {

		case 1:
			system("cls");
			printf("Enter the starting date that you would like to delete by. (DD/MM/YYYY)\n");

			scanf("%d/%d/%d", &deleteTarget.starting_date.day, &deleteTarget.starting_date.month, &deleteTarget.starting_date.year);
			while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
				if (deleteTarget.starting_date.day != tempRec.starting_date.day && deleteTarget.starting_date.month != tempRec.starting_date.month && deleteTarget.starting_date.year != tempRec.starting_date.year) {
					fwrite(&tempRec, sizeof(FacUsageRec), 1, fu_out_ptr);
					continue;
				}
				delCount++;
				if (delCount == 1) {
					printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
					printf("============================================================================================================================================\n");
				};
				printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
			}
			break;

		case 2:
			system("cls");
			printf("Enter the starting year and month that you would like to delete by. (DD/MM/YYYY)\n");
			printf(">");

			while (scanf("%d/%d/%d", &deleteTarget.starting_date.day, &deleteTarget.starting_date.month, &deleteTarget.starting_date.year) < 3) {
				printf("Input error. Please try again\n");
				printf(">");
			};
			while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
				if (deleteTarget.starting_date.year != tempRec.starting_date.year && deleteTarget.starting_date.month != tempRec.starting_date.month) {
					fwrite(&tempRec, sizeof(FacUsageRec), 1, fu_out_ptr);
					continue;
				}
				delCount++;
				if (delCount == 1) {
					printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
					printf("============================================================================================================================================\n");
				};
				printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
			}
			break;

		case 3:
			system("cls");
			printf("Enter the staff ID that you would like to delete by.\n");
			printf(">");
			rewind(stdin);
			gets(deleteTarget.processing_staff);

			while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
				if (strcmp(deleteTarget.processing_staff, tempRec.processing_staff) != 0) {
					fwrite(&tempRec, sizeof(FacUsageRec), 1, fu_out_ptr);
					continue;
				}
				delCount++;
				if (delCount == 1) {
					printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
					printf("============================================================================================================================================\n");
				};
				printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
			}
			break;

		case 4:
			system("cls");
			printf("Enter the facility ID that you would like to delete by.\n");
			printf(">");
			rewind(stdin);
			gets(deleteTarget.facility_id);

			while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
				if (strcmp(deleteTarget.facility_id, tempRec.facility_id) != 0) {
					fwrite(&tempRec, sizeof(FacUsageRec), 1, fu_out_ptr);
					continue;
				}
				delCount++;
				if (delCount == 1) {
					printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
					printf("============================================================================================================================================\n");
				};
				printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
			}
			break;

		case 5:
			system("cls");
			printf("Enter the booking ID that you would like to delete by.\n");
			printf(">");
			rewind(stdin);
			gets(deleteTarget.booking_id);

			while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
				if (strcmp(deleteTarget.booking_id, tempRec.booking_id) != 0) {
					fwrite(&tempRec, sizeof(FacUsageRec), 1, fu_out_ptr);
					continue;
				}
				delCount++;
				if (delCount == 1) {
					printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
					printf("============================================================================================================================================\n");
				};
				printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
			}
			break;

		}

	}

	printf("\n\nFound %d matching record(s).\n", delCount);
	if (delCount > 0) {
		printf("Delete the following record(s)?");
		finalPrompt = toupper(getchar());

		if (finalPrompt == 'Y') {
			printf("%d record(s) deleted.\n", delCount);

			fclose(fu_ptr);
			fu_ptr = NULL;
			fclose(fu_out_ptr);
			fu_out_ptr = NULL;
			remove("fac_usage_rec.bin");
			rename("fu_temp.bin", "fac_usage_rec.bin");

			printf("Press enter to continue.\n");
			rewind(stdin);
			(void)getchar();
			*recNum -= delCount;
			return;
		}
		else { printf("Delete cancelled.\n"); };
	}
	fclose(fu_ptr);
	fu_ptr = NULL;
	fclose(fu_out_ptr);
	fu_out_ptr = NULL;
	remove("fu_temp.bin");
	printf("\nPress enter to exit.\n");
	rewind(stdin);
	(void)getchar();

}
	
void FacUsageSearch() {
	FILE* fu_ptr;
	FacUsageRec tempRec,queryRec;

	int prompt, queryCount = 0;
	
	system("cls");

	printf("Search Selected.\n");
	printf("Search by:\n");
	printf("\t [1] Date Before\n");
	printf("\t [2] Date After\n");
	printf("\t [3] Processing Staff\n");
	printf("\t [4] Duration Above\n");
	printf("\t [5] Duration Below\n");
	printf("\t [6] Member ID\n");
	printf("\t [7] Facility ID\n");
	printf("\t [8] Booking ID\n");
	printf("\t [0] Cancel Search\n\n");

	printf("Enter search type.\n");
	printf(">");
	while (scanf("%d", &prompt) != 1 || prompt > 8 || prompt < 0) {
	
		printf("\nInput Error. Please try again.\n");
		printf(">");
	
	}

	fu_ptr = fopen("fac_usage_rec.bin","rb");
	if (!fu_ptr) {
	
		printf("\nSearch cannot be used at the moment.\n");
		printf("Please contact person in charge for assistance.");
		printf("Press enter to return to menu");
		rewind(stdin);
		
		return;
	}
	system("cls");
	switch (prompt) {
	case 1:
		printf("Please input upper start date boundary.\n");
		printf(">");
		rewind(stdin);
		while (scanf("%d/%d/%d", &queryRec.starting_date.day, &queryRec.starting_date.month, &queryRec.starting_date.year) != 3 || queryRec.starting_date.day > 31 || queryRec.starting_date.month > 12) {
			printf("\nValue or format error.\nPlease try again.\n");
			printf(">");
			rewind(stdin);
		}
		system("cls");
		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			if (tempRec.starting_date.year > queryRec.starting_date.year) {
				continue;
			}
			else if (tempRec.starting_date.year == queryRec.starting_date.year) {
				if (tempRec.starting_date.month > queryRec.starting_date.month) {
					continue;
				}
				else if (tempRec.starting_date.month == queryRec.starting_date.month) {
					if (tempRec.starting_date.day > queryRec.starting_date.day) {
						continue;
					}

				}

			};
			queryCount++;
			if (queryCount == 1) {
				printf("\t\t\t\t\t\tRecords with Start Dates before %d/%d/%d\n\n",tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year);
				printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
				printf("============================================================================================================================================\n");
			}

			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
		}
		break;

	case 2:
		printf("Please input lower start date boundary.\n");
		printf(">");
		rewind(stdin);
		while (scanf("%d/%d/%d", &queryRec.starting_date.day, &queryRec.starting_date.month, &queryRec.starting_date.year) != 3 || queryRec.starting_date.day > 31 || queryRec.starting_date.month > 12) {
			printf("\nValue or format error.\nPlease try again.\n");
			printf(">");
		}
		system("cls");
		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			if (tempRec.starting_date.year < queryRec.starting_date.year) {
				continue;
			}
			else if (tempRec.starting_date.year == queryRec.starting_date.year) {
				if (tempRec.starting_date.month < queryRec.starting_date.month) {
					continue;
				}
				else if (tempRec.starting_date.month == queryRec.starting_date.month) {
					if (tempRec.starting_date.day < queryRec.starting_date.day) {
						continue;
					}

				}

			};
			queryCount++;
			if (queryCount == 1) {
				printf("\t\t\t\t\t\tRecords with Start Dates after %d/%d/%d\n\n", tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year);
				printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
				printf("============================================================================================================================================\n");
			}

			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
		}
		break;

	case 3:
		printf("Please input Staff ID.\n");
		printf(">");
		rewind(stdin);
		gets(queryRec.processing_staff);

		system("cls");
		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			if (strcmp(tempRec.processing_staff, queryRec.processing_staff) != 0) {
				continue;
			}
			queryCount++;
			if (queryCount == 1) {
				printf("\t\t\t\t\t\tRecords Processed by Staff ID  %s\n\n",  tempRec.processing_staff);
				printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
				printf("============================================================================================================================================\n");
			}

			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
		}
		break;

	case 4:
		printf("Please input lower duration boundary. (hr:min:ss)\n");
		printf(">");
		rewind(stdin);
		while (scanf("%d:%d:%d", &queryRec.duration.hour, &queryRec.duration.min, &queryRec.duration.sec) != 3 ||  queryRec.duration.min > 60 || queryRec.duration.sec > 60) {
			printf("\nValue or format error.\nPlease try again.\n");
			printf(">");
		}
		system("cls");
		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			if (queryRec.duration.hour > tempRec.duration.hour) {
				continue;
			}
			else if (queryRec.duration.hour == tempRec.duration.hour) {
				if (queryRec.duration.min > tempRec.duration.min) {
					continue;
				}
				else if (queryRec.duration.min == tempRec.duration.min) {
					if (queryRec.duration.sec > tempRec.duration.sec) {
						continue;
					}

				}

			}
			queryCount++;
			if (queryCount == 1) {
				printf("\t\t\t\t\t\tRecords With Longer Duration than %d hr %d min %d sec\n\n", tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec);
				printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
				printf("============================================================================================================================================\n");
			}

			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
		}
		break;

	case 5:
		printf("Please input upper duration boundary. (hr:min:ss)\n");
		printf(">");
		rewind(stdin);
		while (scanf("%d:%d:%d", &queryRec.duration.hour, &queryRec.duration.min, &queryRec.duration.sec) != 3 || queryRec.duration.min > 60 || queryRec.duration.sec > 60) {
			printf("\nValue or format error.\nPlease try again.\n");
			printf(">");
		}
		system("cls");
		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			if (queryRec.duration.hour < tempRec.duration.hour) {
				continue;
			}
			else if (queryRec.duration.hour == tempRec.duration.hour) {
				if (queryRec.duration.min < tempRec.duration.min) {
					continue;
				}
				else if (queryRec.duration.min == tempRec.duration.min) {
					if (queryRec.duration.sec < tempRec.duration.sec) {
						continue;
					}

				}

			}
			queryCount++;
			if (queryCount == 1) {
				printf("\t\t\t\t\t\tRecords With Shorter Duration than %d hr %d min %d sec\n\n", tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec);
				printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
				printf("============================================================================================================================================\n");
			}

			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
		}
		break;
	case 6:
		printf("Please input Member ID.\n");
		printf(">");
		rewind(stdin);
		gets(queryRec.member_id);

		system("cls");
		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			if (strcmp(tempRec.member_id, queryRec.member_id) != 0) {
				continue;
			}
			queryCount++;
			if (queryCount == 1) {
				printf("\t\t\t\t\t\tRecords of Member ID %s\n\n", tempRec.member_id);
				printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
				printf("============================================================================================================================================\n");
			}

			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
		}
		break;
	case 7:
		printf("Please input Facility ID.\n");
		printf(">");
		rewind(stdin);
		gets(queryRec.facility_id);

		system("cls");
		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			if (strcmp(tempRec.facility_id, queryRec.facility_id) != 0) {
				continue;
			}
			queryCount++;
			if (queryCount == 1) {
				printf("\t\t\t\t\t\tUsages in Facility ID %s\n\n", tempRec.facility_id);
				printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
				printf("============================================================================================================================================\n");
			}

			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
		}
		break;

	case 8:
		printf("Please input Booking ID. (For non-booking records input '-')\n");
		printf(">"); 
		rewind(stdin);
		gets(queryRec.booking_id);

		system("cls");
		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			if (strcmp(tempRec.booking_id, queryRec.booking_id) != 0) {
				continue;
			}
			queryCount++;
			if (queryCount == 1) {
				printf("\t\t\t\t\t\tRecord with Booking ID %s\n\n", tempRec.member_id);
				printf("FacUsage ID\tStart Date And Time\tDuration\tStaff ID    \tMember ID\t\tFacility ID\tBooking ID\n");
				printf("============================================================================================================================================\n");
			}

			printf("%6s\t\t%02d/%02d/%4d %02d:%02d:%02d\t%02d:%02d:%02d\t%-10s\t%-19s\t%-3s\t\t%-20s\n", tempRec.facUsageId, tempRec.starting_date.day, tempRec.starting_date.month, tempRec.starting_date.year, tempRec.starting_time.hour, tempRec.starting_time.min, tempRec.starting_time.sec, tempRec.duration.hour, tempRec.duration.min, tempRec.duration.sec, tempRec.processing_staff, tempRec.member_id, tempRec.facility_id, tempRec.booking_id);
		}
		break;
	case 0:
		printf("Search cancelled. ");
		fclose(fu_ptr);
		fu_ptr = NULL;
		printf("\nPress enter to return to selection screen.\n");
		rewind(stdin);
		(void)getchar();
		return;

	};
	
	printf("\n\n\n\t\t%d record(s) displayed.\n\n",queryCount);
	fclose(fu_ptr);
	fu_ptr = NULL;
	printf("\nPress enter to return to selection screen.\n");
	rewind(stdin);
	(void)getchar();

}

void FacUsageReports() {
	FILE* fu_ptr;
	FacUsageRec tempRec;
	SYSTEMTIME t;
	int choice;
	system("cls");

	fu_ptr = fopen("fac_usage_rec.bin","rb");
	
	if (!fu_ptr) {
	
		printf("Reports cannot currently be generated.\n");
		printf("Please ask person-in-charge for assistance.\n");
		printf("Press enter to return to menu...\n");
		rewind(stdin);
		(void)(getchar());
		return;
	}

	printf("Choose a report to view.\n");
	printf("\t[1] Total Usage By Facility\n");
	printf("\t[2] Total Usage By Member\n");
	printf("\t[0] Return to Menu\n");
	printf(">");

	rewind(stdin);
	while (scanf("%d", &choice) != 1 || choice < 0 || choice > 2) {
		printf("\nInvalid choice entered. Please try again\n");
		printf(">");
	};

	system("cls");

	if (choice == 1) {
		struct FacDuration facCollection[100];
		int facAmount = 0;
		FILE* readFaci;
		Facilities faci;

		readFaci = fopen("facilities.txt","r"); 

		if (!readFaci) {
			printf("Unable to access Facility records.\n");
			fclose(fu_ptr);

			printf("Please inform the person in charge for assistance.\n");
			printf("Press enter to return to menu...\n");
			rewind(stdin);
			(void)(getchar());
			return;
		}

		system("cls");
		
		while (fscanf(readFaci, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%s\n", faci.ID, faci.details.type, faci.details.description, faci.details.venue, &faci.maxUsers, faci.status) != EOF){
			strcpy(facCollection[facAmount].facility_id, faci.ID);
			strcpy(facCollection[facAmount].description, faci.details.description);
			facCollection[facAmount].total_time.hour = 0;
			facCollection[facAmount].total_time.min = 0;
			facCollection[facAmount].total_time.sec = 0;
			facAmount++;
		} // Copies each ID from facility.txt and puts them into a FacDuration Array
	
		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			for (int i = 0; i < facAmount; i++) {
				if (strcmp(facCollection[i].facility_id, tempRec.facility_id) == 0) {
					facCollection[i].total_time.hour += tempRec.duration.hour;
					facCollection[i].total_time.min += tempRec.duration.min;
					facCollection[i].total_time.sec += tempRec.duration.sec;
					break;
				};
			}
		} // Adds time from each record to their respective id.

		fclose(readFaci);
		fclose(fu_ptr);

		
		// sort it
		qsort(facCollection, facAmount, sizeof(struct FacDuration), CompareFacTime);

		GetLocalTime(&t);

		printf("\t\t\tReport on Total Usage Sorted By Faculty\n");
		printf("\t\t\t\t\t\t\t\t\t\tGenerated on %d-%d-%d\n",t.wDay,t.wMonth,t.wYear);
		printf("\t%-6s\t%-31s\tTotal Time\n","FacID","Faculty Description");
		printf("\t==============================================================================================\n");

		for (int j = 0; j < facAmount; j++) {
			
			long totalSec = facCollection[j].total_time.hour * 3600 + facCollection[j].total_time.min * 60 + facCollection[j].total_time.sec;
			
			facCollection[j].total_time.hour = totalSec / 3600;
			facCollection[j].total_time.min = (totalSec % 3600)/60;
			facCollection[j].total_time.sec = totalSec % 60;
			
			if (!facCollection[j].total_time.hour && !facCollection[j].total_time.min && !facCollection[j].total_time.sec) {
				continue;
			}
			printf("\t%-6s\t%-31s\t%d hr %d min %d sec\n", facCollection[j].facility_id,facCollection[j].description,facCollection[j].total_time.hour, facCollection[j].total_time.min,facCollection[j].total_time.sec);

		} // put all in right format and prints it (if file open print it in).

		printf("\n\nPress enter to return to selection screen.\n");
		rewind(stdin);
		(void)getchar();

	
	}
	else if (choice == 2) {
		struct MemberDuration memCollection[100];
		int memAmount = 0;
		FILE* readMember;
		struct member tempMember;

		readMember = fopen("member.txt", "r");

		if (!readMember) {
			printf("Unable to access Facility records.\n");
			fclose(fu_ptr);

			printf("Please inform the person in charge for assistance.\n");
			printf("Press enter to return to menu...\n");
			rewind(stdin);
			(void)(getchar());
			return;
		}

		system("cls");

		while (fscanf(readMember, "%[^|]|%c|%[^|]|%[^|]|%[^|]|%d|%d|%d|%s\n", tempMember.name, &tempMember.gender, tempMember.phoneNum, tempMember.id, tempMember.email, &tempMember.time.regDay, &tempMember.time.regMonth, &tempMember.time.regYear, tempMember.status) != EOF){
			strcpy(memCollection[memAmount].member_id, tempMember.id);
			strcpy(memCollection[memAmount].name, tempMember.name);
			memCollection[memAmount].total_time.hour = 0;
			memCollection[memAmount].total_time.min = 0;
			memCollection[memAmount].total_time.sec = 0;
			memAmount++;
		} // Copies each ID from facility.txt and puts them into a FacDuration Array

		while (fread(&tempRec, sizeof(FacUsageRec), 1, fu_ptr) > 0) {
			for (int i = 0; i < memAmount; i++) {
				if (strcmp(memCollection[i].member_id, tempRec.member_id) == 0) {
					memCollection[i].total_time.hour += tempRec.duration.hour;
					memCollection[i].total_time.min += tempRec.duration.min;
					memCollection[i].total_time.sec += tempRec.duration.sec;
					break;
				};
			}
		} // Adds time from each record to their respective id.

		fclose(readMember);
		fclose(fu_ptr);


		// sort it
		qsort(memCollection, memAmount, sizeof(struct MemberDuration), CompareMemberTime);

		GetLocalTime(&t);

		printf("\t\t\tReport on Total Usage Sorted By Member\n");
		printf("\t\t\t\t\t\t\t\t\t\tGenerated on %d-%d-%d\n", t.wDay, t.wMonth, t.wYear);
		printf("\t%-20s\t%-31s\tTotal Time\n", "Member ID", "Name");
		printf("\t==============================================================================================\n");

		for (int j = 0; j < memAmount; j++) {

			long totalSec = memCollection[j].total_time.hour * 3600 + memCollection[j].total_time.min * 60 + memCollection[j].total_time.sec;

			memCollection[j].total_time.hour = totalSec / 3600;
			memCollection[j].total_time.min = (totalSec % 3600) / 60;
			memCollection[j].total_time.sec = totalSec % 60;
			if (!memCollection[j].total_time.hour && !memCollection[j].total_time.min && !memCollection[j].total_time.sec) {
				continue;
			}
			printf("\t%-20s\t%-31s\t%d hr %d min %d sec\n", memCollection[j].member_id, memCollection[j].name, memCollection[j].total_time.hour, memCollection[j].total_time.min, memCollection[j].total_time.sec);
		}

		printf("\n\nPress enter to return to selection screen.\n");
		rewind(stdin);
		(void)getchar();


	}
	else {
		printf("Report Viewing Cancelled.\n");
		fclose(fu_ptr);
		printf("Press enter to return to menu...\n");
		rewind(stdin);
		(void)(getchar());
		return;
	}


}