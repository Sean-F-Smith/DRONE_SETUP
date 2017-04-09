void rcIn(){
//   local copy of update flags
  static uint8_t bUpdateFlags;

  // check shared update flags to see if any channels have a new signal
  if(bUpdateFlagsShared)
  {
    noInterrupts(); // turn interrupts off quickly while we take local copies of the shared variables

    // take a local copy of which channels were updated in case we need to use this in the rest of loop
    bUpdateFlags = bUpdateFlagsShared;
    
    // in the current code, the shared values are always populated
    // so we could copy them without testing the flags
    // however in the future this could change, so lets
    // only copy when the flags tell us we can.
    
    if(bUpdateFlags & THROTTLE_FLAG){
      unThrottleIn = unThrottleInShared;
    }
    
    if(bUpdateFlags & YAW_FLAG){
      unYawIn = unYawInShared;
    }
    
    if(bUpdateFlags & PITCH_FLAG){
      unPitchIn = unPitchInShared;
    }
    
    if(bUpdateFlags & ROLL_FLAG){
      unRollIn = unRollInShared;
    }
     
    // clear shared copy of updated flags as we have already taken the updates
    // we still have a local copy if we need to use it in bUpdateFlags
    bUpdateFlagsShared = 0;
    
    interrupts(); // we have local copies of the inputs, so now we can turn interrupts back on
    // as soon as interrupts are back on, we can no longer use the shared copies, the interrupt
    // service routines own these and could update them at any time. During the update, the 
    // shared copies may contain junk. Luckily we have our local copies to work with :-)
  }
  
  bUpdateFlags = 0;

  throttleVal = unThrottleIn;
  rollVal = unRollIn;
  pitchVal = unPitchIn;
  yawVal = unYawIn;
  
}
