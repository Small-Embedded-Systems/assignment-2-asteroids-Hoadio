/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"


/** 
 * Joystick 5-way switch
 */
enum position { right,down,left,up, middle };	//Switched left and right.
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3}; //Set pins.

//NEW CODE
bool jsPrsdAndRlsd(position pos);
//END NEW CODE

/**
 * Users input of the joystick.
 */
void controls(void) {
	if (jsPrsdAndRlsd(up)) {
		downPress = false;
		upPress = true;
	}
	else if (jsPrsdAndRlsd(left)) {
		leftPress = true;
	}
	else if (jsPrsdAndRlsd(right)) {
		rightPress = true;
	}
	else if (jsPrsdAndRlsd(down)) {
		downPress = true;
	}
	else if (jsPrsdAndRlsd(middle)) {
		middlePress = true; 
	}
}


//NEW CODE
bool jsPrsdAndRlsd(position pos) {
	bool result = false;
	uint32_t state;
	static uint32_t savedState[5] = {1,1,1,1,1};	//initially all 1s: nothing pressed  //static uint32_t savedState[4] = {1,1,1,1};
	state = joystick[pos].read();
  if ((savedState[pos] == 0) && (state == 1)) {
		result = true;
	}
	savedState[pos] = state;
	return result;
}
