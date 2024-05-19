//IGT Development production by AlexSNAKE
//Version-1.0a - 19.05.2024

//Including necessary libraries
#include "SoftwareSerial.h" //Library version 3.0.0
#include "DFRobotDFPlayerMini.h" //Library version 1.0.6

//PIN configuration parameters
const byte greenLedPin = 2; //PIN of the green LED
const byte redLedPin = 3; //PIN of the red LED
const byte buttonPin = 4; //PIN of the button
const byte gearconPin = 5; //PIN of the gearcon
const byte speakerTxPin = 10; //TX PIN for DFPlayer
const byte speakerRxPin = 11; //RX PIN for DFPlayer
SoftwareSerial softwareSerial(speakerTxPin, speakerRxPin); // Assigning pins for communication with TX and RX

bool lethalMode; //Variable for switching mode (Lethal/Containment) - (True/False)
bool collarReady; //Variable for collar reload (Charged/Discharged) - (True/False)
unsigned int time; //Variable storing the current time from the start of the timer countdown (0 .. 65535)
byte track; //Variable storing the name of the requested audio file (0 .. 255)
byte carma; //Variable storing the time for early collar opening (0 .. 255)
DFRobotDFPlayerMini p; //Creating an object of the class for interaction with the player.

void setup() { //System initialization function
	softwareSerial.begin(9600); //Connection to DFplayer via serial port
  if (!p.begin(softwareSerial)) {while(true);} //Connection to DFplayer
  p.volume(30); //Change the speaker volume

  pinMode(greenLedPin, OUTPUT); //Setting the mode of operation for the PIN - green LED
  pinMode(redLedPin, OUTPUT); //Setting the mode of operation for the PIN - red LED
  pinMode(buttonPin, INPUT_PULLUP); //Setting the mode of operation for the PIN - button
  pinMode(gearconPin, INPUT_PULLUP); //Setting the mode of operation for the PIN - gearcon
}

void loop() { //Main program loop, starts when power is supplied
  if (!collarOpenedState() && collarReady) { //If the collar is closed and reloaded then
    collarActivation(lethalMode); //Activate the collar with the selected mode
  }
  
  if (collarOpenedState()) {//If the collar is open then
    collarReady = true; //Collar reload
    selectedModeIndication(); //LED flashing for selected mode indication
    changeCollarMode(); //Change the collar mode when the button is pressed
  }
}

//Function to implement the main functionality of the collar
void collarActivation(bool monitoringMode) { //The function takes the selected collar mode
  track = monitoringMode ? 2 : 1; //Depending on the mode, set the default track name
  redLedOn();//Turn on the red LED when the collar is closed
  voice(7);//Play a notification about the activation of the collar
  do { //Repeat until while is not equal to false
    delay(1000);//Delay the cycle for 1 second (1000 milliseconds)
    time += 1; //Count the seconds elapsed since the collar was closed
    shouldCollarDetonate(); //Check the collar for early opening
    goodBoy(); //Check the collar for button hold (release)
    checkTimeLeft(); //Check the remaining time and play audio about the remaining time
  } while (time <= 36000);//While the elapsed time is less than or equal to 360000 seconds (1 hour) do {}
  voice(track+4); //At the end of the time or premature event, play the instruction (audio)
  collarReady = false; //Requirement for collar reload (requirement to open the collar for reactivation)
  time = 0; //Reset the elapsed time from the start of the timer
  carma = 0; //Reset the time for early release
}

//Function to check for early collar opening
void shouldCollarDetonate() {
  if (collarOpenedState()) { //If the collar is open then
    track = 1; //Set the audio name - 1 - Explosion
    voice(track); //Play audio - 1 - Explosion
    time = 65535; //Set the remaining time variable to the maximum, to exit the do {} loop
  }
}

//Function for early release of the prisoner by holding the button for 5 seconds
void goodBoy() {
  if (buttonPressedState()) { //If the button is pressed, then
    carma += 1; //Add 1 to the early release variable
    if (carma == 5) { //If the button has been held for 5 seconds, then
      track = 2; //Set the audio name - 2 - Release
      voice(track); //Play audio - 2 - Release
      time = 65535; //Set the remaining time variable to the maximum, to exit the do {} loop
    }
  } else { //If the button is released
    carma = 0; //Reset the time for early release
  }
}

//Function to check the remaining time until the end of the timer
void checkTimeLeft() {
      if (time == 600) { //If 600sec has passed. (10min.), then
      voice(50); //Play audio about the 50-minute countdown.
    } else if (time == 1200) { //If 1200sec has passed. (20min.), then
      voice(40); //Play audio about the 40-minute countdown.
    } else if (time == 1800) { //If 1800sec has passed. (30min.), then
      voice(30); //Play audio about the 30-minute countdown.
    } else if (time == 2400) { //If 2400sec has passed. (40min.), then
      voice(20); //Play audio about the 20-minute countdown.
    } else if (time == 3000) { //If 3000sec has passed. (50min.), then
      voice(10); //Play audio about the 10-minute countdown.
    } else if (time == 3600) { //If 3600sec has passed. (60min.), then
      voice(track); //Play audio about release or detonation, depending on the mode
    }
}

//Function to display the selected collar mode with LEDs
void selectedModeIndication() {
  delay(250); //Delay 250ms.
  (lethalMode) ? redLedOn() : greenLedOn(); //If Lethal mode then turn on the red or green LED
  delay(250); //Delay 250ms.
  redLedOff(); //Turn off the red LED
  greenLedOff(); //Turn off the green LED
}

//Function to switch the collar mode and voice notification of the selected mode
void changeCollarMode() {
      if (buttonPressedState()) { // If the button is pressed
      lethalMode = !lethalMode; //Change the collar mode to the opposite of the current one
      (lethalMode) ? voice(3) : voice(4); //Play a notification about the selected mode
    }
}

//Function to turn on the green LED
void greenLedOn() {
  digitalWrite(greenLedPin, HIGH); //Apply a signal to the PIN of the green LED
}

//Function to turn off the green LED
void greenLedOff() {
  digitalWrite(greenLedPin, LOW); //Remove the signal from the PIN of the green LED
}

//Function to turn on the red LED
void redLedOn() {
  digitalWrite(redLedPin, HIGH); //Apply a signal to the PIN of the red LED
}

//Function to turn off the red LED
void redLedOff() {
  digitalWrite(redLedPin, LOW); //Remove the signal from the PIN of the red LED
}

//Function to transmit the button state (Pressed/Released)
bool buttonPressedState() {
  return(digitalRead(buttonPin) == LOW); //Return true if the button is pressed otherwise false
}

//Function to transmit the gearcon state (Connected/Disconnected)
bool collarOpenedState() {
  return(digitalRead(gearconPin) == HIGH);//Return true if the collar is open otherwise false
}


//The voice function plays the required audio file by taking its name as an input parameter
void voice(byte track) { //Takes a parameter of type int
    p.playMp3Folder(track); //Play the audio file by name
    delay(3000); //Delay for correct audio playback
}

/*
Names - corresponding to the names of audio files stored on the MicroSD card:
1 - Notification of collar detonation;
2 - Notification of collar lock opening;
3 - Notification of lethal mode activation;
4 - Notification of prisoner mode activation;
5 - Instruction with further actions in case of detonation;
6 - Instruction with further actions in case of opening;
7 - Notification of collar activation;
10 - Notification of the end of the timer in 10 minutes;
20 - Notification of the end of the timer in 20 minutes;
30 - Notification of the end of the timer in 30 minutes;
40 - Notification of the end of the timer in 40 minutes;
50 - Notification of the end of the timer in 50 minutes.
*/
