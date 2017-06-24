#include <media.h>
Media_ Media;

const int buttonPin = 2;     // the number of the pushbutton pin

void setup()
{
    // put your setup code here, to run once:
    Media.begin();  
    Media.releaseAll();
    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT);

}

void loop()
{
    Media.releaseAll();
    if(digitalRead(buttonPin) == LOW)
    {
      Media.write(Mute);//the keywords incldue [VolD/VolI/Mute/Pause/Next/Prev/Fast/Rewind]

      while(digitalRead(buttonPin) == LOW);
    }
}

