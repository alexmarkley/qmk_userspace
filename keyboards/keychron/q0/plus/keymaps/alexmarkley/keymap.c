/* Copyright 2023 @ Alex Markley <alex@alexmarkley.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// clang-format off

// An orange that looks to me a little bit like Blackmagic's brand colors.
#define HSV_BLACKMAGIC_ORANGE 5, 255, 255

enum custom_keycodes {
    // Misc keycodes. (Must start at SAFE_RANGE)
    AM_FUNC = SAFE_RANGE,

    // DaVinci Resolve Copy/Paste Buffers
    DR_CBPA, DR_CBPB, DR_CBPC, DR_CBPD, DR_CBPE,
    DR_CBPF, DR_CBPG, DR_CBPH, DR_CBPI,
    DR_CBPJ, DR_CBPK, DR_CBPL, DR_CBPM,
    DR_CBPN, DR_CBPO, DR_CBPP, DR_CBPQ,

    // DaVinci Resolve Script Triggers
    DR_SCR0, DR_SCR1, DR_SCR2,
    DR_SCR3, DR_SCR4, DR_SCR5,
    DR_SCR6, DR_SCR7, DR_SCR8,
    DR_SCR9,

};

// First and last DaVinci Resolve Copy/Paste Buffer
#define DR_CPB_MIN DR_CBPA
#define DR_CPB_MAX DR_CBPQ

// First and last Davinci Resolve Script Trigger
#define DR_SCR_MIN DR_SCR0
#define DR_SCR_MAX DR_SCR9

enum layers {
    BASE,
    FUNC,
    L2,
    L3,
    L4,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_numpad_6x5(
        KC_MUTE, AM_FUNC, TO(L2),   TO(L3),  TO(L4),
        MC_0,    KC_NUM,  KC_PSLS,  KC_PAST, KC_PMNS,
        MC_2,    KC_P7,   KC_P8,    KC_P9,   KC_PPLS,
        MC_3,    KC_P4,   KC_P5,    KC_P6,
        MC_4,    KC_P1,   KC_P2,    KC_P3,   KC_PENT,
        MC_5,    KC_P0,             KC_PDOT),

    [FUNC] = LAYOUT_numpad_6x5(
        RGB_TOG, _______,  _______, _______, _______,
        MC_1,    RGB_MOD,  RGB_VAI, RGB_HUI, KC_DEL,
        MC_2,    RGB_RMOD, RGB_VAD, RGB_HUD, _______,
        MC_3,    RGB_SAI,  RGB_SPI, KC_MPRV,
        MC_4,    RGB_SAD,  RGB_SPD, KC_MPLY, _______,
        MC_5,    RGB_TOG,           KC_MNXT),

    [L2] = LAYOUT_numpad_6x5(
        XXXXXXX, _______, _______, _______, _______,
        DR_CBPA, DR_CBPB, DR_CBPC, DR_CBPD, DR_CBPE,
        DR_CBPF, DR_CBPG, DR_CBPH, DR_CBPI, XXXXXXX,
        DR_CBPJ, DR_CBPK, DR_CBPL, DR_CBPM,
        DR_CBPN, DR_CBPO, DR_CBPP, DR_CBPQ, XXXXXXX,
        KC_RSFT, XXXXXXX,          XXXXXXX),

    [L3] = LAYOUT_numpad_6x5(
        XXXXXXX, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______, DR_SCR0, DR_SCR1, DR_SCR2, _______,
        _______, DR_SCR3, DR_SCR4, DR_SCR5,
        _______, DR_SCR6, DR_SCR7, DR_SCR8, _______,
        _______, DR_SCR9,          _______),

    [L4] = LAYOUT_numpad_6x5(
        QK_BOOT, _______, _______, _______, _______,
        PB_1,    PB_2,    PB_3,    PB_4,    PB_5,
        PB_6,    PB_7,    PB_8,    PB_9,    PB_10,
        PB_11,   PB_12,   PB_13,   PB_14,
        PB_15,   PB_16,   PB_17,   PB_18,   PB_19,
        PB_20,   PB_21,            PB_22),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [FUNC] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [L2] = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [L3] = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
    [L4] = { ENCODER_CCW_CW(XXXXXXX, XXXXXXX) },
};
#endif // ENCODER_MAP_ENABLE

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t val = rgb_matrix_get_val();
    switch (get_highest_layer(state)) {
        case BASE:
            rgb_matrix_reload_from_eeprom();
            break;
        case L2:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_BREATHING);
            rgb_matrix_sethsv_noeeprom(HSV_BLACKMAGIC_ORANGE * 0 + val);
            break;
        case L3:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_BREATHING);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN * 0 + val);
            break;
        case L4:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_BREATHING);
            rgb_matrix_sethsv_noeeprom(HSV_CYAN * 0 + val);
            break;
    }
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case AM_FUNC:
            if (record->event.pressed) {
                uint8_t current_layer = get_highest_layer(layer_state);
                if(current_layer != BASE) {
                    layer_move(BASE);
                }
                layer_on(FUNC);
            } else {
                layer_off(FUNC);
            }
            break;
    }

    if(keycode >= DR_CPB_MIN && keycode <= DR_CPB_MAX) {
        if(record->event.pressed) {
            SEND_STRING(SS_DOWN(X_LCTL) SS_DOWN(X_LCMD));

            char buffer_label[] = "a";
            uint16_t buffer_index = keycode - DR_CPB_MIN;
            buffer_label[0] = 0x61 + (char)buffer_index;
            send_string(buffer_label);

            SEND_STRING(SS_UP(X_LCMD) SS_UP(X_LCTL));
        }
    }

    if(keycode >= DR_SCR_MIN && keycode <= DR_SCR_MAX) {
        if(record->event.pressed) {
            SEND_STRING(SS_DOWN(X_LCTL) SS_DOWN(X_LCMD));

            char script_label[] = "0";
            uint16_t script_index = keycode - DR_SCR_MIN;
            script_label[0] = 0x30 + (char)script_index;
            send_string(script_label);

            SEND_STRING(SS_UP(X_LCMD) SS_UP(X_LCTL));
        }
    }

    return true;
};
