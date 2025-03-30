#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);

	// create an image for sprites being spawned by emitter
	//
	/*if (defaultImage.load("images/space-invaders-ship-scaled.png")) {
		imageLoaded = true;
	}
	else {
		cout << "Can't open image file" << endl;
		ofExit();
	}*/

	ofSetBackgroundColor(ofColor::black);

	
	
	gui.setup();
	gui.add(rate.setup("rate", 1, 1, 10));
	gui.add(life.setup("life", 5, .1, 10));
	gui.add(velocity.setup("velocity", ofVec3f(0, -200, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));
	gui.add(scale.setup("Scale", .1, .05, 1.0));
	gui.add(rotationSpeed.setup("Rotation Speed (deg/Frame)", 0, 0, 10));

	bHide = false;
	// starting position = middle of screen
	player.position = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
	player.createAsteroidsShip(30.0f); // 30px base size

}

//--------------------------------------------------------------
void ofApp::update() {
	

	// zander part 1
	
	// Zander part 2
	if (keymap[OF_KEY_UP]) { player.moveForward(); }
	else {
		player.mainEngine = false;
	}
	if (keymap[OF_KEY_DOWN]) { player.moveBackward(); }
	if (keymap[OF_KEY_LEFT]) { player.rotateLeft(); }
	if (keymap[OF_KEY_RIGHT]) { player.rotateRight(); }
	if (keymap[' ']) { player.fire(); }
	
	player.emitter->update();
	player.updatePlayer();

	
}


//--------------------------------------------------------------
void ofApp::draw(){

	
	//zander part 2
	// cout << "player.draw();" << endl;
	player.draw();
	
	
	
	if (!bHide) {
		gui.draw();
	}
}


//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//	cout << "mouse( " << x << "," << y << ")" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	/*if (bDrag) {
		player.pos = glm::vec3(x, y, 0) + delta;
	}*/
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;

	case 'H':
	case 'h':
		bHide = !bHide;
		break;
	}
	keymap[key] = true;
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
		
	}
	keymap[key] = false;
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

