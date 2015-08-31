#include "Water.h"

Water::Water() {
    
}

void Water::setup() {
    waterWidth = 640;
    waterHeight = 480;
    border = 0;
    waterLevel = 0;
    moveFlag = false;
    
    img.loadImage("bubble.png");
    swell.load("", "Shaders/Swell.frag");
    fbo.allocate(waterWidth, waterHeight+100);
    camTex.allocate(waterWidth, waterHeight+100, GL_RGB);
    
    //　カメラの初期設定
    camera.setVerbose(true);
    camera.setDeviceID(0);
    camera.initGrabber(waterWidth, waterHeight);
    
    // ベクトル場の初期化
    VF.setupField(64, 48, waterWidth, waterHeight);
    
    // pixelsBufferの初期化
    pixelsBuffer = new unsigned char[waterWidth * waterHeight * 3];
    
    // 泡のbox2d
    bubbleWorld.init();
    bubbleWorld.setFPS(60.0);
    
    // 水面のbox2d
    borderWorld.init();
    borderWorld.setFPS(60.0);
    borderWorld.setGravity(0, 0);
    
    // 水面の初期設定
    for ( int i=0; i<waterWidth; i++ ) {
        waterBorder.addVertex(i, border);
    }
    // 水面の基準となる球
    for ( int i=0; i<60; i++ ) {
        shared_ptr<ofxBox2dCircle> circle = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(1, 1, 1);
        circle.get()->setup(borderWorld.getWorld(), i*21 - waterWidth/2, border, 2);
        borderCircles.push_back(circle);
    }
    
}

void Water::borderUpdate() {
     //waterLevel = 150*sin(ofGetFrameNum()/180.0*PI);
    // 水面
    for ( int i=1; i<borderCircles.size()-1; i++ ) {
        ofVec2f pos = borderCircles[i].get()->getPosition();
        float   dist = (border - pos.y) + waterLevel;
        
        // 波をだんだん弱くする
        borderCircles[i].get()->setVelocity(borderCircles[i].get()->getVelocity() * 0.99);
        
        // ベクトル場に力があれば揺らす
        ofVec2f frc = VF.getForceFromPos(pos.x, pos.y);
        if ( frc.length() > 0) {
            borderCircles[i].get()->addForce(ofVec2f(0, frc.y), 15);
        }
        
        // 水面の反発力
        borderCircles[i].get()->addForce(ofVec2f(0, dist), 0.1);
        
        //　隣の水面に力を加える
        ofVec2f pos1 = borderCircles[i-1].get()->getPosition();
        float force1 = pos.y - pos1.y;
        borderCircles[i-1].get()->addForce(ofVec2f(0, force1), 0.3);
        //　隣の水面に力を加える
        ofVec2f pos2 = borderCircles[i+1].get()->getPosition();
        float force2 = pos.y - pos2.y;
        borderCircles[i+1].get()->addForce(ofVec2f(0, force2), 0.3);
    }
    // 端の固定
    borderCircles[0].get()->setPosition(-waterWidth/2, border+waterLevel);
    borderCircles[borderCircles.size()-1].get()->setPosition(waterWidth+waterWidth/2, border+waterLevel);
}

void Water::bubbleUpdate() {
    
    // 下から泡発生
    if ( ofGetFrameNum()%10 == 0 ) {
        ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(1, 0.5, 0.1);
        circle.get()->setup(bubbleWorld.getWorld(),
                            ofRandom(0, waterWidth),
                            waterHeight - 10,
                            ofRandom(1, 5));
        bubbles.push_back(circle);
    }
    
    // 泡の処理
    for ( int i=0; i<bubbles.size(); i++ ) {
        // 浮力
        float size = bubbles[i].get()->getRadius();
        float frequency = (7.0/size) * sin(ofGetFrameNum()/(float)size * PI);
        ofVec2f pos = bubbles[i].get()->getPosition();
        ofVec2f buoyancy = ofVec2f(bubbles[i].get()->getVelocity().x + frequency, -size * size);
        
        bubbles[i].get()->setVelocity(buoyancy);
        
        // ベクトル場に力があればボールに力を得る
        ofVec2f frc = VF.getForceFromPos(pos.x, pos.y);
        if ( frc.length() > 0 ) {
            bubbles[i].get()->addForce(ofVec2f(frc.x, frc.y), 1);
        }
        
        // 泡の削除のため画面外に泡を飛ばす
        for ( int j=0; j<borderCircles.size(); j++ ) {
            ofVec2f posBorder = borderCircles[j].get()->getPosition();
            if ( pos.y < posBorder.y+size*2 && ofDist(posBorder.x, 0, pos.x, 0) <= 21 ) {
                bubbles[i].get()->setPosition(pos.x, -100);
            }
        }
    }
    // 泡の削除のために画面外に泡を飛ばす
    ofRemove(bubbles, ofxBox2dBaseShape::shouldRemoveOffScreen);
}

void Water::createBubble(int x, int y) {
    // 動体検知したところから泡発生
    if ( x%30 == 0 && y%30 == 0 && bubbles.size() < 50 ) {
        float size = ofRandom(1, 5);
        ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(1, 0.5, 0.1);
        circle.get()->setup(bubbleWorld.getWorld(),
                            x + ofRandom(-30, 30),
                            y + ofRandom(-30, 30),
                            size);
        bubbles.push_back(circle);
    }
}

void Water::cameraUpdate() {
    differPixels();
    
    // カメラとピクセル情報の更新
    camera.update();
    if ( camera.isFrameNew() ) {
        unsigned char *pixels = camera.getPixels();
    }
    
}

void Water::update() {
    bubbleUpdate();
    bubbleWorld.update();
    
    if ( moveFlag ) {
        borderUpdate();
        borderWorld.update();
    }
    
    cameraUpdate();
    
    /*camTex.loadData(camera.getPixels(), waterWidth, waterHeight, GL_RGB);
    fbo.begin();
    swell.begin();
    swell.setUniform1f("u_time", ofGetElapsedTimef());
    swell.setUniform1f("u_waterH", border + waterLevel);
    swell.setUniform2f("u_resolution", waterWidth, waterHeight);
    swell.setUniformTexture("u_camTex", camTex, 0);
    ofRect(0, 0, waterWidth, waterHeight);
    swell.end();
    fbo.end();*/
    
    // ベクトル場を弱める
    VF.fadeField(0.8f);
}

void Water::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // 水面と水中の描写
    ofSetLineWidth(0);
    ofSetColor(0, 100, 155, 150);
    ofBeginShape();
    ofVertex(waterWidth, border+waterLevel);
    ofVertex(waterWidth, waterHeight+100);
    ofVertex(0, waterHeight+100);
    ofVertex(0, border+waterLevel);
    for (int i=0; i<borderCircles.size(); i++) {
        ofVec2f pos = borderCircles[i].get()->getPosition();
        ofCurveVertex(pos.x, pos.y);
    }
    ofEndShape();
    
    ofSetColor(255, 255);
    //　泡の描写
    for (int i=0; i<bubbles.size(); i++) {
        ofVec2f pos = bubbles[i].get()->getPosition();
        float size = bubbles[i].get()->getRadius();
        img.draw(pos, size*2, size*2);
    }
    
    ofSetColor(255, 150);
    //fbo.draw(0, border + waterLevel);
    
    ofDisableBlendMode();
}

void Water::differPixels() {
    // 現在のカメラのpixelデータを取得
    unsigned char *pixels = camera.getPixels();
    for ( int y=0; y<waterHeight; y++ ) {
        for ( int x=0; x<waterWidth; x++ ) {
            // pixelのindexの設定
            int index = (y*waterWidth + x) * 3;
            // 前フレームとのRGBの差分を計算
            int R = int(pixels[index]) - int(pixelsBuffer[index]);
            int G = int(pixels[index+1]) - int(pixelsBuffer[index+1]);
            int B = int(pixels[index+2]) - int(pixelsBuffer[index+2]);
            // 前フレームとの差分が一定値を超えたら
            if ( R*R + G*G + B*B >= 70*70 ) {
                // ベクトル場に力を加える
                VF.addOutwardCircle(x, y, 20, 0.05);
                if ( y > border + waterLevel ) {
                    createBubble(x, y);
                }
            }
        }
    }
    // 最後に今のフレームのpixelをbufferに保存
    for ( int i=0; i<waterHeight*waterWidth*3; i++ ) {
        pixelsBuffer[i] = pixels[i];
    }
    
}

Water::~Water() {
    
}