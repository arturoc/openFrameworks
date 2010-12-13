

intFrameCycle = (ofGetFrameNum()%(30*8))/8.
frameCycle = (ofGetFrameNum()%600)/600.

for y in range(5):
    for x in range(6):
        if (y*6+x)==intFrameCycle:
            ofFill()
        else:
            ofNoFill()
        ofRect(340+105*x,390+50*y,110,50)
