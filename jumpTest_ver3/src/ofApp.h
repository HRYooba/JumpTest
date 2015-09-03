/*
 プロジェクターで投影する場合
 */


#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2D.h"
#include "BackImage.h"
#include "CamShader.h"

#define PORT 5555 //自分のポート番号

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
    int     imgNum; // 写真の番号
    int     fadeinCount; // フェードインするカウント
    int     picCount; // 撮った写真を表示する時間
    float   ratioWidth; // 画面の大きさ / カメラの大きさ
    float   ratioHeight; // 画面の大きさ / カメラの大きさ
    bool    isJamp; // ジャンプしたかのフラグ
    bool    isStart; // スタート
    bool    isPic; // フラッシュ
    
    ofImage outImg; // 写真
    ofImage countImg[3]; // カウントの画像
    ofImage joney; // 宇宙飛行士の画像
    
    ofSoundPlayer itemSE;
    ofSoundPlayer flashSE;
    ofSoundPlayer countSE[3];
    
    // osc
    ofxOscReceiver receiver;
    
    CamShader camShader;
    BackImage back;
    
    ofxBox2d world;
    vector<ofPtr<ofxBox2dCircle> > circles;
};
