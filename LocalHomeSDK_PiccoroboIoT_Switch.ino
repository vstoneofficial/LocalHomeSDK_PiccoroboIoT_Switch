#include <vs-rc202.h>
#include <ESP8266WiFi.h>
#include "udp.h"
#include "httpsrv.h"
#include "motion.h"

#define SSID "********"
#define PASSWORD "********"

LocalHomeUDP udp;

void setup() {
    Serial.begin(115200);
    // V-duino(VS-RC202)のライブラリを使用するための初期化処理
    initLib();
    
    // Wifi接続
    Serial.printf("Connecting to %s ", SSID);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println(" connected");
    Serial.printf("IP : %s\n", WiFi.localIP().toString().c_str());

    // UDP通信開始
    udp.begin();
    // Httpサーバ開始
    localHomeSrv.begin();
    
    // 各サーボを有効化
    servoEnable(1, 1);
    servoEnable(2, 1);
    servoEnable(3, 1);
    servoEnable(4, 1);
}

void loop() {
    // UDP通信処理
    udp.task();
    // Httpサーバ待受け処理
    localHomeSrv.task();
    
    detectMotion();
    selectMotion();
}

void detectMotion() {
    int motionNum = M_NUM0;
    // スイッチONの状態で動作するモーションを設定する
    if(status.on){
        motionNum = M_NUM1;
        Serial.println("status on");
    }
    
    // モーション番号を設定する
    setMotionNumber(motionNum);
}

void selectMotion(){
    // 設定していたモーション番号を取得し、その番号毎にモーションを実行する
    switch(getMotionNumber()){              
        case M_NUM0:
            // 初期姿勢(正面を向いている状態)のモーションを一度だけ実行する
            playMotionOnce(motion0, 1);
            break;
        case M_NUM1:
            // モーションを繰り返し実行する
            playMotion(motion3, 5);
            break;
    }
}