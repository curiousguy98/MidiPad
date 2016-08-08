
#include <pushbuttoncommand.h>

///#define TRACE_DEBUG

#define MAX_BUTTONS                 2
#define DEBOUNCE_TIME_PUSH_BUTTONS  3   // x DELAY_TIME
#define DOUBLE_CLICK_TIME           20  // x DELAY_TIME
#define LONG_PUSH_TIME              250 // x DELAY_TIME

#define DELAY_TIME                  10 

#define INPUT1                      8
#define INPUT2                      9
unsigned char inputs[] = {INPUT1, INPUT2};
                        
PushButtonCommand pushbuttons;
unsigned char cmd[MAX_BUTTONS];

void setup() 
{
    /* Init input pins */
    for(int i = 0; i < MAX_BUTTONS; i++)
    {
      pinMode(inputs[i], INPUT_PULLUP);
      cmd[i] = 0;
    }

    /* Init push button command lib, returns button commands
     * to increase useability of switches. 
     * 0 = none
     * 1 = click
     * 2 = long push
     * 3 = double click */
    pushbuttons.setup(MAX_BUTTONS, DEBOUNCE_TIME_PUSH_BUTTONS, DOUBLE_CLICK_TIME, LONG_PUSH_TIME);

    /* Init serial connection */
#if defined(TRACE_DEBUG)    
    Serial.begin(9600);
    Serial.println("*************** Push Button Command Test ********************");
#endif    
}

void loop() 
{   
 
  for(int i = 0; i < MAX_BUTTONS; i++)
  {
    /* update button command loop if no command is set */
    if(cmd[i] == 0)
    {
      cmd[i] = pushbuttons.loop(i, !digitalRead(inputs[i]));
    }

    /* write button command serial */
    switch(cmd[i])
    {
      case PUSH_CMD_CLICK:
#if defined(TRACE_DEBUG)
        Serial.println("Switch " + String(i + 1) + " -> CLICK");
#else
        usbMIDI.sendNoteOn(62, 99, i+1);        
#endif
        break;
      case PUSH_CMD_LONG_PUSH:    
        Serial.println("Switch " + String(i + 1) + " -> LONG PUSH");
        break;
      case PUSH_CMD_DOUBLE_CLICK:
#if defined(TRACE_DEBUG)      
        Serial.println("Switch " + String(i + 1) + " -> DOUBLE CLICK");
#else
        usbMIDI.sendNoteOn(64, 99, i+1);        
#endif
        break;
      case PUSH_CMD_LONG_CLICK:
#if defined(TRACE_DEBUG)      
        Serial.println("Switch " + String(i + 1) + " -> LONG CLICK");
#else
        usbMIDI.sendNoteOn(66, 99, i+1);        
#endif
        break;
    }

    /* reset button command after it was send to serial */
    if(cmd[i] > 0 )
    {
      cmd[i] = 0;
    }
  }
  
  delay(DELAY_TIME);
}
