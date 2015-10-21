//GameStart.cpp
//author jslink
//author jmcgowanmartin

#include "GameStart.h"

#include "ResourceManager.h"
#include "Logmanager.h"
#include "EventKeyboard.h"
#include "GameManager.h"
#include "WorldManager.h"

#include "Player.h"
#include "ItemDrop.h"
#include "Points.h"
#include "Ghost.h"
#include "Plant.h"
#include "Ruins.h"

GameStart::GameStart(){
	setType("GameStart");

	//dragonfly managers needed for this
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();

	//setup GameOversprite
	df::Sprite *p_temp_sprite = resource_manager.getSprite("gamestart");

	if (!p_temp_sprite){
		df::LogManager &log_manager = df::LogManager::getInstance();
		log_manager.writeLog("GameStart::GameStart(): Warning! Sprite '%s' not found", "gamestart");
	}
	else{
		setSprite(p_temp_sprite);
		setSpriteSlowdown(30);
		setTransparency('#');
	}

	//put in center of window
	setLocation(df::CENTER_CENTER);

	//register interest
	registerInterest(df::KEYBOARD_EVENT);

	//play start music
	p_music = df::ResourceManager::getInstance().getMusic("start music");
	playMusic();
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	world_manager.setBoundary(df::Box(df::Position(), 250, 250));
}

//event handler
int GameStart::eventHandler(const df::Event *p_e){
	df::GameManager &game_manager = df::GameManager::getInstance();

	if (p_e->getType() == df::KEYBOARD_EVENT){
		df::EventKeyboard *p_keyboard_event = (df::EventKeyboard *) p_e;
		switch (p_keyboard_event->getKey()){
		case df::Keyboard::Q:				//quit
			game_manager.setGameOver();
			break;
		default:
			start();
			break;
		}
		return 1;
	}
	return 0;
}

void GameStart::start(){
	//changes music
	p_music->stop();
	p_music = df::ResourceManager::getInstance().getMusic("game music");
	p_music->play();
	
	//add player and points object
	Player *p_player = new Player;
	new Points;

	//adds some intitial random itemdrops for player
	for (int i = 0; i < 5; i++){
		ItemDrop *p_item1 = new ItemDrop();
		p_item1->setPosition(df::Position(rand() % 250, rand() % 250));
	}

	//display
	//create lives view object
	df::ViewObject *p_vol = new df::ViewObject;
	p_vol->setLocation(df::TOP_CENTER);
	p_vol->setViewString("LIVES");
	p_vol->setValue(p_player->getLives());
	p_vol->setColor(df::WHITE);

	//create weapon view object
	df::ViewObject *p_wv = new df::ViewObject;
	p_wv->setType("WeaponView");
	p_wv->setLocation(df::TOP_LEFT);
	p_wv->setViewString(p_player->getWeapon()->getType());
	p_wv->setValue(p_player->getWeapon()->getAmmo());
	p_wv->setColor(df::WHITE);

	//add initial ghosts, this number can be tweaked
	for (int i = 0; i < 30; i++){
		new Ghost(p_player);
	}

	df::WorldManager &world_manager = df::WorldManager::getInstance();
	world_manager.setViewFollowing(p_player);

	//populate the map with background plants
	for (int i = 1; i < 150; i++){
		int plantchance = rand() % 20;
		Plant *plant = new Plant;
		plant->setPlantType(plantchance);
	}
	for (int i = 0; i < 20; i++){
		new Ruins;
	}


	//when game starts, become inactive
	setActive(false);
}

//override draw method
void GameStart::draw(){
	df::Object::draw();
}

//play music method
void GameStart::playMusic(){
	if (p_music == df::ResourceManager::getInstance().getMusic("game music")){
		p_music->stop();
	}
	p_music = df::ResourceManager::getInstance().getMusic("start music");
	p_music->play();
}