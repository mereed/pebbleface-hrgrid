#pragma once
#include <pebble.h>

#define SETTINGS_KEY 1

// A structure containing our settings
typedef struct ClaySettings {

  bool blink;
  bool bluetoothvibe;
  bool hourlyvibe;
  int row2;
  int row3;
	
  bool WeatherIsFahrenheit;
  int WeatherTemperature;
  int WeatherIcon;

	
} __attribute__((__packed__)) ClaySettings;





static void prv_default_settings();
static void prv_load_settings();
static void prv_save_settings();
static void prv_update_display();
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context);
static void prv_window_load(Window *window);
static void prv_window_unload(Window *window);
static void prv_init(void);
static void prv_deinit(void);

