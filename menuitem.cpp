#include "menuitem.h"
#include "page.h"


MenuItem::MenuItem() :
    name(NULL),
    type(display),
    action(NULL),
    level(0),
    name_len(0),
    data_col(0),
    target_page(-1),
    command_letter(0)
{}


MenuItem::MenuItem(const char * name_p, MenuAction *action_p,  uint8_t level_p, MenuType type_p, uint8_t target_page_p):
    name(name_p),
    type(type_p),
    action(action_p),
    level(level_p),
    name_len(strlen(name_p)),
    data_col(0),
    target_page(target_page_p),
    command_letter(0)
{}

MenuItem::MenuItem(Page &target_page_p):
    name(target_page_p.name),
    type(menu),
    action(NULL),
    level(0),
    name_len(strlen(name)),
    data_col(0),
    target_page(target_page_p.page_num),
    command_letter(0)
{}

//
// Produce a formatted string in a buffer corresponding to the value provided.
// If the 'width' parameter is non-zero, the value will be padded with leading
// zeroes to achieve the specified width.  The number of characters added to
// the buffer (not including the null termination) is returned.
//
unsigned MenuAction::fmtUnsigned(unsigned long val, char *buf, unsigned bufLen, byte width)
{
 if (!buf || !bufLen)
   return(0);

 // produce the digit string (backwards in the digit buffer)
 char dbuf[10];
 unsigned idx = 0;
 while (idx < sizeof(dbuf))
 {
   dbuf[idx++] = (val % 10) + '0';
   if ((val /= 10) == 0)
     break;
 }

 // copy the optional leading zeroes and digits to the target buffer
 unsigned len = 0;
 byte padding = (width > idx) ? width - idx : 0;
 char c = '0';
 while ((--bufLen > 0) && (idx || padding))
 {
   if (padding)
     padding--;
   else
     c = dbuf[--idx];
   *buf++ = c;
   len++;
 }

 // add the null termination
 *buf = '\0';
 return(len);
}

//
// Format a floating point value with number of decimal places.
// The 'precision' parameter is a number from 0 to 6 indicating the desired decimal places.
// The 'buf' parameter points to a buffer to receive the formatted string.  This must be
// sufficiently large to contain the resulting string.  The buffer's length may be
// optionally specified.  If it is given, the maximum length of the generated string
// will be one less than the specified value.
//
// example: fmtDouble(3.1415, 2, buf); // produces 3.14 (two decimal places)
//
void MenuAction::fmtDouble(double val, byte precision, char *buf, unsigned bufLen)
{
 if (!buf || !bufLen)
   return;

 // limit the precision to the maximum allowed value
 const byte maxPrecision = 6;
 if (precision > maxPrecision)
   precision = maxPrecision;

 if (--bufLen > 0)
 {
   // check for a negative value
   if (val < 0.0)
   {
     val = -val;
     *buf++ = '-';
     bufLen--;
   }

   // compute the rounding factor and fractional multiplier
   double roundingFactor = 0.5;
   unsigned long mult = 1;
   for (byte i = 0; i < precision; i++)
   {
     roundingFactor /= 10.0;
     mult *= 10;
   }

   if (bufLen > 0)
   {
     // apply the rounding factor
     val += roundingFactor;

     // add the integral portion to the buffer
     unsigned len = fmtUnsigned((unsigned long)val, buf, bufLen);
     buf += len;
     bufLen -= len;
   }

   // handle the fractional portion
   if ((precision > 0) && (bufLen > 0))
   {
     *buf++ = '.';
     if (--bufLen > 0)
       buf += fmtUnsigned((unsigned long)((val - (unsigned long)val) * mult), buf, bufLen, precision);
   }
 }

 // null-terminate the string
 *buf = '\0';
}