//
// Ghost.h
// author jmcgownmartin

#ifndef GHOST_H
#define GHOST_H

#include "Object.h"
#include "EventCollision.h"
#include "Player.h"

class Ghost : public df::Object {

private:
	void moveToStart(); //moves to initial starting point
	void updatePath(); //updates velocity based on player location
	void hit(const df::EventCollision *p_collision_event); //handles collisions
	int spawnSide; //which side the ghost spawns on, 0, 1, 2, or 3
	Player *target; //the player character that the ghost is chasing
	bool mega; //true if mega-ghost, false otherwise

public:
	Ghost(Player * hero); //must have a pointer to the player
	~Ghost();
	int eventHandler(const df::Event *p_e);
};

#endif