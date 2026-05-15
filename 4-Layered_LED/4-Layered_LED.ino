// Pin Definitions for MBI5026 / Shift Registers
const int sdiPin   = 4;  // SDI (Serial Data In)
const int clkPin   = 3;  // CLK (Clock)
const int lePin    = 2;  // LE (Latch Enable)

// Layer (Common Cathode/Anode Transistor) Pins
const int layers[] = {5, 6, 7, 8};  
const int numLayers = 4; 

// 3 chips * 16 outputs = 48 bits total 
const int totalBits = 80; 

void setup() {
  pinMode(sdiPin, OUTPUT);
  pinMode(clkPin, OUTPUT);
  pinMode(lePin, OUTPUT);
  
  // Initialize all layer pins as outputs and turn them OFF
  for (int i = 0; i < numLayers; i++) {
    pinMode(layers[i], OUTPUT);
    digitalWrite(layers[i], LOW); 
  }
  
  digitalWrite(lePin, LOW);
  digitalWrite(clkPin, LOW);
}

/**
 * Updates the shift registers.
 * @param state: If true, sets all bits to HIGH. If false, sets all to LOW.
 * @param randomMode: If true, ignores 'state' and pushes random bits.
 */
void updateDrivers(bool state, bool randomMode = false) {
  // LE must be LOW while shifting data
  digitalWrite(lePin, LOW);
  
  for (int i = 0; i < totalBits; i++) {
    digitalWrite(clkPin, LOW);
    
    if (randomMode) {
      digitalWrite(sdiPin, random(0, 2));
    } else {
      digitalWrite(sdiPin, state ? HIGH : LOW);
    }
    
    digitalWrite(clkPin, HIGH); // Data is sampled on the rising edge
  }
  
  // Latch the data to the outputs
  digitalWrite(lePin, HIGH);
  delayMicroseconds(1); 
  digitalWrite(lePin, LOW); 
}

void loop() {
  // --- STEP 1: Test each layer one by one ---
  updateDrivers(true); // Turn all column drivers ON

  for (int i = 0; i < numLayers; i++) {
    digitalWrite(layers[i], HIGH); 
    delay(1000); // Keep layer on for 1 second
    digitalWrite(layers[i], LOW);  
  }

  // --- STEP 2: All layers ON simultaneously ---
  // Turn all column drivers ON (just in case)
  updateDrivers(true); 
  
  // Switch all layer transistors ON
  for (int i = 0; i < numLayers; i++) {
    digitalWrite(layers[i], HIGH);
  }
  
  delay(2000); // Keep the entire cube lit for 2 seconds
  
  // Turn all layers OFF before moving to next animation
  for (int i = 0; i < numLayers; i++) {
    digitalWrite(layers[i], LOW);
  }

  // --- STEP 3: Random Sparkle Animation ---
  unsigned long startTime = millis();
  while (millis() - startTime < 4000) { // Run for 4 seconds
    updateDrivers(false, true); // Push random bits to columns
    
    int rLayer = random(0, numLayers);
    digitalWrite(layers[rLayer], HIGH);
    delay(50); // Short flash duration
    digitalWrite(layers[rLayer], LOW);
  }
  
  // Brief pause before restarting the loop
  delay(500);
}
