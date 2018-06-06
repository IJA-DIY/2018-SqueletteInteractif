
// compiler error handling
#include "Compiler_Errors.h"

// touch includes
#include <MPR121.h>
#include <Wire.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4

// mp3 includes
#include <SPI.h>
#include <SdFat.h>
#include <NewPing.h>
#include <FreeStack.h> 
#include <SFEMP3Shield.h>

// mp3 variables
SFEMP3Shield MP3player;
byte result;
int lastPlayed = 0;

// mp3 behaviour defines
#define REPLAY_MODE TRUE  // By default, touching an electrode repeatedly will 
                          // play the track again from the start each time.
                          //
                          // If you set this to FALSE, repeatedly touching an 
                          // electrode will stop the track if it is already 
                          // playing, or play it from the start if it is not.

// touch behaviour definitions
#define firstPin 0
#define lastPin 11

// sd card instantiation
SdFat sd;


#define firstPin 0
#define lastPin 11
#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define SOUNDCAPTOR_PIN 8
#define DISTANCE_MAX 100

#define shoulder_zone 1
int Anxiety = 0;
unsigned long int timer = 40;
NewPing sonar(TRIGGER_PIN,ECHO_PIN,DISTANCE_MAX);

void setup(){  
  Serial.begin(57600);
  
  pinMode(LED_BUILTIN, OUTPUT);
   
  //while (!Serial) ; {} //uncomment when using the serial monitor 
  Serial.println("Bare Conductive Touch MP3 player");

  if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();

  if(!MPR121.begin(MPR121_ADDR)) Serial.println("error setting up MPR121");
  MPR121.setInterruptPin(MPR121_INT);

  MPR121.setTouchThreshold(20);
  MPR121.setReleaseThreshold(10);

  result = MP3player.begin();
  MP3player.setVolume(10,10);
 
  if(result != 0) {
    Serial.print("Error code: ");
    Serial.print(result);
    Serial.println(" when trying to start MP3 player");
   }
   
}
int rand_a_b(int a,  int b)
{
  return rand()%(b-a) + a;
}

void Quiet(int distance)
{
  
  int rng = rand_a_b(4,9);
  if (distance <= 40 && distance != 0 && (millis()/1000)- timer > 40 )
  { 
     if(MP3player.isPlaying() && lastPlayed ==1)
    {
      Serial.println("je joue");
      MP3player.stopTrack();
      MP3player.playTrack(rng);
    }
    else
    {
      MP3player.playTrack(rng);
    }
    timer = millis()/1000;
    lastPlayed = rng;
  }
  else
  {
    if(!MP3player.isPlaying())
    {
      MP3player.playTrack(1);
      lastPlayed = 1;
    }
  }
}


void Anxious()
{
  if(MP3player.isPlaying() && lastPlayed != 2)
    {
      MP3player.stopTrack();
      MP3player.playTrack(2);
      lastPlayed = 2;
    }
  else
  {
    MP3player.playTrack(2);
    lastPlayed = 2;
  }
}

void veryAnxious()
{
  if(MP3player.isPlaying() && lastPlayed != 3)
    {
      MP3player.stopTrack();
      MP3player.playTrack(3);
      lastPlayed = 3;
    }
  else
  {
    MP3player.playTrack(3);
    lastPlayed = 3;
  }
}
void gestion_son()
{
  int  noise = analogRead(A0);
  if(noise >= 530)
  {
    Anxiety += 50;
    delay(500);
  }
}




bool isShoulderTouched(){
  if(MPR121.touchStatusChanged()){
    MPR121.updateTouchData();
    if (MPR121.isNewTouch(shoulder_zone)) return true;
    return false;
  }
}
void loop(){
 int distance = sonar.ping_cm();
 if(Anxiety < 50) Quiet(distance);
 if(Anxiety >= 50 && Anxiety <100) Anxious();
 if(Anxiety >= 100) veryAnxious();
 if(isShoulderTouched()) Anxiety = 0;
 gestion_son();
}



