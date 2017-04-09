#include <PinChangeInt.h>
#include <PID_v1.h>
#include <Servo.h>
#include <PinChangeInt.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

// Assign your channel in pins
#define throttlePin 13
#define yawPin 12
#define pitchPin 11
#define rollPin 10

// These bit flags are set in bUpdateFlagsShared to indicate which
// channels have new signals
#define THROTTLE_FLAG 1
#define YAW_FLAG 2
#define PITCH_FLAG 3
#define ROLL_FLAG 4

//   create local variables to hold a local copies of the channel inputs
//   these are declared static so that thier values will be retained 
//   between calls to loop.
static uint16_t unThrottleIn;
static uint16_t unYawIn;
static uint16_t unPitchIn;
static uint16_t unRollIn;

int throttleVal;
int rollVal;
int pitchVal;
int yawVal;

// holds the update flags defined above
volatile uint8_t bUpdateFlagsShared;

// shared variables are updated by the ISR and read by loop.
// In loop we immediatley take local copies so that the ISR can keep ownership of the 
// shared ones. To access these in loop
// we first turn interrupts off with noInterrupts
// we take a copy to use in loop and the turn interrupts back on
// as quickly as possible, this ensures that we are always able to receive new signals
volatile uint16_t unThrottleInShared;
volatile uint16_t unYawInShared;
volatile uint16_t unPitchInShared;
volatile uint16_t unRollInShared;

// These are used to record the rising edge of a pulse in the calcInput functions
// They do not need to be volatile as they are only used in the ISR. If we wanted
// to refer to these in loop and the ISR then they would need to be declared volatile
uint32_t throttle;
uint32_t yaw;
uint32_t pitch;
uint32_t roll;

Servo motor0;
Servo motor1;
Servo motor2;
Servo motor3;

double pid_roll_in,   pid_roll_out,   pid_roll_setpoint = 0;
double pid_pitch_in,  pid_pitch_out,  pid_pitch_setpoint = 0;
double pid_yaw_in,    pid_yaw_out,    pid_yaw_setpoint = 0;

#define ROLL_PID_KP  0.15 // .25
#define ROLL_PID_KI  0//.95
#define ROLL_PID_KD  0//.011
#define ROLL_PID_MIN  -200.0
#define ROLL_PID_MAX  200.0

#define PITCH_PID_KP  .15 //.25
#define PITCH_PID_KI  .0 //.95
#define PITCH_PID_KD  .0//.011
#define PITCH_PID_MIN  -200.0
#define PITCH_PID_MAX  200.0

#define YAW_PID_KP  0.680
#define YAW_PID_KI  0.500
#define YAW_PID_KD  0.0001
#define YAW_PID_MIN  100.0
#define YAW_PID_MAX  100.0

#define THROTTLE_RMIN  1000
#define THROTTLE_SAFE_SHUTOFF 1120
#define THROTTLE_RMAX  1900
#define THROTTLE_RMID  1470

#define ROLL_RMIN  THROTTLE_RMIN
#define ROLL_RMAX  THROTTLE_RMAX
#define ROLL_WMIN  -30
#define ROLL_WMAX  30

#define PITCH_RMIN  THROTTLE_RMIN
#define PITCH_RMAX  THROTTLE_RMAX
#define PITCH_WMIN  -30
#define PITCH_WMAX  30

#define YAW_RMIN  THROTTLE_RMIN
#define YAW_RMAX  THROTTLE_RMAX
#define YAW_WMIN  -30
#define YAW_WMAX  30

#define MOTOR_ZERO_LEVEL  1000
#define MOTOR_ARM_START  1500
#define MOTOR_MAX_LEVEL  2000

float angleX,angleY,angleZ = 0.0;

int m0, m1, m2, m3;

void setup(){
  rcSetup();
  imuSetup();
  motorSetup();
  pid_initialize();
}

void loop(){
  rcIn();
  imuLoop();
  motorArm();
  control_update();
  constrain(m0,1000,2000);
  constrain(m1,1000,2000);
  constrain(m2,1000,2000);
  constrain(m3,1000,2000);
  Serial.println(m0);
}
