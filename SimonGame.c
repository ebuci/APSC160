
/ * Purpose: Simon Game*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <DAQlib.h>
#include <Windows.h>
#include <time.h>

#define NULL 0
#define TRUE 1
#define FALSE 0

//The states of LED
#define LED_ON 1
#define LED_OFF 0

//Input channel
#define BUTTON0 0
#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3

//The states of pushbottom
#define BUTTON_ON 1
#define BUTTON_OFF 0

//maximum sequence length
#define SEQUENCE_LENGTH 5

//time period for LED lash in millisecond
#define TIME_PAUSE 700
#define TIME_INTERVAL 500

//function prototypes
int randInt(int lower, int upper);
void generateSequence(int length, int data[]);
void display(int numberDisplay,int sequence[]);
int checkButton(int N, int sequence[], int response[]);
void runSimon(void);

/* main function */
int main(void){
    int setupNum;
    int sequence[SEQUENCE_LENGTH];

    srand((unsigned)time(NULL));  //Initialize the random generator
  
    printf("Please enter 6 to start the game: ");
    scanf("%i", &setupNum);

    if (setupDAQ(setupNum) == TRUE) {
        runSimon();
    }
    else {
        printf("Cannot intialize the game, please restart the game and try agian.");
    }

	system("PAUSE");
	return 0;
}

//This function generates a single random nubmer
int randInt(int lower, int upper) {
    int value;
    value = rand()% (upper+1); //The number generated will be bounded by lower and upper 
    return value;
}

//This function generates an array of random number
void generateSequence(int length, int data[]) {
    int lower = 0, upper = 3; //set up the boundary for the generator
    int i;
    for (i = 0; i < length; i++) {
        data[i] = randInt(lower, upper);
    }
}

//This function displays the corredponding LED of generated number
void display(int numberDisplay, int sequence[]) {
    int i;
    for (i = 0; i < numberDisplay;i++) {
        digitalWrite(sequence[i], LED_ON);
        Sleep(TIME_PAUSE);
        digitalWrite(sequence[i], LED_OFF);
        Sleep(TIME_INTERVAL);
    }
}

//This function tests if the user click the correct button(s)
int checkButton(int N, int sequence[], int response[]) {
    int i;
    int status;
    int counter = 0;
    int BUTTON_CLICK = BUTTON_ON;

    //Check the input
    while (counter < N) {
        if (digitalRead(BUTTON0) == BUTTON_ON) {
            while (BUTTON_CLICK == BUTTON_ON) {
                if (digitalRead(BUTTON0) == BUTTON_OFF) {
                    BUTTON_CLICK = BUTTON_OFF;
                    response[counter] = BUTTON0;
                    counter++;
                }
            }
            BUTTON_CLICK = BUTTON_ON;
        }
        if (digitalRead(BUTTON1) == BUTTON_ON) {
            while (BUTTON_CLICK == BUTTON_ON) {
                if (digitalRead(BUTTON1) == BUTTON_OFF) {
                    BUTTON_CLICK = BUTTON_OFF;
                    response[counter] = BUTTON1;
                    counter++;
                }
            }
            BUTTON_CLICK = BUTTON_ON;
        }
        if (digitalRead(BUTTON2) == BUTTON_ON) {
            while (BUTTON_CLICK == BUTTON_ON) {
                if (digitalRead(BUTTON2) == BUTTON_OFF) {
                    BUTTON_CLICK = BUTTON_OFF;
                    response[counter] = BUTTON2;
                    counter++;
                }
            }
            BUTTON_CLICK = BUTTON_ON;
        }
        if (digitalRead(BUTTON3) == BUTTON_ON) {
            while (BUTTON_CLICK == BUTTON_ON) {
                if (digitalRead(BUTTON3) == BUTTON_OFF) {
                    BUTTON_CLICK = BUTTON_OFF;
                    response[counter] = BUTTON3;
                    counter++;
                }
            }
            BUTTON_CLICK = BUTTON_ON;
        }
    }

    //Check if the input matches the generated array
    for (i = 0; i < N; i++) {
        if (sequence[i] == response[i]) {
            status = TRUE;
        }
        else {
            status = FALSE;
            return status;
        }
    }

    return status;
}
//simon game function
void runSimon(void) {
    int i;
    int N = 1; //number of LED, starts with 1 LED
    int sequence[SEQUENCE_LENGTH];
    int response[SEQUENCE_LENGTH];
    generateSequence(SEQUENCE_LENGTH, sequence);

    Sleep(1500); //This gives the player a response time 
    display(1, sequence);

    while (continueSuperLoop() == TRUE) {

        if (N < SEQUENCE_LENGTH) {

            if (checkButton(N, sequence, response) == TRUE) {
                N++;
                display(N, sequence);
            }
            //Check if the player loses
            else {
                //flash red LED three times
                for (i = 0;i < 3;i++) {
                    digitalWrite(1, LED_ON);
                    Sleep(TIME_PAUSE);
                    digitalWrite(1, LED_OFF);
                    Sleep(TIME_INTERVAL);
                }

                N = 1;
                Sleep(1000);
                generateSequence(SEQUENCE_LENGTH, sequence);
                display(N, sequence);
            }

        }
        //Check if the player wins, flash the green LED three times
        else{
            if (checkButton(N, sequence, response) == TRUE){
                for (i = 0;i < 3;i++) {
                    digitalWrite(0, LED_ON);
                    Sleep(TIME_PAUSE);
                    digitalWrite(0, LED_OFF);
                    Sleep(TIME_INTERVAL);
                }
                //reset the game
                N = 1;
                Sleep(1000);
                generateSequence(SEQUENCE_LENGTH, sequence);
                display(N, sequence);
                }
            else {
                //chek if the player loses, flash red LED three times
                for (i = 0;i < 3;i++) {
                    digitalWrite(1, LED_ON);
                    Sleep(TIME_PAUSE);
                    digitalWrite(1, LED_OFF);
                    Sleep(TIME_INTERVAL);
                }
                //reset the game
                N = 1;
                Sleep(1000);
                generateSequence(SEQUENCE_LENGTH, sequence);
                display(N, sequence);
            }
        }
     
    }
}