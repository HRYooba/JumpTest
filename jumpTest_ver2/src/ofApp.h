#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2D.h"
#include "BackImage.h"
#include "CamShader.h"
#include "Charactor.h"
#include "Water.h"

#define PORT 5555 //自分のポート番号
//#define USE_DISPLAY // コメントアウトを外すとフルスクリーンになり画面が横になる

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int     width; // 画面の幅
    int     height; // 画面の高さ
    float   ratioWidth; // 画面の大きさ / カメラの大きさ
    float   ratioHeight; // 画面の大きさ / カメラの大きさ
    bool    isJamp; // ジャンプしたかのフラグ
    
    ofImage outImg;
    ofSoundPlayer itemSE;
    
    // osc
    ofxOscReceiver receiver;
    
    CamShader camShader;
    BackImage back;
    //Charactor chara;
    //Water water;
    ofxBox2d world;
    vector<ofPtr<ofxBox2dCircle> > circles;
};
