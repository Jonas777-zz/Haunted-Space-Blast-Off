//
// game.cpp
// 

// Engine includes.
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "GraphicsManager.h"

//game includes
#include "Player.h"
#include "Points.h"
#include "ItemDrop.h"
#include "Gamestart.h"

void loadResources();

int main(int argc, char *argv[]) {
  df::LogManager &log_manager = df::LogManager::getInstance();

  // Start up game manager.
  df::GameManager &game_manager = df::GameManager::getInstance();
  if (game_manager.startUp())  {
    log_manager.writeLog("Error starting game manager!");
    game_manager.shutDown();
    return 0;
  }

  df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
  graphics_manager.getWindow()->setTitle("Haunted Space: Blast Off");
  graphics_manager.getWindow()->setSize( sf::Vector2u(1280, 840));
  // Set flush of logfile during development (when done, make false).
  log_manager.setFlush(true);

  // Show splash screen.
  df::splash();

  //load sprites and sounds
  loadResources();

  //add gamestart object
  new GameStart;
  
  //begin game
  game_manager.run();

  // Shut everything down.
  game_manager.shutDown();
}

void loadResources(){
	//load sprites into resourcemanager
	df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
	resource_manager.loadSprite("sprites/player/player-spr.txt", "player");
	resource_manager.loadSprite("sprites/player/player-u.txt", "player-u");
	resource_manager.loadSprite("sprites/player/player-ur.txt", "player-ur");
	resource_manager.loadSprite("sprites/player/player-ul.txt", "player-ul");
	resource_manager.loadSprite("sprites/player/player-l.txt", "player-l");
	resource_manager.loadSprite("sprites/player/player-r.txt", "player-r");
	resource_manager.loadSprite("sprites/player/player-dl.txt", "player-dl");
	resource_manager.loadSprite("sprites/player/player-dr.txt", "player-dr");
	resource_manager.loadSprite("sprites/player/player-d.txt", "player-d");
	resource_manager.loadSprite("sprites/bullet-spr.txt", "bullet");
	resource_manager.loadSprite("sprites/item/rifle-spr.txt", "rifle");
	resource_manager.loadSprite("sprites/item/shotgun-spr.txt", "shotgun");
	resource_manager.loadSprite("sprites/item/healthpack-spr.txt", "healthpack");
	resource_manager.loadSprite("sprites/item/ammo-spr.txt", "ammo");
	resource_manager.loadSprite("sprites/gamestart.txt", "gamestart");
	resource_manager.loadSprite("sprites/megaghost.txt", "megaghost");
	resource_manager.loadSprite("sprites/ghost.txt", "ghost");
	resource_manager.loadSprite("sprites/ruins1.txt", "ruins1");
	resource_manager.loadSprite("sprites/ruins2.txt", "ruins2");
	resource_manager.loadSprite("sprites/ruins3.txt", "ruins3");
	resource_manager.loadSprite("sprites/gameover.txt", "gameover");
	//load sounds into resourcemanager
	resource_manager.loadSound("sounds/fire.wav", "fire");
	resource_manager.loadSound("sounds/ghost1.wav", "ghost1");
	resource_manager.loadSound("sounds/ghost2.wav", "ghost2");
	resource_manager.loadSound("sounds/game-over.wav", "game over");
	resource_manager.loadMusic("sounds/dracula.ogg", "game music");
	resource_manager.loadMusic("sounds/ghosts.ogg", "start music");
}