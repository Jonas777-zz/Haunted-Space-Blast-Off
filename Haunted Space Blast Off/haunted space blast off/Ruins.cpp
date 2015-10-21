//
// Ruins.cpp
//
//author jmcgowanmartin

#include "Ruins.h"
#include "WorldManager.h"
#include "GraphicsManager.h"
#include "EventOut.h"
#include "ResourceManager.h"
#include "LogManager.h"

#include <stdlib.h>

Ruins::Ruins(){
	df::LogManager &log_manager = df::LogManager::getInstance();
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();

	//set object type
	setType("Ruins");

	ruins_type = (rand() % 3) + 1;

	df::Sprite *p_temp_sprite;
	switch (ruins_type){
	case 1:
		p_temp_sprite = resource_manager.getSprite("ruins1");
		if (!p_temp_sprite) {
			log_manager.writeLog("Ghost::Ghost(): Warning! Sprite '%s' not found",
				"ruins1");
		}
		else {
			setSprite(p_temp_sprite);
		}
		break;
	case 2:
		p_temp_sprite = resource_manager.getSprite("ruins2");
		if (!p_temp_sprite) {
			log_manager.writeLog("Ghost::Ghost(): Warning! Sprite '%s' not found",
				"ruins2");
		}
		else {
			setSprite(p_temp_sprite);
		}
		break;
	case 3:
		p_temp_sprite = resource_manager.getSprite("ruins3");
		if (!p_temp_sprite) {
			log_manager.writeLog("Ghost::Ghost(): Warning! Sprite '%s' not found",
				"ruins3");
		}
		else {
			setSprite(p_temp_sprite);
		}
	}
	

	//set solidness
	setSolidness(df::HARD);

	//put Ruins in background
	setAltitude(0);

	//give Ruins a random location
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	df::Position pos(rand() % 250, rand() % 250);
	setPosition(pos);

}

/*
//draw method dependant on which type of ruins it is
void Ruins::draw(){
	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
	if (ruins_type < 15 && ruins_type > 8){
		//graphics_manager.drawFrame(getPosition(), df::Frame()
	}
	else{
		//graphics_manager.drawCh(getPosition(), PLANT_CHAR_2, df::GREEN);
	}
}
*/

//event handler
int Ruins::eventHandler(const df::Event *p_e){
	return 0;
}

//set plant type for drawing
void Ruins::setRuinsType(int new_plant_type){
	ruins_type = new_plant_type;
}
