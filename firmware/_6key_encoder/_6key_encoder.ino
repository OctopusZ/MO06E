#include "media.h"

Media_ Media;
//Media_ Media = new Media_();


/****************************************************************
                         encoder
*****************************************************************/

#include <Encoder.h>
#define CLOCKWISE  1  //0 for clockwise / 1 for anticlockwise
#define ENCODER_SPEED  1      //speed of the vol change. The greater the value, the slower the speed.
Encoder myEnc (8, 9); //connect to the pinouts of the encoder

/****************************************************************
                          keypad
*****************************************************************/

#include <Keypad.h>

const byte ROWS = 2; //2 rows
const byte COLS = 4; //4 columns


char keymap[ROWS][COLS] =
{
    { Mute, Rewind, Fast, 0 },
    { Prev, Pause, Next, Mute }
};


byte rowPins[ROWS] = { A3, 10 }; //connect to the row pinouts of the kpd
byte colPins[COLS] = { 15, 14, 16, 3 }; //connect to the column pinouts of the kpd

Keypad kpd = Keypad (makeKeymap (keymap), rowPins, colPins, ROWS, COLS);


/****************************************************************
                          LED
*****************************************************************/

byte LEDPins[6] = { 1, 0, 2,
                    A0, A1, A2
                  }; //connect to the pinouts of the LEDs

/****************************************************************
*****************************************************************/



void LED_On (char num)
{
    if (num > 3)
        num -= 1;
    digitalWrite (LEDPins[num], HIGH);
}

void LED_Off (char num)
{
    if (num > 3)
        num -= 1;
    digitalWrite (LEDPins[num], LOW);
}

void setup()
{
    // put your setup code here, to run once:
    
    Media.begin();  
    Media.releaseAll();
    Serial.begin (9600);
    for (int i = 0; i < 8; i++)
    {
        pinMode(LEDPins[i], OUTPUT);

    }
    for (int i = 0; i < 8; i++)
    {
        LED_Off (i);

    }

}

void loop()
{
    Media.releaseAll();

    long newPosition = myEnc.read();

    while (newPosition > ENCODER_SPEED)
    {

        if (newPosition / ENCODER_SPEED)
        {
            if (CLOCKWISE == 0)
            {
                Media.write(VolI);
            }
            else
            {
                Media.write(VolD);
            }
        }

        newPosition = newPosition - ENCODER_SPEED;
        myEnc.write (newPosition);

    }
    while (newPosition < - ENCODER_SPEED)
    {
        if ((- newPosition) / ENCODER_SPEED)
        {


            if (CLOCKWISE == 0)
            {
                Media.write(VolD);
            }
            else
            {
                Media.write(VolI);
            }
        }
        newPosition += ENCODER_SPEED;
        myEnc.write (newPosition);

    }


    if (kpd.getKeys() )
    {
        for (int i = 0; i < LIST_MAX; i++) // Scan the whole key list.
        {
            if (kpd.key[i].stateChanged)   // Only find keys that have changed state.
            {
                switch (kpd.key[i].kstate)    // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                {
                case PRESSED:

                    Media.write(kpd.key[i].kchar);
                    LED_On (kpd.key[i].kcode);
                    Serial.println (kpd.key[i].kcode);
                    break;
                case HOLD:

                    break;
                case RELEASED:
                    //LED_Off (kpd.key[i].kcode);
                    break;
                case IDLE:

                    break;
                }

            }
        }
    }


}

