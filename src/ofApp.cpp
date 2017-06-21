#include "ofApp.h"



using namespace ofxCv;
using namespace cv;



void ofApp::setup() {
	//glEnable(GL_DEPTH);

	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	faceFinder.setup("haarcascade_frontalface_default.xml"); //"haarcascade_frontalface_default.xml"
	faceFinder.setPreset(ObjectFinder::Fast);
	faceFinder.getTracker().setSmoothingRate(.3);
	//cam.setDeviceID(1);
	cam.setup(CAM_WIDTH, CAM_HEIGHT);
	apple.load("apple.png");
	arrow.load("arrow2.png");
	rock.load("rock.png");
	clock.load("clock.png");
	startButtonImage.load("start.png");
	gameOverImage.load("gameover.png");


	ofEnableAlphaBlending();

	//GUI

	gameOverSound.load("ring.wav");
	scoreSound.load("point.wav");
	rockHitSound.load("thump.wav"); //thud2
	arrowHitSound.load("hit.wav");
	arrowShotSound.load("arrowShot.wav");


	// setup HUD and fonts
	ofTrueTypeFont::setGlobalDpi(72);

	dunkinFont.load("dunkin.ttf", 30, true, true);
	dunkinFont.setLineHeight(34.0f);

	dunkinFontSmall.load("dunkin.ttf", 14, true, true);
	dunkinFontSmall.setLineHeight(18.0f);

}

void ofApp::update() {

	cam.update();

	gameTimer = gameLength - floor(ofGetElapsedTimef());
	appleRespawnTimer += 0.033f;
	arrowTimeCounter += 0.033f; // depends on your framerate
	rockTimeCounter += 0.033f;
	clockRespawnTimer += 0.33f;

	if (cam.isFrameNew())
	{
		faceFinder.update(cam);

	}


}

void ofApp::draw() {

	cam.draw(640,0,-640,480); //mirrored camera
	//cam.draw(0,0);

	if (gameTimer < 1)
	{	
		if(gameHasStarted)
			gameOverSound.play();

		gameHasStarted = false;
		drawGameOverMenu();
		
	}
	else if (gameHasStarted)
	{
		trackFaceDrawApple();
		drawArrow();
		drawRock();
		drawHUD();
		//drawClock();
	}
	else
	{
		drawStartMenu();
		ofResetElapsedTimeCounter();

	}
}

void ofApp::trackFaceDrawApple() {

	ofPushMatrix();

	for (int i = 0; i < faceFinder.size(); i++) {
		ofRectangle faceObject = faceFinder.getObjectSmoothed(i);
		//apple.setAnchorPercent(.5, 1.0);

		float scaleAmount = .3 * faceObject.width / apple.getWidth();
		//apple matrix and draw
		ofPushMatrix();
		ofTranslate(CAM_WIDTH-(faceObject.x + faceObject.width / 1.5), faceObject.y - faceObject.height * 0.4); // note that camera is mirrored horizontally
		ofScale(scaleAmount, scaleAmount);

		appleBounds[0] = CAM_WIDTH - (faceObject.x + faceObject.width / 1.5); //left bound, from translate
		appleBounds[1] = appleBounds[0] + apple.getWidth()*scaleAmount; //right bound
		appleBounds[2] = faceObject.y - faceObject.height * 0.35; //upper bound, , from translate
		appleBounds[3] = appleBounds[2] + apple.getHeight()*scaleAmount; //lower bound


		if (appleRespawnTimer > appleRespawnTime)
		{
			if (ofInRange(arrowPos.x, appleBounds[0], appleBounds[1]) && ofInRange(arrowTip, appleBounds[2], appleBounds[3]))
			{
				appleRespawnTimer = 0.0;

				score += 1;
				scoreSound.play();
				arrowHitSound.play();
				arrowHit = true;

			}
			else if(ofInRange(rockCenterPos.x, appleBounds[0] - float( rock.getWidth() * 0.02), appleBounds[1] + float(rock.getWidth() * 0.02)) && ofInRange(rockCenterPos.y, appleBounds[2] - float(rock.getHeight() * 0.01), appleBounds[3] + float(rock.getHeight() * 0.01)) )
			{
				appleRespawnTimer = 0.0;
				rockHitSound.play();
				score -= 1;
				arrowHit = false;

			}
			else
			{
				apple.draw(0, 0);
			}

		}
		else {
			if(arrowHit)
				ofTranslate(-appleRespawnTimer * 2000, appleRespawnTimer * 4000);
			else
				ofTranslate(0, appleRespawnTimer * 4000);

			
			apple.draw(0, 0);

		}

		ofPopMatrix();

		//face matrix and draw info
		/*ofPushMatrix();
		ofTranslate(faceObject.getPosition());
		ofDrawBitmapStringHighlight(ofToString(faceFinder.getLabel(i)), 0, 0);
		ofDrawLine(ofVec2f(), toOf(faceFinder.getVelocity(i)) * 10);
		ofPopMatrix();*/

	}

	ofPopMatrix();

}

void ofApp::drawStartMenu() {

	ofSetColor(0, 0, 0, 200);
	ofDrawRectangle(0, 0, CAM_WIDTH, CAM_HEIGHT);
	ofSetColor(255, 255, 255);

	//text
	dunkinFontSmall.drawString(" You have 60 seconds to collect as many points as possible. \n Collect points by shooting down the apples,\n but watch out for falling rocks! \n \n Press 'START' to play.", 0.05 *CAM_WIDTH, 0.7 *CAM_HEIGHT);
	
	//startbutton
	startButtonDraw();

}

void ofApp::drawGameOverMenu()
{

	ofSetColor(0, 0, 0, 200);
	ofDrawRectangle(0, 0, CAM_WIDTH, CAM_HEIGHT);
	ofSetColor(255, 255, 255);
	//game over 
	ofPushMatrix();
		ofTranslate(CAM_WIDTH/2, CAM_HEIGHT/6);
		gameOverImage.setAnchorPercent(0.5, 0.5);
		ofScale(0.5, 0.5, 0.5);
		gameOverImage.draw(0,0 );
	ofPopMatrix();

	//startbutton
	startButtonDraw();


	//text
	dunkinFont.drawString(" You got the score " + ofToString(score) + "! \n Press 'START' to play again." , 0.05 *CAM_WIDTH, 0.8 *CAM_HEIGHT);


}

void ofApp::startButtonDraw()
{
	startButtonImage.setAnchorPercent(0.5, 0.5);
	ofPushMatrix();
	ofTranslate(CAM_WIDTH / 2, CAM_HEIGHT / 2);
	startButtonCenter.x = CAM_WIDTH / 2;
	startButtonCenter.y = CAM_HEIGHT / 2;
	ofScale(0.2, 0.2, 0.2);
	startButtonRadius = startButtonImage.getHeight() / 2 * 0.2; // multiply with scalefactor

	startButtonImage.draw(0.0, 0.0);
	ofPopMatrix();
}

void ofApp::drawHUD() {

	ofSetColor(0, 0, 0, 200);
	ofDrawRectangle(HUDPosX, HUDPosY, CAM_WIDTH, CAM_HEIGHT / 8);

	//score text
	//ofSetColor(245, 58, 135);
	ofSetColor(245, 140, 0);
	dunkinFont.drawString("Score: " + ofToString(score), HUDPosX + 20, HUDPosY + 40);
	dunkinFont.drawString("CountDown: " + ofToString(gameTimer), HUDPosX + 300, HUDPosY + 40);


	ofSetColor(255, 255, 255); // reset color to white  




}

void ofApp::drawArrow() {

	if (arrowPos.x <= 0.0) 	//if arrow out of bounds, generate new position
	{
		arrowPos.x = float(CAM_WIDTH + 10);
		arrowPos.y = ofRandom(0, float(CAM_HEIGHT / 2)); // gen new random y position
														//arrowYPos = float(CAM_HEIGHT / 2);

		arrowTimeCounter = 0.0; // reset the time 
	}

	if (arrowTimeCounter == (0.033f * 5))
		arrowShotSound.play();

	//update Arrow position
	ofPushMatrix();
	arrowPos.x -= (arrowTimeCounter * arrowSpeed);
	//faceVelocity =  toOf(faceFinder.getVelocity(0));
	//arrowYPos += float(faceVelocity.y) * arrowSpeed/2;
	ofTranslate(arrowPos.x, arrowPos.y);

	//ofRotateZ(-arrowTimeCounter * 3);

	ofScale(0.2, 0.2);
	arrow.draw(0, 0);

	//arrowTipX = arrowXPos;
	arrowTip = arrowPos.y + arrow.getHeight() * 0.2 / 2.; // scale/2

	ofPopMatrix();
}

void ofApp::drawRock() {

	if (rockCenterPos.y >= CAM_HEIGHT) 	//if rock out of bounds, generate new position
	{
		rockCenterPos.y = -20.f;
		rockCenterPos.x = ofRandom(10, float(CAM_WIDTH)-10); // gen new random x position

		//rockRespawnTimer = 0.0; // reset the time 
		rockTimeCounter = 0.0;
	}

	//if (arrowTimeCounter == (0.033f * 5))
		//arrowShotSound.play();

	//update rock position
	ofPushMatrix();
	rock.setAnchorPercent(0.5, 0.5);
		rockCenterPos.y += (rockTimeCounter * arrowSpeed * 3);
		ofTranslate(rockCenterPos.x, rockCenterPos.y);
		ofScale(0.03, 0.03);
		rock.draw(0, 0);
	ofPopMatrix();


}

void ofApp::drawClock() {

	if (clockCenterPos.y >= CAM_HEIGHT && clockRespawnTimer > clockRespawnTime) 	//if rock out of bounds, generate new position
	{
		clockCenterPos.y = -20.f;
		clockCenterPos.x = ofRandom(20, float(CAM_WIDTH) - 20); // gen new random x position

															   //rockRespawnTimer = 0.0; // reset the time 
		clockRespawnTimer = 0.0;
	}

	//if (arrowTimeCounter == (0.033f * 5))
	//arrowShotSound.play();

	//update rock position
	ofPushMatrix();
		clock.setAnchorPercent(0.5, 0.5);
		clockCenterPos.y += (clockRespawnTimer * arrowSpeed);
		ofTranslate(clockCenterPos.x, clockCenterPos.y);
		ofScale(0.03, 0.03);
		clock.draw(0, 0);
	ofPopMatrix();


}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	
	if(ofInRange(float(x), startButtonCenter.x - startButtonRadius, startButtonCenter.x + startButtonRadius) && ofInRange(float(y), startButtonCenter.y - startButtonRadius, startButtonCenter.y + startButtonRadius))
	{
		cout << "START";
		arrowPos.x = float(CAM_WIDTH + 10);
		rockCenterPos.y = -20.;
		rockTimeCounter = 0.0;
		arrowTimeCounter = 0.0;

		gameHasStarted = true;
		score = 0;
		ofResetElapsedTimeCounter();


	}




}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
