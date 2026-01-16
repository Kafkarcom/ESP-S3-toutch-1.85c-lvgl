#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#include "display.h"       // Display functions
#include "esp_lcd_touch.h" // Defines esp_lcd_touch_point_t
#include "extendio.h"      // Extended IO functions
#include "i2c_utils.h"     // I2C utilities
#include "lvgl_module.h"   // LVGL module
#include "touch.h"         // Touch functions


static const char *TAG = "Touch 1.85 sampole";

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

  // --- 4. SELECTIVE TOUCH RESET VIA TCA9554 ---
  reset_touch_via_tca9554(i2c_handles.tca_handle);
  esp_lcd_touch_handle_t touch_handle;

  // Initialize touch sensor
  ESP_ERROR_CHECK(touch_init(i2c_handles.bus_handle, &touch_handle));

  // --- 5. INITIALIZE LVGL MODULE ---
  lvgl_module_init(io_handle, panel_handle, touch_handle);

  ESP_LOGI(TAG, "System Ready. LVGL UI is running!");

  // Keep the main task alive
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}