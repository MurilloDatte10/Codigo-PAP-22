#include <SPI.h>
#include<Wire.h>
#include "RF24.h"
#include "printf.h"
RF24 myRadio (9, 10);

const int x_out = A0;
const int y_out = A1;
RF24 radio(10, 9);
const byte address[6] = "00001";
struct data {
  int xAxis;
  int yAxis;

};
data send_data;
int value;
byte addresses[][6] = {"0"};

const uint8_t num_channels = 126;
uint8_t values[num_channels];

void WriteData()
{
  send_data.xAxis = analogRead(x_out);
  send_data.yAxis = analogRead(y_out);
  myRadio.stopListening(); //Stop Receiving and start transminitng
  myRadio.openWritingPipe(0xF0F0F0F066);//Sends data on this 40-bit address
  myRadio.write(&send_data, sizeof(data));

  Serial.print("WriteData");
  Serial.print(".........");
  Serial.print(send_data.xAxis);
  Serial.print(" ");
  Serial.println(send_data.yAxis);
}

void ReadData()
{
  myRadio.openReadingPipe(1, 0xF0F0F0F066); //Which pipe to read, 40 bit Address
  myRadio.startListening(); //Stop Transminting and start Reveicing
  if ( myRadio.available())
  {
    while (myRadio.available())
    {
      myRadio.read( &value, sizeof(value) );
    }
    Serial.print("ReadData");
    Serial.print(".........");
    Serial.println(value);
    myRadio.stopListening();
  }
}
void setup() {
  // put your setup code here, to run once:
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

}

void loop() {
  WriteData();
}
