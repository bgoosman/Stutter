#include "ofApp.h"

void ofApp::setup() {
    sampleRate = 44100;
    audioBufferSize = 512;
    forwardSample.load(ofToDataPath("fancy-fort-audio-tour.wav"));
    backwardSample.load(ofToDataPath("fancy-fort-audio-tour-backwards.wav"));
    audio = new ofxBenG::audio(sampleRate, 2, audioBufferSize);
    ofSoundStreamSetup(2, 0, this, sampleRate, audioBufferSize, 4);

    ableton = new ofxBenG::ableton();
    ableton->setupLink(beatsPerMinute, 8.0);

    const float desiredWidth = ofApp::width;
    const float desiredHeight = ofApp::height;
    playModes = new ofxBenG::playmodes(ofxBenG::playmodes::c920, desiredWidth, desiredHeight, 60);
    ofSetWindowShape(playModes->getWidth(), playModes->getHeight());

    syphon = new ofxBenG::syphon(playModes->getBufferCount());

    beatsPerMinute.addSubscriber([&]() { ableton->setBeatsPerMinute((int)beatsPerMinute); });
    propertyBag.add(δ(beatsPerMinute));
    propertyBag.add(δ(stutterTimes));
    propertyBag.add(δ(recordLengthBeats));
    propertyBag.add(δ(stutterLengthBeats));
    propertyBag.add(δ(stutterDelayBeats));
    propertyBag.loadFromXml();

    twister = new ofxBenG::twister();
    twister->bindToMultipleEncoders(&propertyBag);

    timeline = new ofxBenG::timeline(0.0);
}

ofApp::ofApp() {}
ofApp::~ofApp() {
    ofSoundStreamClose();
    propertyBag.saveToXml();
    delete ableton;
    delete syphon;
    delete twister;
    delete timeline;
    delete audio;
}

void ofApp::update() {
    float const beat = ableton->getBeat();
    propertyBag.update();

    if (!playModes->isInitialized()) {
        playModes->setup();
    }

    if (playModes->isInitialized()) {
        playModes->update();
    }

    timeline->update(beat);
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels) {
    for (int i = 0; i < bufferSize; i++) {
        float mix = audio->getMix();
        output[nChannels * i] = mix;
        output[nChannels * i + 1] = mix;
    }
}

void ofApp::draw(){
    ofBackground(0);

    if (playModes->isInitialized()) {
        playModes->draw(0, 0, ofGetWidth(), ofGetHeight());
        for (int i = 0; i < playModes->getBufferCount(); i++) {
            syphon->publishTexture(i, &playModes->getBufferTexture(i));
        }
    }

    if (!inFullscreen) {
        float y = 15;
        ofxBenG::utilities::drawLabelValue("beat", ableton->getBeat(), y);
        ofxBenG::utilities::drawLabelValue("bpm", ableton->getTempo(), y += 20);
        ofxBenG::utilities::drawLabelValue("stutterTimes", stutterTimes, y += 20);
        ofxBenG::utilities::drawLabelValue("recordLengthBeats", recordLengthBeats, y += 20);
        ofxBenG::utilities::drawLabelValue("stutterLengthBeats", stutterLengthBeats, y += 20);
        ofxBenG::utilities::drawLabelValue("stutterDelayBeats", stutterDelayBeats, y += 20);
    }
}

void ofApp::log(float beat, const string &message) const {
    cout << beat << ": " << message << endl;
}

void ofApp::keyPressed(int key) {

}

void ofApp::keyReleased(int key) {
    float nextWholeBeat = ableton->getNextWholeBeat();

    if (key == 'f') {
        ofToggleFullscreen();
        inFullscreen = !inFullscreen;
    }

    if (key == 's') {
        auto stutter = ofxBenG::stutter::make_random(nextWholeBeat, beatsPerMinute, playModes, &forwardSample, audio);
        timeline->schedule(stutter, nextWholeBeat);
    }

    if (key == 'r') {
        auto rewind = ofxBenG::rewind::make_random(nextWholeBeat, beatsPerMinute, playModes, &forwardSample, &backwardSample, audio);
        timeline->schedule(rewind, nextWholeBeat);
    }

    if (key == ' ') {
        auto effectGenerator = new ofxBenG::effect_generator(nextWholeBeat, beatsPerMinute, playModes,
                &forwardSample, &backwardSample, audio);
        timeline->schedule(effectGenerator, nextWholeBeat);
    }
}

void ofApp::mouseMoved(int x, int y){

}

void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){

}

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
