//Reticle.cpp
//author jslink

#include "Reticle.h"
#include "EventMouse.h"
#include "GraphicsManager.h"
#include "WorldManager.h"
#include "utility.h"

Reticle::Reticle(){

	//dragonfly managers needed for this method
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	//set object type
	setType("Reticle");

	//set solidness
	setSolidness(df::SPECTRAL);

	//draw in foreground
	setAltitude(df::MAX_ALTITUDE);

	//register for interest
	registerInterest(df::MOUSE_EVENT);

	//set intial position
	int world_horiz = world_manager.getBoundary().getHorizontal();
	int world_vert = world_manager.getBoundary().getVertical();
	df::Position pos(world_horiz / 2, world_vert / 2);
	setPosition(pos);
}

//event handler
int Reticle::eventHandler(const df::Event *p_e){

	if (p_e->getType() == df::MOUSE_EVENT){
		const df::EventMouse *p_mouse_event = static_cast <const df::EventMouse *> (p_e);
		if (p_mouse_event->getMouseAction() == df::MOVED){
			//change location to follow mouse
			setPosition(df::viewToWorld(p_mouse_event->getMousePosition()));
			return 1;
		}
	}
	return 0;
}

//draw reticle on window
void Reticle::draw(){
	df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
	graphics_manager.drawCh(getPosition(), RETICLE_CHAR, df::RED);
}