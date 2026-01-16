#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>


#include "display.h"       // Display functions
#include "esp_lcd_touch.h" // Defines esp_lcd_touch_point_t
#include "extendio.h"      // Extended IO functions
#include "i2c_utils.h"     // I2C utilities
#include "touch.h"         // Touch functions


#include "esp_lvgl_port.h" // LVGL port for ESP-IDF
#include "lvgl.h"          // LVGL library

static const char *TAG = "Touch 1.85 sampole";

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

void app_main(void) {
  // --- 1. INITIALIZE I2C ---
  i2c_system_handles_t i2c_handles;
  ESP_ERROR_CHECK(i2c_system_init(&i2c_handles));

  // --- 2. HARDWARE RESET VIA TCA9554 ---
  reset_lcd_via_tca9554(i2c_handles.tca_handle);

  // --- 3. CONFIGURE LCD PANEL ---
  esp_lcd_panel_handle_t panel_handle = NULL;
  esp_lcd_panel_io_handle_t io_handle = NULL;
  ESP_ERROR_CHECK(display_init(&panel_handle, &io_handle));

  ESP_LOGI(TAG, "Heartbeat: ST77916 360x360 is active!");

  // --- 4. INITIALIZE LVGL ---
  const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
  ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

  const lvgl_port_display_cfg_t disp_cfg = {
      .io_handle = io_handle,
      .panel_handle = panel_handle,
      .buffer_size = LCD_H_RES * LCD_V_RES * 2 /
                     10, // 1/10th of screen size in bytes for partial rendering
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

  // --- 2. SELECTIVE TOUCH RESET VIA TCA9554 ---
  reset_touch_via_tca9554(i2c_handles.tca_handle);
  esp_lcd_touch_handle_t touch_handle;

  // Initialize touch sensor
  ESP_ERROR_CHECK(touch_init(i2c_handles.bus_handle, &touch_handle));

  // --- After LCD/Touch Initialization is complete ---

  // Register touch with LVGL
  const lvgl_port_touch_cfg_t touch_cfg = {
      .disp = disp,
      .handle = touch_handle,
  };
  lvgl_port_add_touch(&touch_cfg);

  ESP_LOGI(TAG, "System Ready. LVGL UI is running!");

  // Create LVGL task

  // Keep the main task alive
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}