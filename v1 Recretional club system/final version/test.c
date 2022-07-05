#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#pragma warning (disable :4996)


void main() {
	/*
	time_t t;
	time(&t);
	char date[51];
	printf("%s", ctime(&t));
	strcpy(date, ctime(&t));
	printf("%s", date);*/
	/*
	int a = 0;
	scanf("%d", &a);

	printf("%d", a);
	*/
	/*	char test[15] = "Hello World";

	printf("Enter new char:");
	rewind(stdin);
	gets(test);

	printf("%s", test);
	*/
	int pwLengthCount = 0;
	int pin;
	char pinTrack[7], ch;


	printf("Enter your pin");
	while (1) {
		rewind(stdin);
		ch = getch();
		if (ch == 13) {//if user key in "enter"
			pinTrack[pwLengthCount] = '\0'; //assign end of string at where
			//user key in "enter"
			break;
		}
		else if (ch == 8) {//if user key in "backspace"
			if (pwLengthCount > 0) {//and the length of password is >0
				pwLengthCount--;// -- the length
				printf("\b \b");//delete the previos input
			}
			else
				continue;//else of the length is 0, skip user backspace command
		}
		else if (ch == 9 || ch == 32) //if user input space / tab
			continue;//ignore the line

		else if (pwLengthCount == 20) {
			//red();//if user input hit the maximum password length, which is 20
			printf("\n\t\tMaximum password range!\n");
			printf("\t\tRe-enter password\n");
			//resetColor();//reset the password legth= 0; OR re-enter password
			pwLengthCount = 0;
		}

		else {
			pinTrack[pwLengthCount] = ch;//otherwise then assign whatever use
			pwLengthCount++;//input into password, and ++ the length
			printf("*");//output the password in asterick
		}

	}

	pin = atoi(pinTrack);
	
	printf("%d", pin);

}
