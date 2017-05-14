/* Asteroids view
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

#include "asteroids.h"
#include "model.h"
#include "utils.h"

#include "objects.h"

Display *graphics = Display::theDisplay();

//const colour_t background = rgb(0,51,102); /* Midnight Blue */

const colour_t background = rgb(0,0,0);

void timerHandler();

/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
	uint16_t *bufferbase = graphics->getFb();
	uint16_t *nextbuffer = bufferbase+ (480*272);
	LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
	uint16_t *buffer = graphics->getFb();
	graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
	LPC_LCD->UPBASE = (uint32_t)buffer;
}

void DrawPlayingArea() {
	//graphics->drawLine(0,26, 480,26, WHITE);	//LCD=480x272 pixels
}

/**
 * Displays the name of assignment.
 */
void DrawName() {
	graphics->setTextSize(1);
	graphics->setCursor(205,6);
	graphics->printf("ASTEROIDS");
}

/**
 * Displays the score for the user to view, updating based on the ticker.
 */
void DrawScore() {
	graphics->setTextSize(1);
	graphics->setCursor(390,6);
	graphics->printf("SCORE: %d", score);
}

/**
 * Displays the number of lives remaining.
 */
void DrawLives() {
	graphics->setTextSize(1);
	graphics->setCursor(4,6);
	graphics->printf("LIVES: %02d",lives); 
}

/**
 * Draws the ship on the screen.
 */
void DrawShip() {
	graphics->drawLine(shipAX,shipAY, shipBX,shipBY, RED); //Ship left					
	graphics->drawLine(shipAX,shipAY, shipCX,shipCY, RED); //Ship right					
	graphics->drawLine(shipBX,shipBY, centreX,centreY, GREEN); //Ship back left		
	graphics->drawLine(shipCX,shipCY, centreX,centreY, GREEN); //Ship back right		
	
	//Relocates ship if it goes off the screen.
	if (shipAX>500 && shipBX>480 && shipCX>480) {
		shipAX = 0;
		centreX = shipAX-10;
		shipBX = centreX-10;
		shipCX = centreX-10;
	} else if (shipAX<-10 && shipBX<0 && shipCX<0) {
		shipAX = 480;
		centreX = shipAX+10;
		shipBX = centreX+10;
		shipCX = centreX+10;
	}
	if (shipAY>276 && shipBY>276 && shipCY>276) {
		shipAY = 36;
		centreY = shipAY-10;
		shipBY = centreY-10;
		shipCY = centreY-10;
	}	else if (shipAY<26 && shipBY<26 && shipCY<26) {
		shipAY = 272;
		centreY = shipAY+10;
		shipBY = centreY+10;
		shipCY = centreY+10;
	}
}

/**
 * Draws the shieds around the ship.
 * Collision with asteroids (not working).
 */
void DrawShield (rock_t* head) {
	rock_t* current = head;
	for(int i=1; i<=shields; i++) {
		graphics->drawCircle(centreX, centreY, 15 + (i*3), BLUE);
	}
	if (current->p.x+8 > (centreX-10) && current->p.x+8 < (centreX+10) && current->p.y+8 > (centreY-10) && current->p.y+8 < (centreY+10)) {
		current->next->p.x = randrange(10, 470); //Relocates asteroid if it hits ship
		current->next->p.y = randrange(10, 260);
		shields--; //shield lost if it is hit by asteroid.
		score = score - 50; //Score deducted for losing a shield.
		if (shields < 1) {
			lives--; //lives are lost after all the shield are used.
			score -= 100; //Score deducted for losing a life.
		}
	}
}

/**
 * Draws the asteroid as a bitmap.
 */
void DrawAsteroid (rock_t *head) {
	rock_t* current = head;
	while (current != NULL) {
		graphics->drawBitmap(current->p.x, current->p.y, Asteroid, 16,16, WHITE); //Uses asteroid bitmap.
		current = current->next;
	}
}

/**
 * Draws the 'Game Over' screen, displays the total score also.
 */
void drawGameOver() {
	swap_DBuffer();
	graphics->fillScreen(background); //Fills the screen black.
	graphics->setTextSize(3);
	graphics->setCursor(150,56);
	graphics->printf("GAME OVER!"); //Displays 'Game Over' text.
	graphics->setTextSize(2);
	graphics->setCursor(180,150);
	graphics->printf("Score: %d", score); //Displays the users total, final score.
	init_DBuffer();
}

/**
 * Increments the score.
 */
void incrementScore() {
	score++;
}

/**
 * Draws the game.
 */
void draw(void) {
	graphics->fillScreen(background);
	incrementScore();
	DrawPlayingArea();
	DrawName();
	DrawScore();
	DrawLives();
	DrawShip();
	DrawShield(asteroids);
	DrawAsteroid(asteroids);	
	swap_DBuffer();
}
