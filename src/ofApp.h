#pragma once

#include "ofMain.h"
#include "ofxOscParameterSync.h"
#include "ofxArtnet.h"
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"


#define PLAYERPORT 6000
#define CONTROLERPORT 7000
//#define HOSTNAMEPLAYER "localhost"
#define HOSTNAMEPLAYER "Pierres-MacBook-Pro-2.local"

using namespace std;


class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
    
        ofVideoPlayer mPlayer;
    ofVec2f videoSize;
    unsigned long videoChannels;
    ofxDatGui* gui;
    float spacingSlider;
    ofxOscParameterSync sync;
    bool playVideo;
    bool showGUI;
    string nameVideo;
    string Node_IP;
    // artnet pixels
    unsigned int WindowWidth;
    unsigned int WindowHeight;
    ofxArtnetSender artnetNode_IP;
    
    void sendArtnet(ofPixels & pixels);
    void onButtonEvent(ofxDatGuiButtonEvent(e));
    void onSliderEvent(ofxDatGuiSliderEvent e);
    ofPixels dataToSend;
    unsigned int net;
    unsigned int subnet;
    unsigned int amntUniverses;
    unsigned int firstUniverses;
    unsigned int port;
    ofxXmlSettings XML;
    void loadVideo();
    string videoName;
    string message;



        

};
