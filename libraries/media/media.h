#ifndef media_h
#define media_h

#include "HID.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else
#define VolD  1
#define VolI  2
#define Mute  4
#define Pause 8
#define Next  16
#define Prev  32
#define Fast  64
#define Rewind  128

//================================================================================
//================================================================================

//  Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
  uint8_t keys[2];
} MediaReport;

class Media_ : public Print
{
private:
  MediaReport _mediaReport;
  void sendReport(MediaReport* keys);
public:
  Media_ (void);
  void begin(void);
  void end(void);
  void releaseAll(void);
  size_t write(uint8_t k);
};
extern Media_ Media;

#endif
#endif

