#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#pragma warning(disable:4996)

struct PW {
	char password[21];
	int pwResetPin;
};

typedef struct {
	char ID[7];
	char name[31];
	struct PW pw;
	char position[21];
	char email[31];
	char status[9];
	int annualLeaveCount;
}Staff;

void main() {
	Staff staff, editedStaff;
	FILE* readStaff;
	FILE* modifyStaff;
	readStaff = fopen("staff.txt", "r");
	modifyStaff = fopen("temp_staff.txt", "w");


	char targetID[7];
	printf("Enter staff ID to modify: ");
	rewind(stdin);
	gets(targetID);
	while (!feof(readStaff)) {
		fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
		editedStaff = staff;
		if (strcmp(editedStaff.ID, targetID) == 0) {
			printf("Record found\n");
			printf("ID: %s\nName: %-31s\nPosition: %-21s\nEmail: %-31s\nStatus: %s\n\n", editedStaff.ID, editedStaff.name, editedStaff.position, editedStaff.email, editedStaff.status);
			char newName[31];
			printf("Enter new staff name: ");
			rewind(stdin);
			gets(newName);
			strcpy(editedStaff.name, newName);
		}
		fprintf(modifyStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", editedStaff.ID, editedStaff.name, editedStaff.pw.password, editedStaff.pw.pwResetPin, editedStaff.position, editedStaff.email, editedStaff.status, editedStaff.annualLeaveCount);
	}
	system("pause");

	fclose(readStaff);
	fclose(modifyStaff);

	remove("staff.txt");
	rename("tempStaff.txt", "staff.txt");

}