

#include "matrixManipulator.h"




void matrixManipulator::setup(std::string name, bool global) {
	isGlobal = global;

	params.setName(name);

	params.add(useNode.set("Use ofNode", false));

	params.add(translationCoarseVec.set("Pre translation", glm::vec3(0, 0, 0), glm::vec3(-2500.0, -2500.0, -2500.0), glm::vec3(2500.0, 2500.0, 2500.0)));
	translationCoarseVec.addListener(this, &matrixManipulator::translationCoarseVecChanged);

	params.add(rotationCoarseX.set("Local rot Coarse X", 0, -360.0, 360.0));
	rotationCoarseX.addListener(this, &matrixManipulator::rotationCoarseXChanged);

	params.add(rotationFineX.set("Local rot Fine X", 0, -5.0, 5.0));
	rotationFineX.addListener(this, &matrixManipulator::rotationFineXChanged);


	params.add(rotationCoarseY.set("Local rot Coarse Y", 0, -360.0, 360.0));
	rotationCoarseY.addListener(this, &matrixManipulator::rotationCoarseYChanged);

	params.add(rotationFineY.set("Local rot Fine Y", 0, -5.0, 5.0));
	rotationFineY.addListener(this, &matrixManipulator::rotationFineYChanged);

	params.add(rotationCoarseZ.set("Local rot Coarse Z", 0, -360.0, 360.0));
	rotationCoarseZ.addListener(this, &matrixManipulator::rotationCoarseZChanged);

	params.add(rotationFineZ.set("Local rot Fine Z", 0, -5.0, 5.0));
	rotationFineZ.addListener(this, &matrixManipulator::rotationFineZChanged);

	params.add(translationFineVec.set("post translation", glm::vec3(0, 0, 0), glm::vec3(-600.0, -600.0, -600.0), glm::vec3(600.0, 600.0, 600.0)));
	translationFineVec.addListener(this, &matrixManipulator::translationFineVecChanged);

	cameraNode.setParent(parentNode);
	parentNode.setParent(masterNode);
}

void matrixManipulator::setGlobalMatrix(ofMatrix4x4 inMat)
{
	
		globalMatrix = inMat;
		calcuateDrawMatrix();
	
}


void matrixManipulator::translationCoarseVecChanged(glm::vec3 & _translationCoarseVec) {
	translationMatrix.setTranslation((translationCoarseVec.get() + translationFineVec.get()));

	//this->setPosition(translationCoarseVec.get() + translationFineVec.get());

	calcuateDrawMatrix();
}

void matrixManipulator::translationFineVecChanged(glm::vec3 & _translationFineVec) {
	translationMatrix.setTranslation((translationCoarseVec.get() + translationFineVec.get()));

	//this->setPosition(translationCoarseVec.get() + translationFineVec.get());

	calcuateDrawMatrix();
}

void matrixManipulator::rotationCoarseXChanged(float & _rotationCoarseX) {
	
	qRx.makeRotate(rotationCoarseX + rotationFineX, 1, 0, 0);
	rotationXMatrix.setRotate(qRx);

	calcuateDrawMatrix();
}

void matrixManipulator::rotationCoarseYChanged(float & _rotationCoarseY) {
	
	qRy.makeRotate(rotationCoarseY + rotationFineY, 0, 1, 0);
	rotationYMatrix.setRotate(qRy);

	calcuateDrawMatrix();
}

void matrixManipulator::rotationCoarseZChanged(float & _rotationCoarseZ) {
	
	qRz.makeRotate(rotationCoarseZ + rotationFineZ, 0, 0, 1);
	rotationZMatrix.setRotate(qRz);

	calcuateDrawMatrix();
}

void matrixManipulator::rotationFineXChanged(float & _rotationFineX) {
	
	qRx.makeRotate(rotationCoarseX + rotationFineX, 1, 0, 0);
	rotationXMatrix.setRotate(qRx);
	calcuateDrawMatrix();
}

void matrixManipulator::rotationFineYChanged(float & _rotationFineY) {
	
	qRy.makeRotate(rotationCoarseY + rotationFineY, 0, 1, 0);
	rotationYMatrix.setRotate(qRy);
	
	calcuateDrawMatrix();
}

void matrixManipulator::rotationFineZChanged(float & _rotationFineZ) {
	
	qRz.makeRotate(rotationCoarseZ + rotationFineZ, 0, 0, 1);
	rotationZMatrix.setRotate(qRz);

	calcuateDrawMatrix();
}

ofMatrix4x4 matrixManipulator::getDrawMatrix() {
	return drawMatrix;
}

void matrixManipulator::calcuateDrawMatrix() {
	if (isGlobal)
	{
		drawMatrix = translationMatrix * rotationYMatrix * rotationXMatrix * rotationZMatrix  ;

	}
	else {
		drawMatrix = translationMatrix *rotationYMatrix * rotationXMatrix * rotationZMatrix  * globalMatrix;

	}
	if (useNode)
	{
		
		//cameraNode.resetTransform();
		cameraNode.setPosition(translationCoarseVec.get());


		parentNode.resetTransform();
		parentNode.rotate(rotationCoarseZ + rotationFineZ, glm::vec3(0, 0, 1));
		parentNode.rotate(rotationCoarseX + rotationFineX, glm::vec3(1, 0, 0));
		parentNode.rotate(rotationCoarseY + rotationFineY, glm::vec3(0, 1, 0));
		
		masterNode.setPosition(translationFineVec.get());
		


		//ofQuaternion finalRot = qRx * qRy * qRz;
		//this->setOrientation(finalRot);

		//this->setOrientation(glm::vec3(rotationCoarseX + rotationFineX, rotationCoarseY + rotationFineY, rotationCoarseZ + rotationFineZ));

		
		drawMatrix = cameraNode.getGlobalTransformMatrix();
		drawNode.setOrientation(cameraNode.getGlobalOrientation());
		drawNode.setPosition(cameraNode.getGlobalPosition());
	}

}

ofNode matrixManipulator::getNode()
{
	
	return drawNode;
}


