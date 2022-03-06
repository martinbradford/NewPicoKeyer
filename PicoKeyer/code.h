// code.h

#ifndef _CODE_h
#define _CODE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


extern void initialize_pins();
extern void initialize_sd_card();
extern void initialize_keyer_state();
extern void initialize_potentiometer();
extern void initialize_rotary_encoder();
extern void initialize_default_modes();
extern void initialize_watchdog();
extern void check_eeprom_for_initialization();
extern void check_for_beacon_mode();
extern void check_for_debug_modes();
extern void check_buttons();
extern void check_paddles();
extern void check_ptt_tail();
extern void check_potentiometer();
extern void initialize_analog_button_array();
extern void initialize_serial_ports();
extern void initialize_ps2_keyboard();
extern void initialize_usb();
extern void initialize_display();
extern void initialize_debug_startup();
extern void initialize_cw_keyboard();
extern void service_second_core();
extern void service_send_buffer(byte no_print);
extern byte keyer_machine_mode;
extern void service_dit_dah_buffers();
extern void check_for_dirty_configuration();
extern void check_memory_repeat();
extern void check_backlight();
extern void service_sd_card();
extern void service_async_eeprom_write();
extern void service_millis_rollover();
extern void check_serial();
extern void service_paddle_echo();
extern void service_display();
extern void service_client();
extern void initializeCommandMap();

#endif

