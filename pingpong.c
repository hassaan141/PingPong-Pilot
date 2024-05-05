/****************************************************************************
***
By: Muhammad Farooqi Filippo Di Franco, Chavi Sharma, Rhitikha
Description: Code for ping pong robot
Version: 4
Help Recived: TA's, Ryan Consell
27
*****************************************************************************
**/
//Here we have a function to configure all the sensors, so we dont clutter up
our main.
void configureSensors()
{
SensorType[S1]= sensorEV3_Ultrasonic;
SensorType[S2] = sensorEV3_Gyro;
wait1Msec(50);
SensorMode[S2] = modeEV3Gyro_Calibration;
wait1Msec (100);
SensorMode[S2]= modeEV3Gyro_RateAndAngle;
wait1Msec(50);
return;
}
//Here we have a function that plays start down music, for loop for repeating
the same tone
void startCountdownMusic()
{
// Play "5" tone
for(int i = 0; i <3; i++)
{
playTone(200, 75);
wait1Msec(1200);
}
// Play "1" tone
playTone(500, 75);
wait1Msec(1200);
return;
}
28
//Here we have our shoot ball function which takes in motor power. It first
flicks it at 100% power, and after flicking, it goes back to its
//original position using motor Encoder. Goes back slowly so less room for
error
void shootBall(int motor_pow)
{
const int SLOW_SPEED_MOTOR=10;
nMotorEncoder[motorA] = 0;
motor[motorA] = motor_pow;
while(nMotorEncoder[motorA] < 180)
{}
motor[motorA] = 0;
wait1Msec(1000);
motor[motorA] = SLOW_SPEED_MOTOR;
while(nMotorEncoder[motorA] < 360)
{}
motor[motorA] = 0;
}
//This functions rotates the bot, by taking in motor power and angle, if the
angle is positive, then it moves robot to the left
//if it is negative, it goes to the right, angles are changed depending on
which mode is selected. Uses gyro to move it either positve 30 or negative
30.
void rotateBot(int motor_pow, float angle)
{
if (angle > 0)
{
motor[motorD] = -1*motor_pow;
motor[motorB] = motor_pow;
29
displayString(2,"yay") ;
while(getGyroDegrees(S4) < angle)
{
displayString(4,"%d", getGyroDegrees(S4)) ;
}
motor[motorD] = motor[motorB]=0;
}
else if (angle < 0)
{
motor[motorD] = motor_pow;
motor[motorB] = -1*motor_pow;
displayString(2,"boo") ;
while(getGyroDegrees(S4) > angle)
{
displayString(4,"%d", getGyroDegrees(S4)) ;
}
motor[motorD] = motor[motorB]=0;
}
}
//This functions checks the ultrasonic value, if it is less than 5.5, it
means that there is a ball in the feed so it calls the shoot and rotate
funcgtion.
//There is also an if statement that checks if help timer is needed, if it is
needed, then it plays the countdown for easy mode, to notify the new playersa
void checkUltrasonic(int motor_pow_shoot, int motor_pow_rotate, int angle,
int waitTime, int helpTimer)
{
while(SensorValue[S1] < 5.5)
{
if (helpTimer ==1 )
30
startCountdownMusic();
shootBall(motor_pow_shoot);
wait1Msec(500);
rotateBot(motor_pow_rotate, angle);
angle*=-1;
}
return;
}
//This is called when the user selects medium mode, it passes the appropiate
parameters
void mediumMode()
{
const int SHOOT_POW = 100;
const int ROTATE_POW = 20;
int ROTATE_ANGLE = -15;
const int WAIT_TIME = 1500;
const int HELPTIMER=1;
checkUltrasonic(SHOOT_POW, ROTATE_POW, ROTATE_ANGLE, WAIT_TIME, HELPTIMER);
return;
}
//This is called when the user selects hard mode, it passes the appropiate
parameters
void hardMode()
{
const int SHOOT_POW = 100;
const int ROTATE_POW = 20;
int ROTATE_ANGLE = -15;
const int WAIT_TIME = 1500;
const int HELPTIMER=0;
31
startCountdownMusic();
checkUltrasonic(SHOOT_POW, ROTATE_POW, ROTATE_ANGLE, WAIT_TIME, HELPTIMER);
return;
}
//This function is for mode selection, depending on what mode the user wants,
they press the appropiate button to play that mode
void modes()
{
displayString(4, "Select the level of difficulty");
displayString(6, "Select Left Button for Easy");
displayString(7, "Select Down Button for Medium");
displayString(8, "Select Right Button for Hard");
while(!getButtonPress(ANY_BUTTON))
{}
if (getButtonPress(buttonLeft))
{
checkUltrasonic(100, 0, 0, 1000, 1);
}
else if (getButtonPress(buttonDown))
{
mediumMode();
}
else if (getButtonPress(buttonRight))
{
hardMode();
}
32
}
//This is the startup function that is played when the robot is first
started, it waits for the enter button to be pressed so it can proceed.
//If enter button isnt pressed in 10 seconds, the robot automatically skips
the mode selection and goes straight to the ending.
int startup()
{
 displayString(6, "Welcome to PING PONG PILOT");
 displayString(8, "Press Enter to start");
 time1[T1] = 0; // Initialize the timer
 while (!getButtonPress(ENTER_BUTTON))
 {
 if (time1[T1] > 10000)
 {
 displayString(10, "Time limit reached. Returning 0.");
 return 0; // Return 0 if the time limit is exceeded
 }
 wait1Msec(10); // Add a short delay to avoid continuous checking
 }
 while (getButtonPress(ENTER_BUTTON))
 {}
 return 1;
}
//This is the endling function which plays the ending music and shuts down
the whole prgram when and give s a good message when its done
void ending()
33
{
int notes[] = { 900, 800, 700, 600, 500,900, 800, 700, 600, 1200};
int durations[] = { 25, 25, 25, 25, 50, 25, 25, 25, 25, 70};
for (int i = 0; i < sizeof(notes) / sizeof(notes[0]); i++)
{
playTone(notes[i], durations[i]);
wait1Msec(durations[i] + 50);
}
motor[motorB] = motor[motorD] = motor[motorA] = 0;
displayString (7, "Good Job. You are improving!");
wait1Msec(5000);
}
//Our main is where we call our 3 main functions start, modes and ending. If
a value of 1 isnt returned from startup, this means that the button wasnt
pressed and a value, of 0 is returned
//This means that we skip the modes function and go straight to the ending
function, shutting down the program
task main ()
{
configureSensors();
wait1Msec(20);
int readyToPlay = startup();
if (readyToPlay ==1)
{
modes();
}
34
eraseDisplay();
ending();
}