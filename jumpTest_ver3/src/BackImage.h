#pragma once

#include "ofMain.h"
#define  OSC_SPEED 0.13

class BackImage {
    
public:
    float   dispImgHeight; // ディスプレイを使う時の背景の高さ
    float   position; // 後ろの空の画像を動かすための座標
    float   speed; // それに伴う速度
    float   gravity; // 重力加速度
    bool    moveFlag; // isJampを保存する
    bool    takePicture; // 写真を撮るフラグ
    
    // 画像
    ofImage img; // 背景の画像
    //音
    ofSoundPlayer jumpSE; // ジャンプ音
    ofSoundPlayer fallSE;
    
    //コンストラクタとデストラクタ
    BackImage();
    virtual ~BackImage();
    
    void setup(bool displayF);
    void copyFlag(bool flag);
    void jamping(float initSpeed);
    void update(bool displayF);
    void draw(float x, float y, bool displayF);
    void goal(bool displayF);
    void stopper(bool displayF);
    
};