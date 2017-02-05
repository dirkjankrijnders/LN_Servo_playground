/*
If serial output is desired uncommment the #undef line. For production, i.e.
without computer connected this line should be commented out.
*/
#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG_OUTPUT
//#undef DEBUG_OUTPUT

#ifdef DEBUG_OUTPUT
#define USE_SERIAL
#define DEBUG(x) Serial.print(x);
#define DEBUGLN(x) Serial.println(x);
#else
#define DEBUG(x)
#define DEBUGLN(x)
#endif

#ifndef PINSERVO
#define PINSERVO 0
#endif

#endif // CONFIG_H