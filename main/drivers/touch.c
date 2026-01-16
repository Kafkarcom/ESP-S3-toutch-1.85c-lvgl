#include "touch.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_touch_cst816s.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *TAG = "TOUCH";

/**
 * @brief Initialize touch sensor
 * @param bus_handle I2C bus handle
 * @param touch_handle Pointer to store the touch handle
 * @return ESP_OK on success
 */
esp_err_t touch_init(i2c_master_bus_handle_t bus_handle,
                     esp_lcd_touch_handle_t *touch_handle) {
  // Configure touch I2C IO
  esp_lcd_panel_io_i2c_config_t tp_io_cfg = {
      .dev_addr = ESP_LCD_TOUCH_IO_I2C_CST816S_ADDRESS,
      .scl_speed_hz = 100000,
      .control_phase_bytes = 1,
      .lcd_cmd_bits = 8,
      .flags.disable_control_phase = 1,
  };

  esp_lcd_panel_io_handle_t tp_io_handle;
  ESP_RETURN_ON_ERROR(
      esp_lcd_new_panel_io_i2c(bus_handle, &tp_io_cfg, &tp_io_handle), TAG,
      "Failed to create touch IO");

  // Configure touch
  esp_lcd_touch_config_t tp_cfg = {
      .x_max = 360,
      .y_max = 360,
      .rst_gpio_num = -1,
      .int_gpio_num = TOUCH_INT_IO,
  };

  ESP_RETURN_ON_ERROR(
      esp_lcd_touch_new_i2c_cst816s(tp_io_handle, &tp_cfg, touch_handle), TAG,
      "Failed to create touch handle");

  return ESP_OK;
}