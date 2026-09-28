#include <Arduino.h>
#include <ESP32CAN.h>
#include <CAN_config.h>

/* the variable name CAN_cfg is fixed, do not change */
CAN_device_t CAN_cfg;


void setup() {
    Serial.begin(115200);
    Serial.println("Starting CAN RX");
    /* set CAN pins and baudrate */
    CAN_cfg.speed=CAN_SPEED_100KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_21;
    CAN_cfg.rx_pin_id = GPIO_NUM_22;
    /* create a queue for CAN receiving: 10 frames */
    CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));
    //initialize CAN Module
    ESP32Can.CANInit();

    CAN_filter_t r_filter;
    r_filter.FM = Single_Mode;
    r_filter.ACR0 = 0x00;
    r_filter.ACR1 = 0x20;
    r_filter.ACR2 = 0x00;
    r_filter.ACR3 = 0x00;
    r_filter.AMR0 = 0x00;
    r_filter.AMR1 = 0x1F;
    r_filter.AMR2 = 0xFF;
    r_filter.AMR3 = 0xFF;
    // ESP32Can.CANConfigFilter(&r_filter);
}

void loop() {
  
  CAN_frame_t rx_frame;

  if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE) {

    if(rx_frame.FIR.B.FF==CAN_frame_std)
      printf("New standard frame");
    else
      printf("New extended frame");

    int idField = rx_frame.MsgID;
    int dataFieldSize = rx_frame.FIR.B.DLC;

    if(rx_frame.FIR.B.RTR==CAN_RTR) {
      printf(" RTR from %d, DLC %d\r\n",idField,dataFieldSize);
    }
    else {
      printf(" from %d, DLC %d, Data: ",idField,dataFieldSize);
      for(int i = 0; i < dataFieldSize; i++) {
        printf("%d",rx_frame.data.u8[i]);
      }
      printf("\n");
    }

  } else {
    printf("No frame received\n");
  }

  delay(500);
  
}
