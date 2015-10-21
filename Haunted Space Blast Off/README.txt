README
******************************************************************************
*A duplicate of this README is located in the Haunted Space Blast Off folder.*
******************************************************************************
Jacob Link - jslink@wpi.edu
Jonas McGowan-Martin - jmcgowanmartin@wpi.edu

Haunted Space: Blast Off
A game by EA Sports.

---- ENGINE ----
This game runs using Professor Claypool's precompiled Dragonfly Engine. The folder
containing the engine should be in the same directory as the folder containing the 
game files (eg Visual Studio 2013/Projects).

---- COMPILATION ----
All header and source code files needed for compilation are included in this 
directory. This project was made and compiled in Visual Studio 2013.

A precompiled standalone executable is also included with this submission.
It is located in the vs-2013/Release folder, and called "game.exe".
The Release folder can be put on any computer and the standalone run from game.exe
within it to run the game.

---- CONTROLS / MECHANICS ----
W - Up
A - Left
S - Down
D - Right

Any Key, but not Q - Advance from title screen to play screen
Q - Quit, first back to title screen, then out of game.

Mouse to aim, and left click (can be held down) to shoot.
Walk over powerups to pick them up.

The player starts with 3 lives. You lose a life if you touch a ghost.

If you run out of ammo, you switch to the Space Revolver, a slower weapon that
does not consume ammunition. You can still stockpile ammo while you have the 
Space Revolver equipped, and use this ammo when you get a different weapon.

Bullets can pass through powerups, but not ruins. 
Ghosts can pass through ruins, since they're ghosts, but the player cannot.

---- POWERUPS ----
-==TO-  : Space Rifle
     	   A faster, more dangerous weapon, from a less civilized age.
==-[]=< : Space Shotgun
           A slower weapon that shoots a cone of 3 bullets every time it ever fires.
  [+]   : Healthpack
   	   Adds 1 life to your overall total.
 """"   : Ammo pack
	   Bullets for your cool space weapons. Not consumed by Space Revolvers.

---- CHARACTERS ----
 (H)    : The Player character. Note his heroic H.
 <(OO)> : A spooky space ghost. Such menacing eyes.
   \ /   
\_(0_0)_/ : An angry mega-ghost. Watch out, he's fast and mean!

---- BACKGROUND ----
  ^  : A space plant, native to this planet.
  ~  : Another space plant, but an invasive pest, like space crabgrass.
|\_.   
|  \_ .
| [] \ 
|_____|

  .     _  .
    ___/ |_ 
 . /      _|
 _/ []   |_ 
/         _|
|______  |_ 
     . \___|

 ___ 
|___|
		: Ruins left over from a forgotten civilization, overrun by
		  the space ghosts.

---- NOTES ----
- This game runs at a 1600 x 900 resolution instead of the default Dragonfly window size.
- Music credit: Purchased from portopak.bandcamp.com
		Title screen music: Troubled Ghost - Portopak
                Game music: Doubting Dracula - Portopak
- It is possible, but very unlikely, that the player may start in a ruin. If this happens, 
you'll probably have a pretty short session, but when you respawn, the ruins will be randomly 
generated again, and you should be able to play normally.