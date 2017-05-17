
/****************************************************************
                       HID Report
*****************************************************************/

#include <HID.h>

static const uint8_t _hidReportDescriptor[] PROGMEM =
{

    0x05, 0x0c,       //   USAGE_PAGE (Consumer Page)
    0x09, 0x01,       //   USAGE (Consumer Control)
    0xa1, 0x01,       //   COLLECTION (Application)
    0x85, 0x05,       //   REPORT_ID (5)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)

    0x09, 0xea,       //   USAGE (Volume Decrement)
    0x09, 0xe9,       //   USAGE (Volume Increment)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x02,       //   Report Count (2)
    0x81, 0x06,       //   Input (Data, Variable, Relative)


    0x09, 0xe2,       //   USAGE (Mute)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x06,       //   Input (Data, Variable, Relative)

    0x09, 0xcd,       //   USAGE (Play/Pause)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x06,       //   Input (Data, Variable, Relative)

    0x09, 0xb5,       //   USAGE (Next)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x06,       //   Input (Data, Variable, Relative)

    0x09, 0xb6,       //   USAGE (Prev)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x06,       //   Input (Data, Variable, Relative)

    0x09, 0xb3,       //   USAGE (Fast Forward)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x06,       //   Input (Data, Variable, Relative)

    0x09, 0xb4,       //   USAGE (Rewind)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x06,       //   Input (Data, Variable, Relative)

    0x0a, 0x83, 0x01, //   USAGE (Windows Media Player(0x183))
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x06,       //   Input (Data, Variable, Relative)

    0x95, 0x07,       //   Report Count (7) Number of bits remaining in byte
    0x81, 0x07,       //   Input (Constant, Variable, Relative)

    0xc0              //   END_COLLECTION
};



//the USAGE of the keyboard
#define VolD  1
#define VolI  2
#define Mute  4
#define Pause 8
#define Next  16
#define Prev  32
#define Fast  64
#define Rewind  128


#define REPORT_LENGTH  2 //the length of HID report buffer
uint8_t report[REPORT_LENGTH];//buffer of the HID report


/****************************************************************
                         encoder
*****************************************************************/

#include <Encoder.h>
#define CLOCKWISE  0
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
    { Prev, Pause, Next, Mute },

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

void Report_clean (void)
{
    for (size_t i = 0; i < REPORT_LENGTH; i++)
    {
        report[i] = 0;
    }

}

void Media_press (char dat)
{
    report[0] = dat;
    HID().SendReport (5, report, 2); //(id,data,len)
    Report_clean();
    HID().SendReport (5, report, 2); //(id,data,len)
}

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
    static HIDSubDescriptor node (_hidReportDescriptor, sizeof (_hidReportDescriptor) );
    HID().AppendDescriptor (&node);

    Report_clean();
    Serial.begin (9600);
    for (size_t i = 0; i < 8; i++)
    {
        pinMode(LEDPins[i], OUTPUT);

    }
    for (size_t i = 0; i < 8; i++)
    {
        LED_Off (i);

    }

}

void loop()
{
    Report_clean();

    long newPosition = myEnc.read();


    while (newPosition > 0)
    {

		if (CLOCKWISE == 0)
		{
			Media_press(VolD);
		}
		else
		{
			Media_press(VolI);
		}

        newPosition -= 1;
        myEnc.write (newPosition);

    }
    while (newPosition < 0)
    {

		if (CLOCKWISE == 0)
		{
			Media_press(VolI);
		}
		else
		{
			Media_press(VolD);
		}

        newPosition += 1;
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

                    Media_press (kpd.key[i].kchar); //send the keychar to the computer
                    LED_On (kpd.key[i].kcode);
                    Serial.println (kpd.key[i].kcode);
                    break;
                case HOLD:

                    break;
                case RELEASED:
                    LED_Off (kpd.key[i].kcode);
                    break;
                case IDLE:

                    break;
                }

            }
        }
    }


}

