
#include "media.h"


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


Media_::Media_(void)
{
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);
 
}

void Media_::begin(void)
{
  
}

void Media_::end(void)
{
}

void Media_::sendReport(MediaReport* keys)
{
	HID().SendReport(5,keys,sizeof(MediaReport));
}

void Media_::releaseAll(void)
{
	for (int i = 0; i < sizeof(_mediaReport.keys); i++)
	{
		_mediaReport.keys[i] = 0;
	}
  sendReport(&_mediaReport);


}

size_t Media_::write(uint8_t k) 
{
  _mediaReport.keys[0] = k;
  sendReport(&_mediaReport);
  releaseAll();
}














