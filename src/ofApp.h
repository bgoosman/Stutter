#pragma once

#include "ofMain.h"
#include "ofxBenG.h"
#include "ofxSyphon.h"
#include "ofxMaxim.h"
#include "ofxPS3EyeGrabber.h"
#include "ofxMaxim.h"

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
	void log(float beat, const string &message) const;
    void onEncoderPressed(int& encoder);
	void onEffectScheduled(int& totalEffectsScheduled);
	void loadSample(int index);
	struct sample {
		std::string forwards;
		std::string backwards;
	};
	std::vector<sample> samples;
    ofxBenG::ableton* ableton;
    ofxBenG::twister* twister;
    ofxBenG::playmodes* playModes;
	ofxBenG::syphon* syphon;
	ofxBenG::audio* audio;
	ofxBenG::timeline* timeline = nullptr;
    ofxBenG::property_bag propertyBag;
    ofxBenG::property<float> beatsPerMinute = {"beatsPerMinute", 60, 0, 480};
    ofxBenG::property<float> recordLengthBeats = {"recordLengthBeats", 0.25, 0.0, 8.0};
    ofxBenG::property<float> rewindLengthBeats = {"rewindLengthBeats", 0.0, 0.0, 8.0};
	ofxBenG::property<float> stutterLengthBeats = {"stutterLengthBeats", 0.25, 0.0, 8.0};
    static float constexpr width = 1280;
    static float constexpr height = width / (1920.0/1080.0);
    bool inFullscreen = false;
	ofxMaxiSample forwardSample;
	ofxMaxiSample backwardSample;
	int audioBufferSize, sampleRate;
};
