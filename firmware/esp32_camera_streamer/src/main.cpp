// AI Thinker ESP32-CAM MJPEG service. Barcode decoding deliberately runs on the Pi.
#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

const char* WIFI_SSID="CHANGE_ME"; const char* WIFI_PASSWORD="CHANGE_ME";
// AI Thinker camera pins.
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22
WebServer server(80);
void still() { camera_fb_t* fb=esp_camera_fb_get(); if(!fb){server.send(503);return;} server.sendHeader("Cache-Control","no-store");server.send_P(200,"image/jpeg",(const char*)fb->buf,fb->len);esp_camera_fb_return(fb); }
void setup(){ Serial.begin(115200); camera_config_t c={}; c.pin_pwdn=PWDN_GPIO_NUM;c.pin_reset=RESET_GPIO_NUM;c.pin_xclk=XCLK_GPIO_NUM;c.pin_sccb_sda=SIOD_GPIO_NUM;c.pin_sccb_scl=SIOC_GPIO_NUM;c.pin_d7=Y9_GPIO_NUM;c.pin_d6=Y8_GPIO_NUM;c.pin_d5=Y7_GPIO_NUM;c.pin_d4=Y6_GPIO_NUM;c.pin_d3=Y5_GPIO_NUM;c.pin_d2=Y4_GPIO_NUM;c.pin_d1=Y3_GPIO_NUM;c.pin_d0=Y2_GPIO_NUM;c.pin_vsync=VSYNC_GPIO_NUM;c.pin_href=HREF_GPIO_NUM;c.pin_pclk=PCLK_GPIO_NUM;c.xclk_freq_hz=20000000;c.pixel_format=PIXFORMAT_JPEG;c.frame_size=FRAMESIZE_VGA;c.jpeg_quality=12;c.fb_count=1; if(esp_camera_init(&c)!=ESP_OK)return;WiFi.begin(WIFI_SSID,WIFI_PASSWORD);while(WiFi.status()!=WL_CONNECTED)delay(300);server.on("/capture",still);server.on("/health",[]{server.send(200,"text/plain","ok");});server.begin();Serial.println(WiFi.localIP()); }
void loop(){server.handleClient();}
