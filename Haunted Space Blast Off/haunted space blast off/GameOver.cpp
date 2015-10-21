//GameOver.cpp
//author jslink
//author jmcgowanmartin

#include "GameOver.h"

#include "Logmanager.h"
#include "ResourceManager.h"
#include "EventStep.h"
#include "WorldManager.h"
#include "GameManager.h"
#include "utility.h"
#include "GraphicsManager.h"

#include "GameStart.h"

GameOver::GameOver(){
	setType("GameOver");

	//dragonfly managers needed for this
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();

	//setup GameOversprite
	df::Sprite *p_temp_sprite = resource_manager.getSprite("gameover");

	if (!p_temp_sprite){
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("GameOver::GameOver(): Warning! Sprite '%s' not found", "gameover");
	}
	else{
		setSprite(p_temp_sprite);
		setSpriteSlowdown(15);
		setTransparency('#');
		time_to_live = (p_temp_sprite->getFrameCount() * 45);
	}

	//put in center of window
	setLocation(df::CENTER_CENTER);

	//register for step event
	registerInterest(df::STEP_EVENT);

	//play gameover sound
	df::Sound *p_sound = df::ResourceManager::getInstance().getSound("game over");
	p_sound->play();

	//when object exits, indicate game over
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	//remove ghosts and viewobjects, re-activate GameStart
	df::ObjectList object_list = world_manager.getAllObjects(true);
	df::ObjectListIterator i(&object_list);

}

//event handler
int GameOver::eventHandler(const df::Event *p_e){
	if (p_e->getType() == df::STEP_EVENT){
		step();
		return 1;
	}
	return 0;
}

//step method
void GameOver::step(){
	time_to_live--;
	if (time_to_live <= 0){
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.markForDelete(this);
	}
}

//destructor
GameOver::~GameOver(){
	//when object exits, indicate game over
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	
	//remove ghosts and viewobjects, re-activate GameStart
	df::ObjectList object_list = world_manager.getAllObjects(true);
	df::ObjectListIterator i(&object_list);
	
	for (i.first(); !i.isDone(); i.next()){
		df::Object *p_o = i.currentObject();
		if (p_o->getType() == "ghost" || p_o->getType() == "ViewObject" || p_o->getType() == "WeaponView" || p_o->getType() == "Plant"
			|| p_o->getType()=="Ruins"){
			if (p_o->getType() == "ViewObject"){
				if (static_cast <ViewObject *>  (p_o)->getViewString() == "POINTS"){
					df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
					int points = static_cast <ViewObject *>  (p_o)->getValue();
					char points_str[32];
					itoa(points, points_str,10);
					graphics_manager.drawString(viewToWorld(getPosition()), points_str , df::LEFT_JUSTIFIED, df::WHITE);
				}
			}
			world_manager.markForDelete(p_o);
		}
	}
	for (i.first(); !i.isDone(); i.next()){
		df::Object *p_o = i.currentObject();
		if (p_o->getType() == "Rifle" || p_o->getType() == "HealthPack" || p_o->getType() == "Ammo" || p_o->getType()=="Shotgun"){
			world_manager.markForDelete(p_o);
		}
		if (p_o->getType() == "GameStart"){
			p_o->setActive(true);
			p_o->setPosition(df::viewToWorld(df::Position(world_manager.getView().getHorizontal() / 2, world_manager.getView().getVertical() / 2)));
			static_cast <GameStart *>  (p_o)->playMusic();
		}
	}
}

//draw method to override default
void GameOver::draw(){
	df::Object::draw();
}