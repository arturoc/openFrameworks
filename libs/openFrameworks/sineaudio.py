
ofNoFill()
frameCycle = (ofGetFrameNum()%1200)/1200.
ofTranslate(ofGetWidth()/2,ofGetHeight()/2,0)
ofRotate(360*frameCycle,0,1,0)
ofTranslate(-ofGetWidth()/2,-ofGetHeight()/2,0)


for y in range(4):
    ofPushMatrix()
    ofTranslate(0,100*y,0)
    for i in range(ofGetWidth()):
        circleY = 200+200.*math.sin(i/float(ofGetWidth())*2*math.pi)
        posInBuffer = int(i/(ofGetWidth()/256.))*2+1
        sample = floatArray_getitem(buff,posInBuffer)
        ofPushMatrix()
        ofTranslate(i*4,circleY,0)
        ofRotate(90,0,1,0)
        ofTranslate(-i*4,-circleY,0)
        gray = int(ofMap(sample,-1,1,100,255))
        ofSetColor(gray,gray,gray);
        ofCircle(i*4,circleY,sample*100);
        ofPopMatrix()
    ofPopMatrix()
