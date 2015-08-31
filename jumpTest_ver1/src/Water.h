#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "VectorField.h"

class Water {
public:
    int waterWidth;
    int waterHeight;
    float border;
    float waterLevel;
    bool moveFlag;
    
    unsigned char *pixelsBuffer; //1フレーム前のpixel情報を保存するところ
    
    ofImage img;
    ofPolyline waterBorder;
    ofShader swell;
    ofFbo fbo;
    ofTexture camTex;
    ofVideoGrabber camera;
    
    ofxBox2d bubbleWorld;
    ofxBox2d borderWorld;
    vector <ofPtr<ofxBox2dCircle> > bubbles;
    vector <ofPtr<ofxBox2dCircle> > borderCircles;
    
    VectorField VF;
    
    
    Water();
    virtual ~Water();
    
    void setup();
    void update();
    void draw();
    void createBubble(int x, int y);
    void borderUpdate();
    void bubbleUpdate();
    void cameraUpdate();
    void differPixels();
};