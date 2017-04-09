void rcSetup(){
    Serial.begin(9600);

  // using the PinChangeInt library, attach the interrupts
  // used to read the channels
  PCintPort::attachInterrupt(throttlePin, calcThrottle,CHANGE); 
  PCintPort::attachInterrupt(yawPin, calcYaw,CHANGE); 
  PCintPort::attachInterrupt(pitchPin, calcPitch,CHANGE);
  PCintPort::attachInterrupt(rollPin, calcRoll,CHANGE); 
}
