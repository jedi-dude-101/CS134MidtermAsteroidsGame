#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Shape.h"
#include "TransformObject.h"
#include "ParticleEmitter.h"
#include "DynamicObject.cpp"

class ImageShape : public Shape {
public:
	void set(ofImage image) { this->image = image; }

	void draw() {
		ofPushMatrix();
		ofMultMatrix(getTransform());
		image.draw(-image.getWidth() / 2.0, -image.getHeight() / 2.0);
		ofPopMatrix();

	}

	bool inside(glm::vec2 p) {
		glm::mat4 inv = glm::inverse(getTransform());
		glm::vec3 p2 = inv * glm::vec4(p, 0, 1);


		// cout << p2 << endl;

		return p2.x <= image.getWidth() / 2.0f && p2.x >= -image.getWidth() / 2.0f &&
			p2.y <= image.getHeight() / 2.0f && p2.y >= -image.getHeight() / 2.0f;
	}
	ofImage image;

};
// might not need this class
class TriangleShape : public Shape {
public:

	void draw() {
		// cout << "TriangleShape draw funcion" << endl;
		ofPushMatrix();
		ofMultMatrix(getTransform());

		/*cout << "p1:" << p1.x << p1.y << endl;
		cout << "P2:" << p2.x << p2.y << endl;*/
		ofDrawTriangle(p1, p2, p3);
		ofPopMatrix();
	}

	bool inside(glm::vec2 p) {
		glm::mat4 inv = glm::inverse(getTransform());
		glm::vec3 p2 = inv * glm::vec4(p, 0, 1);

		cout << p2 << endl;

		return isPointInsideTriangle(p2);
	}
	void setup() {
		float height = (sqrt(3) / 2.0f) * size;
		p1 = glm::vec3(0, -(2.0f / 3.0f) * height, 0); // Top vertex
		p2 = glm::vec3(-size / 2.0f, (1.0f / 3.0f) * height, 0); // Bottom-left vertex
		p3 = glm::vec3(size / 2.0f, (1.0f / 3.0f) * height, 0); // Bottom-right vertex
	}
	bool isPointInsideTriangle(const glm::vec2& point) {
		// Compute vectors
		glm::vec2 v0 = p3 - p1;
		glm::vec2 v1 = p2 - p1;
		glm::vec2 v2 = point - p1;

		// Compute dot products
		float dot00 = glm::dot(v0, v0);
		float dot01 = glm::dot(v0, v1);
		float dot02 = glm::dot(v0, v2);
		float dot11 = glm::dot(v1, v1);
		float dot12 = glm::dot(v1, v2);

		// Compute barycentric coordinates
		float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is inside triangle
		return (u >= 0) && (v >= 0) && (u + v <= 1);
	}
	float size = 50;
	glm::vec3 p1, p2, p3;
};

// Zander notes: 3-20-2025: 
// for asteroids use the following + circle with random diameters:
//ofPolyline polyline;
//polyline.addVertex(100, 100);
//polyline.addVertex(200, 150);
//polyline.addVertex(150, 250);
//polyline.close();
// does this work with a transform?

class Player : public DynamicObject {
public:
	bool mainEngine;
	/*bool rotateLeft;
	bool rotateRight;*/
	float thrust; // defines how powerful the thrust is
	float bulletSpeed; // defines how fast bullets travel
	float bulletDelay; // defines the minimum time between fire()
	float lastShot; // tracks the time since the last fire();
	ofPolyline ship;
	ParticleEmitter* emitter;

	Player() {
		emitter = new ParticleEmitter();
		
		
	}
	~Player() {
		delete emitter;
	}
	void createAsteroidsShip(float size = 1.0f) {
		// Classic Asteroids ship shape (4 points + closed)
		ship.addVertex(ofVec3f(0, -20,0));      // Nose (top)
		ship.addVertex(ofVec3f(-15, 15,0));     // Bottom-left
		ship.addVertex(ofVec3f(0, 5,0));        // Middle indent
		ship.addVertex(ofVec3f(15, 15,0));  // Bottom rear
		ship.close(); // Connect back to nose
		ship.draw();

		// setup emitter turrent / gun
		bulletSpeed = 500;
		bulletDelay = 1000; // milliseconds
		lastShot = 0; // initialize;
		emitter->setOneShot(true);
		emitter->setLifespan(50);
		emitter->setParticleRadius(3);
		
	}

	void draw() {
		// emitter->visible = false;
		emitter->draw();
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
		emitter->setP(position);
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
		cout << "firedin"<<endl;
		
		emitter->setVelocity(heading() * bulletSpeed);
		cout << "bulletDelay" << bulletDelay << endl;
		cout << "LastShot:" << lastShot << endl;
		if(bulletDelay < (ofGetElapsedTimeMillis()-lastShot) ){
			emitter->start();
			lastShot = ofGetElapsedTimeMillis();
		}
		
	}
	bool inside(glm::vec2 p) {
		
	}

};