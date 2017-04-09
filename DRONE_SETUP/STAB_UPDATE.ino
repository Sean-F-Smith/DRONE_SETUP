void control_update(){
  throttle=map(throttleVal,THROTTLE_RMIN,THROTTLE_RMAX,MOTOR_ZERO_LEVEL,MOTOR_MAX_LEVEL);
  
  setpoint_update();
  pid_update();
  pid_compute();
  
  // yaw control disabled for stabilization testing...
  m0 = throttleVal + pid_pitch_out ;//+ pid_yaw_out;
  m1 = throttleVal + pid_roll_out ;//- pid_yaw_out;
  m2 = throttleVal - pid_pitch_out ;//+ pid_yaw_out;
  m3 = throttleVal - pid_roll_out ;//- pid_yaw_out;
  
  #ifdef SAFE
    if(throttleVal < THROTTLE_SAFE_SHUTOFF)
    {
      m0 = m1 = m2 = m3 = MOTOR_ZERO_LEVEL;
    }
  #endif
  
  update_motors(m0, m1, m2, m3);
}

void setpoint_update() {
  // here we allow +- 20 for noise and sensitivity on the RX controls...
  // ROLL rx at mid level?
  if(rollVal > THROTTLE_RMID - 20 && rollVal < THROTTLE_RMID + 20)
    pid_roll_setpoint = 0;
  else
    pid_roll_setpoint = map(rollVal,ROLL_RMIN,ROLL_RMAX,ROLL_WMIN,ROLL_WMAX);
  //PITCH rx at mid level?
  if(pitchVal > THROTTLE_RMID - 20 && pitchVal < THROTTLE_RMID + 20)
    pid_pitch_setpoint = 0;
  else
    pid_pitch_setpoint = map(pitchVal,PITCH_RMIN,PITCH_RMAX,PITCH_WMIN,PITCH_WMAX);
  //YAW rx at mid level?
  if(yawVal > THROTTLE_RMID - 20 && yawVal < THROTTLE_RMID + 20)
    pid_yaw_setpoint = 0;
  else
    pid_yaw_setpoint = map(yawVal,YAW_RMIN,YAW_RMAX,YAW_WMIN,YAW_WMAX);
}
