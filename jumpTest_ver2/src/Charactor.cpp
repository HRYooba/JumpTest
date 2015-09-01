#include "Charactor.h"


Charactor::Charactor() {
    
}

void Charactor::setup() {
    img.loadImage("Image/player.png");
    init();
}

void Charactor::init() {
    charaSpeed   = 40;
    charaGravity = 2.0;
    charaCount   = 0;
    charaPos     = 0;
}

void Charactor::copyFlag(bool flag) {
    moveFlag = flag;
}

void Charactor::jamping() {
    init();
}

void Charactor::update() {
    if (moveFlag) {
        if (charaCount < 36) {
            charaSpeed -= charaGravity;
            charaPos -= charaSpeed;
            charaCount++;
        } else {
            moveFlag = false;
        }
    } else {
        init();
    }
}

void Charactor::draw(float x, float y) {
    float size = 200;
    img.draw(x-size/2, y+charaPos-size/2, size, size);
}

Charactor::~Charactor() {
    
}