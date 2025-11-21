 /** 
 * @brief Global Error Code Definitions
 * @author yedouuu
 * @brief 
 * @version 0.1
 * @date 2025-11-19
 * 
 * @copyright Copyright (c) 2025 yedouuu
 * @file errno.h
 */ 

#ifndef __ERRNO_H__
#define __ERRNO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 
 * 
 * These error codes are used throughout the HAL, Middleware, and Application layers
 * to indicate the status of operations.
 */
typedef enum {
  /* General Status */
  kStatus_Success         = 0,    /**< Operation completed successfully */
  kStatus_Fail            = -1,   /**< Generic failure */
  kStatus_Error           = -2,   /**< Generic error */
  
  /* Argument & State Errors */
  kStatus_InvalidArgument = -3,   /**< Invalid argument passed to function */
  kStatus_InvalidState    = -4,   /**< System or peripheral is in an invalid state for the operation */
  kStatus_NotInitialized  = -5,   /**< Module or peripheral not initialized */
  kStatus_NotSupported    = -6,   /**< Operation or feature not supported */
  
  /* Resource Errors */
  kStatus_Busy            = -10,  /**< Resource is currently busy */
  kStatus_Timeout         = -11,  /**< Operation timed out */
  kStatus_OutOfMemory     = -12,  /**< Memory allocation failed */
  kStatus_QueueFull       = -13,  /**< Queue or buffer is full */
  kStatus_QueueEmpty      = -14,  /**< Queue or buffer is empty */
  kStatus_NotFound        = -15,  /**< Requested resource or item not found */
  
  /* Communication/IO Errors */
  kStatus_IOError         = -20,  /**< Hardware I/O error */
  kStatus_TxError         = -21,  /**< Transmission error */
  kStatus_RxError         = -22,  /**< Reception error */
  kStatus_CRCError        = -23,  /**< CRC check failed */
  kStatus_ParseError      = -24,  /**< Data parsing failed */
  
  /* Hardware Specific */
  kStatus_DeviceMissing   = -30,  /**< External device not detected */
  kStatus_DeviceError     = -31,  /**< External device reported an error */

} Status_t;

#ifdef __cplusplus
}
#endif

#endif /* __ERRNO_H__ */
