#include "BackImage.h"

BackImage::BackImage() {
    
}

// setup
void BackImage::setup(bool displayF) {
    // 読み込む画像の設定
    img.loadImage("Image/back.jpg");
    // soundPlayer
    jumpSE.loadSound("SE/jump.mp3");
    fallSE.loadSound("SE/fall.mp3");
    // 初期化
    if ( displayF ) {
        dispImgHeight = img.height * ofGetHeight() / img.width;
        position      = -dispImgHeight;
    } else {
        position      = -img.height;
    }
    gravity       = 1.8;
    moveFlag      = false;
    speed         = 0;
}

// ジャンプした判定をコピーする
void BackImage::copyFlag(bool flag) {
    moveFlag = flag;
}

// ジャンプした時に呼ばれる関数
void BackImage::jamping(float intiSpeed) {
    speed = intiSpeed * OSC_SPEED;
    if ( speed >= 100 ) {
        speed = 100;
    }
    if ( speed <= 30 ) {
        speed = 30;
    }
    printf("initSpeed : ");
    printf("%f\n", speed);
    //jumpSE.play();
}

// update
void BackImage::update(bool displayF) {
    goal(displayF);
    if ( moveFlag ) {
        speed -= gravity;
        position += speed;
        stopper(displayF);
    }
}


// ゴール
void BackImage::goal(bool displayF) {
    if ( displayF ) {
        if ( position >=  -ofGetWidth()) {
            takePicture = true;
            moveFlag    = false;
            position    = -dispImgHeight;
        }
    } else {
        if ( position >= -ofGetHeight() ) {
            takePicture = true;
            moveFlag    = false;
            position    = -img.height;
        }
    }
}

// 最地点の設定
void BackImage::stopper(bool displayF) {
    if ( displayF ) {
        if ( position <= -dispImgHeight ) {
            position = -dispImgHeight;
        }
    } else {
        if ( position <= -img.height ) {
            position = -img.height;
        }
    }
}


// draw
void BackImage::draw(float x, float y, bool displayF) {
    if ( displayF ) {
        img.draw(x, y+position, ofGetHeight(), dispImgHeight);
    } else {
        img.draw(x, y+position, ofGetWidth(), img.height);
    }
}

BackImage::~BackImage() {
    
}