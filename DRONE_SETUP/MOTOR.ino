void motorSetup(){
  motor0.attach(6);
  motor1.attach(7);
  motor2.attach(8);
  motor3.attach(9);
  
  motor0.writeMicroseconds(MOTOR_ZERO_LEVEL);
  motor1.writeMicroseconds(MOTOR_ZERO_LEVEL);
  motor2.writeMicroseconds(MOTOR_ZERO_LEVEL);
  motor3.writeMicroseconds(MOTOR_ZERO_LEVEL);
}

void motorArm(){
  motor0.writeMicroseconds(MOTOR_ZERO_LEVEL);
  motor1.writeMicroseconds(MOTOR_ZERO_LEVEL);
  motor2.writeMicroseconds(MOTOR_ZERO_LEVEL);
  motor3.writeMicroseconds(MOTOR_ZERO_LEVEL);
}

void update_motors(int m0, int m1, int m2, int m3){
  motor0.writeMicroseconds(m0);
  motor1.writeMicroseconds(m1);
  motor2.writeMicroseconds(m2);
  motor3.writeMicroseconds(m3);
}

