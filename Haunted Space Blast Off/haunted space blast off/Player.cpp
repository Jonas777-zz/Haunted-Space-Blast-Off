//Player.cpp
//author jslink
//author jmcgowanmartin

//game includes
#include "Player.h"

//engine includes
#include "EventStep.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "LogManager.h"
#include "GameManager.h"
#include "EventView.h"
#include "ViewObject.h"

#include "GameOver.h"
#include "Ghost.h"

//default constructor
Player::Player(){
	// link to player sprite
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	df::LogManager &log_manager = df::LogManager::getInstance();
	df::Sprite *p_temp_sprite;
	p_temp_sprite = resource_manager.getSprite("player");

	//movement slowdowns
	move_slowdown = 2;
	move_countdown = move_slowdown;

	//starts out with a faster weapon
	//weapon = new Weapon(15, 30, "Space Shotgun");

	//starts out with default weapon
	weapon = new Weapon();

	//set number of lives
	lives = 3;

	//create reticle
	p_reticle = new Reticle();
	p_reticle->draw();

	if (!p_temp_sprite){
		log_manager.writeLog("Player::Player(): Warning! Sprite '%s' not found",
			"player");
	}
	else{
		setSprite(p_temp_sprite);
		setSpriteSlowdown(3);	// 1/3 speed animation
		setTransparency();		//transparent sprite
	}

	//registers for various interests
	registerInterest(df::KEYBOARD_EVENT);
	registerInterest(df::STEP_EVENT);
	registerInterest(df::MOUSE_EVENT);

	//sets type
	setType("Player");
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	df::Position pos(world_manager.getBoundary().getHorizontal() / 2 , world_manager.getBoundary().getVertical() / 2);
	setPosition(pos);

}

//destructor
Player::~Player(){
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	GameOver *go = new GameOver;
	go->setPosition(df::viewToWorld(df::Position(world_manager.getView().getHorizontal() / 2, world_manager.getView().getVertical() / 2)));
	
	world_manager.markForDelete(p_reticle);
}

//event handler
int Player::eventHandler(const df::Event *p_e){
	//keyboard event
	if (p_e->getType() == df::KEYBOARD_EVENT){
		const df::EventKeyboard *p_keyboard_event = static_cast <const df::EventKeyboard *> (p_e);
		kbd(p_keyboard_event);
		return 1;
	}
	//step event
	if (p_e->getType() == df::STEP_EVENT){
		step();
		return 1;
	}
	//mouse event
	if (p_e->getType() == df::MOUSE_EVENT){
		const df::EventMouse *p_mouse_event = static_cast <const df::EventMouse *> (p_e);
		mouse(p_mouse_event);
		return 1;
	}
	//collision with powerup or ghost
	if (p_e->getType() == df::COLLISION_EVENT){
		const df::EventCollision *p_collision_event = static_cast <const df::EventCollision *> (p_e);
		hit(p_collision_event);
		return 1;
	}

	return 0;
}

//move function
void Player::move(int dx, int dy){
	//see if time to move
	if (move_countdown > 0){
		return;
	}
	move_countdown = move_slowdown;

	//if the player stays on window, allow move
	//should be redundant because view is locked to player
	df::Position new_pos(getPosition().getX() + dx, getPosition().getY() + dy);
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	if ((new_pos.getY() > 3) && (new_pos.getY() < world_manager.getBoundary().getVertical() - 1)){
		if ((new_pos.getX() > 0) && new_pos.getX() < world_manager.getBoundary().getHorizontal() - 1){
			//only move if in boundaries
			world_manager.moveObject(this, new_pos);
		}
	}
}

//keyboard button handler
void Player::kbd(const df::EventKeyboard *p_keyboard_event){
	switch (p_keyboard_event->getKey()){
	case df::Keyboard::Q:				//quit
		if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED){
			df::WorldManager &world_manager = df::WorldManager::getInstance();
			world_manager.markForDelete(this);
		}
		break;
	case df::Keyboard::W:				//up
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN){
			move(0,-1);
		}
		break;
	case df::Keyboard::S:				//down
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN){
			move(0, 1);
		}
		break;
	case df::Keyboard::A:				//left
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN){
			move(-2, 0);
		}
		break;
	case df::Keyboard::D:				//right
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN){
			move(2, 0);
		}
		break;
	}
}

//decrease rate restriction counters
void Player::step(){
	//move countdown
	move_countdown--;
	if (move_countdown < 0){
		move_countdown = 0;
	}

	//fire countdown
	weapon->tick();

}

//spawns the bullet displaced from the player in the appropriate
//direction, based on the target position
df::Position Player::bulletSpawn(df::Position target){
	df::Position b_pos;

	if (getPosition().getX()<target.getX() && getPosition().getY() > target.getY()){
		b_pos = df::Position(getPosition().getX()+2, getPosition().getY()-1);
	}
	else if (getPosition().getX()<target.getX() && getPosition().getY()==target.getY()){
		b_pos = df::Position(getPosition().getX() + 2, getPosition().getY());
	}
	else if (getPosition().getX()<target.getX() && getPosition().getY() < target.getY()){
		b_pos = df::Position(getPosition().getX() + 2, getPosition().getY()+1);
	}
	else if (getPosition().getX()>target.getX() && getPosition().getY() > target.getY()){
		b_pos = df::Position(getPosition().getX() - 2, getPosition().getY()-1);
	}
	else if (getPosition().getX()>target.getX() && getPosition().getY() < target.getY()){
		b_pos = df::Position(getPosition().getX() - 2, getPosition().getY() + 1);
	}
	else if (getPosition().getX()>target.getX() && getPosition().getY() == target.getY()){
		b_pos = df::Position(getPosition().getX() - 2, getPosition().getY());
	}
	else if (getPosition().getX()==target.getX() && getPosition().getY() > target.getY()){
		b_pos = df::Position(getPosition().getX(), getPosition().getY()-1);
	}
	else if (getPosition().getX() == target.getX() && getPosition().getY() < target.getY()){
		b_pos = df::Position(getPosition().getX(), getPosition().getY() + 1);
	}
	else{
		b_pos = getPosition();
	}
	
	return b_pos;
}

//make bullet with correct velocity
//helper for fire
void Player::makeBullet(df::Position target){
	df::Position b_pos = bulletSpawn(target);
	Bullet *p = new Bullet(b_pos);
	if (target.getX() != getPosition().getX()){
		p->setYVelocity((float)(target.getY() - getPosition().getY()) / (float)(target.getX() - getPosition().getX()));
	}
	else{
		if (target.getY() < getPosition().getY()){
			p->setYVelocity(-1);
		}
		else{
			p->setYVelocity(1);
		}
		p->setXVelocity(0);
	}
	if (target.getX() < getPosition().getX()){
		p->setXVelocity(p->getXVelocity() * -1);
		p->setYVelocity(p->getYVelocity() * -1);
	}
}

//fire method
void Player::fire(df::Position target){
	//check if weapon can fire
	if (weapon->canFire()){
		//fire bullet towards target
		makeBullet(target);
		if (getWeapon()->getType() == "Space Shotgun"){
			if (getWeapon()->getAmmo() >= 2){
				makeBullet(df::Position(target.getX() - 3, target.getY() - 1));
				makeBullet(df::Position(target.getX() + 3, target.getY() + 1));
				int ammo = getWeapon()->getAmmo() - 2;
				getWeapon()->setAmmo(ammo);
				df::EventView ev(getWeapon()->getType(), -2, true);
				df::WorldManager &world_manager = df::WorldManager::getInstance();
				world_manager.onEvent(&ev);
			}
		}
		//play fire sound
		df::Sound *p_sound = df::ResourceManager::getInstance().getSound("fire");
		p_sound->play();
	}

}

//mouse method
void Player::mouse(const df::EventMouse *p_mouse_event){
	//pressed button?
	if ((p_mouse_event->getMouseAction() == df::PRESSED) && (p_mouse_event->getMouseButton() == df::Mouse::LEFT)){
		fire(df::viewToWorld(p_mouse_event->getMousePosition()));
	}
	
	if (p_mouse_event->getMouseAction() == df::MOVED){
		df::Position target = df::viewToWorld(p_mouse_event->getMousePosition());
		df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
		df::LogManager &log_manager = df::LogManager::getInstance();
		df::Sprite *p_temp_sprite;

		if (getPosition().getX()<target.getX() && getPosition().getY() > target.getY()+1){
			p_temp_sprite = resource_manager.getSprite("player-ur"); //facing up and right
			if (!p_temp_sprite){
				log_manager.writeLog("Player::Player(): Warning! Sprite '%s' not found",
					"player-ur");
			}
			else{
				setSprite(p_temp_sprite);
				setTransparency();		//transparent sprite
			}
		}
		else if (getPosition().getX()<target.getX() && (getPosition().getY() - target.getY() == -1 || getPosition().getY() - target.getY() == 0
			|| getPosition().getY() - target.getY() == 1)){
			p_temp_sprite = resource_manager.getSprite("player-r"); //facing right
			if (!p_temp_sprite){
				log_manager.writeLog("Player::Player(): Warning! Sprite '%s' not found",
					"player-r");
			}
			else{
				setSprite(p_temp_sprite);
				setTransparency();		//transparent sprite
			}
		}
		else if (getPosition().getX()<target.getX() && getPosition().getY() < target.getY()-1){
			p_temp_sprite = resource_manager.getSprite("player-dr"); //facing down and right
			if (!p_temp_sprite){
				log_manager.writeLog("Player::Player(): Warning! Sprite '%s' not found",
					"player-dr");
			}
			else{
				setSprite(p_temp_sprite);
				setTransparency();		//transparent sprite
			}
		}
		else if (getPosition().getX()>target.getX() && getPosition().getY() > target.getY()+1){
			p_temp_sprite = resource_manager.getSprite("player-ul"); //facing up and left
			if (!p_temp_sprite){
				log_manager.writeLog("Player::Player(): Warning! Sprite '%s' not found",
					"player-ul");
			}
			else{
				setSprite(p_temp_sprite);
				setTransparency();		//transparent sprite
			}
		}
		else if (getPosition().getX()>target.getX() && getPosition().getY() < target.getY()-1){
			p_temp_sprite = resource_manager.getSprite("player-dl"); //facing down and left
			if (!p_temp_sprite){
				log_manager.writeLog("Player::Player(): Warning! Sprite '%s' not found",
					"player-dl");
			}
			else{
				setSprite(p_temp_sprite);
				setTransparency();		//transparent sprite
			}
		}
		else if (getPosition().getX()>target.getX() && (getPosition().getY() - target.getY() == -1 || getPosition().getY() - target.getY() == 0
			|| getPosition().getY() - target.getY() == 1)){
			p_temp_sprite = resource_manager.getSprite("player-l"); //facing left
			if (!p_temp_sprite){
				log_manager.writeLog("Player::Player(): Warning! Sprite '%s' not found",
					"player-l");
			}
			else{
				setSprite(p_temp_sprite);
				setTransparency();		//transparent sprite
			}
		}
		else if (getPosition().getX() == target.getX() && getPosition().getY() > target.getY()){
			p_temp_sprite = resource_manager.getSprite("player-u"); //facing up
			if (!p_temp_sprite){
				log_manager.writeLog("Player::Player(): Warning! Sprite '%s' not found",
					"player-u");
			}
			else{
				setSprite(p_temp_sprite);
				setTransparency();		//transparent sprite
			}
		}
		else if (getPosition().getX() == target.getX() && getPosition().getY() < target.getY()){
			p_temp_sprite = resource_manager.getSprite("player-d"); //facing down
			if (!p_temp_sprite){
				log_manager.writeLog("Player::Player(): Warning! Sprite '%s' not found",
					"player-d");
			}
			else{
				setSprite(p_temp_sprite);
				setTransparency();		//transparent sprite
			}
		}
		else{
			p_temp_sprite = resource_manager.getSprite("player"); //default player sprite
			if (!p_temp_sprite){
				log_manager.writeLog("Player::Player(): Warning! Sprite '%s' not found",
					"player");
			}
			else{
				setSprite(p_temp_sprite);
				setTransparency();		//transparent sprite
			}
		}
	}
}

//getters and setters for lives and weapon
int Player::getLives() const{
	return lives;
}

void Player::setLives(int new_lives){
	lives = new_lives;
}

Weapon *Player::getWeapon() const{
	return weapon;
}

void Player::setWeapon(Weapon *new_weapon){
	weapon = new_weapon;
}

//hit method, for when player collides with other objects
void Player::hit(const df::EventCollision *p_c){
	//player collides with a powerup
	if (p_c->getObject1()->getType() == "Rifle" || p_c->getObject2()->getType() == "Rifle"){
		//collision with rifle powerup
		weapon = new Weapon(5, weapon->getAmmo()+40, "Space Rifle");
		df::WorldManager &world_manager = df::WorldManager::getInstance();

		//update weapon view object
		df::ObjectList object_list = world_manager.getAllObjects(true);
		df::ObjectListIterator i(&object_list);

		for (i.first(); !i.isDone(); i.next()){
			df::Object *p_o = i.currentObject();
			if (p_o->getType() == "WeaponView"){
				(dynamic_cast <df::ViewObject *> (p_o))->setViewString(weapon->getType());
			}
		}
		df::EventView ev(weapon->getType(), +(weapon->getAmmo()), true);
		world_manager.onEvent(&ev);
	}
	if (p_c->getObject1()->getType() == "Shotgun" || p_c->getObject2()->getType() == "Shotgun"){
		//collision with rifle powerup
		weapon = new Weapon(17, weapon->getAmmo() + 30, "Space Shotgun");
		df::WorldManager &world_manager = df::WorldManager::getInstance();

		//update weapon view object
		df::ObjectList object_list = world_manager.getAllObjects(true);
		df::ObjectListIterator i(&object_list);

		for (i.first(); !i.isDone(); i.next()){
			df::Object *p_o = i.currentObject();
			if (p_o->getType() == "WeaponView"){
				(dynamic_cast <df::ViewObject *> (p_o))->setViewString(weapon->getType());
			}
		}
		df::EventView ev(weapon->getType(), +(weapon->getAmmo()), true);
		world_manager.onEvent(&ev);
	}
	if (p_c->getObject1()->getType() == "HealthPack" || p_c->getObject2()->getType() == "HealthPack"){
		//collision with health powerup
		lives++;
		df::EventView ev("LIVES", +1, true);
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.onEvent(&ev);
	}
	if (p_c->getObject1()->getType() == "Ammo" || p_c->getObject2()->getType() == "Ammo"){
		//collision with ammo powerup, adds ammo to weapon
		weapon->setAmmo(weapon->getAmmo() + 10);
		df::EventView ev(weapon->getType(), +10, true);
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		world_manager.onEvent(&ev);
	}

	//player collides with ghost, remove 1 life
	if (p_c->getObject1()->getType() == "ghost" || p_c->getObject2()->getType() == "ghost"){
		df::WorldManager &world_manager = df::WorldManager::getInstance();
		
		if (p_c->getObject1()->getType() == "ghost"){
			world_manager.markForDelete(p_c->getObject1());
		}
		else{
			world_manager.markForDelete(p_c->getObject2());
		}

		new Ghost(this);

		//take one life away
		lives--;
		if (lives >= 0){
			df::EventView ev("LIVES", -1, true);
			world_manager.onEvent(&ev);
			//play fire sound
			df::Sound *p_sound = df::ResourceManager::getInstance().getSound("fire");
			p_sound->play();
		}

		//if no lives left, kill player and end game
		if (lives == 0){
			world_manager.markForDelete(this);
		}
	}
}