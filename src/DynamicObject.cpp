
#include "ofMain.h"
class DynamicObject {
public:
	DynamicObject() {
		velocity.set(0, 0, 0);
		acceleration.set(0, 0, 0);
		// position.set(0, 0, 0);
		forces.set(0, 0, 0);
		lifespan = 5;
		birthtime = 0;
		radius = .1;
		damping = .99;
		mass = 1;
		color = ofColor::aquamarine;
	}
	ofVec3f position;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f forces;
	float	damping;
	float   mass;
	float   lifespan;
	float   radius;
	float   birthtime;
	ofColor color;
	

	glm::mat4 getTransform() {
		glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(position));
		glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
		glm::mat4 S = glm::scale(glm::mat4(1.0), scale);      // added this scale if you want to change size of object
		return T * R * S;
	}
	glm::vec3 scale = glm::vec3(1, 1, 1);
	float rotation = 0.0;

	void integrate() {
		// interval for this step
		
		float dt = 1.0 / ofGetFrameRate();
		dt = (dt > 0.0f && !std::isinf(dt)) ? dt : 1.0f / 60.0f;
		
		// update position based on velocity
		position += (velocity * dt);
		// wrap around when going off screen
		if (position.x > ofGetWindowWidth()) { position.x = 0; }
		if (position.x < -1) { position.x = ofGetWindowWidth(); }
		if (position.y > ofGetWindowHeight()) { position.y = 0; }
		if (position.y < -1) { position.y = ofGetWindowHeight(); }
		// update acceleration with accumulated paritcles forces
		// remember :  (f = ma) OR (a = 1/m * f)
		//
		ofVec3f accel = acceleration;    // start with any acceleration already on the particle
		accel += (forces * (1.0 / mass));
		velocity += accel * dt;

		// add a little damping for good measure
		//
		velocity *= damping;
		
		// clear forces on particle (they get re-added each step)
		//
		forces.set(0, 0, 0);
	}

	float angularVelocity;  // radians/sec
	float torque;           // rotational force
	float inertia = 1.0f;     // moment of inertia (adjust as needed)
	float angularDamping = 0.99f; // damping to slow rotation over time

	void integrateRotation() {
		float dt = 1.0 / ofGetFrameRate();
		dt = (dt > 0.0f && !std::isinf(dt)) ? dt : 1.0f / 60.0f;
		// Step 1: Update angular velocity with torque (a =  t / I)
		
		angularVelocity += torque * dt;

		// Step 2: Apply damping
		angularVelocity *= angularDamping;

		// Step 3: Update rotation angle
		rotation += angularVelocity * dt; 

		// Step 4: Clear torque for next frame
		torque = 0;
	}
	float age() {
		return (ofGetElapsedTimeMillis() - birthtime) / 1000.0;
	}
	void addForce(ofVec3f force) { forces += force; }
	bool collided(DynamicObject other) {
		float distance = position.distance(other.position);
		float minDistance = radius + other.radius;
		return distance < minDistance;
	}
};

