#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
#define LED 2

// Variables for calibration
int minR, maxR;
int minG, maxG;
int minB, maxB;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(LED, OUTPUT);

  // Set sensor scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  digitalWrite(LED, HIGH); // Turn on the sensor's white LED for consistent lighting

  Serial.begin(9600);
  Serial.println("Starting calibration... Point sensor at a white surface.");
  delay(5000); // Give user time to place the sensor
  
  calibrateWhite();

  Serial.println("Calibration complete. Point sensor at colors to get readings.");
  delay(1000);
}

void loop() {
  int redFrequency = getColorFrequency(LOW, LOW);
  int greenFrequency = getColorFrequency(HIGH, HIGH);
  int blueFrequency = getColorFrequency(LOW, HIGH);

  // ✅ Fixed mapping (min→max mapped to 255→0)
  int redValue = constrain(map(redFrequency, minR, maxR, 255, 0), 0, 255);
  int greenValue = constrain(map(greenFrequency, minG, maxG, 255, 0), 0, 255);
  int blueValue = constrain(map(blueFrequency, minB, maxB, 255, 0), 0, 255);

  Serial.print("R: ");
  Serial.print(redValue);
  Serial.print(" G: ");
  Serial.print(greenValue);
  Serial.print(" B: ");
  Serial.println(blueValue);
  
  delay(500);
}

// Function to get frequency for a specific color filter
int getColorFrequency(int S2_value, int S3_value) {
  digitalWrite(S2, S2_value);
  digitalWrite(S3, S3_value);
  return pulseIn(sensorOut, LOW);
}

// Function to automatically calibrate white and black
void calibrateWhite() {
  Serial.println("Calibrating white point...");
  
  // Read minimum frequency values from a white surface
  minR = getColorFrequency(LOW, LOW);
  minG = getColorFrequency(HIGH, HIGH);
  minB = getColorFrequency(LOW, HIGH);
  
  // A rough estimate for max values (darkest possible reading)
  maxR = minR * 3; 
  maxG = minG * 3;
  maxB = minB * 3;

  Serial.print("Calibrated Min R:"); Serial.print(minR);
  Serial.print(" G:"); Serial.print(minG);
  Serial.print(" B:"); Serial.println(minB);
  Serial.println("Now point sensor at a black surface to get max values.");
  delay(5000);
  
  // Calibrate max values from a black surface
  maxR = getColorFrequency(LOW, LOW);
  maxG = getColorFrequency(HIGH, HIGH);
  maxB = getColorFrequency(LOW, HIGH);
  
  Serial.print("Calibrated Max R:"); Serial.print(maxR);
  Serial.print(" G:"); Serial.print(maxG);
  Serial.print(" B:"); Serial.println(maxB);
  
  // Safety buffer in case min/max are too close
  if (minR >= maxR) maxR = minR + 50;
  if (minG >= maxG) maxG = minG + 50;
  if (minB >= maxB) maxB = minB + 50;
}
