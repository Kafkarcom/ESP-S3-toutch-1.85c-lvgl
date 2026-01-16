#include "lvgl_module.h"
#include "display.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"


static const char *TAG = "LVGL_MODULE";
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

void lvgl_module_init(esp_lcd_panel_io_handle_t io_handle,
                      esp_lcd_panel_handle_t panel_handle,
                      esp_lcd_touch_handle_t touch_handle) {
  // --- INITIALIZE LVGL ---
  const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
  ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

  const lvgl_port_display_cfg_t disp_cfg = {
      .io_handle = io_handle,
      .panel_handle = panel_handle,
      .buffer_size = LCD_H_RES * LCD_V_RES * 2 / 10, // 1/10th of screen size
      .double_buffer = true,
      .hres = LCD_H_RES,
      .vres = LCD_V_RES,
      .monochrome = false,
      .rotation =
          {
              .swap_xy = false,
              .mirror_x = false,
              .mirror_y = false,
          },
      .flags = {
          .buff_dma = true,
      }};

  lv_display_t *disp = lvgl_port_add_disp(&disp_cfg);
  if (disp == NULL) {
    ESP_LOGE(TAG, "Failed to create LVGL display");
    return;
  }

  // Register touch with LVGL
  if (touch_handle) {
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = disp,
        .handle = touch_handle,
    };
    lvgl_port_add_touch(&touch_cfg);
  }

  /* Lock the mutex due to the LVGL task running */
  if (lvgl_port_lock(0)) {
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

    // Set backlight
    set_backlight_brightness(50);

    /* Release the mutex */
    lvgl_port_unlock();
  }

  ESP_LOGI(TAG, "LVGL UI initialized");
}
