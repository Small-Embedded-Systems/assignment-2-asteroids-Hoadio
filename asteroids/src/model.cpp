/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"

//NEW CODE
double AX, AY, BX, BY, CX, CY;
double  hypotenuse = 10;
float angle = 0.0;
float angle_stepsize = 0.5;
//NEW CODE

/**
 * Turns the ship left.
 */
void rotateShipLeft() {
	angle -= angle_stepsize;
	AX = hypotenuse * cos(angle);
	AY = hypotenuse * sin(angle);
	BX = hypotenuse * cos(angle+2.44346);	
	BY = hypotenuse * sin(angle+2.44346);	
	CX = hypotenuse * cos(angle+3.83972);	
	CY = hypotenuse * sin(angle+3.83972);		
	shipAX = centreX + AX;
	shipAY = centreY + AY;
	shipBX = centreX + BX;
	shipBY = centreY + BY;
	shipCX = centreX + CX;
	shipCY = centreY + CY;
	leftPress = false;
}

/**
 * Turns the ship right.
 */
void rotateShipRight() {
	angle += angle_stepsize;
	//Workout new coordinates based on rotation.
	AX = hypotenuse * cos(angle);
	AY = hypotenuse * sin(angle);
	BX = hypotenuse * cos(angle+2.44346);
	BY = hypotenuse * sin(angle+2.44346);
	CX = hypotenuse * cos(angle+3.83972);
	CY = hypotenuse * sin(angle+3.83972);	
	//Sets ship to new coordinates.
	shipAX = centreX + AX;
	shipAY = centreY + AY;
	shipBX = centreX + BX;
	shipBY = centreY + BY;
	shipCX = centreX + CX;
	shipCY = centreY + CY;
	rightPress = false;
}

/**
 * Moves ship in the forward direction.
 */
void moveShip() {
	float vX = 0.1*(hypotenuse * cos (angle));
	float vY = 0.1*(hypotenuse * sin (angle));
	centreX += vX;
	centreY += vY;
	shipAX += vX;
	shipAY += vY;
	shipBX += vX;
	shipBY += vY;
	shipCX += vX;
	shipCY += vY;
}

/**
 * Creates a new asteroid.
 */
void newRock(rock_t* head) {
	if (rockCount < 4) {
		rock_t* current = head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = (rock_t*)malloc(sizeof(rock_t));
		current->next->p.x = randrange(10, 480); //Sets random y coord
		current->next->p.y = randrange(10, 260); //Sets random y coord
		current->next->v.x = randrange(-2, 2); //Sets random velocity in x
		current->next->v.y = randrange(-2, 2); //Sets random velcoity in y
		if (current->next->v.x == 0 && current->next->v.y == 0) {		//if asteroid has no velocity
			current->next->v.x = 1;
			current->next->v.y = 1;
		}
		current->next->next = NULL;
		rockCount++;
	}
}

/**
 * Moves the asteroid.
 */
void moveRock(rock_t* head) {
	rock_t* current = head;
	if (current !=NULL) {
		current->p.x += current->v.x;
		current->p.y += current->v.y;
		
		//Relocates asteroid if it goes off the screen.
		if (current->p.x>490) {
			current->p.x = -10;
		} if (current->p.x<-10) {
			current->p.x = 490;
		} if (current->p.y>280) {
			current->p.y = -10;
		} if (current->p.y<-10) {
			current->p.y = 280;
		}
		current = current->next;
		moveRock(current);
	}
}

void physics(void) {
	newRock(asteroids);
	moveRock(asteroids);	
	//Movement of ship based on joystick input.
	if (leftPress == true)
		rotateShipLeft();
	if (rightPress == true)
		rotateShipRight();
	if (upPress == true) 
		moveShip();
}
