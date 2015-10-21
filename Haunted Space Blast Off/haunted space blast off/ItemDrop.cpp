//ItemDrop.cpp
//author jslink

#include "ItemDrop.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "Player.h"

#include <stdlib.h>

ItemDrop::ItemDrop(){

	//dragonfly managers needed for this class
	df::LogManager &log_manager = df::LogManager::getInstance();
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	//sets what type of item the itemdrop is
	//and sets the type of sprite accordingly
	
	int typeId = (rand() % 99) + 1;
	df::Sprite *p_temp_sprite;
	
	if (typeId >= 1 && typeId <10){
		setType("HealthPack");
		p_temp_sprite = resource_manager.getSprite("healthpack");
	}
	else if (typeId >= 10 && typeId <35){
		setType("Rifle");
		p_temp_sprite = resource_manager.getSprite("rifle");
	}
	else if (typeId >= 35 && typeId < 50){
		setType("Shotgun");
		p_temp_sprite = resource_manager.getSprite("shotgun");
	}
	else{
		setType("Ammo");
		p_temp_sprite = resource_manager.getSprite("ammo");
	}

	//sets sprite
	if (!p_temp_sprite){
		log_manager.writeLog("ItemDrop::ItemDrop(): Warning! Sprite '%s' not found",  getType());
	}
	else{
		setSprite(p_temp_sprite);
		setSpriteSlowdown(5);
	}

	setSolidness(df::SOFT);
}

//event handler
int ItemDrop::eventHandler(const df::Event *p_e){
	//collision with player
	if (p_e->getType() == df::COLLISION_EVENT){
		const df::EventCollision *p_collision_event = static_cast <const df::EventCollision *> (p_e);
		hit(p_collision_event);
		return 1;
	}
	return 0;
}

//when itemdrop collides with player, remove itemdrop
void ItemDrop::hit(const df::EventCollision *p_c){
	if (p_c->getObject1()->getType() == "Player"){
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.markForDelete(p_c->getObject2());
	}
	if (p_c->getObject2()->getType() == "Player"){
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.markForDelete(p_c->getObject1());
	}
}