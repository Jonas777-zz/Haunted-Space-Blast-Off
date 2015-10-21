//Player.h
//author jslink

#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "Reticle.h"
#include "Bullet.h"
#include "Weapon.h"
#include "EventCollision.h"

class Player : public df::Object{

private:
	void kbd(const df::EventKeyboard *p_keyboard_event); //keyboard switch handler
	void move(int dx, int dy); //move function
	int move_slowdown; //move slowdown
	int move_countdown; //move countdown
	void step(); //step function
	void fire(df::Position target); //firing function, checks with weapon
	void mouse(const df::EventMouse *p_mouse_event); //mouse function
	Reticle *p_reticle; //the reticle
	int lives; //number of lives
	Weapon *weapon; //player's weapon
	df::Position bulletSpawn(df::Position target); //determines where the bullet should spawn, based on target location
	void hit(const df::EventCollision *p_collision_event); //hit function for when player collides with objects
	void makeBullet(df::Position target); //make bullet with correct velocity, helper for fire function

public:
	Player();
	int eventHandler(const df::Event *p_e);
	~Player();
	int getLives() const;
	void setLives(int new_lives);
	Weapon *getWeapon() const;
	void setWeapon(Weapon *new_weapon);
};

#endif