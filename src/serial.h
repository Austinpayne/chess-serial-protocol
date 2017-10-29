#pragma once

/* serial library by Austin Payne
 */

// This will load the definition for common Particle variable types
#include "Particle.h"

// This is your main class that users will import into their application
class Serial
{
public:
  /**
   * Constructor
   */
  Serial();

  /**
   * Example method
   */
  void begin();

  /**
   * Example method
   */
  void process();

private:
  /**
   * Example private method
   */
  void doit();
};
