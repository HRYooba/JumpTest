#pragma once

#include "ofMain.h"

class BackImage {
    
public:
    int     takePicCount; // 写真を撮るカウント
    float   ratioSpeed; // speedにかける比率
    float   dispImgHeight; // ディスプレイを使う時の背景の高さ
    float   position; // 後ろの空の画像を動かすための座標
    float   speed; // それに伴う速度
    float   gravity; // 重力加速度
    bool    moveFlag; // isJampを保存する
    bool    takePicture; // 写真を撮るフラグ
    bool    isGoal; //　ゴールしたかの判定
    
    // 画像
    ofImage img; // 背景の画像
    
    //音
    ofSoundPlayer jumpSE; // ジャンプ音
    ofSoundPlayer fallSE;
    
    //コンストラクタとデストラクタ
    BackImage();
    virtual ~BackImage();
    
    void setup();
    void init();
    void copyFlag(bool flag);
    void jamping(float initSpeed);
    void update();
    void draw(float x, float y);
    void goal();
    void stopper();
    void takePictureMode(bool isGoal);
    
};