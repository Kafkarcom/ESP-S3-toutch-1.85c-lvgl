#include "lvgl_module.h"
#include "display.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "ui.h"

static const char *TAG = "LVGL_MODULE";

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
          .swap_bytes = true,
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
    // Initialize UI
    ui_init();

    // Set backlight
    set_backlight_brightness(50);

    /* Release the mutex */
    lvgl_port_unlock();
  }

  ESP_LOGI(TAG, "LVGL UI initialized");
}
