
//--------------------------------------------------------------
//
//  CS235 - User Interface Design - Kevin M. Smith
//
//  Example of how to implement single selection of images which
//  are drag and dropped into viewport.
// 
//  


#include "ofApp.h"

Image::Image()
{
    trans.x = 0;
    trans.y = 0;
    scale.x = 1.0;
    scale.y = 1.0;
    rot = 0;
    bSelected = false;
}

void Image::draw(bool bSelectMode, int index) {
    ofPushMatrix();
    ofTranslate(trans);
    ofScale(scale);
    ofRotate(rot);
    
    //
    // if drawing image normally
    //
    if (!bSelectMode) {
        
        //
        // if selected, draw a light yellowo box around image
        //
        if (bSelected) {
            ofNoFill();
            ofSetColor(255, 255, 0);
            ofSetLineWidth(5);
            ofDrawRectangle(-image.getWidth() / 2.0, -image.getHeight() / 2.0,
                            image.getWidth(), image.getHeight());
        }
        ofSetColor(255, 255, 255, 255);
        image.draw(-image.getWidth() / 2.0, -image.getHeight() / 2.0);
        
    }
    //
    // else if drawing to test selection
    //
    else {
        ofFill();
        ofSetColor(index + 1, 0, 0);
        ofDrawRectangle(-image.getWidth() / 2.0, -image.getHeight() / 2.0,
                        image.getWidth(), image.getHeight());
    }
    ofPopMatrix();
}

void ofApp::setup(){
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();

	// this sets the camera's distance from the object
	cam.setDistance(1000);
	cam.disableMouseInput();
	cam.setFarClip(-100);

	//
	// set when <control> key is held down
	//
	selectedImage = NULL;
}

void ofApp::draw(){
	cam.begin();
	ofBackground(0);
	ofFill();

	for (int i = 0; i < images.size(); i++ ) {
		images[i]->draw();
	}
	cam.end();
}

void ofApp::exit() {
	for (int i = 0; i < images.size(); i++) {
		delete images[i];
	}
}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'S':
		saveImage();
		break;
	case OF_KEY_UP:
		moveUpSelectedImage();
		break;
	case OF_KEY_DOWN:
		moveDownSelectedImage();
		break;
	}
}

void ofApp::saveImage() {
	screen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	screen.save("project_01.png");
}

void ofApp::moveUpSelectedImage() {
	long imageIndex = find(images.begin(), images.end(), selectedImage) - images.begin();
    if ( imageIndex + 1 >= images.size()) {
        cout << "Invalid option: Already at top of Stack!!!";
        return;
    }
	images.erase(images.begin() + imageIndex);
	images.insert(images.begin() + imageIndex + 1, selectedImage);
}

void ofApp::moveDownSelectedImage() {
    long imageIndex = find(images.begin(), images.end(), selectedImage) - images.begin();
    if (imageIndex - 1 < 0) {
        cout << "Invalid option: Already at bottom of Stack!!!";
        return;
    }
    images.erase(images.begin() + imageIndex);
    images.insert(images.begin() + imageIndex - 1, selectedImage);
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {

	case OF_KEY_ALT:
		cam.disableMouseInput();
		break;
	}
}

void ofApp::mouseDragged(int x, int y, int button) {
	ofPoint mouse_cur = ofPoint(x, y);
	ofVec3f delta = mouse_cur - mouse_last;
	selectedImage->trans.x += delta.x;
	selectedImage->trans.y -= delta.y;
	mouse_last = mouse_cur;
}


void ofApp::mousePressed(int x, int y, int button){
	mouse_last = ofPoint(x, y);
	renderSelection();
	processSelection(x, y);
}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

	Image *imageObj = new Image();
	imageObj->trans = cam.screenToWorld(dragInfo.position);
	if (imageObj->image.load(dragInfo.files[0]) == true)
		images.push_back(imageObj);
	else {
		cout << "Can't load image: " << dragInfo.files[0] << endl;
		delete imageObj;
	}
}

//
// Render for the purposes of selection hit testing.  In this case
// we use the color method. We render echo object as a different
// value of r,g,b.  We then compare the pixel under the mouse. The
// value is the index into the image list;
//
void ofApp::renderSelection() {
	cam.begin();
	ofBackground(0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (int i = 0; i < images.size(); i++) {
		images[i]->draw(true, i);
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	cam.end();
}

Image* ofApp::processSelection(int x, int y) {
	unsigned char res[4];
	GLint viewport[4];

	// read pixel under mouse x y
	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

	if (selectedImage) {
		selectedImage->bSelected = false;
		selectedImage = NULL;
	}

	//  lookup image by color of pixel
	if (res[0] > 0 && res[0] <= images.size()) {
		Image *image = images[res[0] - 1];
		image->bSelected = true;
		selectedImage = image;		
	}

	return selectedImage;
}



