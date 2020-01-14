#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include "bubblesort.h"
#include "leon3.h"

#define CALC 6
#define FIN 7
#define ERR 8

/*Code Generator Matrix*/
const unsigned char G[7][4] = {
	{1,1,0,1},
	{1,0,1,1},
	{1,0,0,0},
	{0,1,1,1},
	{0,1,0,0},
	{0,0,1,0},
	{0,0,0,1}
};

/*Data to be transmitted*/
const unsigned char data[4] = {1,0,1,0};

/*Encoded msg*/
unsigned char msg[7] = {0,0,0,0,0,0,0};

/*Verify*/
unsigned char verify[7] = {1,0,1,1,0,1,0};

void encode(unsigned char*,const unsigned char*);

void encode(unsigned char *encoded, const unsigned char *dt){
	int _i,_j;
	for(_i=0;_i<7;_i++){
		for(_j=0;_j<4;_j++){
			encoded[_i] += G[_i][_j] & dt[_j]; /*it is possible to either AND it or multiply*/
		}
		encoded[_i]&=1;
	}
}

volatile int i=0;
volatile uint8_t c0Running = 1;
volatile uint8_t c1Running = 1;
volatile uint32_t C0total = 0;
volatile uint32_t C1total = 0;
uint8_t bsArray0[BSIZE];

int main(void){
	if(get_proc_index() == 0){//CPU0

	int x,error=0, y;
	GPIO_SET_OUTPUTS;
	GPIO_WRITE(0x0);

	start_processor(1);//start CPU1

		for(y=0;y<=1000;y++){

		}

		c0Running = 1;

		GPIO_SETPIN(CALC);
		encode(msg,data);

		for(x=0;x<7;x++){
			if(msg[x]!=verify[x]) error=1;
		}
		if(error) {
			GPIO_SETPIN(ERR);
		}

		for(x=0;x<7;x++) msg[x]=0; //msg initial state
		error=0;
		GPIO_WRITE(0x0);
		GPIO_SETPIN(FIN);
		c0Running = 0;
	}//fin CPU0	
	else{ //CPU(!=0)
		uint8_t bsArray1[BSIZE];
		c1Running = 1;

		C1total = 0;
		do{
			fillArray(bsArray1);
			bubblesort(bsArray1);
			C1total++;
		}while(c1Running == 1);




	}//fin CPU(!=0)
	
	return 0;
}
