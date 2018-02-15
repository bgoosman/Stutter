#pragma once

#include "ofMain.h"
#include "ofxBenG.h"
#include "ofxSyphon.h"
#include "ofxMaxim.h"
#include "ofxPS3EyeGrabber.h"

class ofApp : public ofBaseApp {

public:
    ofApp();~ofApp();
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void audioOut(float* output, int bufferSize, int nChannels);

	// App
    ofxBenG::ableton ableton;
    ofxBenG::osc osc = {"10.1.10.202", 1234};
    ofxBenG::twister* twister;
    ofxBenG::playmodes playModes;
    ofxSyphonServer syphon[ofxBenG::playmodes::bufferCount];
    ofxBenG::property_bag propertyBag;
    ofxBenG::property<float> beatsPerMinute = {"beatsPerMinute", 60, 0, 480};
    ofxBenG::property<int> stutterTimes = {"stutterTimes", 4, 1, 64};
    ofxBenG::property<float> recordLengthBeats = {"recordLengthBeats", 0.25, 0.025, 4};
    ofxBenG::property<float> stutterLengthBeats = {"stutterLengthBeats", 0.25, 0.025, 4};
    ofxBenG::property<float> stutterDelayBeats = {"stutterDelayBeats", 0.0, 0.0, 8.0};
	ofxBenG::stutter* stutter = nullptr;
    ofxPm::BasicVideoRenderer renderer;
    static float constexpr width = 1280;
    static float constexpr height = width / (1920.0/1080.0);
    bool inFullscreen = false;
    bool stutterScheduled = false;
    bool isPlayingTone = false;

    // Maximillian
    ofxMaxiOsc oscillator;
    float frequency, currentSample;
    int audioBufferSize, sampleRate;

    void log(float beat, const string &message) const;
    void deleteStutter(float beat);
    void createStutter(float beat);
};
