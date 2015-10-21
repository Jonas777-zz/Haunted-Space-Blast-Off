//Plant.cpp
//author jslink

#include "Plant.h"
#include "WorldManager.h"
#include "GraphicsManager.h"
#include "EventOut.h"


#include <stdlib.h>

Plant::Plant(){

	//set object type
	setType("Plant");

	//set solidness
	setSolidness(df::SPECTRAL);

	//put Plants in background
	setAltitude(0);

	//give Plant a random location
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	df::Position pos(rand() % 250, rand() % 250);
	setPosition(pos);

}

//draw method dependant on which type of plant it is
void Plant::draw(){
	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
	if (plant_type < 15){
		graphics_manager.drawCh(getPosition(), PLANT_CHAR, df::GREEN);
	}
	else{
		graphics_manager.drawCh(getPosition(), PLANT_CHAR_2, df::GREEN);
	}
}

//event handler
int Plant::eventHandler(const df::Event *p_e){
	return 0;
}

//set plant type for drawing
void Plant::setPlantType(int new_plant_type){
	plant_type = new_plant_type;
}
