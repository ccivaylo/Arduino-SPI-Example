#include <SPI.h>
#include <Adafruit_SSD1306.h>

/*
Thermistor Parameters:
* RT0 = 10kOhm
* B = 3977K +/- 0.75%
* T0 = 25C - Temperature at RT0
* +/- 5% - Tolerance
*/

#define RT0 10000   // Resistance of the thermistor at 25C is 10kOhm
#define RT50 4500   // Resistance of thermistor at 50C is ~4.5kOhm
#define B 3977      // B coefficient

#define VCC 5       // 5V input VCC
#define R 10000     // Resistor equal to thermistor for calculation

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define THERMISTOR 0  // Thermistor connected to analog pin 0
#define BUTTON_1 7    // Button connected to digital pin 7
#define REFRESH_RATE 50
#define TEMP_TIMER 500
#define TRUE 1
#define FALSE 0

#define OLED_MOSI   9 //Display data/MOSI (D1)
#define OLED_CLK   10 //Display clock (D0)
#define OLED_DC    11 //Display 
#define OLED_CS    12 //Display chip select
#define OLED_RESET 13 //Display reset
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

typedef unsigned int uint;

float ln, temperature;
float T0K = 25 + 273.15; // Convert 25C to Kelvin
float VRT;  // Voltage of thermistor
float RT;   // Resistance of thermistor
float VR;   // Voltage of resistor
unsigned int temp_timer_counter = TEMP_TIMER;
bool temp_timer_running_flag = FALSE;
String ipAddress = ""; // Store the received IP address

float lastTemperature = -999.0;  // Initialize to an impossible temperature value

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(BUTTON_1, INPUT); // Set button pin as input

  // Initialize the OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Stop execution
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Screen initialized!");
  display.display();
  delay(500);
  display.clearDisplay();
}

void loop() {
  // Check for new data on Serial
  if (Serial.available()) {
    ipAddress = Serial.readStringUntil('\n'); // Read incoming IP address
    Serial.println("Received IP: " + ipAddress); // Debug output
    updatePage1(); // Refresh the OLED to show the IP
  }

  // Temperature calculation logic
  Timer_Start(&temp_timer_counter, &temp_timer_running_flag);
  if (FALSE == Timer_IsStarted_Elapsed(temp_timer_counter)) {
    Timer_Reset(&temp_timer_counter, TEMP_TIMER, &temp_timer_running_flag);

    VRT = analogRead(THERMISTOR);  // Read thermistor value
    VRT = (5.00 / 1023.00) * VRT; // Convert to voltage
    VR = VCC - VRT;
    RT = VRT / (VR / R);          // Calculate resistance
    ln = log(RT / RT0);
    temperature = (1 / ((ln / B) + (1 / T0K))) - 273.15; // Convert to Celsius

    // Only send temperature if it has changed
    if (abs(temperature - lastTemperature) >= 0.1) {  // Change threshold
      Serial.print("Temperature: ");
      Serial.println(temperature);
      lastTemperature = temperature;  // Update last temperature
      updatePage1();  // Refresh the OLED
    }
  }

  delay(REFRESH_RATE);
}

void updatePage1() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Display IP Address
  display.setCursor(0, 0);
  display.print("Local IP");
  display.setCursor(30, 10);
  display.print(ipAddress);

  // Display Temperature
  display.setCursor(0, 20);
  display.print("Temperature ");
  display.setCursor(30, 30);
  display.print(temperature);
  display.print(" C");

  display.display();
}

/*
  Timer Functions for handling timing
*/
void Timer_Start(uint* counter, bool* flag) {
  *counter -= REFRESH_RATE;
  *flag = TRUE;
}

void Timer_Reset(uint* counter, uint timer, bool* flag) {
  *counter = timer;
  *flag = FALSE;
}

bool Timer_IsStarted_Elapsed(uint counter) {
  return counter != 0;
}
