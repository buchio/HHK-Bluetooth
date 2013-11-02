// -*- coding:utf-8 -*-
/*
 *  Copyright (c) 2013 Yukio Obuchi
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation files
 *  (the "Software"), to deal in the Software without restriction,
 *  including without limitation the rights to use, copy, modify, merge,
 *  publish, distribute, sublicense, and/or sell copies of the Software,
 *  and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 *  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 *  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 */

#ifndef H_LED_H_131103070409_
#define H_LED_H_131103070409_


////////////////////////////////////////////////////////////////////////////////////////////////////
/// \defgroup LED LED点滅制御
/// @{


/// 
/// LED点滅状態
/// 
typedef enum {
    LED_off,       ///< 常時消灯
    LED_on,        ///< 常時点灯
    LED_blink0,    ///< ****----
    LED_blink1,    ///< *---*---
    LED_blink2,    ///< ***-***-
    LED_blink3,    ///< *-*-*---
    LED_STATE_END, ///< ユーザーが指定する状態はここまで
    LED_blink3_1,  ///< 複雑な点滅を実現するためのサブ状態
    LED_blink3_2,  ///< 複雑な点滅を実現するためのサブ状態
    LED_ENUM_END   ///< enumの最後
} ledState_t;

extern ledState_t ledState;
void LedStateChange( ledState_t state; );
void LedInit( void );

/// @}

#endif // H_LED_H_131103070409_
