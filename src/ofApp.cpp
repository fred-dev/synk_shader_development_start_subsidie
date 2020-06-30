#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    

    panel.setup("Controls", "settings.xml");
	panel.add(useSceneShader.set("Scene shader", true));
	panel.add(useTexShader.set("Texture shader", true));
    panel.add(drawWireframe.set("Draw wireframe", false));
    panel.add(drawAxis.set("Draw Axis", false));
	panel.add(openClDevice.set("openCL Device", 2,0,3));
	panel.add(OSCPort.set("OSC Port", 1234, 1000, 20000));
    panel.loadFromFile("settings.xml");


    ofxKinectV2 tmp;
    std::vector <ofxKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
    if (deviceList.size()>0) {
        kinect = std::make_shared<ofxKinectV2>();
        kinect->open(deviceList[0].serial, ofProtonect::PacketPipelineType::OPENCL, 2, true, true, true, true, true, true, true);
        panel.add(kinect->params);
        hasKinect = true;
    }
    else{
        hasKinect = false;
    }
    
	shaderControls.setup("Shader Controls", "shaderControls.xml");

	sceneShaderPanelMode.setName("Scene shader mode");
	sceneShaderPanelMode.add(sceneShaderMode.set("Scene Mode", 0, 0, 10));
	shaderControls.add(sceneShaderPanelMode);

	sceneShaderPanelVert.setName("Scene vertex control");
	for (int i = 0; i < 6; i++)
	{
		sceneShaderPanelVert.add(floatsVertScene[i].set("Scene vert float " + ofToString(i + 1), 0.0, 0.0, 1.0));

	}
	shaderControls.add(sceneShaderPanelVert);

	sceneShaderPanelFrag.setName("Scene frag control");

	for (int i = 0; i < 6; i++)
	{
		sceneShaderPanelFrag.add(floatsFragScene[i].set("Scene frag float " + ofToString(i + 1), 0.0, 0.0, 1.0));

	}
	shaderControls.add(sceneShaderPanelFrag);


	texShaderPanelMode.setName("Tex shader mode");
	texShaderPanelMode.add(texShaderMode.set("Tex Mode", 0, 0, 10));
	shaderControls.add(texShaderPanelMode);

	texShaderPanelVert.setName("Tex vertex control");
	for (int i = 0; i < 6; i++)
	{
		texShaderPanelVert.add(floatsVertTexture[i].set("Tex vert float " + ofToString(i + 1), 0.0, 0.0, 1.0));

	}

	shaderControls.add(texShaderPanelVert);

	texShaderPanelFrag.setName("Tex frag control");

	for (int i = 0; i < 6; i++)
	{
		texShaderPanelFrag.add(floatsFragTexture[i].set("Tex frag float " + ofToString(i + 1), 0.0, 0.0, 1.0));

	}
	shaderControls.add(texShaderPanelFrag);

	shaderControls.setPosition(panel.getPosition().x + panel.getWidth() + 20, panel.getPosition().y);

    cam.setFarClip(100000);
    cam.setNearClip(0);

	cout << "listening for osc messages on port " << OSCPort << "\n";
	receiver.setup(OSCPort);

	finalShader.load("shaders/sceneShader.vert", "shaders/sceneShader.frag");
	texShader.load("shaders/texShader.vert", "shaders/texShader.frag");

	texShaderFbo.allocate(512, 424, GL_RGBA);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(hasKinect){
        kinect->update();

        if (kinect->isFrameNew())
        {
        texRGBRegistered.loadData(kinect->getRegisteredPixels());

		if (useTexShader)
		{
			texShaderFbo.begin();
			ofClear(0, 0, 0, 0);
			texShader.begin();
			texShader.setUniform1i("mode", texShaderMode);
			texShader.setUniform1f("time", ofGetElapsedTimef());
			for (int i = 0; i < 6; i++) {
				texShader.setUniform1f("FloatVert" + ofToString(i + 1), floatsVertTexture[i]);
			}

			for (int i = 0; i < 6; i++) {
				texShader.setUniform1f("FloatFrag" + ofToString(i + 1), floatsFragTexture[i]);
			}
			texRGBRegistered.draw(0, 0);
			texShader.end();
			texShaderFbo.end();
		}
        }
    }
    
	
	while (receiver.hasWaitingMessages()) {
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);

		if (m.getAddress() == "/texShaderMode") {
			texShaderMode = m.getArgAsInt(0);
		}

		if (m.getAddress() == "/sceneShaderMode") {
			sceneShaderMode = m.getArgAsInt(0);
		}
		for (int i = 0; i < 6; i++)
		{
			if (m.getAddress() == "/floatFragScene/"+ ofToString(i)) {
				floatsFragScene[i] = m.getArgAsFloat(0);
			}

			if (m.getAddress() == "/floatVertScene/" + ofToString(i)) {
				floatsVertScene[i] = m.getArgAsFloat(0);
			}

			if (m.getAddress() == "/floatFragtex/" + ofToString(i)) {
				floatsFragTexture[i] = m.getArgAsFloat(0);
			}

			if (m.getAddress() == "/floatVertTex/" + ofToString(i)) {
				floatsVertTexture[i] = m.getArgAsFloat(0);
			}
		}
		// check for mouse moved message
		
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableDepthTest();
    
    cam.begin();
    if (drawAxis) {
        ofDrawAxis(100);
    }
    
    ofPushMatrix();
    if (useSceneShader) {
        ofMatrix4x4 currentViewProjectionMatrix = cam.getModelViewProjectionMatrix();
		finalShader.begin();
		finalShader.setUniformMatrix4f("matrix", currentViewProjectionMatrix, 1);
		finalShader.setUniform1i("mode", sceneShaderMode);
		finalShader.setUniform1f("time", ofGetElapsedTimef());
		for (int i = 0; i < 6; i++) {
			finalShader.setUniform1f("FloatVert" + ofToString(i + 1), floatsVertScene[i]);
		}

		for (int i = 0; i < 6; i++) {
			finalShader.setUniform1f("FloatFrag" + ofToString(i + 1), floatsFragScene[i]);
		}
    }
    if (hasKinect) {
		if (useTexShader)
		{
			texShaderFbo.getTexture().bind();
		}
		else {
			texRGBRegistered.bind();
		}
        
        
        if (drawWireframe) {
            kinect->getPointCloud().drawWireframe();
        }
        else{
            kinect->getPointCloud().draw();
        }
		if (useTexShader)
		{
			texShaderFbo.getTexture().unbind();

		}
		else {
			texRGBRegistered.unbind();
		}
    }
    
    
    if (useSceneShader) {
		finalShader.end();
    }
    
    ofPopMatrix();
    
    cam.end();
    
    ofDisableDepthTest();
    
    panel.draw();
	shaderControls.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  
    
    if (key == 'l') {
		finalShader.load("shaders/sceneShader.vert", "shaders/sceneShader.frag");
		texShader.load("shaders/texShader.vert", "shaders/texShader.frag");
    }
    if (key == 'f'){
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
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
