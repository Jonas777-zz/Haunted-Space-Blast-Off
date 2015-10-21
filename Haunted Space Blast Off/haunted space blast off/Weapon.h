//Weapon.h
//author jslink

#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include "ViewObject.h"

class Weapon{
private:
	int fire_slowdown; //fire slowdown
	int fire_countdown; //counts down to 0, then resets to fire slowdown
	std::string type; //type of weapon
	int ammo; //amount of shots left

public:
	Weapon();
	Weapon(int fire_rate, int init_ammo, std::string init_type);
	bool canFire(); //checks if the weapon can fire, and handles reverting back to default if out of ammo
	int getAmmo() const; 
	void setAmmo(int new_ammo);
	std::string getType() const;
	void setType(std::string new_type);
	void tick(); //counts down fire countdown, called in player's step()
};

#endif