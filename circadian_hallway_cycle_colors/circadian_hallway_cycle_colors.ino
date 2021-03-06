// Tested on Arduino Pro Micro 5V 16 MHz 

#define GRN_PIN 9
#define RED_PIN 6
#define BLU_PIN 5

byte RED, GREEN, BLUE; 
byte RED_A = 0;
byte GREEN_A = 0; 
byte BLUE_A = 0;
int led_delay = 0;
byte color_count = 1;     // Count the colors out
#define color_count_max 5 // Set this to the max number of colors defined
#define color_delay 60000 // Define the delay between changing colors in ms
#define time_at_color 0   // Time to stay on a color in ms

// Some Time values
unsigned long TIME_LED = 0;
unsigned long TIME_PHASE = 0;
unsigned long TIME_color = 0;
boolean is_using_morning_colors = true;
boolean is_using_night_color = true;

// Night color defined here
#define N_C1_R 4
#define N_C1_G 1
#define N_C1_B 0

// Evening colors defined here
#define E_C1_R 255
#define E_C1_G 61
#define E_C1_B 0

#define E_C2_R 255
#define E_C2_G 10
#define E_C2_B 0

#define E_C3_R 100
#define E_C3_G 25
#define E_C3_B 0

#define E_C4_R 150
#define E_C4_G 10
#define E_C4_B 0

#define E_C5_R 200
#define E_C5_G 50
#define E_C5_B 0

#define E_MAX_COLOR_DIFFERENCE 105

// Morning colors defined here
#define M_C1_R 50
#define M_C1_G 50
#define M_C1_B 50

#define M_C2_R 20
#define M_C2_G 20
#define M_C2_B 20

#define M_C3_R 150
#define M_C3_G 150
#define M_C3_B 150

#define M_C4_R 100
#define M_C4_G 100
#define M_C4_B 100

#define M_C5_R 255
#define M_C5_G 255
#define M_C5_B 255

#define M_MAX_COLOR_DIFFERENCE 243

void turn_on_morning_colors()
{
  // Assign initial values
  RED = M_C1_R;
  GREEN = M_C1_G;
  BLUE = M_C1_B;
  // Get the led_delay speed
  led_delay = (color_delay - time_at_color) / M_MAX_COLOR_DIFFERENCE; 
  if (led_delay > 41) led_delay = 41;
  is_using_morning_colors = true;
}

void turn_on_evening_colors()
{
  // Assign initial values
  RED = E_C1_R;
  GREEN = E_C1_G;
  BLUE = E_C1_B;
  // Get the led_delay speed
  led_delay = (color_delay - time_at_color) / E_MAX_COLOR_DIFFERENCE; 
  if (led_delay > 41) led_delay = 41;
  is_using_morning_colors = false;
}

void turn_on_night_color()
{
  // Assign initial values
  RED = N_C1_R;
  GREEN = N_C1_G;
  BLUE = N_C1_B;
  // Get the led_delay speed
  led_delay = (color_delay - time_at_color) / E_MAX_COLOR_DIFFERENCE; 
  if (led_delay > 41) led_delay = 41;
  is_using_night_color = true;
}

void setup()
{
  turn_on_night_color();
  
  analogWrite(GRN_PIN, 0);
  analogWrite(RED_PIN, 0);
  analogWrite(BLU_PIN, 0);

  Serial.begin(9600);
}

void loop()
{
  if (millis() - TIME_LED >= led_delay) {
    TIME_LED = millis();

    // Run the LED Function to check and adjust the values
    LED();
  }

  if (millis() - TIME_color >= color_delay) {
    TIME_color = millis();

    // Run the color change function
    color();
  }

  while (Serial.available() > 0) {
    int red = Serial.parseInt();
    int green = Serial.parseInt();
    int blue = Serial.parseInt();
    if (Serial.read() == '\n') {
      if (red == 255) {
        turn_on_evening_colors();  
      }
      if (green == 255) {
        turn_on_night_color();  
      }
      if (blue == 255) {
        turn_on_morning_colors();  
      }
      Serial.print(red, HEX);
      Serial.print(green, HEX);
      Serial.println(blue, HEX);
    }
  }
}

void LED()
{
  // Check Values and adjust "Active" Value
  if (RED != RED_A) {
    if (RED_A > RED) RED_A = RED_A - 1;
    if (RED_A < RED) RED_A++;
  }
  if (GREEN != GREEN_A) {
    if (GREEN_A > GREEN) GREEN_A = GREEN_A - 1;
    if (GREEN_A < GREEN) GREEN_A++;
  }
  if (BLUE != BLUE_A) {
    if (BLUE_A > BLUE) BLUE_A = BLUE_A - 1;
    if (BLUE_A < BLUE) BLUE_A++;
  }

  // Assign modified values to the pwm outputs for each color led
  analogWrite(RED_PIN, RED_A);
  analogWrite(GRN_PIN, GREEN_A);
  analogWrite(BLU_PIN, BLUE_A);
}

void color()
{
  if (is_using_night_color) {
    return;
  }
  
  // Increment the color by one or go back to 1 if maxed out
  if (color_count < color_count_max) {
    color_count++;
  } else {
    color_count = 1;
  }

  if (color_count == 1) {
    if (is_using_morning_colors) {
      RED = M_C1_R;
      GREEN = M_C1_G;
      BLUE = M_C1_B;    
    } else {
      RED = E_C1_R;
      GREEN = E_C1_G;
      BLUE = E_C1_B;      
    }
  } else if (color_count == 2) {
    if (is_using_morning_colors) {
      RED = M_C2_R;
      GREEN = M_C2_G;
      BLUE = M_C2_B;    
    } else {
      RED = E_C2_R;
      GREEN = E_C2_G;
      BLUE = E_C2_B;      
    }
  } else if (color_count == 3) {
    if (is_using_morning_colors) {
      RED = M_C3_R;
      GREEN = M_C3_G;
      BLUE = M_C3_B;    
    } else {
      RED = E_C3_R;
      GREEN = E_C3_G;
      BLUE = E_C3_B;      
    }
  } else if (color_count == 4) {
    if (is_using_morning_colors) {
      RED = M_C4_R;
      GREEN = M_C4_G;
      BLUE = M_C4_B;    
    } else {
      RED = E_C4_R;
      GREEN = E_C4_G;
      BLUE = E_C4_B;      
    }
  } else if (color_count == 5) {
    if (is_using_morning_colors) {
      RED = M_C5_R;
      GREEN = M_C5_G;
      BLUE = M_C5_B;    
    } else {
      RED = E_C5_R;
      GREEN = E_C5_G;
      BLUE = E_C5_B;      
    }
  }
}
