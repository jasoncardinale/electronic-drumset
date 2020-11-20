int pinRead;
char pinAssignments[5] ={'A0','A1','A2','A3','A4'};
byte PadNote[5] = {53,69,11,57,36}; // MIDI notes from 0 to 127 (Mid C = 60)
//53 Campana
//69 Tarola
//11 Hithat cerrado
//57 Crash
//36 Bombo
int PadCutOff[5] = {500,200,350,300,400}; // Minimum Analog value to cause a drum hit
int MaxPlayTime[5] = {90,90,90,90,90}; // Cycles before a 2nd hit is allowed
#define midichannel 1; // MIDI channel from 0 to 15 (+1 in "real world")
boolean VelocityFlag = true; // Velocity ON (true) or OFF (false)

boolean activePad[5] = {0,0,0,0,0}; // Array of flags of pad currently playing
int PinPlayTime[5] = {0,0,0,0,0}; // Counter since pad started to play
byte status1;
int pin = 0;
int hitavg = 0;

void setup()
{
Serial.begin(57600); // SET HAIRLESS TO THE SAME BAUD RATE IN THE SETTING
}

void loop()
{
for(int pin=0; pin < 5; pin++) //
{
//int pin = 3;
// for (pinRead=0; pinRead < 16, pin++){
hitavg = analogRead(pinAssignments[pin]);
//Serial.println(hitavg);
// read the input pin
if((hitavg > PadCutOff[pin]))
{
if((activePad[pin] == false))
{
if(VelocityFlag == true)
{
hitavg=127;
//hitavg = 127 / ((1023 - PadCutOff[pin]) / (hitavg - PadCutOff[pin])); // With full range (Too sensitive ?)
//hitavg = (hitavg / 8) -1 ; //8 // Upper range
}
else
{
hitavg = 127;
}
MIDI_TX(144,PadNote[pin],hitavg); //note on
PinPlayTime[pin] = 0;
activePad[pin] = true;
}
else
{
PinPlayTime[pin] = PinPlayTime[pin] + 1;
}
}
else if((activePad[pin] == true))
{
PinPlayTime[pin] = PinPlayTime[pin] + 1;
if(PinPlayTime[pin] > MaxPlayTime[pin])
{
activePad[pin] = false;
MIDI_TX(144,PadNote[pin],0);
}
}
}
}

void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY)
{
status1 = MESSAGE + midichannel;
Serial.write(status1);
Serial.write(PITCH);
Serial.write(VELOCITY);
}
