
#pragma once

#include "ofMain.h"
#include "ofxGui.h"


class matrixManipulator {
public:
    
	ofNode cameraNode;
	ofNode parentNode;
	ofNode masterNode;
	ofNode drawNode;
    ofParameterGroup params;
    ofParameter <glm::vec3> translationCoarseVec;
    ofParameter <glm::vec3> translationFineVec;
    ofParameter <float> rotationCoarseX, rotationCoarseY, rotationCoarseZ, rotationFineX, rotationFineY, rotationFineZ;
	ofParameter <bool> useNode;
    
    void translationCoarseVecChanged(glm::vec3 & _translationCoarseVec);
    void translationFineVecChanged(glm::vec3 & _translationFineVec);
    void rotationCoarseXChanged(float & _rotationX);
    void rotationCoarseYChanged(float & _rotationY);
    void rotationCoarseZChanged(float & _rotationZ);
    void rotationFineXChanged(float & _rotationX);
    void rotationFineYChanged(float & _rotationY);
    void rotationFineZChanged(float & _rotationZ);
  
 
    ofMatrix4x4 rotationXMatrix;
    ofMatrix4x4 rotationYMatrix;
    ofMatrix4x4 rotationZMatrix;
    ofMatrix4x4 drawMatrix;
    ofMatrix4x4 translationMatrix;
    ofMatrix4x4 globalMatrix;

	ofQuaternion qRx, qRy, qRz;

	bool isGlobal;
    void setup(std::string name, bool global);
	void setGlobalMatrix(ofMatrix4x4 inMat);
    ofMatrix4x4 getDrawMatrix();
    void calcuateDrawMatrix();
	ofNode getNode();
    
};
