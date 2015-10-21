//Weapon.cpp
//author jslink

//game includes
#include "Weapon.h"

//engine includes
#include "EventView.h"
#include "WorldManager.h"

//default constructor
//normal fire rate, unlimited ammo
Weapon::Weapon(){
	//firing slowdowns
	fire_slowdown = 14;
	fire_countdown = fire_slowdown;

	ammo = 0;
	setType("Space Revolver");
}

//constructor given fire slowdown, ammo count, and weapon type
Weapon::Weapon(int fire_rate, int init_ammo, std::string init_type){
	fire_slowdown = fire_rate;
	fire_countdown = fire_slowdown;

	ammo = init_ammo;
	setType(init_type);
}

//-checks if the weapon can fire
//-if it is a space revolver, just checks countdown
//-if it is another type, checks countdown and ammo
//-if ammo is <=0, and type is not default, switches
//  type to default and does countdown check
bool Weapon::canFire(){
	if (getType() == "Space Revolver"){
		//space revolver does not consume ammo
		if (fire_countdown > 0){
			return false;
		}
		fire_countdown = fire_slowdown;
		return true;
	}
	else if (ammo > 0 && getType() != "Space Revolver"){
		//if weapon consumes ammo, do so
		if (fire_countdown > 0){
			return false;
		}
		fire_countdown = fire_slowdown;
		ammo--;
		df::EventView ev(getType(), -1, true);
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.onEvent(&ev);
		return true;
	}
	else if(ammo <= 0 && getType() != "Space Revolver" ){
		//if weapon runs out of ammo, reverts back to space revolver
		setType("Space Revolver"); 
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		df::ObjectList object_list = world_manager.getAllObjects(true);
		df::ObjectListIterator i(&object_list);

		for (i.first(); !i.isDone(); i.next()){
			df::Object *p_o = i.currentObject();
			if (p_o->getType() == "WeaponView"){
				(dynamic_cast <df::ViewObject *> (p_o))->setViewString(getType());
				(dynamic_cast <df::ViewObject *> (p_o))->setValue(0);
			}
		}
		fire_slowdown = 14;
		fire_countdown = fire_slowdown;
		return false;
	}
	else{
		return false;
	}
}

//getters and setters for ammo and type
int Weapon::getAmmo()const{
	return ammo;
}

void Weapon::setAmmo(int new_ammo){
	ammo = new_ammo;
}

std::string Weapon::getType()const{
	return type;
}

void Weapon::setType(std::string new_type){
	type = new_type;
}

//tick function, counts down firing from player
void Weapon::tick(){
	fire_countdown--;
	if (fire_countdown < 0){
		fire_countdown = 0;
	}
}