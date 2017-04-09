void imuLoop(){
    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize)
    {
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
  
    // get Current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ( (mpuIntStatus & 0x10) || fifoCount == 1024)
    {
      mpu.resetFIFO();        // reset so we can continue cleanly
    } 
  
    else if (mpuIntStatus & 0x02)
    {     
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();   // wait for correct available data length, should be a VERY short wait
      mpu.getFIFOBytes(fifoBuffer, packetSize);                        // read a packet from FIFO

      // track FIFO count here in case there is > 1 packet available
      // (this lets us immediately read more without waiting for an interrupt)
      fifoCount -= packetSize;

      /*===== display Euler ANGLES in DEGREES ======*/
      mpu.dmpGetQuaternion      (&q, fifoBuffer);
      mpu.dmpGetGravity         (&gravity, &q);
      mpu.dmpGetYawPitchRoll    (ypr, &q, &gravity);    
      ypr[1] = ypr[1] * 180/M_PI;  // Pitch
      ypr[2] = ypr[2] * 180/M_PI;  // Roll  

      
      angleX = ypr[1];
      angleY = ypr[2];
      angleZ = ypr[0];

  
}
}
