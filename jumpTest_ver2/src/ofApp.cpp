#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // OSCの初期設定
    receiver.setup(PORT);
    
    // setup
    ofSetFrameRate(60);
    ofBackground(0);
    width  = ofGetWidth();
    height = ofGetHeight();
    
    // 音
    itemSE.loadSound("SE/item.mp3");
    
    // 各クラス
    camShader.setup();
    back.setup();
    //chara.setup();
    //water.setup();
    
    // box2D
    world.init();
    world.setFPS(60.0);
#ifdef USE_DISPLAY
    world.setGravity(-10, 0);
#else
    world.setGravity(0, 10);
#endif
    
    // 画面の比
    ratioWidth  = ofGetWidth() / (float)camShader.camWidth;
    ratioHeight = ofGetHeight() / (float)camShader.camHeight;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // osc---------------------------------------------
    if( receiver.hasWaitingMessages() )
    {
        printf("JUMP!!\n");
        //次のメッセージを取得
        ofxOscMessage m;
        receiver.getNextMessage( &m );
        
        if ( m.getAddress() == "/jump" ){
            isJamp = true;
            back.jamping(m.getArgAsInt32(0));
            back.copyFlag(isJamp);
        }
    }
    /*if  ( receiver.hasWaitingMessages() ) {
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        isJamp = true;
        //chara.jamping();
        //chara.copyFlag(isJamp);
        back.jamping();
        back.copyFlag(isJamp);
    }*/
    // ------------------------------------------------
    
    // もし球のある座標に力があれば球を消す
    for ( int i=0; i<circles.size(); i++ ) {
        ofVec2f frc = camShader.VF.getForceFromPos(circles[i].get()->getPosition().x, circles[i].get()->getPosition().y);
        if ( frc.length() > 0.01 ) {
            circles[i].get()->setPosition(-1000, height+100);
            camShader.decisionEffect();
            itemSE.play();
            camShader.isEffect = true;
        }
    }
    // 球を発生させている
    /*if ( ofGetFrameNum()%300 == 100 && !camShader.isEffect ) {
        shared_ptr<ofxBox2dCircle> circle = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(1, 0.5, 1);
        float size = ofRandom(10, 15);
#ifdef USE_DISPLAY
        circle.get()->setup(world.getWorld(), height, ofRandom(camShader.camHeight/3, camShader.camHeight*2/3), 15);
#else
        circle.get()->setup(world.getWorld(), ofRandom(camShader.camWidth/3, camShader.camWidth*2/3), 10, 15);
#endif
        circles.push_back(circle);
    }*/
    
    // 球が外に出たらvector配列から消去
    ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
    world.update();
    camShader.update();
    back.update();
    //chara.update();
    //water.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
#ifdef USE_DISPLAY
    ofPushMatrix();
    ofTranslate(width/2, height/2);
    ofRotate(-90);
    ofSetColor(255);
    back.draw(-height/2, width, true);
    ofPopMatrix();
#else
    ofSetColor(255);
    back.draw(0, height, false);
#endif
    
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
        //chara.jamping();
        //chara.copyFlag(isJamp);
        back.jamping(200);
        back.copyFlag(isJamp);
    }
    if ( key == 's' ) {
        outImg.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        outImg.saveImage("/Users/oobahiroya/Desktop/SavedImage/img.png", OF_IMAGE_QUALITY_BEST);
    }
    if ( key == '1' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 0;
        camShader.isEffect = true;
    }
    if ( key == '2' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 1;
        camShader.isEffect = true;
    }
    if ( key == '3' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 2;
        camShader.isEffect = true;
    }
    if ( key == '4' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 3;
        camShader.isEffect = true;
    }
    if ( key == '5' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 4;
        camShader.isEffect = true;
    }
    if ( key == '6' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 5;
        camShader.isEffect = true;
    }
    if ( key == '0' ) {
        camShader.isEffect = false;
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
