#include "lvgl_ui.h"
#include "lvgl.h"
#include <stdio.h>

static int color_index = 0;

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

void lvgl_ui_init(void) {
  // Create a label
  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello World");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  // Create a button
  lv_obj_t *btn = lv_btn_create(lv_screen_active());
  lv_obj_set_size(btn, 120, 50);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 60);
  lv_obj_set_style_bg_color(btn, lv_color_make(255, 0, 0), 0); // Initial red
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

  // Add label to button
  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Change Color");
  lv_obj_center(btn_label);
}
