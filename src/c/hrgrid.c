/*

Copyright (C) 2017 Mark Reed

Original GRID code by Lukasz Z
https://github.com/pebble-hacks/lukasz-projects/tree/master/gridlite

Original Weather/Clay code by orviwan (Jon Barlow)
https://github.com/orviwan/Chunk-Weather-v2.0

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

-------------------------------------------------------------------

*/

#include <pebble.h>
#include "main.h"
#include <time.h>
#include <ctype.h>
#include "config.h"

Window *window;
TextLayer* time_layer[16];
char time_text[] = "1234";
char month_text[] = "DECEMBER    ";
char weekday_text[] = "THURDSDAY    ";
char day_text[] = "23RD";
char grid[16][2];
GColor foregroundcolor;
GColor backgroundcolor;

static TextLayer *layer_day_text, *layer_month_text, *layer_time_text, *layer_time2_text, *layer_date_text, *s_hrm_layer, *s_temperature_layer;
static BitmapLayer *s_weather_icon_layer;
static GBitmap *s_weather_icon;
static char s_temperature_text[8];

static int s_timer_minute = 0;
static int s_initial_minute;

static int s_hr;
static char s_current_hr_buffer[8];
static BitmapLayer *heart_layer;
static BitmapLayer *wk_layer;
static GBitmap *heart_image;
static GBitmap *wk_image;
TextLayer *layer_week_text;
int charge_percent = 0;
TextLayer *battery_text_layer;

GBitmap *img_battery_100;
GBitmap *img_battery_80;
GBitmap *img_battery_60;
GBitmap *img_battery_40;
GBitmap *img_battery_20;
GBitmap *img_battery_00;
BitmapLayer *layer_batt_img;

int cur_day = -1;

GFont time_font;
  GFont time_font2;
  GFont time_font3;
  GFont time_font4;

static uint8_t WEATHER_ICONS[] = {
  RESOURCE_ID_IMAGE_TORNADO,
  RESOURCE_ID_IMAGE_TROPICAL_STORM,
  RESOURCE_ID_IMAGE_HURRICANE,
  RESOURCE_ID_IMAGE_SEVERE_THUNDERSTORMS,
  RESOURCE_ID_IMAGE_THUNDERSTORMS,
  RESOURCE_ID_IMAGE_MIXED_RAIN_AND_SNOW,
  RESOURCE_ID_IMAGE_MIXED_RAIN_AND_SLEET,
  RESOURCE_ID_IMAGE_MIXED_SNOW_AND_SLEET,
  RESOURCE_ID_IMAGE_FREEZING_DRIZZLE,
  RESOURCE_ID_IMAGE_DRIZZLE,
  RESOURCE_ID_IMAGE_FREEZING_RAIN,
  RESOURCE_ID_IMAGE_SHOWERS,
  RESOURCE_ID_IMAGE_SHOWERS2,
  RESOURCE_ID_IMAGE_SNOW_FLURRIES,
  RESOURCE_ID_IMAGE_LIGHT_SNOW_SHOWERS,
  RESOURCE_ID_IMAGE_BLOWING_SNOW,
  RESOURCE_ID_IMAGE_SNOW,
  RESOURCE_ID_IMAGE_HAIL,
  RESOURCE_ID_IMAGE_SLEET,
  RESOURCE_ID_IMAGE_DUST,
  RESOURCE_ID_IMAGE_FOGGY,
  RESOURCE_ID_IMAGE_HAZE,
  RESOURCE_ID_IMAGE_SMOKY,
  RESOURCE_ID_IMAGE_BLUSTERY,
  RESOURCE_ID_IMAGE_WINDY,
  RESOURCE_ID_IMAGE_COLD,
  RESOURCE_ID_IMAGE_CLOUDY,
  RESOURCE_ID_IMAGE_MOSTLY_CLOUDY_NIGHT,
  RESOURCE_ID_IMAGE_MOSTLY_CLOUDY_DAY,
  RESOURCE_ID_IMAGE_PARTLY_CLOUDY_NIGHT,
  RESOURCE_ID_IMAGE_PARTLY_CLOUDY_DAY,
  RESOURCE_ID_IMAGE_CLEAR_NIGHT,
  RESOURCE_ID_IMAGE_SUNNY,
  RESOURCE_ID_IMAGE_FAIR_NIGHT,
  RESOURCE_ID_IMAGE_FAIR_DAY,
  RESOURCE_ID_IMAGE_MIXED_RAIN_AND_HAIL,
  RESOURCE_ID_IMAGE_HOT,
  RESOURCE_ID_IMAGE_ISOLATED_THUNDERSTORMS,
  RESOURCE_ID_IMAGE_SCATTERED_THUNDERSTORMS,
  RESOURCE_ID_IMAGE_SCATTERED_THUNDERSTORMS2,
  RESOURCE_ID_IMAGE_SCATTERED_SHOWERS,
  RESOURCE_ID_IMAGE_HEAVY_SNOW,
  RESOURCE_ID_IMAGE_PARTLY_CLOUDY,
  RESOURCE_ID_IMAGE_SCATTERED_SNOW_SHOWERS,
  RESOURCE_ID_IMAGE_HEAVY_SNOW2,
  RESOURCE_ID_IMAGE_THUNDERSHOWERS,
  RESOURCE_ID_IMAGE_SNOW_SHOWERS,
  RESOURCE_ID_IMAGE_ISOLATED_THUNDERSHOWERS,
  RESOURCE_ID_IMAGE_NOT_AVAILABLE  
};

typedef enum {
  WEATHER_ICON_TORNADO=0,
  WEATHER_ICON_TROPICAL_STORM=1,
  WEATHER_ICON_HURRICANE=2,
  WEATHER_ICON_SEVERE_THUNDERSTORMS=3,
  WEATHER_ICON_THUNDERSTORMS=4,
  WEATHER_ICON_MIXED_RAIN_AND_SNOW=5,
  WEATHER_ICON_MIXED_RAIN_AND_SLEET=6,
  WEATHER_ICON_MIXED_SNOW_AND_SLEET=7,
  WEATHER_ICON_FREEZING_DRIZZLE=8,
  WEATHER_ICON_DRIZZLE=9,
  WEATHER_ICON_FREEZING_RAIN=10,
  WEATHER_ICON_SHOWERS=11,
  WEATHER_ICON_SHOWERS2=12,
  WEATHER_ICON_SNOW_FLURRIES=13,
  WEATHER_ICON_LIGHT_SNOW_SHOWERS=14,
  WEATHER_ICON_BLOWING_SNOW=15,
  WEATHER_ICON_SNOW=16,
  WEATHER_ICON_HAIL=17,
  WEATHER_ICON_SLEET=18,
  WEATHER_ICON_DUST=19,
  WEATHER_ICON_FOGGY=20,
  WEATHER_ICON_HAZE=21,
  WEATHER_ICON_SMOKY=22,
  WEATHER_ICON_BLUSTERY=23,
  WEATHER_ICON_WINDY=24,
  WEATHER_ICON_COLD=25,
  WEATHER_ICON_CLOUDY=26,
  WEATHER_ICON_MOSTLY_CLOUDY_NIGHT=27,
  WEATHER_ICON_MOSTLY_CLOUDY_DAY=28,
  WEATHER_ICON_PARTLY_CLOUDY_NIGHT=29,
  WEATHER_ICON_PARTLY_CLOUDY_DAY=30,
  WEATHER_ICON_CLEAR_NIGHT=31,
  WEATHER_ICON_SUNNY=32,
  WEATHER_ICON_FAIR_NIGHT=33,
  WEATHER_ICON_FAIR_DAY=34,
  WEATHER_ICON_MIXED_RAIN_AND_HAIL=35,
  WEATHER_ICON_HOT=36,
  WEATHER_ICON_ISOLATED_THUNDERSTORMS=37,
  WEATHER_ICON_SCATTERED_THUNDERSTORMS=38,
  WEATHER_ICON_SCATTERED_THUNDERSTORMS2=39,
  WEATHER_ICON_SCATTERED_SHOWERS=40,
  WEATHER_ICON_HEAVY_SNOW=41,
  WEATHER_ICON_SCATTERED_SNOW_SHOWERS=42,
  WEATHER_ICON_HEAVY_SNOW2=43,
  WEATHER_ICON_PARTLY_CLOUDY=44,
  WEATHER_ICON_THUNDERSHOWERS=45,
  WEATHER_ICON_SNOW_SHOWERS=46,
  WEATHER_ICON_ISOLATED_THUNDERSHOWERS=47,
  WEATHER_ICON_NOT_AVAILABLE=48
} WeatherIcon;



// A struct for our specific settings (see main.h)
ClaySettings settings;

int stringToInt(char *str);


// Initialize the default settings
static void prv_default_settings() {
  settings.bluetoothvibe = false;
  settings.hourlyvibe = false;
  settings.row2 = 0;
  settings.row3 = 0;
  settings.WeatherIsFahrenheit = true;
  settings.WeatherTemperature = 999;
  settings.WeatherIcon = 48;	
}

void update_battery_state(BatteryChargeState charge_state) {
    static char battery_text[] = "100";

    if (charge_state.is_charging) {
        snprintf(battery_text, sizeof(battery_text), "%d", charge_state.charge_percent);
    } else {
        snprintf(battery_text, sizeof(battery_text), "%d", charge_state.charge_percent);       

        if (charge_state.charge_percent <= 10) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_00);
	    } else if (charge_state.charge_percent <= 20) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_20);
		} else if (charge_state.charge_percent <= 40) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_40);
        } else if (charge_state.charge_percent <= 60) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_60);
		} else if (charge_state.charge_percent <= 80) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_80);
		} else {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_100);
        } 
    } 
    charge_percent = charge_state.charge_percent;   
    text_layer_set_text(battery_text_layer, battery_text);
} 


void draw_time(){
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  
  if(clock_is_24h_style()==true){
    strftime(time_text, sizeof(time_text), "%H%M", tick_time);
  }else{
    strftime(time_text, sizeof(time_text), "%I%M", tick_time);
  }
  
  for(int i=0; i<4; i++){
    char s[] = " ";
    s[0] = time_text[i];
    strcpy(grid[i], s);
    text_layer_set_text(time_layer[i], grid[i]);
  }
}

void draw_date(){
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  
  strftime(weekday_text, sizeof(weekday_text), "%A   ", tick_time);
  for(int i=0; i<(int)(strlen(weekday_text)); i++){
	if(weekday_text[i]>96)weekday_text[i]-=32;
  }
    
  for(int i=0; i<4; i++){
    char s[] = " ";
    s[0] = weekday_text[i];
    strcpy(grid[4+i], s);
    text_layer_set_text(time_layer[4+i], grid[4+i]);
  }
    
  strftime(day_text, sizeof(day_text), "%dTH", tick_time);
  if(day_text[1]=='1'&&day_text[0]!='1'){
    day_text[2]='S';
    day_text[3]='T';
  }else if(day_text[1]=='2'&&day_text[0]!='1'){
    day_text[2]='N';
    day_text[3]='D';
  }else if(day_text[1]=='3'&&day_text[0]!='1'){
    day_text[2]='R';
    day_text[3]='D';
  }else{
    day_text[2]='T';
    day_text[3]='H';
  }
    
  for(int i=0; i<4; i++){
    char s[] = " ";
    s[0] = day_text[i];
    strcpy(grid[8+i], s);
    text_layer_set_text(time_layer[8+i], grid[8+i]);
  }
 
  strftime(month_text, sizeof(month_text), "%B   ", tick_time);
  for(int i=0; i<(int)(strlen(month_text)); i++){
	if(month_text[i]>96)month_text[i]-=32;
  }
    
  for(int i=0; i<4; i++){
    char s[] = " ";
    s[0] = month_text[i];
    strcpy(grid[12+i], s);
    text_layer_set_text(time_layer[12+i], grid[12+i]);
  }
  
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  // Update the display based on new settings
  prv_update_display();
}

static void set_container_image(GBitmap **bmp_image, BitmapLayer *bmp_layer, const int resource_id, GPoint origin) {
  gbitmap_destroy(*bmp_image);
  *bmp_image = gbitmap_create_with_resource(resource_id);
  GRect bounds = gbitmap_get_bounds(*bmp_image);
  GRect frame = (GRect) {
    .origin = origin,
    .size = bounds.size
  };
  bitmap_layer_set_bitmap(bmp_layer, *bmp_image);
  layer_set_frame(bitmap_layer_get_layer(bmp_layer), frame);
}


void weather_set_icon(WeatherIcon icon) {
  set_container_image(&s_weather_icon, s_weather_icon_layer, WEATHER_ICONS[icon], GPoint(0,88));
    APP_LOG(APP_LOG_LEVEL_DEBUG, "icon: %d", settings.WeatherIcon);
}


void weather_set_temperature(int16_t t) {
  if (t == 999) {
    snprintf(s_temperature_text, sizeof(s_temperature_text), "%s", "NA");
//    snprintf(s_temperature_text, sizeof(s_temperature_text), "%s\u00B0", "???");
  } else {
    snprintf(s_temperature_text, sizeof(s_temperature_text), "%d", t);
//    snprintf(s_temperature_text, sizeof(s_temperature_text), "%d\u00B0", t);
  }
  text_layer_set_text(s_temperature_layer, s_temperature_text);
}

void weather_set_loading() {

  weather_set_icon(48);
  weather_set_temperature(999);
}

// Update the display elements
static void prv_update_display() {
		
switch (settings.row2) {
			
case 0: // dayname

  layer_set_hidden(text_layer_get_layer(layer_week_text), true);
  layer_set_hidden(text_layer_get_layer(battery_text_layer), true);
	
  layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), true); 
  layer_set_hidden(bitmap_layer_get_layer(wk_layer), true); 
	
	break;
case 1:  // week
  layer_set_hidden(text_layer_get_layer(battery_text_layer), true);
  layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), true); 
	
  layer_set_hidden(text_layer_get_layer(layer_week_text), false);
  layer_set_hidden(bitmap_layer_get_layer(wk_layer), false); 
		break;

case 2:  //batt

  layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), false); 
  layer_set_hidden(text_layer_get_layer(battery_text_layer), false);
	
  layer_set_hidden(bitmap_layer_get_layer(wk_layer), true); 
  layer_set_hidden(text_layer_get_layer(layer_week_text), true);
	
break;
}
	
switch (settings.row3) {
			
case 0: // daynum
  layer_set_hidden(bitmap_layer_get_layer(s_weather_icon_layer), true); 
  layer_set_hidden(text_layer_get_layer(s_temperature_layer), true);
break;
		
case 1:  // weather
    layer_set_hidden(bitmap_layer_get_layer(s_weather_icon_layer), false); 
	layer_set_hidden(text_layer_get_layer(s_temperature_layer), false);
break;

}
}

// Current heart rate
static void get_hr() {
  s_hr = (int)health_service_peek_current_value(HealthMetricHeartRateBPM);
}

static void display_heart_rate() {
  if(s_hr>0) {
    snprintf(s_current_hr_buffer,sizeof(s_current_hr_buffer),"%d",s_hr);
    text_layer_set_text(s_hrm_layer,s_current_hr_buffer);
    layer_set_hidden(bitmap_layer_get_layer(heart_layer),false);
    layer_set_hidden(text_layer_get_layer(s_hrm_layer),false);

  } else {
    layer_set_hidden(bitmap_layer_get_layer(heart_layer),true);
    layer_set_hidden(text_layer_get_layer(s_hrm_layer),true);

  }
}

static void health_handler(HealthEventType event, void *context) {
 
  if(event != HealthEventSleepUpdate) {
    get_hr();
    display_heart_rate(); 
	  
  }
}

int stringToInt(char *str){
    int i=0,sum=0;
    while(str[i]!='\0'){
         if(str[i]< 48 || str[i] > 57){
            // if (DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to convert it into integer.");
          //   return 0;
         }
         else{
             sum = sum*10 + (str[i] - 48);
             i++;
         }
    }
    return sum;
}

static void fetch_data(void) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter == NULL) {
    return;
  }
  app_message_outbox_send();
}

// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {

 Tuple *ready_tuple = dict_find(iter, MESSAGE_KEY_JSReady);
  if(ready_tuple) {
    fetch_data();
    return;
  }
	
	// Bluetoothvibe
  Tuple *animations_t = dict_find(iter, MESSAGE_KEY_bluetoothvibe);
  if (animations_t) {
    settings.bluetoothvibe = animations_t->value->int32 == 1;
  }

  // hourlyvibe
  Tuple *hourlyvibe_t = dict_find(iter, MESSAGE_KEY_hourlyvibe);
  if (hourlyvibe_t) {
    settings.hourlyvibe = hourlyvibe_t->value->int32 == 1;
  }

  // row2
  Tuple *language_t = dict_find(iter, MESSAGE_KEY_row2);
  if (language_t) {
     settings.row2 = stringToInt( (char*) language_t->value->data );
  }

  // row2
  Tuple *row3_t = dict_find(iter, MESSAGE_KEY_row3);
  if (row3_t) {
     settings.row3 = stringToInt( (char*) row3_t->value->data );
  }
	
 // Weather Is Fahrenheit
  Tuple *weather_is_fahrenheit_t = dict_find(iter, MESSAGE_KEY_WeatherIsFahrenheit);
  if (weather_is_fahrenheit_t) {
    bool WeatherIsFahrenheit = weather_is_fahrenheit_t->value->int32 == 1;
    if (settings.WeatherIsFahrenheit != WeatherIsFahrenheit) {
      settings.WeatherIsFahrenheit = WeatherIsFahrenheit;
      fetch_data();
      return;
    }
  }
	
 Tuple *weather_temperature_tuple = dict_find(iter, MESSAGE_KEY_WeatherTemperature);
  if (weather_temperature_tuple) {
    settings.WeatherTemperature = weather_temperature_tuple->value->int16;
    weather_set_temperature(settings.WeatherTemperature);
  }

  Tuple *weather_icon_tuple = dict_find(iter, MESSAGE_KEY_WeatherIcon);
  if (weather_icon_tuple) {
    settings.WeatherIcon = weather_icon_tuple->value->uint8;
   	  weather_set_icon(settings.WeatherIcon);
  }
	
  	
// Save the new settings to persistent storage
  prv_save_settings();
}

static void toggle_bluetooth_icon(bool connected) {

	if(!connected && settings.bluetoothvibe) {
    //vibe!
    vibes_long_pulse();  
  }
}

void bluetooth_connection_callback(bool connected) {
  toggle_bluetooth_icon(connected);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  

 if (units_changed & HOUR_UNIT) {
  if(settings.hourlyvibe) {
    //vibe!
    vibes_short_pulse();
  }
 }
	
if (units_changed & MINUTE_UNIT) {

    if (FREQUENCY_MINUTES == s_timer_minute) {
      fetch_data();
      s_timer_minute = 0;
    } else {
      s_timer_minute++;
    }
  }
	
	static char week_text[] = "00";
   strftime(week_text, sizeof(week_text), "%V", tick_time);
   text_layer_set_text(layer_week_text, week_text);
	
#ifdef PBL_PLATFORM_CHALK
	
	// commented out for testing
	
//	static char day_text[] = "WEDNESDAY";
//    strftime(day_text, sizeof(day_text), "%A", tick_time);
//    text_layer_set_text(layer_day_text, day_text);
	
//	static char month_text[] = "SEPTEMBER";
//    strftime(month_text, sizeof(month_text), "%B", tick_time);
//    text_layer_set_text(layer_month_text, month_text);	

	static char *time_format;
	static char time_text[] = "00";
	
	if (clock_is_24h_style()) {
        time_format = "%H";	
    } else {
        time_format = "%I";
    }
	strftime(time_text, sizeof(time_text), time_format, tick_time);
    text_layer_set_text(layer_time_text, time_text);
	
	
	static char *time_format2;
	static char time_text2[] = "00";
	
    time_format2 = "%M";	

	strftime(time_text2, sizeof(time_text2), time_format2, tick_time);
    text_layer_set_text(layer_time2_text, time_text2);	
	
	
	
    static char date_text[] = "00XX";	
	int new_cur_day = tick_time->tm_year*1000 + tick_time->tm_yday;

	if (new_cur_day != cur_day) {
        cur_day = new_cur_day;

	switch(tick_time->tm_mday)
    {
    case 1 :
    case 21 :
    case 31 :
      strftime(date_text, sizeof(date_text), "%dst", tick_time);
      break;
    case 2 :
    case 22 :
      strftime(date_text, sizeof(date_text), "%dnd", tick_time);
      break;
    case 3 :
    case 23 :
      strftime(date_text, sizeof(date_text), "%drd", tick_time);
      break;
    default :
      strftime(date_text, sizeof(date_text), "%dth", tick_time);
      break;
    }
	  text_layer_set_text(layer_date_text, date_text);
  }	
	
#else
		draw_time();
#endif
}

static void init() {
	
  prv_load_settings();
	
  // Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
	
  backgroundcolor = GColorBlack;
  foregroundcolor = GColorWhite;
  window = window_create();
  window_stack_push(window, true);
  window_set_background_color(window, backgroundcolor);

  Layer* window_layer = window_get_root_layer(window);

  time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITAL_50));
  time_font2 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HR_50));
  time_font3 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITALROUND_50));
  time_font4 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DIGITALROUND_29));
	
  img_battery_100   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_100);
  img_battery_80   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_80);
  img_battery_60   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_60);
  img_battery_40   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_40);
  img_battery_20    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_20);
  img_battery_00    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_00);

#ifdef PBL_PLATFORM_CHALK
	
// time - hours
  layer_time_text = text_layer_create( GRect(0, 9, 120, 51));
  text_layer_set_font(layer_time_text, time_font3);
  text_layer_set_text_alignment(layer_time_text, GTextAlignmentCenter);
  text_layer_set_text_color(layer_time_text, GColorWhite);		
  text_layer_set_background_color(layer_time_text, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(layer_time_text));

// time - mins
  layer_time2_text = text_layer_create( GRect(61, 9, 126, 51));
  text_layer_set_font(layer_time2_text, time_font3);
  text_layer_set_text_alignment(layer_time2_text, GTextAlignmentCenter);
  text_layer_set_text_color(layer_time2_text, GColorWhite);		
  text_layer_set_background_color(layer_time2_text, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(layer_time2_text));
	
// day name
  layer_day_text = text_layer_create( GRect(0, 57, 180, 51));
  text_layer_set_font(layer_day_text, time_font4);
  text_layer_set_text_alignment(layer_day_text, GTextAlignmentCenter);
  text_layer_set_text_color(layer_day_text, GColorWhite);		
  text_layer_set_background_color(layer_day_text, GColorClear);
  text_layer_set_text(layer_day_text, "WEDNESDAY");   // inserted for testing
  layer_add_child(window_layer, text_layer_get_layer(layer_day_text));
		

// month name
  layer_month_text = text_layer_create( GRect(0, 83, 180, 51));
  text_layer_set_font(layer_month_text, time_font4);
  text_layer_set_text_alignment(layer_month_text, GTextAlignmentCenter);
  text_layer_set_text_color(layer_month_text, GColorWhite);		
  text_layer_set_background_color(layer_month_text, GColorClear);
  text_layer_set_text(layer_month_text, "SEPTEMBER");   // inserted for testing
  layer_add_child(window_layer, text_layer_get_layer(layer_month_text));
	
// day number
  layer_date_text = text_layer_create( GRect(0, 104, 186, 51));
  text_layer_set_font(layer_date_text, time_font3);
  text_layer_set_text_alignment(layer_date_text, GTextAlignmentCenter);
  text_layer_set_text_color(layer_date_text, GColorWhite);		
  text_layer_set_background_color(layer_date_text, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(layer_date_text));
	
#else
	
  for(int i = 0; i<4; i++){
    for(int j = 0; j<4; j++){
      time_layer[i*4+j] = text_layer_create(GRect(j*36-4, i*42-8, 44, 52));
      text_layer_set_text_color(time_layer[i*4+j], foregroundcolor);
      text_layer_set_font(time_layer[i*4+j], time_font);
      text_layer_set_text_alignment(time_layer[i*4+j], GTextAlignmentCenter);
      text_layer_set_background_color(time_layer[i*4+j], GColorClear);
      layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer[i*4+j]));
    }
  }
  for(int i=0; i<16; i++){
    strcpy(grid[i], " ");
  }
  
  draw_time();
  draw_date();
	
#endif
	
// wk image
  wk_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WK);
#ifdef PBL_PLATFORM_CHALK
  GRect wkframe = GRect(0, 40, 144, 40);
#else
  GRect wkframe = GRect(0, 40, 144, 40);
#endif
  wk_layer = bitmap_layer_create(wkframe);
  bitmap_layer_set_bitmap(wk_layer, wk_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(wk_layer));   
  layer_set_hidden(bitmap_layer_get_layer(wk_layer), true); 
	
  layer_week_text = text_layer_create(GRect(0, 34, 150, 51));
  text_layer_set_text_color(layer_week_text, GColorWhite );	
  text_layer_set_background_color(layer_week_text, GColorClear);
  text_layer_set_font(layer_week_text, time_font2);
  text_layer_set_text_alignment(layer_week_text, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(layer_week_text));	
  layer_set_hidden(text_layer_get_layer(layer_week_text), true);

// batt
#ifdef PBL_PLATFORM_CHALK
  layer_batt_img  = bitmap_layer_create(GRect(0, 55, 180, 40));
#else
  layer_batt_img  = bitmap_layer_create(GRect(0, 40, 144, 40));
#endif
  bitmap_layer_set_bitmap(layer_batt_img, img_battery_100);
  layer_add_child(window_layer, bitmap_layer_get_layer(layer_batt_img));
  layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), true); 

#ifdef PBL_PLATFORM_CHALK
  battery_text_layer = text_layer_create(GRect(0, 61, 152, 51));
  text_layer_set_font(battery_text_layer, time_font4);
#else
  battery_text_layer = text_layer_create(GRect(0, 34, 150, 51));
  text_layer_set_font(battery_text_layer, time_font2);
#endif
  text_layer_set_text_color(battery_text_layer, GColorWhite);
  text_layer_set_background_color(battery_text_layer, GColorClear);
  text_layer_set_text_alignment(battery_text_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(battery_text_layer));		
  layer_set_hidden(text_layer_get_layer(battery_text_layer), true);
	
// heart image
  heart_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HEART);
  GRect hrbitmap_bounds = gbitmap_get_bounds(heart_image);	
  GRect heartframe = GRect(0, 130, hrbitmap_bounds.size.w, hrbitmap_bounds.size.h);
  heart_layer = bitmap_layer_create(heartframe);
  bitmap_layer_set_bitmap(heart_layer, heart_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(heart_layer));   
	
// Create a layer to hold the current heart rate
  s_hrm_layer = text_layer_create(GRect(0, 118, 150, 51));
  text_layer_set_background_color(s_hrm_layer, GColorClear);
  text_layer_set_text_color(s_hrm_layer, GColorWhite);
  text_layer_set_font(s_hrm_layer,time_font2);
  text_layer_set_text_alignment(s_hrm_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(s_hrm_layer));

// WEATHER ICON //
#ifdef PBL_PLATFORM_CHALK
  s_weather_icon_layer = bitmap_layer_create(GRect(0,85,180,32));
#else
  s_weather_icon_layer = bitmap_layer_create(GRect(0,84,144,40));
#endif
  layer_add_child(window_layer, bitmap_layer_get_layer(s_weather_icon_layer));

// TEMPERATURE //
#ifdef PBL_PLATFORM_CHALK
  s_temperature_layer = text_layer_create(GRect(0, 84, 152, 51));
  text_layer_set_font(s_temperature_layer, time_font4); 
#else
  s_temperature_layer = text_layer_create(GRect(0, 76, 150, 51));
  text_layer_set_font(s_temperature_layer, time_font2); 
#endif
  text_layer_set_background_color(s_temperature_layer, GColorClear);
  text_layer_set_text_color(s_temperature_layer, GColorWhite);
//  text_layer_set_font(s_temperature_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(s_temperature_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(s_temperature_layer));
  layer_set_hidden(text_layer_get_layer(s_temperature_layer), true);

  weather_set_loading();
	

  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);

  s_initial_minute = (tick_time->tm_min % FREQUENCY_MINUTES);

  handle_minute_tick(tick_time, DAY_UNIT + HOUR_UNIT + MINUTE_UNIT );
		
  toggle_bluetooth_icon(bluetooth_connection_service_peek());
  update_battery_state(battery_state_service_peek());
	
  prv_update_display();

  health_service_events_subscribe(health_handler, NULL);

// force initial display
  health_handler(HealthEventMovementUpdate, NULL);
	
  battery_state_service_subscribe(&update_battery_state);
  bluetooth_connection_service_subscribe(&toggle_bluetooth_icon);
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);

}

static void deinit() {
  health_service_events_unsubscribe();
  tick_timer_service_unsubscribe();

  text_layer_destroy(s_hrm_layer);
  text_layer_destroy(layer_week_text);
  text_layer_destroy(battery_text_layer);
  text_layer_destroy(s_temperature_layer);
	
  layer_remove_from_parent(bitmap_layer_get_layer(s_weather_icon_layer));
  bitmap_layer_destroy(s_weather_icon_layer);
  gbitmap_destroy(s_weather_icon);	
	
  layer_remove_from_parent(bitmap_layer_get_layer(heart_layer));
  bitmap_layer_destroy(heart_layer);
  gbitmap_destroy(heart_image);	
	
  layer_remove_from_parent(bitmap_layer_get_layer(wk_layer));
  bitmap_layer_destroy(wk_layer);
  gbitmap_destroy(wk_image);	
	
  for(int i = 0; i<16; i++){
    text_layer_destroy(time_layer[i]);
  }
 
  layer_remove_from_parent(bitmap_layer_get_layer(layer_batt_img));
  bitmap_layer_destroy(layer_batt_img);
	
  gbitmap_destroy(img_battery_100);
  gbitmap_destroy(img_battery_80);
  gbitmap_destroy(img_battery_60);
  gbitmap_destroy(img_battery_40);
  gbitmap_destroy(img_battery_20);
  gbitmap_destroy(img_battery_00);	
	
  fonts_unload_custom_font(time_font);
  fonts_unload_custom_font(time_font2);

  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}   