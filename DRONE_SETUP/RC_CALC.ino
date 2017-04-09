// simple interrupt service routine
void calcThrottle()
{
  // if the pin is high, its a rising edge of the signal pulse, so lets record its value
  if(digitalRead(throttlePin) == HIGH){ 
    throttle = micros();
  }
  else{
    // else it must be a falling edge, so lets get the time and subtract the time of the rising edge
    // this gives use the time between the rising and falling edges i.e. the pulse duration.
    unThrottleInShared = (uint16_t)(micros() - throttle);
    // use set the throttle flag to indicate that a new throttle signal has been received
    bUpdateFlagsShared |= THROTTLE_FLAG;
  }
}

void calcYaw()
{
  if(digitalRead(yawPin) == HIGH){ 
    yaw = micros();
    }
  else{
    unYawInShared = (uint16_t)(micros() - yaw);
    bUpdateFlagsShared |= YAW_FLAG;
  }
}

void calcPitch()
{
  if(digitalRead(pitchPin) == HIGH){ 
    pitch = micros();
  }
  else{
    unPitchInShared = (uint16_t)(micros() - pitch);
    bUpdateFlagsShared |= PITCH_FLAG;
  }
}

void calcRoll()
{
  if(digitalRead(rollPin) == HIGH){ 
    roll = micros();
  }
  else{
    unRollInShared = (uint16_t)(micros() - roll);
    bUpdateFlagsShared |= ROLL_FLAG;
  }
}
