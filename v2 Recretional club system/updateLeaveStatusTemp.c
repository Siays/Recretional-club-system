/*
	for (int i = 0; i < totalRecord; i++) {
	while (!feof(readApprovedRec)) {
		fscanf(readApprovedRec, "%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^|]|%[^\n]\n", approvedLeave.ID, &approvedLeave.totalDay, &approvedLeave.sDay, &approvedLeave.sMonth, &approvedLeave.sYear, &approvedLeave.eDay, &approvedLeave.eMonth, &approvedLeave.eYear, approvedLeave.reason, approvedLeave.leaveStatus);
		while (!feof(readStaff)) {
			fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
			modifyStaff = staff;
			if (strcmp(approvedLeave.ID, modifyStaff.ID) == 0) {
				if (approvedLeave.sDay == sysDay && approvedLeave.sMonth == sysMonth && approvedLeave.sYear == sysYear && strcmp(modifyStaff.status, "Active") == 0) {
					strcpy(modifyStaff.status, "On leave");
				}
				validateEndOfLeaveDate(&approvedLeave.eDay + 1, &approvedLeave.eMonth, &approvedLeave.eYear);
				if (approvedLeave.eDay == sysDay && approvedLeave.eMonth == sysMonth && approvedLeave.eYear == sysYear && strcmp(modifyStaff.status, "On leave") == 0) {
					strcpy(modifyStaff.status, "Active");
				}
			}
			fprintf(writeStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", modifyStaff.ID, modifyStaff.name, modifyStaff.pw.password, modifyStaff.pw.pwResetPin, modifyStaff.position, modifyStaff.email, modifyStaff.status, modifyStaff.annualLeaveCount);
			printf("Hello");
		}
	}
	}*/

for (int i = 0; i < totalRecord; i++) {
	if (readApprovedRec != EOF)
		fscanf(readApprovedRec, "%[^|]|%d|%d-%d-%d|%d-%d-%d|%[^|]|%[^\n]\n", approvedLeave[i].ID, &approvedLeave[i].totalDay, &approvedLeave[i].sDay, &approvedLeave[i].sMonth, &approvedLeave[i].sYear, &approvedLeave[i].eDay, &approvedLeave[i].eMonth, &approvedLeave[i].eYear, approvedLeave[i].reason, approvedLeave[i].leaveStatus);

	else
		break;
}

int recordEndOfLeave;
while (!feof(readStaff)) {
	fscanf(readStaff, "%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^|]|%[^|]|%d\n", staff.ID, staff.name, staff.pw.password, &staff.pw.pwResetPin, staff.position, staff.email, staff.status, &staff.annualLeaveCount);
	modifyStaff = staff;
	for (int i = 0; i < totalRecord; i++) {
		tempRec = approvedLeave[i];
		if (strcmp(tempRec.ID, modifyStaff.ID) == 0) {
			if (tempRec.sDay == sysDay && tempRec.sMonth == sysMonth && tempRec.sYear == sysYear && strcmp(modifyStaff.status, "Active") == 0) {
				strcpy(modifyStaff.status, "On leave");
			}

			validateEndOfLeaveDate(&tempRec.eDay, &tempRec.eMonth, &tempRec.eYear);
			if (tempRec.eDay + 1 == sysDay && tempRec.eMonth == sysMonth && tempRec.eYear == sysYear && strcmp(modifyStaff.status, "On leave") == 0) {
				strcpy(modifyStaff.status, "Active");
				FILE* writeRec;
				writeRec = fopen("leaveResponseRecord.txt", "a");
				if (writeRec == NULL) {
					yellow();
					printf("Failed to open file.");
					resetColor();
					return;
				}
				fprintf(writeRec, "%s|%d|%d-%d-%d|%d-%d-%d|%s|%s\n", tempRec.ID, tempRec.totalDay, tempRec.sDay, tempRec.sMonth, tempRec.sYear, tempRec.eDay, tempRec.eMonth, tempRec.eYear, tempRec.reason, tempRec.leaveStatus);
				fclose(writeRec);
			}
			/*
			FILE* writeTempRec;
			//continue
			writeTempRec = fopen("tempLeaveApproved.txt", "w");
			if (writeTempRec == NULL) {
				yellow();
				printf("Failed to open file.");
				resetColor();
				return;
			}
			if (approvedLeave[i].eDay < sysDay && approvedLeave[i].eMonth < sysMonth && approvedLeave[i].eYear < sysYear) {
				fprintf(writeTempRec, "%s|%d|%d-%d-%d|%d-%d-%d|%s|%s\n", approvedLeave[i].ID, approvedLeave[i].totalDay, approvedLeave[i].sDay, approvedLeave[i].sMonth, approvedLeave[i].sYear, approvedLeave[i].eDay, approvedLeave[i].eMonth, approvedLeave[i].eYear, approvedLeave[i].reason, approvedLeave[i].leaveStatus);
			}
			else {
				continue;

			}
			fclose(writeTempRec);*/
		}
	}
	fprintf(writeStaff, "%s|%s|%s|%d|%s|%s|%s|%d\n", modifyStaff.ID, modifyStaff.name, modifyStaff.pw.password, modifyStaff.pw.pwResetPin, modifyStaff.position, modifyStaff.email, modifyStaff.status, modifyStaff.annualLeaveCount);
}
fclose(readApprovedRec);
fclose(readStaff);
fclose(writeStaff);
//remove("leaveApproved.txt");
//rename("tempLeaveApproved.txt", "leaveApproved.txt");
