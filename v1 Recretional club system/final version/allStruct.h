#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning (disable : 4996)

SYSTEMTIME t;
// Staff struct
struct loginTime {
	char date[31];
};

typedef struct {
	char name[41];
	char contact[13];
	char gender;
	char state[15];
	char ID[7];
	char password[21];
	int pwRecovery;
	char position[21];
	char email[41];
	int age;
	char status[10];
}Staff;

typedef struct {
	char ID[12];
	char name[41];
	char position[21];
	struct loginTime;
	char status[18];
}StaffLoginRec;
//End of Staff struct


//Faci struct
struct FacilitiesDetails {
	char type[21];
	char description[31];
	char venue[16];
};

typedef struct {
	char ID[5];
	struct FacilitiesDetails details;
	int maxUsers;
	char status[14];

}Facilities;
//End of faci struct


//Booking struct
struct BookingSystem {
	char bookingId[10];
	int day, mth, year;
	int bookDay, bookMth, bookYear;
	char memberId[10];
	int bookingTime;
	char facilityId[10];
};
//End of booking struct


//Member struct
struct memberRegis {
	int regDay, regMonth, regYear;
};

struct member {
	char name[30], phoneNum[15], id[20], gender, email[30];
	struct memberRegis time;
	char status[21];
};
//end of member struct


//Faci usage struct
typedef struct {

	int hour, min, sec;

}Time;

typedef struct {

	int day, month, year;

}Date;

typedef struct {

	char facUsageId[7]; //FU0001
	Date starting_date;
	Time starting_time;
	char processing_staff[20];
	Time duration;
	char member_id[20], facility_id[4], booking_id[10];

}FacUsageRec;

struct FacDuration {
	char facility_id[4];
	char description[31];
	Time total_time;
};

struct MemberDuration {
	char member_id[20];
	char name[31];
	Time total_time;
};
//end of faci usage struct