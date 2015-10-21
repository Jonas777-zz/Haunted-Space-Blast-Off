//Bullet.cpp
//author jslink

#include "Bullet.h"

#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "EventOut.h"

Bullet::Bullet(df::Position hero_pos){

	//dragonfly managers needed for this method
	df::LogManager &log_manager = df::LogManager::getInstance();
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	//setup bullet sprite
	df::Sprite *p_temp_sprite = resource_manager.getSprite("bullet");
	if (!p_temp_sprite){
		log_manager.writeLog("Bullet::Bullet(): Warning! Sprite '%s' not found", "bullet");
	}
	else{
		setSprite(p_temp_sprite);
		setSpriteSlowdown(5);
	}

	//set object type
	setType("Bullet");
	setSolidness(df::SOFT);

	//set velocity
	setXVelocity(1);

	//set starting location based on player location
	df::Position pos(hero_pos.getX(), hero_pos.getY());
	setPosition(pos);

}

//event handler
int Bullet::eventHandler(const df::Event *p_e){

	if (p_e->getType() == df::COLLISION_EVENT){
		const df::EventCollision *p_collision_event = static_cast <const df::EventCollision *> (p_e);
		hit(p_collision_event);
		return 1;
	}

	if (p_e->getType() == df::OUT_EVENT) {
		out();
		return 1;
	}
	return 0;
}

//out method, bullet is marked for deletion after leaving screen
void Bullet::out(){
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	world_manager.markForDelete(this);
}

//hit method, if bullet hits something, mark both objects for deletion
void Bullet::hit(const df::EventCollision *p_collision_event){
	//bullets dont hit player
	std::string o1 = p_collision_event->getObject1()->getType();
	std::string o2 = p_collision_event->getObject2()->getType();
	if ((o1 == "Player") || (o2 == "Player")){
		return;
	}
	//bullets dont hit each other
	if ((o1 == "Bullet") && (o2 == "Bullet")){
		return;
	}
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	if ((o1 == "HealthPack") || (o2 == "HealthPack") || (o1 == "Rifle") || (o2 == "Rifle") || 
		(o1 == "Ammo") || (o2=="Ammo") || (o1=="Ruins") || (o2=="Ruins") || (o1 == "Shotgun")
		|| (o2 == "Shotgun")){
		if ((o1 == "Ruins") || (o2 == "Ruins")){
			if (o1 == "Bullet"){
				world_manager.markForDelete(p_collision_event->getObject1());
			}
			else{
				world_manager.markForDelete(p_collision_event->getObject2());
			}
		}
		return;
	}
	world_manager.markForDelete(p_collision_event->getObject1());
	world_manager.markForDelete(p_collision_event->getObject2());
}