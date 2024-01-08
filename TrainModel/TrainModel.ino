const int TimeDelay = 5000; // 5 seconds

const uint8_t ReedSwitch = 4;
Motor motor(5, 6, 7, 8);  // from Motor driver tab

//===================Multi tasking related======================
unsigned long prev_time_Sensor = millis();
long timeIntervalSensor = 600;

void setup() {
  pinMode(ReedSwitch, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long current_time = millis();
  static bool TrainShouldCoast = false; //slows down until comes to a complete halt
  static int speed = 0;

  if(!TrainShouldCoast)
    motor.drive(&speed);
  else{
    motor.brake(*speed);
    delay(TimeDelay);
    TrainShouldCoast = false;
  }
  // Detecting the presence of the magnet task; This task need to be constantly
  // being monitored
  if(current_time - prev_time_Sensor > timeIntervalSensor)
  {
    prev_time_Sensor = current_time;
    if(digitalRead(ReedSwitch))
      TrainShouldCoast = true;
  }
}
