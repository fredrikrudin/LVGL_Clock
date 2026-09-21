#include "ui_manager.h"
#include "time_manager.h"
#include "hardware_manager.h"
#include <stdio.h>

namespace UIManager {
    static lv_obj_t* main_scr = nullptr;
    static lv_obj_t* settings_scr = nullptr;
    
    static lv_obj_t* time_label = nullptr;
    static lv_obj_t* bat_label = nullptr; 
    static lv_obj_t* hour_roller = nullptr;
    static lv_obj_t* min_roller = nullptr;

    static void open_settings_cb(lv_event_t* e) { showSettingsScreen(); }
    static void save_settings_cb(lv_event_t* e) {
        int h = lv_roller_get_selected(hour_roller);
        int m = lv_roller_get_selected(min_roller);
        TimeManager::setTime(h, m, 0); 
        showMainScreen();
    }

    void init() {
        // Huvudskärm (Svart design)
        main_scr = lv_obj_create(NULL);
        lv_obj_set_style_bg_color(main_scr, lv_color_black(), LV_PART_MAIN);

        // Tidstext (Vit färg, stor storlek)
        time_label = lv_label_create(main_scr);
        lv_obj_set_style_text_color(time_label, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, LV_PART_MAIN); 
        lv_obj_align(time_label, LV_ALIGN_CENTER, 0, -20);

        // Batteriindikator 
        bat_label = lv_label_create(main_scr);
        lv_obj_set_style_text_color(bat_label, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_text_font(bat_label, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_align(bat_label, LV_ALIGN_TOP_RIGHT, -40, 40); 

        // Inställningsknapp
        lv_obj_t* btn_settings = lv_btn_create(main_scr);
        lv_obj_align(btn_settings, LV_ALIGN_BOTTOM_CENTER, 0, -40);
        lv_obj_set_style_bg_color(btn_settings, lv_color_make(40, 40, 40), LV_PART_MAIN);
        lv_obj_add_event_cb(btn_settings, open_settings_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t* btn_label = lv_label_create(btn_settings);
        lv_label_set_text(btn_label, "Inställningar");
        lv_obj_set_style_text_color(btn_label, lv_color_white(), LV_PART_MAIN);

        // Inställningsskärm (Svart design)
        settings_scr = lv_obj_create(NULL);
        lv_obj_set_style_bg_color(settings_scr, lv_color_black(), LV_PART_MAIN);

        // Rullister (Rollers) för tid
        hour_roller = lv_roller_create(settings_scr);
        lv_roller_set_options(hour_roller, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23", LV_ROLLER_MODE_NORMAL);
        lv_obj_align(hour_roller, LV_ALIGN_CENTER, -60, -20);
        lv_obj_set_style_bg_color(hour_roller, lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_text_color(hour_roller, lv_color_white(), LV_PART_MAIN);

        min_roller = lv_roller_create(settings_scr);
        lv_roller_set_options(min_roller, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59", LV_ROLLER_MODE_NORMAL);
        lv_obj_align(min_roller, LV_ALIGN_CENTER, 60, -20);
        lv_obj_set_style_bg_color(min_roller, lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_text_color(min_roller, lv_color_white(), LV_PART_MAIN);

        // Spara-knapp
        lv_obj_t* btn_save = lv_btn_create(settings_scr);
        lv_obj_align(btn_save, LV_ALIGN_BOTTOM_CENTER, 0, -40);
        lv_obj_set_style_bg_color(btn_save, lv_color_make(0, 150, 0), LV_PART_MAIN); 
        lv_obj_add_event_cb(btn_save, save_settings_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t* save_label = lv_label_create(btn_save);
        lv_label_set_text(save_label, "Spara");
        lv_obj_set_style_text_color(save_label, lv_color_white(), LV_PART_MAIN);

        showMainScreen();
    }

    void showMainScreen() {
        lv_scr_load(main_scr);
    }

    void showSettingsScreen() {
        lv_roller_set_selected(hour_roller, TimeManager::getHour(), LV_ANIM_OFF);
        lv_roller_set_selected(min_roller, TimeManager::getMinute(), LV_ANIM_OFF);
        lv_scr_load(settings_scr);
    }

    void update() {
        if (lv_scr_act() == main_scr) {
            if (time_label != nullptr) {
                char time_buf[16];
                snprintf(time_buf, sizeof(time_buf), "%02d:%02d:%02d", 
                         TimeManager::getHour(), 
                         TimeManager::getMinute(), 
                         TimeManager::getSecond());
                lv_label_set_text(time_label, time_buf);
            }

            if (bat_label != nullptr) {
                char bat_buf[16];
                snprintf(bat_buf, sizeof(bat_buf), "BAT: %d%%", HardwareManager::getLipoPercentage());
                lv_label_set_text(bat_label, bat_buf);
            }
        }
    }
}
