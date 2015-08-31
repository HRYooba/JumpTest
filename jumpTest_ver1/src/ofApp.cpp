#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // OSCの初期設定
    receiver.setup(PORT);
    
    // setup
    ofSetFrameRate(60);
    ofBackground(0);
    
    width = ofGetWidth();
    height = ofGetHeight();
    
    backImg.setup();
    camShader.setup();
    chara.setup();
    water.setup();
    
    world.init();
    world.setFPS(60.0);
    world.setGravity(0, 10);
    
    ratioWidth = ofGetWidth() / (float)camShader.camWidth;
    ratioHeight = ofGetHeight() / (float)camShader.camHeight;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // osc---------------------------------------------
    if  ( receiver.hasWaitingMessages() ) {
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        isJamp = true;
        chara.jamping();
        chara.copyFlag(isJamp);
        backImg.jamping();
        backImg.copyFlag(isJamp);
    }
    // ------------------------------------------------
    
    // もし球のある座標に力があれば力を加える
    for ( int i=0; i<circles.size(); i++ ) {
        ofVec2f frc = camShader.VF.getForceFromPos(circles[i].get()->getPosition().x, circles[i].get()->getPosition().y);
        if ( frc.length() > 0.01 ) {
            circles[i].get()->setPosition(-1000, height+100);
            camShader.isEffect = camShader.isEffect ? false : true;
            camShader.decisionEffect();
            //camShader.effectNum = 1;
            //camShader.isEffect = true;
            //circles[i].get()->addForce(ofVec2f(frc.x, frc.y), 1500);
        }
    }
    // 球を発生させている
    if ( ofGetFrameNum()%300 == 100 ) {
        shared_ptr<ofxBox2dCircle> circle = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(1, 0.5, 1);
        float size = ofRandom(10, 15);
        circle.get()->setup(world.getWorld(), ofRandom(camShader.camWidth/3, camShader.camWidth*2/3), 10, 15);
        circles.push_back(circle);
    }
    
    // 球が外に出たらvector配列から消去
    ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
    world.update();
    backImg.update();
    camShader.update();
    chara.update();
    //water.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    backImg.draw(0, height);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    // size(640, 480)のカメラ関連のものを画面の大きさに合わせる
    ofPushMatrix();
    ofScale(-ratioWidth, ratioHeight);
    ofTranslate(-camShader.camWidth, 0);
    ofSetColor(255);
    camShader.draw(0, 0);
    for ( int i=0; i<circles.size(); i++ ) {
        ofSetColor(255, 50, 150);
        float x = circles[i].get()->getPosition().x;
        float y = circles[i].get()->getPosition().y;
        float r = circles[i].get()->getRadius();
        ofCircle(x, y, r);
    }
    ofSetColor(255);
    //water.draw();
    ofPopMatrix();
    
    //chara.draw(width/2, height-100);
    ofDisableBlendMode();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == ' ' ) {
        isJamp = true;
        chara.jamping();
        chara.copyFlag(isJamp);
        backImg.jamping();
        backImg.copyFlag(isJamp);
    }
    if ( key == 's' ) {
        outImg.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        //outImg.saveImage("./SavedImage/img.png", OF_IMAGE_QUALITY_BEST);
        outImg.saveImage("/Users/oobahiroya/Desktop/SavedImage/img.png", OF_IMAGE_QUALITY_BEST);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (button == 0) {
        camShader.pastUpdate();
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
