#pragma once

#include "ofMain.h"
#include "ofxQuadWarp.h"
#include "ofxGifEncoder.h"
#include "ofxCsv.h"

typedef struct{
  ofPoint startUL;
  ofPoint endUL;
  
  ofPoint startUR;
  ofPoint endUR;
  
  ofPoint startDL;
  ofPoint endDL;
  
  ofPoint startDR;
  ofPoint endDR;

} warpParameters;


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
    
        string constructFilePath(int index);
        warpParameters genearteWarpParameters(string filePath, float mag);
        void createGifForImage(string filePath, int currentFrame, warpParameters params);
        void drawImage();


        int currentFrame;
        int currentRowIdx;
        bool isSaving;
        bool pixelsReady;

        //Reading from CSV
        wng::ofxCsv csv;
        
        //Warper
        ofxQuadWarp warper;
        
        ofImage img;
        ofFbo fbo;
    
        //Gif Encoder
        void onGifSaved(string & fileName);
        //void exit();
        void captureFrame();
    
        vector <ofTexture *> txs; // for previewing
        vector <ofxGifFrame *> pxs;
        
        int frameW, frameH;
        int nFrames;
        
        ofxGifEncoder gifEncoder;

        int width;
        int height;
		
};
