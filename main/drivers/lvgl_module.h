#pragma once

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_touch.h"

/**
 * @brief Initialize LVGL, create a display and touch, and start the UI
 *
 * @param io_handle   LCD panel IO handle
 * @param panel_handle LCD panel handle
 * @param touch_handle Touch controller handle
 */
void lvgl_module_init(esp_lcd_panel_io_handle_t io_handle,
                      esp_lcd_panel_handle_t panel_handle,
                      esp_lcd_touch_handle_t touch_handle);
