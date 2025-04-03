#include "ofMain.h"
#include "DynamicObject.cpp"

class Asteroid : public DynamicObject {
public:
	int vertices;
	vector<Asteroid> children; // length = 4
	bool child;
    ofPolyline asteroidShape;
    Asteroid() {
        child = false;
    }
    Asteroid(bool isChild) {
        child = isChild;
    }
	// when shot, split into children and set child = true;
    ofPolyline createRandomAsteroid(float radius, int points = 5) {
        ofPolyline asteroid;
        // Create jagged vertices around a circle
        for (int i = 0; i < points; i++) {
            float angle = ofMap(i, 0, points, 0, TWO_PI);
            // Randomize radius variance (60% jitter)
            float r = radius * (0.4f + 0.6f * ofRandomuf());
            asteroid.addVertex(r * cos(angle), r * sin(angle));
        }
        asteroid.close(); // Connect last point to first
        //asteroid.draw();
        return asteroid;
    }
    void setup(float radius) {
        // cout << "asteroid setup"<<endl;
        angularVelocity = 0;
        // cout << "angular Velocity:" << angularVelocity << endl;
        asteroidShape = createRandomAsteroid(radius);

    }
	void draw() {
        
        // cout << "Torque:" << torque << endl;
        // cout << "angularVelocity:" << angularVelocity << endl;
        ofPushMatrix();
        // cout << position.x << " , " << position.y << endl;
        ofMultMatrix(getTransform());

        // Draw main ship
        ofSetColor(color);
        asteroidShape.draw();
        ofPopMatrix();
	}
    void subDivide() {
        for (int i = 0;i < 4;i++) {
            children[i] = new Asteroid(true);
        }
    }
};