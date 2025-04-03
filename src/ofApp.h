#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Shape.h"
#include "Player.cpp"



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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		// Zander function
		ofVec3f getRandomEdgePosition(float padding = 0.0f);
		ofVec3f getVectorToCenter(ofVec3f startPos);
		ParticleEmitter asteroidEmitter;
		Player player = Player();

		ofImage defaultImage;
		ofVec3f mouse_last;
		bool imageLoaded;

		// Some basic UI
		//
		bool bHide;
		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxVec3Slider velocity;
		ofxLabel screenSize;
		ofxFloatSlider scale;
		ofxFloatSlider rotationSpeed = 3.0;

		map<int, bool> keymap;
		ofxPanel gui;
};
