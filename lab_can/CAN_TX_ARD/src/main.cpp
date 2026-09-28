#include <Arduino.h>
#include <ESP32CAN.h>
#include <CAN_config.h>

/* the variable name CAN_cfg is fixed, do not change */
CAN_device_t CAN_cfg;
void sendMessageType1();
void sendMessageType2();
void sendMessageType3();
void sendPotenciometer();

int8_t counter=0;
const int potentiometerPin = GPIO_NUM_34;


void setup() {
    Serial.begin(115200);
    Serial.println("Starting CAN TX");
    /* set CAN pins and baudrate */
    CAN_cfg.speed=CAN_SPEED_100KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_21;
    CAN_cfg.rx_pin_id = GPIO_NUM_22;

    //initialize CAN Module
    ESP32Can.CANInit();

}

void loop() {

  //sendMessageType1();
  //sendMessageType2();
  //sendMessageType3();

  sendPotenciometer();

}

/*void loop() {
  CAN_frame_t tx_frame;
  
  tx_frame.FIR.B.FF = CAN_frame_std;
  tx_frame.FIR.B.RTR = CAN_no_RTR; //before it was: tx_frame_1
  tx_frame.MsgID = 1;
  tx_frame.FIR.B.DLC = 8;
  tx_frame.data.u8[0] = 'F';
  tx_frame.data.u8[1] = 'r';
  tx_frame.data.u8[2] = 'a';
  tx_frame.data.u8[3] = 'm';
  tx_frame.data.u8[4] = 'e';
  tx_frame.data.u8[5] = '_';
  tx_frame.data.u8[6] = '_';
  tx_frame.data.u8[7] = '1';

  int ret_code = ESP32Can.CANWriteFrame(&tx_frame);
  printf("Transmitting CAN frame. Return code: ");
  printf("%d\n",ret_code);
  
  delay(2000);

}*/

void sendMessageType1() {
  //Send counter from 0 to 7 via CAN BUS
  // Data is the counter, so 1 "octet" (1 byte)

  CAN_frame_t tx_frame;
  
  tx_frame.FIR.B.FF = CAN_frame_std;
  tx_frame.FIR.B.RTR = CAN_no_RTR; //before it was: tx_frame_1
  tx_frame.MsgID = 1;
  tx_frame.FIR.B.DLC = 1;

  tx_frame.data.u8[0] = counter;

    int ret_code = ESP32Can.CANWriteFrame(&tx_frame);
  printf("Transmitting CAN frame. Return code: ");
  printf("%d\n",ret_code);
  
  delay(2000);

  counter++;


}

void sendMessageType2() {
  CAN_frame_t tx_frame;
  
  tx_frame.FIR.B.FF = CAN_frame_std;
  tx_frame.FIR.B.RTR = CAN_no_RTR; //before it was: tx_frame_1
  tx_frame.MsgID = 2;
  tx_frame.FIR.B.DLC = 6;

  tx_frame.data.u8[0] = 0;
  tx_frame.data.u8[1] = 0;
  tx_frame.data.u8[2] = 0;
  tx_frame.data.u8[3] = 0;
  tx_frame.data.u8[4] = 0;
  tx_frame.data.u8[5] = 0;

    int ret_code = ESP32Can.CANWriteFrame(&tx_frame);
  printf("Transmitting CAN frame. Return code: ");
  printf("%d\n",ret_code);
  
  delay(2000);

}

void sendMessageType3() {
  CAN_frame_t tx_frame;
  
  tx_frame.FIR.B.FF = CAN_frame_std;
  tx_frame.FIR.B.RTR = CAN_RTR; //before it was: tx_frame_1
  tx_frame.MsgID = 3;
  tx_frame.FIR.B.DLC = 4;

  /*
  RTR: NO SE NECESITA ENVIAR DATOS REMOTOS
  tx_frame.data.u8[0] = 'P';
  tx_frame.data.u8[0] = 'X';
  tx_frame.data.u8[0] = 'M';
  tx_frame.data.u8[0] = 'B';
  */


    int ret_code = ESP32Can.CANWriteFrame(&tx_frame);
  printf("Transmitting CAN frame. Return code: ");
  printf("%d\n",ret_code);
  
  delay(2000);

}
void sendPotenciometer() {

  int rawValue = analogRead(potentiometerPin); // Read the analog input
  Serial.println(rawValue);


  CAN_frame_t tx_frame;
  
  tx_frame.FIR.B.FF = CAN_frame_std;
  tx_frame.FIR.B.RTR = CAN_no_RTR; //before it was: tx_frame_1
  tx_frame.MsgID = 4;
  tx_frame.FIR.B.DLC = 2;

  tx_frame.data.u8[0] = rawValue&0xFF;
  tx_frame.data.u8[1] = rawValue&0xFF00;


    int ret_code = ESP32Can.CANWriteFrame(&tx_frame);
  printf("Transmitting CAN frame. Return code: ");
  printf("%d\n",ret_code);
  
  delay(1000);

}

