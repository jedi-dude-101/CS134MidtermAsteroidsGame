#include "ofMain.h"
#include "DynamicObject.cpp"

class Asteroid : public DynamicObject {
public:
	int vertices;
	bool child; // defines if it is a child, used when subdividing or not
    ofPolyline asteroidShape;
    
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
        
        return asteroid;
    }
    void setup(float radiuse) {
        
        torque = 0;
        angularVelocity = 0;
        radius = radiuse;
        asteroidShape = createRandomAsteroid(radius);

    }
	void draw() {
        ofPushMatrix();
        ofMultMatrix(getTransform());
        ofSetColor(color);
        asteroidShape.draw();
        ofPopMatrix();
	}
    
};