
// Kevin M.Smith - CS 134 SJSU

#include "ParticleSystem.h"
#include "Player.cpp"


void ParticleSystem::add(const Particle &p) {
	particles.push_back(p);
}

void ParticleSystem::addForce(ParticleForce *f) {
	forces.push_back(f);
}

void ParticleSystem::remove(int i) {
	particles.erase(particles.begin() + i);
}

void ParticleSystem::setLifespan(float l) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].lifespan = l;
	}
}

void ParticleSystem::reset() {
	for (int i = 0; i < forces.size(); i++) {
		forces[i]->applied = false;
	}
}

void ParticleSystem::update() {
	// check if empty and just return
	if (particles.empty()) return;
	
	vector<Particle>::iterator p = particles.begin();
	vector<Particle>::iterator i = particles.begin();
	vector<Particle>::iterator tmp;

	// check which particles have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, we need to use an iterator.
	//
	while (p != particles.end()) {
		if (p->lifespan != -1 && p->age() > p->lifespan) {
			tmp = particles.erase(p);
			p = tmp;
		}
		else p++;
	}

	// update forces on all particles first 
	//
	for (int i = 0; i < particles.size(); i++) {
		for (int k = 0; k < forces.size(); k++) {
			if (!forces[k]->applied)
				forces[k]->updateForce( &particles[i] );
		}
	}

	// update all forces only applied once to "applied"
	// so they are not applied again.
	//
	for (int i = 0; i < forces.size(); i++) {
		if (forces[i]->applyOnce)
			forces[i]->applied = true;
	}

	// integrate all the particles in the store
	//
	for (int i = 0; i < particles.size(); i++)
		particles[i].integrate();

}


// zander modified

	void ParticleSystem::checkCollisions(Player & player) {
		
		if (particles.empty()) return;
		
		// Check asteroid-player and asteroid-bullet collisions
		for (auto p = particles.begin(); p != particles.end(); ) {
			bool p_erased = false;
			
			// 1. Asteroid vs Player collision
			if (p->asteroid.collided(player)) {
				cout << "player took damage" << endl;
				player.takeDamage(1);  // Handle player damage
			}

			// 2. Asteroid vs Bullets (from player's gun)
			if (player.gun && player.gun->sys) { // ensures player's gun initialized correctly
				auto& bullets = player.gun->sys->particles; // retrieve the bullets list
				for (auto j = bullets.begin(); j != bullets.end(); ) {

					if (p->asteroid.collided(j->asteroid)) { // if asteroid shot
						cout << "asteroid shot" << endl;
						if (p->asteroid.child) {
							p = particles.erase(p);
							p_erased = true;
							break;
						}
						else {
							// Split into 4 children
							for (int k = 0; k < 4; k++) {
								Particle child;
								child.asteroid.child = true;
								child.position = p->position;
								child.velocity = p->velocity * 0.5f;  // Inherit reduced velocity
								child.asteroid.setup(p->radius / 3.5f);
								particles.push_back(child);
							}
							p = particles.erase(p);
							p_erased = true;
							break;
						}
					}
					else {
						++j;
					}
				}
			}

			// 3. Asteroid vs Asteroid collisions (only if p wasn't erased)
			if (!p_erased) {
				for (auto i = next(p); i != particles.end(); ++i) {
					if (p->asteroid.collided(i->asteroid)) {
						// Simple elastic collision response
						ofVec2f collisionNormal = (p->position - i->position).normalized();
						float impulse = (p->velocity - i->velocity).dot(collisionNormal);

						// Apply impulse (swap velocities along collision normal)
						p->velocity -= impulse * collisionNormal;
						i->velocity += impulse * collisionNormal;

						// Optional: Add separation to prevent sticking
						float overlap = (p->asteroid.radius + i->asteroid.radius) -
							p->position.distance(i->position);
						if (overlap > 0) {
							ofVec2f separation = collisionNormal * overlap * 0.5f;
							p->position += separation;
							i->position -= separation;
						}
					}
				}
				++p;
			}
		}
	}

// remove all particlies within "dist" of point (not implemented as yet)
//
int ParticleSystem::removeNear(const ofVec3f & point, float dist) { return 0; }

//  draw the particle cloud
//
void ParticleSystem::draw() {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].draw();
	}
}


// Gravity Force Field 
//
GravityForce::GravityForce(const ofVec3f &g) {
	gravity = g;
}

void GravityForce::updateForce(Particle * particle) {
	//
	// f = mg
	//
	particle->forces += gravity * particle->mass;
}

// Turbulence Force Field 
//
TurbulenceForce::TurbulenceForce(const ofVec3f &min, const ofVec3f &max) {
	tmin = min;
	tmax = max;
}

void TurbulenceForce::updateForce(Particle * particle) {
	//
	// We are going to add a little "noise" to a particles
	// forces to achieve a more natual look to the motion
	//
	particle->forces.x += ofRandom(tmin.x, tmax.x);
	particle->forces.y += ofRandom(tmin.y, tmax.y);
	particle->forces.z += ofRandom(tmin.z, tmax.z);
}

// Impulse Radial Force - this is a "one shot" force that
// eminates radially outward in random directions.
//
ImpulseRadialForce::ImpulseRadialForce(float magnitude) {
	this->magnitude = magnitude;
	applyOnce = true;
}

void ImpulseRadialForce::updateForce(Particle * particle) {

	// we basically create a random direction for each particle
	// the force is only added once after it is triggered.
	//
	ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
	particle->forces += dir.getNormalized() * magnitude;
}