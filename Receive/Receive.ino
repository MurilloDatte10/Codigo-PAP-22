#include <SPI.h>
#include<Wire.h>
#include "RF24.h"
#include "printf.h"
RF24 myRadio (9, 10);

int value;
byte addresses[][6] = {"0"};

const uint8_t num_channels = 126;
uint8_t values[num_channels];

int ENA = 3;
int ENB = 9;
int MotorA1 = 4;
int MotorA2 = 5;
int MotorB1 = 6;
int MotorB2 = 7;


struct data {
  int xAxis;
  int yAxis;

};
data receive_data;


//void WriteData()
//{
//  value = random(255);
//  myRadio.stopListening(); //Stop Receiving and start transminitng
//  myRadio.openWritingPipe(0xF0F0F0F066);//Sends data on this 40-bit address
//  myRadio.write(&value, sizeof(value));
//  Serial.print("WriteData");
//  Serial.print(".........");
//  Serial.println(value);
//}

void ReadData()
{
  myRadio.openReadingPipe(1, 0xF0F0F0F066); //Which pipe to read, 40 bit Address
  myRadio.startListening(); //Stop Transminting and start Reveicing
  if ( myRadio.available())
  {
    while (myRadio.available())
    {
      myRadio.read( &receive_data, sizeof(receive_data) );
    }
    //    Serial.print("ReadData");
    //    Serial.print(".........");
    //    Serial.print(receive_data.xAxis);
    //    Serial.print(" ");
    //    Serial.println(receive_data.yAxis);
    myRadio.stopListening();
    Movimento();
  }
}



void setup() {
  // put your setup code here, to run once:
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  Serial.begin(9600);

  printf_begin();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(F("\n\RF_NANO v3.0 Test"));
  Serial.println();
  Serial.println();



  //
  // Setup and configure rf radio
  //

  myRadio.begin();
  myRadio.setAutoAck(false);

  // Get into standby mode
  myRadio.startListening();
  myRadio.stopListening();
  myRadio.printDetails();

  myRadio.begin();
  myRadio.setChannel(115);  //115 band above WIFI signals
  myRadio.setPALevel(RF24_PA_MAX); //MIN power low rage
  myRadio.setDataRate( RF24_1MBPS ) ;  //Minimum speed
  //  Serial.print("Setup Initialized");
  delay(500);
}

void loop() {
  ReadData();



}

void Movimento() {
  Serial.print("X: ");
  Serial.print(receive_data.xAxis);
  Serial.print(" Y: ");
  Serial.print(receive_data.yAxis);




  if (receive_data.yAxis > 460) {
    moveF();
    Serial.println("frente");
  } else if (receive_data.yAxis < 370) {
    moveB();
    Serial.println("BAck");
  } else if (receive_data.xAxis <= 360) {
    moveL();
    Serial.println("left");
  } else if (receive_data.xAxis >= 420) {
    moveR();
    Serial.println("right");
  } else {
    Serial.println("stop");
    moveS();
  }

}


void moveF() {

  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}


void moveB() {

  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, HIGH);
  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, HIGH);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void moveL() {

  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, HIGH);
  digitalWrite(MotorB1, HIGH);
  digitalWrite(MotorB2, LOW);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void moveR() {
  digitalWrite(MotorA1, HIGH);
  digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, HIGH);
  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}
void moveS() {
  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
