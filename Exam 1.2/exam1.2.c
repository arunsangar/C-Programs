//---------------------------------------------------------------------------
//Name: Arun Sangar
//Course: CPSC 223C
//Date: 22 October 2015
//Exam: 1.2
//---------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct node NODE;

struct node
{
	int day;
	char name[15];
	char appointment[8];
};

void InitializeRAF(FILE*);
void CopyTxtToRAF(FILE*, FILE*);
void MoveWeekendAppointment(int, FILE*);
void ChangeAppointmentTime(char*, int, FILE*);
void SwapAppointments(int, int, FILE*);

int main()
{
	//print date and time
	time_t t = time(NULL);
	printf(ctime(&t));

	FILE *ftxt = fopen("data.txt", "r");
	FILE *fraf = fopen("data.bin", "wb+");

	InitializeRAF(fraf);

	CopyTxtToRAF(ftxt, fraf);

	MoveWeekendAppointment(5, fraf);
	MoveWeekendAppointment(6, fraf);

	ChangeAppointmentTime("4:00PM", 3, fraf);
	ChangeAppointmentTime("3:00PM", 4, fraf);
	
	SwapAppointments(0, 3, fraf);

	fclose(ftxt);
	fclose(fraf);

	return 0;
}

void InitializeRAF(FILE *fraf)
{
	NODE zero = { 0, "nnnn", "tttt" };

	//zero every record
	for (int i = 0; i < 7; i++)
		fwrite((NODE*)&zero, sizeof(NODE), 1, fraf);

	//move file pointer to start of file
	rewind(fraf);
}

void CopyTxtToRAF(FILE *ftxt, FILE *fraf)
{
	NODE p;
	char tempString[4], tempChar = 'a';

	while (!feof(ftxt))
	{
		fscanf(ftxt, "%s", tempString);
		//strcmp was not working for me, I had to use this method..
		switch (tempString[0])
		{
		case 'M': p.day = 0; break;
		case 'T': if (tempString[1] == 'u') p.day = 1; else p.day = 3; break;
		case 'W': p.day = 2; break;
		case 'F': p.day = 4; break;
		case 'S': if (tempString[1] == 'a') p.day = 5; else p.day = 6; break;
		}

		//check if the record is active
		fscanf(ftxt, "%c", &tempChar);
		if (tempChar == '\n') continue;

		//get the name and appointment time if it is
		fscanf(ftxt, "%s", p.name);
		fscanf(ftxt, "%s%c", p.appointment, &tempChar);

		//calculate byteoffset
		int byteoffset = (p.day * sizeof(NODE));

		//write the record to the random access file
		fseek(fraf, byteoffset, SEEK_SET);
		fwrite((NODE*)&p, sizeof(NODE), 1, fraf);
	}

	//move both file pointers back to start
	rewind(fraf);
	rewind(ftxt);
}

void MoveWeekendAppointment(int WeekendDay, FILE *fraf)
{
	NODE zero = { 0, "nnnn", "tttt" }, p;
	int openSlots[5] = { 0, 0, 0, 0, 0 };

	//check all the weekdays for open slots
	for (int i = 0; i < 5; i++)
	{
		fread((NODE*)&p, sizeof(NODE), 1, fraf);
		if (!strcmp(p.name, "nnnn")) openSlots[i] = 1;
	}

	//calculate byteoffset
	int byteoffset = WeekendDay * sizeof(NODE);

	//get the weekend appointment into temp storage 
	fseek(fraf, byteoffset, SEEK_SET);
	fread((NODE*)&p, sizeof(NODE), 1, fraf);
	//overwrite the weekend appointment
	fseek(fraf, byteoffset, SEEK_SET);
	fwrite((NODE*)&zero, sizeof(NODE), 1, fraf);

	//check for any open slots
	for (int i = 0; i < 5; i++)
	{
		if (openSlots[i] == 1)
		{
			//calculate the byteoffset of the open slot
			byteoffset = (i * sizeof(NODE));

			//write the record into the open slot
			fseek(fraf, byteoffset, SEEK_SET);
			fwrite((NODE*)&p, sizeof(NODE), 1, fraf);
		}
	}

	//move file pointer back to start
	rewind(fraf);
}

void ChangeAppointmentTime(char *time, int day, FILE *fraf)
{
	NODE p;
	int byteoffset = day * sizeof(NODE);

	//find the desired appointment appointment
	fseek(fraf, byteoffset, SEEK_SET);
	fread((NODE*)&p, sizeof(NODE), 1, fraf);
	//change the appointment time to the new time in temp storage
	strcpy(p.appointment, time);
	//write the new time to the raf file
	fseek(fraf, byteoffset, SEEK_SET);
	fwrite((NODE*)&p, sizeof(NODE), 1, fraf);

	//move file pointer back to start
	rewind(fraf);
}

void SwapAppointments(int appone, int apptwo, FILE *fraf)
{
	NODE personOne, personTwo;

	//calculate both byte offsets
	int byteoffsetOne = appone * sizeof(NODE);
	int byteoffsetTwo = apptwo * sizeof(NODE);

	//find the first one and read it inot temp storage
	fseek(fraf, byteoffsetOne, SEEK_SET);
	fread((NODE*)&personOne, sizeof(NODE), 1, fraf);

	//find second and read into temp storage
	fseek(fraf, byteoffsetTwo, SEEK_SET);
	fread((NODE*)&personTwo, sizeof(NODE), 1, fraf);

	//write the first record found into the second record's space
	fseek(fraf, byteoffsetTwo, SEEK_SET);
	fwrite((NODE*)&personOne, sizeof(NODE), 1, fraf);

	//write the second record found into the first record's space
	fseek(fraf, byteoffsetOne, SEEK_SET);
	fwrite((NODE*)&personTwo, sizeof(NODE), 1, fraf);

	//move file pointer back to start
	rewind(fraf);
}

