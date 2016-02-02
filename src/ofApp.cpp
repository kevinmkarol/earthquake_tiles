#include "ofApp.h"

#define GIF_DURATION 16
#define MAX_MAGNITUDE 6.7f

//--------------------------------------------------------------
void ofApp::setup(){
    //Warper
    ofBackground(50);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    
    //Gif
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
    
    
    //Read from csv file
    csv.loadFile(ofToDataPath("earthquakes_all_month.csv"));
    
    currentFrame = 0;
    currentRowIdx = 0;
    isSaving = false;
    pixelsReady = false;
   }


warpParameters ofApp::genearteWarpParameters(string filePath ,float mag){
  warpParameters params;

  ofImage img;
  img.loadImage(filePath);
  
  float height = img.getHeight();
  float width = img.getWidth();
  
  //In the event of the Max magnitude, the corners would meet
  float upperCornerY =  (height/10) * (mag/MAX_MAGNITUDE);
  float lowerCornerY =  height - ((height/10) * (mag/MAX_MAGNITUDE));
  
  
  params.startUL = ofPoint(0,0);
  params.startUR = ofPoint(width, upperCornerY);
  params.startDL = ofPoint(0, height);
  params.startDR = ofPoint(width, lowerCornerY);
  
  params.endUL = ofPoint(0,upperCornerY);
  params.endUR = ofPoint(width, 0);
  params.endDL = ofPoint(0, lowerCornerY);
  params.endDR = ofPoint(width, height);
  
  return params;

}

string ofApp::constructFilePath(int index){
  return "map_images/csv_" + std::to_string(index) + ".png";
}


void ofApp::createGifForImage(string filePath, int currentFrame, warpParameters params){

    img.loadImage(filePath);

    int x = 0;
    int y = 0;
    width = img.getWidth();
    height = img.getHeight();
    
    gifEncoder.setup(width, height, .25, 256);

    fbo.allocate(width, height);

    warper.setSourceRect(ofRectangle(0, 0, width, height));              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    warper.setTopLeftCornerPosition(ofPoint(x, y));             // this is position of the quad warp corners, centering the image on the screen.
    warper.setTopRightCornerPosition(ofPoint(x + width, y));        // this is position of the quad warp corners, centering the image on the screen.
    warper.setBottomLeftCornerPosition(ofPoint(x, y + height));      // this is position of the quad warp corners, centering the image on the screen.
    warper.setBottomRightCornerPosition(ofPoint(x + width, y + height)); // this is position of the quad warp corners, centering the image on the screen.
    warper.setup();
    warper.load(); // reload last saved changes.
    
    float diffPerFrame = params.endUL.y - params.startUL.y;
    float maxUpperY = params.endUL.y;
    float maxLowerY = params.endDL.y;
    
    
    if(currentFrame < GIF_DURATION/4){
        std::vector<ofPoint> warpPoints;
        float distance = currentFrame*diffPerFrame;

        int leftX = 0;       // center on screen.
        int rightX = width;
        int leftUpperY = distance;
        int rightUpperY = maxUpperY - distance;
        int leftLowerY = height - distance;
        int rightLowerY = maxLowerY + distance;
        
        warpPoints.push_back(ofPoint(leftX,leftUpperY));
        warpPoints.push_back(ofPoint(rightX,rightUpperY));
        warpPoints.push_back(ofPoint(rightX,rightLowerY));
        warpPoints.push_back(ofPoint(leftX,leftLowerY));
        
        warper.setSourcePoints(warpPoints);
        
    }else if(currentFrame < GIF_DURATION/2){
        std::vector<ofPoint> warpPoints;
        float distance = (GIF_DURATION/4 - (currentFrame - GIF_DURATION/4))*diffPerFrame;

        int leftX = 0;       // center on screen.
        int rightX = width;
        int leftUpperY = distance;
        int rightUpperY = maxUpperY - distance;
        int leftLowerY = height - distance;
        int rightLowerY = maxLowerY + distance;
        
        warpPoints.push_back(ofPoint(leftX,leftUpperY));
        warpPoints.push_back(ofPoint(rightX,rightUpperY));
        warpPoints.push_back(ofPoint(rightX,rightLowerY));
        warpPoints.push_back(ofPoint(leftX,leftLowerY));

        warper.setSourcePoints(warpPoints);
        
    }else if(currentFrame < (3*GIF_DURATION/4)){
        std::vector<ofPoint> warpPoints;
        float distance = (currentFrame - GIF_DURATION/2)*diffPerFrame;

        int leftX = 0;       // center on screen.
        int rightX = width;
        int leftUpperY = maxUpperY - distance;
        int rightUpperY = distance;
        int leftLowerY =  maxLowerY + distance;
        int rightLowerY = height - distance;
        
        warpPoints.push_back(ofPoint(leftX,leftUpperY));
        warpPoints.push_back(ofPoint(rightX,rightUpperY));
        warpPoints.push_back(ofPoint(rightX,rightLowerY));
        warpPoints.push_back(ofPoint(leftX,leftLowerY));
        
        warper.setSourcePoints(warpPoints);
        
    }else{
        std::vector<ofPoint> warpPoints;
        float distance = (GIF_DURATION/4 -(currentFrame - 3*GIF_DURATION/4))*diffPerFrame;

        int leftX = 0;       // center on screen.
        int rightX = width;
        int leftUpperY = maxUpperY - distance;
        int rightUpperY = distance;
        int leftLowerY =  maxLowerY + distance;
        int rightLowerY = height - distance;
        
        warpPoints.push_back(ofPoint(leftX,leftUpperY));
        warpPoints.push_back(ofPoint(rightX,rightUpperY));
        warpPoints.push_back(ofPoint(rightX,rightLowerY));
        warpPoints.push_back(ofPoint(leftX,leftLowerY));

        warper.setSourcePoints(warpPoints);
        
    }
    
    
    
    cout << "doing stuff" <<endl;
}

void ofApp::drawImage(){


}


//--------------------------------------------------------------
void ofApp::update(){
    if(currentFrame == 0 && !isSaving){
        std::string fileName = "gif_outputs/" + std::to_string(currentRowIdx) + ".gif";
        gifEncoder.save(fileName);
        currentRowIdx++;
        while(csv.data[currentRowIdx].size() < 5){
            currentRowIdx++;
        }
        isSaving = true;
        pixelsReady = false;
        currentFrame++;
    }
    
    if(!isSaving){
      double lat = ofToFloat(csv.data[currentRowIdx][1]);
      float lon = ofToFloat(csv.data[currentRowIdx][2]);
      float mag = ofToFloat(csv.data[currentRowIdx][4]);
      string filePath = constructFilePath(currentRowIdx);
    
      warpParameters params = genearteWarpParameters(filePath, mag);
      createGifForImage(filePath, currentFrame, params);


      currentFrame = (currentFrame + 1) % (GIF_DURATION);
      pixelsReady = true;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(!isSaving && pixelsReady){
        ofSetColor(255);
        
        //======================== draw image into fbo.
        
        fbo.begin();
        img.draw(0, 0);
        fbo.end();
        
        //======================== get our quad warp matrix.
        
        ofMatrix4x4 mat = warper.getMatrix();
        
        //======================== use the matrix to transform our fbo.
        
        ofPushMatrix();
        ofMultMatrix(mat);
        fbo.draw(0, 0);
        ofPopMatrix();
        
        
        captureFrame();
    }

    //======================== draw quad warp ui.
    
    /**ofSetColor(ofColor::magenta);
    warper.drawQuadOutline();
    
    ofSetColor(ofColor::yellow);
    warper.drawCorners();
    
    ofSetColor(ofColor::magenta);
    warper.drawHighlightedCorner();
    
    ofSetColor(ofColor::red);
    warper.drawSelectedCorner();**/
    
    
    
    
    //GIF STUFF
    /**for (int i = 0; i < nFrames; i ++) {
        txs[i]->draw(i* (frameW/2 + 5), frameH, frameW/2, frameH/2);
    }
    ofEnableAlphaBlending();
    ofDisableAlphaBlending();
	ofDrawBitmapString("KEYS\n----\nspacebar: capture frame\ns: save gif", frameW+10, 20);**/

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
 
}

void ofApp::keyReleased(int key){
    switch (key) {
        case ' ':
            captureFrame();
            break;
        case 's':
            cout <<"start saving\n" << endl;
            gifEncoder.save("test.gif");
            break;
        default:
            std::vector<ofPoint> test;

            int x = (ofGetWidth() - img.getWidth() ) * 0.5;       // center on screen.
            int y = (ofGetHeight() - img.getHeight()) * 0.5;     // center on screen.
            
            test.push_back(ofPoint(x,y));
            test.push_back(ofPoint(100,200));
            test.push_back(ofPoint(200,300));
            test.push_back(ofPoint(300,400));
            
            warper.setSourcePoints(test);
    }
}


void ofApp::onGifSaved(string &fileName) {
    gifEncoder.reset();
    isSaving = false;
    cout << "gif saved as " << fileName << endl;
}

void ofApp::captureFrame() {
    ofImage screen;

    screen.grabScreen(0, 0, width, height);
    //display->grabImage(screen);
    printf("%i", screen.getPixels().getBitsPerPixel());

    gifEncoder.addFrame(screen.getPixels(),
                        screen.getWidth(),
                        screen.getHeight(),
                        screen.getPixels().getBitsPerPixel(),
                        .1f
                        );
    
    nFrames++;
}

