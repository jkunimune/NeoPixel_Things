/*
  FireballSurvival.pde
  by Justin Kunimune
 
 Example file using the The Meggy Jr Simplified Library (MJSL)
  from the Meggy Jr RGB library for Arduino
   
   
 Oh no!  You're stuck in a dungeon full of fireballs and lasers!  You need to
 survive long enough to escape.  Use coins and bombs to advance through the
 levels.  Beware, though, the deeper you go, the more perilous the castle
 becomes.
 
 
 Version 1.25 - 12/2/2008
 Copyright (c) 2008 Windell H. Oskay.  All right reserved.
 http://www.evilmadscientist.com/
 
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 you should have received a copy of the GNU General Public License
 along with this library.  If not, see <http://www.gnu.org/licenses/>.
 	  
 */

 
 
 
 

/*
 * Adapted from "Blink,"  The basic Arduino example.  
 * http://www.arduino.cc/en/Tutorial/Blink
 */

#include <MeggyJrSimple.h> 


int px;              // declaration of variables for player position,
int py;              // time, level, and LED values
int timer;
int difficulty;
int timeRemaining;
int LED;
int pcolor;
boolean moved;      // moved is used for smoothing the gap between holding and pressing
                    // the d-pad
                    
void setup()              
{
  MeggyJrSimpleSetup();
  px=3;                  // initialization of variables and custom colors
  py=3;
  pcolor=Green;
  difficulty=0;
  timer=3;
  timeRemaining=599;
  LED=0;
  moved=0;
  EditColor(Red,15,0,0);           // To keep track of fireballs going in
  EditColor(CustomColor2,11,0,0);  // different directions and lasers in
  EditColor(CustomColor3,11,0,0);  // different stages, CustomColors 2-7 are
  EditColor(CustomColor4,11,0,0);  // crimson, scarlet, maroon, reddish-yellow, 
  EditColor(Orange,15,1,0);        // tangerine, and eggshell.
  EditColor(CustomColor5,15,1,0);
  EditColor(CustomColor6,15,1,0);
  EditColor(White,20,20,5);
  EditColor(CustomColor7,20,20,5);
  EditColor(DimRed,1,0,0);
  reset();
}                


void loop()                 
{
  for(int c=0;c<10;c++)      // I must check for buttons being pressed frequently
  {
    DrawPx(px,py,Dark);
    goPress();  
    DrawPx(px,py,pcolor);      // I always redraw the player before displaying slate
    DisplaySlate();    
    delay(10); 
  }
     
  CheckButtonsDown(); 
  
  while(Button_B)           // Button_B pauses the game
  {                       
    CheckButtonsDown();
  }
  
  if(!moved)        // After pressing a movement button, you must wait at least a tenth of
    goPlayer();     // a second for the game to realize you are holding the button down.
  moved=false;
                    
  pushFireball();      // handles basic enemy mechanics
  generateCoin();      // produces gold coins
  generateBomb();      // produces bomb powerups
  generateFreeze();    // produces freeze powerups
  generateShield();    // produces shielf powerups
  countDown();         // handles levelling up
}


void goPlayer()            // handles basic movement
{
  DrawPx(px,py,Dark);
  if(Button_Up)
    goUp();
  if(Button_Down)
    goDown();
  if(Button_Left)
    goLeft();
  if(Button_Right)
    goRight();
}


void goPress()          // also handles basic movement (player can hold or press buttons
{                       // based on preference)
  CheckButtonsPress();
  
  if(Button_Up)
  {
    goUp();
    moved=true;
  }
  
  if(Button_Down)
  {
    goDown();
    moved=true;
  }
  
  if(Button_Left)
  {
    goLeft();
    moved=true;
  }
  
  if(Button_Right)
  {
    goRight();
    moved=true;
  }
}


void reset()        // resets variables and clears slate
{
  px=3;
  py=3;
  pcolor=Green;
  timer=3;
  timeRemaining=599;
  moved=0;
  ClearSlate();
  DisplaySlate();
  LED=1;
  for(int n=0;n<difficulty;n++)
    LED=LED*2+1;
  SetAuxLEDs(LED);        // displays the level each time you restart
  delay(300);
  SetAuxLEDs(0);
  delay(200);
  SetAuxLEDs(LED);
  delay(300);
  SetAuxLEDs(0);
  delay(200);
  SetAuxLEDs(LED);
  delay(300);
  SetAuxLEDs(0);
  LED=0;
}


void goUp()        // moves the player up
{
  py++;
  wrapAround();
  checkCollision();
}


void goDown()        // moves the player down
{
  py--;
  wrapAround();
  checkCollision();
}


void goLeft()        // moves the player left
{
  px--;
  wrapAround();
  checkCollision();
}


void goRight()        // moves the player right
{
  px++;
  wrapAround();
  checkCollision();
}


void checkCollision()        // does something based on what kind of space
{                            // player has landed on
  switch(ReadPx(px,py))
  {                            
    case 1:        // red, crimson, scarlett, and maroon are deadly fireballs
      gameOver();
      break;
    case 2:        // orange, reddish-yellow, and tangerine are lasers
      gameOver();
    case 3:
      timeRemaining=timeRemaining-75;  // yellow is coins, which increase score
      LED=LED*2+1;
      Tone_Start(ToneG5,100);
      delay(100);
      Tone_Start(ToneC6,100);
      break;
    case 5:        // blue is bombs
      explode();
      break;
    case 6:          // purple is freeze bombs
      freeze();      
      break;
    case 18:
      gameOver();
      break;
    case 19:
      gameOver();
      break;
    case 20:
      gameOver();
      break;
    case 21:
      gameOver();
      break;
    case 22:
      gameOver();
      break;
    case 23:
      shield();        // eggshell is shields
  }
}


void moveRight(int x, int y)        // These are commands to move fireballs
{
  if (x < 7)
    DrawPx(x+1,y,ReadPx(x,y));
  DrawPx(x,y,DimRed);
}


void moveLeft(int x, int y)
{
  if (x > 0)
    DrawPx(x-1,y,ReadPx(x,y));
  DrawPx(x,y,DimRed);
}


void moveUp(int x, int y)
{
  if (y < 7)
    DrawPx(x,y+1,ReadPx(x,y));
  DrawPx(x,y,DimRed);
}


void moveDown(int x, int y)
{
  if (y > 0)
    DrawPx(x,y-1,ReadPx(x,y));
  DrawPx(x,y,DimRed);
}


void pushFireball()        // This huge method advances fireballs in their
{                          // correct directions based on color as well as
  switch(timer)            // charges/fires lasers.
  { 
    case 0:                    // each of these checks every tile to see if it
      for(int j=0;j<8;j++)     // needs to update
        for(int k=0;k<8;k++)   // I need separate loops because they all need
          if(ReadPx(j,k)==8)   // to load in a certain order to prevent glitches
            DrawPx(j,k,Dark);         // erases fireball trail
      
      for(int j=0;j<8;j++)         // moves fireballs along trajectory
        for(int k=0;k<8;k++)
          if(ReadPx(j,k)==1)  
            moveLeft(j,k);
            
      for(int j=7;j>-1;j--)     
        for(int k=0;k<8;k++)
          if(ReadPx(j,k)==18)
            moveRight(j,k);
            
      for(int j=0;j<8;j++)
        for(int k=0;k<8;k++)
          if(ReadPx(j,k)==19)
            moveDown(j,k);
            
      for(int j=0;j<8;j++)
        for(int k=7;k>-1;k--)
          if(ReadPx(j,k)==20)
            moveUp(j,k);
            
      generateFireball();        // produces new fireballs randomly
      
      if(random(10)==1)        // makes a fireball go towards the player every
        target();              // ten turns or so (so you can't sit in one spot)
                 
      for(int j=0;j<8;j++)        // deletes lasers after use
        for(int k=0;k<8;k++)
          if(ReadPx(j,k)==22)
            DrawPx(j,k,Dark);
                 
      for(int j=0;j<8;j++)        // laser-fire animation
        if(ReadPx(j,7)==21)
        {
          DrawPx(px,py,pcolor);
          for(int k=7;k>-1;k--)
          {
            Tone_Start(ToneB4,15);
            DrawPx(j,k,22);
            DisplaySlate();
            delay(9);
          }
        }
        
      for(int j=0;j<8;j++)        // charges lasers for one frame
        for(int k=0;k<8;k++)
          if(ReadPx(j,k)==Orange)
            DrawPx(j,k,21);
            
      generateLaser();        // produces lasers every once-in-a-while
      
      timer=7-difficulty;        // speeds up game based on difficulty
      
      Tone_Start(700000,50);        // creates a "tempo" for the game
      break;
      
    default:        // this is about 7/8 of the time.  If it is between frames,
      timer--;      // do nothing but count down
  }
  
  checkCollision();
  // this lets fireballs running into you cause death--not just you
  // running into fireballs
}


void generateFireball()
{
  for(int l=1;l<7;l++)        // has a  chance to create fireballs every turn
    if(random(120-difficulty)<2 && ReadPx(7,l)==Dark)   
      DrawPx(7,l,Red);

  for(int l=1;l<7;l++)
    if(random(120-difficulty)<2 && ReadPx(0,l)==Dark)
      DrawPx(0,l,CustomColor2);
      
  for(int l=1;l<7;l++)
    if(random(120-difficulty)<2 && ReadPx(l,7)==Dark)
      DrawPx(l,7,CustomColor3);
      
  for(int l=1;l<7;l++)
    if(random(120-difficulty)<2 && ReadPx(l,0)==Dark)
      DrawPx(l,0,CustomColor4);
}


void generateCoin()        // has a slight chance to spawn a coin on each tile
{                          // coins do no become more or less frequent as
  for(int j=0;j<8;j++)     // difficulty increases
    for(int k=0;k<8;k++)
      if(random(7000)==1)
        DrawPx(j,k,Yellow);
}


void generateLaser()        // difficulty*6 makes lasers increase faster than
{                           // fireballs later in the game
  for(int l=0;l<8;l++)
    if(random(275-difficulty*6)==1)
      DrawPx(l,7,Orange);
}


void generateBomb()        // bombs are rarer than coins
{
  for(int j=0;j<8;j++)
    for(int k=0;k<8;k++)
      if(random(25000)==1)
        DrawPx(j,k,Blue);
}


void generateFreeze()        // freezes and shields generate just like bombs
{
  for(int j=0;j<8;j++)
    for(int k=0;k<8;k++)
      if(random(25000)==1)
        DrawPx(j,k,Violet);
}


void generateShield()
{
  for(int j=0;j<8;j++)
    for(int k=0;k<8;k++)
      if(random(25000)==1)
        DrawPx(j,k,CustomColor7);
}
        

void explode()            // bombs clear the board with a brief animation
{
  for(int j=3;j<5;j++)
    for(int k=3;k<5;k++)
      DrawPx(j,k,FullOn);
  DisplaySlate();
  Tone_Start(ToneA5,85);
  delay(40);
  
  for(int j=2;j<6;j++)
    for(int k=2;k<6;k++)
      DrawPx(j,k,FullOn);
  DisplaySlate();
  delay(40);
  
  for(int j=1;j<7;j++)
    for(int k=1;k<7;k++)
      DrawPx(j,k,FullOn);
  DisplaySlate();
  Tone_Start(ToneE5,85);
  delay(40);
  
  for(int j=0;j<8;j++)
    for(int k=0;k<8;k++)
      DrawPx(j,k,FullOn);
  DisplaySlate();
  delay(40);
  
  ClearSlate();
  DrawPx(px,py,pcolor);
  DisplaySlate;
  Tone_Start(ToneB4,80);
}


void freeze()        // freezes temporarily pauses all enemies
{
  timer=25;
  DisplaySlate();
  Tone_Start(ToneE6,75);
  delay(100);
  Tone_Start(ToneE6,75);
  delay(100);
  Tone_Start(ToneE6,100);
}


void shield()          // shields allow you to survive one hit
{
  pcolor=White;
  DrawPx(px,py,pcolor);
  Tone_Start(ToneE4,200);
  delay(100);
  Tone_Start(ToneA4,200);
  delay(100);
  Tone_Start(ToneE5,200);
}


void wrapAround()      // just a short method to let player wrap around screen
{
  if(px>7)
    px=px-8;
  if(px<0)
    px=px+8;
  if(py>7)
    py=py-8;
  if(py<0)
    py=py+8;
}


void countDown()          // allows player to progress every sixty seconds, or
{                         // after collecting enough coins
  if(timeRemaining%75==0)
    LED=LED*2+1;
    
  SetAuxLEDs(LED);        // LEDs show how much longer you have to go
  
  if(timeRemaining<=0)
    victory();
    
  timeRemaining--;
}


void target()        // shoots a fireball directly at the player
{
  switch(random(4))
  {
    case 0:
      DrawPx(7,py,Red);
      break;
    case 1:
      DrawPx(0,py,CustomColor2);
      break;
    case 2:
      DrawPx(px,7,CustomColor3);
      break;
    case 3:
      DrawPx(px,0,CustomColor4);
  }
}


void victory()            // This is the green flash and fanfare you get when
{                         // you beat a level.
  delay(500);
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      DrawPx(i,j,Green);
  DisplaySlate();
  Tone_Start(ToneC5,140);
  delay(140);
  Tone_Start(ToneE5,140);
  delay(140);
  Tone_Start(ToneC6,150);
  delay(150);
  difficulty++;        // The game gets more difficult the longer you play
  reset();
}


void gameOver()            // This is the red flash and buzzing you get when
{                          // you die.
  if(pcolor==Green)
  {
    DisplaySlate();
    delay(200);
    for(int i=0;i<8;i++)
      for(int j=0;j<8;j++)
        DrawPx(i,j,Red);
    DisplaySlate();
    Tone_Start(ToneB2,400);
    delay(500);
    Tone_Start(ToneB2,400);
    delay(500);
    Tone_Start(ToneB2,500);
    delay(500);
    reset();
  }
  else          // checks if the player was wearing a shield
  {
    DrawPx(px,py,Green);
    pcolor=Green;
    Tone_Start(ToneE5,200);
    delay(100);
    Tone_Start(ToneA4,200);
    delay(100);
    Tone_Start(ToneE4,100);
  }
}


void DrawFBS()
{
  DrawPx(0, 2, 1);
}
