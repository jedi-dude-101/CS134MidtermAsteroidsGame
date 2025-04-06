
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

	void ParticleSystem::checkCollisions(Player & player, SoundManager& sounds) {
		
		if (particles.empty()) return;
		
		// Check asteroid-player and asteroid-bullet collisions
		for (size_t i = 0; i < particles.size(); ) {
			Particle& p1 = particles[i];
			bool i_erased = false;
			
			// 1. Asteroid vs Player collision
			if (p1.asteroid.collided(player)) {
				sounds.play("Explosion");
				player.takeDamage(1);  // Handle player damage
			}

			// 2. Asteroid vs Bullets (from player's gun)
			if (player.gun && player.gun->sys) { // ensures player's gun initialized correctly
				auto& bullets = player.gun->sys->particles; // retrieve the bullets list
				for (size_t j = 0; j<bullets.size(); ) {

					if (p1.asteroid.collided(bullets[j].asteroid)) { // if asteroid shot
						sounds.play("Explosion");
						bullets.erase(bullets.begin() + j);
						if (!p1.asteroid.child) {
							for (int k = 0; k < 4; k++) {

								Particle child;
								child.lifespan = 10;
								child.birthtime = ofGetElapsedTimeMillis();
								float childRadius = p1.radius / 3.5f;
								child.radius = childRadius;
								child.asteroid.child = true;
								child.position = p1.position + ofVec3f(k * childRadius * 3.0f, k * childRadius * 3.0f, 0);
								child.velocity = (p1.velocity.length() * ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), 0)) * 1.5f;  // Inherit reduced velocity
								child.asteroid.setup(childRadius);
								particles.push_back(child);
							}

						}
						particles.erase(particles.begin() + i);
						i_erased = true;
						break;
					}
					else {
						++j; // next bullet
					}
				}
			}

			// 3. Asteroid vs Asteroid collisions (only if p wasn't erased)
			if (!i_erased) {
				for (size_t j = i + 1; j < particles.size(); ) {
					Particle& p2 = particles[j];
					bool j_erased = false;

					if (p1.asteroid.collided(p2.asteroid)) {
						// Elastic collision response
						ofVec2f collisionNormal = (p1.position - p2.position).normalized();
						float impulse = (p1.velocity - p2.velocity).dot(collisionNormal);

						p1.velocity -= impulse * collisionNormal * 0.8f; // 0.8 = elasticity
						p2.velocity += impulse * collisionNormal * 0.8f;

						// Positional correction to prevent sticking
						float overlap = (p1.radius + p2.radius) - p1.position.distance(p2.position);
						if (overlap > 0) {
							ofVec2f separation = collisionNormal * overlap * 0.5f;
							p1.position += separation;
							p2.position -= separation;
						}
					}
					j++;
				}
				i++;
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