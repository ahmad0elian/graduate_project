#include<Wire.h>

// ESP
int espLine = 10;

// gyroscope 
const int MPU = 0x68; 
int16_t GyX, GyY, GyZ;
char tmp_str[7];

int isFlipped = 0;

// distance sensor
int distance;

int trig = 3;
int echo = 2;

// buzzer 
int buzzer = 8;

// LEDs 
#define red A2
#define green A1
#define blue A0

// car controllers
int LF = 4;
int LB = 5;
int RF = 6;
int RB = 7;

// received value from Bluetooth
char val;
 
void setup() {
  // bluetooth 
  Serial.begin(9600);

  // esp
  pinMode(espLine, OUTPUT);
  
  // gyroscope 
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  
  Wire.write(0);    
  Wire.endTransmission(true);

  // distance sensor
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // buzzer 
  pinMode(buzzer, OUTPUT);

  // LEDs 
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  // car controllers
  pinMode(LF,OUTPUT);  
  pinMode(LB,OUTPUT); 
  pinMode(RF,OUTPUT);  
  pinMode(RB,OUTPUT); 
}

// update the flipped state 
bool updateIsFlipped(int x, int y) {
  if (x < -8000 | x > 8000 | y < -8000 | y > 8000) {
    return true;
  } else {
    return false;
  }
}

// move the car according to the received value from Bluetooth
void control(uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4){
  digitalWrite(LF, x1);
  digitalWrite(LB, x2);
  digitalWrite(RF, x3);
  digitalWrite(RB, x4);
}

// calculate the current distance from the sensor
int calcDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  return pulseIn(echo, HIGH) * 0.034 / 2;
}

// switch the LEDs 
void ledSwitchRGB(uint8_t l1, uint8_t l2, uint8_t l3) {
    digitalWrite(red, l1);
    digitalWrite(green, l2);
    digitalWrite(blue, l3);
}

// Go back and rotate the car 
void dodge() {
  // Go back
  control(HIGH,LOW,LOW,HIGH);
  delay(250);

  // Rotate
  control(LOW,HIGH,LOW,HIGH);
  delay(300);

  // Stop
  control(LOW,LOW,LOW,LOW);
}


void loop() {
  // bluetooth
  while(Serial.available() > 0){
    val = Serial.read();
    // Serial.println(val);

        if (val == 'F') {control(LOW,HIGH,HIGH,LOW);}
    else if (val == 'B') {control(HIGH,LOW,LOW,HIGH);}
    else if (val == 'R') {control(LOW,HIGH,LOW,HIGH);}
    else if (val == 'L') {control(HIGH,LOW,HIGH,LOW);}
    else if (val == 'G') {control(LOW,LOW,HIGH,LOW);}
    else if (val == 'H') {control(LOW,LOW,LOW,HIGH);}
    else if (val == 'I') {control(LOW,HIGH,LOW,LOW);}
    else if (val == 'J') {control(HIGH,LOW,LOW,LOW);}
    else if (val == 'S') {control(LOW,LOW,LOW,LOW);}  // stop the car
  }


  // ultrasonic
  distance = calcDistance();
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);

  // gyroscope 
  GyX = (Wire.read()<<8|Wire.read()) + 480;
  GyY = (Wire.read()<<8|Wire.read()) + 170;

  isFlipped = updateIsFlipped(GyX, GyY);

  // conditions

  if (distance < 25) {
    ledSwitchRGB(HIGH,LOW,LOW);
    digitalWrite(buzzer, HIGH);
    dodge();
  } else if (isFlipped == 1) {
    ledSwitchRGB(HIGH,LOW,LOW);
    digitalWrite(buzzer, HIGH);
    digitalWrite(espLine, HIGH);
  } else if (distance < 50) {
    ledSwitchRGB(LOW,HIGH,LOW);
    digitalWrite(buzzer, LOW);
    digitalWrite(espLine, LOW);
  } else if (distance < 75) {
    ledSwitchRGB(LOW,LOW,HIGH);
    digitalWrite(buzzer, LOW);
    digitalWrite(espLine, LOW);
  } else {
    ledSwitchRGB(LOW,LOW,LOW);
    digitalWrite(buzzer, LOW);
    digitalWrite(espLine, LOW);
  }

  delay(300);
}



