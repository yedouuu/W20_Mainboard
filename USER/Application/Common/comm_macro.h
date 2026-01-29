/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file commonMacro.h
 * 
 * @author Yedou | UNION
 * 
 * @brief Provide the Common Macros simplify the code and improve readability.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2025-10-29 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 * 
 *****************************************************************************/

#ifndef __COMMON_MACRO_H__
#define __COMMON_MACRO_H__

#define PI                      3.1415926535897932384626433832795f
#define HALF_PI                 1.5707963267948966192313216916398f
#define TWO_PI                  6.283185307179586476925286766559f
#define DEG_TO_RAD              0.017453292519943295769236907684886f
#define RAD_TO_DEG              57.295779513082320876798154814105f
#define EULER                   2.718281828459045235360287471352f

#define LSBFIRST                0x0
#define MSBFIRST                0x1

#define LOW                     0x0
#define HIGH                    0x1

#define min(a,b)                ((a)<(b)?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define ABS(x)                  (((x)>0)?(x):-(x)) //abs(x) is define in stdlib.h
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)                ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg)            ((deg)*DEG_TO_RAD)
#define degrees(rad)            ((rad)*RAD_TO_DEG)
#define sq(x)                   ((x)*(x))

#define lowByte(w)              ((uint8_t) ((w) & 0xff))
#define highByte(w)             ((uint8_t) ((w) >> 8))

#define bitRead(value, bit)     (((value) >> (bit)) & 0x01)
#define bitSet(value, bit)      ((value) |= (1UL << (bit)))
#define bitClear(value, bit)    ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define delay(ms)                    delay_ms(ms)
#define delayMicroseconds(us)        delay_us(us)

#define DISABLE_ALL_IRQ()                 sei()
#define ENABLE_ALL_IRQ()                  cli()

#endif /* __COMMON_MACRO_H__ */
