#include <ESP8266WiFi.h>  // 引入ESP8266WiFi库

int location = 3;  // 当前所处位置
const char* ssid = "ChinaNet-chaoke1209";        // WIFI名称
const char* password = "chaokehotel";    // WIFI密码
const char* serverIP = "192.168.1.11";    // 服务器(PC)IP地址

const int serverPort = 28397;            // 设置接收数据的服务器（PC）的端口

WiFiClient client;  // 创建一个WiFiClient对象

unsigned long checkConnectionInterval = 10000;  // 设置检查连接间隔（毫秒）
unsigned long lastCheckConnectionTime = 0;      // 上一次检查连接的时间

void setup() 
{
  switch(location)
  {
    case 1:
      ssid = "iscit31624"; // 设置WiFi名称
      password = "3162431624"; // 设置WiFi密码
      serverIP = "192.168.31.28";  // 设置接收数据的服务器（PC）的IP地址
      break;
    case 2:
      ssid = "HUAWEI-F38LTN";      // 设置WiFi名称
      password = "GaGeAsSeBr027";  // 设置WiFi密码
      serverIP = "192.168.3.15";   // 设置接收数据的服务器（PC）的IP地址
      break;
    case 3:
      ssid = "ChinaNet-chaoke1209";      // 设置WiFi名称
      password = "chaokehotel";  // 设置WiFi密码
      serverIP = "192.168.1.11";   // 设置接收数据的服务器（PC）的IP地址
      break;
  }
  Serial.begin(9600);        // 初始化串口，设置波特率为9600
  WiFi.begin(ssid, password);  // 连接WiFi

  // 等待直到连接成功
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // 连接到服务器（PC）
  client.connect(serverIP, serverPort);
}

void loop() {
  
  // 当从串口接收到数据时
  if (Serial.available()) {
    String data = Serial.readStringUntil('\0');  // 读取一行数据，以波浪号(~)为结束标志
    client.print(data);                        // 将数据发送到服务器（PC）
    client.print(238);
  }

  // 检查客户端连接
  if (millis() - lastCheckConnectionTime >= checkConnectionInterval) {
    lastCheckConnectionTime = millis();

    // 如果客户端未连接到服务器，尝试重新连接
    if (!client.connected()) {
      client.connect(serverIP, serverPort);
    }
  }
}
