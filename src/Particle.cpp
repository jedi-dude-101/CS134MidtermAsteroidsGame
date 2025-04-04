#include "Particle.h"


Particle::Particle() {

	// initialize particle with some reasonable values first;
	//
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	position.set(0, 0, 0);
	forces.set(0, 0, 0);
	lifespan = 5;
	birthtime = 0;
	radius = .1;
	damping = .99;
	mass = 1;
	color = ofColor::aquamarine;
	
}

void Particle::draw() {
	ofSetColor(color);
	// Zander Modified
	if (isAsteroid) {
		asteroid.position = position;
		asteroid.radius = radius;
		asteroid.draw();
	}
	else {
		// ofSetColor(ofMap(age(), 0, lifespan, 255, 10), 0, 0);
		ofDrawSphere(position, radius);
	}
}

// write your own integrator here.. (hint: it's only 3 lines of code)
//
void Particle::integrate() {

	
	// interval for this step
	//
	float dt = 1.0 / ofGetFrameRate();
	dt = (dt > 0.0f && !std::isinf(dt)) ? dt : 1.0f / 60.0f; // Zander: ensures dt is not infinity
	// update position based on velocity
	//
	position += (velocity * dt);

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

	// Zander:
	
	if (isAsteroid) { 
		asteroid.integrateRotation(); 
	}
}

//  return age in seconds
//
float Particle::age() {
	return (ofGetElapsedTimeMillis() - birthtime)/1000.0;
}


