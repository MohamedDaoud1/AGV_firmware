#include <Wire.h>
#include <NewPing.h>

#define SLAVE_ADDRESS 0x08
#define SONAR_NUM     4 // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

/*
 Arduino pin 10 -> HX711 CLK
 9 -> DOUT
 5V -> VCC
 GND -> GND
 
 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 
*/

#define DOUT4 9
#define CLK4  8
#define DOUT3 7
#define CLK3  6
#define DOUT2 5
#define CLK2  4
#define DOUT1 3
#define CLK1  2

#define gain 128
#define times 10
#define US_Tr1 17
#define US_Ec1 16
#define US_Tr2 15
#define US_Ec2 14
#define US_Tr3 13
#define US_Ec3 12
#define US_Tr4 11
#define US_Ec4 10
unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.
char count;
char Readings[8];
NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(US_Tr1, US_Ec1, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(US_Tr2, US_Ec2, MAX_DISTANCE),
  NewPing(US_Tr3, US_Ec3, MAX_DISTANCE),
  NewPing(US_Tr4, US_Ec4, MAX_DISTANCE)
};
byte GAIN;    // amplification factor

long OFFSET1 = 0;  // used for tare weight
long OFFSET2 = 0;  // used for tare weight
long OFFSET3 = 0;  // used for tare weight
long OFFSET4 = 0;  // used for tare weight
float SCALE1 = 1;  // used to return weight in grams, kg, ounces, whatever
float SCALE2 = 1;  // used to return weight in grams, kg, ounces, whatever
float SCALE3 = 1;  // used to return weight in grams, kg, ounces, whatever
float SCALE4 = 1;  // used to return weight in grams, kg, ounces, whatever

void Begin1() 
{
  pinMode(CLK1, OUTPUT);
  pinMode(DOUT1, INPUT);
  set_gain1();
}

void Begin2() 
{
  pinMode(CLK2, OUTPUT);
  pinMode(DOUT2, INPUT);
  set_gain2();
}
void Begin3() 
{
  pinMode(CLK3, OUTPUT);
  pinMode(DOUT3, INPUT);
  set_gain3();
}
void Begin4() 
{
  pinMode(CLK4, OUTPUT);
  pinMode(DOUT4, INPUT);
  set_gain4();
}
void set_gain1() 
{
  switch (gain) 
  {
    case 128:   // channel A, gain factor 128
    GAIN = 1;
    break;
    case 64:    // channel A, gain factor 64
    GAIN = 3;
    break;
    case 32:    // channel B, gain factor 32
    GAIN = 2;
    break;
  }
  digitalWrite(CLK1, LOW);
  Read1();
}

void set_gain2() 
{
  switch (gain) 
  {
    case 128:   // channel A, gain factor 128
    GAIN = 1;
    break;
    case 64:    // channel A, gain factor 64
    GAIN = 3;
    break;
    case 32:    // channel B, gain factor 32
    GAIN = 2;
    break;
  }
  digitalWrite(CLK2, LOW);
  Read2();
}

void set_gain3() 
{
  switch (gain) 
  {
    case 128:   // channel A, gain factor 128
    GAIN = 1;
    break;
    case 64:    // channel A, gain factor 64
    GAIN = 3;
    break;
    case 32:    // channel B, gain factor 32
    GAIN = 2;
    break;
  }
  digitalWrite(CLK3, LOW);
  Read3();
}

void set_gain4() 
{
  switch (gain) 
  {
    case 128:   // channel A, gain factor 128
    GAIN = 1;
    break;
    case 64:    // channel A, gain factor 64
    GAIN = 3;
    break;
    case 32:    // channel B, gain factor 32
    GAIN = 2;
    break;
  }
  digitalWrite(CLK4, LOW);
  Read4();
}



bool is_ready1() 
{
  return (digitalRead(DOUT1) == LOW);
}
bool is_ready2() 
{
  return (digitalRead(DOUT2) == LOW);
}
bool is_ready3() 
{
  return (digitalRead(DOUT3) == LOW);
}
bool is_ready4() 
{
  return (digitalRead(DOUT4) == LOW);
}
long Read1() 
{
  // wait for the chip to become ready
  while (!is_ready1()) {}
  unsigned long value = 0;
  byte data[3] = { 0 };
  byte filler = 0x00;

  // pulse the clock pin 24 times to read the data
  // Shift the data and store 24 bit in one array
  for(byte i = 0; i < 3; i++)
  {
    for(byte n = 0; n < 8; ++n)
    {
      digitalWrite(CLK1,HIGH);
      data[2-i]  |= digitalRead(DOUT1) << (7 - n);
      digitalWrite(CLK1,LOW);  
    }
  } 
  // set the channel and the gain factor for the next reading using the clock pin
  for (unsigned int i = 0; i < GAIN; i++) 
  {
    digitalWrite(CLK1, HIGH);
    digitalWrite(CLK1, LOW);
  }

  // Replicate the most significant bit to pad out a 32-bit signed integer
  if (data[2] & 0x80) 
  {
    filler = 0xFF;
  } 
  else 
  {
    filler = 0x00;
  }
  // Construct a 32-bit signed integer
  value = ( static_cast<unsigned long>(filler) << 24
  | static_cast<unsigned long>(data[2]) << 16
  | static_cast<unsigned long>(data[1]) << 8
  | static_cast<unsigned long>(data[0]) );
  return static_cast<long>(value);
}

long Read2() 
{
  // wait for the chip to become ready
  while (!is_ready2()) {}
  unsigned long value = 0;
  byte data[3] = { 0 };
  byte filler = 0x00;

  // pulse the clock pin 24 times to read the data
  // Shift the data and store 24 bit in one array
  for(byte i = 0; i < 3; i++)
  {
    for(byte n = 0; n < 8; ++n)
    {
      digitalWrite(CLK2,HIGH);
      data[2-i]  |= digitalRead(DOUT2) << (7 - n);
      digitalWrite(CLK2,LOW);  
    }
  } 
  // set the channel and the gain factor for the next reading using the clock pin
  for (unsigned int i = 0; i < GAIN; i++) 
  {
    digitalWrite(CLK2, HIGH);
    digitalWrite(CLK2, LOW);
  }

  // Replicate the most significant bit to pad out a 32-bit signed integer
  if (data[2] & 0x80) 
  {
    filler = 0xFF;
  } 
  else 
  {
    filler = 0x00;
  }
  // Construct a 32-bit signed integer
  value = ( static_cast<unsigned long>(filler) << 24
  | static_cast<unsigned long>(data[2]) << 16
  | static_cast<unsigned long>(data[1]) << 8
  | static_cast<unsigned long>(data[0]) );
  return static_cast<long>(value);
}

long Read3() 
{
  // wait for the chip to become ready
  while (!is_ready3()) {}
  unsigned long value = 0;
  byte data[3] = { 0 };
  byte filler = 0x00;

  // pulse the clock pin 24 times to read the data
  // Shift the data and store 24 bit in one array
  for(byte i = 0; i < 3; i++)
  {
    for(byte n = 0; n < 8; ++n)
    {
      digitalWrite(CLK3,HIGH);
      data[2-i]  |= digitalRead(DOUT3) << (7 - n);
      digitalWrite(CLK3,LOW);  
    }
  } 
  // set the channel and the gain factor for the next reading using the clock pin
  for (unsigned int i = 0; i < GAIN; i++) 
  {
    digitalWrite(CLK3, HIGH);
    digitalWrite(CLK3, LOW);
  }

  // Replicate the most significant bit to pad out a 32-bit signed integer
  if (data[2] & 0x80) 
  {
    filler = 0xFF;
  } 
  else 
  {
    filler = 0x00;
  }
  // Construct a 32-bit signed integer
  value = ( static_cast<unsigned long>(filler) << 24
  | static_cast<unsigned long>(data[2]) << 16
  | static_cast<unsigned long>(data[1]) << 8
  | static_cast<unsigned long>(data[0]) );
  return static_cast<long>(value);
}

long Read4() 
{
  // wait for the chip to become ready
  while (!is_ready4()) {}
  unsigned long value = 0;
  byte data[3] = { 0 };
  byte filler = 0x00;

  // pulse the clock pin 24 times to read the data
  // Shift the data and store 24 bit in one array
  for(byte i = 0; i < 3; i++)
  {
    for(byte n = 0; n < 8; ++n)
    {
      digitalWrite(CLK4,HIGH);
      data[2-i]  |= digitalRead(DOUT4) << (7 - n);
      digitalWrite(CLK4,LOW);  
    }
  } 
  // set the channel and the gain factor for the next reading using the clock pin
  for (unsigned int i = 0; i < GAIN; i++) 
  {
    digitalWrite(CLK4, HIGH);
    digitalWrite(CLK4, LOW);
  }

  // Replicate the most significant bit to pad out a 32-bit signed integer
  if (data[2] & 0x80) 
  {
    filler = 0xFF;
  } 
  else 
  {
    filler = 0x00;
  }
  // Construct a 32-bit signed integer
  value = ( static_cast<unsigned long>(filler) << 24
  | static_cast<unsigned long>(data[2]) << 16
  | static_cast<unsigned long>(data[1]) << 8
  | static_cast<unsigned long>(data[0]) );
  return static_cast<long>(value);
}

void tare1() 
{
  double sum = read_average1();
  OFFSET1=sum;
}

void tare2() 
{
  double sum = read_average2();
  OFFSET2=sum;
}

void tare3() 
{
  double sum = read_average3();
  OFFSET3=sum;
}

void tare4() 
{
  double sum = read_average4();
  OFFSET4=sum;
}

long read_average1() 
{
  long sum = 0;
  for (byte i = 0; i < times; i++) 
  {
    sum += Read1();
    //yield();
  }
  return sum / times;
}

long read_average2() 
{
  long sum = 0;
  for (byte i = 0; i < times; i++) 
  {
    sum += Read2();
    //yield();
  }
  return sum / times;
}

long read_average3() 
{
  long sum = 0;
  for (byte i = 0; i < times; i++) 
  {
    sum += Read3();
    //yield();
  }
  return sum / times;
}

long read_average4() 
{
  long sum = 0;
  for (byte i = 0; i < times; i++) 
  {
    sum += Read4();
    //yield();
  }
  return sum / times;
}

float calibration_factor1 = 110000; //-7050 worked for my 440lb max scale setup
float calibration_factor2 = 110000; //-7050 worked for my 440lb max scale setup
float calibration_factor3 = 110000; //-7050 worked for my 440lb max scale setup
float calibration_factor4 = 110000; //-7050 worked for my 440lb max scale setup


double Weight1()
{
  double Val;
  SCALE1=calibration_factor1; //Adjust to this calibration factor
  Serial.print("Reading1: ");
  Val = (read_average1() - OFFSET1)/ SCALE1;
  Serial.print(Val);
  Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor1: ");
  Serial.print(calibration_factor1);
  Serial.println();
  return Val;
}


double Weight2()
{
   double Val;
  SCALE2=calibration_factor2; //Adjust to this calibration factor
  Serial.print("Reading2: ");
  Val = (read_average2() - OFFSET2)/ SCALE2;
  Serial.print(Val);
  Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor2: ");
  Serial.print(calibration_factor2);
  Serial.println();
  return Val;
}

double Weight3()
{
  double Val;
  SCALE3=calibration_factor3; //Adjust to this calibration factor
  Serial.print("Reading3: ");
  Val = (read_average3() - OFFSET3)/ SCALE3;
  Serial.print(Val);
  Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor3: ");
  Serial.print(calibration_factor3);
  Serial.println();
  return Val;
}

double Weight4()
{
  double Val;
  SCALE4=calibration_factor4; //Adjust to this calibration factor
  Serial.print("Reading4: ");
  Val = (read_average4() - OFFSET4)/ SCALE4;
  Serial.print(Val);
  Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor4: ");
  Serial.print(calibration_factor4);
  Serial.println();
  return Val;
}

double W1,W2,W3,W4;

void setup() 
{
  Serial.begin(115200);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  Wire.begin(SLAVE_ADDRESS); // this will begin I2C Connection with 0x40 address
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  
  Begin1();
  Begin2();
  Begin3();
  Begin4();
  //scale.set_scale();
  tare1(); //Reset the scale to 0
  tare2(); //Reset the scale to 0
  tare3(); //Reset the scale to 0
  tare4(); //Reset the scale to 0
}
void loop() 
{
  delay(200);
  Readings[0] = Weight1();
  Readings[1] = Weight2();
  Readings[2] = Weight3();
  Readings[3] = Weight4();
  for (uint8_t i = 0; i < SONAR_NUM; i++) 
  { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) 
    {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      Readings[4+currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    Readings[4+currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  // The following code would be replaced with your code that does something with the ping results.
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(Readings[4+i]);
    Serial.print("cm ");
  }
  Serial.println();
}

void receiveEvent(int howMany)
{
  count= Wire.read(); // receive byte as a character
}

void requestEvent()
{
  Wire.write(Readings[count]); // respond with message of 6 bytes as expected by master
}
