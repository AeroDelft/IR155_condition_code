const int pwmPin = 3; //Replace pin number with actual pin on the Arduino

void setup() {
  Serial.begin(9600); //Set 9600 baud for serial communication to the computer
  pinMode(pwmPin, INPUT); //Set the selected pin as the PWM input
}

void loop() {
  //Initialize previous frequency to an out-of-range value
  static float prevFrequency = -1.0;
  
  //Measure pulse width when the signal is a digital HIGH (above 0.6Vin approx. 3V)
  unsigned long pulseOnTime = pulseIn(pwmPin, HIGH);
  
  //Measure the pulse width when the signal is a digital LOW (below 0.6Vin)
  unsigned long pulseOffTime = pulseIn(pwmPin, LOW);
  
  //Calculate frequency and duty cycle of PWM signal
  unsigned long period = pulseOnTime + pulseOffTime;
  float frequency = 1000000.0 / period; // Frequency in Hz because the Arduino measures in microseconds 1s = 10^6 μs
  float dutyCycle = (pulseOnTime * 100.0) / period;
  
  //Calculate the insulation resistance in kOhms
  float resistanceFloating = ((90.0 * 1200.0)/(dutyCycle - 5.0));
  
  if (frequency != prevFrequency) {
    Serial.print("Frequency (Hz): ");
    Serial.println(frequency);
    Serial.print("Duty Cycle (%): ");
    Serial.println(dutyCycle);
    
    // Condition 1: Normal operation (10 Hz)
    if (abs(frequency - 10) <= (0.1*10)) {
      if (dutyCycle >= 4.5 && dutyCycle <= 95.5) {
        Serial.println("Normal condition");
        Serial.print("Insulation resistance (kOhm): ");
        Serial.println(resistanceFloating);
      }
      else {
        Serial.println("Ouside working range");
      }
    }
    // Condition 2: Undervoltage detected (20 Hz)
    else if (abs(frequency - 20) <= (0.1*20)) {
      if (dutyCycle >= 4.5 && dutyCycle <= 95.5) {
        Serial.println("Undervoltage detected");
        Serial.print("Insulation resistance (kOhm): ");
        Serial.println(resistanceFloating);
      }
      else {
        Serial.println("Outisde working PWM duty cycle range");
      }
    }
    //Condition 3: Speed start measurement (30 Hz)
    else if (abs(frequency - 30) <= (0.1*30)) {
      if (dutyCycle >= 4.5 && dutyCycle <= 10.5) {
        Serial.println("Good");
        Serial.print("Insulation resistance (kOhm): ");
        Serial.println(resistanceFloating);
      }
      else if (dutyCycle >= 89.5 && dutyCycle <= 95.5) {
        Serial.println("Bad");
        Serial.print("Insulation resistance (kOhm): ");
        Serial.println(resistanceFloating);
      }
      else {
        Serial.println("Outisde working PWM duty cycle range");
      }
    }
    //Condition 4: Device error (40 Hz)
    else if (abs(frequency - 40) <= (0.1*40)) {
      if (dutyCycle >= 47.0 && dutyCycle <= 53.0) {
        Serial.println("Device error detected");
        Serial.print("Insulation resistance (kOhm): ");
        Serial.println(resistanceFloating);
      }
      else {
        Serial.println("Outside working PWM duty cycle range");
      }
    }
    //Condition 5: Connection fault earth (50 Hz)
    else if (abs(frequency - 50) <= (0.1*50)) {
      if (dutyCycle >= 47.0 && dutyCycle <= 53.0) {
        Serial.println("Connection fault earth");
      }
      else {
        Serial.println("Outside working PWM duty cycle range");
      }
    }
    }
}
