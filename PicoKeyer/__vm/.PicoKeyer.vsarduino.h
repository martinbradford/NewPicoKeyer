/* 
	Editor: https://www.visualmicro.com/
			This file is for intellisense purpose only.
			Visual micro (and the arduino ide) ignore this code during compilation. This code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			The contents of the _vm sub folder can be deleted prior to publishing a project
			All non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			Note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Raspberry Pi Pico                                                                                                                (rp2040_rpipico), Platform=rp2040, Package=rp2040
*/

#if defined(_VMICRO_INTELLISENSE)

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define CFG_TUSB_MCU OPT_MCU_RP2040
#define USB_VID 0x2e8a
#define USB_PID 0x000a
#define USB_MANUFACTURER "\"Raspberry
#define USB_PRODUCT "\"Pico\""
#define PICO_CYW43_ARCH_THREADSAFE_BACKGROUND 1
#define CYW43_LWIP 0
#define LWIP_IPV6 0
#define LWIP_IPV4 1
#define LWIP_IGMP 1
#define LWIP_CHECKSUM_CTRL_PER_NETIF 1
#define ARDUINO_VARIANT "rpipico"
#define ARM_MATH_CM0_FAMILY 1
#define ARM_MATH_CM0_PLUS 1
#define SERIALUSB_PID 0x000a
#define USBD_MAX_POWER_MA 250
#define F_CPU 125000000
#define ARDUINO 108019
#define ARDUINO_RASPBERRY_PI_PICO 1
#define BOARD_NAME "RASPBERRY_PI_PICO"
#define ARDUINO_ARCH_RP2040 1
#define __cplusplus 201103L
#define __GNUC__ 7
#define _Pragma(x)
#define __ARMCC_VERSION 6010050

#define __PTRDIFF_TYPE__ int
#define __ARM__
//#define __arm__ 1
#define always_inline
#define __inline__
#define __asm__(x)
#define __attribute__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __volatile__
#define _Pragma(x)
#define __ASM
#define __INLINE

#define __INT32_TYPE__ long int
#define __INTMAX_MAX__ 0x7fffffffffffffffLL
#define STM32H747xx 1
#define CORE_CM7
#define __INTPTR_TYPE__ int
#define __SIZE_TYPE__ unsigned int


#define __ARM_COMPAT_H
#undef __cplusplus
#define __cplusplus 201103L
#define __cplusplus__ 1
#define DOXYGEN_ONLY 1

typedef void* common_type_t;
typedef  int PinName;
#define PinName int
typedef  int PinMode;
#define PinMode int
#define _GLIBCXX_CSTRING 1
#define _BASIC_IOS_H 1
#define __COMPAT_H__ 1
//#define  _STDLIB_H_ 1
typedef void* __builtin_va_list;

class VM_DBG {
public:
	// Send a Message to the Serial Monitor via WiFi Connection 
	void sendUserMessage(const char* theMessage) {};
} MicroDebug;
#include <arduino.h>
#include <pins_arduino.h> 
#undef INPUT
#define INPUT 0x0
#undef OUTPUT
#define OUTPUT 0x1
#undef LOW
#define LOW 0u
#undef HIGH
#define HIGH 1u

// Additions to override the introduced code from overrides.h which confuses intellisense
#undef _GLIBCXX_DEFAULT_ABI_TAG
#define _GLIBCXX_DEFAULT_ABI_TAG 
#undef _GLIBCXX_HOSTED
#define _GLIBCXX_HOSTED 0

void pinMode(int pinNumber, int pinMode);
void pinMode(int pinNumber, int pinMode) {}
void digitalWrite(unsigned int pinNumber, PinStatus status);
void digitalWrite(unsigned int pinNumber, PinStatus status) {}
void digitalWrite(unsigned int pinNumber, unsigned int status);
void digitalWrite(unsigned int pinNumber, unsigned int status) {}
PinStatus digitalRead(unsigned int pinNumber);
PinStatus digitalRead(unsigned int pinNumber) {}
int analogRead(int pinNumber);
int analogRead(int pinNumber) {}
void analogWrite(int pinNumber, int value) {}
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout) {}
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout) {}
void shiftOut(int dataPin, int clockPin, BitOrder bitOrder, uint8_t val) {}
uint8_t shiftIn(int dataPin, int clockPin, BitOrder bitOrder) {}
void attachInterrupt(int interruptNumber, voidFuncPtr callback, PinStatus mode) {}
void attachInterruptParam(int interruptNumber, voidFuncPtrParam callback, PinStatus mode, void* param) {}
void detachInterrupt(int interruptNumber) {}

#include "PicoKeyer.ino"
#endif
#endif
