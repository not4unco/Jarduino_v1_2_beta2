//************************************************************************************/
// Jarduino Aquarium Controller v.1.2(beta) - release date: May 2012
//   Written and Debugged by Jamie M. Jardin aka "TheDOdblG"
//   Copyright 2011, 2012 Jamie M. Jardin
//   email:  jjardi56@msn.com
// 
// Aquarium and Controller Build Details
//   http://ukreefs.com/index.php?topic=117.0
//   http://www.reefcentral.co.uk/showthread.php/26598-44-Gallon-Pentagon-Corner-Tank-LED-Hood-DIY-Build-EXTRAS!-(Sump-amp-Wave-Maker-etc)
//
// Jarduino Code Download Page
//   http://code.google.com/p/jarduino-aquarium-controller/
//
// Jarduino Aquarium Controller v.1.2 Updates Demonstration Video
//   Coming soon to a YouTube near you!
//
// Jarduino Aquarium Controller v.1.1 Demonstration Video
//   http://www.youtube.com/watch?v=gN4XQHNijxw
//
//************************************************************************************/
//
// Previous Versions 
//   Jarduino Aquarium Controller v.1.0 (December 2011)
//   Jarduino Aquarium Controller v.1.1 (April 2012)
//
// Updates to Current Version (Jarduino Aquarium Controller v.1.2)
//   • Added support for Arduino 1.0 IDE (Must use patched version)
//   • Replaced the ITDB02_Graph16.h Library with UTFT.h Library by Henning Karlsen
//   • Replaced Matt Joyce's DS1307 Library with Henning Karlsen's DS1307 Library
//       - modified formatting in Karlsen's DS1307 Library
//       - rewrote all RTC coding in sketch, based on Karlsen's "ITDB02_Analog_Clock"
//       - added "Day of the Week" to the "Date & Time Bar."
//       - fixed setting a date that does not exist possibility (ie. FEB 31, 2011)
//   • Added 24 Hour Time formatting to the "Test LED Array Output Settings" screen
//   • Modified "View/Change Moon LED Max Output"
//       - Added pictures and the ability to set/save the Minimum Illumination
//   • Replaced LED Output Testing "Widget" with "Slider Bars" 
//   • Upgraded "Change LED Output Values" page with "Slider Bars" 
//   • Included more Built-In Protections and Redundancies
//   • Added an additional page of user settings including:
//       - choice of showing the Day of the Week
//       - setting the Fan(s) startup temperature
//       - choosing to Dim the LEDs at user defined temperature & How much to Dim them
//       - added four user-defined options to the Screensaver
//   • Changed the look of some buttons as well as some other visual tweaks
//
// Coming soon to Jarduino v.1.3
//   • WiFi and possibly Android/iPhone App
//
// Other Possible Future Developments
//   • Improved User Interfaces (Main Screen Panels & Shortcuts)
//   • Replace the ITDB02_Touch.h Library with the latest release (when available)
//   • Random Weather (Clouds, Storms, etc. along with Increased Wave Action)
//   • Real Weather (Mirror the real weather conditions from user-selected locations)
//   • Modification of Wave Maker to include Tidal Forces with Lunar Cycle 
//   • Automatic Water Changer & Top Off 
//   • Inclusion of Other Sensors: 
//       1. pH 
//       2. Salinity 
//       3. Calcium 
//       4. Orp 
//   • Automatic Dosing 
//    
//************************************************************************************/
//
// Main code based on Stilo
//   http://code.google.com/p/stilo/
//
// LED controlling algorithm is based on Krusduino by Hugh Dangerfield
//   http://code.google.com/p/dangerduino/
//
// A number of functions and some button designs based on the work of Henning Karlsen
//   http://www.henningkarlsen.com/electronics/
//
// Moon Phase algorithm is based in part on a Moon Phase function by NightAtTheOpera
//   http://www.nano-reef.com/forums/index.php?showtopic=217305&st=0&
//
// Special Thanks:
//    Dave Rosser & Hugh Dangerfield - (aka Lewis & Clark?) - Reef Controller Pioneers
//      http://www.ultimatereef.net/forums/showthread.php?t=363432
//    Mark Chester aka "Koyaanisqatsi" - Knows everything there's to know about LEDs
//      http://www.chestersgarage.com/
//    Henning Karlsen
//      http://www.henningkarlsen.com/electronics/
//    Kev Tench aka "tangtastic" - DIY Reef Wizard!
//      http://ukreefs.com/index.php?action=profile;u=1
//    Ned Simpson aka "Surff" - Another DIY Reef Guy
//      http://www.ultimatereef.net/forums/showthread.php?t=400993
//    Neil Williams aka "neildotwilliams" - Yet another DIY Reefer
//      http://www.ultimatereef.net/forums/member.php?u=37721
//    Albert aka "selfonlypath" - Arduino genius
//      http://arduino.cc/forum/index.php?action=profile;u=12410
//
// An Especially Big Thank You to all who Donated, helping to further development.
//
//************************************************************************************/
//
// Known Bugs/Issues:
//   • The level of automation may make you lazy
//   • If you spot an error or bug in this sketch, please let me know!
//   • 
//
//************************************************************************************/
//
// LEGAL DISCLAIMER:
//   Jarduino Aquarium Controller v.1.1 and v.1.2, Copyright 2011, 2012 Jamie M. Jardin.
//   I'm providing this program as free software with the sole intent on furthering 
//   DIY Aquarium Lighting, but WITHOUT ANY WARRANTY; without even the implied warranty 
//   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. You may modify and/or use 
//   it under the terms of the GNU General Public License as published by the Free 
//   Software Foundation version 3 of the License, or (at your option) any later 
//   version.  However if you intend on using the program (either in its entirety or any 
//   part of it) in any way, shape, or form for PROFIT, you MUST contact me and obtain 
//   my EXPRESS WRITTEN CONSENT (contact information is provided above).
//   VIOLATORS WILL BE PROSECUTED TO THE FULLEST EXTENT OF THE LAW.
//
//************************************************************************************/
//
// IF YOU WANT SOME HELP, PLEASE READ ME!
//   Feel free to make changes to suit your needs.  For you convenience, I’v listed the 
//   line numbers in the sketch most commonly changed.  Some of these values will require 
//   a change while other values can simply be modified according to user preference.  
//   If you make changes to lines other than those listed below, know that it may render 
//   the program inoperable or cause unpredictable behavior.  
//
//   143, 147/148, 191-197, 200-203, 206-209, 212, 216-218, 230, 254, 384-470
//
//************************************************************************************/

//LIBRARIES
#include <UTFT.h>
#include <UTouch.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include <EEPROM.h>
#include "writeAnything.h"
#include <DS1307.h>
#include <OneWire.h>
#include <DallasTemperature.h>


//Default Controller Settings
boolean RECOM_RCD = false;            //For Mean Well drivers change "true" to "false"
                                     
//LCD TOUCH PANEL and ITDB02 MEGA SHIELD v1.1
//(Mega Shield utilizes pins 5V, 3V3, GND, 2-6, 20-41, & (50-53 for SD Card))
UTFT myGLCD(ITDB32S,38,39,40,41);   //Uncomment this line for the SSD1289
//UTFT myGLCD(ITDB32, 38,39,40,41);    //Uncomment this line for the HX8347-A 
UTouch myTouch (6,5,4,3,2);


//Initialize the DS1307
DS1307 rtc();
DS1307_RAM ramBuffer;                //Declare a buffer for use

float Time t;
float t_temp;                      //Init Time-data structure
int rtcSetMin, rtcSetHr, rtcSetDy, 
    rtcSetMon, rtcSetYr;    

int setCalendarFormat = 0;           //DD/MM/YYYY=0 || Month DD, YYYY=1 (change in prog)
int displayDOW = 0;                  //Hide=0 || Show=1 (change in prog)
int setTimeFormat = 0;               //24HR=0 || 12HR=1 (change in prog)
int AM_PM, yTime;                    //Setting clock stuff
int timeDispH, timeDispM, 
    xTimeH, xTimeM10, xTimeM1, 
    xTimeAMPM, xColon;
String time, day; 

int setClockOrBlank = 0;             //Clock Screensaver=0 || Blank Screen=1 (change in prog)
int setScreensaverOnOff = 0;         //OFF=0 || ON=1 Turns it ON/OFF (change in prog)
int setScreensaverDOWonOff = 0;      //OFF=0 || ON=1 Shows/Hides DOW in Screensaver (change in prog)
int SS_DOW_x;                        //Moves the DOW to correct position
int setSSmintues;                    //Time in (minutes) before Screensaver comes on (change in program)
int TempSSminutes;                   //Temporary SetSSminutes used in calcs and prints, etc.
int setScreenSaverTimer;             //how long in (minutes) before Screensaver comes on (change in program)
int screenSaverCounter = 0;          //counter for Screen Saver
boolean SCREEN_RETURN = true;        //Auto Return to mainScreen() after so long of inactivity
int returnTimer = 0;                 //counter for Screen Return
int setReturnTimer;                  //Return to main screen 75% of time before the screensaver turns on

//Declare which fonts to be utilized
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];


#define LARGE true
#define SMALL false

//Define the PWM PINS for the LEDs
const int ledPinSump = 7;            //PowerLed Shield pin 10
const int ledPinBlue = 8;            //PowerLed Shield pin 5
const int ledPinWhite = 9;           //PowerLed Shield pin 3
const int ledPinRoyBlue = 10;        //PowerLed Shield pin 6
const int ledPinRed = 11;            //PowerLed Shield pin 9
const int ledPinUV = 12;             //PowerLed Shield pin 11
const int ledPinMoon = 13;           //PowerLed Shield pin 13 (Modification to Shield & NOT controlled by an array)

// Define the other DIGITAL and/or PWM PINS being used
const int tempHeatPin = 52;          //Heater on/off (set thermostat on heater to highest desired level)
const int tempChillPin = 53;         //Chiller on/off (set thermostat on chiller to lowest desired level)
const int WaveMakerTop = 42;         //Hydor Koralia Evolution (Top Plug)
const int WaveMakerBottom = 43;      //Hydor Koralia Evolution (Bottom Plug)
const int HoodFansPWM = 44;          //PWM Hood Heatsink Fan (code added so frequency = 25kHz)
const int SumpFanPWM = 45;           //PWM Sump Heatsink Fan (code added so frequency = 25kHz)
const int HoodFansTranzPin = 47;     //Hood Heatsink Fan on/off
const int SumpFanTranzPin = 48;      //Sump Heatsink Fan on/off
const int tempAlarmPin = 49;         //Buzzer Alarm for Temperature offsets
const int autoFeeder = 50;           //Automatic Fish Feeder

// DS18B20 Temperature sensors plugged into pin 51 (Water, Hood, & Sump)
OneWire OneWireBus(51);     //Choose a digital pin 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&OneWireBus);   
// Assign the addresses of temperature sensors.  Add/Change addresses as needed.
DeviceAddress waterThermometer = { 0x28, 0x5C, 0x56, 0x59, 0x03, 0x00, 0x00, 0xEB };
DeviceAddress hoodThermometer = { 0x28, 0xBE, 0xFA, 0xBC, 0x02, 0x00, 0x00, 0xDD };
DeviceAddress sumpThermometer = { 0x28, 0x72, 0x22, 0x37, 0x03, 0x00, 0x00, 0xB9 };

float tempW = 0;                     //Water temperature values
float tempH = 0;                     //Heatsink temperature
float tempS = 0;                     //Sump heatsink temperature
float setTempToBeginHoodFanC=0.0;    //Temperature to Turn on Hood Fans (in Degrees C)
float setTempToBeginSumpFanC=0.0;    //Temperature to Turn on Sump Fan (in Degrees C)
float setTempToBeginHoodFanF=0.0;    //Temperature to Turn on Hood Fans (in Degrees F)
float setTempToBeginSumpFanF=0.0;    //Temperature to Turn on Sump Fan (in Degrees F)
float temp2beHFan;                   //Temporary Temperature Values
float temp2beSFan;                   //Temporary Temperature Values

float FanOn = 0.2;                   //Starts Fan(s) at 20% Duty Cycle (choose 0.2 or higher)
int HoodTempInterval = 0;            //Used for PWM Duty calculations
int SumpTempInterval = 0;            //Used for PWM Duty calculations
float HoodFanSpeedIncrease = 0;      //Used for PWM Duty calculations
float SumpFanSpeedIncrease = 0;      //Used for PWM Duty calculations
float HoodPWM = 0;                   //Used for PWM Duty calculations
float SumpPWM = 0;                   //Used for PWM Duty calculations

float setTempC = 0.0;                //Desired Water Temperature (User input in program)
float setTempF = 0.0;
float offTempC = 0.0;                //Desired Water Temp. Offsets for Heater & Chiller (User input in program)
float offTempF = 0.0;
float alarmTempC = 0.0;              //Temperature the Alarm will sound (User input in program)
float alarmTempF = 0.0;
boolean tempCoolflag = 0;            //1 if cooling on
boolean tempHeatflag = 0;            //1 if heating on
boolean tempAlarmflag = 0;           //1 if alarm on
long intervalAlarm = 1000 * 30;      //Interval to beep the Alarm (1000 * seconds)
float temp2beS;                      //Temporary Temperature Values
float temp2beO;                      //Temporary Temperature Values
float temp2beA;                      //Temporary Temperature Values
int setTempScale = 0;                //Celsius=0 || Fahrenheit=1 (change in prog)
String degC_F;                       //Used in the Conversion of Celsius to Fahrenheit 

float TempToFall=5.0;                //Temperature to fall by this amount before resuming normal operation
int setDimLEDsOnOff = 0;             //If/When LEDs reach a certain temp they can dim down (feature off by default)
int setLEDsDimTempC = 0;             //Default value is 0, set this value in program
int setLEDsDimTempF = 0;             //Conversion of C-F
int TempLEDsDimTemp;                 //Temporary LED Dimming Temp
int setLEDsDimPercent = 0;           //Choose value to failsafe dim LEDs in program
int TempLEDsDimPercent;              //Temporary LED Dimming Percent
float PercentDim = 0.0;              //Converts saved value in EEPROM to a percentage
int tempLED=255;                     //Temporarily high value, this will change once the program starts
int tempLEDsump=255;                 //Temporarily high value, this will change once the program starts
extern unsigned int                  //Tool on Fire Pic
   HotLEDsFix[0x4C9];

float LC = 29.53059;                 //1 Lunar Cycle = 29.53059 days
String LP;                           //LP = Lunar Phase - variable used to print out Moon Phase
double AG;
int MaxI, tMaxI;                     //Maximum Illumination of Moon (User Defined/Set in Prog. -- Default = 0)
int MinI, tMinI;                     //Minimum Illumination of Moon (User Defined/Set in Prog. -- Default = 0)
                                     //Highest Value (0-255) at which Led is Still OFF, or the Value
                                     //you wish the New Moon to Shine (it will increase from here to MaxI)

unsigned int *MoonPic;               //Pointer to the Lunar Phase Pics
extern unsigned int                  //Lunar Phase Pics
  New_Moon[0xA90],
  Waxing_Crescent[0xA90],
  First_Quarter[0xA90],
  Waxing_Gibbous[0xA90],
  Full_Moon[0xA90],
  Waning_Gibbous[0xA90],
  Last_Quarter[0xA90],
  Waning_Crescent[0xA90];

int dispScreen=0;                    //0-Main Screen, 1-Menu, 2-Clock Setup, 3-Temp Control, 
                                     //4-LED Test Options, 5-Test LED Arrays, 6-Test Individual
                                     //LED Colors, 7-Choose LED Color, 8-View Selected Color
                                     //Values, 9-Change Selected Color Array Values
                                     //10-Wavemaker, 11-Wavemaker Settings, 12-General
                                     //Settings, 13-Automatic Feeder, 14-Set Feeder Timers,
                                     //15-About
                                     
int x, y;                            //touch coordinates

long previousMillisLED = 0;          //Used in the Test LED Array Function
long previousMillisWave = 0;         //Used in the WaveMaker Function wave_output()
long previousMillisFive = 0;         //Used in the Main Loop (Checks Time,Temp,LEDs,Screen)
long previousMillisLEDoff = 0;       //Used in Shut LEDs Off if Temp too high
long previousMillisAlarm = 0;        //Used in the Alarm

boolean waveMakerOff = false;        //For Turning ON/OFF WaveMaker
boolean waveMakerTest = false;       //For Testing Wave Settings
long wPump1, wPump2;                 //Total Alternating Times
long intervalAlt = wPump1;           //Changing Interval for Alternating Mode
long wOnForT, wOffForT;              //Total Synchronous-Pulse Times
long intervalSynch = wOnForT;        //Changing Interval for Synch-Pulse Mode
int PumpTstate = LOW;                //Used to set the Top Powerhead ON or OFF
int PumpBstate = LOW;                //Used to set the Bottom Powerhead ON or OFF

int WAVE, Pump1m, Pump1s, Pump2m,    //EEPROM vars
    Pump2s, Synch, OnForTm, OnForTs, 
    OffForTm, OffForTs;
int MODE = WAVE;
int MIN1=0, SEC1=0, MIN2=0, SEC2=0,  //Used in the Wavemaker viewWaveTimes()
    minY1, minY2;    
int Min1=0, Sec1=0, Min2=0, Sec2=0;  //Used in the Wavemaker viewWaveTimesPage() & wave+/-
int min1X=91, sec1X=237,             //Used in the Wavemaker waveModePlusMinus()
    min2X=91, sec2X=237,
    tTime1=0, tTime2=0;
int WaveCorrector = 2;               //Fix for halving of wave seconds (Change to "1" if 
                                     //your wave seconds are doubled)
    
long previousMillisCt = 0;           //stores the last update for the Countdown Timer
long intervalCt = 1000;              //One Second Interval for Countdown Timer
int countDown  = 5*60 + 0;           //Countdown for 5 minutes and zero seconds
int MIN_O = 5;                       //Start the Time at 5 (for looks only)
int SEC_T = 0;
int SEC_O = 0;

int LedChangTime = 0;                //LED change page, time and values

int min_cnt;                         //Used to determine the place in the color arrays

boolean LEDtestTick = false;         //for testing leds and speed up clock

int whiteLed, blueLed, rblueLed,     //previous LED output values
    redLed, uvLed, sumpLed;
int bled_out, wled_out, rbled_out,   //current LED output values
    rled_out, uvled_out, sled_out,
    moonled_out, colorled_out;

int COLOR=0, WHITE=1, BLUE=2,        //Used to Determine View/Change Color LED Values
    ROYAL=3, RED=4, ULTRA=5, 
    SUMP=6, MOON=7;

boolean colorLEDtest = false;        //To test individual color LEDs
int sbR, sbG, sbB, sbX1, sbX2;       //Used in the Slider Bars
int ySMP=0, yRED=0, yWHT=0, yBLU=0, 
    yRBL=0, yUVL=0, yLUN=0;
int yStore=0, k=0, tSlide=0;    
boolean TopRows = false;             //Allows selection of changing LED values
int bcol_out, wcol_out, rbcol_out,   //Current LED output values for Test Ind. Color LEDs
    rcol_out, uvcol_out, scol_out,
    mooncol_out;
int x1Bar=0, x2Bar=0,                //Used in LED Output Chart on Test Ind. LEDs Screen
    xValue=0, yValue=0, 
    LEDlevel, yBar; 
    
int feedTime;
int FEEDTime1, FEEDTime2,
    FEEDTime3, FEEDTime4;

int feedFish1H, feedFish1M,          //Times to feed the fish
    feedFish2H, feedFish2M,
    feedFish3H, feedFish3M,
    feedFish4H, feedFish4M;
    
int setAutoStop = 0;                 //OFF=0 || ON=1 (change in prog)    
int fiveTillBackOn1, fiveTillBackOn2,
    fiveTillBackOn3, fiveTillBackOn4;
boolean FeedWaveCtrl_1 = false;
boolean FeedWaveCtrl_2 = false;
boolean FeedWaveCtrl_3 = false;
boolean FeedWaveCtrl_4 = false;


//DIMMING VALUES can be changed below BUT the EEPROM must be cleared first.
//To CLEAR EEPROM, use ...arduino-1.0-Jarduino\libraries\EEPROM\examples\eeprom_clear.ino
//and change the 512 to 4096 before Upload.  After the LED comes on indicating the EEPROM
//has been cleared, it is now ok to change DIMMING VALUES below & Upload the sketch.
//SUMP Dimming Values 8pm to 8am
byte sled[96] = {
  192, 200, 200, 200, 200, 200, 200, 200,   //0 - 1
  200, 200, 200, 200, 200, 200, 200, 200,   //2 - 3
  197, 195, 192, 190, 175, 175, 175, 175,   //4 - 5
  140, 140, 140, 140, 105, 95, 85, 75,      //6 - 7
  0, 0, 0, 0, 0, 0, 0, 0,                   //8 - 9
  0, 0, 0, 0, 0, 0, 0, 0,                   //10 - 11
  0, 0, 0, 0, 0, 0, 0, 0,                   //12 - 13
  0, 0, 0, 0, 0, 0, 0, 0,                   //14 - 15
  0, 0, 0, 0, 0, 0, 0, 0,                   //16 - 17
  0, 0, 0, 0, 0, 0, 0, 0,                   //18 - 19
  75, 85, 95, 105, 140, 140, 140, 140,      //20 - 21
  175, 175, 175, 175, 190, 192, 195, 197    //22 - 23
};  
//REGULAR BLUE Dimming Values
byte bled[96] = {
  0, 0, 0, 0, 0, 0, 0, 0,                   //0 - 1
  0, 0, 0, 0, 0, 0, 0, 0,                   //2 - 3
  0, 0, 0, 0, 0, 0, 0, 0,                   //4 - 5
  0, 0, 0, 0, 0, 0, 28, 28,                 //6 - 7
  30, 30, 32, 55, 70, 70, 75, 80,           //8 - 9
  80, 85, 90, 110, 120, 125, 130, 135,      //10 - 11
  140, 145, 150, 160, 160, 160, 165, 170,   //12 - 13
  175, 180, 185, 190, 195, 195, 195, 195,   //14 - 15
  195, 195, 195, 195, 190, 185, 180, 175,   //16 - 17
  170, 165, 160, 160, 160, 150, 145, 140,   //18 - 19
  135, 130, 125, 115, 100, 75, 60, 30,      //20 - 21
  30, 30, 28, 28, 0, 0, 0, 0                //22 - 23
};  
//WHITE Dimming Values (White LED array in RAM)
byte wled[96] = {
  0, 0, 0, 0, 0, 0, 0, 0,                   //0 - 1
  0, 0, 0, 0, 0, 0, 0, 0,                   //2 - 3
  0, 0, 0, 0, 0, 0, 0, 0,                   //4 - 5
  0, 0, 0, 0, 0, 0, 0, 0,                   //6 - 7
  0, 0, 0, 0, 0, 0, 0, 0,                   //8 - 9
  30, 35, 45, 65, 80, 95, 100, 105,         //10 - 11
  110, 115, 120, 125, 130, 130, 135, 140,   //12 - 13
  140, 140, 143, 145, 147, 150, 150, 150,   //14 - 15
  150, 150, 150, 150, 147, 145, 143, 140,   //16 - 17
  140, 140, 130, 125, 120, 115, 105, 100,   //18 - 19
  85, 70, 60, 45, 35, 0, 0, 0,              //20 - 21
  0, 0, 0, 0, 0, 0, 0, 0                    //22 - 23
};
//ROYAL BLUE Dimming Values
byte rbled[96] = {
  0, 0, 0, 0, 0, 0, 0, 0,                   //0 - 1
  0, 0, 0, 0, 0, 0, 0, 0,                   //2 - 3
  0, 0, 0, 0, 0, 0, 0, 0,                   //4 - 5
  0, 0, 0, 0, 0, 0, 35, 40,                 //6 - 7
  40, 43, 47, 55, 65, 75, 80, 85,           //8 - 9
  90, 95, 95, 100, 110, 110, 115, 120,      //10 - 11
  125, 130, 135, 145, 145, 145, 150, 155,   //12 - 13
  160, 165, 170, 175, 180, 180, 180, 180,   //14 - 15
  180, 180, 180, 180, 175, 170, 165, 160,   //16 - 17
  155, 150, 145, 145, 145, 135, 130, 125,   //18 - 19
  120, 115, 110, 110, 100, 75, 65, 50,      //20 - 21
  40, 35, 33, 28, 0, 0, 0, 0                //22 - 23
};  
//RED Dimming Values
byte rled[96] = {
  0, 0, 0, 0, 0, 0, 0, 0,                   //0 - 1
  0, 0, 0, 0, 0, 0, 0, 0,                   //2 - 3
  0, 0, 0, 0, 0, 0, 0, 0,                   //4 - 5
  0, 0, 0, 0, 0, 0, 0, 0,                   //6 - 7
  0, 0, 0, 0, 0, 0, 0, 0,                   //8 - 9
  0, 0, 0, 0, 0, 0, 0, 0,                   //10 - 11
  30, 30, 40, 40, 50, 50, 60, 60,           //12 - 13
  60, 60, 50, 50, 40, 40, 30, 30,           //14 - 15
  0, 0, 0, 0, 0, 0, 0, 0,                   //16 - 17
  0, 0, 0, 0, 0, 0, 0, 0,                   //18 - 19
  0, 0, 0, 0, 0, 0, 0, 0,                   //20 - 21
  0, 0, 0, 0, 0, 0, 0, 0    
};  
//ULTRA VIOLET (UV) Dimming Values
byte uvled[96] = {
  0, 0, 0, 0, 0, 0, 0, 0,                   //0 - 1
  0, 0, 0, 0, 0, 0, 0, 0,                   //2 - 3
  0, 0, 0, 0, 0, 0, 0, 0,                   //4 - 5
  0, 0, 0, 0, 0, 0, 0, 0,                   //6 - 7
  0, 0, 0, 0, 0, 0, 0, 0,                   //8 - 9
  0, 0, 0, 0, 20, 25, 30, 35,               //10 - 11
  40, 45, 50, 55, 60, 65, 70, 70,           //12 - 13
  65, 60, 55, 50, 45, 40, 35, 30,           //14 - 15
  25, 20, 0, 0, 0, 0, 0, 0,                 //16 - 17
  0, 0, 0, 0, 0, 0, 0, 0,                   //18 - 19
  0, 0, 0, 0, 0, 0, 0, 0,                   //20 - 21
  0, 0, 0, 0, 0, 0, 0, 0    
};
byte tled[96];     //Temporary Array to Hold changed LED Values


/**************************** CHOOSE OPTION MENU BUTTONS *****************************/
const int tanD[]= {10, 29, 155, 59};        //"TIME and DATE" settings
const int temC[]= {10, 69, 155, 99};        //"H2O TEMP CONTROL" settings
const int wave[]= {10, 109, 155, 139};      //"Wavemaker CONTROL" settings
const int gSet[]= {10, 149, 155, 179};      //"GENERAL SETTINGS" page
const int tesT[]= {165, 29, 310, 59};       //"LED TESTING OPTIONS" menu
const int ledChM[]= {165, 69, 310, 99};     //"CHANGE LED VALUES" menu
const int aFeed[]= {165, 109, 310, 139};    //"AUTOMATIC FEEDER" menu
const int about[]= {165, 149, 310, 179};    //"ABOUT" program information
/**************************** TIME AND DATE SCREEN BUTTONS ***************************/
const int houU[]= {110, 22, 135, 47};       //hour up
const int minU[]= {180, 22, 205, 47};       //min up
const int ampmU[]= {265, 22, 290, 47};      //AM/PM up
const int houD[]= {110, 73, 135, 98};       //hour down
const int minD[]= {180, 73, 205, 98};       //min down
const int ampmD[]= {265, 73, 290, 98};      //AM/PM down
const int dayU[]= {110, 112, 135, 137};     //day up
const int monU[]= {180, 112, 205, 137};     //month up
const int yeaU[]= {265, 112, 290, 137};     //year up
const int dayD[]= {110, 162, 135, 187};     //day down
const int monD[]= {180, 162, 205, 187};     //month down
const int yeaD[]= {265, 162, 290, 187};     //year down
/*************************** H2O TEMP CONTROL SCREEN BUTTONS *************************/
const int temM[]= {90, 49, 115, 74};        //temp. minus
const int temP[]= {205, 49, 230, 74};       //temp. plus
const int offM[]= {90, 99, 115, 124};       //offset minus
const int offP[]= {205, 99, 230, 124};      //offset plus
const int almM[]= {90, 149, 115, 174};      //alarm minus
const int almP[]= {205, 149, 230, 174};     //alarm plus
/**************************** LED TESTING MENU BUTTONS *******************************/
const int tstLA[] = {40, 59, 280, 99};      //"Test LED Array Output" settings
const int cntIL[] = {40, 109, 280, 149};    //"Control Individual Leds" settings
/********************** TEST LED ARRAY OUTPUT SCREEN BUTTONS *************************/
const int stsT[]= {110, 105, 200, 175};     //start/stop
const int tenM[]= {20, 120, 90, 160};       //-10s
const int tenP[]= {220, 120, 290, 160};     //+10s
/******************** TEST INDIVIDUAL LED VALUES SCREEN BUTTONS **********************/
//See at the end of these buttons for the button functions
/****************** CHANGE INDIVIDUAL LED VALUES SCREEN BUTTONS **********************/
//These Buttons are made within the function
/************************* CHANGE LED VALUES MENU BUTTONS ****************************/
const int btCIL[]= {5, 188, 90, 220};       //back to Change Individual LEDs Screen
const int ledChV[]= {110, 200, 210, 220};   //LED Change Values
const int eeprom[]= {215, 200, 315, 220};   //Save to EEPROM (Right Button)
const int MINiM[]= {17, 147, 42, 172};      //MinI minus
const int MINiP[]= {116, 147, 141, 172};    //MinI plus
const int MAXiM[]= {179, 147, 204, 172};    //MaxI minus
const int MAXiP[]= {278, 147, 303, 172};    //MaxI plus
/********************* WAVEMAKER SCREEN & STETTINGS BUTTONS **************************/
//Many Wavemaker Buttons are made within the function(s)
const int pump1Mm[]= {21, 70, 46, 95};      //Pump 1 minute minus
const int pump1Mp[]= {120, 70, 145, 95};    //Pump 1 minute plus
const int pump1Sm[]= {175, 70, 200, 95};    //Pump 1 second minus
const int pump1Sp[]= {274, 70, 299, 95};    //Pump 1 second plus
const int pump2Mm[]= {21, 147, 46, 172};    //Pump 2 minute minus
const int pump2Mp[]= {120, 147, 145, 172};  //Pump 2 minute plus
const int pump2Sm[]= {175, 147, 200, 172};  //Pump 2 second minus
const int pump2Sp[]= {274, 147, 299, 172};  //Pump 2 second plus
/************************* AUTOMATIC FISH FEEDER BUTTONS *****************************/
//These Buttons are made within the function
/******************* SET AUTOMATIC FISH FEEDING TIMES BUTTONS ************************/
const int houP[]= {110, 38, 135, 63};       //hour up
const int minP[]= {180, 38, 205, 63};       //min up
const int ampmP[]= {265, 38, 290, 63};      //AM/PM up
const int houM[]= {110, 89, 135, 114};      //hour down
const int minM[]= {180, 89, 205, 114};      //min down
const int ampmM[]= {265, 89, 290, 114};     //AM/PM down
/***************************** MISCELLANEOUS BUTTONS *********************************/
const int backGS[]= {4, 200, 78, 220};      //BACK
const int nextGS[]= {83, 200, 157, 220};    //NEXT
const int prSAVEgs[]= {162, 200, 236, 220}; //SAVE
const int canCgs[]= {241, 200, 315, 220};   //CANCEL

const int HoodFanTm[]= {90, 56, 115, 81};   //Hood Fan Temp -
const int HoodFanTp[]= {205, 56, 230, 81};  //Hood Fan Temp +
const int SumpFanTm[]= {90, 147, 115, 172}; //Sump Fan Temp -
const int SumpFanTp[]= {205, 147, 230, 172};//Sump Fan Temp +

const int back[]= {5, 200, 105, 220};       //BACK
const int prSAVE[]= {110, 200, 210, 220};   //SAVE
const int canC[]= {215, 200, 315, 220};     //CANCEL

void drawUpButtonSlide(int x, int y)
{
  myGLCD.setColor(64, 64, 128);
  myGLCD.fillRoundRect(x, y, x+30, y+22);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(x, y, x+30, y+22);
  myGLCD.setColor(128, 128, 255);
  for (int i=0; i<15; i++)
    myGLCD.drawLine(x+5+(i/1.5), y+18-i, x+26-(i/1.5), y+18-i);
}
void drawDownButtonSlide(int x, int y)
{
  myGLCD.setColor(64, 64, 128);
  myGLCD.fillRoundRect(x, y, x+30, y+22);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(x, y, x+30, y+22);
  myGLCD.setColor(128, 128, 255);
  for (int i=0; i<15; i++)
    myGLCD.drawLine(x+5+(i/1.5), y+4+i, x+26-(i/1.5), y+4+i);
}

void drawUpButton(int x, int y)
{
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect(x, y, x+25, y+25);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(x, y, x+25, y+25);
  for (int i=0; i<15; i++)
    myGLCD.drawLine(x+3+(i/1.5), y+19-i, x+22-(i/1.5), y+19-i);
}
void drawDownButton(int x, int y)
{
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect(x, y, x+25, y+25);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(x, y, x+25, y+25);
  for (int i=0; i<15; i++)
    myGLCD.drawLine(x+3+(i/1.5), y+6+i, x+22-(i/1.5), y+6+i);
}
void printButton(char* text, int x1, int y1, int x2, int y2, boolean fontsize = false)
{
  int stl = strlen(text);
  int fx, fy;
  
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect (x1, y1, x2, y2);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
 
  myGLCD.setBackColor(0, 0, 255);
  if (fontsize) {
    myGLCD.setFont(BigFont); 
    fx = x1+(((x2 - x1+1)-(stl*16))/2);
    fy = y1+(((y2 - y1+1)-16)/2);
    myGLCD.print(text, fx, fy);
    }
  else {
    myGLCD.setFont(SmallFont); 
    fx = x1+(((x2 - x1)-(stl*8))/2);
    fy = y1+(((y2 - y1-1)-8)/2);
    myGLCD.print(text, fx, fy);
    }
}

void waitForIt(int x1, int y1, int x2, int y2)   // Draw a red frame while a button is touched
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  waitForTouchRelease();
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
}
/**************************** END OF PRIMARY BUTTONS **********************************/


/********************************* EEPROM FUNCTIONS ***********************************/
struct config_t
{
  int tempset;
  int tempFset;
  int tempoff;
  int tempFoff;
  int tempalarm;
  int tempFalarm;
} tempSettings;

struct config_m
{
  int MinI_t;
  int MaxI_t;
} MinMaxIsettings;

struct config_L
{
  int DimLEDsOnOff;
  int LEDsDimTempC;
  int LEDsDimTempF;  
  int LEDsDimPercent;
} LEDsFailsafeSettings;

struct config_w
{
  int waveMode;
  int altPump1m;
  int altPump1s;  
  int altPump2m;
  int altPump2s;
  int synchMode;
  int synchPumpOnM;
  int synchPumpOnS; 
  int synchPumpOffM;  
  int synchPumpOffS;
} WAVEsettings;

struct config_g
{
  int calendarFormat;
  int ShowHideDOW;
  int timeFormat;
  int tempScale;
  int HoodFanTempC;
  int HoodFanTempF;
  int SumpFanTempC;
  int SumpFanTempF;
  int SCREENsaver;
  int ScreensaverClockOrBlank;
  int ScreensaverDOWonOff;
  int ScreensaverTimer;
  int autoStop;
} GENERALsettings;

struct config_f
{
  int feedFish1h;
  int feedFish1m;
  int feedFish2h;
  int feedFish2m;
  int feedFish3h;
  int feedFish3m;
  int feedFish4h;
  int feedFish4m;
  int feedTime1;
  int feedTime2;
  int feedTime3;
  int feedTime4;  
} FEEDERsettings;

void SaveLEDToEEPROM()
{
  EEPROM.write(0, 123);         //to determine if data available in EEPROM
  for (int i=1; i<97; i++)  
    {
     EEPROM.write(i+(96*0), wled[i]);
     EEPROM.write(i+(96*1), bled[i]);
     EEPROM.write(i+(96*2), rbled[i]);
     EEPROM.write(i+(96*3), rled[i]);
     EEPROM.write(i+(96*4), sled[i]);
     EEPROM.write(i+(96*5), uvled[i]);    
    } 
}

void SaveMoonLEDToEEPROM()
{
  MinMaxIsettings.MinI_t = int(MinI);
  MinMaxIsettings.MaxI_t = int(MaxI);
  EEPROM_writeAnything(600, MinMaxIsettings);
}

void SaveLEDsFailsafeToEEPROM()
{
  LEDsFailsafeSettings.DimLEDsOnOff = int(setDimLEDsOnOff);
  LEDsFailsafeSettings.LEDsDimTempC = int(setLEDsDimTempC);
  LEDsFailsafeSettings.LEDsDimTempF = int(setLEDsDimTempF);  
  LEDsFailsafeSettings.LEDsDimPercent = int(setLEDsDimPercent);
  EEPROM_writeAnything(630, LEDsFailsafeSettings);  
}

void SaveWaveToEEPROM()
{
  WAVEsettings.waveMode = int(WAVE);
  WAVEsettings.altPump1m = int(Pump1m);
  WAVEsettings.altPump1s = int(Pump1s);  
  WAVEsettings.altPump2m = int(Pump2m);
  WAVEsettings.altPump2s = int(Pump2s);  
  WAVEsettings.synchMode = int(Synch);
  WAVEsettings.synchPumpOnM = int(OnForTm);
  WAVEsettings.synchPumpOnS = int(OnForTs);  
  WAVEsettings.synchPumpOffM = int(OffForTm);
  WAVEsettings.synchPumpOffS = int(OffForTs);  
  EEPROM_writeAnything(660, WAVEsettings);
}

void SaveTempToEEPROM()
{
  tempSettings.tempset = int(setTempC*10);
  tempSettings.tempFset = int(setTempF*10);  
  tempSettings.tempoff = int(offTempC*10);
  tempSettings.tempFoff = int(offTempF*10);  
  tempSettings.tempalarm = int(alarmTempC*10);
  tempSettings.tempFalarm = int(alarmTempF*10);
  EEPROM_writeAnything(690, tempSettings);
}

void SaveGenSetsToEEPROM()
{
  GENERALsettings.calendarFormat = int(setCalendarFormat);
  GENERALsettings.ShowHideDOW = int(displayDOW);
  GENERALsettings.timeFormat = int(setTimeFormat);
  GENERALsettings.tempScale = int(setTempScale);
  GENERALsettings.HoodFanTempC = int(setTempToBeginHoodFanC*10); 
  GENERALsettings.HoodFanTempF = int(setTempToBeginHoodFanF*10); 
  GENERALsettings.SumpFanTempC = int(setTempToBeginSumpFanC*10);  
  GENERALsettings.SumpFanTempF = int(setTempToBeginSumpFanF*10);  
  GENERALsettings.SCREENsaver = int(setScreensaverOnOff);
  GENERALsettings.ScreensaverClockOrBlank = int(setClockOrBlank);      
  GENERALsettings.ScreensaverDOWonOff = int(setScreensaverDOWonOff);    
  GENERALsettings.ScreensaverTimer = int(setSSmintues);    
  GENERALsettings.autoStop = int(setAutoStop);
  EEPROM_writeAnything(720, GENERALsettings);
}

void SaveFeedTimesToEEPROM()
{
  FEEDERsettings.feedFish1h = int(feedFish1H);
  FEEDERsettings.feedFish1m = int(feedFish1M);  
  FEEDERsettings.feedFish2h = int(feedFish2H);
  FEEDERsettings.feedFish2m = int(feedFish2M);  
  FEEDERsettings.feedFish3h = int(feedFish3H);
  FEEDERsettings.feedFish3m = int(feedFish3M);  
  FEEDERsettings.feedFish4h = int(feedFish4H);
  FEEDERsettings.feedFish4m = int(feedFish4M);
  FEEDERsettings.feedTime1 = int(FEEDTime1);  
  FEEDERsettings.feedTime2 = int(FEEDTime2);  
  FEEDERsettings.feedTime3 = int(FEEDTime3);  
  FEEDERsettings.feedTime4 = int(FEEDTime4);    
  EEPROM_writeAnything(750, FEEDERsettings);
}

void ReadFromEEPROM()
{
  int k = EEPROM.read(0);
  char tempString[3];
  
  if (k==123) {
    for (int i=1; i<97; i++)  {
      wled[i] = EEPROM.read(i+(96*0));
      bled[i] = EEPROM.read(i+(96*1));
     rbled[i] = EEPROM.read(i+(96*2));
      rled[i] = EEPROM.read(i+(96*3));
      sled[i] = EEPROM.read(i+(96*4));
     uvled[i] = EEPROM.read(i+(96*5));   
    }  
   }

  EEPROM_readAnything(600, MinMaxIsettings);  
  MinI = MinMaxIsettings.MinI_t;
  MaxI = MinMaxIsettings.MaxI_t;
  
  EEPROM_readAnything(630, LEDsFailsafeSettings);
  setDimLEDsOnOff = LEDsFailsafeSettings.DimLEDsOnOff;
  setLEDsDimTempC = LEDsFailsafeSettings.LEDsDimTempC;
  setLEDsDimTempF = LEDsFailsafeSettings.LEDsDimTempF;  
  setLEDsDimPercent = LEDsFailsafeSettings.LEDsDimPercent;
  
  EEPROM_readAnything(660, WAVEsettings);  
  WAVE = WAVEsettings.waveMode;
  Pump1m = WAVEsettings.altPump1m;
  Pump1s = WAVEsettings.altPump1s;
  Pump2m = WAVEsettings.altPump2m;
  Pump2s = WAVEsettings.altPump2s;
  Synch = WAVEsettings.synchMode;
  OnForTm = WAVEsettings.synchPumpOnM;
  OnForTs = WAVEsettings.synchPumpOnS;
  OffForTm = WAVEsettings.synchPumpOffM;
  OffForTs = WAVEsettings.synchPumpOffS;
   
  EEPROM_readAnything(690, tempSettings);  
  setTempC = tempSettings.tempset;
  setTempC /=10;
  setTempF = tempSettings.tempFset;
  setTempF /=10;  
  offTempC = tempSettings.tempoff;
  offTempC /=10;
  offTempF = tempSettings.tempFoff;
  offTempF /=10;  
  alarmTempC = tempSettings.tempalarm;
  alarmTempC /= 10;
  alarmTempF = tempSettings.tempFalarm;
  alarmTempF /= 10;
  
  EEPROM_readAnything(720, GENERALsettings);
  setCalendarFormat = GENERALsettings.calendarFormat;
  displayDOW = GENERALsettings.ShowHideDOW;
  setTimeFormat = GENERALsettings.timeFormat;
  setTempScale = GENERALsettings.tempScale;
  setTempToBeginHoodFanC = GENERALsettings.HoodFanTempC;
  setTempToBeginHoodFanC /= 10;
  if (setTempToBeginHoodFanC==0) {setTempToBeginHoodFanC=29.0;}      
  setTempToBeginHoodFanF = GENERALsettings.HoodFanTempF;
  setTempToBeginHoodFanF /= 10;
  if (setTempToBeginHoodFanF==0) {setTempToBeginHoodFanF=84.2;}    
  setTempToBeginSumpFanC = GENERALsettings.SumpFanTempC;  
  setTempToBeginSumpFanC /= 10;
  if (setTempToBeginSumpFanC==0) {setTempToBeginSumpFanC=29.0;}  
  setTempToBeginSumpFanF = GENERALsettings.SumpFanTempF;  
  setTempToBeginSumpFanF /= 10;
  if (setTempToBeginSumpFanF==0) {setTempToBeginSumpFanF=84.2;}  
  setScreensaverOnOff = GENERALsettings.SCREENsaver;  
  setClockOrBlank = GENERALsettings.ScreensaverClockOrBlank;      
  setScreensaverDOWonOff = GENERALsettings.ScreensaverDOWonOff;    
  setSSmintues = GENERALsettings.ScreensaverTimer;   
  if (setSSmintues==0) {setSSmintues=20;}
  setAutoStop = GENERALsettings.autoStop;
  
  
  EEPROM_readAnything(750, FEEDERsettings);  
  feedFish1H = FEEDERsettings.feedFish1h;
  feedFish1M = FEEDERsettings.feedFish1m;  
  feedFish2H = FEEDERsettings.feedFish2h;
  feedFish2M = FEEDERsettings.feedFish2m;  
  feedFish3H = FEEDERsettings.feedFish3h;
  feedFish3M = FEEDERsettings.feedFish3m;  
  feedFish4H = FEEDERsettings.feedFish4h;
  feedFish4M = FEEDERsettings.feedFish4m;
  FEEDTime1 = FEEDERsettings.feedTime1;
  FEEDTime2 = FEEDERsettings.feedTime2;
  FEEDTime3 = FEEDERsettings.feedTime3;
  FEEDTime4 = FEEDERsettings.feedTime4;  
}
/***************************** END OF EEPROM FUNCTIONS ********************************/


/********************************** RTC FUNCTIONS *************************************/
void TimeDateBar(boolean refreshAll=false)
{
  int Hour12;
  String ampm;
  t = rtc.getTime();
  
  //Date
  setFont(SMALL, 255, 255, 0, 64, 64, 64);
  if ((setCalendarFormat==0) && (displayDOW==1))
    { myGLCD.print("  ", 48, 227);
      myGLCD.print(rtc.getDateStr(FORMAT_LONG, FORMAT_LITTLEENDIAN, '/'), 56, 227);
      myGLCD.print("  ", 136, 227);
      t.dow=calcDOW(t.date, t.mon, t.year);          
      showDOWonBar(t.dow); 
    }
  if ((setCalendarFormat==1) && (displayDOW==1))  
    { myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 56, 227);    //3 letter Month
      myGLCD.print(" ", 80, 227);
      myGLCD.printNumI(t.date, 88, 227);
      if (t.date<10)
        { myGLCD.print(", ", 96, 227);
          myGLCD.printNumI(t.year, 112, 227);
          myGLCD.print("   ", 144, 227);}          
      else
        { myGLCD.print(", ", 104, 227);
          myGLCD.printNumI(t.year, 120, 227);
          myGLCD.print("   ", 152, 227);
        }
      t.dow=calcDOW(t.date, t.mon, t.year);          
      showDOWonBar(t.dow);        
    }
  if ((setCalendarFormat==0) && (displayDOW==0))
    { myGLCD.print("    ", 13, 227);
      myGLCD.print(rtc.getDateStr(FORMAT_LONG, FORMAT_LITTLEENDIAN, '/'), 45, 227);
      myGLCD.print("   ", 125, 227);}
  if ((setCalendarFormat==1) && (displayDOW==0))
    { myGLCD.print("    ", 5, 227);      
      myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 37, 227);    //3 letter Month
      myGLCD.print(" ", 61, 227);    
      myGLCD.printNumI(t.date, 69, 227);
      if (t.date<10)
        { myGLCD.print(", ", 77, 227);
          myGLCD.printNumI(t.year, 93, 227);
          myGLCD.print("   ", 125, 227);}          
      else
        { myGLCD.print(", ", 85, 227);
          myGLCD.printNumI(t.year, 101, 227);
          myGLCD.print("   ", 133, 227);
        }
    }

  //Time
  if (setTimeFormat==1) 
    {
      if (t.hour==0) { Hour12 = 12; }                          //12 HR Format
        else {
          if (t.hour>12) { Hour12 = t.hour-12; }
            else { Hour12 = t.hour; }}

      if (Hour12<10)
        { myGLCD.print(" ", 220, 227);
          myGLCD.printNumI(Hour12, 228, 227);}
      else
        { myGLCD.printNumI(Hour12, 220, 227);}
      myGLCD.print(":", 236, 227);
      if (t.min<10)
        { myGLCD.printNumI(0, 244, 227);
          myGLCD.printNumI(t.min, 252, 227);}
      else
        { myGLCD.printNumI(t.min, 244, 227);}
      if(t.hour < 12){ myGLCD.print(" AM  ", 260, 227); }      //Adding the AM/PM sufffix
        else { myGLCD.print(" PM  ", 260, 227); }
    }
  else
    { myGLCD.print(rtc.getTimeStr(FORMAT_SHORT), 220, 227);    //24 HR Format
      myGLCD.print("    ", 260, 227);}
}


byte calcDOW(byte d, byte m, int y)
{
  int dow;
  byte mArr[12] = {6,2,2,5,0,3,5,1,4,6,2,4};
  
  dow = (y % 100);
  dow = dow*1.25;
  dow += d;
  dow += mArr[m-1];
  if (((y % 4)==0) && (m<3))
    dow -= 1;
  while (dow>7)
    dow -= 7;
    
  return dow;
}


void showDOW(byte dow)
{
  char* str[] = {"MON","TUE","WED","THU","FRI","SAT","SUN"};
  
  setFont(SMALL, 255, 255, 255, 0, 0, 0); 
  myGLCD.print("         ", 17, 178);
  myGLCD.print(str[dow-1], 41, 178);
}


void showDOWonBar(byte dow)
{
  char* str[] = {"MON,","TUE,","WED,","THU,","FRI,","SAT,","SUN,"};
  myGLCD.print(str[dow-1], 16, 227);
}

byte validateDate(byte d, byte m, word y)
{
  byte mArr[12] = {31,0,31,30,31,30,31,31,30,31,30,31};
  byte od;
  
  if (m==2)
  { if ((y % 4)==0)
    { if (d==30)
        od=1;
      else if (d==0)
        od=29;
      else
        od=d;
    }
    else
    { if (d==29)
        od=1;
      else if (d==0)
        od=28;
      else
        od=d;
    }
  }
  else
  { if (d==0)
      od=mArr[m-1];
    else if (d==(mArr[m-1]+1))
      od=1;
    else
      od=d;
  }
  return od;
}


byte validateDateForMonth(byte d, byte m, word y)
{
  byte mArr[12] = {31,0,31,30,31,30,31,31,30,31,30,31};
  byte od;
  boolean dc=false;
  
  if (m==2)
  { if ((y % 4)==0)
    { if (d>29)
      { d=29;}
    }
    else
    { if (d>28)
      { d=28;}
    }
  }
  else
  { if (d>mArr[m-1])
    { d=mArr[m-1];}
  }
  return d;
}
/********************************* END OF RTC FUNCTIONS *******************************/


/************************************* LED LEVELS *************************************/
void LED_levels_output()
{
  int sector, sstep, t1, t2 ;
  int s_out, b_out, w_out, rb_out, r_out, uv_out, moon_out;
 
  if (colorLEDtest) 
    {
     sled_out = scol_out; 
     rled_out = rcol_out; 
     wled_out = wcol_out; 
     bled_out = bcol_out; 
     rbled_out = rbcol_out; 
     uvled_out = uvcol_out; 
     moonled_out = mooncol_out;
    } 
  else 
    {
     if (min_cnt>=1440) {min_cnt=1;}   // 24 hours of minutes 
     sector = min_cnt/15;              // divided by gives sector -- 15 minute
     sstep = min_cnt%15;               // remainder gives add on to sector value 
 
     t1 =sector;
     if (t1==95) {t2=0;}
       else {t2 = t1+1;}
  
     if (sstep==0) 
       {
        sled_out = sled[t1];
        bled_out = bled[t1];
        wled_out = wled[t1];
        rbled_out = rbled[t1];
        rled_out = rled[t1];
        uvled_out = uvled[t1];
       } else 
           {
            sled_out = check(&sled[t1], &sled[t2], sstep);
            bled_out = check(&bled[t1], &bled[t2], sstep);
            wled_out = check(&wled[t1], &wled[t2], sstep);
            rbled_out = check(&rbled[t1], &rbled[t2], sstep);
            rled_out = check(&rled[t1], &rled[t2], sstep);
            uvled_out = check(&uvled[t1], &uvled[t2], sstep);
           }  
     float lunarCycle = moonPhase(t.year, t.mon, t.date); //get a value for the lunar cycle
     moonled_out = MinI *(1 - lunarCycle) +            
                   MaxI * lunarCycle + 0.5;               //MaximumIllumination * % of Full Moon (0-100)
    }
  if (setDimLEDsOnOff==1) {HOT_LEDs();}
  
  if (RECOM_RCD) {
    s_out = sled_out;
    b_out = bled_out;
    w_out = wled_out;
    rb_out = rbled_out;
    r_out = rled_out;
    uv_out = uvled_out;
    moon_out = moonled_out;
  } else {
    s_out = 255 - sled_out;
    b_out = 255 - bled_out;
    w_out = 255 - wled_out;
    rb_out = 255 - rbled_out;
    r_out = 255 - rled_out;
    uv_out = 255 - uvled_out; 
    moon_out = 255 - moonled_out;
  }

  analogWrite(ledPinSump, s_out);
  analogWrite(ledPinBlue, b_out);
  analogWrite(ledPinWhite, w_out);
  analogWrite(ledPinRoyBlue, rb_out);
  analogWrite(ledPinRed, r_out);
  analogWrite(ledPinUV, uv_out); 
  analogWrite(ledPinMoon, moon_out);
}

int check( byte *pt1, byte *pt2, int lstep)
{
  int result;
  float fresult;
   
  if (*pt1==*pt2) {result = *pt1;}     // No change
    else if (*pt1<*pt2)                //Increasing brightness
    { fresult = ((float(*pt2-*pt1)/15.0) * float(lstep))+float(*pt1);
     result = int(fresult);
     }
    //Decreasing brightness
    else {fresult = -((float(*pt1-*pt2)/15.0) * float(lstep))+float(*pt1);
     result = int(fresult);                     
    } 
    return result;  
}
/********************************* END OF LED LEVELS **********************************/


/************************************ WAVE OUTPUT *************************************/
void wave_output()
{
  unsigned long currentMillis = millis();

  if (WAVE==1)                         //Alternating Mode         
    {
     if (waveMakerTest==true)
       { 
        wPump1 = ((Min1*60)+Sec1*WaveCorrector);
        wPump1 = wPump1*1000;
        wPump2 = ((Min2*60)+Sec2*WaveCorrector);
        wPump2 = wPump2*1000;
       } 
     else
       { 
        wPump1 = ((Pump1m*60)+Pump1s*WaveCorrector);
        wPump1 = wPump1*1000;
        wPump2 = ((Pump2m*60)+Pump2s*WaveCorrector);
        wPump2 = wPump2*1000;
       }
       
     if (currentMillis - previousMillisWave > intervalAlt)
       {
        previousMillisWave = currentMillis;
        
        if (wPump1==wPump2) { wPump2 = wPump2+1;}
        
        if (intervalAlt==wPump1)      
          { intervalAlt=wPump2;
            PumpTstate = LOW; 
            PumpBstate = HIGH;}
        else 
          { intervalAlt=wPump1;
            PumpTstate = HIGH; 
            PumpBstate = LOW;}
       }
    }
  if ((WAVE==2) && (Synch==1))         //Synchronous - Constanly ON
    {
     PumpTstate = HIGH;
     PumpBstate = HIGH;
    } else    

  if ((WAVE==2) && (Synch==2))         //Synchronous - Pulsating
    {
     if (waveMakerTest==true)
       { 
        wOnForT = ((Min1*60)+Sec1*WaveCorrector);
        wOnForT = wOnForT*1000;
        wOffForT = ((Min2*60)+Sec2*WaveCorrector);
        wOffForT = wOffForT*1000;
       } 
     else
       {  
        wOnForT = (OnForTm*60)+OnForTs*WaveCorrector;
        wOnForT = wOnForT*1000;
        wOffForT = (OffForTm*60)+OffForTs*WaveCorrector;
        wOffForT = wOffForT*1000;
       }
       
     if (currentMillis - previousMillisWave > intervalSynch)
       {
        previousMillisWave = currentMillis;
        
        if (wOnForT==wOffForT) { wOffForT = wOffForT+1;}        
        
        if (intervalSynch==wOnForT) 
          { intervalSynch=wOffForT;
            PumpTstate = LOW; 
            PumpBstate = LOW;}
        else 
          { intervalSynch=wOnForT;
            PumpTstate = HIGH; 
            PumpBstate = HIGH;}
       }
    }
    digitalWrite(WaveMakerTop, PumpTstate);
    digitalWrite(WaveMakerBottom, PumpBstate);
}
/********************************* END OF WAVE OUTPUT *********************************/


/******************************** TEMPERATURE FUNCTIONS *******************************/
void checkTempC()
{ 
  sensors.requestTemperatures();   // call sensors.requestTemperatures() to issue a global 
                                   // temperature request to all devices on the bus
  tempW = (sensors.getTempC(waterThermometer));  //read water temperature
  tempH = (sensors.getTempC(hoodThermometer));   //read hood's heatsink temperature
  tempS = (sensors.getTempC(sumpThermometer));   //read sump's heatsink temperature

  if (tempW<(setTempC+offTempC+alarmTempC) && tempW>(setTempC-offTempC-alarmTempC)) {
    tempAlarmflag = false;
    digitalWrite(tempAlarmPin, LOW);                           //turn off alarm
    }
  if (tempW<(setTempC+offTempC) && tempW>(setTempC-offTempC))  //turn off chiller/heater
      {
       tempCoolflag=false;
       tempHeatflag=false;
       digitalWrite(tempHeatPin, LOW);
       digitalWrite(tempChillPin, LOW);   
       }
  if (offTempC>0)
    {
     if (tempW >=(setTempC+offTempC))            //turn on chiller
       {
        tempCoolflag=true;
        digitalWrite(tempChillPin, HIGH);
       }
     if (tempW<=(setTempC-offTempC))             //turn an heater
       {
        tempHeatflag=true;
        digitalWrite(tempHeatPin, HIGH);
       }
    }
  if (alarmTempC>0)                              //turn on alarm
    {
     if ((tempW>=(setTempC+offTempC+alarmTempC)) || (tempW<=(setTempC-offTempC-alarmTempC)))
       {
        tempAlarmflag=true;
        unsigned long cMillis = millis();
        if (cMillis - previousMillisAlarm > intervalAlarm)
          {
           previousMillisAlarm = cMillis;
           digitalWrite(tempAlarmPin, HIGH);
           delay(1000);
           digitalWrite(tempAlarmPin, LOW);      
          }
       }
    }
  
  //Fan Controller for Hood
  HoodTempInterval = (tempH - setTempToBeginHoodFanC);     //Sets the interval to start from 0
  HoodFanSpeedIncrease = HoodTempInterval*0.1;  //Fan's speed increases 10% every degree over set temperature
  digitalWrite(HoodFansTranzPin, HIGH);  
  if (tempH < setTempToBeginHoodFanC)            //If Temp's less than defined value, leave fan off
    { HoodPWM = 0;
      digitalWrite(HoodFansTranzPin, LOW);}
  if ((tempH  >= setTempToBeginHoodFanC) && (HoodFanSpeedIncrease < 1))   //For every degree over defined value, increase by 10%
    { HoodPWM = FanOn + HoodFanSpeedIncrease;}
  if (HoodFanSpeedIncrease >= 1)                //If the temperature is 10 or more degrees C higher than user 
    { HoodPWM = 1;}                             //defined value to start, leave it at 100%
 
  //Fan Controller for Sump
  SumpTempInterval = (tempS - setTempToBeginSumpFanC);     //Sets the interval to start from 0
  SumpFanSpeedIncrease = SumpTempInterval*0.1;  //Fan's speed increases 10% every degree over set temperature
  digitalWrite(SumpFanTranzPin, HIGH);  
  if (tempS < setTempToBeginSumpFanC)            //If Temp's less than defined value, leave fan off
    { SumpPWM = 0;
      digitalWrite(SumpFanTranzPin, LOW);}
  if ((tempS  >= setTempToBeginSumpFanC) && (SumpFanSpeedIncrease < 1))   //For every degree over defined value, increase by 10%
    { SumpPWM = FanOn + SumpFanSpeedIncrease;}
  if (SumpFanSpeedIncrease >= 1)                //If the temperature is 10 or more degrees C higher than user 
    { SumpPWM = 1;}                             //defined value to start, leave it at 100%
}
/*************************** END OF TEMPERATURE FUNCTIONS *****************************/


/**************************** DIM LEDs WHEN HOT FUNCTION ******************************/
void HOT_LEDs()
{
  float TTF;
  if (setTempScale==1) 
    {TTF=(TempToFall*.55556)+0.05;} 
  else TTF=TempToFall-0.05;   
  
  if (tempH>=setLEDsDimTempC)                   //Trigger For LED dimming in main Hood
    {
     PercentDim = setLEDsDimPercent*0.01;
     bled_out = PercentDim*bled_out;
     wled_out = PercentDim*wled_out;
     rbled_out = PercentDim*rbled_out;
     rled_out = PercentDim*rled_out;
     uvled_out = PercentDim*uvled_out;
     moonled_out = PercentDim*moonled_out;      
     tempLED=tempH;
     if (dispScreen==0) 
       {myGLCD.drawBitmap(5, 166, 35, 35, HotLEDsFix, 1);}
    } else
  if (tempLED<=tempH+TTF)
    {
     PercentDim = setLEDsDimPercent*0.01;
     bled_out = PercentDim*bled_out;
     wled_out = PercentDim*wled_out;
     rbled_out = PercentDim*rbled_out;
     rled_out = PercentDim*rled_out;
     uvled_out = PercentDim*uvled_out;
     moonled_out = PercentDim*moonled_out;  
     if (dispScreen==0) 
       {myGLCD.drawBitmap(5, 166, 35, 35, HotLEDsFix, 1);}
    }
  else {tempLED=255;}  
  
  if (tempS>=setLEDsDimTempC)                   //Trigger For LED dimming in Sump Light
    {
     PercentDim = setLEDsDimPercent*0.01;
     sled_out = PercentDim*sled_out;
     tempLEDsump=tempS;
     if (dispScreen==0) 
       {myGLCD.drawBitmap(5, 166, 35, 35, HotLEDsFix, 1);}
    } else
  if (tempLEDsump<=tempS+TTF)
    {
     PercentDim = setLEDsDimPercent*0.01;
     sled_out = PercentDim*sled_out;
     if (dispScreen==0) 
       {myGLCD.drawBitmap(5, 166, 35, 35, HotLEDsFix, 1);}
    }
  else {tempLEDsump=255;}    
}
/*************************END OF DIM LEDs WHEN HOT FUNCTION ***************************/


/******************************* LUNAR PHASE FUNCTION *********************************/
float moonPhase(int moonYear, int moonMonth, int moonDay)
{ 
  float phase;
  double IP; 
  long YY, MM, K1, K2, K3, JulianDay; 
  YY = moonYear - floor((12 - moonMonth) / 10); 
  MM = moonMonth + 9;
  if (MM >= 12)
    { MM = MM - 12; }
  K1 = floor(365.25 * (YY + 4712));
  K2 = floor(30.6 * MM + 0.5);
  K3 = floor(floor((YY / 100) + 49) * 0.75) - 38;
  JulianDay = K1 + K2 + moonDay + 59;
  if (JulianDay > 2299160)
    { JulianDay = JulianDay - K3; }
  IP = MyNormalize((JulianDay - 2451550.1) / LC);
  AG = IP*LC;
  phase = 0; 
  
  //Determine the Moon Illumination %
  if ((AG >= 0) && (AG <= LC/2))             //FROM New Moon 0% TO Full Moon 100%
    { phase = (2*AG)/LC; }
  if ((AG > LC/2) && (AG <= LC))             //FROM Full Moon 100% TO New Moon 0%
    { phase = 2*(LC-AG)/LC; }

  //Determine the Lunar Phase
  if ((AG >= 0) && (AG <= 1.85))             //New Moon; ~0-12.5% illuminated
    { LP = "    New Moon   "; 
      MoonPic = New_Moon; }
  if ((AG > 1.85) && (AG <= 5.54))           //New Crescent; ~12.5-37.5% illuminated
    { LP = "Waxing Crescent";
      MoonPic = Waxing_Crescent; }
  if ((AG > 5.54) && (AG <= 9.23))           //First Quarter; ~37.5-62.5% illuminated
    { LP = " First Quarter ";
      MoonPic = First_Quarter; }
  if ((AG > 9.23) && (AG <= 12.92))          //Waxing Gibbous; ~62.5-87.5% illuminated
    { LP = "Waxing Gibbous ";
      MoonPic = Waxing_Gibbous; }
  if ((AG > 12.92) && (AG <= 16.61))         //Full Moon; ~87.5-100-87.5% illuminated
    { LP = "   Full Moon   ";
      MoonPic = Full_Moon; }    
  if ((AG > 16.61) && (AG <= 20.30))         //Waning Gibbous; ~87.5-62.5% illuminated
    { LP = "Waning Gibbous ";
      MoonPic = Waning_Gibbous; }
  if ((AG > 20.30) && (AG <= 23.99))         //Last Quarter; ~62.5-37.5% illuminated
    { LP = " Last Quarter  ";
      MoonPic = Last_Quarter; }
  if ((AG > 23.99) && (AG <= 27.68))         //Old Crescent; ~37.5-12.5% illuminated
    { LP = "Waning Crescent";
      MoonPic = Waning_Crescent; }
  if ((AG >= 27.68) && (AG <= LC))           //New Moon; ~12.5-0% illuminated
    { LP = "    New Moon   ";
      MoonPic = New_Moon; }
        
  return phase; 
}

double MyNormalize(double v) 
{ 
  v = v - floor(v);
  if (v < 0)
    v = v + 1;
  return v;
} 
/**************************** END OF LUNAR PHASE FUNCTION *****************************/


/********************************* MISC. FUNCTIONS ************************************/
void clearScreen()
{
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect(1, 15, 318, 226);
}


void printHeader(char* headline)
{
  setFont(SMALL, 255, 255, 0, 255, 255, 0);
  myGLCD.fillRect (1, 1, 318, 14);
  myGLCD.setColor(0, 0, 0);
  myGLCD.print(headline, CENTER, 1);   
}


void setFont(boolean font, byte cr, byte cg, byte cb, byte br, byte bg, byte bb)
{
  myGLCD.setBackColor(br, bg, bb);               //font background black
  myGLCD.setColor(cr, cg, cb);                   //font color white
  if (font==LARGE)
    myGLCD.setFont(BigFont);                     //font size LARGE
  else if (font==SMALL)
    myGLCD.setFont(SmallFont);
}


void waitForTouchRelease()
{
  while (myTouch.dataAvailable()==true)          //Wait for release
    myTouch.read();
}


int LedToPercent (int Led_out)                   //returns LED output in %
{
  int result;
 
  if (Led_out==0) {
    result = 0; }
  else {
    result = map(Led_out, 1, 255, 1, 100);  }
  
  return result; 
}


void drawBarGraph()
{
  myGLCD.setColor(255, 255, 255);                //LED Chart
  setFont(SMALL, 255, 255, 255, 0, 0, 0);     
   
  myGLCD.drawRect(30, 137, 148, 138);            //x-line
  myGLCD.drawRect(30, 137, 31, 36);              //y-line

  for (int i=0; i<5; i++)                        //tick-marks
    { myGLCD.drawLine(31, (i*20)+36, 35, (i*20)+36); }  
  myGLCD.print("100", 4, 30);  
  myGLCD.print("80", 12, 50);       
  myGLCD.print("60", 12, 70);
  myGLCD.print("40", 12, 90); 
  myGLCD.print("20", 12, 110);
  myGLCD.print("0", 20, 130);
  
  myGLCD.setColor(0, 150, 0);
  myGLCD.drawRect(40, 136, 52, 135);             //SUMP %bar place holder
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRect(57, 136, 69, 135);             //red %bar place holder
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRect(74, 136, 86, 135);             //white %bar place holder
  myGLCD.setColor(9, 184, 255);
  myGLCD.drawRect(91, 136, 103, 135);            //blue %bar place holder
  myGLCD.setColor(58, 95, 205);
  myGLCD.drawRect(108, 136, 120, 135);           //royal %bar place holder
  myGLCD.setColor(224, 102, 255);  
  myGLCD.drawRect(125, 136, 137, 135);           //UV %bar place holder    
}


void drawSliderBarGraph()
{
  myGLCD.setColor(255, 255, 255);            
  setFont(SMALL, 255, 255, 255, 0, 0, 0); 
  
  myGLCD.drawRect(30, 173, 304, 174);         //x-line
  myGLCD.drawRect(30, 173, 31, 44);           //y-line
  
  for (int i=0; i<6; i++)                     //tick-marks
    { myGLCD.drawLine(31, (i*26)+44, 35, (i*26)+44); }    
  
  myGLCD.print("100", 4, 38);  
  myGLCD.print("80", 12, 64);       
  myGLCD.print("60", 12, 90);
  myGLCD.print("40", 12, 116); 
  myGLCD.print("20", 12, 142);
  myGLCD.print("0", 20, 168);     
  
  myGLCD.setColor(0, 150, 0);
  myGLCD.drawRect(39, 44, 69, 172);           //SUMP %bar place holder
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRect(77, 44, 107, 172);          //red %bar place holder
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRect(115, 44, 145, 172);         //white %bar place holder
  myGLCD.setColor(9, 184, 255);
  myGLCD.drawRect(153, 44, 183, 172);         //blue %bar place holder
  myGLCD.setColor(58, 95, 205);
  myGLCD.drawRect(191, 44, 221, 172);         //royal %bar place holder
  myGLCD.setColor(224, 102, 255);  
  myGLCD.drawRect(229, 44, 259, 172);         //UV %bar place holder      
  myGLCD.setColor(176, 176, 176);
  myGLCD.drawRect(267, 44, 297, 172);         //LUNAR %bar place holder   

  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRoundRect(2, 181, 28, 224);
  myGLCD.setColor(34, 177, 76);
  myGLCD.drawRoundRect(2, 181, 28, 224);
  setFont(SMALL, 34, 177, 76, 255, 255, 255); 
  myGLCD.print("E", 13, 182);    
  myGLCD.print("X", 13, 192);
  myGLCD.print("I", 13, 202);
  myGLCD.print("T", 13, 212);
}  


void SliderBars()
{
  int TempY;

  myGLCD.setColor(sbR, sbG, sbB);             //Slider Bar Color
  myGLCD.fillRect(sbX1, y, sbX2, 172);        //draw the bar where you touch
  myGLCD.setColor(0, 0, 0);  
  myGLCD.fillRect(sbX1+1, y, sbX2-1, 45);     //hide the bar
  myGLCD.setColor(sbR, sbG, sbB);             //Slider Bar Color
  myGLCD.drawLine(sbX1, 44, sbX2, 44);        //fills in the top of bar

  setFont(SMALL, sbR, sbG, sbB, 0, 0, 0); 
  if (y<=44) {y=44;}
  if (y>=172) {y=172;} 
  TempY = map(y, 172, 44, 0, 255);
  if (TempY>=255) {TempY=255;}
  if (TempY<=0) {TempY=0;}  
  yStore = TempY;
  if (TempY<10) 
    {myGLCD.print("00", sbX1+5, 186); 
     myGLCD.printNumI(TempY, sbX1+21, 186);}
  if ((TempY>=10)&&(TempY<100)) 
    {myGLCD.print("0", sbX1+5, 186); 
     myGLCD.printNumI(TempY, sbX1+13, 186);}
  if (TempY>=100) {myGLCD.printNumI(TempY, sbX1+5, 186);}
  
  for (int i=0; i<7; i++) 
    {
     if ((x>=(i*38)+39) && (x<=(i*38)+69)) 
       {
        if (i==0) {ySMP=TempY; scol_out=ySMP;}
        if (i==1) {yRED=TempY; rcol_out=yRED;}
        if (i==2) {yWHT=TempY; wcol_out=yWHT;}
        if (i==3) {yBLU=TempY; bcol_out=yBLU;}
        if (i==4) {yRBL=TempY; rbcol_out=yRBL;}
        if (i==5) {yUVL=TempY; uvcol_out=yUVL;}
        if (i==6) {yLUN=TempY; mooncol_out=yLUN;}
       }  
    }
  LED_levels_output();
}


void SliderBarsForChange()
{
  int TempY;

  myGLCD.setColor(sbR, sbG, sbB);             //Slider Bar Color
  myGLCD.fillRect(sbX1, y, sbX2, 185);        //draw the bar where you touch
  myGLCD.setColor(0, 0, 0);  
  myGLCD.fillRect(sbX1+1, y, sbX2-1, 72);     //hide the bar
  myGLCD.setColor(sbR, sbG, sbB);             //Slider Bar Color
  myGLCD.drawLine(sbX1, 71, sbX2, 71);        //fills in the top of bar

  setFont(SMALL, sbR, sbG, sbB, 0, 0, 0); 

  if (y<=71) {y=71;}
  if (y>=185) {y=185;} 
  TempY = map(y, 185, 71, 0, 255);
  tSlide=TempY;
  if (TempY>=255) {TempY=255;}
  if (TempY<=0) {TempY=0;}  
  if (TempY<10) 
    {myGLCD.print("00", sbX1+5, 187); 
     myGLCD.printNumI(TempY, sbX1+21, 187);}
  if ((TempY>=10)&&(TempY<100)) 
    {myGLCD.print("0", sbX1+5, 187); 
     myGLCD.printNumI(TempY, sbX1+13, 187);}
  if (TempY>=100) {myGLCD.printNumI(TempY, sbX1+5, 187);}

  for (int i=0; i<8; i++) 
    {
     if ((x>=(i*36)+34) && (x<=(i*36)+64)) 
       {tled[k]=TempY;}
    }  
}

  
void UpDnButtonSlide()
{
  int yTemp;

  if (ySMP>=255) {ySMP=255;} if (ySMP<=0) {ySMP=0;}
  if (yRED>=255) {yRED=255;} if (yRED<=0) {yRED=0;}
  if (yWHT>=255) {yWHT=255;} if (yWHT<=0) {yWHT=0;}
  if (yBLU>=255) {yBLU=255;} if (yBLU<=0) {yBLU=0;}
  if (yRBL>=255) {yRBL=255;} if (yRBL<=0) {yRBL=0;}
  if (yUVL>=255) {yUVL=255;} if (yUVL<=0) {yUVL=0;}
  if (yLUN>=255) {yLUN=255;} if (yLUN<=0) {yLUN=0;}

  for (int i=0; i<7; i++) 
    {
     if ((x>=(i*38)+39) && (x<=(i*38)+69)) 
       {
        if (i==0) {yStore=ySMP; scol_out=ySMP;}
        if (i==1) {yStore=yRED; rcol_out=yRED;}
        if (i==2) {yStore=yWHT; wcol_out=yWHT;}
        if (i==3) {yStore=yBLU; bcol_out=yBLU;}
        if (i==4) {yStore=yRBL; rbcol_out=yRBL;}
        if (i==5) {yStore=yUVL; uvcol_out=yUVL;}
        if (i==6) {yStore=yLUN; mooncol_out=yLUN;}
       }  
    }    
  if (yStore>=255) {yStore=255;}
  if (yStore<=0) {yStore=0;}      
  setFont(SMALL, sbR, sbG, sbB, 0, 0, 0); 
  myGLCD.print("    ", sbX1+5, 186);
  if (yStore<10) 
    {myGLCD.print("00", sbX1+5, 186); 
     myGLCD.printNumI(yStore, sbX1+21, 186);}
  if ((yStore>=10)&&(yStore<100)) 
    {myGLCD.print("0", sbX1+5, 186); 
     myGLCD.printNumI(yStore, sbX1+13, 186);}
  if (yStore>=100) {myGLCD.printNumI(yStore, sbX1+5, 186);}

  yTemp=map(yStore, 0, 255, 172, 44);
  myGLCD.setColor(sbR, sbG, sbB);             //Bar Color
  myGLCD.fillRect(sbX1, yTemp, sbX2, 172);    //draw the bar from where it was last touched
  myGLCD.setColor(0, 0, 0);  
  myGLCD.fillRect(sbX1+1, yTemp, sbX2-1, 45); //hide the bar
  myGLCD.setColor(sbR, sbG, sbB);             //Bar Color
  myGLCD.drawLine(sbX1, 44, sbX2, 44);        //fills in the top of bar
  
  LED_levels_output();
}


void TimeSaver(boolean refreshAll=false)
{
  if (setTimeFormat==0)                                 //24HR Format
    { 
     myGLCD.setColor(0, 0, 255);
     myGLCD.setBackColor(0, 0, 0);  
     myGLCD.setFont(SevenSegNumFont);
     myGLCD.print(rtc.getTimeStr(FORMAT_SHORT), CENTER, 95);
    }

  if (setTimeFormat==1)                                 //12HR Format
    {
     myGLCD.setColor(0, 0, 255);
     myGLCD.setBackColor(0, 0, 0);  
     myGLCD.setFont(SevenSegNumFont);
     if (t.hour==0)                                     //Display HOUR
       { myGLCD.print("12", 61, 95);}
     if ((t.hour>=1) && (t.hour<=9))
       { myGLCD.setColor(0, 0, 0);
         myGLCD.fillRect(61, 95, 92, 145);
         myGLCD.setColor(0, 0, 255);
         myGLCD.printNumI(t.hour, 93, 95);}
     if ((t.hour>=10) && (t.hour<=12))
       { myGLCD.printNumI(t.hour, 61, 95);}
     if ((t.hour>=13) && (t.hour<=21))
       { myGLCD.setColor(0, 0, 0);
         myGLCD.fillRect(61, 95, 92, 145);
         myGLCD.setColor(0, 0, 255);
         myGLCD.printNumI(t.hour-12, 93, 95);}
     if (t.hour>=22)
       { myGLCD.printNumI(t.hour-12, 61, 95);}
      
     myGLCD.setColor(0, 0, 255);
     myGLCD.setBackColor(0, 0, 0);  
     myGLCD.setFont(SevenSegNumFont);
     myGLCD.fillCircle(141, 108, 4);
     myGLCD.fillCircle(141, 132, 4);  
     if ((t.min>=0) && (t.min<=9))                      //Display MINUTES
       { myGLCD.print("0", 157, 95);
         myGLCD.printNumI(t.min, 189, 95);}
     else { myGLCD.printNumI(t.min, 157, 95);}             

     if ((t.hour>=0) && (t.hour<=11))                   //Display AM/PM
       { setFont(LARGE, 0, 0, 255, 0, 0, 0);
         myGLCD.print("AM", 225, 99);}
     else
       { setFont(LARGE, 0, 0, 255, 0, 0, 0);
         myGLCD.print("PM", 225, 99);} 
    }

  if (setScreensaverDOWonOff==1)                        //Date and Date
    {
     t.dow=calcDOW(t.date, t.mon, t.year);                
     if (setTimeFormat==0)
       { 
        SS_DOW_x=122;
        showDOW_ScreenSaver(t.dow);
        setFont(SMALL, 0, 0, 255, 0, 0, 0); 
        if (setCalendarFormat==0)
          {            
           if (t.date<=9) 
             { myGLCD.printNumI(t.date, 162, 157);
               myGLCD.print(" ", 170, 157);
               myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 178, 157);  //3 letter Month
               myGLCD.print(" ", 202, 157);
             }
           else 
             { myGLCD.printNumI(t.date, 162, 157);    
               myGLCD.print(" ", 178, 157);             
               myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 186, 157);}  
          }
        else
          {            
           myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 162, 157);    //3 letter Month
           myGLCD.printNumI(t.date, 194, 157);
           if (t.date<=9) {myGLCD.print(" ", 202, 157);}
          }
       }
     if (setTimeFormat==1)
       { 
        if (((t.hour>=1)&&(t.hour<=9)) || ((t.hour>=13) && (t.hour<=21)))
          {
           myGLCD.print("  ", 105, 157);
           SS_DOW_x=122;
           showDOW_ScreenSaver(t.dow);
           setFont(SMALL, 0, 0, 255, 0, 0, 0); 
           if (setCalendarFormat==0)
             {            
              if (t.date<=9) 
                { myGLCD.printNumI(t.date, 162, 157);
                  myGLCD.print(" ", 170, 157);
                  myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 178, 157);  //3 letter Month
                  myGLCD.print(" ", 202, 157);
                }
              else 
                { myGLCD.printNumI(t.date, 162, 157);
                  myGLCD.print(" ", 178, 157);
                  myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 186, 157);
                  myGLCD.print(" ", 210, 157);                
                }  
             }
           else
             {
              myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 162, 157);    //3 letter Month
              myGLCD.print(" ", 186, 157);
              myGLCD.printNumI(t.date, 194, 157);
              if (t.date<=9) {myGLCD.print(" ", 202, 157);}
             }
          }
        else
          {
           SS_DOW_x=106;
           showDOW_ScreenSaver(t.dow);
           setFont(SMALL, 0, 0, 255, 0, 0, 0); 
           
           if (setCalendarFormat==0)
             {            
              if (t.date<=9) 
                { myGLCD.printNumI(t.date, 146, 157);
                  myGLCD.print(" ", 154, 157);
                  myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 162, 157);  //3 letter Month
                  myGLCD.print(" ", 186, 157);                                    
                }
              else 
                { myGLCD.printNumI(t.date, 146, 157);
                  myGLCD.print(" ", 162, 157);
                  myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 170, 157);
                  myGLCD.print(" ", 194, 157);                  
                }  
             }
           else
             {           
              myGLCD.print(rtc.getMonthStr(FORMAT_SHORT), 146, 157);    //3 letter Month
              myGLCD.print(" ", 170, 157);
              myGLCD.printNumI(t.date, 178, 157); 
              if (t.date<=9) {myGLCD.print(" ", 186, 157);}
              myGLCD.print("   ", 194, 157);           
             }
          }
       }
    }
}


void showDOW_ScreenSaver(byte dow)
{
  char* str[] = {"MON, ","TUE, ","WED, ","THU, ","FRI, ","SAT, ","SUN, "};
  
  setFont(SMALL, 0, 0, 255, 0, 0, 0); 
  myGLCD.print(str[dow-1], SS_DOW_x, 157);
}


void screenSaver()                               //Make the Screen Go Blank after so long
{
  setScreenSaverTimer = setSSmintues * 12;
  if ((setScreensaverOnOff==1) && (tempAlarmflag==false))
    {
     if (myTouch.dataAvailable())
       { processMyTouch();} 
     else { screenSaverCounter++;}
     if (screenSaverCounter==setScreenSaverTimer) 
       { dispScreen=0;
         myGLCD.clrScr(); }
     if (setClockOrBlank==0)
       { if (screenSaverCounter>setScreenSaverTimer) 
           { dispScreen=0;
             TimeSaver(true); }
       } 
    }
}


void ScreensaverSelect()
{
  if (setClockOrBlank==0)                            //Choose Screensaver Buttons
    { myGLCD.setColor(0, 0, 255);
      myGLCD.fillRoundRect(185, 20, 235, 40);
      setFont(SMALL, 255, 255, 255, 0, 0, 255);  
      myGLCD.print("BLANK", 191, 24);    
      myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(255, 20, 305, 40);      
      setFont(SMALL, 0, 0, 0, 0, 255, 0);  
      myGLCD.print("CLOCK", 261, 24);
      myGLCD.setColor(64, 64, 64);
      myGLCD.drawLine(0, 76, 319, 76);   
      setFont(SMALL, 0, 255, 0, 0, 0, 0);
      myGLCD.print("Show Date w/ Clock", 25, 55);        
    }
    else 
      { myGLCD.setColor(0, 255, 0);
        myGLCD.fillRoundRect(185, 20, 235, 40);
        setFont(SMALL, 0, 0, 0, 0, 255, 0);  
        myGLCD.print("BLANK", 191, 24);    
        myGLCD.setColor(0, 0, 255);
        myGLCD.fillRoundRect(255, 20, 305, 40);    
        setFont(SMALL, 255, 255, 255, 0, 0, 255);  
        myGLCD.print("CLOCK", 261, 24);
        myGLCD.setColor(0, 0, 0);
        myGLCD.fillRect(1, 47, 318, 77);         
      }

  if (setClockOrBlank==0)
    {
     if (setScreensaverDOWonOff==0)                  //Show Date on Screensaver
       { myGLCD.setColor(0, 0, 255);
         myGLCD.fillRoundRect(185, 51, 235, 71);
         setFont(SMALL, 255, 255, 255, 0, 0, 255);  
         myGLCD.print("YES", 198, 55);    
         myGLCD.setColor(0, 255, 0);
         myGLCD.fillRoundRect(255, 51, 305, 71);      
         setFont(SMALL, 0, 0, 0, 0, 255, 0);  
         myGLCD.print("NO", 272, 55);   
       }
       else 
         { myGLCD.setColor(0, 255, 0);
           myGLCD.fillRoundRect(185, 51, 235, 71);
           setFont(SMALL, 0, 0, 0, 0, 255, 0);  
           myGLCD.print("YES", 198, 55);    
           myGLCD.setColor(0, 0, 255);
           myGLCD.fillRoundRect(255, 51, 305, 71);    
           setFont(SMALL, 255, 255, 255, 0, 0, 255);  
           myGLCD.print("NO", 272, 55); 
         }
     myGLCD.setColor(255, 255, 255);
     myGLCD.drawRoundRect(185, 51, 235, 71);
     myGLCD.drawRoundRect(255, 51, 305, 71);           
    }
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(185, 20, 235, 40);
  myGLCD.drawRoundRect(255, 20, 305, 40);        
}


void genSetSelect_1()
{
  if (setCalendarFormat==0)                      //Calendar Format Buttons
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(185, 19, 305, 39);
      setFont(SMALL, 0, 0, 0, 0, 255, 0);  
      myGLCD.print("DD/MM/YYYY", 207, 23);
      myGLCD.setColor(0, 0, 255);
      myGLCD.fillRoundRect(185, 45, 305, 65);    
      setFont(SMALL, 255, 255, 255, 0, 0, 255);  
      myGLCD.print("MTH DD, YYYY", 199, 49);   
    }
    else 
      { myGLCD.setColor(0, 0, 255);
        myGLCD.fillRoundRect(185, 19, 305, 39);
        setFont(SMALL, 255, 255, 255, 0, 0, 255);  
        myGLCD.print("DD/MM/YYYY", 207, 23);
        myGLCD.setColor(0, 255, 0);
        myGLCD.fillRoundRect(185, 45, 305, 65);    
        setFont(SMALL, 0, 0, 0, 0, 255, 0);  
        myGLCD.print("MTH DD, YYYY", 199, 49);   
      }
  
  if (displayDOW==1)                             //Show/Hide the Day of the Week 
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(195, 76, 235, 96);
      setFont(SMALL, 0, 0, 0, 0, 255, 0);  
      myGLCD.print("ON", 209, 80);    
      myGLCD.setColor(0, 0, 255);
      myGLCD.fillRoundRect(255, 76, 295, 96);    
      setFont(SMALL, 255, 255, 255, 0, 0, 255);  
      myGLCD.print("OFF", 265, 80); 
    }
    else 
      { myGLCD.setColor(0, 0, 255);
        myGLCD.fillRoundRect(195, 76, 235, 96);
        setFont(SMALL, 255, 255, 255, 0, 0, 255);  
        myGLCD.print("ON", 209, 80);    
        myGLCD.setColor(0, 255, 0);
        myGLCD.fillRoundRect(255, 76, 295, 96);      
        setFont(SMALL, 0, 0, 0, 0, 255, 0);  
        myGLCD.print("OFF", 265, 80); 
      }  
      
  if (setTimeFormat==0)                          //Time Format Buttons
    { myGLCD.setColor(0, 0, 255);
      myGLCD.fillRoundRect(195, 107, 235, 127);
      setFont(SMALL, 255, 255, 255, 0, 0, 255);  
      myGLCD.print("12HR", 201, 111);    
      myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(255, 107, 295, 127);      
      setFont(SMALL, 0, 0, 0, 0, 255, 0);  
      myGLCD.print("24HR", 261, 111);   
    }
    else 
      { myGLCD.setColor(0, 255, 0);
        myGLCD.fillRoundRect(195, 107, 235, 127);
        setFont(SMALL, 0, 0, 0, 0, 255, 0);  
        myGLCD.print("12HR", 201, 111);    
        myGLCD.setColor(0, 0, 255);
        myGLCD.fillRoundRect(255, 107, 295, 127);    
        setFont(SMALL, 255, 255, 255, 0, 0, 255);  
        myGLCD.print("24HR", 261, 111); 
      }
      
  if (setTempScale==0)                           //Temperature Scale Buttons
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(195, 138, 235, 158);
      setFont(SMALL, 0, 0, 0, 0, 255, 0);  
      myGLCD.print("C", 215, 142);   
      myGLCD.setColor(0, 0, 255);
      myGLCD.fillRoundRect(255, 138, 295, 158);    
      setFont(SMALL, 255, 255, 255, 0, 0, 255);  
      myGLCD.print("F", 275, 142);
      myGLCD.setColor(0, 0, 0);   
      myGLCD.drawCircle(210, 144, 1);              
      myGLCD.setColor(255, 255, 255);
      myGLCD.drawCircle(270, 144, 1);
    }
    else 
      { myGLCD.setColor(0, 0, 255);
        myGLCD.fillRoundRect(195, 138, 235, 158);
        setFont(SMALL, 255, 255, 255, 0, 0, 255);  
        myGLCD.print("C", 215, 142);    
        myGLCD.setColor(0, 255, 0);
        myGLCD.fillRoundRect(255, 138, 295, 158);      
        setFont(SMALL, 0, 0, 0, 0, 255, 0);  
        myGLCD.print("F", 275, 142);
        myGLCD.setColor(255, 255, 255);
        myGLCD.drawCircle(210, 144, 1);              
        myGLCD.setColor(0, 0, 0);  
        myGLCD.drawCircle(270, 144, 1);
      }  

  myGLCD.setColor(0, 0, 255);                    //Change Fan Startup Temps Button
  myGLCD.fillRoundRect(185, 169, 305, 189);
  setFont(SMALL, 255, 255, 255, 0, 0, 255);  
  myGLCD.print("CHANGE TEMPS", 199, 173);

  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(185, 169, 305, 189);    
  myGLCD.drawRoundRect(185, 19, 305, 39);
  myGLCD.drawRoundRect(185, 45, 305, 65);
  for (int x=0; x<2; x++)
    { for (int y=0; y<3; y++)
        { myGLCD.drawRoundRect((x*60)+195, (y*31)+76, (x*60)+235, (y*31)+96); }
    }
}    


void genSetSelect_2()
{
  if (setDimLEDsOnOff==1)                        //Dim LEDs Temp Buttons
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(195, 27, 235, 47);
      setFont(SMALL, 0, 0, 0, 0, 255, 0);  
      myGLCD.print("ON", 209, 31);    
      myGLCD.setColor(0, 0, 255);
      myGLCD.fillRoundRect(255, 27, 295, 47);    
      setFont(SMALL, 255, 255, 255, 0, 0, 255);  
      myGLCD.print("OFF", 265, 31); 
    }
    else 
      { myGLCD.setColor(0, 0, 255);
        myGLCD.fillRoundRect(195, 27, 235, 47);
        setFont(SMALL, 255, 255, 255, 0, 0, 255);  
        myGLCD.print("ON", 209, 31);    
        myGLCD.setColor(0, 255, 0);
        myGLCD.fillRoundRect(255, 27, 295, 47);      
        setFont(SMALL, 0, 0, 0, 0, 255, 0);  
        myGLCD.print("OFF", 265, 31); 
      }  
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect(185, 55, 305, 75);
  setFont(SMALL, 255, 255, 255, 0, 0, 255);  
  myGLCD.print("CHANGE TEMP", 203, 59);
 
  if (setScreensaverOnOff==1)                         //Screensaver Buttons
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(195, 101, 235, 121);
      setFont(SMALL, 0, 0, 0, 0, 255, 0);  
      myGLCD.print("ON", 209, 105);    
      myGLCD.setColor(0, 0, 255);
      myGLCD.fillRoundRect(255, 101, 295, 121);    
      setFont(SMALL, 255, 255, 255, 0, 0, 255);  
      myGLCD.print("OFF", 265, 105); 
    }
    else 
      { myGLCD.setColor(0, 0, 255);
        myGLCD.fillRoundRect(195, 101, 235, 121);
        setFont(SMALL, 255, 255, 255, 0, 0, 255);  
        myGLCD.print("ON", 209, 105);    
        myGLCD.setColor(0, 255, 0);
        myGLCD.fillRoundRect(255, 101, 295, 121);      
        setFont(SMALL, 0, 0, 0, 0, 255, 0);  
        myGLCD.print("OFF", 265, 105); 
      }  
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect(185, 129, 305, 149);
  setFont(SMALL, 255, 255, 255, 0, 0, 255);  
  myGLCD.print("SETTINGS", 215, 133);
      
  if (setAutoStop==1)                            //Auto-Stop on Feed Buttons
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(195, 169, 235, 189);
      setFont(SMALL, 0, 0, 0, 0, 255, 0);  
      myGLCD.print("ON", 209, 173);    
      myGLCD.setColor(0, 0, 255);
      myGLCD.fillRoundRect(255, 169, 295, 189);    
      setFont(SMALL, 255, 255, 255, 0, 0, 255);  
      myGLCD.print("OFF", 265, 173); 
    }
    else 
      { myGLCD.setColor(0, 0, 255);
        myGLCD.fillRoundRect(195, 169, 235, 189);
        setFont(SMALL, 255, 255, 255, 0, 0, 255);  
        myGLCD.print("ON", 209, 173);    
        myGLCD.setColor(0, 255, 0);
        myGLCD.fillRoundRect(255, 169, 295, 189);      
        setFont(SMALL, 0, 0, 0, 0, 255, 0);  
        myGLCD.print("OFF", 265, 173); 
      }  
      
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(195, 27, 235, 47);
  myGLCD.drawRoundRect(255, 27, 295, 47);    
  myGLCD.drawRoundRect(185, 55, 305, 75);
  myGLCD.drawRoundRect(195, 101, 235, 121);
  myGLCD.drawRoundRect(255, 101, 295, 121);    
  myGLCD.drawRoundRect(185, 129, 305, 149);
  myGLCD.drawRoundRect(195, 169, 235, 189);
  myGLCD.drawRoundRect(255, 169, 295, 189);
}  


void feedingTimeOnOff()
{
  if ((feedTime==1) && (FEEDTime1==1))
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(70, 150, 250, 170); 
      setFont(SMALL, 0, 0, 0, 0, 255, 0);
      myGLCD.print("Feeding Time 1 ON", 94, 154);}
  if ((feedTime==1) && (FEEDTime1==0))
    { myGLCD.setColor(255, 0, 0);
      myGLCD.fillRoundRect(70, 150, 250, 170); 
      setFont(SMALL, 255, 255, 255, 255, 0, 0);
      myGLCD.print("Feeding Time 1 OFF", 90, 154);}
  if ((feedTime==2) && (FEEDTime2==1))
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(70, 150, 250, 170); 
      setFont(SMALL, 0, 0, 0, 0, 255, 0);
      myGLCD.print("Feeding Time 2 ON", 94, 154);}
  if ((feedTime==2) && (FEEDTime2==0))
    { myGLCD.setColor(255, 0, 0);
      myGLCD.fillRoundRect(70, 150, 250, 170); 
      setFont(SMALL, 255, 255, 255, 255, 0, 0);
      myGLCD.print("Feeding Time 2 OFF", 90, 154);}
  if ((feedTime==3) && (FEEDTime3==1))
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(70, 150, 250, 170); 
      setFont(SMALL, 0, 0, 0, 0, 255, 0);
      myGLCD.print("Feeding Time 3 ON", 94, 154);}
  if ((feedTime==3) && (FEEDTime3==0))
    { myGLCD.setColor(255, 0, 0);
      myGLCD.fillRoundRect(70, 150, 250, 170); 
      setFont(SMALL, 255, 255, 255, 255, 0, 0);
      myGLCD.print("Feeding Time 3 OFF", 90, 154);}
  if ((feedTime==4) && (FEEDTime4==1))
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(70, 150, 250, 170); 
      setFont(SMALL, 0, 0, 0, 0, 255, 0);
      myGLCD.print("Feeding Time 4 ON", 94, 154);}
  if ((feedTime==4) && (FEEDTime4==0))
    { myGLCD.setColor(255, 0, 0);
      myGLCD.fillRoundRect(70, 150, 250, 170); 
      setFont(SMALL, 255, 255, 255, 255, 0, 0);
      myGLCD.print("Feeding Time 4 OFF", 90, 154);}

  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(70, 150, 250, 170);   
}
/******************************* END OF MISC. FUNCTIONS *******************************/


/*********************** MAIN SCREEN ********** dispScreen = 0 ************************/
void mainScreen(boolean refreshAll=false)
{
  int ledLevel, bar;
  String oldval, deg;  
  
  TimeDateBar(true);
  
  oldval = day;                                  //refresh day if different
  day = String(t.date);
  if ((oldval!=day) || refreshAll)
    {
     myGLCD.setColor(64, 64, 64);                //Draw Borders & Dividers in Grey
     myGLCD.drawRect(0, 0, 319, 239);            //Outside Border
     myGLCD.drawRect(158, 14, 160, 226);         //Vertical Divider
     myGLCD.drawRect(160, 125, 319, 127);        //Horizontal Divider
     myGLCD.fillRect(0, 0, 319, 14);             //Top Bar
     myGLCD.setColor(0, 0, 0);
     myGLCD.drawLine(159, 126, 161, 126);        //Horizontal Divider Separator     
     setFont(SMALL, 255, 255, 0, 64, 64, 64);
     myGLCD.print("Jarduino Aquarium Controller v.1.2", CENTER, 1);
     setFont(SMALL, 255, 255, 255, 0, 0, 0);
     myGLCD.print("LED ARRAY", 52, 20);
     myGLCD.print("LUNAR PHASE", 196, 20); 
     myGLCD.print("MONITORS & ALERTS", 174, 133);
     
     float lunarCycle = moonPhase(t.year, t.mon, t.date); //get a value for the lunar cycle
     myGLCD.drawBitmap(214, 38, 52, 52, MoonPic, 1);      //Moon Phase Picture (middle 240,64)
     setFont(SMALL, 176, 176, 176, 0, 0, 0);            
     char bufferLP[16];
     LP.toCharArray(bufferLP, 16);
     myGLCD.print(bufferLP, 180, 96);            //Print Moon Phase Description to LCD
     if ((lunarCycle*100) < 1)                   //Print % of Full to LCD
       { myGLCD.print(" 0.0", 188, 108); }
       else { myGLCD.printNumF(lunarCycle*100, 1, 188, 108);}
     myGLCD.print("% of Full", 220, 108);     
    }

  drawBarGraph();    
  if ((sumpLed!=sled_out) || refreshAll)         //refresh red led display
   {
    sumpLed = sled_out;
    ledLevel = LedToPercent(sled_out);
    oldval = "SUMP:   " + String(ledLevel) + "%  " + "  ";
    char bufferS[13];
    oldval.toCharArray(bufferS, 13);
    bar= 136 - sled_out*.39;
      
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(40, bar, 52, 36);            //hide end of last bar
    myGLCD.setColor(0, 150, 0);
    setFont(SMALL, 0, 150, 0, 0, 0, 0);
    myGLCD.print(bufferS, 45, 147);              //display SUMP LEDs % output
    myGLCD.drawRect(40, 136, 52, 135);           //SUMP %bar place holder
    myGLCD.fillRect(40, 136, 52, bar);           //SUMP percentage bar
   }    

  if ((redLed!=rled_out) || refreshAll)          //refresh red led display
   {
    redLed = rled_out;
    ledLevel = LedToPercent(rled_out);
    oldval = "Red:    " + String(ledLevel) + "%  " + "  ";
    char bufferR[13];
    oldval.toCharArray(bufferR, 13);
    bar= 136 - rled_out*.39;
      
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(57, bar, 69, 36);            //hide end of last bar
    myGLCD.setColor(255, 0, 0);
    setFont(SMALL, 255, 0, 0, 0, 0, 0);
    myGLCD.print(bufferR, 45, 159);              //display red LEDs % output
    myGLCD.drawRect(57, 136, 69, 135);           //red %bar place holder
    myGLCD.fillRect(57, 136, 69, bar);           //red percentage bar
   }    
        
  if ((whiteLed!=wled_out) || refreshAll)        //refresh white led display
   {
    whiteLed = wled_out;
    ledLevel = LedToPercent(wled_out);
    oldval = "White:  " + String(ledLevel) + "%  " + "  ";
    char bufferW[13];
    oldval.toCharArray(bufferW, 13);
    bar= 136 - wled_out*.39;
      
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(74, bar, 86, 36);            //hide end of last bar
    myGLCD.setColor(255, 255, 255);
    setFont(SMALL, 255, 255, 255, 0, 0, 0);
    myGLCD.print(bufferW, 45, 171);              //display white LEDs % output
    myGLCD.drawRect(74, 136, 86, 135);           //white %bar place holder
    myGLCD.fillRect(74, 136, 86, bar);           //white percentage bar
   }
     
  if ((blueLed!=bled_out) || refreshAll)         //refresh blue led displays
   {
    blueLed = bled_out;
    ledLevel = LedToPercent(bled_out);
    oldval = "Blue:   " + String(ledLevel) + "%" + "  ";
    char bufferB[13];
    oldval.toCharArray(bufferB, 13);
    bar= 136 - bled_out*.39;
     
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(91, bar, 103, 36);           //hide end of last bar 
    myGLCD.setColor(9, 184, 255);
    setFont(SMALL, 99, 184, 255, 0, 0, 0);
    myGLCD.print(bufferB, 45, 183);              //display blue LEDs % output
    myGLCD.drawRect(91, 136, 103, 135);          //blue %bar place holder
    myGLCD.fillRect(91, 136, 103, bar);          //blue percentage bar
   }

  if ((rblueLed!=rbled_out) || refreshAll)       //refresh royal blue led display
   {
    rblueLed = rbled_out;
    ledLevel = LedToPercent(rbled_out);
    oldval = "Royal:  " + String(ledLevel) + "%  " + "  ";
    char bufferRB[13];
    oldval.toCharArray(bufferRB, 13);
    bar= 136 - rbled_out*.39;
      
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(108, bar, 120, 36);          //hide end of last bar
    myGLCD.setColor(58, 95, 205);
    setFont(SMALL, 58, 95, 205, 0, 0, 0);
    myGLCD.print(bufferRB, 45, 195);             //display royal blue LEDs % output
    myGLCD.drawRect(108, 136, 120, 135);         //royal %bar place holder
    myGLCD.fillRect(108, 136, 120, bar);         //royal percentage bar
   }
    
  if ((uvLed!=uvled_out) || refreshAll)          //refresh UV led display
   {
    uvLed = uvled_out;
    ledLevel = LedToPercent(uvled_out);
    oldval = "Ultra:  " + String(ledLevel) + "%  " + "  ";
    char bufferUV[13];
    oldval.toCharArray(bufferUV, 13);
    bar= 136 - uvled_out*.39;
      
    myGLCD.setColor(0, 0, 0);
    myGLCD.fillRect(125, bar, 137, 36);          //hide end of last bar
    myGLCD.setColor(224, 102, 255);
    setFont(SMALL, 224, 102, 255, 0, 0, 0);
    myGLCD.print(bufferUV, 45, 207);             //display UV LEDs % output
    myGLCD.drawRect(125, 136, 137, 135);         //UV %bar place holder
    myGLCD.fillRect(125, 136, 137, bar);         //UV percentage bar
   }

  if (setTempScale==1) {deg ="F";}               //Print deg C or deg F
    else {deg = "C";}
  degC_F=deg;
  char bufferDeg[2];
  degC_F.toCharArray(bufferDeg,2);
                                  
  if (refreshAll)                                //draw static elements
   {
    setFont(SMALL, 0, 255, 0, 0, 0 , 0);
    myGLCD.print("Water Temp:", 169, 148);
    myGLCD.drawCircle(304, 150, 1);              
    myGLCD.print(bufferDeg, 309, 148);

    myGLCD.print("Hood Temp:", 169 , 161);
    myGLCD.drawCircle(304, 163, 1);              
    myGLCD.print(bufferDeg, 309, 161);
     
    myGLCD.print("Sump Temp:", 169, 174);
    myGLCD.drawCircle(304, 176, 1);              
    myGLCD.print(bufferDeg, 309, 174); 
   }
  
  myGLCD.setColor(0, 0, 0);                      //clear cooler/heater & alarm notices
  myGLCD.fillRect(200, 189, 303, 203);
  myGLCD.fillRect(182, 203, 315, 221);

  if ((tempW>50) || (tempW<10))                  //range in deg C no matter what
    { setFont(SMALL, 255, 0, 0, 0, 0, 0);
      myGLCD.print("Error", 260, 148);}
  else 
    {
     if (setTempScale==1) 
       { tempW = ((tempW*1.8) + 32.05);}         //C to F with rounding
     if (tempCoolflag==true)                     //Water temperature too HIGH
       {
        setFont(SMALL, 255, 0, 0, 0, 0, 0);
        myGLCD.printNumF( tempW, 1, 260, 148);
        setFont(SMALL, 255, 255, 0, 0, 0, 0);
        myGLCD.print("Chiller ON", 200, 191);}
        else 
          if (tempHeatflag==true)                      //Water temperature too LOW
            { setFont(SMALL, 0, 0, 255, 0, 0, 0);
              myGLCD.printNumF( tempW, 1, 260, 148);
              setFont(SMALL, 255, 255, 0, 0, 0, 0);
              myGLCD.print("Heater ON", 203, 191);}
          else 
            { setFont(SMALL, 0, 255, 0, 0, 0, 0);
              myGLCD.printNumF( tempW, 1, 260, 148);} 
     if ((tempW<100) && (tempW>=0))
       { myGLCD.setColor(0, 0, 0);
         myGLCD.fillRect(292, 148, 300, 160);}
    }          
     
  if ((tempH>50) || (tempH<10))
    { setFont(SMALL, 255, 0, 0, 0, 0, 0);
      myGLCD.print("Error", 260, 161);}
  else
    {
     if (setTempScale==1) 
       { tempH = ((tempH*1.8) + 32.05);}      
     setFont(SMALL, 0, 255, 0, 0, 0, 0);
     myGLCD.printNumF( tempH, 1, 260, 161);            //Hood temperature (No Flags)
     if ((tempH<100) && (tempH>=0)) 
       { myGLCD.setColor(0, 0, 0);
         myGLCD.fillRect(292, 161, 300, 173);}
    }     
         
  if ((tempS>50) || (tempS<10))
    { setFont(SMALL, 255, 0, 0, 0, 0, 0);
      myGLCD.print("Error", 260, 174);}
  else
    {
     if (setTempScale==1) 
       { tempS = ((tempS*1.8) + 32.05); }
     setFont(SMALL, 0, 255, 0, 0, 0, 0);
     myGLCD.printNumF( tempS, 1, 260, 174);            //Sump temperature (No Flags)         
     if ((tempS<100) && (tempS>=0))
       { myGLCD.setColor(0, 0, 0);
         myGLCD.fillRect(292, 174, 300, 186);}
    }
    
  if ((tempAlarmflag==true)&&(tempHeatflag==true))     //Alarm: H20 temp Below offsets
    { setFont(LARGE, 0, 0, 255, 0, 0, 0);
      myGLCD.print("ALARM!!", 185, 204);}    
  if ((tempAlarmflag==true)&&(tempCoolflag==true))     //Alarm: H20 temp Above offsets
    { setFont(LARGE, 255, 0, 0, 0, 0, 0);
      myGLCD.print("ALARM!!", 185, 204);}
}
/******************************** END OF MAIN SCREEN **********************************/


void screenReturn()                                    //Auto Return to MainScreen()
{
  setReturnTimer = setScreenSaverTimer * .75;
  if (SCREEN_RETURN==true)
  {
    if (dispScreen!=0)
    {
      if (myTouch.dataAvailable())
      { processMyTouch();} 
      else { returnTimer++; }
      if (returnTimer>setReturnTimer) 
      {
        returnTimer=0;
        LEDtestTick = false;       
        colorLEDtest = false;           
        ReadFromEEPROM();
        dispScreen=0;
        clearScreen();
        mainScreen(true);
      }
    }
  } 
}


/*********************** MENU SCREEN ********** dispScreen = 1 ************************/
void menuScreen()
{
  printHeader("Choose Option");
  
  myGLCD.setColor(64, 64, 64);
  myGLCD.drawRect(0, 196, 319, 194);
  printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);  
  
  printButton("Time and Date", tanD[0], tanD[1], tanD[2], tanD[3]);
  printButton("H2O Temp Control", temC[0], temC[1], temC[2], temC[3]);
  printButton("WaveMaker", wave[0], wave[1], wave[2], wave[3]);
  printButton("General Settings", gSet[0], gSet[1], gSet[2], gSet[3]);    
  printButton("LED Testing", tesT[0], tesT[1], tesT[2], tesT[3]);
  printButton("Change LED Values", ledChM[0], ledChM[1], ledChM[2], ledChM[3]);
  printButton("Automatic Feeder", aFeed[0], aFeed[1], aFeed[2], aFeed[3]);
  printButton("About", about[0], about[1], about[2], about[3]);  
}
/********************************* END OF MENU SCREEN *********************************/


/************** TIME and DATE SCREEN ********** dispScreen = 2 ************************/
void clockScreen(boolean refreshAll=true) 
{
  if (refreshAll)
  {
    rtcSetMin=t.min; rtcSetHr=t.hour; 
    rtcSetDy=t.date; rtcSetMon=t.mon; rtcSetYr=t.year;
    
    
    printHeader("Time and Date Settings");

    myGLCD.setColor(64, 64, 64);                   //Draw Dividers in Grey
    myGLCD.drawRect(0, 196, 319, 194);             //Bottom Horizontal Divider
    myGLCD.drawLine(0, 104, 319, 104);             //Middle Horizontal Divider
    printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
    printButton("SAVE", prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3], SMALL);
    printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);

    drawUpButton(houU[0], houU[1]);                //hour up
    drawUpButton(minU[0], minU[1]);                //min up
    drawDownButton(houD[0], houD[1]);              //hour down
    drawDownButton(minD[0], minD[1]);              //min down
    if (setTimeFormat==1)
      { drawUpButton(ampmU[0], ampmU[1]);          //AM/PM up   
        drawDownButton(ampmD[0], ampmD[1]);}       //AM/PM down  
    
    drawUpButton(dayU[0], dayU[1]);                //day up
    drawUpButton(monU[0], monU[1]);                //month up
    drawUpButton(yeaU[0], yeaU[1]);                //year up
    drawDownButton(dayD[0], dayD[1]);              //day down
    drawDownButton(monD[0], monD[1]);              //month down
    drawDownButton(yeaD[0], yeaD[1]);              //year down
  }
     
  timeDispH=rtcSetHr; timeDispM=rtcSetMin; 
  xTimeH=107; yTime=52; xColon=xTimeH+42;
  xTimeM10=xTimeH+70; xTimeM1=xTimeH+86; xTimeAMPM=xTimeH+155;
  timeChange();

  setFont(LARGE, 0, 0, 255, 0, 0, 0);    
  myGLCD.print("Date", 20, 142);
  setFont(LARGE, 255, 255, 255, 0, 0, 0);      
  myGLCD.print("/", 149, 142);    
  myGLCD.print("/", 219, 142);

  if (setCalendarFormat==0)                             //DD/MM/YYYY Format
    {
     setFont(SMALL, 0, 0, 255, 0, 0, 0); 
     myGLCD.print("(DD/MM/YYYY)", 5, 160); 
     setFont(LARGE, 255, 255, 255, 0, 0, 0);          
     if ((rtcSetDy>=0) && (rtcSetDy<=9))                //Set DAY
       { myGLCD.print("0", 107, 142);
         myGLCD.printNumI(rtcSetDy, 123, 142);}
     else { myGLCD.printNumI(rtcSetDy, 107, 142);} 
     if ((rtcSetMon>=0) && (rtcSetMon<=9))              //Set MONTH
       { myGLCD.print("0", 177, 142);
         myGLCD.printNumI(rtcSetMon, 193, 142);}
     else { myGLCD.printNumI(rtcSetMon, 177, 142);} 
    } else
  if (setCalendarFormat==1)                             //MM/DD/YYYY Format
    {
     setFont(SMALL, 0, 0, 255, 0, 0, 0); 
     myGLCD.print("(MM/DD/YYYY)", 5, 160); 
     setFont(LARGE, 255, 255, 255, 0, 0, 0);          
     if ((rtcSetMon>=0) && (rtcSetMon<=9))              //Set MONTH
       { myGLCD.print("0", 107, 142);
         myGLCD.printNumI(rtcSetMon, 123, 142);}
     else { myGLCD.printNumI(rtcSetMon, 107, 142);} 
     if ((rtcSetDy>=0) && (rtcSetDy<=9))                //Set DAY
       { myGLCD.print("0", 177, 142);
         myGLCD.printNumI(rtcSetDy, 193, 142);}
     else { myGLCD.printNumI(rtcSetDy, 177, 142);} 
    }  
  myGLCD.printNumI(rtcSetYr, 247, 142);                 //Set YEAR
}

void timeChange()
{
  setFont(LARGE, 0, 0, 255, 0, 0, 0); 
  myGLCD.print("Time", 20, yTime);

  if (setTimeFormat==0)                                 //24HR Format
    { setFont(SMALL, 0, 0, 255, 0, 0, 0); 
      myGLCD.print("(24HR)", 29, yTime+18);}

  if (setTimeFormat==1)                                 //12HR Format
    { setFont(SMALL, 0, 0, 255, 0, 0, 0);  
      myGLCD.print("(12HR)", 29, yTime+18);}

  timeCorrectFormat();
}

void timeCorrectFormat()
{
  setFont(LARGE, 255, 255, 255, 0, 0, 0);   
  myGLCD.print(":",  xColon, yTime);  
  if (setTimeFormat==0)                                 //24HR Format
    {
     setFont(LARGE, 255, 255, 255, 0, 0, 0);  
     if ((timeDispH>=0) && (timeDispH<=9))              //Set HOUR
       { myGLCD.print("0", xTimeH, yTime);
         myGLCD.printNumI(timeDispH, xTimeH+16, yTime);}
     else { myGLCD.printNumI(timeDispH, xTimeH, yTime);} 
    }    
  if (setTimeFormat==1)                                 //12HR Format
    {
     setFont(LARGE, 255, 255, 255, 0, 0, 0);  
     if (timeDispH==0)                                  //Set HOUR
       { myGLCD.print("12", xTimeH, yTime);}
     if ((timeDispH>=1) && (timeDispH<=9))
       { myGLCD.print("0", xTimeH, yTime);
         myGLCD.printNumI(timeDispH, xTimeH+16, yTime);}
     if ((timeDispH>=10) && (timeDispH<=12))
       { myGLCD.printNumI(timeDispH, xTimeH, yTime);}
     if ((timeDispH>=13) && (timeDispH<=21))
       { myGLCD.print("0", xTimeH, yTime);
         myGLCD.printNumI(timeDispH-12, xTimeH+16, yTime);}
     if (timeDispH>=22)
       { myGLCD.printNumI(timeDispH-12, xTimeH, yTime);}
      
     if (AM_PM==1)
       { myGLCD.print("AM", xTimeAMPM, yTime); }
     if (AM_PM==2)
       { myGLCD.print("PM", xTimeAMPM, yTime); }
    }
  if ((timeDispM>=0) && (timeDispM<=9))                 //Set MINUTES
    { myGLCD.print("0", xTimeM10, yTime);
      myGLCD.printNumI(timeDispM, xTimeM1, yTime);}
  else { myGLCD.printNumI(timeDispM, xTimeM10, yTime);} 
}    
/**************************** END OF TIME and DATE SCREEN *****************************/


/*********** H2O TEMP CONTROL SCREEN ********** dispScreen = 3 ************************/
void tempScreen(boolean refreshAll=false)
{
  String deg;
  if (refreshAll)
    {
     if ((setTempC==0) && (setTempScale==0)) {
       setTempC = 26.1;  }                         //change to 26.1 deg C
     if (((setTempF==0) || (setTempF==setTempC)) && (setTempScale==1)) {
       setTempF = 79.0;  }                         //change to 79.0 deg F
       
     if (setTempScale==1) {
       temp2beS = setTempF;
       temp2beO = offTempF;
       temp2beA = alarmTempF; }
     else {
     temp2beS = setTempC;
     temp2beO = offTempC;
     temp2beA = alarmTempC; }
     
     printHeader("H2O Temperature Control Settings");
     
     myGLCD.setColor(64, 64, 64);                    //Draw Dividers in Grey
     myGLCD.drawRect(0, 196, 319, 194);              //Bottom Horizontal Divider
     printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
     printButton("SAVE", prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3], SMALL);
     printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);

     if (setTempScale==1) {deg ="F";}                //Print deg C or deg F
       else {deg = "C";}
     degC_F=deg;
     char bufferDeg[2];
     degC_F.toCharArray(bufferDeg,2);

     setFont(SMALL, 255, 255, 255, 0, 0, 0);
     myGLCD.print("Desired Temperature in", 60, 34);
     myGLCD.drawCircle(245, 36, 1);              
     myGLCD.print(bufferDeg, 250, 34);
     myGLCD.print(":",258,34);
     myGLCD.print("Temperature Offset:", CENTER, 84);
     myGLCD.print("Alarm Offset:", CENTER, 134);  
     
     printButton("-", temM[0], temM[1], temM[2], temM[3], true);      //temp minus
     printButton("+", temP[0], temP[1], temP[2], temP[3], true);      //temp plus
     printButton("-", offM[0], offM[1], offM[2], offM[3], true);      //offset minus
     printButton("+", offP[0], offP[1], offP[2], offP[3], true);      //offset plus
     printButton("-", almM[0], almM[1], almM[2], almM[3], true);      //alarm minus
     printButton("+", almP[0], almP[1], almP[2], almP[3], true);      //alarm plus
    }

  setFont(LARGE, 255, 255, 255, 0, 0, 0);
  myGLCD.printNumF(temp2beS, 1, CENTER, 54);
  myGLCD.printNumF(temp2beO, 1, CENTER, 104);
  myGLCD.printNumF(temp2beA, 1, CENTER, 154);
}
/************************** END of H20 TEMP CONTROL SCREEN ****************************/


/********** LED TESTING OPTIONS SCREEN ******** dispScreen = 4 ************************/
void ledTestOptionsScreen()
{
  printHeader("LED Testing Options"); 

  myGLCD.setColor(64, 64, 64);                       //Draw Dividers in Grey
  myGLCD.drawRect(0, 196, 319, 194);                 //Bottom Horizontal Divider   
  printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
  printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);
  
  printButton("Test LED Array Output", tstLA[0], tstLA[1], tstLA[2], tstLA[3]);
  printButton("Control Individual Leds", cntIL[0], cntIL[1], cntIL[2], cntIL[3]);
}
/*************************** END OF LED TEST OPTIONS SCREEN ***************************/


/********** TEST LED ARRAY SCREEN ************* dispScreen = 5 ************************/
void testArrayScreen(boolean refreshAll=false)
{    
  if (refreshAll) 
    {  
     printHeader("Test LED Array Output Settings");
     myGLCD.fillRect (1, 15, 318, 37);       //clear "Test in Progress" Banner

     myGLCD.setColor(64, 64, 64);            //Draw Dividers in Grey
     myGLCD.drawRect(0, 196, 319, 194);      //Bottom Horizontal Divider
     printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
     printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);     

     printButton ("", stsT[0], stsT[1], stsT[2], stsT[3], true);      //start/stop
     printButton ("-10s", tenM[0], tenM[1], tenM[2], tenM[3], true);  //-10s
     printButton ("+10s", tenP[0], tenP[1], tenP[2], tenP[3], true);  //+10s
     myGLCD.print("START", stsT[0]+6, stsT[1]+15);
     myGLCD.print("TEST", stsT[0]+15, stsT[1]+40);   
    } 
  else
    {
     min_cnt=0;
     
     myGLCD.setColor(0, 0, 0);
     myGLCD.fillRect (1, 15, 318, 99);       //clear test results if any
     myGLCD.fillRect (1, 187, 318, 227);     //clear the "Back" and "Cancel" Buttons
      
     myGLCD.setColor(0, 0, 255);
     myGLCD.fillRect(stsT[0]+5, stsT[1]+5, stsT[2]-5, stsT[3]-40);    //clear 'start'
     setFont(LARGE, 255, 255, 255, 0, 0, 255);
     myGLCD.print("STOP", stsT[0]+15, stsT[1]+15);
     
     myGLCD.setColor(255, 0, 0);
     myGLCD.fillRect (1, 15, 318, 37);
     myGLCD.drawRoundRect (stsT[0], stsT[1], stsT[2], stsT[3]); //red button during test
     setFont(LARGE, 255, 255, 255, 255, 0, 0);
     myGLCD.print("Test in Progress", CENTER, 16);
     setFont(SMALL, 255, 255, 255, 0, 0, 0);
     if (setTimeFormat==1) 
       { myGLCD.print("TIME:", 52, 40);}
     else { myGLCD.print("TIME:", 30, 40);}
     myGLCD.print("LIGHT OUTPUT (0--255):", 140, 40);
     
   
     while (LEDtestTick)               //test LED and speed up time
     {
      unsigned long currentMillis = millis();
       
      if (myTouch.dataAvailable()) 
        { processMyTouch();}
     
      if (currentMillis - previousMillisLED > 500)    //change time every 0.5s
        {
         previousMillisLED = currentMillis;

         min_cnt++;
         String AMPM;
         int hrsPM;

         int hours = min_cnt/60;
         int minut = min_cnt%60;

         setFont(LARGE, 255, 255, 255, 0, 0, 0);               
         hrsPM=hours-12;
         if (setTimeFormat==1) 
           {
             if (hours==0) 
               { myGLCD.printNumI(12, 7, 55);} else { 
             if ((hours>=1)&&(hours<=9))
               { myGLCD.print(" ", 7, 55);
                 myGLCD.printNumI(hours, 23, 55);} else { 
             if ((hours>=13)&&(hours<=21))
               { myGLCD.print(" ", 7, 55);
                 myGLCD.printNumI(hrsPM, 23, 55);} else {           
             if ((hours>=22)&&(hours<=24))
               { myGLCD.printNumI(hrsPM, 7, 55);} else { 
               { myGLCD.printNumI(hours, 7, 55);}}}}}
             if (hours<12){myGLCD.print("AM", 95, 55);}         //Adding the AM/PM suffix
               else{myGLCD.print("PM", 95, 55);}                
           }
         else
           {
             if ((hours>=0)&&(hours<=9))
               { myGLCD.printNumI(0, 7, 55);
                 myGLCD.printNumI(hours, 23, 55);}
             else  
               { myGLCD.printNumI(hours, 7, 55);}
           }
         myGLCD.print(":", 39, 55);                   
         if ((minut>=0)&&(minut<=9))
           { myGLCD.printNumI(0, 55, 55);
             myGLCD.printNumI(minut, 71, 55);}
         else { myGLCD.printNumI(minut, 55, 55);}
                  
         setFont(SMALL, 0, 150, 0, 0, 0, 0);
         String sled = "SUMP:  " + String(sled_out) + " " + " ";
         char bufferS[11];
         sled.toCharArray(bufferS, 11);
         myGLCD.print(bufferS, 145, 55);

         setFont(SMALL, 255, 0, 0, 0, 0, 0);
         String rled = "Red:   " + String(rled_out) + "  " + " ";
         char bufferR[11];
         rled.toCharArray(bufferR, 11);
         myGLCD.print(bufferR, 145, 67);
         
         setFont(SMALL, 255, 255, 255, 0, 0, 0);
         String wled = "White: " + String(wled_out) + " " + " ";
         char bufferW[11];
         wled.toCharArray(bufferW, 11);
         myGLCD.print(bufferW, 145, 79);
         
         setFont(SMALL, 9, 184, 255, 0, 0, 0);
         String bled = "Blue:  " + String(bled_out) + " " + " ";      
         char bufferB[11];
         bled.toCharArray(bufferB, 11);
         myGLCD.print(bufferB, 235, 55);      
         
         setFont(SMALL, 58, 95, 205, 0, 0, 0);
         String rbled = "Royal: " + String(rbled_out) + "  " + " ";
         char bufferRB[11];
         rbled.toCharArray(bufferRB, 11);
         myGLCD.print(bufferRB, 235, 67);
        
         setFont(SMALL, 224, 102, 255, 0, 0, 0);
         String uvled = "Ultra: " + String(uvled_out) + "  " + " ";
         char bufferUV[11];
         uvled.toCharArray(bufferUV, 11);
         myGLCD.print(bufferUV, 235, 79);         
        
         LED_levels_output();
         checkTempC();
         TimeDateBar();
         }
      }
    } 
}
/*************************** END OF TEST LED ARRAY SCREEN *****************************/


/********* TEST INDIVIDUAL LED(S) SCREEN ****** dispScreen = 6 ************************/
void testIndLedScreen()
{
  printHeader("Test Individual LED Values (0--255)");

  setFont(SMALL, 255, 255, 255, 0, 0, 0); 
  myGLCD.setColor(0, 150, 0);                 //initial Prints
  myGLCD.print("SMP", 44, 176);    
  myGLCD.print("000", 44, 186);  
  myGLCD.setColor(255, 0, 0);
  myGLCD.print("RED", 82, 176);    
  myGLCD.print("000", 82, 186);  
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("WHT", 120, 176);    
  myGLCD.print("000", 120, 186);  
  myGLCD.setColor(9, 184, 255);
  myGLCD.print("BLU", 158, 176);    
  myGLCD.print("000", 158, 186);  
  myGLCD.setColor(58, 95, 205);
  myGLCD.print("RBL", 196, 176);    
  myGLCD.print("000", 196, 186);  
  myGLCD.setColor(224, 102, 255);  
  myGLCD.print("UVL", 234, 176);    
  myGLCD.print("000", 234, 186);  
  myGLCD.setColor(176, 176, 176);
  myGLCD.print("LUN", 272, 176);    
  myGLCD.print("000", 272, 186);  
 
  for (int b=0; b<7; b++)
    { drawUpButtonSlide((b*38)+39, 17);}
  for (int b=0; b<7; b++)
    { drawDownButtonSlide((b*38)+39, 200);}     

  drawSliderBarGraph();  
}
/************************ END OF TEST INDIVIDUAL LED(S) SCREEN ************************/


/******** SHOW LED COLOR CHOICES SCREEN ******* dispScreen = 7 ************************/
void ledColorViewScreen()
{
  printHeader("Individual LED Outputs: Color Choices");

  myGLCD.setColor(64, 64, 64);                      //Draw Dividers in Grey
  myGLCD.drawRect(0, 196, 319, 194);                //Bottom Horizontal Divider
  printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
  printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL); 
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRoundRect(10, 20, 150, 50);
  setFont(SMALL, 0, 0, 0, 255, 255, 255);   
  myGLCD.print("White", 60, 29);
  
  myGLCD.setColor(58, 95, 205);
  myGLCD.fillRoundRect(10, 60, 150, 90);
  setFont(SMALL, 255, 255, 255, 58, 95, 205);
  myGLCD.print("Royal Blue", 40, 69);

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect(10, 100, 150, 130);
  setFont(SMALL, 255, 255, 255, 255, 0, 0);   
  myGLCD.print("Red", 68, 109);

  myGLCD.setColor(176, 176, 176);  
  myGLCD.fillRoundRect(90, 140, 230, 170);
  setFont(SMALL, 0, 0, 0, 176, 176, 176);   
  myGLCD.print("Lunar", 140, 149);
  
  myGLCD.setColor(0, 150, 0);
  myGLCD.fillRoundRect(170, 100, 310, 130);
  setFont(SMALL, 255, 255, 255, 0, 150, 0);      
  myGLCD.print("Sump", 224, 109);
   
  myGLCD.setColor(224, 102, 255);
  myGLCD.fillRoundRect(170, 60, 310, 90);
  setFont(SMALL, 255, 255, 255, 224, 102, 255);   
  myGLCD.print("Ultraviolet", 196, 69);
   
  myGLCD.setColor(9, 184, 255);
  myGLCD.fillRoundRect(170, 20, 310, 50);
  setFont(SMALL, 255, 255, 255, 9, 184, 255);   
  myGLCD.print("Blue", 224, 29);

  myGLCD.setColor(255, 255, 255);
  for (int x=0; x<2; x++)
  { for (int y=0; y<3; y++)
      { myGLCD.drawRoundRect((x*160)+10, (y*40)+20, (x*160)+150, (y*40)+50); }
  }
  myGLCD.drawRoundRect(90, 140, 230, 170); 
}
/************************** END OF LED COLOR CHOICES SCREEN ***************************/


/****** SHOW LED VALUES FOR CHOICE SCREEN ********** dispScreen = 8 *******************/
void ledValuesScreen()
{
  int a;
  
  if (COLOR==1) {
    for (int i; i<96; i++)
      tled[i] = wled[i];
    printHeader("White LED Output Values");
    }
  if (COLOR==2) {
    for (int i; i<96; i++)
      tled[i] = bled[i];
    printHeader("Blue LED Output Values");
    }
  if (COLOR==3) {
    for (int i; i<96; i++)
      tled[i] = rbled[i];
    printHeader("Roayl Blue LED Output Values");
    }
  if (COLOR==4) {
    for (int i; i<96; i++)
      tled[i] = rled[i];
    printHeader("Red LED Output Values");
    }  
  if (COLOR==5) {
    for (int i; i<96; i++)
      tled[i] = uvled[i];
    printHeader("Ultraviolet LED Output Values");
    }
  if (COLOR==6) {
    for (int i; i<96; i++)
      tled[i] = sled[i];
    printHeader("Sump LED Output Values");
    }
  if (COLOR==7) {
    tMinI=MinI;
    tMaxI=MaxI;
    
    
    printHeader("View/Change Moon LED Min & Max Output");
    setFont(SMALL, 255, 255, 255, 0, 0, 0);
    myGLCD.print("Set Minimum", 36, 20); 
    myGLCD.print("Illumination", 32, 32); 
    myGLCD.drawBitmap(52, 47, 58, 58, New_Moon, 1);      //New Moon Picture
    myGLCD.print("(0--255)", 47, 132);
    myGLCD.print("-1", 22, 177);        
    myGLCD.print("+5", 121, 177);  
    myGLCD.print("Set Maximum", 196, 20); 
    myGLCD.print("Illumination", 192, 32); 
    myGLCD.drawBitmap(211, 47, 58, 58, Full_Moon, 1);    //Full Moon Picture
    myGLCD.print("(0--255)", 209, 132);
    myGLCD.print("-1", 184, 177);        
    myGLCD.print("+5", 283, 177);      
    setFont(SMALL, 176, 176, 176, 0, 0, 0);            
    myGLCD.print("New Moon", 48, 108); 
    myGLCD.print("Full Moon", 204, 108); 

    setFont(LARGE, 255, 255, 255, 0, 0, 0);
    myGLCD.print("   ", 55, 152);
    if (tMinI<=9)
      { myGLCD.printNumI(tMinI, 71, 152);}
    if ((tMinI>=10)&&(tMinI<=99))
      { myGLCD.printNumI(tMinI, 63, 152);}
    if (tMinI>=100)
      { myGLCD.printNumI(tMinI, 55, 152);}
      
    myGLCD.print("   ", 217, 152);
    if (tMaxI<=9)
      { myGLCD.printNumI(tMaxI, 233, 152);}
    if ((tMaxI>=10)&&(tMaxI<=99))
      { myGLCD.printNumI(tMaxI, 225, 152);}
    if (tMaxI>=100)
      { myGLCD.printNumI(tMaxI, 217, 152);}
   
    printButton("-", MINiM[0], MINiM[1], MINiM[2], MINiM[3], true);  //Minimum Illum. minus
    printButton("+", MINiP[0], MINiP[1], MINiP[2], MINiP[3], true);  //Minimum Illum. plus
    printButton("-", MAXiM[0], MAXiM[1], MAXiM[2], MAXiM[3], true);  //Max Illum. minus
    printButton("+", MAXiP[0], MAXiP[1], MAXiP[2], MAXiP[3], true);  //Max Illum. plus

    myGLCD.setColor(64, 64, 64);
    myGLCD.drawRect(158, 14, 160, 194);
    myGLCD.drawRect(0, 196, 319, 194);
    myGLCD.setColor(0, 0, 0);
    myGLCD.drawLine(159, 193, 159, 195);
    printButton("MORE COLORS", back[0], back[1], back[2], back[3], SMALL);
    printButton("SAVE", prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3], SMALL);
    printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);    
    }  
    
  if (COLOR!=7) {
    setFont(SMALL, 255, 255, 255, 0, 0, 0);
    for (int i=0; i<12; i++) 
    {
      myGLCD.setColor(0, 255, 255);
      myGLCD.printNumI((i*2), (i*26)+13, 14);
      myGLCD.printNumI(((i*2)+1), (i*26)+13, 24);
       for (int j=0; j<8; j++) 
       {
          a= (i*8)+j;         
          myGLCD.setColor(255, 255, 255);
          myGLCD.printNumI(tled[a], (i*26)+7, (j*18)+39);
          myGLCD.setColor(100, 100, 100);
          myGLCD.drawRect((i*26)+4, (j*18)+35, (i*26)+30, (j*18)+53);
       }
    }
    myGLCD.setColor(64, 64, 64);       
    myGLCD.drawRect(0, 196, 319, 194); 
    printButton("MORE COLORS", back[0], back[1], back[2], back[3], SMALL);   
    printButton("CHANGE", ledChV[0], ledChV[1], ledChV[2], ledChV[3], SMALL);
    printButton("SAVE", eeprom[0], eeprom[1], eeprom[2], eeprom[3]);
  }
}
/*************************** END OF SHOW LED VALUES SCREEN ****************************/


/********** CHANGE LED VALUES SCREEN ********** dispScreen = 9 ************************/
void ledChangeScreen()
{
  if (COLOR==1)
    { printHeader("Change White LED Output Values"); 
      sbR=255; sbG=255; sbB=255; }
  if (COLOR==2)
    { printHeader("Change Blue LED Output Values"); 
      sbR=9; sbG=184; sbB=255; }  
  if (COLOR==3)
    { printHeader("Change Royal Blue LED Output Values"); 
      sbR=58; sbG=95; sbB=205; }
  if (COLOR==4)
    { printHeader("Change Red LED Output Values"); 
      sbR=255; sbG=0; sbB=0; }      
  if (COLOR==5)
    { printHeader("Change Ultraviolet LED Output Values"); 
      sbR=224; sbG=102; sbB=255; }  
  if (COLOR==6)
    { printHeader("Change Sump LED Output Values"); 
      sbR=0; sbG=150; sbB=0; }
      
  setFont(SMALL, 0, 255, 255, 0,0,0);                  //Top time selector bar
  for (int i=0; i<12; i++) {
    myGLCD.setColor(0, 255, 255);
    if (i<5) {
      myGLCD.printNumI((i*2), (i*26)+15, 17);
      myGLCD.printNumI(((i*2)+1), (i*26)+15, 28);}
    else {
      myGLCD.printNumI((i*2), (i*26)+11, 17);
      myGLCD.printNumI(((i*2)+1), (i*26)+11, 28);}
    myGLCD.setColor(100, 100, 100);
    myGLCD.drawRect((i*26)+4, 16 , (i*26)+30, 41);
    }      

  myGLCD.drawRect(4, 43, 30, 223);                     //left menu bar
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect(8, 47, 26, 101);
  myGLCD.fillRoundRect(8, 106, 26, 140);
  myGLCD.fillRoundRect(8, 145, 26, 219);  
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(8, 47, 26, 101);                //MENU Button
  myGLCD.drawRoundRect(8, 106, 26, 140);               //OK Button
  myGLCD.drawRoundRect(8, 145, 26, 219);               //CANCEL Button  
  
  setFont(SMALL, 255, 255, 255, 0, 0, 255); 
  myGLCD.print("M", 14, 54);    
  myGLCD.print("E", 14, 64);
  myGLCD.print("N", 14, 74);
  myGLCD.print("U", 14, 84);  
  myGLCD.print("O", 14, 113);    
  myGLCD.print("K", 14, 123);
  myGLCD.print("C", 14, 152);    
  myGLCD.print("A", 14, 162);
  myGLCD.print("N", 14, 172);
  myGLCD.print("C", 14, 182);  
  myGLCD.print("E", 14, 192);
  myGLCD.print("L", 14, 202);  
  
  setFont(SMALL, 255, 0, 0, 0, 0, 0);     
  myGLCD.print("Make a selection above. Choose", 57, 110);
  myGLCD.print("any Two-Hour Increment", 89, 122);
  myGLCD.print("then you can adjust values.", 73, 134);       
}
/*************************** END OF CHANGE LED VALUES SCREEN **************************/  


/************ WAVEMAKER SCREEN **************** dispScreen = 10 ***********************/
void WaveMakerButtons()
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect(5, 20, 155, 40);        //Alternating Mode Button
  myGLCD.fillRoundRect(165, 20, 315, 40);      //Synchronous Mode Button
  myGLCD.fillRoundRect(5, 46, 155, 66);        //Feeding Mode Button
  myGLCD.fillRoundRect(165, 46, 315, 66);      //Turn Pumps ON/OFF Button
  setFont(SMALL, 255, 255, 255, 255, 0, 0);
  myGLCD.print("Alternating Mode", 16, 24); 
  myGLCD.print("Synchronous Mode", 176, 24);
  myGLCD.print("Feeding Mode", 32, 50);
  myGLCD.print("Turn Pumps OFF", 184, 50);
}

void WaveMakerScreen()
{ 
  printHeader("View/Change Wavemaker Settings");
   
  myGLCD.setColor(64, 64, 64);                 //Draw Dividers in Grey
  myGLCD.drawRect(0, 70, 319, 72);             //Top Horizontal Divider
  myGLCD.drawRect(0, 196, 319, 194);           //Bottom Horizontal Divider
  printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
  printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL); 
   
  setFont(LARGE, 255, 255, 255, 0, 0, 0);
  myGLCD.print("Current Status:", CENTER, 80);
} 

void viewWaveTimes()
{
  setFont(SMALL, 0, 255, 0, 0, 0, 0);     
  int dynamicXm=135;
  if ((MIN1<=99) && (MIN1>=10)) {dynamicXm+=8;}
  if (MIN1<=9) {dynamicXm+=16;}
  myGLCD.printNumI(MIN1, dynamicXm, minY1);
  int dynamicXs=231;
  if (SEC1<=9) {dynamicXs+=8;}
  myGLCD.printNumI(SEC1, dynamicXs, minY1);     
  dynamicXm=135;
  if ((MIN2<=99) && (MIN2>=10)) {dynamicXm+=8;}
  if (MIN2<=9) {dynamicXm+=16;}
  myGLCD.printNumI(MIN2, dynamicXm, minY2);
  dynamicXs=231;
  if (SEC2<=9) {dynamicXs+=8;}
  myGLCD.printNumI(SEC2, dynamicXs, minY2);     
}

void WaveMakerStatusScreen()
{
  myGLCD.setColor(0, 0, 0);                    //Clear Status Area
  myGLCD.fillRect( 1, 96, 318, 193);
  
  WaveMakerButtons();
  
  if (WAVE==0)
    {
     setFont(LARGE, 255, 0, 0, 0, 0, 0);
     myGLCD.print("No Mode Saved!", CENTER, 100);
     setFont(SMALL, 255, 0, 0, 0, 0, 0);     
     myGLCD.print("Make a selection above. Choose", CENTER, 130);
     myGLCD.print("either Alternating or Synchronous", CENTER, 142);
     myGLCD.print("then save your settings.", CENTER, 154);          
    } else
      
  if (WAVE==1)
    {
     myGLCD.setColor(0, 255, 0);
     myGLCD.fillRoundRect(5, 20, 155, 40);  
     setFont(SMALL, 0, 0, 0, 0, 255, 0);
     myGLCD.print("Alternating Mode", 16, 24); 
     myGLCD.setColor(255, 255, 255);
     myGLCD.drawRoundRect(5, 20, 155, 40);
     
     setFont(LARGE, 0, 255, 0, 0, 0, 0);
     myGLCD.print("Alternating Flow", CENTER, 100);
     
     myGLCD.print("Pump1->", 15, 140);
     myGLCD.print("min", 167, 140); 
     myGLCD.print("sec", 255, 140); 
     myGLCD.print("Pump2->", 15, 160);    
     myGLCD.print("min", 167, 160); 
     myGLCD.print("sec", 255, 160);     

     MIN1=Pump1m, SEC1=Pump1s, MIN2=Pump2m, SEC2=Pump2s;
     minY1=144, minY2=164;
     viewWaveTimes();
    } else
    
    
  if (WAVE==2)
    {
     myGLCD.setColor(0, 255, 0);
     myGLCD.fillRoundRect(165, 20, 315, 40);  
     setFont(SMALL, 0, 0, 0, 0, 255, 0);
     myGLCD.print("Synchronous Mode", 176, 24);
     myGLCD.setColor(255, 255, 255);
     myGLCD.drawRoundRect(165, 20, 315, 40);
     
     setFont(LARGE, 0, 255, 0, 0, 0, 0);
     myGLCD.print("Synchronous Flow", CENTER, 100);
     
     if (Synch==1) 
       { 
        myGLCD.print("Constantly ON", CENTER, 116);
        setFont(SMALL, 255, 0, 0, 0, 0, 0);
        myGLCD.print("Both powerheads are ON constantly.", CENTER, 144);
        myGLCD.print("To change to Pulsating Mode, push", CENTER, 156);  
        myGLCD.print("the Synchronous Mode button above", CENTER, 168);         
       }     
     if (Synch==2) 
       { 
        myGLCD.print("Pulsating Mode", CENTER, 116);

        myGLCD.print("ON for", 15, 146);
        myGLCD.print("min", 167, 146); 
        myGLCD.print("sec", 255, 146); 
        myGLCD.print("OFF for", 15, 166);    
        myGLCD.print("min", 167, 166); 
        myGLCD.print("sec", 255, 166);     

        MIN1=OnForTm, SEC1=OnForTs, MIN2=OffForTm, SEC2=OffForTs;
        minY1=150, minY2=170;
        viewWaveTimes();
       }
    } else
    
    
  if (WAVE==3)
    {
     myGLCD.setColor(0, 255, 0);
     myGLCD.fillRoundRect(165, 46, 315, 66);  
     setFont(SMALL, 0, 0, 0, 0, 255, 0);
     myGLCD.print("Turn Pumps ON", 188, 50);      
     myGLCD.setColor(255, 255, 255);
     myGLCD.drawRoundRect(165, 46, 315, 66);
     
     setFont(LARGE, 0, 255, 0, 0, 0, 0);
     myGLCD.print("Powerheads are OFF", CENTER, 100);
     setFont(SMALL, 0, 255, 0, 0, 0, 0);
     myGLCD.print("To turn the powerheads back ON", CENTER, 125);
     if (MODE==1) { myGLCD.print("and resume in Alternating Mode,", CENTER, 137);} else
     if (MODE==2) { myGLCD.print("and resume in Synchronous Mode,", CENTER, 137);}
     myGLCD.print("push the Turn Pumps ON button above", CENTER, 149);     
     
     waveMakerOff=true;
     digitalWrite(WaveMakerTop, LOW);
     digitalWrite(WaveMakerBottom, LOW);    
    }  
    
  myGLCD.setColor(255, 255, 255);
  for (int x=0; x<2; x++)
    { for (int y=0; y<2; y++)
        { myGLCD.drawRoundRect((x*160)+5, (y*26)+20, (x*160)+155, (y*26)+40); }
    }  
        
  if (WAVE==4)
    {
     myGLCD.setColor(0, 255, 0);
     myGLCD.fillRoundRect(5, 46, 155, 66);  
     setFont(SMALL, 0, 0, 0, 0, 255, 0);
     myGLCD.print("Feeding Mode", 32, 50);
     myGLCD.setColor(255, 255, 255);
     for (int x=0; x<2; x++)
       { for (int y=0; y<2; y++)
           { myGLCD.drawRoundRect((x*160)+5, (y*26)+20, (x*160)+155, (y*26)+40); }
       }  
     
     setFont(LARGE, 0, 255, 0, 0, 0, 0);
     myGLCD.print("Feeding", CENTER, 100);
     setFont(SMALL, 0, 255, 0, 0, 0, 0);
     myGLCD.print("Powerheads will remain OFF", CENTER, 125);
     myGLCD.print("for 5 minutes", CENTER, 137);
     if (MODE==1) { myGLCD.print("Alternating Mode will resume in:", CENTER, 155);} else
     if (MODE==2) { myGLCD.print("Synchronous Mode will resume in:", CENTER, 155);}
     
     waveMakerOff=true;   
     digitalWrite(WaveMakerTop, LOW);
     digitalWrite(WaveMakerBottom, LOW);    
    }
    while (WAVE==4)
      {
       unsigned long currentMillis = millis();
     
       if (myTouch.dataAvailable())  
         { processMyTouch();}
    
       if(currentMillis - previousMillisCt >= intervalCt) 
         {
          previousMillisCt = currentMillis;   
          if(countDown>=0)
            {
             if (countDown<=30) 
               { myGLCD.setColor(0, 0, 0);
                 myGLCD.fillRect(128, 173, 192, 189);}
             MIN_O= (countDown / 60) % 10;
             SEC_T= (countDown % 60) / 10;
             SEC_O= (countDown % 60) % 10;
             setFont(LARGE, 255, 0, 0, 0, 0, 0);       
             myGLCD.printNumI(MIN_O, 128, 173);
             myGLCD.print(":", 144, 173);
             myGLCD.printNumI(SEC_T, 160, 173);
             myGLCD.printNumI(SEC_O, 176, 173);
             if(countDown==0)
               {
                WAVE=MODE;
                WaveMakerStatusScreen(); 
               }
             countDown--;
             checkTempC();
             TimeDateBar();             
            }
          waveMakerOff=false;            
         }   
      }
}
/******************************* END OF WAVEMAKER SCREEN ******************************/  


/******** WAVEMAKER SETTINGS SCREEN ************** dispScreen = 11 ********************/
void WaveMakerSettingsScreen()
{ 
  myGLCD.setColor(64, 64, 64);                 //Draw Dividers in Grey
  myGLCD.drawRect(0, 196, 319, 194);           //Bottom Horizontal Divider
  
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect(5, 200, 105, 220);      //BACK Button
  myGLCD.fillRoundRect(110, 200, 210, 220);    //TEST Button
  myGLCD.fillRoundRect(215, 200, 315, 220);    //SAVE Button

  setFont(SMALL, 255, 255, 255, 0, 0, 255);
  myGLCD.print("<< BACK", 27, 204);
  myGLCD.print("TEST", 144, 204);  
  myGLCD.print("SAVE", 241, 204);
  
  myGLCD.setColor(255, 255, 255);
  for (int x=0; x<3; x++)
    { myGLCD.drawRoundRect((x*105)+5, 200, (x*105)+105, 220); }    


  setFont(LARGE, 255, 255, 255, 0, 0, 0);
  if (WAVE==1) 
    { printHeader("Alternating Flow Mode Settings");
      myGLCD.setColor(64, 64, 64);                 //Draw Dividers in Grey
      myGLCD.drawRect(0, 38, 319, 40);             //Top Horizontal Divider    
      myGLCD.drawRect(0, 115, 319, 117);           //Middle Horizontal Divider          
      setFont(SMALL, 255, 0, 0, 0, 0, 0);
      myGLCD.print("Set how long each powerhead will be ON", CENTER, 20); 
      setFont(SMALL, 0, 255, 0, 0, 0, 0);
      myGLCD.print("POWERHEAD 1", CENTER, 50);
      myGLCD.print("POWERHEAD 2", CENTER, 127);
      setFont(SMALL, 255, 255, 255, 0, 0, 0);
      myGLCD.print("Minutes", 57, 100);
      myGLCD.print("Seconds", 211, 100);
      myGLCD.print("Minutes", 57, 177);
      myGLCD.print("Seconds", 211, 177);
      
      Min1=Pump1m, Sec1=Pump1s, Min2=Pump2m, Sec2=Pump2s;     
      viewWaveTimesPage();
    }
    
  if (WAVE==2) 
    {
     printHeader("Synchronous Flow Mode Settings");
     myGLCD.setColor(64, 64, 64);                 //Draw Dividers in Grey
     myGLCD.drawRect(0, 44, 319, 46);             //Top Horizontal Divider     
    }  
}

void synchronousSynch()
{
  myGLCD.setColor(0, 0, 0);                       //Clear Status Area
  myGLCD.fillRect( 1, 47, 318, 193);  
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect(5, 20, 155, 40);           //Constantly ON Mode Button
  myGLCD.fillRoundRect(165, 20, 315, 40);         //Pulsating Mode Mode Button
  setFont(SMALL, 255, 255, 255, 255, 0, 0);
  myGLCD.print("Constantly ON", 28, 24); 
  myGLCD.print("Pulsating Mode", 184, 24);  
  for (int x=0; x<2; x++)
    { myGLCD.drawRoundRect((x*160)+5, 20, (x*160)+155, 40); } 
       
  if (Synch==0)
    {  
     setFont(LARGE, 255, 0, 0, 0, 0, 0);
     myGLCD.print("NO Synchronous Mode", CENTER, 80);
     myGLCD.print("has been selected!", CENTER, 100);       
     setFont(SMALL, 255, 0, 0, 0, 0, 0);     
     myGLCD.print("Choose either Constantly ON or", CENTER, 130);
     myGLCD.print("Pulsating Mode. Afterwards, you", CENTER, 142);       
     myGLCD.print("can alter the settings.", CENTER, 154);                
    } else

  if (Synch==1)
    {  
     myGLCD.setColor(0, 255, 0);
     myGLCD.fillRoundRect(5, 20, 155, 40);  
     setFont(SMALL, 0, 0, 0, 0, 255, 0);
     myGLCD.print("Constantly ON", 28, 24); 
     myGLCD.setColor(255, 255, 255);
     myGLCD.drawRoundRect(5, 20, 155, 40);
 
     setFont(LARGE, 0, 255, 0, 0, 0, 0);
     myGLCD.print("Constantly ON Mode", CENTER, 80);
     myGLCD.print("has been selected!", CENTER, 100);       
     setFont(SMALL, 255, 0, 0, 0, 0, 0);     
     myGLCD.print("Both powerheads will remain", CENTER, 130);
     myGLCD.print("ON constantly. To change to", CENTER, 142);       
     myGLCD.print("Pulsating Mode select it above.", CENTER, 154);             
    } else

  if (Synch==2)
    {  
     myGLCD.setColor(0, 255, 0);
     myGLCD.fillRoundRect(165, 20, 315, 40);  
     setFont(SMALL, 0, 0, 0, 0, 255, 0);
     myGLCD.print("Pulsating Mode", 184, 24);
     myGLCD.setColor(255, 255, 255);
     myGLCD.drawRoundRect(165, 20, 315, 40);
   
     myGLCD.setColor(64, 64, 64);                 //Draw Dividers in Grey
     myGLCD.drawRect(0, 120, 319, 122);           //Middle Horizontal Divider          
     setFont(SMALL, 0, 255, 0, 0, 0, 0);
     myGLCD.print("POWERHEADS WIll RUN FOR", CENTER, 50);
     myGLCD.print("POWERHEADS WILL STOP FOR", CENTER, 127);
     setFont(SMALL, 255, 255, 255, 0, 0, 0);
     myGLCD.print("Minutes", 57, 100);
     myGLCD.print("Seconds", 211, 100);
     myGLCD.print("Minutes", 57, 177);
     myGLCD.print("Seconds", 211, 177);
     
     Min1=OnForTm, Sec1=OnForTs, Min2=OffForTm, Sec2=OffForTs;     
     viewWaveTimesPage();     
    }
}

void viewWaveTimesPage()
{
  setFont(LARGE, 255, 255, 255, 0, 0, 0);
  if (Min1==0) { myGLCD.print("000", 59, 75); }
  if ((Min1>=1) && (Min1<=9)) 
    { myGLCD.print("00", 59, 75); 
      myGLCD.printNumI(Min1, 91, 75);}
  if ((Min1>=10) && (Min1<=99)) 
    { myGLCD.print("0", 59, 75); 
      myGLCD.printNumI(Min1, 75, 75);}
  if (Min1>=100) { myGLCD.printNumI(Min1, 59, 75);}

  if (Sec1==0) { myGLCD.print("00", 221, 75); }
  if ((Sec1>=1) && (Sec1<=9)) 
    { myGLCD.print("0", 221, 75); 
      myGLCD.printNumI(Sec1, 237, 75);}
  if (Sec1>=10) { myGLCD.printNumI(Sec1, 221, 75);}

  if (Min2==0) { myGLCD.print("000", 59, 152); }
  if ((Min2>=1) && (Min2<=9)) 
    { myGLCD.print("00", 59, 152); 
      myGLCD.printNumI(Min2, 91, 152);}
  if ((Min2>=10) && (Min2<=99)) 
    { myGLCD.print("0", 59, 152); 
      myGLCD.printNumI(Min2, 75, 152);}
  if (Min2>=100) { myGLCD.printNumI(Min2, 59, 152);}
      
  if (Sec2==0) { myGLCD.print("00", 221, 152); }
  if ((Sec2>=1) && (Sec2<=9)) 
    { myGLCD.print("0", 221, 152); 
      myGLCD.printNumI(Sec2, 237, 152);}
  if (Sec2>=10) { myGLCD.printNumI(Sec2, 221, 152);}
  
  printButton("-", pump1Mm[0], pump1Mm[1], pump1Mm[2], pump1Mm[3], LARGE);
  printButton("+", pump1Mp[0], pump1Mp[1], pump1Mp[2], pump1Mp[3], LARGE);      
  printButton("-", pump1Sm[0], pump1Sm[1], pump1Sm[2], pump1Sm[3], LARGE);
  printButton("+", pump1Sp[0], pump1Sp[1], pump1Sp[2], pump1Sp[3], LARGE);      
  printButton("-", pump2Mm[0], pump2Mm[1], pump2Mm[2], pump2Mm[3], LARGE);
  printButton("+", pump2Mp[0], pump2Mp[1], pump2Mp[2], pump2Mp[3], LARGE);      
  printButton("-", pump2Sm[0], pump2Sm[1], pump2Sm[2], pump2Sm[3], LARGE);
  printButton("+", pump2Sp[0], pump2Sp[1], pump2Sp[2], pump2Sp[3], LARGE);  
}

void waveModePlusMinus()
{
  setFont(LARGE, 255, 255, 255, 0, 0, 0);  
  if ((y>=70) && (y<=95))                        //First Row   
    {
     if ((x>=21) && (x<=46))                     //Pump 1 Minute Minus
       {
        waitForIt(21, 70, 46, 95);
        Min1 -= 1;
        if ((min1X>=10) && (Min1<=99)) {min1X=75; myGLCD.print("0", 59, 75);}
        if ((Min1<=9) && (Min1>0)) {min1X=91; myGLCD.print("00", 59, 75);}
        if (Min1<=0) {Min1=0; myGLCD.print("0", 91, 75);}
        setFont(LARGE, 255, 255, 255, 0, 0, 0);
        myGLCD.printNumI(Min1, min1X, 75);
       }
     if ((x>=120) && (x<=145))                   //Pump 1 Minute Plus
       {
        waitForIt(120, 70, 145, 95);
        Min1 += 1;
        if ((Min1>=0) && (Min1<=9)) {min1X=91;}
        if (Min1>9) {min1X=75;}
        if (Min1>99) {min1X=59;}
        if (Min1>=999) {Min1=999;}             
        setFont(LARGE, 255, 255, 255, 0, 0, 0);
        myGLCD.printNumI(Min1, min1X, 75);
       }
     if ((x>=175) && (x<=200))                   //Pump 1 Second Minus
       {
        waitForIt(175, 70, 200, 95);
        Sec1 -= 1;
        if ((sec1X>=10) && (Sec1<=59)) {sec1X=221;}
        if ((Sec1<=9) && (Sec1>0)) {sec1X=237; myGLCD.print("0", 221, 75);}
        if (Sec1<=0) {Sec1=0; myGLCD.print("0", 237, 75);}
        setFont(LARGE, 255, 255, 255, 0, 0, 0);
        myGLCD.printNumI(Sec1, sec1X, 75);
       }
     if ((x>=274) && (x<=299))                   //Pump 1 Second Plus
       {
        waitForIt(274, 70, 299, 95);
        Sec1 += 1;
        if ((Sec1>=0) && (Sec1<=9)) {sec1X=237;}
        if (Sec1>9) {sec1X=221;}
        if (Sec1>=59) {Sec1=59;}             
        setFont(LARGE, 255, 255, 255, 0, 0, 0);
        myGLCD.printNumI(Sec1, sec1X, 75);
       }
    }
      
  if ((y>=147) && (y<=172))                         //Second Row
    {
     if ((x>=21) && (x<=46))                        //Pump 2 Minute Minus
       {
        waitForIt(21, 147, 46, 172);
        Min2 -= 1;
        if ((min2X>=10) && (Min2<=99)) {min2X=75; myGLCD.print("0", 59, 152);}
        if ((Min2<=9) && (Min2>0)) {min2X=91; myGLCD.print("00", 59, 152);}
        if (Min2<=0) {Min2=0; myGLCD.print("0", 91, 152);}
        setFont(LARGE, 255, 255, 255, 0, 0, 0);
        myGLCD.printNumI(Min2, min2X, 152);
       }
     if ((x>=120) && (x<=145))                      //Pump 2 Minute Plus
       {
        waitForIt(120, 147, 145, 172);
        Min2 += 1;
        if ((Min2>=0) && (Min2<=9)) {min2X=91;}
        if (Min2>9) {min2X=75;}
        if (Min2>99) {min2X=59;}
        if (Min2>=999) {Min2=999;}             
        setFont(LARGE, 255, 255, 255, 0, 0, 0);
        myGLCD.printNumI(Min2, min2X, 152);
       }
     if ((x>=175) && (x<=200))                      //Pump 2 Second Minus
       {
        waitForIt(175, 147, 200, 172);
        Sec2 -= 1;
        if ((sec2X>=10) && (Sec2<=59)) {sec2X=221;}
        if ((Sec2<=9) && (Sec2>0)) {sec2X=237; myGLCD.print("0", 221, 152);}
        if (Sec2<=0) {Sec2=0; myGLCD.print("0", 237, 152);}
        setFont(LARGE, 255, 255, 255, 0, 0, 0);
        myGLCD.printNumI(Sec2, sec2X, 152);
       } 
     if ((x>=274) && (x<=299))                      //Pump 2 Second Plus
       {
        waitForIt(274, 147, 299, 172);
        Sec2 += 1;
        if ((Sec2>=0) && (Sec2<=9)) {sec2X=237;}
        if (Sec2>9) {sec2X=221;}
        if (Sec2>=59) {Sec2=59;}             
        setFont(LARGE, 255, 255, 255, 0, 0, 0);
        myGLCD.printNumI(Sec2, sec2X, 152);
       }  
    }
}
/************************** END OF WAVEMAKER SETTINGS SCREEN **************************/  


/******** AUTOMATIC FEEDER SCREEN ************* dispScreen = 12 ***********************/
void autoFeederScreen()
{
  printHeader("Automatic Fish Feeder Page");  
  
  myGLCD.setColor(64, 64, 64);
  myGLCD.drawRect(0, 196, 319, 194);
  printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
  printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);     
  myGLCD.setColor(64, 64, 64);
  myGLCD.drawRect(159, 194, 161, 121);
  myGLCD.drawRoundRect(78, 87, 242, 121);    
  myGLCD.drawRoundRect(80, 89, 240, 119);  
  myGLCD.drawRect(0, 103, 78, 105);
  myGLCD.drawRect(242, 103, 319, 105); 
  myGLCD.drawLine(159, 87, 159, 14);  
  myGLCD.drawLine(161, 87, 161, 14);  
  myGLCD.setColor(0, 0, 0);
  myGLCD.drawLine(160, 195, 160, 193);
  myGLCD.drawLine(160, 122, 160, 120);
  myGLCD.drawLine(77, 104, 79, 104);
  myGLCD.drawLine(241, 104, 243, 104);  
  myGLCD.drawLine(160, 88, 160, 86);

  myGLCD.setColor(153, 0, 102);
  myGLCD.fillRoundRect(85, 94, 235, 114);           //Feed Fish Now Button
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(85, 94, 235, 114); 
  setFont(SMALL, 255, 255, 255, 153, 0, 102);
  myGLCD.print("Feed Fish Now!", 106, 98);

  if (FEEDTime1==0)                                 //Feeding Time 1 Button 
    { myGLCD.setColor(255, 0, 0);
      myGLCD.fillRoundRect(5, 20, 155, 40);
      setFont(SMALL, 255, 255, 255, 255, 0, 0);
      myGLCD.print("Feeding Time 1", 24, 24);
      setFont(SMALL, 255, 0, 0, 0, 0, 0);     
      myGLCD.print("This time has not", 12, 52);
      myGLCD.print("been scheduled", 24, 65);}      
  else
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(5, 20, 155, 40);
      setFont(SMALL, 0, 0, 0, 0, 255, 0);
      myGLCD.print("Feeding Time 1", 24, 24); 
      timeDispH=feedFish1H; timeDispM=feedFish1M; 
      if (setTimeFormat==0) { xTimeH=40;}
      if (setTimeFormat==1) { xTimeH=16;}
      ReadFromEEPROM();
      if ((timeDispH>=0) && (timeDispH<=11)) { AM_PM=1;}
      else { AM_PM=2;}          
      yTime=56; xColon=xTimeH+32;
      xTimeM10=xTimeH+48; xTimeM1=xTimeH+64; xTimeAMPM=xTimeH+96;
      timeCorrectFormat();}
  if (FEEDTime2==0)                                 //Feeding Time 2 Button
    { myGLCD.setColor(255, 0, 0);
      myGLCD.fillRoundRect(165, 20, 315, 40);
      setFont(SMALL, 255, 255, 255, 255, 0, 0);
      myGLCD.print("Feeding Time 2", 184, 24); 
      setFont(SMALL, 255, 0, 0, 0, 0, 0);     
      myGLCD.print("This time has not", 172, 52);
      myGLCD.print("been scheduled", 184, 65);}      
  else
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(165, 20, 315, 40);
      setFont(SMALL, 0, 0, 0, 0, 255, 0);
      myGLCD.print("Feeding Time 2", 184, 24);
      timeDispH=feedFish2H; timeDispM=feedFish2M; 
      if (setTimeFormat==0) { xTimeH=200;}
      if (setTimeFormat==1) { xTimeH=176;}      
      ReadFromEEPROM();
      if ((timeDispH>=0) && (timeDispH<=11)) { AM_PM=1;}
      else { AM_PM=2;}          
      yTime=56; xColon=xTimeH+32;
      xTimeM10=xTimeH+48; xTimeM1=xTimeH+64; xTimeAMPM=xTimeH+96;      
      timeCorrectFormat();}          
  if (FEEDTime3==0)                                 //Feeding Time 3 Button
    { myGLCD.setColor(255, 0, 0);
      myGLCD.fillRoundRect(5, 168, 155, 188);
      setFont(SMALL, 255, 255, 255, 255, 0, 0);
      myGLCD.print("Feeding Time 3", 24, 172);
      setFont(SMALL, 255, 0, 0, 0, 0, 0);     
      myGLCD.print("This time has not", 12, 133);
      myGLCD.print("been scheduled", 24, 146);}       
  else
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(5, 168, 155, 188);
      setFont(SMALL, 0, 0, 0, 0, 255, 0);
      myGLCD.print("Feeding Time 3", 24, 172);
      timeDispH=feedFish3H; timeDispM=feedFish3M; 
      if (setTimeFormat==0) { xTimeH=40;}
      if (setTimeFormat==1) { xTimeH=16;}
      ReadFromEEPROM();
      if ((timeDispH>=0) && (timeDispH<=11)) { AM_PM=1;}
      else { AM_PM=2;}          
      yTime=137; xColon=xTimeH+32;
      xTimeM10=xTimeH+48; xTimeM1=xTimeH+64; xTimeAMPM=xTimeH+96;      
      timeCorrectFormat();}          
  if (FEEDTime4==0)                                 //Feeding Time 4 Button
    { myGLCD.setColor(255, 0, 0);
      myGLCD.fillRoundRect(165, 168, 315, 188);
      setFont(SMALL, 255, 255, 255, 255, 0, 0);
      myGLCD.print("Feeding Time 4", 184, 172); 
      setFont(SMALL, 255, 0, 0, 0, 0, 0);     
      myGLCD.print("This time has not", 172, 133);
      myGLCD.print("been scheduled", 184, 146);}
  else
    { myGLCD.setColor(0, 255, 0);
      myGLCD.fillRoundRect(165, 168, 315, 188);
      setFont(SMALL, 0, 0, 0, 0, 255, 0);
      myGLCD.print("Feeding Time 4", 184, 172);
      timeDispH=feedFish4H; timeDispM=feedFish4M;
      if (setTimeFormat==0) { xTimeH=200;}
      if (setTimeFormat==1) { xTimeH=176;}      
      ReadFromEEPROM();
      if ((timeDispH>=0) && (timeDispH<=11)) { AM_PM=1;}
      else { AM_PM=2;}          
      yTime=137; xColon=xTimeH+32;
      xTimeM10=xTimeH+48; xTimeM1=xTimeH+64; xTimeAMPM=xTimeH+96;      
      timeCorrectFormat();}      

  myGLCD.setColor(255, 255, 255);
  for (int x=0; x<2; x++)
    { for (int y=0; y<2; y++)
        { myGLCD.drawRoundRect((x*160)+5, (y*148)+20, (x*160)+155, (y*148)+40); }
    }    
}


void feedingTimeOutput()
{
  if ((FEEDTime1==1) && (feedFish1H==t.hour) && (feedFish1M==t.min) 
     && (t.sec>=0 || t.sec<5))
    { if (setAutoStop==1)
        { waveMakerOff=true;
          digitalWrite(WaveMakerTop, LOW);
          digitalWrite(WaveMakerBottom, LOW);
          fiveTillBackOn1=0;
          FeedWaveCtrl_1=true;}
      digitalWrite(autoFeeder, HIGH);
      delay(6000);
      t=rtc.getTime();
      digitalWrite(autoFeeder, LOW); 
    }
 if (FeedWaveCtrl_1==true)
    { fiveTillBackOn1++;
      if (fiveTillBackOn1>60)                       //60 is 5 minutes (60/12=5) 
        { waveMakerOff=false; FeedWaveCtrl_1=false;}
    }

  if ((FEEDTime2==1) && (feedFish2H==t.hour) && (feedFish2M==t.min)
     && (t.sec>=0 || t.sec<5))  
    { if (setAutoStop==1)
        { waveMakerOff=true;
          digitalWrite(WaveMakerTop, LOW);
          digitalWrite(WaveMakerBottom, LOW);
          fiveTillBackOn2=0;
          FeedWaveCtrl_2=true;}
      digitalWrite(autoFeeder, HIGH);
      delay(6000);
      t=rtc.getTime();
      digitalWrite(autoFeeder, LOW); 
    }
 if (FeedWaveCtrl_2==true)
    { fiveTillBackOn2++;
      if (fiveTillBackOn2>60) 
        { waveMakerOff=false; FeedWaveCtrl_2=false;}
    }
    
  if ((FEEDTime3==1) && (feedFish3H==t.hour) && (feedFish3M==t.min)
     && (t.sec>=0 || t.sec<5))
    { if (setAutoStop==1)
        { waveMakerOff=true;
          digitalWrite(WaveMakerTop, LOW);
          digitalWrite(WaveMakerBottom, LOW);
          fiveTillBackOn3=0;
          FeedWaveCtrl_3=true;}
      digitalWrite(autoFeeder, HIGH);
      delay(6000);
      t=rtc.getTime();
      digitalWrite(autoFeeder, LOW); 
    }
 if (FeedWaveCtrl_3==true)
    { fiveTillBackOn3++;
      if (fiveTillBackOn3>60) 
        { waveMakerOff=false; FeedWaveCtrl_3=false;}
    }    

  if ((FEEDTime4==1) && (feedFish4H==t.hour) && (feedFish4M==t.min)
     && (t.sec>=0 || t.sec<5))
    { if (setAutoStop==1)
        { waveMakerOff=true;
          digitalWrite(WaveMakerTop, LOW);
          digitalWrite(WaveMakerBottom, LOW);
          fiveTillBackOn4=0;
          FeedWaveCtrl_4=true;}
      digitalWrite(autoFeeder, HIGH);
      delay(6000);
      t=rtc.getTime();
      digitalWrite(autoFeeder, LOW); 
    }
 if (FeedWaveCtrl_4==true)
    { fiveTillBackOn4++;
      if (fiveTillBackOn4>60) 
        { waveMakerOff=false; FeedWaveCtrl_4=false;}
    }    
}
/*********************** END OF AUTOMATIC FEEDER SETTINGS SCREEN **********************/  


/***** SET AUTOMATIC FEEDER TIMES SCREEN ********** dispScreen = 13 *******************/
void setFeederTimesScreen(boolean refreshAll=true) 
{
  if (feedTime==1)
    { printHeader("Set Feeding Time 1"); }
  if (feedTime==2)
    { printHeader("Set Feeding Time 2");}
  if (feedTime==3)
    { printHeader("Set Feeding Time 3");}
  if (feedTime==4)
    { printHeader("Set Feeding Time 4");}

  if (refreshAll)
  { 
   rtcSetMin=t.min; rtcSetHr=t.hour; 
  
   myGLCD.setColor(64, 64, 64);
   myGLCD.drawRect(0, 196, 319, 194);
   printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
   printButton("SAVE", prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3], SMALL);
   printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);   

   feedingTimeOnOff();

   drawUpButton(houP[0], houP[1]);                //hour up
   drawUpButton(minP[0], minP[1]);                //min up
   drawDownButton(houM[0], houM[1]);              //hour down
   drawDownButton(minM[0], minM[1]);              //min down
   if (setTimeFormat==1)
     { drawUpButton(ampmP[0], ampmP[1]);          //AM/PM up   
       drawDownButton(ampmM[0], ampmM[1]);}       //AM/PM down  
  }    
  
  timeDispH=rtcSetHr; timeDispM=rtcSetMin;
  xTimeH=107; yTime=68;  xColon=xTimeH+42;
  xTimeM10=xTimeH+70; xTimeM1=xTimeH+86; xTimeAMPM=xTimeH+155;
  timeChange();
}
/********************** END OF SET AUTOMATIC FEEDER TIMES SCREEN **********************/


/******* GENERAL SETTINGS PG1 SCREEN ************ dispScreen = 14 *********************/
void generalSettingsScreen_1()
{
  printHeader("View/Change General Settings: Page 1");  

  myGLCD.setColor(64, 64, 64);
  myGLCD.drawRect(0, 196, 319, 194);
  for (int y=0; y<4; y++)
    { myGLCD.drawLine(0, (y*31)+70, 319, (y*31)+70); }

  printButton("<< BACK", backGS[0], backGS[1], backGS[2], backGS[3], SMALL);
  printButton("NEXT >>", nextGS[0], nextGS[1], nextGS[2], nextGS[3], SMALL);
  printButton("SAVE", prSAVEgs[0], prSAVEgs[1], prSAVEgs[2], prSAVEgs[3], SMALL);
  printButton("CANCEL", canCgs[0], canCgs[1], canCgs[2], canCgs[3], SMALL); 
    
  setFont(SMALL, 0, 255, 0, 0, 0, 0);
  myGLCD.print("Calendar Format", 25, 36);  
  myGLCD.print("Show Day of Week", 25, 80);    
  myGLCD.print("Time Format", 25, 111);  
  myGLCD.print("Temperature Scale", 25, 142);    
  myGLCD.print("Fan Startup Temps", 25, 173);    

  genSetSelect_1();
}
/************************ END OF GENERAL SETTINGS PAGE 1 SCREEN ***********************/ 


/******* GENERAL SETTINGS PG2 SCREEN ************ dispScreen = 15 *********************/
void generalSettingsScreen_2()
{
  printHeader("View/Change General Settings: Page 2");  

  myGLCD.setColor(64, 64, 64);
  myGLCD.drawLine(0, 88, 319, 88);
  myGLCD.drawLine(0, 162, 319, 162);
  myGLCD.drawRect(0, 196, 319, 194);
  
  printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
  printButton("SAVE", prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3], SMALL);
  printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);
    
  setFont(SMALL, 0, 255, 0, 0, 0, 0);
  myGLCD.print("Dim LEDs Temp.", 25, 45);  
  myGLCD.print("Screensaver", 25, 119);    
  myGLCD.print("Auto-Stop on Feed", 25, 173);    

  genSetSelect_2();
}
/************************ END OF GENERAL SETTINGS PAGE 2 SCREEN ***********************/ 


/******** CHANGE FAN TEMPS SCREEN ************* dispScreen = 16 ***********************/
void ChangeFanTempsScreen(boolean refreshAll=false)
{
  String deg;
  if (refreshAll)
    {
     if ((setTempToBeginHoodFanC==0) && (setTempScale==0)) 
       { setTempToBeginHoodFanC = 29.0;}                         //change to 29 deg C
     if ((setTempToBeginSumpFanC==0) && (setTempScale==0)) 
       { setTempToBeginSumpFanC = 29.0;}                         //change to 29 deg C
     if (((setTempToBeginHoodFanF==0) || (setTempToBeginHoodFanF==setTempToBeginHoodFanC)) 
        && (setTempScale==1)) 
       { setTempToBeginHoodFanF = 84.2;}                         //change to 84 deg F
     if (((setTempToBeginSumpFanF==0) || (setTempToBeginSumpFanF==setTempToBeginSumpFanC)) 
        && (setTempScale==1)) 
       { setTempToBeginSumpFanF = 84.2;}                         //change to 84 deg F    

     if (setTempScale==1)
       { temp2beHFan=setTempToBeginHoodFanF;
         temp2beSFan=setTempToBeginSumpFanF; }
     else
       { temp2beHFan=setTempToBeginHoodFanC;
         temp2beSFan=setTempToBeginSumpFanC;} 
         
     printHeader("View/Change Fans Startup Temperatures");    
 
     myGLCD.setColor(64, 64, 64);
     myGLCD.drawRect(0, 103, 319, 105);   

     setFont(SMALL, 0, 255, 0, 0, 0, 0);
     myGLCD.print("MAIN HOOD FANS", CENTER, 20);  
     myGLCD.print("SUMP LIGHT FAN", CENTER, 111);   

     if (setTempScale==1) {deg ="F";}                //Print deg C or deg F
       else {deg = "C";}
     degC_F=deg;
     char bufferDeg[2];
     degC_F.toCharArray(bufferDeg,2);
     
     setFont(SMALL, 255, 255, 255, 0, 0, 0);
     myGLCD.print("Startup Temperature in", 60, 41);  
     myGLCD.drawCircle(245, 43, 1);              
     myGLCD.print(bufferDeg, 250, 41);
     myGLCD.print(":", 258, 41);
     myGLCD.print("Startup Temperature in", 60, 132);  
     myGLCD.drawCircle(245, 134, 1);              
     myGLCD.print(bufferDeg, 250, 132);
     myGLCD.print(":", 258, 132);

     if (setTempScale==1)
       { myGLCD.print("(68.0--99.9)", CENTER, 86);
         myGLCD.print("(68.0--99.9)", CENTER, 177);}
     else 
       { myGLCD.print("(20.0--37.7)", CENTER, 86);
         myGLCD.print("(20.0--37.7)", CENTER, 177);}

     printButton("-", HoodFanTm[0], HoodFanTm[1], HoodFanTm[2], HoodFanTm[3], LARGE);
     printButton("+", HoodFanTp[0], HoodFanTp[1], HoodFanTp[2], HoodFanTp[3], LARGE);      
     printButton("-", SumpFanTm[0], SumpFanTm[1], SumpFanTm[2], SumpFanTm[3], LARGE);
     printButton("+", SumpFanTp[0], SumpFanTp[1], SumpFanTp[2], SumpFanTp[3], LARGE);
 
     myGLCD.setColor(64, 64, 64);
     myGLCD.drawRect(0, 196, 319, 194);  
     printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
     printButton("SAVE", prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3], SMALL);
     printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);
    }
    
  setFont(LARGE, 255, 255, 255, 0, 0, 0);
  myGLCD.printNumF(temp2beHFan, 1, CENTER, 61);
  myGLCD.printNumF(temp2beSFan, 1, CENTER, 152);  
}
/*************************** END OF CHANGE FAN TEMPS SCREEN ***************************/ 


/******** DIM LEDs AT TEMP SCREEN ************* dispScreen = 17 ***********************/
void DimLEDsAtTempScreen()
{
  printHeader("View/Change LEDs' Dimming Temperature");    
  
  setFont(SMALL, 0, 255, 0, 0, 0, 0);
  myGLCD.print("If temp reaches:", 39, 59);  
  String deg;
  if (setTempScale==1) {deg ="F,";}                //Print deg C or deg F
    else {deg = "C,";}
  degC_F=deg;
  char bufferDeg[3];
  degC_F.toCharArray(bufferDeg,3);  
  myGLCD.drawCircle(273, 61, 1);              
  myGLCD.print(bufferDeg, 278, 59);  

  myGLCD.print("dim the LEDs to:", 39, 140);  
  myGLCD.print("%.", 273, 140);  
  
  myGLCD.setColor(64, 64, 64);  
  myGLCD.drawRoundRect(175, 31, 265, 96);
  drawUpButton(235, 36);  
  drawDownButton(235, 66);
  myGLCD.setColor(64, 64, 64);    
  myGLCD.drawRoundRect(175, 112, 265, 177);
  drawUpButton(235, 117);  
  drawDownButton(235, 147);

  if (setTempScale==1)  
    { TempLEDsDimTemp=setLEDsDimTempF;}
  else {TempLEDsDimTemp=setLEDsDimTempC;}
  TempLEDsDimPercent=setLEDsDimPercent;  
  setFont(LARGE, 0, 255, 0, 0, 0, 0);
  if (TempLEDsDimTemp>=100)
    { myGLCD.printNumI(TempLEDsDimTemp, 181, 55);}
  if ((TempLEDsDimTemp<=99)&&(TempLEDsDimTemp>=10))
    { myGLCD.printNumI(TempLEDsDimTemp, 189, 55);}
  if (TempLEDsDimTemp<=9)
    { myGLCD.printNumI(TempLEDsDimTemp, 198, 55);}  
  if (TempLEDsDimPercent>=100)
    { myGLCD.printNumI(TempLEDsDimPercent, 181, 136);}
  if ((TempLEDsDimPercent<=99)&&(TempLEDsDimPercent>=10))
    { myGLCD.printNumI(TempLEDsDimPercent, 189, 136);}
  if (TempLEDsDimPercent<=9)
    { myGLCD.printNumI(TempLEDsDimPercent, 198, 136);}              
  
  myGLCD.setColor(64, 64, 64);
  myGLCD.drawRect(0, 196, 319, 194);  
  printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
  printButton("SAVE", prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3], SMALL);
  printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);  
}
/*************************** END OF DIM LEDs AT TEMP SCREEN ***************************/ 


/***** SET SCREENSAVER WAIT TIME SCREEN ********** dispScreen = 18 ********************/
void ScreensaverSettingsScreen()
{
  printHeader("View/Change Screensaver Settings");

  myGLCD.setColor(64, 64, 64);
  myGLCD.drawLine(0, 45, 319, 45);   
  
  setFont(SMALL, 0, 255, 0, 0, 0, 0);
  myGLCD.print("Choose Screensaver", 25, 24); 

  myGLCD.print("Wait:", 67, 130);  
  myGLCD.print("minutes", 213, 130);
  myGLCD.setColor(64, 64, 64);  
  myGLCD.drawRoundRect(115, 102, 205,167);
  drawUpButton(175, 107);  
  drawDownButton(175, 137);
  
  setFont(LARGE, 0, 255, 0, 0, 0, 0);
  TempSSminutes=setSSmintues;
  if (TempSSminutes>=10)
    { myGLCD.printNumI(TempSSminutes, 129, 126);}
  else { myGLCD.printNumI(TempSSminutes, 137, 126);}
  
  myGLCD.setColor(64, 64, 64);
  myGLCD.drawRect(0, 196, 319, 194);  
  printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
  printButton("SAVE", prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3], SMALL);
  printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL); 
 
  ScreensaverSelect();
}
/********************** END OF SET SCREENSAVER WAIT TIME SCREEN ***********************/ 


/************** ABOUT SCREEN ****************** dispScreen = 19 ***********************/
void AboutScreen()
{ 
  printHeader("Jarduino Aquarium Controller v.1.2");
  
  setFont(SMALL, 0, 255, 0, 0, 0 , 0);
  myGLCD.print("BETA Release Date: May 2012", CENTER, 20);
  myGLCD.print("Written by Jamie Jardin", CENTER, 32);
  
  myGLCD.print("Main code based on Stilo", 5, 52);
  myGLCD.print("http://code.google.com/p/stilo/", 5, 64);
  
  myGLCD.print("LED controlling algorithm based on", 5, 84);
  myGLCD.print("Krusduino by Hugh Dangerfield", 5, 96);
  myGLCD.print("and Dave Rosser", 5, 108);
  myGLCD.print("http://code.google.com/p/dangerduino/", 5, 120);

  myGLCD.print("Special Thanks: Hugh & Dave, Kev Tench,", 5, 140); 
  myGLCD.print("Mark Chester, Ned Simpson, Stilo, Neil", 5, 152);
  myGLCD.print("Williams, my Dad, and my loving Wife", 5, 164);
  
  myGLCD.setColor(64, 64, 64);
  myGLCD.drawRect(0, 196, 319, 194);
  printButton("<< BACK", back[0], back[1], back[2], back[3], SMALL);
  printButton("CANCEL", canC[0], canC[1], canC[2], canC[3], SMALL);  
} 
/********************************* END OF ABOUT SCREEN ********************************/  


/************************************ TOUCH SCREEN ************************************/
void processMyTouch()
{
  myTouch.read();
  x=myTouch.getX();
  y=myTouch.getY();
  
  returnTimer=0;
  screenSaverCounter=0;  
    
  if ((x>=canC[0]) && (x<=canC[2]) && (y>=canC[1]) && (y<=canC[3])  //press cancel 
     && (dispScreen!=0) && (dispScreen!=5) && (dispScreen!=6) && (dispScreen!=8) 
     && (dispScreen!=9) && (dispScreen!=11) && (dispScreen!=14))
    {
     waitForIt(canC[0], canC[1], canC[2], canC[3]);
     LEDtestTick = false;
     waveMakerOff = false;     
     ReadFromEEPROM();     
     dispScreen=0;
     clearScreen();
     mainScreen(true);
    }  else
  if ((x>=back[0]) && (x<=back[2]) && (y>=back[1]) && (y<=back[3])  //press back    
     && (dispScreen!=0) && (dispScreen!=1) && (dispScreen!=5) && (dispScreen!=6)
     && (dispScreen!=8) && (dispScreen!=9) && (dispScreen!=11) && (dispScreen!=13)
     && (dispScreen!=14) && (dispScreen!=15) && (dispScreen!=16) && (dispScreen!=17)
     && (dispScreen!=18))
    {
     waitForIt(back[0], back[1], back[2], back[3]);
     LEDtestTick = false;
     waveMakerOff = false;          
     ReadFromEEPROM();
     dispScreen=1;
     clearScreen();
     menuScreen();
    }      
    else
  {
  switch (dispScreen) {
  case 0:     //--------------- MAIN SCREEN (Press Any Key) ---------------
    dispScreen=1;
    clearScreen();
    menuScreen();
  break;
      
  case 1:     //--------------------- MENU SCREEN -------------------------
    if ((x>=tanD[0]) && (x<=tanD[2]))                      //first column
      {
       if ((y>=tanD[1]) && (y<=tanD[3]))                   //press Date & Clock Screen
         {
          waitForIt(tanD[0], tanD[1], tanD[2], tanD[3]);
          if ((timeDispH>=0) && (timeDispH<=11)) { AM_PM=1;}
          else { AM_PM=2;}          
          dispScreen=2;
          clearScreen();
          clockScreen();
          t_temp = rtc.getTime();
          t_temp.dow=calcDOW(rtcSetDy, rtcSetMon, rtcSetYr);
          showDOW(t_temp.dow);             
         } 
       if ((y>=temC[1]) && (y<=temC[3]))                   //press H2O Temp Control
         {
          waitForIt(temC[0], temC[1], temC[2], temC[3]);
          ReadFromEEPROM();
          dispScreen=3;
          clearScreen();
          tempScreen(true); 
         }
       if ((y>=wave[1]) && (y<=wave[3]))                   //press Wavemaker Screen
         {
          waitForIt(wave[0], wave[1], wave[2], wave[3]);
          dispScreen=10;
          clearScreen();
          WaveMakerScreen(); 
          WaveMakerStatusScreen();
         }
       if ((y>=gSet[1]) && (y<=gSet[3]))                   //press General Settings
         {
          waitForIt(gSet[0], gSet[1], gSet[2], gSet[3]);
          dispScreen=14;
          clearScreen();
          generalSettingsScreen_1();
         }
      }
    if ((x>=tesT[0]) && (x<=tesT[2]))                      //second column
      {
        if ((y>=tesT[1]) && (y<=tesT[3]))                  //press LED Testing page
         {
          waitForIt(tesT[0], tesT[1], tesT[2], tesT[3]);
          dispScreen=4;
          clearScreen();
          ledTestOptionsScreen();
         }  
        if  ((y>=ledChM[1]) && (y<=ledChM[3]))             //press Change LED values
         {
          waitForIt(ledChM[0], ledChM[1], ledChM[2], ledChM[3]);
          dispScreen=7;
          clearScreen();
          ledColorViewScreen(); 
         } 
        if  ((y>=aFeed[1]) && (y<=aFeed[3]))               //press Automatic Feeder screen
         {
          waitForIt(aFeed[0], aFeed[1], aFeed[2], aFeed[3]);
          dispScreen=12;
          clearScreen();
          autoFeederScreen();
         }
        if  ((y>=about[1]) && (y<=about[3]))               //press About sketch
         {
          waitForIt(about[0], about[1], about[2], about[3]);
          dispScreen=19;
          clearScreen();
          AboutScreen(); 
         }         
      }
  break;
      
  case 2:     //--------------- CLOCK & DATE SETUP SCREEN -----------------
    if ((x>=prSAVE[0]) && (x<=prSAVE[2]) && (y>=prSAVE[1]) && (y<=prSAVE[3])) //press SAVE
      {
       waitForIt(prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3]);
       if (setTimeFormat==1)
         {
          if ((rtcSetHr==0) && (AM_PM==2))
            { rtcSetHr+=12;}
          if (((rtcSetHr>=1) && (rtcSetHr<=11)) && (AM_PM==2))
            { rtcSetHr+=12;}  
          if (((rtcSetHr>=12) && (rtcSetHr<=23)) && (AM_PM==1))
            { rtcSetHr-=12;}           
         }
       
       rtc.setTime(rtcSetHr, rtcSetMin, 0);
       rtc.setDate(rtcSetDy, rtcSetMon, rtcSetYr);
       
       dispScreen=0;
       clearScreen();
       mainScreen(true);
      }
    else
     {
      if ((y>=houU[1]) && (y<=houU[3]))                            //FIRST ROW (TIME UP)
        {
         if ((x>=houU[0]) && (x<=houU[2]))                         //press hour up
           {
            waitForIt(houU[0], houU[1], houU[2], houU[3]);
            rtcSetHr++;
            if (rtcSetHr>=24) 
              { rtcSetHr=0; }
            setFont(SMALL, 255, 255, 255, 0, 0, 0);
            myGLCD.print("Changed!", 21, 28);
           }
         if ((x>=minU[0]) && (x<=minU[2]))                         //press min up
           {
            waitForIt(minU[0], minU[1], minU[2], minU[3]);
            rtcSetMin++;
            if (rtcSetMin>=60) {rtcSetMin = 0; }
            setFont(SMALL, 255, 255, 255, 0, 0, 0);
            myGLCD.print("Changed!", 21, 28);
           }
         if ((x>=ampmU[0]) && (x<=ampmU[2]) && (setTimeFormat==1)) //press AMPM up
           {
            waitForIt(ampmU[0], ampmU[1], ampmU[2], ampmU[3]);
            if (AM_PM==1) {AM_PM=2;}
            else {AM_PM=1;}
            setFont(SMALL, 255, 255, 255, 0, 0, 0);
            myGLCD.print("Changed!", 21, 28);
           }
        }
      if ((y>=houD[1]) && (y<=houD[3]))                            //SECOND ROW (TIME DOWN)
        {
         if ((x>=houD[0]) && (x<=houD[2]))                         //press hour down
           {
            waitForIt(houD[0], houD[1], houD[2], houD[3]);             
            rtcSetHr--;
            if (rtcSetHr<0) 
              { rtcSetHr=23; }
            setFont(SMALL, 255, 255, 255, 0, 0, 0);
            myGLCD.print("Changed!", 21, 28);
           }
         if ((x>=minD[0]) && (x<=minD[2]))                         //press min down
           {
            waitForIt(minD[0], minD[1], minD[2], minD[3]);
            rtcSetMin--;
            if (rtcSetMin<0) {rtcSetMin = 59; } 
            setFont(SMALL, 255, 255, 255, 0, 0, 0);
            myGLCD.print("Changed!", 21, 28);
           }
         if ((x>=ampmD[0]) && (x<=ampmD[2]) && (setTimeFormat==1)) //press AMPM down
           {
            waitForIt(ampmD[0], ampmD[1], ampmD[2], ampmD[3]);
            if (AM_PM==1) {AM_PM=2;}
            else {AM_PM=1;}
            setFont(SMALL, 255, 255, 255, 0, 0, 0);
            myGLCD.print("Changed!", 21, 28);
           }           
        }
      if ((y>=dayU[1]) && (y<=dayU[3]))                            //THIRD ROW (DATE UP)
        {
         if (setCalendarFormat==0)                                 //DD/MM/YYYY Format
           {
            if ((x>=dayU[0]) && (x<=dayU[2]))                      //press day up
              {
               waitForIt(dayU[0], dayU[1], dayU[2], dayU[3]);
               rtcSetDy++;
               rtcSetDy=validateDate(rtcSetDy, rtcSetMon, rtcSetYr);
               setFont(SMALL, 255, 255, 255, 0, 0, 0);
               myGLCD.print("Changed!", 21, 118);
              }
            if ((x>=monU[0]) && (x<=monU[2]))                      //press month up
              {
               waitForIt(monU[0], monU[1], monU[2], monU[3]);
               rtcSetMon++;
               rtcSetDy=validateDateForMonth(rtcSetDy, rtcSetMon, rtcSetYr);
               setFont(SMALL, 255, 255, 255, 0, 0, 0);
               myGLCD.print("Changed!", 21, 118);
              }
           } else {
         if (setCalendarFormat==1)                                 //MM/DD/YYYY Format
           {
            if ((x>=dayU[0]) && (x<=dayU[2]))                      //press month up
              {
               waitForIt(dayU[0], dayU[1], dayU[2], dayU[3]);
               rtcSetMon++;
               rtcSetDy=validateDateForMonth(rtcSetDy, rtcSetMon, rtcSetYr);
               setFont(SMALL, 255, 255, 255, 0, 0, 0);
               myGLCD.print("Changed!", 21, 118);
              }
            if ((x>=monU[0]) && (x<=monU[2]))                      //press day up
              {
               waitForIt(monU[0], monU[1], monU[2], monU[3]);
               rtcSetDy++;
               rtcSetDy=validateDate(rtcSetDy, rtcSetMon, rtcSetYr);
               setFont(SMALL, 255, 255, 255, 0, 0, 0);
               myGLCD.print("Changed!", 21, 118);
              }
           }      }
         if ((x>=yeaU[0]) && (x<=yeaU[2]))                         //press year up
           {
            waitForIt(yeaU[0], yeaU[1], yeaU[2], yeaU[3]);
            rtcSetYr++;
            rtcSetDy=validateDateForMonth(rtcSetDy, rtcSetMon, rtcSetYr);
            setFont(SMALL, 255, 255, 255, 0, 0, 0);
            myGLCD.print("Changed!", 21, 118);            
           }
         t_temp = rtc.getTime();
         t_temp.dow=calcDOW(rtcSetDy, rtcSetMon, rtcSetYr);
         showDOW(t_temp.dow);            
        }
      if ((y>=dayD[1]) && (y<=dayD[3]))                            //FOURTH ROW (DATE DOWN)
        {
         if (setCalendarFormat==0)                                 //DD/MM/YYYY Format
           {
            if ((x>=dayD[0]) && (x<=dayD[2]))                      //press day down
              {
               waitForIt(dayD[0], dayD[1], dayD[2], dayD[3]);
               rtcSetDy--;
               rtcSetDy=validateDate(rtcSetDy, rtcSetMon, rtcSetYr);
               setFont(SMALL, 255, 255, 255, 0, 0, 0);
               myGLCD.print("Changed!", 21, 118);
              }
            if ((x>=monD[0]) && (x<=monD[2]))                      //press month down
              {
               waitForIt(monD[0], monD[1], monD[2], monD[3]);
               rtcSetMon--;
               rtcSetDy=validateDateForMonth(rtcSetDy, rtcSetMon, rtcSetYr);
               setFont(SMALL, 255, 255, 255, 0, 0, 0);
               myGLCD.print("Changed!", 21, 118);
              }
           } else {
         if (setCalendarFormat==1)                                 //MM/DD/YYYY Format
           {
            if ((x>=dayD[0]) && (x<=dayD[2]))                      //press month down
              {
               waitForIt(dayD[0], dayD[1], dayD[2], dayD[3]);
               rtcSetMon--;
               rtcSetDy=validateDateForMonth(rtcSetDy, rtcSetMon, rtcSetYr);
               setFont(SMALL, 255, 255, 255, 0, 0, 0);
               myGLCD.print("Changed!", 21, 118);               
              }
            if ((x>=monD[0]) && (x<=monD[2]))                      //press day down
              {
               waitForIt(monD[0], monD[1], monD[2], monD[3]);
               rtcSetDy--;
               rtcSetDy=validateDate(rtcSetDy, rtcSetMon, rtcSetYr);
               setFont(SMALL, 255, 255, 255, 0, 0, 0);
               myGLCD.print("Changed!", 21, 118);               
              }
           }      }   
         if ((x>=yeaD[0]) && (x<=yeaD[2]))                         //press year down
           {
            waitForIt(yeaD[0], yeaD[1], yeaD[2], yeaD[3]);
            rtcSetYr--;
            rtcSetDy=validateDateForMonth(rtcSetDy, rtcSetMon, rtcSetYr);
            setFont(SMALL, 255, 255, 255, 0, 0, 0);
            myGLCD.print("Changed!", 21, 118);            
           }
         t_temp = rtc.getTime();
         t_temp.dow=calcDOW(rtcSetDy, rtcSetMon, rtcSetYr);
         showDOW(t_temp.dow);             
        }
      clockScreen(false); 
     }       
  break;
  
  case 3:     //------------------ H20 TEMPERATURE CONTROL ---------------
    if ((x>=prSAVE[0]) && (x<=prSAVE[2]) && (y>=prSAVE[1]) && (y<=prSAVE[3])) //press SAVE
      {
       waitForIt(prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3]);
       setTempC = temp2beS;
       setTempF = temp2beS;
       offTempC = temp2beO;
       offTempF = temp2beO;
       alarmTempC = temp2beA;
       alarmTempF = temp2beA;
       if (setTempScale==0)                      //Celsius to Farenheit (Consistency Conversion)
         {
          setTempF=((1.8*setTempC)+32.05);
          offTempF=((1.8*offTempC)+0.05);
          alarmTempF=((1.8*alarmTempC)+0.05);
         }    
       if (setTempScale==1)                      //Farenheit to Celsius (Consistency Conversion)
         {
          setTempC=((.55556*(setTempF-32))+.05);
          offTempC=(.55556)*offTempF+.05;
          alarmTempC=(.55556)*alarmTempF+.05;
         }
       dispScreen=0;
       SaveTempToEEPROM();
       clearScreen();
       mainScreen(true);
       } else
         setFont(LARGE, 255, 255, 255, 0, 0, 0);
         {
          if ((x>=temM[0]) && (x<=temM[2]))                        //first column
            {
             if ((y>=temM[1]) && (y<=temM[3]))                     //press temp minus
               {
                temp2beS -= 0.1;
                if ((setTempScale==1) && (temp2beS <= 50)){
                  temp2beS = 50; }
                if ((setTempScale==0) && (temp2beS <= 10)) {
                  temp2beS = 10; }
                tempScreen();
               }
             if ((y>=offM[1]) && (y<=offM[3]))                     //press offset minus
               {
                temp2beO -= 0.1;
                if (temp2beO < 0.1) {
                  temp2beO = 0.0; }
                tempScreen();
               }          
             if ((y>=almM[1]) && (y<=almM[3]))                     //press alarm minus
               {
                temp2beA -= 0.1;
                if (temp2beA < 0.1) {
                  temp2beA = 0.0;  }
                tempScreen();
               }
            }
          if ((x>=temP[0]) && (x<=temP[2]))                        //second column
            {
             if ((y>=temP[1]) && (y<=temP[3]))                     //press temp plus
               {
                temp2beS += 0.1;
                if ((setTempScale==1) && (temp2beS >= 104)){
                  temp2beS = 104; }
                if ((setTempScale==0) && (temp2beS >= 40)) {
                  temp2beS = 40; }
                tempScreen();
               }
             if ((y>=offP[1]) && (y<=offP[3]))                     //press offset plus
               {
                temp2beO += 0.1;
                if (temp2beO >= 10) {
                  temp2beO = 9.9; }
                tempScreen();
               }
             if ((y>=almP[1]) && (y<=almP[3]))                     //press alarm plus
               {
                temp2beA += 0.1;
                if (temp2beA >= 10) {
                  temp2beA = 9.9;  }
                tempScreen();
               }
            }
         }
  break;
  
  case 4:     // -------------- LED TEST OPTIONS SCREEN -----------------    
    if ((x>=tstLA[0]) && (x<=tstLA[2]) && (y>=tstLA[1]) && (y<=tstLA[3]))   //Test LED Array Output
      {
       waitForIt(tstLA[0], tstLA[1], tstLA[2], tstLA[3]); 
       dispScreen=5;
       clearScreen();
       testArrayScreen(true);
      }  
    if ((x>=cntIL[0]) && (x<=cntIL[2]) && (y>=cntIL[1]) && (y<=cntIL[3]))   //Test Individual LEDs
      {
       waitForIt(cntIL[0], cntIL[1], cntIL[2], cntIL[3]); 
       dispScreen=6;
       clearScreen();
       testIndLedScreen();
       colorLEDtest = true;
      }         
  break;
        
  case 5:     //---------------- TEST LED ARRAY SCREEN ------------------
    if ((x>=back[0]) && (x<=back[2]) && (y>=back[1]) && (y<=back[3])   //press back    
       && (LEDtestTick==false))    
      {
       waitForIt(back[0], back[1], back[2], back[3]);
       LEDtestTick = false;
       ReadFromEEPROM();     
       dispScreen=4;
       clearScreen();
       ledTestOptionsScreen();
      }      
    if ((x>=canC[0]) && (x<=canC[2]) && (y>=canC[1]) && (y<=canC[3])   //press CANCEL
       && (LEDtestTick==false))    
      {
       waitForIt(canC[0], canC[1], canC[2], canC[3]);
       LEDtestTick = false;
       ReadFromEEPROM();     
       dispScreen=0;
       clearScreen();
       mainScreen(true);
      }  
    if ((x>=stsT[0]) && (x<=stsT[2]) && (y>=stsT[1]) && (y<=stsT[3]))  //press start/stop test
      {
       waitForIt(stsT[0], stsT[1], stsT[2], stsT[3]); 
           
       if (LEDtestTick) {
         LEDtestTick = false;
         testArrayScreen(true);
         }
         else {
           LEDtestTick = true;             
           testArrayScreen();
         }
      } else
          {
           if ((x>=tenM[0]) && (x<=tenM[2]) && (y>=tenM[1]) && (y<=tenM[3]))   //press -10s
             {
              min_cnt -= 10;
              if (min_cnt<0) {
                min_cnt= 0; }
              delay(50);
             }
           if ((x>=tenP[0]) && (x<=tenP[2]) && (y>=tenP[1]) && (y<=tenP[2]))   //press +10s
             {
              min_cnt += 10;
              if (min_cnt>1440) {
                min_cnt = 1440; }
              delay(50);
             }
          }
  break;
        
  case 6:     // --------------- TEST INDIVIDUAL LED SCREEN --------------    
    int CL_check, CL_check2;
    if ((x>=2) && (x<=28) && (y>=181) && (y<=224))             //press back    
      {
       waitForIt(2, 181, 28, 224);
       LEDtestTick = false;
       dispScreen=4;
       clearScreen();
       ledTestOptionsScreen();
       colorLEDtest = false;
      }
    if ((y>=44)&&(y<=172))                                     //Slider Bars
      {
       for (int i=0; i<7; i++) 
         {
          if ((x>=(i*38)+39) && (x<=(i*38)+69)) 
            {
             if (i==0) {sbR=0; sbG=150; sbB=0;                 //SUMP LED Slider
               sbX1=39; sbX2=69; SliderBars();}
             if (i==1) {sbR=255; sbG=0; sbB=0;                 //RED LED Slider
               sbX1=77; sbX2=107; SliderBars();}        
             if (i==2) {sbR=255; sbG=255; sbB=255;             //WHITE LED Slider
               sbX1=115; sbX2=145; SliderBars();}
             if (i==3) {sbR=9; sbG=184; sbB=255;               //BLUE LED Slider
               sbX1=153; sbX2=183; SliderBars();}
             if (i==4) {sbR=58; sbG=95; sbB=205;               //ROYAL BLUE LED Slider
               sbX1=191; sbX2=221; SliderBars();}
             if (i==5) {sbR=224; sbG=102; sbB=255;             //UV LED Slider
               sbX1=229; sbX2=259; SliderBars();}
             if (i==6) {sbR=176; sbG=176; sbB=176;             //LUNAR LED Slider
               sbX1=267; sbX2=297; SliderBars();}
            }
           }
       }
      
    if ((y>=17) && (y<=39))                                    //Up buttons were touched
      {
       for (int i=0; i<7; i++) 
         {
          if ((x>=(i*38)+39) && (x<=(i*38)+69)) 
            {
             if (i==0) {sbR=0; sbG=150; sbB=0;                 //SUMP +
               sbX1=39; sbX2=69; ySMP+=1; UpDnButtonSlide();}
             if (i==1) {sbR=255; sbG=0; sbB=0;                 //RED +
               sbX1=77; sbX2=107; yRED+=1; UpDnButtonSlide();}
             if (i==2) {sbR=255; sbG=255; sbB=255;             //WHITE +
               sbX1=115; sbX2=145; yWHT+=1; UpDnButtonSlide();}                
             if (i==3) {sbR=9; sbG=184; sbB=255;               //BLUE +
               sbX1=153; sbX2=183; yBLU+=1; UpDnButtonSlide();}                
             if (i==4) {sbR=58; sbG=95; sbB=205;               //ROYAL BLUE +
               sbX1=191; sbX2=221; yRBL+=1; UpDnButtonSlide();}
             if (i==5) {sbR=224; sbG=102; sbB=255;             //UV +
               sbX1=229; sbX2=259; yUVL+=1; UpDnButtonSlide();}                
             if (i==6) {sbR=176; sbG=176; sbB=176;             //LUNAR +
               sbX1=267; sbX2=297; yLUN+=1; UpDnButtonSlide();}
            }
         }
      }   

    if ((y>=200) && (y<=222))                                  //Down buttons were touched
      {
       for (int i=0; i<7; i++) 
         {
          if ((x>=(i*38)+39) && (x<=(i*38)+69)) 
            {
             if (i==0) {sbR=0; sbG=150; sbB=0;                 //SUMP -
               sbX1=39; sbX2=69; ySMP-=1; UpDnButtonSlide();}
             if (i==1) {sbR=255; sbG=0; sbB=0;                 //RED -
               sbX1=77; sbX2=107; yRED-=1; UpDnButtonSlide();}
             if (i==2) {sbR=255; sbG=255; sbB=255;             //WHITE -
               sbX1=115; sbX2=145; yWHT-=1; UpDnButtonSlide();}                
             if (i==3) {sbR=9; sbG=184; sbB=255;               //BLUE -
               sbX1=153; sbX2=183; yBLU-=1; UpDnButtonSlide();}                
             if (i==4) {sbR=58; sbG=95; sbB=205;               //ROYAL BLUE -
               sbX1=191; sbX2=221; yRBL-=1; UpDnButtonSlide();}
             if (i==5) {sbR=224; sbG=102; sbB=255;             //UV -
               sbX1=229; sbX2=259; yUVL-=1; UpDnButtonSlide();}                
             if (i==6) {sbR=176; sbG=176; sbB=176;             //LUNAR -
               sbX1=267; sbX2=297; yLUN-=1; UpDnButtonSlide();}
            }
         }
      }      
  break;
    
  case 7:     // ----------- VIEW INDIVIDUAL LED COLORS SCREEN -----------    
    if ((x>=10) && (x<=150))                             //first column
      {
       if ((y>=20) && (y<=50))                           //View White LEDs Array
         {
          waitForIt(10, 20, 150, 50); 
          dispScreen=8;
          COLOR = WHITE;
          clearScreen();
          ledValuesScreen();
         }  
       if ((y>=60) && (y<=90))                           //View Royal Blue LEDs Array
         {
          waitForIt(10, 60, 150, 90); 
          dispScreen=8;
          COLOR = ROYAL;
          clearScreen();
          ledValuesScreen();
         }
       if ((y>=100) && (y<=130))                         //View Red LEDs Array
         {
          waitForIt(10, 100, 150, 130); 
          dispScreen=8;
          COLOR = RED;
          clearScreen();
          ledValuesScreen();
         }  
      }
    if ((x>=170) && (x<=310))                            //second column
      {
       if ((y>=20) && (y<=50))                           //View Blue LEDs Array
         { 
          waitForIt(170, 20, 310, 50); 
          dispScreen=8;
          COLOR = BLUE;
          clearScreen();
          ledValuesScreen();
         }         
       if ((y>=60) && (y<=90))                           //View Ultra LEDs Array
         {
          waitForIt(170, 60, 310, 90); 
          dispScreen=8;
          COLOR = ULTRA;
          clearScreen();
          ledValuesScreen();
         }  
       if ((y>=100) && (y<=130))                         //View Sump LEDs Array
         {
          waitForIt(170, 100, 310, 130); 
          dispScreen=8;
          COLOR = SUMP;
          clearScreen();
          ledValuesScreen();
         }         
      }
    if ((x>=90) && (x<=230) && (y>=140) && (y<=170))     //View Moon LEDs MaxI
      {
       waitForIt(90, 140, 230, 170); 
       ReadFromEEPROM();
       dispScreen=8;
       COLOR = MOON;
       clearScreen();
       ledValuesScreen();
      }
  break;    
    
  case 8:     // -------------- SHOW LED VALUES TABLE SCREEN -------------
    if ((x>=back[0]) && (x<=back[2]) && (y>=back[1]) && (y<=back[3]))   //press MORE COLORS
      {
       waitForIt(back[0], back[1], back[2], back[3]);
       ReadFromEEPROM();
       dispScreen=7;
       clearScreen();
       ledColorViewScreen();
      } else 
    if ((x>=ledChV[0]) && (x<=ledChV[2]) && (y>=ledChV[1]) && (y<=ledChV[3]) && (COLOR!=7))   //press CHANGE
      {
       waitForIt(ledChV[0], ledChV[1], ledChV[2], ledChV[3]);
       ReadFromEEPROM();
       dispScreen=9;
       clearScreen(); 
       ledChangeScreen();
      } else 
    if ((x>=eeprom[0]) && (x<=eeprom[2]) && (y>=eeprom[1]) && (y<=eeprom[3]) && (COLOR!=7))  //press SAVE
      {
       waitForIt(eeprom[0], eeprom[1], eeprom[2], eeprom[3]);
       SaveLEDToEEPROM();
       dispScreen=7;
       clearScreen();
       ledColorViewScreen();
      } else 
    if ((x>=prSAVE[0]) && (x<=prSAVE[2]) && (y>=prSAVE[1]) && (y<=prSAVE[3])) //press SAVE
      {
       waitForIt(prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3]);
       SaveMoonLEDToEEPROM();
       dispScreen=7;
       clearScreen();
       ledColorViewScreen();
      } else             
    if ((x>=canC[0]) && (x<=canC[2]) && (y>=canC[1]) && (y<=canC[3]))         //press CANCEL
      {
       waitForIt(canC[0], canC[1], canC[2], canC[3]);
       ReadFromEEPROM();
       LEDtestTick = false;
       dispScreen=0;
       clearScreen();
       mainScreen(true);
      } else               

    if ((x>=MINiM[0]) && (x<=MINiM[2]) && (y>=MINiM[1]) && (y<=MINiM[3]))   //press MinI minus
      {
       waitForIt(MINiM[0], MINiM[1], MINiM[2], MINiM[3]);
       tMinI -= 1;
       if (tMinI <= 0) {
         tMinI = 0; }
       MinI = tMinI;         
       setFont(LARGE, 255, 255, 255, 0, 0, 0);
       myGLCD.print("   ", 55, 152);
       if (tMinI<=9)
         { myGLCD.printNumI(tMinI, 71, 152);}
       if ((tMinI>=10)&&(tMinI<=99))
         { myGLCD.printNumI(tMinI, 63, 152);}
       if (tMinI>=100)
         { myGLCD.printNumI(tMinI, 55, 152);}
      } else      
    if ((x>=MINiP[0]) && (x<=MINiP[2]) && (y>=MINiP[1]) && (y<=MINiP[3]))   //press MinI plus
      {
       waitForIt(MINiP[0], MINiP[1], MINiP[2], MINiP[3]);
       tMinI += 5;
         if (tMinI >255) {
       tMinI = 255; }
       MinI = tMinI;         
       setFont(LARGE, 255, 255, 255, 0, 0, 0);
       myGLCD.print("   ", 55, 152);
       if (tMinI<=9)
         { myGLCD.printNumI(tMinI, 71, 152);}
       if ((tMinI>=10)&&(tMinI<=99))
         { myGLCD.printNumI(tMinI, 63, 152);}
       if (tMinI>=100)
         { myGLCD.printNumI(tMinI, 55, 152);}
      } else

    if ((x>=MAXiM[0]) && (x<=MAXiM[2]) && (y>=MAXiM[1]) && (y<=MAXiM[3]))   //press MaxI minus
      {
       waitForIt(MAXiM[0], MAXiM[1], MAXiM[2], MAXiM[3]);
       tMaxI -= 1;
       if (tMaxI <= 0) {
         tMaxI = 0; }
       MaxI = tMaxI;         
       setFont(LARGE, 255, 255, 255, 0, 0, 0);
       myGLCD.print("   ", 217, 152);
       if (tMaxI<=9)
         { myGLCD.printNumI(tMaxI, 233, 152);}
       if ((tMaxI>=10)&&(tMaxI<=99))
         { myGLCD.printNumI(tMaxI, 225, 152);}
       if (tMaxI>=100)
         { myGLCD.printNumI(tMaxI, 217, 152);}
      } else      
    if ((x>=MAXiP[0]) && (x<=MAXiP[2]) && (y>=MAXiP[1]) && (y<=MAXiP[3]))   //press MaxI plus
      {
       waitForIt(MAXiP[0], MAXiP[1], MAXiP[2], MAXiP[3]);
       tMaxI += 5;
         if (tMaxI >255) {
       tMaxI = 255; }
       MaxI = tMaxI;         
       setFont(LARGE, 255, 255, 255, 0, 0, 0);
       myGLCD.print("   ", 217, 152);
       if (tMaxI<=9)
         { myGLCD.printNumI(tMaxI, 233, 152);}
       if ((tMaxI>=10)&&(tMaxI<=99))
         { myGLCD.printNumI(tMaxI, 225, 152);}
       if (tMaxI>=100)
         { myGLCD.printNumI(tMaxI, 217, 152);}
      }
  break;
      
  case 9:     //---------------- CHANGE LED VALUES SCREEN ---------------
    if ((x>=8) && (x<=26) && (y>=47) && (y<=101))                   //press MENU    
      {
       waitForIt(8, 47, 26, 101);
       LEDtestTick = false;
       waveMakerOff = false;   
       TopRows = false;
       ReadFromEEPROM();
       dispScreen=1;
       clearScreen();
       menuScreen();
      } else
    if ((x>=8) && (x<=26) && (y>=106) && (y<=140))                  //press OK
      {
       waitForIt(8, 106, 26, 140);
       dispScreen=8;
       if (COLOR==1) {
         for (int i; i<96; i++) {
           wled[i]=tled[i]; }
         }
       if (COLOR==2) {
         for (int i; i<96; i++) {
           bled[i]=tled[i]; }
         }
       if (COLOR==3) {
         for (int i; i<96; i++) {
           rbled[i]=tled[i]; }
         }
       if (COLOR==4) {
         for (int i; i<96; i++) {
           rled[i]=tled[i]; }
         }  
       if (COLOR==5) {
         for (int i; i<96; i++) {
           uvled[i]=tled[i]; }
         }
       if (COLOR==6) {
         for (int i; i<96; i++) {
           sled[i]=tled[i]; }
         }
         TopRows = false;
         clearScreen();
         ledValuesScreen();
      } else
    if ((x>=8) && (x<=26) && (y>=145) && (y<=219))                  //press CANCEL 
     {
       waitForIt(8, 145, 26, 219);
       LEDtestTick = false;
       waveMakerOff = false;     
       TopRows = false;
       ReadFromEEPROM();     
       dispScreen=0;
       clearScreen();
       mainScreen(true);
     }  else
    if ((y>=15) && (y<=41))                //top row with times was touched
      {
        if ((x>=4) && (x<=316))
          {
            if (TopRows==false)
              {
                myGLCD.setColor(0, 0, 0);
                myGLCD.fillRect(50, 110, 318, 146);
                for (int b=0; b<8; b++)                             //UP Buttons
                  { drawUpButtonSlide((b*36)+34, 45);}
                for (int b=0; b<8; b++)                             //DOWN Buttons
                  { drawDownButtonSlide((b*36)+34, 200);}  
                setFont(SMALL, sbR, sbG, sbB, 0, 0, 0); 
                for (int i=0; i<8; i++)                             //Slider Bar Outlines
                  { myGLCD.drawRect((i*36)+34, 71, (i*36)+64, 185);}    
              }
            TopRows=true;
            int oldLCT = LedChangTime;
            LedChangTime = map(x, 3, 320, 0, 12);                
               
            if (oldLCT != LedChangTime)    //highlight touched time
              {
                myGLCD.setColor(0, 0, 0);
                myGLCD.fillRect((oldLCT*26)+5, 16, (oldLCT*26)+29, 41);
                myGLCD.setColor(100, 100, 100);
                myGLCD.drawRect((oldLCT*26)+4, 16 , (oldLCT*26)+30, 41);
                setFont(SMALL, 0, 255, 255, 0, 0, 0);
                if (oldLCT<5) {
                  myGLCD.printNumI((oldLCT*2), (oldLCT*26)+15, 17);
                  myGLCD.printNumI(((oldLCT*2)+1), (oldLCT*26)+15, 28);}
                else {
                  myGLCD.printNumI((oldLCT*2), (oldLCT*26)+11, 17);
                  myGLCD.printNumI(((oldLCT*2)+1), (oldLCT*26)+11, 28);}
  
                myGLCD.setColor(255, 0, 0);
                if (LedChangTime<5) {
                  myGLCD.fillRect((LedChangTime*26)+5, 17, (LedChangTime*26)+29, 41);
                  setFont(SMALL, 255, 255, 255, 255, 0, 0);
                  myGLCD.printNumI((LedChangTime*2), (LedChangTime*26)+15, 17);
                  myGLCD.printNumI(((LedChangTime*2)+1), (LedChangTime*26)+15, 28);}
                else {
                  myGLCD.fillRect((LedChangTime*26)+5, 17, (LedChangTime*26)+29, 41);
                  setFont(SMALL, 255, 255, 255, 255, 0, 0);
                  myGLCD.printNumI((LedChangTime*2), (LedChangTime*26)+11, 17);
                  myGLCD.printNumI(((LedChangTime*2)+1), (LedChangTime*26)+11, 28);}
                  
                for (int i=0; i<8; i++)    //print led values & bars for highlighted time
                  {
                   int k=(LedChangTime*8)+i;
       
                   y = map(tled[k], 0, 255, 185, 71);
                   sbX1=(i*36)+34; sbX2=(i*36)+64;
                   SliderBarsForChange();
                  }
              }
          }
      } else
   if ((y>=71)&&(y<=185) && (TopRows==true))             //Slider Bars
      {
       for (int i=0; i<8; i++) 
         {
          if ((x>=(i*36)+34) && (x<=(i*36)+64)) 
            {
             int k=(LedChangTime*8)+i;
             sbX1=(i*36)+34; sbX2=(i*36)+64;
             SliderBarsForChange();
             tled[k]=tSlide;
            }
         }
       } else
    if ((y>=45) && (y<=67) && (TopRows==true))           //UP Buttons were touched
      {
        for (int i=0; i<8; i++) 
          {                
           if ((x>=(i*36)+34) && (x<=(i*36)+64)) 
             {
              int k= (LedChangTime*8)+i;
              tled[k]++;
              if (tled[k]>=255) {tled[k]=255; }
              y = map(tled[k], 0, 255, 185, 71);  
              myGLCD.setColor(sbR, sbG, sbB);     
              myGLCD.fillRect((i*36)+34, y, (i*36)+64, 185);
              myGLCD.setColor(0, 0, 0);  
              myGLCD.fillRect((i*36)+35, y, (i*36)+63, 72); 
              myGLCD.setColor(sbR, sbG, sbB);
              myGLCD.drawLine((i*36)+34, 71, (i*36)+64, 71);
                
              setFont(SMALL, sbR, sbG, sbB, 0, 0, 0); 
              if (tled[k]<10) 
                {myGLCD.print("00", (i*36)+39, 187); 
                 myGLCD.printNumI(tled[k], (i*36)+39+16, 187);}
              if ((tled[k]>=10)&&(tled[k]<100)) 
                {myGLCD.print("0", (i*36)+39, 187); 
                 myGLCD.printNumI(tled[k], (i*36)+39+8, 187);}
              if (tled[k]>=100) {myGLCD.printNumI(tled[k], (i*36)+39, 187);}
             }
          }
      } else
    if ((y>=200) && (y<=222) && (TopRows==true))         //DOWN Buttons were touched
      {
        for (int i=0; i<8; i++) 
          {                
           if ((x>=(i*36)+34) && (x<=(i*36)+64)) 
             {
              int k= (LedChangTime*8)+i;
              tled[k]--;
              if (tled[k]<=0) {tled[k]=0; }
              y = map(tled[k], 0, 255, 185, 71);  
              myGLCD.setColor(sbR, sbG, sbB);
              myGLCD.fillRect((i*36)+34, y, (i*36)+64, 185);
              myGLCD.setColor(0, 0, 0);  
              myGLCD.fillRect((i*36)+35, y, (i*36)+63, 72);
              myGLCD.setColor(sbR, sbG, sbB);
              myGLCD.drawLine((i*36)+34, 71, (i*36)+64, 71);              
                
              setFont(SMALL, sbR, sbG, sbB, 0, 0, 0);
              if (tled[k]<10) 
                {myGLCD.print("00", (i*36)+39, 187); 
                 myGLCD.printNumI(tled[k], (i*36)+39+16, 187);}
              if ((tled[k]>=10)&&(tled[k]<100)) 
                {myGLCD.print("0", (i*36)+39, 187); 
                 myGLCD.printNumI(tled[k], (i*36)+39+8, 187);}
              if (tled[k]>=100) {myGLCD.printNumI(tled[k], (i*36)+39, 187);}
             }
          }
      }
    break;
    
    case 10:     //-------------------- WAVEMAKER SCREEN -------------------
      if ((x>=5) && (x<=155) && (y>=20) && (y<=40))      //press Alternating Mode
        {
         waitForIt(5, 20, 155, 40);
         Synch=0;
         WAVE=1;
         dispScreen=11;
         clearScreen();
         WaveMakerSettingsScreen();
         waveMakerOff=true;
         digitalWrite(WaveMakerTop, LOW);
         digitalWrite(WaveMakerBottom, LOW);    
        } else
      if ((x>=165) && (x<=315) && (y>=20) && (y<=40))    //press Synchronous Mode
        {
         waitForIt(165, 20, 315, 40);
         WAVE=2;
         dispScreen=11;
         clearScreen();
         WaveMakerSettingsScreen();
         synchronousSynch();    
         waveMakerOff=true;
         digitalWrite(WaveMakerTop, LOW);
         digitalWrite(WaveMakerBottom, LOW);    
        } else             
      if ((x>=165) && (x<=315) && (y>=46) && (y<=66))    //press Turn Pumps OFF/ON
        {
         waitForIt(165, 46, 315, 66);
         if (WAVE==3) {
           ReadFromEEPROM();
           waveMakerOff=false;
           WaveMakerStatusScreen();
           }
           else {
             MODE=WAVE;
             WAVE=3;
             WaveMakerStatusScreen();
           }
        } else 
      if ((x>=5) && (x<=155) && (y>=46) && (y<=66))      //press Feeding Mode
        {
         waitForIt(5, 46, 155, 66);
         if (WAVE==4) {
           ReadFromEEPROM();
           waveMakerOff=false;
           WaveMakerStatusScreen();
           }
           else {
             ReadFromEEPROM();
             MODE=WAVE;
             WAVE=4;
             countDown  = 5*60 + 0;     //Reset countdown for 5 minutes and zero seconds
             MIN_O = 5;                 //Reset start time at 5 Minutes
             SEC_T = 0;
             SEC_O = 0;
             WaveMakerStatusScreen();
           }
        } 
    break;

    case 11:     //---------------- WAVEMAKER SETTINGS SCREEN --------------
      if ((y>=200) && (y<=220))                          //Bottom Row
        {
         if ((x>=5) && (x<=105))                         //press BACK
           {
            waitForIt(5, 200, 105, 220);
            ReadFromEEPROM();
            waveMakerTest=false;
            waveMakerOff=false;
            dispScreen=10;
            clearScreen();
            WaveMakerScreen();
            WaveMakerStatusScreen();
           } else
         if ((x>=110) && (x<=210))                       //press TEST
           {
            waitForIt(110, 200, 210, 220);
            myGLCD.setColor(0, 255, 0);
            myGLCD.fillRoundRect(110, 200, 210, 220);    //TEST Button in Green
            setFont(SMALL, 0, 0, 0, 0, 255, 0);
            myGLCD.print("TESTING", 132, 204);
            myGLCD.setColor(255, 255, 255);            
            myGLCD.drawRoundRect(110, 200, 210, 220);
            waveMakerOff=false;
            waveMakerTest=true;
            } else
         if ((x>=210) && (x<=315))                       //press SAVE
           {
            waitForIt(215, 200, 315, 220);
            if (WAVE==1) { Pump1m=Min1; Pump1s=Sec1; Pump2m=Min2; Pump2s=Sec2;}
            if (WAVE==2) { OnForTm=Min1; OnForTs=Sec1; OffForTm=Min2; OffForTs=Sec2;}
            SaveWaveToEEPROM();
            waveMakerTest=false;
            waveMakerOff=false;
            dispScreen=10;
            clearScreen();
            WaveMakerScreen();
            WaveMakerStatusScreen();
           }
        }
      else 
        {
         if (WAVE==1)                                    //Alternating Mode Settings
           { 
            waveModePlusMinus();
           }
        
         if (WAVE==2)                                    //Synchronous Mode Settings
           {
            if ((x>=5) && (x<=155) && (y>=20) && (y<=40))   //Constantly ON
              {
               waitForIt(5, 20, 155, 40);
               Synch=1;
               synchronousSynch();
              } else 
            if ((x>=165) && (x<=315) && (y>=20) && (y<=40)) //Pulsating Mode
              {
               waitForIt(165, 20, 315, 40);
               Synch=2;
               synchronousSynch();
              }
            if (Synch==2)                                //Synchronous-Pulsating Settings
              {
               waveModePlusMinus();
              }
           }
        }
    break;

    case 12:     //--------------- AUTOMATIC FISH FEEDER PAGE --------------
      if ((x>=5) && (x<=155) && (y>=20) && (y<=40))      //press Feeding Time 1
        {
         waitForIt(5, 20, 155, 40);
         feedTime=1;
         dispScreen=13;
         clearScreen();
         setFeederTimesScreen();
        } else
      if ((x>=165) && (x<=315) && (y>=20) && (y<=40))    //press Feeding Time 2
        {
         waitForIt(165, 20, 315, 40);
         feedTime=2;
         dispScreen=13;
         clearScreen();
         setFeederTimesScreen();
        } else
      if ((x>=5) && (x<=155) && (y>=168) && (y<=188))    //press Feeding Time 3
        {
         waitForIt(5, 168, 155, 188);
         feedTime=3;
         dispScreen=13;
         clearScreen();
         setFeederTimesScreen();
        } else
      if ((x>=165) && (x<=315) && (y>=168) && (y<=188))  //press Feeding Time 4
        {
         waitForIt(165, 168, 315, 188);
         feedTime=4;
         dispScreen=13;
         clearScreen();
         setFeederTimesScreen();
        } else
      if ((x>=85) && (x<=235) && (y>=94) && (y<=114))    //press Feeding Fish Now!
        {
         waitForIt(85, 94, 235, 114);
         myGLCD.setColor(0, 255, 0);
         myGLCD.fillRoundRect(85, 94, 235, 114);
         myGLCD.setColor(255, 255, 255);
         myGLCD.drawRoundRect(85, 94, 235, 114); 
         setFont(SMALL, 0, 0, 0, 0, 255, 0);
         myGLCD.print("Now Feeding", 118, 98); 
         digitalWrite(autoFeeder, HIGH);
         delay(5000);
         myGLCD.setColor(153, 0, 102);
         myGLCD.fillRoundRect(85, 94, 235, 114);
         myGLCD.setColor(255, 255, 255);
         myGLCD.drawRoundRect(85, 94, 235, 114); 
         setFont(SMALL, 255, 255, 255, 153, 0, 102);
         myGLCD.print("Feed Fish Now!", 106, 98);  
         digitalWrite(autoFeeder, LOW);
        } 
    break;

    case 13:     //------------ SET AUTOMATIC FISH FEEDER TIMES ------------
      if ((x>=back[0]) && (x<=back[2]) && (y>back[1]) && (y<=back[3]))          //press back    
        {
         waitForIt(back[0], back[1], back[2], back[3]);
         ReadFromEEPROM();
         if ((timeDispH>=0) && (timeDispH<=11)) { AM_PM=1;}
         else { AM_PM=2;}
         dispScreen=12;
         clearScreen();
         autoFeederScreen();
        } else
      if ((x>=prSAVE[0]) && (x<=prSAVE[2]) && (y>=prSAVE[1]) && (y<=prSAVE[3])) //press SAVE
        {
         waitForIt(prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3]);
         if (setTimeFormat==1)
           {
            if ((rtcSetHr==0) && (AM_PM==2))
              { rtcSetHr+=12;}
            if (((rtcSetHr>=1) && (rtcSetHr<=11)) && (AM_PM==2))
              { rtcSetHr+=12;}  
            if (((rtcSetHr>=12) && (rtcSetHr<=23)) && (AM_PM==1))
              { rtcSetHr-=12;}           
           }                  
         if (feedTime==1)
           { feedFish1H=rtcSetHr; feedFish1M=rtcSetMin;}
         if (feedTime==2)
           { feedFish2H=rtcSetHr; feedFish2M=rtcSetMin;}
         if (feedTime==3)
           { feedFish3H=rtcSetHr; feedFish3M=rtcSetMin;}
         if (feedTime==4)
           { feedFish4H=rtcSetHr; feedFish4M=rtcSetMin;}        
         SaveFeedTimesToEEPROM();
         dispScreen=12;
         clearScreen();
         autoFeederScreen();
        } else
      if ((x>=70) && (x<=250) && (y>=150) && (y<=170))   //Feeding ON/OFF
        {
         waitForIt(70, 150, 250, 170);
         if (feedTime==1) 
           { if (FEEDTime1==1) { FEEDTime1=0;} 
             else { FEEDTime1=1;}}
         if (feedTime==2) 
           { if (FEEDTime2==1) { FEEDTime2=0;} 
             else { FEEDTime2=1;}}
         if (feedTime==3) 
           { if (FEEDTime3==1) { FEEDTime3=0;} 
             else { FEEDTime3=1;}}
         if (feedTime==4) 
           { if (FEEDTime4==1) { FEEDTime4=0;} 
             else { FEEDTime4=1;}}
         feedingTimeOnOff();
        } 
      else      
        {
         if ((y>=houP[1]) && (y<=houP[3]))               //FIRST ROW
           {
            if ((x>=houP[0]) && (x<=houP[2]))            //press hour up
              {
               waitForIt(houP[0], houP[1], houP[2], houP[3]);
               rtcSetHr++;
               if (rtcSetHr>=24) 
                 { rtcSetHr=0; }
              }
            if ((x>=minP[0]) && (x<=minP[2]))            //press min up
              {
               waitForIt(minP[0], minP[1], minP[2], minP[3]);
               rtcSetMin++;
               if (rtcSetMin>=60) {rtcSetMin=0; }
              }
            if ((x>=ampmP[0]) && (x<=ampmP[2])           //press AMPM up
               && (setTimeFormat==1))         
              {
               waitForIt(ampmP[0], ampmP[1], ampmP[2], ampmP[3]);
               if (AM_PM==1) {AM_PM=2;}
               else {AM_PM=1;}
              }
           }
         if ((y>=houM[1]) && (y<=houM[3]))               //SECOND ROW
           {
            if ((x>=houM[0]) && (x<=houM[2]))            //press hour down
              {
               waitForIt(houM[0], houM[1], houM[2], houM[3]);             
               rtcSetHr--;
               if (rtcSetHr<0) 
                 { rtcSetHr=23; }
              }
            if ((x>=minM[0]) && (x<=minM[2]))            //press min down
              {
               waitForIt(minM[0], minM[1], minM[2], minM[3]);
               rtcSetMin--;
               if (rtcSetMin<0) {rtcSetMin=59; } 
              }
            if ((x>=ampmM[0]) && (x<=ampmM[2])           //press AMPM down
                && (setTimeFormat==1))        
              {
               waitForIt(ampmM[0], ampmM[1], ampmM[2], ampmM[3]);
               if (AM_PM==1) {AM_PM=2;}
               else {AM_PM=1;}
              }  
           } 
         setFeederTimesScreen(false);
        }
    break;
    
    case 14:     //------------- GENERAL SETTINGS (PAGE 1) -----------------
      if ((x>=backGS[0]) && (x<=backGS[2]) && (y>=backGS[1]) && (y<=backGS[3]))  //press back    
        {
         waitForIt(backGS[0], backGS[1], backGS[2], backGS[3]);
         LEDtestTick = false;
         waveMakerOff = false;          
         ReadFromEEPROM();
         dispScreen=1;
         clearScreen();
         menuScreen();
        } else
      if ((x>=nextGS[0]) && (x<=nextGS[2]) && (y>=nextGS[1]) && (y<=nextGS[3]))  //press next
        {
         waitForIt(nextGS[0], nextGS[1], nextGS[2], nextGS[3]);
         dispScreen=15;
         clearScreen();
         generalSettingsScreen_2();
        } else        
      if ((x>=prSAVEgs[0]) && (x<=prSAVEgs[2]) && (y>=prSAVEgs[1]) && (y<=prSAVEgs[3])) //press SAVE
        {
         waitForIt(prSAVEgs[0], prSAVEgs[1], prSAVEgs[2], prSAVEgs[3]);
         SaveGenSetsToEEPROM();
         SaveLEDsFailsafeToEEPROM();         
         dispScreen=1;
         clearScreen();
         menuScreen();
        }
      if ((x>=canCgs[0]) && (x<=canCgs[2]) && (y>=canCgs[1]) && (y<=canCgs[3]))  //press cancel 
        {
         waitForIt(canCgs[0], canCgs[1], canCgs[2], canCgs[3]);
         LEDtestTick = false;
         waveMakerOff = false;     
         ReadFromEEPROM();     
         dispScreen=0;
         clearScreen();
         mainScreen(true);
        } else        

      if ((x>=185) && (x<=305) && (y>=19) && (y<=39))    //press DD/MM/YYYY Button
        {
         waitForIt(185, 19, 305, 39);
         setFont(SMALL, 255, 255, 0, 64, 64, 64);         
         myGLCD.print("                    ", 1, 227);            
         setCalendarFormat = 0;
         genSetSelect_1();
        }

      if ((x>=185) && (x<=305) && (y>=45) && (y<=65))    //press Month DD, YYYY Button
        {
         waitForIt(185, 45, 305, 65);
         setFont(SMALL, 255, 255, 0, 64, 64, 64);         
         myGLCD.print("                    ", 1, 227);            
         setCalendarFormat = 1;
         genSetSelect_1();
        }

      if ((x>=195) && (x<=235))                          //first column
        {
         if ((y>=76) && (y<=96))                         //press Show DOW "ON"
           { 
            waitForIt(195, 76, 235, 96);
            setFont(SMALL, 255, 255, 0, 64, 64, 64);         
            myGLCD.print("                    ", 16, 227);            
            displayDOW = 1;
            genSetSelect_1();
           }
         if ((y>=107) && (y<=127))                       //press 12HR Button
           { 
            waitForIt(195, 107, 235, 127);
            setFont(SMALL, 255, 255, 0, 64, 64, 64);         
            myGLCD.print("            ", 220, 227);            
            setTimeFormat = 1;
            genSetSelect_1();
           }
         if ((y>=138) && (y<=158))                       //press deg C
           { 
            waitForIt(195, 138, 235, 158);
            setTempScale = 0;
            genSetSelect_1();            
           }
        } 
      if ((x>=255) && (x<=295))                          //second column
        {
         if ((y>=76) && (y<=96))                         //press Show DOW "OFF"
           { 
            waitForIt(255, 76, 295, 96);
            setFont(SMALL, 255, 255, 0, 64, 64, 64);         
            myGLCD.print("                    ", 1, 227);            
            displayDOW = 0;
            genSetSelect_1();
           }        
         if ((y>=107) && (y<=127))                       //press 24HR Button
           { 
            waitForIt(255, 107, 295, 127);
            setFont(SMALL, 255, 255, 0, 64, 64, 64);         
            myGLCD.print("            ", 220, 227);             
            setTimeFormat = 0;
            genSetSelect_1();            
           }
         if ((y>=138) && (y<=158))                       //press deg F
           { 
            waitForIt(255, 138, 295, 158);
            setTempScale = 1;
            genSetSelect_1();               
           }
        }            

      if ((x>=185) && (x<=305) && (y>=169) && (y<=189))  //press CHANGE TEMPS
        {
         waitForIt(185, 169, 305, 189);
         ReadFromEEPROM();         
         dispScreen=16;
         clearScreen();
         ChangeFanTempsScreen(true);
        }
    break;
    case 15:     //------------- GENERAL SETTINGS (PAGE 2) -----------------
      if ((x>=back[0]) && (x<=back[2]) && (y>=back[1]) && (y<=back[3]))  //press back    
        {
         waitForIt(back[0], back[1], back[2], back[3]);
         dispScreen=14;
         clearScreen();
         generalSettingsScreen_1();
        } else
      if ((x>=prSAVE[0]) && (x<=prSAVE[2]) && (y>=prSAVE[1]) && (y<=prSAVE[3])) //press SAVE
        {
         waitForIt(prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3]);
         SaveGenSetsToEEPROM();
         SaveLEDsFailsafeToEEPROM();
         dispScreen=1;
         clearScreen();
         menuScreen();
        }
      if ((x>=canC[0]) && (x<=canC[2]) && (y>=canC[1]) && (y<=canC[3]))  //press cancel 
        {
         waitForIt(canC[0], canC[1], canC[2], canC[3]);
         LEDtestTick = false;
         waveMakerOff = false;     
         ReadFromEEPROM();     
         dispScreen=0;
         clearScreen();
         mainScreen(true);
        } else        

      if ((x>=185) && (x<=305) && (y>=55) && (y<=75))    //press CHANGE TEMP (Dim LEDs)
        {
         waitForIt(185, 55, 305, 75);
         dispScreen=17;
         clearScreen();
         DimLEDsAtTempScreen();
        }

      if ((x>=185) && (x<=305) && (y>=129) && (y<=149))  //press SETTINGS (Screensaver)
        {
         waitForIt(185, 129, 305, 149);
         dispScreen=18;
         clearScreen();
         ScreensaverSettingsScreen();
        }

      if ((x>=195) && (x<=235))                          //first column
        {
         if ((y>=27) && (y<=47))                         //press ON (Dim LEDs)
           { 
            waitForIt(195, 27, 235, 47);
            setDimLEDsOnOff = 1;
            genSetSelect_2();
           }
         if ((y>=101) && (y<=121))                       //press ON (Screensaver)
           { 
            waitForIt(195, 101, 235, 121);
            setScreensaverOnOff = 1;
            genSetSelect_2();                 
           }
         if ((y>=169) && (y<=189))                       //press ON (Auto-Stop on Feed)
           { 
            waitForIt(195, 169, 235, 189);
            setAutoStop = 1;
            genSetSelect_2();                       
           }           
        } 
      if ((x>=255) && (x<=295))                          //second column
        {
         if ((y>=27) && (y<=47))                         //press OFF (Dim LEDs)
           { 
            waitForIt(255, 27, 295, 47);
            setDimLEDsOnOff = 0;
            genSetSelect_2();            
           }
         if ((y>=101) && (y<=121))                       //press OFF (Screensaver)
           { 
            waitForIt(255, 101, 295, 121);
            setScreensaverOnOff = 0;
            genSetSelect_2();                      
           }
         if ((y>=169) && (y<=189))                       //press OFF (Auto-Stop on Feed) 
           { 
            waitForIt(255, 169, 295, 189);
            setAutoStop = 0;
            genSetSelect_2();                      
           }           
        } 
    break;
    case 16:     //------------------ CHANGE FAN TEMPS ---------------------
      if ((x>=back[0]) && (x<=back[2]) && (y>=back[1]) && (y<=back[3]))  //press back    
        {
         waitForIt(back[0], back[1], back[2], back[3]);
         dispScreen=14;
         clearScreen();
         generalSettingsScreen_1();
        } else
      if ((x>=prSAVE[0]) && (x<=prSAVE[2]) && (y>=prSAVE[1]) && (y<=prSAVE[3])) //press SAVE
        {
         waitForIt(prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3]);
         setTempToBeginHoodFanC=temp2beHFan;
         setTempToBeginHoodFanF=temp2beHFan;
         setTempToBeginSumpFanC=temp2beSFan;
         setTempToBeginSumpFanF=temp2beSFan;
         if (setTempScale==0)
           { setTempToBeginHoodFanF=((1.8*setTempToBeginHoodFanC)+32.05);
             setTempToBeginSumpFanF=((1.8*setTempToBeginSumpFanC)+32.05);}
         if (setTempScale==1)
           { setTempToBeginHoodFanC=((.55556*(setTempToBeginHoodFanF-32))+.05);
             setTempToBeginSumpFanC=((.55556*(setTempToBeginSumpFanF-32))+.05);}
         dispScreen=14;       
         SaveGenSetsToEEPROM();
         clearScreen();
         generalSettingsScreen_1();
        } else
      if ((x>=canC[0]) && (x<=canC[2]) && (y>=canC[1]) && (y<=canC[3]))  //press cancel 
        {
         waitForIt(canC[0], canC[1], canC[2], canC[3]);
         LEDtestTick = false;
         waveMakerOff = false;     
         ReadFromEEPROM();     
         dispScreen=0;
         clearScreen();
         mainScreen(true);
        } else      
      setFont(LARGE, 255, 255, 255, 0, 0, 0);
      if ((x>=HoodFanTm[0]) && (x<=HoodFanTm[2]))              //first column
        {
         if ((y>=HoodFanTm[1]) && (y<=HoodFanTm[3]))           //press Hood Fan Temp -0.1
           {
            temp2beHFan -= 0.1;
            if ((setTempScale==1) && (temp2beHFan <= 68.0)){
              temp2beHFan = 68.0; }
            if ((setTempScale==0) && (temp2beHFan <= 20.0)) {
              temp2beHFan = 20.0; }
            ChangeFanTempsScreen();
           }
         if ((y>=SumpFanTm[1]) && (y<=SumpFanTm[3]))           //press Sump Fan Temp -0.1
           {
            temp2beSFan -= 0.1;             
            if ((setTempScale==1) && (temp2beSFan <= 68.0)){
              temp2beSFan = 68.0; }
            if ((setTempScale==0) && (temp2beSFan <= 20.0)) {
              temp2beSFan = 20.0; }
            ChangeFanTempsScreen();
           }          
        }
      if ((x>=HoodFanTp[0]) && (x<=HoodFanTp[2]))              //second column
        {
         if ((y>=HoodFanTp[1]) && (y<=HoodFanTp[3]))           //press Hood Fan Temp +0.1
           {
            temp2beHFan += 0.1;
            if ((setTempScale==1) && (temp2beHFan >= 99.9)){
              temp2beHFan = 99.9; }
            if ((setTempScale==0) && (temp2beHFan >= 37.7)) {
              temp2beHFan = 37.7; }
            ChangeFanTempsScreen();
           }
         if ((y>=SumpFanTp[1]) && (y<=SumpFanTp[3]))           //press Sump Fan Temp +0.1
           {
            temp2beSFan += 0.1;             
            if ((setTempScale==1) && (temp2beSFan >= 99.9)){
              temp2beSFan = 99.9; }
            if ((setTempScale==0) && (temp2beSFan >= 37.7)) {
              temp2beSFan = 37.7; }
            ChangeFanTempsScreen();
           }
        }
    break;
    case 17:     //------------------ DIM LEDs AT TEMP ---------------------
      if ((x>=back[0]) && (x<=back[2]) && (y>=back[1]) && (y<=back[3]))  //press back    
        {
         waitForIt(back[0], back[1], back[2], back[3]);
         dispScreen=15;
         clearScreen();
         generalSettingsScreen_2();
        } else
      if ((x>=prSAVE[0]) && (x<=prSAVE[2]) && (y>=prSAVE[1]) && (y<=prSAVE[3])) //press SAVE
        {
         waitForIt(prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3]);
         setLEDsDimTempC=TempLEDsDimTemp;
         setLEDsDimTempF=TempLEDsDimTemp;       
         if (setTempScale==0)
           { setLEDsDimTempF=((1.8*setLEDsDimTempC)+32.5);}
         if (setTempScale==1)
           { setLEDsDimTempC=((.55556*(setLEDsDimTempF-32))+.5);}         
         setLEDsDimPercent=TempLEDsDimPercent;   
         SaveLEDsFailsafeToEEPROM();
         dispScreen=15;
         clearScreen();
         generalSettingsScreen_2();
        } else
      if ((x>=canC[0]) && (x<=canC[2]) && (y>=canC[1]) && (y<=canC[3]))  //press cancel 
        {
         waitForIt(canC[0], canC[1], canC[2], canC[3]);
         LEDtestTick = false;
         waveMakerOff = false;     
         ReadFromEEPROM();     
         dispScreen=0;
         clearScreen();
         mainScreen(true);
        } else

      if ((x>=235) && (x<=260) && (y>=36) && (y<=61))    //press TEMP UP button
        {
         TempLEDsDimTemp++;
         setFont(LARGE, 0, 255, 0, 0, 0, 0);
         if (TempLEDsDimTemp>=255)
           { TempLEDsDimTemp=255;}
         if (TempLEDsDimTemp>=100)
           { myGLCD.printNumI(TempLEDsDimTemp, 181, 55);}
         if ((TempLEDsDimTemp<=99)&&(TempLEDsDimTemp>=10))
           { myGLCD.printNumI(TempLEDsDimTemp, 189, 55);}
         if (TempLEDsDimTemp<=9)         
           { myGLCD.printNumI(TempLEDsDimTemp, 198, 55);}         
        } else  
      if ((x>=235) && (x<=260) && (y>=66) && (y<=91))    //press TEMP DOWN button
        {
         TempLEDsDimTemp--;
         setFont(LARGE, 0, 255, 0, 0, 0, 0);
         if (TempLEDsDimTemp<=0)
           { TempLEDsDimTemp=0;}
         if (TempLEDsDimTemp>=100)
           { myGLCD.printNumI(TempLEDsDimTemp, 181, 55);}
         if ((TempLEDsDimTemp<=99)&&(TempLEDsDimTemp>=10))
           { myGLCD.setColor(0, 0, 0);
             myGLCD.fillRect(181, 55, 188, 71);
             myGLCD.fillRect(221, 55, 229, 71);             
             setFont(LARGE, 0, 255, 0, 0, 0, 0);
             myGLCD.printNumI(TempLEDsDimTemp, 189, 55);}
         if (TempLEDsDimTemp<=9)
           { myGLCD.setColor(0, 0, 0);
             myGLCD.fillRect(181, 55, 197, 71);
             myGLCD.fillRect(214, 55, 229, 71);             
             setFont(LARGE, 0, 255, 0, 0, 0, 0);
             myGLCD.printNumI(TempLEDsDimTemp, 198, 55);
           }  
        } else        

      if ((x>=235) && (x<=260) && (y>=117) && (y<=142))  //press % UP button
        {
         TempLEDsDimPercent++;
         setFont(LARGE, 0, 255, 0, 0, 0, 0);
         if (TempLEDsDimPercent>=100)
           { TempLEDsDimPercent=100;}
         if (TempLEDsDimPercent>=100)
           { myGLCD.printNumI(TempLEDsDimPercent, 181, 136);}
         if ((TempLEDsDimPercent<=99)&&(TempLEDsDimPercent>=10))
           { myGLCD.printNumI(TempLEDsDimPercent, 189, 136);}
         if (TempLEDsDimPercent<=9)         
           { myGLCD.printNumI(TempLEDsDimPercent, 198, 136);}         
        } else  
      if ((x>=235) && (x<=260) && (y>=147) && (y<=172))  //press % DOWN button
        {
         TempLEDsDimPercent--;
         setFont(LARGE, 0, 255, 0, 0, 0, 0);
         if (TempLEDsDimPercent<=0)
           { TempLEDsDimPercent=0;}
         if ((TempLEDsDimPercent<=99)&&(TempLEDsDimPercent>=10))
           { myGLCD.setColor(0, 0, 0);
             myGLCD.fillRect(181, 136, 188, 152);
             myGLCD.fillRect(221, 136, 229, 152);             
             setFont(LARGE, 0, 255, 0, 0, 0, 0);
             myGLCD.printNumI(TempLEDsDimPercent, 189, 136);}
         if (TempLEDsDimPercent<=9)
           { myGLCD.setColor(0, 0, 0);
             myGLCD.fillRect(181, 136, 197, 152);
             myGLCD.fillRect(214, 136, 229, 152);             
             setFont(LARGE, 0, 255, 0, 0, 0, 0);
             myGLCD.printNumI(TempLEDsDimPercent, 198, 136);
           } 
        }        
    break;    
    case 18:     //-------------- SET SCREENSAVER WAIT TIME ----------------
      if ((x>=back[0]) && (x<=back[2]) && (y>=back[1]) && (y<=back[3]))  //press back    
        {
         waitForIt(back[0], back[1], back[2], back[3]);
         dispScreen=15;
         clearScreen();
         generalSettingsScreen_2();
        } else
      if ((x>=prSAVE[0]) && (x<=prSAVE[2]) && (y>=prSAVE[1]) && (y<=prSAVE[3])) //press SAVE
        {
         waitForIt(prSAVE[0], prSAVE[1], prSAVE[2], prSAVE[3]);
         setSSmintues=TempSSminutes;
         SaveGenSetsToEEPROM();
         dispScreen=15;
         clearScreen();
         generalSettingsScreen_2();
        } else
      if ((x>=canC[0]) && (x<=canC[2]) && (y>=canC[1]) && (y<=canC[3]))  //press cancel 
        {
         waitForIt(canC[0], canC[1], canC[2], canC[3]);
         LEDtestTick = false;
         waveMakerOff = false;     
         ReadFromEEPROM();     
         dispScreen=0;
         clearScreen();
         mainScreen(true);
        } else  

      if ((x>=185) && (x<=235))                          //first column
        {
         if ((y>=20) && (y<=40))                         //press BLANK screensaver
           { 
            waitForIt(185, 20, 235, 40);
            setClockOrBlank = 1;
            ScreensaverSelect();
           }
         if ((y>=51) && (y<=71))                         //press YES, show Date on Screensaver
           { 
            waitForIt(185, 51, 235, 71);
            setScreensaverDOWonOff = 1;
            ScreensaverSelect();                 
           }
        } 
      if ((x>=255) && (x<=305))                          //second column
        {
         if ((y>=20) && (y<=40))                         //press CLOCK screensaver
           { 
            waitForIt(255, 20, 305, 40);
            setClockOrBlank = 0;
            ScreensaverSelect();            
           }
         if ((y>=51) && (y<=71))                         //press NO, show Date on Screensaver
           { 
            waitForIt(255, 51, 305, 71);
            setScreensaverDOWonOff = 0;
            ScreensaverSelect();                      
           }
        }         

      if ((x>=175) && (x<=200) && (y>=107) && (y<=132))  //press Minute UP button
        {
         TempSSminutes++;
         setFont(LARGE, 0, 255, 0, 0, 0, 0);
         if (TempSSminutes>=99)
           { TempSSminutes=99;}
         if (TempSSminutes>=10)
           { myGLCD.printNumI(TempSSminutes, 129, 126);}
         else { myGLCD.printNumI(TempSSminutes, 137, 126);}         
        } else  
      if ((x>=175) && (x<=200) && (y>=137) && (y<=162))  //press Minute DOWN button
        {
         TempSSminutes--;
         setFont(LARGE, 0, 255, 0, 0, 0, 0);
         if (TempSSminutes<=1)
           { TempSSminutes=1;}
         if (TempSSminutes>=10)
           { myGLCD.printNumI(TempSSminutes, 129, 126);}
         else { myGLCD.printNumI(TempSSminutes, 137, 126);
                myGLCD.setColor(0, 0, 0);
                myGLCD.fillRect(129, 126, 136, 142);
                myGLCD.fillRect(153, 126, 161, 142);}         
        }        
    break;    
    case 19:     //------------------------- ABOUT -------------------------
    if ((x>=back[0]) && (x<=back[2]) && (y>=back[1]) && (y<=back[3])) //press back    
      {
       waitForIt(back[0], back[1], back[2], back[3]);
       dispScreen=1;
       clearScreen();
       menuScreen();
      }          
    break;
    } 
  } 
  delay(100);
}
/********************************* END of TOUCH SCREEN ********************************/


/**************************************** SETUP **************************************/
void setup()
{
  Serial.begin(9600);
  
  TCCR5A = B00101011; // Fast PWM change at OCR5A (Timer 5 - pins 44 & 45)
  TCCR5B = B10001;    // No Prescalering (Timer 5 - pins 44 & 45)
  
  pinMode(ledPinSump, OUTPUT);  
  pinMode(ledPinBlue, OUTPUT);
  pinMode(ledPinWhite, OUTPUT);
  pinMode(ledPinRoyBlue, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinUV, OUTPUT);
  pinMode(ledPinMoon, OUTPUT);

  pinMode(HoodFansPWM, OUTPUT);
  pinMode(SumpFanPWM, OUTPUT);
  pinMode(HoodFansTranzPin, OUTPUT);
  pinMode(SumpFanTranzPin, OUTPUT);
  
  pinMode(tempHeatPin, OUTPUT);
  pinMode(tempChillPin, OUTPUT);
  pinMode(tempAlarmPin, OUTPUT);
  
  pinMode(WaveMakerTop, OUTPUT);
  pinMode(WaveMakerBottom, OUTPUT);
  
  pinMode(autoFeeder, OUTPUT);  

   
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_MEDIUM);
  
  sensors.begin();     //start up temperature library
  // set the resolution to 9 bit
  sensors.setResolution(waterThermometer, 9);
  sensors.setResolution(hoodThermometer, 9);
  sensors.setResolution(sumpThermometer, 9);

  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 226, 319, 239);                     //Bottom Bar

  rtc.halt(false);                                       //Set the clock to run-mode
 
  min_cnt= (t.hour*60)+t.min;
  ReadFromEEPROM();
  LED_levels_output();
  wave_output();
  screenReturn();
  screenSaver();  
  mainScreen(true);
}
/*********************************** END of SETUP ************************************/


/********************************** BEGIN MAIN LOOP **********************************/
void loop()
{
  t = rtc.getTime();
  if ((myTouch.dataAvailable()) && (screenSaverCounter>=setScreenSaverTimer))  
    { LEDtestTick = false;
      myGLCD.setColor(64, 64, 64);
      myGLCD.fillRect(0, 226, 319, 239);                     //Bottom Bar    
      screenSaverCounter=0;
      clearScreen();     
      mainScreen(true);
      dispScreen=0;} 
  else 
    { if (myTouch.dataAvailable())  
        { processMyTouch();}}

  if (waveMakerOff==false) 
    { wave_output();}
  else 
    { PumpTstate=LOW; 
      PumpBstate=LOW;}

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisFive > 5000)   //check time, temp and LED levels every 5s
    { previousMillisFive = currentMillis;  
      t = rtc.getTime(); 
      feedingTimeOutput();
      
      if (screenSaverCounter<setScreenSaverTimer)
        { TimeDateBar(); }
      
      checkTempC();
      OCR5A = 16000000.0 / (2*25000.0);            //25kHz PWM - above our audible range so fans are quiet
      OCR5B = 16000000.0 / (2*25000.0) * SumpPWM;  //"SumpPWM" is the % duty cycle for pin 45
      OCR5C = 16000000.0 / (2*25000.0) * HoodPWM;  //"HoodPWM" is the % duty cycle for pin 44

      min_cnt= (t.hour*60)+t.min; 
      LED_levels_output();

      screenReturn();
      screenSaver();

      if ((dispScreen == 0) && (screenSaverCounter<setScreenSaverTimer)) 
        { mainScreen();}
    } 
}
/********************************** END OF MAIN LOOP *********************************/
