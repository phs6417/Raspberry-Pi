#include <wiringPi.h>
#include <stdio.h>
#include <softTone.h>
#include <time.h>

#define TouchPin	0
#define Gpin		1
#define Rpin		2
#define BtnPin		3
#define BuzPin    	4
#define LED1		5
	


#define  CM1  262
#define  CM2  294
#define  CM3  330
#define  CM4  350
#define  CM5  393
#define  CM6  441
#define  CM7  495


int song_1[] = {CM3,CM5,CM6,CM3,CM2,CM3,0,0,0,0,0};

int beat_1[] = {1,1,3,1,1,3,1,1,1,1,1,1,1};



int tmp = 0;

void LED(int color)
{
	pinMode(Gpin, OUTPUT);
	pinMode(Rpin, OUTPUT);
	pinMode(LED1, OUTPUT);
	

	if (color == 0)
	{
		digitalWrite(Rpin, HIGH);
		digitalWrite(Gpin, LOW);
		digitalWrite(LED1, 1);
	}

	else if (color == 1)
	{
		digitalWrite(Rpin, LOW);
		digitalWrite(Gpin, HIGH);
		digitalWrite(LED1, 0);

	}
	else
		printf("LED Error");
}

int main(void)
{

	int i, j;
	int TIME=0,ONTIME=0,OFFTIME=0;
	int count1=0,count2=0;

	time_t Itime;
	struct tm *today;
	
	

	if(wiringPiSetup() == -1){ //when initialize wiring failed,print message to screen
		printf("setup wiringPi failed !");
		return 1; 
	}
	
	pinMode(TouchPin, INPUT);
	pinMode(BtnPin,INPUT);

	if(softToneCreate(BuzPin) == -1){
		printf("setup softTone failed !");
		return 1; 
	}
	
	while(1){

		count1++;
		count2++;

		time(&Itime);
		today = localtime(&Itime);
		LED(digitalRead(TouchPin));
		if( digitalRead(TouchPin) != tmp ) 
		{
			if( digitalRead(TouchPin) == 0 )
			{
				
				printf("...ON	%d	hour	%d	minute	%d	second\n"
					,today->tm_hour,today->tm_min,today->tm_sec);
				count1=0;
			}
		
			if( digitalRead(TouchPin) == 1 )
			{
				
				printf("OFF..	%d	hour	%d	minute	%d	second\n"
					,today->tm_hour,today->tm_min,today->tm_sec);
				count2=0;
			}
			tmp = digitalRead(TouchPin);
		}

		if(digitalRead(BtnPin) == 0 )
		{
			
			for(i=0;i<10;i++){
				softToneWrite(BuzPin, song_1[i]);	
				delay(beat_1[i] * 500);
			}

			
		}

		if( count1 > 700000 || count2 > 700000 )
		{
			count1=0;
			count2=0;
			for(i=0;i<10;i++){
				softToneWrite(BuzPin, song_1[i]);	
				delay(beat_1[i] * 500);
			}
		}


	}
	return 0;
}
