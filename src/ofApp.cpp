#include "ofApp.h"
#include "math.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();

    sampleRate = 44100;
    audioBufferSize = 512;
    frequency = 440;
    ofSoundStreamSetup(2, 0, this, sampleRate, audioBufferSize, 4);

    ableton.setupLink(beatsPerMinute, 8.0);

    const float desiredWidth = ofApp::width;
    const float desiredHeight = ofApp::height;
    playModes.setup(ofxBenG::playmodes::c920, desiredWidth, desiredHeight, 60);
    ofSetWindowShape(playModes.getVideoWidth(), playModes.getVideoHeight());
    
    beatsPerMinute.addSubscriber([&]() { ableton.setBeatsPerMinute((int)beatsPerMinute); });

    propertyBag.add(δ(beatsPerMinute));
    propertyBag.add(δ(stutterTimes));
    propertyBag.add(δ(recordLengthBeats));
    propertyBag.add(δ(stutterLengthBeats));
    propertyBag.add(δ(stutterDelayBeats));
    propertyBag.loadFromXml();
    
    twister = new ofxBenG::twister();
    twister->bindToMultipleEncoders(&propertyBag);

    for (int i = 0; i < playModes.getBufferCount(); i++) {
        syphon[i].setName(ofToString(i));
    }
}

ofApp::ofApp() {}
ofApp::~ofApp() {
    propertyBag.saveToXml();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofxBenG::TimeDiff time = ofGetElapsedTimeMicros();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    propertyBag.update();
    playModes.update();

    float beat = ableton.getBeat();
    double intPart, fractionalPart;
    fractionalPart = modf(beat, &intPart);
    if (stutterScheduled && fractionalPart <= 0.05) {
        if (stutter != nullptr)
            deleteStutter(beat);
        createStutter(beat);
    }

    if (stutter != nullptr) {
        stutter->update(time, beat);
        if (stutter->isDone()) {
            deleteStutter(beat);
        }
    }
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels) {
    if (isPlayingTone) {
        for (int i = 0; i < bufferSize; i++) {
            currentSample = oscillator.sinewave(frequency);
            output[i * nChannels] = currentSample;
            output[i * nChannels + 1] = currentSample;
        }
    }
}

void ofApp::draw(){
    ofBackground(0);

    if (playModes.isInitialized()) {
        playModes.drawColumns(1);

        for (int i = 0; i < playModes.getBufferCount(); i++) {
            syphon[i].publishTexture(&playModes.getBufferTexture(i));
        }

        if (stutter != nullptr) {
            renderer.draw(0, 0, ofGetWidth(), ofGetHeight());
        }
    }

    if (!inFullscreen) {
        float y = 15;
        ofxBenG::utilities::drawLabelValue("beat", ableton.getBeat(), y);
        ofxBenG::utilities::drawLabelValue("bpm", ableton.getTempo(), y += 20);
        ofxBenG::utilities::drawLabelValue("stutterTimes", stutterTimes, y += 20);
        ofxBenG::utilities::drawLabelValue("recordLengthBeats", recordLengthBeats, y += 20);
        ofxBenG::utilities::drawLabelValue("stutterLengthBeats", stutterLengthBeats, y += 20);
        ofxBenG::utilities::drawLabelValue("stutterDelayBeats", stutterDelayBeats, y += 20);
    }
}

void ofApp::createStutter(float beat) {
    log(beat, "creating new stutter");
    stutter = new ofxBenG::stutter(beat, stutterDelayBeats, recordLengthBeats, stutterLengthBeats,
            beatsPerMinute, 60.0f, stutterTimes, &playModes.buffers[0], &playModes.rate,
            &isPlayingTone, &frequency);
    stutterScheduled = false;
    renderer.setup(*stutter->getHeader());
}

void ofApp::deleteStutter(float beat) {
    log(beat, "deleteStutter");
    delete stutter;
    stutter = nullptr;
}

void ofApp::log(float beat, const string &message) const {
    cout << beat << ": " << message << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'f') {
        ofToggleFullscreen();
        inFullscreen = !inFullscreen;
    }
    if (key == ' ') {
        stutterScheduled = true;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
