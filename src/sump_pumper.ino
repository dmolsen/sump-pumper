/*
 * Sump Pumper - v1.0.0
 *
 * Copyright (c) 2015 Dave Olsen, http://dmolsen.com
 * Licensed under the MIT license
 *
 * Monitors a water sensor and publishes alerts if water is detected.
 * Meant to be used with IFTTT. IFTT will send SMS messages.
 *
 */

 // default vars. loopDelay is in milliseconds.
int waterSensor            = D0;
int loopDelay              = 5000;
int publishedLoopCount     = 0;
int publishedLoopMax       = 120;
boolean publishedHighWater = false;

void setup() {

  // enable the pin
  pinMode(waterSensor, INPUT);

  // take control of the main LED. show green by default
  RGB.control(true);
  RGB.color(0, 255, 0);

}

void loop() {

  // see if the water sensor has detected water
  if (isHighWater() == true) {

    if ((publishedHighWater == true) && (publishedLoopCount == publishedLoopMax)) {

      // the first alert was sent and we've cycled enough times for a reminder
      Spark.publish("highwater","reminder");
      publishedLoopCount = 0;

      // switch the LED color to blue to note that the reminder is being sent
      RGB.color(0,0,255);

      // delay a second
      delay(1000);

      // switch the LED color back to red
      RGB.color(255,0,0);

    } else if (publishedHighWater == true) {

      // the first alert was sent and we're waiting to send reminder. increment counter
      publishedLoopCount++;

    } else {

      // the water is high. send the first alert and set defaults
      Spark.publish("highwater","alert");
      publishedHighWater = true;
      publishedLoopCount = 0;

      // switch the LED to red
      RGB.color(255, 0, 0);

    }

  } else {

    // see if we're coming off of an alert
    if (publishedHighWater == true) {

      // send the all-clear message and reset defaults
      Spark.publish("highwater","clear");
      publishedHighWater = false;
      publishedLoopCount = 0;

      // switch the LED to green
      RGB.color(0, 255, 0);

    }

  }

  // pause the loop since we don't need to check a zillion times a second
  delay(loopDelay);

}

boolean isHighWater() {

  // check the water sensor on D0
  // LOW is in reference to the voltage the spark core is seeing. not water level.
  if (digitalRead(waterSensor) == LOW) {
    return true;
  }

  return false;

}
