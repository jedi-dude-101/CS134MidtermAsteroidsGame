#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Shape.h"
#include "TransformObject.h"
#include "ParticleEmitter.h"
#include "DynamicObject.cpp"

class Player : public DynamicObject {
public:
	bool mainEngine; // flag for drawing thrust
	float thrust; // defines how powerful the thrust is
	float bulletSpeed; // defines how fast bullets travel
	float bulletDelay; // defines the minimum time between fire()
	float lastShot; // tracks the time since the last fire();
	int health; // health points or lives
	ofPolyline ship;
	ParticleEmitter* gun;

	Player() {
		gun = new ParticleEmitter();
		
		
	}
	~Player() {
		delete gun;
	}
	void createAsteroidsShip(float size = 1.0f) {
		// Classic Asteroids ship shape (4 points + closed)
		ship.addVertex(ofVec3f(0, -20,0));      // Nose (top)
		ship.addVertex(ofVec3f(-15, 15,0));     // Bottom-left
		ship.addVertex(ofVec3f(0, 5,0));        // Middle indent
		ship.addVertex(ofVec3f(15, 15,0));  // Bottom rear
		ship.close(); // Connect back to nose
		ship.draw();
		radius = 21;

		// setup emitter turrent / gun
		bulletSpeed = 500;
		bulletDelay = 1000; // milliseconds
		lastShot = 0; // initialize;
		gun->setOneShot(true);
		gun->setLifespan(50);
		gun->setParticleRadius(3);
		gun->setAsteroid(true);
		
	}

	void draw() {
		
		gun->draw();
		// Apply transform
		ofPushMatrix();
		// cout << position.x << " , " << position.y << endl;
		ofMultMatrix(getTransform());

		// Draw main ship
		ofSetColor(color);
		ship.draw();

		// Draw cockpit line (classic Asteroids detail)
		ofSetLineWidth(2);
		ofDrawLine(0, -10, 0, 0);

		// Draw thrust flame when accelerating
		if (mainEngine) {
			ofSetColor(255, 100, 0); // Orange flame
			ofBeginShape();
			ofVertex(-8, 15);
			ofVertex(0, 25);
			ofVertex(8, 15);
			ofEndShape();
		}

		ofPopMatrix();
	}
	void updatePlayer() {
		integrate();
		integrateRotation();
		gun->setP(position);
	}
	
	glm::vec3 heading() {
		glm::mat4 rot1 = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
		return glm::normalize(rot1 * glm::vec4(glm::vec3(0, -1, 0), 1));
	}
	void moveForward() {
		// use: r(t) = o + dt;
		thrust = 1000;
		addForce(heading()*thrust);
		mainEngine = true;
	}

	void moveBackward() {
		thrust = 1000;
		addForce(-heading() * thrust);
	}

	void rotateRight() {
		torque = 500;
	}

	void rotateLeft() {
		torque = -500;
	}
	void fire() {
		gun->setVelocity(heading() * bulletSpeed); // aim
		if(bulletDelay < (ofGetElapsedTimeMillis()-lastShot) ){
			cout << "gun fired";
			gun->start(); // pew
			lastShot = ofGetElapsedTimeMillis();
		}
		
	}
	void takeDamage(int damagePoints) {
		health -= damagePoints;
		if(health<=0){}// game over
	}

};