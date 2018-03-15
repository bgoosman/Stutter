#include "ofApp.h"
#include "math.h"

void ofApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();

    sampleRate = 44100;
    audioBufferSize = 512;
    frequency = 440;
    ofxMaxiSettings::setup(sampleRate, 2, audioBufferSize);
    mySample.load(ofToDataPath("new dreams tonite forward.wav"));
    backwardSample.load(ofToDataPath("new dreams tonite backward.wav"));
    currentSample = &mySample;

    audio = new ofxBenG::audio();
    ofSoundStreamSetup(2, 0, this, sampleRate, audioBufferSize, 4);

    ableton.setupLink(beatsPerMinute, 8.0);

    const float desiredWidth = ofApp::width;
    const float desiredHeight = ofApp::height;
    playModes = new ofxBenG::playmodes(ofxBenG::playmodes::c920, desiredWidth, desiredHeight, 60);
    ofSetWindowShape(playModes->getWidth(), playModes->getHeight());
    renderer.setup(playModes->getHeader(0));

    beatsPerMinute.addSubscriber([&]() { ableton.setBeatsPerMinute((int)beatsPerMinute); });

    propertyBag.add(δ(beatsPerMinute));
    propertyBag.add(δ(stutterTimes));
    propertyBag.add(δ(recordLengthBeats));
    propertyBag.add(δ(stutterLengthBeats));
    propertyBag.add(δ(stutterDelayBeats));
    propertyBag.loadFromXml();

    twister = new ofxBenG::twister();
    twister->bindToMultipleEncoders(&propertyBag);

    syphon = new ofxBenG::syphon(playModes->getBufferCount());
}

ofApp::ofApp() {}
ofApp::~ofApp() {
    propertyBag.saveToXml();
}

void ofApp::update() {
    float const beat = ableton.getBeat();
    propertyBag.update();

    if (!playModes->isInitialized()) {
        playModes->setup();
    }

    if (playModes->isInitialized()) {
        playModes->update();

        if (ofxBenG::utilities::closeToInteger(beat)) {
            if (effectGeneratorScheduled) {
                effectGeneratorScheduled = false;
                effectGenerator = (ofxBenG::beat_action *) new ofxBenG::effect_generator(
                        beat + 5, beatsPerMinute,
                        60.0f, &playModes->getBuffer(0), &playModes->getRate(), &renderer,
                        &mySample, &backwardSample, audio);
                effectGenerator->start(beat);
            }

            if (stutterScheduled) {
                if (stutter != nullptr)
                    deleteStutter(beat);
                createStutter(beat);
                stutterScheduled = false;
            }

            if (reverseScheduled) {
                if (reverse != nullptr)
                    deleteReverse(beat);
                createReverse(beat);
                reverseScheduled = false;
            }
        }

        if (effectGenerator != nullptr) {
            effectGenerator->update(beat);
        }

        if (stutter != nullptr) {
            stutter->update(beat);
            if (stutter->isDone(beat)) {
                deleteStutter(beat);
            }
        }

        if (reverse != nullptr) {
            reverse->update(beat);
            if (reverse->isDone(beat)) {
                deleteReverse(beat);
            }
        }
    }
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
        renderer.draw(0, 0, ofGetWidth(), ofGetHeight());
        for (int i = 0; i < playModes->getBufferCount(); i++) {
            syphon->publishTexture(i, &playModes->getBufferTexture(i));
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
//    log(beat, "creating new stutter");
//    ofxBenG::stutter* s = new ofxBenG::stutter(
//            beat,
//            recordLengthBeats,
//            stutterLengthBeats,
//            beatsPerMinute,
//            60.0f,
//            (int)stutterTimes,
//            &playModes.buffers[0],
//            &playModes.rate,
//            &mySample,
//            &isPlayingTone,
//            &frequency);
//    stutterScheduled = false;
//    renderer.setup(*s->getHeader());
//    stutter = (ofxBenG::beat_action*) s;
//    stutter->start(beat);
}

void ofApp::deleteStutter(float beat) {
    log(beat, "deleteStutter");
    delete stutter;
    stutter = nullptr;
}

void ofApp::createReverse(float beat) {
//    log(beat, "creating new reverse");
//    ofxBenG::reverse* r = new ofxBenG::reverse(
//            beat,
//            recordLengthBeats,
//            stutterLengthBeats,
//            beatsPerMinute,
//            60.0f,
//            &playModes.buffers[0],
//            &playModes.rate,
//            &mySample,
//            &backwardSample,
//            [&](ofxMaxiSample* s) {this->setCurrentSample(s);},
//            &isPlayingTone,
//            &frequency);
//    renderer.setup(*r->getHeader());
//    reverse = (ofxBenG::beat_action*) r;
//    reverse->start(beat);
}

void ofApp::deleteReverse(float beat) {
    log(beat, "deleteReverse");
    delete reverse;
    reverse = nullptr;
}

void ofApp::log(float beat, const string &message) const {
    cout << beat << ": " << message << endl;
}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){
    if (key == 'f') {
        ofToggleFullscreen();
        inFullscreen = !inFullscreen;
    }

    if (key == 's') {
        stutterScheduled = true;
    }

    if (key == 'r') {
        reverseScheduled = true;
    }

    if (key == ' ') {
        effectGeneratorScheduled = true;
        timeStarted = ofGetElapsedTimef();
    }
}

void ofApp::mouseMoved(int x, int y ){

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
