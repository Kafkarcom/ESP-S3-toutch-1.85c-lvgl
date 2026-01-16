#include "lvgl_ui.h"
#include "lvgl.h"
#include <stdio.h>

static int color_index = 0;
static lv_obj_t *main_screen;
static lv_obj_t *settings_screen;

// --- Callback Functions ---

static void btn_event_cb(lv_event_t *e) {
  lv_obj_t *btn = lv_event_get_target(e);
  color_index = (color_index + 1) % 3;
  if (color_index == 0) {
    lv_obj_set_style_bg_color(btn, lv_color_make(255, 0, 0), 0); // Red
  } else if (color_index == 1) {
    lv_obj_set_style_bg_color(btn, lv_color_make(0, 255, 0), 0); // Green
  } else {
    lv_obj_set_style_bg_color(btn, lv_color_make(0, 0, 255), 0); // Blue
  }
}

static void to_settings_cb(lv_event_t *e) { lv_screen_load(settings_screen); }

static void to_main_cb(lv_event_t *e) { lv_screen_load(main_screen); }

// --- Init Function ---

void lvgl_ui_init(void) {
  // 1. Create Screens
  main_screen = lv_obj_create(NULL);
  settings_screen = lv_obj_create(NULL);

  // --- MAIN SCREEN SETUP ---

  // Set background to black
  lv_obj_set_style_bg_color(main_screen, lv_color_make(0, 0, 0), 0);
  lv_obj_set_style_bg_opa(main_screen, LV_OPA_COVER, 0);

  // Create a label
  lv_obj_t *label = lv_label_create(main_screen);
  lv_label_set_text(label, "Hello World");

  // Set text color to white
  lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), 0);

  // Make text bigger using transform scale (1.5x)
  lv_obj_set_style_transform_scale(label, 384, 0);

  // Align label
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -40);

  // Create a button
  lv_obj_t *btn = lv_btn_create(main_screen);
  lv_obj_set_size(btn, 120, 50);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 60);
  lv_obj_set_style_bg_color(btn, lv_color_make(255, 0, 0), 0); // Initial red
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

  // Add label to button
  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Change Color");
  lv_obj_center(btn_label);

  // Create Settings Button (Bottom Center)
  lv_obj_t *settings_btn = lv_btn_create(main_screen);
  lv_obj_set_size(settings_btn, 50, 50);
  lv_obj_align(settings_btn, LV_ALIGN_BOTTOM_MID, 0, -20);
  lv_obj_add_event_cb(settings_btn, to_settings_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t *settings_label = lv_label_create(settings_btn);
  lv_label_set_text(settings_label, LV_SYMBOL_SETTINGS);
  lv_obj_center(settings_label);

  // --- SETTINGS SCREEN SETUP ---

  // Set background to black
  lv_obj_set_style_bg_color(settings_screen, lv_color_make(0, 0, 0), 0);
  lv_obj_set_style_bg_opa(settings_screen, LV_OPA_COVER, 0);

  // Title Label
  lv_obj_t *title_label = lv_label_create(settings_screen);
  lv_label_set_text(title_label, "Settings");
  lv_obj_set_style_text_color(title_label, lv_color_make(255, 255, 255), 0);
  lv_obj_set_style_transform_scale(title_label, 384, 0); // 1.5x scale
  lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

  // Close Button (Bottom Center)
  lv_obj_t *close_btn = lv_btn_create(settings_screen);
  lv_obj_set_size(close_btn, 50, 50);
  lv_obj_align(close_btn, LV_ALIGN_BOTTOM_MID, 0, -20);
  lv_obj_add_event_cb(close_btn, to_main_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t *close_label = lv_label_create(close_btn);
  lv_label_set_text(close_label, LV_SYMBOL_CLOSE);
  lv_obj_center(close_label);

  // --- LOAD START SCREEEN ---
  lv_screen_load(main_screen);
}
