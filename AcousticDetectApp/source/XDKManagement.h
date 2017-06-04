#ifndef XDK110_XDKMANAGEMENT_H_
#define XDK110_XDKMANAGEMENT_H_

#define SECONDS(x) ((portTickType) (x * 1000) / portTICK_RATE_MS)
//#define THING_ID                "com.bosch.sensors:sgcanteen_6"         /**< Macros to define WPA/WPA2 network settings */
//#define THING_ID                "com.bosch.sgp.cr:xdk13"         /**< Macros to define WPA/WPA2 network settings */
//#define THING_ID                "com.bosch.hackathonsg:xdk1"         /**< Macros to define WPA/WPA2 network settings */
//#define THING_ID                "sp:xdk1"         /**< Macros to define WPA/WPA2 network settings */
#define THING_ID                "boschxdk22"         /**< Macros to define WPA/WPA2 network settings */

#define BUTTON1PRESSED                1
#define BUTTON2PRESSED                2
#define BUTTON1RELEASED                4
#define BUTTON2RELEASED                8

uint32_t noiseLevel;
uint32_t temperature;
uint32_t humidity;
uint32_t pressure;
int32_t xmagnetic;
int32_t ymagnetic;
int32_t zmagnetic;
int32_t milliLux;
uint32_t speed;
uint32_t distance;
int32_t crash;

int32_t xAccelData;
int32_t yAccelData;
int32_t zAccelData;
int32_t xGyroscopeData;
int32_t yGyroscopeData;
int32_t zGyroscopeData;
int32_t degree;
double speed_2;
double distance_2;
double xspeed;
double yspeed;

extern void requestOneTimeDataSend(uint32_t buttonStatus);

#endif /* XDK110_XDKMANAGEMENT_H_ */
