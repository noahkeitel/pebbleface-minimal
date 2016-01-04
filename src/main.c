#include <pebble.h>
static Window *s_main_window;
static Layer *s_simple_bg_layer, *s_date_layer;
static TextLayer *s_time_layer, *s_bg_layer, *s_day_label, *s_num_label;
static char s_num_buffer[4], s_day_buffer[6], s_time_buffer[8];


static void update_time() {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", t);
  text_layer_set_text(s_time_layer, s_time_buffer);
  
  strftime(s_day_buffer, sizeof(s_day_buffer), "%b%e", t);
  text_layer_set_text(s_day_label, s_day_buffer);

}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // BACKGROUND
  s_bg_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(0, 0), bounds.size.w, bounds.size.h)
  );
  text_layer_set_background_color(s_bg_layer, GColorDarkGreen );
  
  // TIME
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, bounds.size.h)
  );
  text_layer_set_text(s_time_layer, s_time_buffer);
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // DATE
  s_day_label = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(30, 24), bounds.size.w, bounds.size.h)
  );
  text_layer_set_text(s_day_label, s_day_buffer);
  text_layer_set_background_color(s_day_label, GColorClear);
  text_layer_set_text_color(s_day_label, GColorWhite);
  text_layer_set_font(s_day_label, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21 ));
  text_layer_set_text_alignment(s_day_label, GTextAlignmentCenter);

  
  layer_add_child(window_layer, text_layer_get_layer(s_bg_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_day_label));
}

static void main_window_unload(Window *window) {
  layer_destroy(s_simple_bg_layer);
  layer_destroy(s_date_layer);
  
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_bg_layer);
  text_layer_destroy(s_day_label);
  text_layer_destroy(s_num_label);
}



static void init() {
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);
  
  s_day_buffer[0] = '\0';
  s_num_buffer[0] = '\0';
  
}

static void deinit() {
  window_destroy(s_main_window);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}


int main(void) {
  init();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  app_event_loop();
  deinit();
}
