// ************************************************
//               Global Variables
// ************************************************

// These variables are accessible anywhere in this program, unlike
// local variables, which are only accessible inside the "scope"
// in which they are created. A "scope" is defined by curly brackets -> {}
// Functions start and end with curly brackets, but so do "if" statements,
// "for" loops, "while" loops, and several other more advanced control
// structures.

// The "const" before the "int" in these variable declarations means that
// they are "constant" variables. If you try to assign a new value to a
// constant variable elsewhere in the program, you will encounter an error
// when you compile your program. This keeps you from accidentally changing
// the value of a variable that should not be changed. Coding style convention
// dictates that constant variables should be written entirely in uppercase,
// which makes them easier to identify elsewhere in the program as being constant.

// Note that the value assigned to "SOUND" doesn't look like an integer. That's
// because "A0" is actually a constant integer variable that is created for you 
// inside the Arduino standard library (which is included in your program, even 
// though it doesn't appear as an #include statement at the top of your file)
const int SOUND = A0; // Input pin for the sound sensor
const int DIN   = 8;  // Input pin for the line sensor
const int LED   = 10; // Output pin for the LED

// ************************************************
//               Required Functions
// ************************************************

// setup() and loop() are the bare minimum functions you need
// to define an Arduino program (also known as a "sketch").

/*
 * Function: setup()
 * 
 * Description: This function runs once when the Arduino is
 * powered on or reset. This is where all the setup code that
 * should not be run over and over should go.
 */
void setup() {

  // Activate serial connection so we can print to the serial
  // monitor. You can use any of the baud rate settings that are 
  // available in the dropdown menu of the serial monitor window, 
  // but you'll find that 9600 is the most commonly used speed.
  Serial.begin(9600);

  // Set the pin directions using the variables established above
  pinMode(DIN, INPUT);
  pinMode(SOUND, INPUT);
  pinMode(LED, OUTPUT);
}


/*
 * Function: loop()
 * 
 * Description: This is the program's main loop. After the setup()
 * function has completed, this function runs over and over until
 * the Arduino is unplugged or reset.
 */
void loop() {

  // Run the function that checks the line sensor
  checkLineSensor();

  // Run the function that checks the sound sensor
  checkSoundSensor();

  // Wait 100 milliseconds before running the loop again so we don't have
  // so don't print to the serial monitor so much when we run the functions
  // above (which contain "Serial.print" statements) over and over in this loop
  delay(100);
}


// ************************************************
//                Other Functions
// ************************************************

/*
 * Function: checkLineSensor()
 * 
 * Description: Checks input from the line sensor, then prints a message
 * to the serial monitor if the sensor is active.
 */
void checkLineSensor() {
  
  // Read the line sensor. The "!" negates the boolean value of "digitalRead(DIN)"
  // This is because the line sensor gives a high signal on the digital pin when it
  // is off and a low signal when it is on, so we want to reverse those values so
  // data simply equals whether the line sensor is on or not.
  bool line_sensor_active = !digitalRead(DIN);

  // If the line sensor is active, trigger an alarm necessary
  if(line_sensor_active){
    Serial.println("Alarm has been triggered!");
  }
}

/*
 * Function: checkSoundLevel() 
 * 
 * Description: Checks input from the sound sensor, then prints to the
 * serial monitor if the value is above a certain threshold, and
 * also controls the brightness of an LED based upon the sound level.
 */
void checkSoundSensor() {

  // Because analog sensor readings tend to jump around quite a bit,
  // we need to take the average of several readings to get things to
  // smooth out a bit. We'll use a "for" loop to take 20 readings, each
  // time adding the new reading the variable "sound_level". After taking
  // all the readings, we'll divide "sound_level" by 20 to get the average.
  
  // Create an integer varaible with an initial value of 0
  int sound_level = 0;
  
  // int i = 0 -- Create a new counter variable for the loop
  // i < 20    -- Keep running the loop until "i" is no longer less than 20
  // i++       -- Add 1 to "i" at the end of each loop
  for(int i = 0; i < 20; i++){
    // This is equivalent to sound_level = sound_level + analogRead(SOUND)
    sound_level += analogRead(SOUND);
  }
  
  // This is equivalent to sound_level = sound_level / 20
  sound_level /= 20; // This is the average value of 20 readings of the sound sensor

  // Print some text to make our serial readout look nice. We use "Serial.print" here
  // instead of "Serial.println" because we don't want to move to a new line yet. We
  // want the "sound_level" value to be printed on the same line. We do, however, use
  // "Serial.println" for printing the value of "sound_level", since we want whatever
  // we print next to start on a new line.
  Serial.print("Sound level: ");
  Serial.println(sound_level);

  // We can set a threshold for the value read from the sound sensor and effectively use
  // it like a digital sensor, where something happens when we exceed that threshold, but
  // nothing happens below. In this case, we'll set the threshold value at 350 and print
  // a message when the sound sensor signal exceeds that. If you try to replicate this
  // example at home, your actual values may be very differnt, so check the serial monitor
  // to see what values you're recieving and adjust accordingly
  const int SOUND_THRESHOLD = 350;
  if(sound_level > SOUND_THRESHOLD){
    Serial.println("Sound sensor triggered!");
  }

  // We can also use the analog sensor reading to control an analog output. In this example,
  // we control the brightness of an LED based upon the input value of the sound sensor. In
  // order to do that, however, we need to use the "map" function to convert the values we
  // receive from the sound sensor to a range appropriate for the "analogWrite" function,
  // which can only use values from 0-255.

  // "map" Function Parameters:
  // 1) The source variable to be mapped 
  // 2) Minimum source value 
  // 3) Maximum source value
  // 4) Minimum target value 
  // 5) Maximum target value
  int brightness = map(sound_level, 300, 450, 0, 255);
  analogWrite(LED, brightness);
}

