//
// Ghost.cpp
//author jmcgownmartin

// System includes.
#include <stdlib.h> // for rand()

// Engine includes.
#include "EventCollision.h"
#include "EventOut.h"
#include "EventStep.h"
#include "EventView.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "LogManager.h"

// Game includes.
#include "Points.h"
#include "Ghost.h"
#include "ItemDrop.h"

// Define registerInterest in case engine does not.
static void registerInterest(std::string s) {};

Ghost::Ghost(Player * hero) {
	df::LogManager &log_manager = df::LogManager::getInstance();
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();

	if ((rand() % 5) == 3){
		mega = true;
	}
	else{
		mega = false;
	}

	// Setup "ghost" sprite.
	if (mega){
		df::Sprite *p_temp_sprite = resource_manager.getSprite("megaghost");
		if (!p_temp_sprite) {
			log_manager.writeLog("Ghost::Ghost(): Warning! Sprite '%s' not found",
				"megaghost");
		}
		else {
			setSprite(p_temp_sprite);
			setSpriteSlowdown(5);
		}
	}
	else{
		df::Sprite *p_temp_sprite = resource_manager.getSprite("ghost");
		if (!p_temp_sprite) {
			log_manager.writeLog("Ghost::Ghost(): Warning! Sprite '%s' not found",
				"ghost");
		}
		else {
			setSprite(p_temp_sprite);
			setSpriteSlowdown(5);
		}
	}

	// Set object type.
	setType("ghost");
	setSolidness(df::SOFT);
	target = hero;
	//spawns from a random side of screen
	spawnSide = rand() % 4;

	// Move ghost to start location.
	moveToStart();

	// Register interest in step event.
	registerInterest(df::STEP_EVENT);
}

Ghost::~Ghost() {
	// Send "view" event with points to interested ViewObjects.
	// Add 10 points.
	df::EventView ev(POINTS_STRING, 10, true);
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	world_manager.onEvent(&ev);
}

// Handle event.
// Return 0 if ignored, else 1.
int Ghost::eventHandler(const df::Event *p_e) {
	//df::LogManager &lm = df::LogManager::getInstance();

	if (p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision *p_collision_event = static_cast <df::EventCollision const *> (p_e);
		hit(p_collision_event);
		return 1;
	}

	if (p_e->getType() == df::STEP_EVENT){
		//lm.writeLog("ghost step");
		updatePath();
		return 1;
	}

	// If get here, have ignored this event.
	return 0;
}

//update the velocity to follow the player
void Ghost::updatePath(){
	int targetX = target->getPosition().getX();
	int targetY = target->getPosition().getY();

	int currentX = getPosition().getX();
	int currentY = getPosition().getY();

	if (targetX > currentX){
		setXVelocity(1);
	}
	else if (targetX < currentX){
		setXVelocity(-1);
	}
	else{
		setXVelocity(0);
	}

	if (targetY > currentY){
		setYVelocity(1);
	}
	else if (targetY < currentY){
		setYVelocity(-1);
	}
	else{
		setYVelocity(0);
	}

	if (!mega){
		setXVelocity(getXVelocity() / 2);
		setYVelocity(getYVelocity() / 8);
	}
	else{
		setXVelocity(getXVelocity() / 2);
		setYVelocity(getYVelocity() / 4);
	}
}

// Called with ghost collides.
void Ghost::hit(const df::EventCollision *p_collision_event) {

	// If ghost on ghost, ignore.
	if ((p_collision_event->getObject1()->getType() == "ghost") &&
		(p_collision_event->getObject2()->getType() == "ghost"))
		return;

	// If Bullet, create explosion and make new ghost.
	if ((p_collision_event->getObject1()->getType() == "Bullet") ||
		(p_collision_event->getObject2()->getType() == "Bullet")) {

		// Play "explode" sound
		if (!mega){
			df::Sound *p_sound = df::ResourceManager::getInstance().getSound("ghost1");
			p_sound->play();
		}
		else{
			df::Sound *p_sound = df::ResourceManager::getInstance().getSound("ghost2");
			p_sound->play();
		}

		//chance for item drop
		int itemchance = rand() % 6;
		if (itemchance == 0){
			ItemDrop *pi = new ItemDrop;
			pi->setPosition(getPosition());
		}

		// ghosts appear stay around perpetually.
		new Ghost(target);
	}

}

// Move ghost to starting location on correct side of window.
void Ghost::moveToStart() {
	df::WorldManager &world_manager = df::WorldManager::getInstance();
	df::Position temp_pos;

	// Get world boundaries.
	int world_horiz = world_manager.getBoundary().getHorizontal();
	int world_vert = world_manager.getBoundary().getVertical();

	switch (spawnSide){
	case 0:
		// x is in range.
		temp_pos.setX(rand() % world_horiz);

		// y is off the top side of the window.
		temp_pos.setY(-1 * (rand() % 15));
		break;
	case 1:
		// x is off right side of window.
		temp_pos.setX(world_horiz + (rand() % 15));

		// y is in vertical range.
		temp_pos.setY(rand() % world_vert);
		break;
	case 2:
		// x is in range.
		temp_pos.setX(rand() % world_horiz);

		// y is off bottom side of window.
		temp_pos.setY(world_vert + (rand() % 15));
		break;
	case 3:
		// x is off left side of window.
		temp_pos.setX(-1 * (rand() % 15));

		// y is in vertical range.
		temp_pos.setY(rand() % world_vert);
		break;
	}

	//sets velocity to be towards player
	updatePath();

	// If collision, move right slightly until empty space.
	df::ObjectList collision_list = world_manager.isCollision(this, temp_pos);
	while (!collision_list.isEmpty()) {
		temp_pos.setX(temp_pos.getX() + 1);
		collision_list = world_manager.isCollision(this, temp_pos);
	}

	world_manager.moveObject(this, temp_pos);
}