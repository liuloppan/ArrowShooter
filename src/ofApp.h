#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{

	public:
		//----variables----//

		int CAM_WIDTH = 640;
		int CAM_HEIGHT = 480;
		float HUDPosX = 0.0;
		float HUDPosY = 7 * CAM_HEIGHT / 8;

		//drawing objects
		ofVideoGrabber cam;
		ofxCv::ObjectFinder faceFinder;

		//gui
		//ofxPanel gui;
		//ofxButton startButton;
		//ofxToggle filled;

		//ofParameter<bool> filled;
		ofTrueTypeFont	dunkinFont;
		ofTrueTypeFont	dunkinFontSmall;


		//Gameplay
		int score = 0;
		float gameTimer;
		float gameLength = 30;
		bool gameHasStarted = false;
		bool arrowHit = false;

		ofImage arrow;
		ofImage apple;
		ofImage rock;
		ofImage clock;
		ofImage startButtonImage;
		ofImage gameOverImage;

		ofVec2f startButtonCenter;
		float startButtonRadius;

		// arrow variables;
		float arrowTimeCounter = 0.0;
		ofVec2f arrowPos = ofVec2f(0.0, 650.0);
		float arrowSpeed = 4.0f;
		float arrowTip; // the y pos of the arrow tip

		//apple variables
		float appleBounds[4]; //left, upper, right, lower
		float appleRespawnTime = 1.0f;
		float appleRespawnTimer = 0.0;

		//rock variables
		ofVec2f rockCenterPos = ofVec2f(0.0, 0.0);
		float rockTimeCounter = 0.0;

		//clock variables
		ofVec2f clockCenterPos = ofVec2f(0.0, 0.0);
		float clockRespawnTime = 2.0f;
		float clockRespawnTimer = 0.0;
		float clockTimer = 0.0;
		float clockRadius;



		//sounds
		//ofSoundPlayer startSound;
		ofSoundPlayer gameOverSound;

		ofSoundPlayer scoreSound;
		ofSoundPlayer rockHitSound;
		ofSoundPlayer arrowHitSound;
		ofSoundPlayer arrowShotSound;

		//----functions----//

		//general
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		//other
		void drawArrow();
		void drawHUD();
		void trackFaceDrawApple();
		void drawStartMenu();
		void drawGameOverMenu();
		void startButtonDraw();
		void drawRock();
		void drawClock();


		
};
