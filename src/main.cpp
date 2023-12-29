#include <Arduino.h>
//#include "screen.hpp"
#include "class.hpp"

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

#include <U8g2lib.h>
#include <Wire.h>
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 9, /* data=*/ 10, /* cs=*/ 13, /* dc=*/ 12, /* reset=*/ 11);

//接收机的MAC地址，全为FF则是广播地址
uint8_t receive_MACAddress[] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

//发送的数据类型
class DATA {
public:
    float ly;
    float lx;
    float ry;
    float rx;
    float lknob;
    float rknob;
};
DATA value;//发送的数据

//Screen screen;
Rocker leftjoy(6,7,1,1,0.02);
Rocker rightjoy(1,2,1,-1,0.02);
Knob leftK(5);
Knob rightK(8);
Toggle_Switch R1(36,37);
Toggle_Switch R2(35,48);
Toggle_Switch L1(16,19);
Toggle_Switch L2(3,20);
Encoder_Konb Lencoder(0,17,18,-1);
Encoder_Konb Rencoder(0,38,39,-1);
Button BL1(15);
Button BL2(4);
Button BR1(40);
Button BR2(41);
void updata_data(void* pvParameters){
    while(1){
        leftjoy.get_x();
        leftjoy.get_y();
        rightjoy.get_x();
        rightjoy.get_y();
        leftK.callback();
        rightK.callback();
        L1.callback();
        L2.callback();
        R1.callback();
        R2.callback();
        delay(10);
    }
}

void updata_encoder_data(void* pvParameters){
    while(1){
        Lencoder.get_value();
        Rencoder.get_value();
        delay(10);
    }
}
// 回调函数,函数将在发送消息时执行。此函数告诉我们信息是否成功发送;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void screen_update(void* pvParameters){
    String send_MAC; 
    for (int i = 0; i < 6; i++){
        send_MAC=send_MAC+String(receive_MACAddress[i],HEX);
    }
    
    while (true){
        u8g2.clearBuffer();
        switch (L1.get_state()){
            case -1:
                u8g2.setCursor(0, 10);
                u8g2.print("LX:");
                u8g2.print(leftjoy.X);
                u8g2.print(" ");
                u8g2.print("RX:");
                u8g2.print(rightjoy.X);
                u8g2.setCursor(0, 22);
                u8g2.print("LY:");
                u8g2.print(leftjoy.Y);
                u8g2.print(" ");
                u8g2.print("RY:");
                u8g2.print(rightjoy.Y);
                u8g2.setCursor(0, 34);
                u8g2.print("LE:");
                u8g2.print(Lencoder.get_value());
                u8g2.print(" ");
                u8g2.print("RE:");
                u8g2.print(Rencoder.get_value());
                u8g2.setCursor(0, 46);
                u8g2.print("LK:");
                u8g2.print(leftK.get_value());
                u8g2.print(" ");
                u8g2.print("RK:");
                u8g2.print(rightK.get_value());
                u8g2.setCursor(0, 58);
                break;
            case 0:

                u8g2.drawCircle(32,31,30);
                u8g2.drawCircle(95,31,30);
                u8g2.drawLine(32,31,32+int(30*leftjoy.X),31-int(30*leftjoy.Y));
                u8g2.drawLine(95,31,95+int(30*rightjoy.X),31-int(30*rightjoy.Y));
                break;
            case 1:
                u8g2.setCursor(0, 10);
                u8g2.print("L1:");
                u8g2.print(L1.get_state());

                u8g2.print("L2:");
                u8g2.print(L2.get_state());

                u8g2.print("R1:");
                u8g2.print(R1.get_state());

                u8g2.print("R2:");
                u8g2.print(R2.get_state());

                u8g2.setCursor(0, 22);
                u8g2.print("BL1:");
                u8g2.print(BL1.get_state());
                u8g2.print(" ");
                u8g2.print("BR1:");
                u8g2.print(BR1.get_state());
                u8g2.setCursor(0, 34);
                u8g2.print("BL2:");
                u8g2.print(BL2.get_state());
                u8g2.print(" ");
                u8g2.print("BR2:");
                u8g2.print(BR2.get_state());


                u8g2.setCursor(0, 58);
                u8g2.print("SendMAC:");
                u8g2.print(send_MAC);
                break;
        }

        u8g2.sendBuffer();
        delay(20);
    }
    
}
void LEInterrupt(){
    Lencoder.dt_callback();
}

void REInterrupt(){
    Rencoder.dt_callback();
}

void R2Interrupt(){
    R2.callback();
}
void BL1Interrupt(){
    BL1.callback();
}
void BL2Interrupt(){
    BL2.callback();
}
void BR1Interrupt(){
    BR1.callback();
}
void BR2Interrupt(){
    BR2.callback();
}
void setup() {
    //put your setup code here, to run once:
    u8g2.begin();
    //u8g2.setBusClock(200000);
    u8g2.enableUTF8Print();		// 启用UTF8打印函数支持
    u8g2.setFont(u8g2_font_wqy13_t_gb2312b);  // 使用 u8g2_font_wqy13_t_gb2312 字体渲染
    u8g2.setFontDirection(0);
    Serial.begin(9600);
    leftjoy.setup();
    rightjoy.setup();
    leftK.setup();
    rightK.setup();
    L1.setup();
    L2.setup();
    R1.setup();
    R2.setup();
    Lencoder.setup();
    Rencoder.setup();
    BL1.setup();
    BL2.setup();
    BR1.setup();
    BR2.setup();

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed");
        return;
    }
        //注册回调函数
    esp_now_register_send_cb(OnDataSent);

    esp_now_peer_info_t peerInfo;
    peerInfo.ifidx = WIFI_IF_STA;
    memcpy(peerInfo.peer_addr, receive_MACAddress, 6);

    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }

    attachInterrupt(digitalPinToInterrupt(15),BL1Interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(4),BL2Interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(40),BR1Interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(41),BR2Interrupt, CHANGE);

    attachInterrupt(digitalPinToInterrupt(18),LEInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(39),REInterrupt, CHANGE);
    xTaskCreatePinnedToCore(updata_data, "updata_data", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(updata_encoder_data, "updata_encoder_data", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(screen_update, "screen_update", 20*1000, NULL, 2, NULL, 0);
    //screen.setup();
}

void loop() {

    value.ly = leftjoy.Y;
    value.lx = leftjoy.X;
    value.ry = rightjoy.Y;
    value.rx = rightjoy.X;
    value.lknob = leftK.get_value();
    value.rknob = rightK.get_value();

    //发送数据
    esp_err_t result = esp_now_send(receive_MACAddress, (uint8_t *) &value, sizeof(value));

    delay(20);


}

