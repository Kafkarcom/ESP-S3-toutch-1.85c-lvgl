idf_component_register(SRCS "main.c" "drivers/i2c_utils.c" "drivers/display.c" "drivers/touch.c" "drivers/extendio.c" "drivers/lvgl_module.c"
                    "ui/ui.c" "ui/ui_helpers.c"
                    "ui/screens/ui_Screen1.c" "ui/screens/ui_About.c" "ui/screens/ui_flash_screen.c"
                    "ui/fonts/ui_font_ArchivoBlackRegular.c" "ui/fonts/ui_font_Font1.c" "ui/fonts/ui_font_MomoTrustDisplay.c"
                    "ui/images/ui_img_kafkar_logo_no_boarder_100_white_png.c"
                    INCLUDE_DIRS "." "drivers" "ui")
