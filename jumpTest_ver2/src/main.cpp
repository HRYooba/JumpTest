#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    //ofSetupOpenGL(640*1.5,480*1.5,OF_WINDOW);			// <-------- setup the GL context
    //ofSetupOpenGL(640, 480*1.5,OF_WINDOW);
    ofSetupOpenGL(640, 480, OF_FULLSCREEN);
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
    
}
