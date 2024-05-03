 
 #ifndef _HAL_TARGET_H_
 #define _HAL_TARGET_H_
  
 #ifdef __cplusplus
 extern "C"{
 #endif
  
 #include <stdint.h>
  
 #define HAL_MODULE_ERROR (hal_base_addr_t)(0xFFFFFFFF) 
 #define HAL_CHANNEL_ERROR (hal_channel_t)(0xFFFFFFFF) 
 #define HAL_PIN_NC (hal_pin_name_t)(0xFFFFFFFF) 
 #define HAL_PORT_NC (hal_port_name_t)(0xFFFFFFFF) 
 typedef hal_base_addr_t; 
 typedef hal_channel_t; 
 typedef hal_pin_name_t; 
 typedef hal_port_name_t; 
 typedef hal_port_size_t; 
 typedef int32_t err_t; 
 typedef enum
 {
  ACQUIRE_SUCCESS = 0, 
  ACQUIRE_INIT, 
  ACQUIRE_FAIL = (-1) 
 } acquire_t;
  
 #ifdef __cplusplus
 }
 #endif
  
 #endif // _HAL_TARGET_H_
 // ------------------------------------------------------------------------- END