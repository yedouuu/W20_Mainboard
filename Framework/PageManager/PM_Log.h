/*
 * MIT License
 * Copyright (c) 2026 Yedouuu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef __PM_LOG_H__
#define __PM_LOG_H__

#define PAGE_MANAGER_LOG_ENABLE 1

#if PAGE_MANAGER_LOG_ENABLE
    #include "Utils/Logger.h"
    #define PM_LOG_I(fmt, ...)   log_i(fmt, ##__VA_ARGS__)
    #define PM_LOG_D(fmt, ...)   log_d(fmt, ##__VA_ARGS__)
    #define PM_LOG_W(fmt, ...)   log_w(fmt, ##__VA_ARGS__)
    #define PM_LOG_E(fmt, ...)   log_e(fmt, ##__VA_ARGS__)
#else
    #define PM_LOG_I(fmt, ...)
    #define PM_LOG_D(fmt, ...)
    #define PM_LOG_W(fmt, ...)
    #define PM_LOG_E(fmt, ...)
#endif // PAGE_MANAGER_LOG_ENABLE

#endif // __PM_LOG_H__
