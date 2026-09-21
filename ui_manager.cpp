#include "ui_manager.h"
#include "time_manager.h"
#include <stdio.h>

namespace UIManager {
    // Skärmobjekt
    static lv_obj_t* main_scr = nullptr;
    static lv_obj_t* settings_scr = nullptr;
    
    // Gränssnittskomponenter
    static lv_obj_t* time_label = nullptr;
    static lv_obj_t* hour_roller = nullptr;
    static lv_obj_t* min_roller = nullptr;

    // Event-callbacks för navigering och sparande
    static void open_settings_cb(lv_event_t* e) { showSettingsScreen(); }
    static void save_settings_cb(lv_event_t* e) {
        int h = lv_roller_get_selected(hour_roller);
        int m = lv_roller_get_selected(min_roller);
        TimeManager::setTime(h, m, 0);
        showMainScreen();
    }

    void applyDarkTheme() {
        // Sätt bakgrundsfärg på den aktiva skärmen till svart
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN);
    }

    void init() {
        // Skapa huvudskärmen (Svart design)
        main_scr = lv_obj_create(NULL);
        lv_obj_set_style_bg_color(main_scr, lv_color_black(), LV_PART_MAIN);

        // Tidstext (Vit färg, stor storlek)
        time_label = lv_label_create(main_scr);
        lv_obj_set_style_text_color(time_label, lv_color_white(), LV_PART_MAIN);
        // Tips: Öka fontstorlek i lv_conf.h (t.ex. LV_FONT_MONO_NUM_ARRAY eller LV_FONT_DEJAVU_24/48)
        lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, LV_PART_MAIN); 
        lv_obj_align(time_label, LV_ALIGN_CENTER, 0, -20);

        // Inställningsknapp på huvudskärmen
        lv_obj_t* btn_settings = lv_btn_create(main_scr);
        lv_obj_align(btn_settings, LV_ALIGN_BOTTOM_CENTER, 0, -40);
        lv_obj_set_style_bg_color(btn_settings, lv_color_make(40, 40, 40), LV_PART_MAIN); // Mörkgrå knapp
        lv_obj_add_event_cb(btn_settings, open_settings_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t* btn_label = lv_label_create(btn_settings);
        lv_label_set_text(btn_label, "Installningar");
        lv_obj_set_style_text_color(btn_label, lv_color_white(), LV_PART_MAIN);

        // Skapa inställningsskärmen (Svart design)
        settings_scr = lv_obj_create(NULL);
        lv_obj_set_style_bg_color(settings_scr, lv_color_black(), LV_PART_MAIN);

        // Skapa rullister (Rollers) för att ställa in tid
        hour_roller = lv_roller_create(settings_scr);
        lv_roller_set_options(hour_roller, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23", LV_ROLLER_MODE_NORMAL);
        lv_obj_align(hour_roller, LV_ALIGN_CENTER, -60, -20);
        lv_obj_set_style_bg_color(hour_roller, lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_text_color(hour_roller, lv_color_white(), LV_PART_MAIN);

        min_roller = lv_roller_create(settings_scr);
        lv_roller_set_options(min_roller, "00\n05\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55", LV_ROLLER_MODE_NORMAL); // Går att expandera till 00-59
        lv_obj_align(min_roller, LV_ALIGN_CENTER, 60, -20);
        lv_obj_set_style_bg_color(min_roller, lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_text_color(min_roller, lv_color_white(), LV_PART_MAIN);

        // Spara-knapp
        lv_obj_t* btn_save = lv_btn_create(settings_scr);
        lv_obj_align(btn_save, LV_ALIGN_BOTTOM_CENTER, 0, -40);
        lv_obj_set_style_bg_color(btn_save, lv_color_make(0, 150, 0), LV_PART_MAIN); // Grön sparaknapp
        lv_obj_add_event_cb(btn_save, save_settings_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t* save_label = lv_label_create(btn_save);
        lv_label_set_text(save_label, "Spara");
        lv_obj_set_style_text_color(save_label, lv_color_white(), LV_PART_MAIN);

        // Visa klockan direkt vid start
        showMainScreen();
    }

    void showMainScreen() {
        lv_scr_load(main_scr);
    }

    void showSettingsScreen() {
        // Uppdatera rullisterna till den nuvarande tiden innan skärmen laddas
        lv_roller_set_selected(hour_roller, TimeManager::getHour(), LV_ANIM_OFF);
        lv_roller_set_selected(min_roller, TimeManager::getMinute() / 5, LV_ANIM_OFF); // Matchar 5-minutersintervallet ovan
        lv_scr_load(settings_scr);
    }

    void update() {
        // Uppdatera bara texten om huvudskärmen faktiskt visas just nu
        if (lv_scr_act() == main_scr && time_label != nullptr) {
            char buf[16];
            snprintf(buf, sizeof(buf), "%02d:%02d:%02d", 
                     TimeManager::getHour(), 
                     TimeManager::getMinute(), 
                     TimeManager::getSecond());
            lv_label_set_text(time_label, buf);
        }
    }
}
