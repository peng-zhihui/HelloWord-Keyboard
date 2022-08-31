#ifndef HELLO_WORD_KEYBOARD_FW_HW_KEYBOARD_H
#define HELLO_WORD_KEYBOARD_FW_HW_KEYBOARD_H

#include "spi.h"


class HWKeyboard
{
public:
    explicit HWKeyboard(SPI_HandleTypeDef* _spi) :
        spiHandle(_spi)
    {
        scanBuffer = &spiBuffer[1];

        // Enable 74HC165 clock
        HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);
    }


    static const uint8_t IO_NUMBER = 11 * 8; // Used 11 pcs 74HC165
    static const uint8_t KEY_NUMBER = 82;
    static const uint8_t TOUCHPAD_NUMBER = 6;
    static const uint8_t LED_NUMBER = 104;
    static const uint8_t KEYMAP_NUM = 5;
    static const uint16_t KEY_REPORT_SIZE = 1 + 16;
    static const uint16_t RAW_REPORT_SIZE = 1 + 32;
    static const uint16_t HID_REPORT_SIZE = KEY_REPORT_SIZE + RAW_REPORT_SIZE;

    enum KeyCode_t : int16_t
    {
        /*------------------------- HID report data -------------------------*/
        LEFT_CTRL = -8, LEFT_SHIFT = -7, LEFT_ALT = -6, LEFT_GUI = -5,
        RIGHT_CTRL = -4, RIGHT_SHIFT = -3, RIGHT_ALT = -2, RIGHT_GUI = -1,

        RESERVED = 0, ERROR_ROLL_OVER, POST_FAIL, ERROR_UNDEFINED,
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        NUM_1/*1!*/, NUM_2/*2@*/, NUM_3/*3#*/, NUM_4/*4$*/, NUM_5/*5%*/,
        NUM_6/*6^*/, NUM_7/*7&*/, NUM_8/*8**/, NUM_9/*9(*/, NUM_0/*0)*/,
        ENTER, ESC, BACKSPACE, TAB, SPACE,
        MINUS/*-_*/, EQUAL/*=+*/, LEFT_U_BRACE/*[{*/, RIGHT_U_BRACE/*]}*/,
        BACKSLASH/*\|*/, NONE_US/**/, SEMI_COLON/*;:*/, QUOTE/*'"*/,
        GRAVE_ACCENT/*`~*/, COMMA/*,<*/, PERIOD/*.>*/, SLASH/*/?*/,
        CAP_LOCK, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        PRINT, SCROLL_LOCK, PAUSE, INSERT, HOME, PAGE_UP, DELETE, END, PAGE_DOWN,
        RIGHT_ARROW, LEFT_ARROW, DOWN_ARROW, UP_ARROW, PAD_NUM_LOCK,
        PAD_SLASH, PAD_ASTERISK, PAD_MINUS, PAD_PLUS, PAD_ENTER,
        PAD_NUM_1, PAD_NUM_2, PAD_NUM_3, PAD_NUM_4, PAD_NUM_5,
        PAD_NUM_6, PAD_NUM_7, PAD_NUM_8, PAD_NUM_9, PAD_NUM_0,
        PAD_PERIOD = 99, // followed with 20 bits 0, total 16 Bytes report data
        /*------------------------- HID report data -------------------------*/

//      /*-------------------------- Virtual keys ---------------------------*/
//      EXECUTE = 116,
//      HELP, MENU, SELECT, STOP, AGAIN, UNDO, CUT, COPY, PASTE, FIND, MUTE,
//      VOLUME_UP, VOLUME_DOWN,
//      /*-------------------------- Virtual keys ---------------------------*/

        /*--------------------------- Smart keys ----------------------------*/
        FN = 1000,
        TOUCH_BAR_1, TOUCH_BAR_2, TOUCH_BAR_3, TOUCH_BAR_4, TOUCH_BAR_5, TOUCH_BAR_6
        /*--------------------------- Smart keys ----------------------------*/
    };

    struct Color_t
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    enum SpiWs2812Byte_t : uint8_t
    {
        // This should be tuned considering WS2812b clock timing
        WS_HIGH = 0xFE,
        WS_LOW = 0xE0
    };


    uint8_t* ScanKeyStates();
    void ApplyDebounceFilter(uint32_t _filterTimeUs = 100);
    uint8_t* Remap(uint8_t _layer = 1);
    void SetRgbBuffer(uint8_t _keyId, Color_t _color);
    void SyncLights();
    bool FnPressed();
    uint8_t* GetHidReportBuffer(uint8_t _reportId = 0);


    int16_t keyMap[KEYMAP_NUM][IO_NUMBER] = {
        // The first layer, used for aligning 74HC165 IO pins to PCB key layout
        {67,61, 60, 58, 59, 52, 55, 51, 50, 49, 48,47,46,3,
            80, 81,64,57,62,63,53,54,45,44,40,31,26,18,2,
            19,70, 71, 66, 65, 56, 36, 37, 38, 39, 43, 42, 41,28,1,
            15, 74, 73, 72, 68, 69, 29, 30, 35, 34, 33, 32,24,0,
            14, 76, 77, 78, 79, 16, 20, 21, 22,23, 27,25,17, 4,
            13,12, 8,75,9, 10,7,11, 6,5,
            82,83,84,85,86,87},

        // Other layer, used for mapping default key layout to custom layout
        {ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PAUSE,
            GRAVE_ACCENT, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0, MINUS, EQUAL, BACKSPACE, INSERT,
            TAB, Q,W,E,R,T,Y,U,I,O,P,LEFT_U_BRACE, RIGHT_U_BRACE, BACKSLASH, DELETE,
            CAP_LOCK, A,S,D,F,G,H,J,K,L,SEMI_COLON, QUOTE, ENTER, PAGE_UP,
            LEFT_SHIFT, Z,X,C,V,B,N,M,COMMA, PERIOD, SLASH, RIGHT_SHIFT, UP_ARROW, PAGE_DOWN,
            LEFT_CTRL, LEFT_GUI, LEFT_ALT, SPACE, RIGHT_ALT, RIGHT_GUI, FN, LEFT_ARROW, DOWN_ARROW, RIGHT_ARROW,
            PAD_NUM_1,PAD_NUM_2,PAD_NUM_3,PAD_NUM_4,PAD_NUM_5,PAD_NUM_6},

            {ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PAUSE,
            GRAVE_ACCENT, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0, MINUS, EQUAL, BACKSPACE, INSERT,
            TAB, Q,W,E,R,T,Y,U,I,O,P,LEFT_U_BRACE, RIGHT_U_BRACE, BACKSLASH, DELETE,
            CAP_LOCK, A,S,D,F,G,H,J,K,L,SEMI_COLON, QUOTE, ENTER, PAGE_UP,
            LEFT_SHIFT, Z,X,C,V,B,N,M,COMMA, PERIOD, SLASH, RIGHT_SHIFT, A,PAGE_DOWN,
            LEFT_CTRL, LEFT_GUI, LEFT_ALT, SPACE, RIGHT_ALT, RIGHT_GUI, FN, LEFT_ARROW, C,RIGHT_ARROW,
            PAD_NUM_1,PAD_NUM_2,PAD_NUM_3,PAD_NUM_4,PAD_NUM_5,PAD_NUM_6}
    };

    volatile bool isRgbTxBusy{};
    uint8_t hidBuffer[HID_REPORT_SIZE]{};


private:
    SPI_HandleTypeDef* spiHandle;
    uint8_t spiBuffer[IO_NUMBER / 8 + 1]{};
    uint8_t* scanBuffer;
    uint8_t debounceBuffer[IO_NUMBER / 8 + 1]{};
    uint8_t remapBuffer[IO_NUMBER / 8]{};

    uint8_t rgbBuffer[LED_NUMBER][3][8]{};
    uint8_t wsCommit[64] = {0};
    uint8_t brightnessPrediv = 2; // means 1/4 brightness
};


#endif
