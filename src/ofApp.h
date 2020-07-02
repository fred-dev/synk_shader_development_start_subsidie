#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKinectV2.h"
#include "matrixManipulator.h"

#include "ofxOsc.h"

// listen on port 12345
#define PORT 12345

class ofApp : public ofBaseApp{
	public:
		
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    void reloadShader();
    
	ofShader finalShader;
	ofShader texShader;
	ofFbo texShaderFboPing, texShaderFboPong;


    ofEasyCam cam;
    int width, height;
    
	ofParameter<bool> useSceneShader;
	ofParameter<bool> useTexShader;
	ofParameterGroup sceneShaderPanelVert;
	ofParameterGroup sceneShaderPanelFrag;
	ofParameterGroup sceneShaderPanelMode;

	ofParameter<float> floatsVertScene[6];
	ofParameter<float> floatsFragScene[6];
	ofParameter<int> sceneShaderMode;

	ofParameterGroup texShaderPanelVert;
	ofParameterGroup texShaderPanelFrag;
	ofParameterGroup texShaderPanelMode;

	ofParameter<float> floatsVertTexture[6];
	ofParameter<float> floatsFragTexture[6];
	ofParameter<int> texShaderMode;

	ofxPanel shaderControls;
	ofParameter<int> OSCPort;
	ofParameter<int> openClDevice;
    ofParameter<bool> drawAxis;
	ofParameter<int> drawMode;
    
   // bool useShader;
    ofxPanel panel;
    
    ofxPanel kinectPanel;
    
    std::shared_ptr<ofxKinectV2> kinect;
    bool hasKinect;
     
  

    ofVboMesh pointClouds;
    ofTexture texRGBRegistered;

	ofxOscReceiver receiver;

	matrixManipulator matrixControls;
	ofxPanel matrixPanel;

};
