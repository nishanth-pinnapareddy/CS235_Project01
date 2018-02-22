#pragma once

#include "ofMain.h"

class Image {

	public:
		Image();
		ofVec2f trans, scale;
		float	rot;
		bool	bSelected;
		ofImage image;
		void	draw(bool bSelectMode = false, int index = 0);
};



class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();
		void exit();
		void renderSelection();
		Image* processSelection(int x, int y);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void dragEvent(ofDragInfo dragInfo);
		void moveUpSelectedImage();
        void moveDownSelectedImage();
		void saveImage();

	private:
		ofEasyCam	cam;
		vector<Image *> images;
		Image *selectedImage;
		ofVec3f mouse_last;
		ofImage screen;
};
