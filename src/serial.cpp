/* serial library by Austin Payne
 */

#include "serial.h"

/**
 * Constructor.
 */
Serial::Serial()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void Serial::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void Serial::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void Serial::doit()
{
    Serial.println("called doit");
}
