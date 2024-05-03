#include "bm832.h"
#include <stdint.h>

err_t bm83_Initialise(BM83_t *dev, UART_HandleTypeDef *UARTHandle)
{
    err_t error_flag = BM83_OK;
    
    BM83_set_application_mode(dev);
    BM83_set_mfb_pin(dev, 1);
    BM83_soft_unmute_output(dev);
    BM83_reset_device(dev);

    error_flag |= BM83_power_on_device(dev);
    error_flag |= BM83_reset_to_default(dev);
    error_flag |= BM83_enter_pairing_mode(dev);
    BM83_clear_events(dev);

    return error_flag;
}

//Low level functions 

err_t BM83_generic_write(BM83_t *dev, uint8_t *data_buf, uint16_t len)
{
    return HAL_UART_Transmit(dev->uartHandle, data_buf, len, HAL_MAX_DELAY);
}

err_t BM83_generic_read(BM83_t *dev, uint8_t *data_buf, uint16_t max_len)
{
    return HAL_UART_Receive(dev->uartHandle, data_buf, max_len, HAL_MAX_DELAY);
}

void BM83_set_mfb_pin(BM83_t *dev, uint8_t state)
{
    if (state == 0) {
        HAL_GPIO_WritePin(rst_port, rst_pin, GPIO_PIN_RESET); // Set pin to low
    } else {
        HAL_GPIO_WritePin(rst_port, rst_pin, GPIO_PIN_SET); // Set pin to high
    }
}

void BM83_set_application_mode ( BM83_t *dev )
{
    HAL_GPIO_WritePin ( mod_pin, mod_pin, GPIO_PIN_SET );
}

void BM83_set_test_mode ( BM83_t *dev )
{
    HAL_GPIO_WritePin ( mod_pin, mod_pin, GPIO_PIN_RESET );
}

void BM83_reset_device ( BM83_t *dev )
{
    HAL_GPIO_WritePin(rst_port, rst_pin, GPIO_PIN_RESET);
    HAL_Delay(1000);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(rst_port, rst_pin, GPIO_PIN_SET);
    BM83_clear_events ( dev );
}

void BM83_soft_mute_output ( BM83_t *dev )
{
    HAL_GPIO_WritePin(sm_port, sm_pin, GPIO_PIN_SET);
}

void BM83_soft_unmute_output ( BM83_t *dev )
{
    HAL_GPIO_WritePin(sm_port, sm_pin, GPIO_PIN_RESET);
}

void BM83_send_command ( BM83_t *dev )
{
    uint8_t cmd_buf[ 4 ] = { 0 };
    uint8_t package_len = dev->command_packet.param_len + 1;
    cmd_buf[ 0 ] = BM83_START_HEADER;
    cmd_buf[ 1 ] = ( uint8_t ) ( ( package_len >> 8 ) & 0xFF );
    cmd_buf[ 2 ] = ( uint8_t ) ( package_len & 0xFF );
    cmd_buf[ 3 ] = dev->command_packet.opcode;
    
    HAL_Delay(10);
    BM83_generic_write ( dev, cmd_buf, 4 );
    uint8_t checksum = cmd_buf[ 1 ] + cmd_buf[ 2 ] + cmd_buf[ 3 ];
    if ( dev->command_packet.param_len ) 
    {
        BM83_generic_write ( dev, dev->command_packet.param_buf, dev->command_packet.param_len );
        for ( uint16_t cnt = 0; cnt < dev->command_packet.param_len; cnt++ )
        {
            checksum += dev->command_packet.param_buf[ cnt ];
        }
    }    
    checksum = ~checksum + 1;
    BM83_generic_write ( dev, &checksum, 1 );
    HAL_Delay(10);
}

err_t BM83_send_command_with_ack_check ( BM83_t *dev )
{
    BM83_send_command ( dev );
    if ( BM83_ERROR == BM83_read_event ( dev ) )
    {
        BM83_send_command ( dev );
        if ( BM83_ERROR == BM83_read_event ( dev ) )
        {
            return BM83_ERROR;
        }
    }
    if ( ( BM83_EVT_CMD_ACK == dev->event_packet.opcode ) && 
         ( dev->command_packet.opcode == dev->event_packet.param_buf[ 0 ] ) && 
         ( 2 == dev->event_packet.param_len ) )
    {
        return dev->event_packet.param_buf[ 1 ];
    }
    return BM83_ERROR;
}