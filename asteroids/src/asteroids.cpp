/* Asteroids
Sample solution for assignment
Semester 2 -- Small Embedded Systems
Dr Alun Moon
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"
#include "utils.h"

/* Game state */
float elapsed_time;
int   score;
int   lives;
int		shields;
struct ship player;
int rockCount;

float Dt = 0.01f;

//NEW CODE
const coordinate_t centre = {
	240,136
};

const coordinate_t corner[] = {		
	{0,-10}, {-10,10}, {10,10}
};

double centreX = centre.x; //Center.
double centreY = centre.y;
double shipAX = centre.x+corner[0].x; //Top corner.
double shipAY = centre.y+corner[0].y;
double shipBX = centre.x+corner[1].x; //Bottom left corner.
double shipBY = centre.y+corner[1].y;
double shipCX = centre.x+corner[2].x; //Bottom right corner.
double shipCY = centre.y+corner[2].y;

bool leftPress = false;
bool rightPress = false;
bool upPress = false;
bool downPress = false;
bool middlePress = false;

rock_t *asteroids;
missile_t *shots;

Ticker model, view, controller;

bool paused = true;
/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);

/**
 * Intialise asteroid and shot linked list.
 */
void initialise() {
	asteroids = static_cast<rock_t*>(malloc(sizeof(rock_t)));
	asteroids->next=NULL;
	asteroids->p.x=0;
	asteroids->p.y=0;
	asteroids->v.x=2;
	asteroids->v.y=2;
	shots = static_cast<missile_t*>(malloc(sizeof(missile_t)));
	shots->next=NULL;
}


int main() {
	init_DBuffer();	
	initialise(); //Initialise linked lists.
	view.attach( draw, 0.025);
	model.attach( physics, Dt);
	controller.attach( controls, 0.1);
	lives = 3; //Number of lives given.
	shields = 3; //Number of shields given.
	while(true) {
		if (lives < 1) { //Game ends.
			view.detach();
			model.detach();
			controller.detach(); 
			drawGameOver(); //Displays 'Game Over' screen 
			//init_DBuffer();
		}
	}
}
