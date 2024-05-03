#include "bm832.h"
#include <stdint.h>
#include <string.h>

err_t BM83_Initialise(BM83_t *dev)
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
    HAL_GPIO_WritePin ( mod_port, mod_pin, GPIO_PIN_SET );
}

void BM83_set_test_mode ( BM83_t *dev )
{
    HAL_GPIO_WritePin ( mod_port, mod_pin, GPIO_PIN_RESET );
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

err_t BM83_read_event ( BM83_t *dev )
{
    uint8_t timeout = 200;
    uint8_t evt_buf[ 4 ] = { 0 };
    while ( BM83_START_HEADER != evt_buf[ 0 ] )
    {
        BM83_generic_read( dev, evt_buf, 1 );
        HAL_Delay(1);
        timeout--;
        if ( 0 == timeout )
        {
            return BM83_ERROR;
        }
    }
    HAL_Delay(10);
    if ( 3 == BM83_generic_read( dev, &evt_buf[ 1 ], 3 ) )
    {
        dev->event_packet.param_len = ( uint16_t ) ( ( ( uint16_t ) evt_buf[ 1 ] << 8 ) | evt_buf[ 2 ] ) - 1;
        dev->event_packet.opcode = evt_buf[ 3 ];
        for ( uint16_t cnt = 0; cnt < dev->event_packet.param_len; cnt++ )
        {
            HAL_Delay(1);
        }
        if ( dev->event_packet.param_len == 
             BM83_generic_read( dev, dev->event_packet.param_buf, dev->event_packet.param_len ) )
        {
            uint8_t checksum_read = 0;
            if ( 1 == BM83_generic_read( dev, &checksum_read, 1 ) )
            {
                uint8_t checksum = evt_buf[ 1 ] + evt_buf[ 2 ] + evt_buf[ 3 ];
                for ( uint16_t cnt = 0; cnt < dev->event_packet.param_len; cnt++ )
                {
                    checksum += dev->event_packet.param_buf[ cnt ];
                }
                checksum = ~checksum + 1;
                if ( checksum == checksum_read )
                {
                    if ( BM83_EVT_CMD_ACK != dev->event_packet.opcode )
                    {
                        dev->command_packet.opcode = BM83_CMD_EVENT_ACK;
                        dev->command_packet.param_buf[ 0 ] = dev->event_packet.opcode;
                        dev->command_packet.param_len = 1;
                        BM83_send_command ( dev );
                    }
                    return BM83_OK;
                }
            }
        }
    }
    return BM83_ERROR;
}

void BM83_wait_for_event ( BM83_t *dev, uint8_t opcode )
{
    do 
    {
        BM83_read_event ( dev );
    } while ( dev->event_packet.opcode != opcode );
}

void BM83_clear_events ( BM83_t *dev )
{
    while ( BM83_OK == BM83_read_event ( dev ) );
    dev->event_packet.opcode = 0;
    memset ( dev->event_packet.param_buf, 0, 256 );
    dev->event_packet.param_len = 0;
}

err_t BM83_send_mmi_action ( BM83_t *dev, uint8_t database_index, uint8_t action )
{
    dev->command_packet.opcode = BM83_CMD_MMI_ACTION;
    dev->command_packet.param_buf[ 0 ] = database_index;
    dev->command_packet.param_buf[ 1 ] = action;
    dev->command_packet.param_len = 2;
    return BM83_send_command_with_ack_check ( dev );
}

err_t BM83_power_on_device ( BM83_t *dev )
{
    BM83_clear_events ( dev );
    err_t error_flag = BM83_send_mmi_action ( dev, 0, BM83_MMI_POWER_ON_BUTTON_PRESS );
    error_flag |= BM83_send_mmi_action ( dev, 0, BM83_MMI_POWER_ON_BUTTON_RELEASE );
    do
    {
        BM83_wait_for_event ( dev, BM83_EVT_BTM_STATE );
    } while ( BM83_BTM_STATE_POWER_ON != dev->event_packet.param_buf[ 0 ] );
    return error_flag;
}

err_t BM83_power_off_device ( BM83_t *dev )
{
    BM83_clear_events ( dev );
    err_t error_flag = BM83_send_mmi_action ( dev, 0, BM83_MMI_POWER_OFF_BUTTON_PRESS );
    error_flag |= BM83_send_mmi_action ( dev, 0, BM83_MMI_POWER_OFF_BUTTON_RELEASE );
    do
    {
        BM83_wait_for_event ( dev, BM83_EVT_BTM_STATE );
    } while ( BM83_BTM_STATE_POWER_OFF != dev->event_packet.param_buf[ 0 ] );
    return error_flag;
}

err_t BM83_enter_pairing_mode ( BM83_t *dev )
{
    BM83_clear_events ( dev );
    err_t error_flag = BM83_send_mmi_action ( dev, 0, BM83_MMI_ENTER_PAIRING_MODE );
    do
    {
        BM83_wait_for_event ( dev, BM83_EVT_BTM_STATE );
    } while ( BM83_BTM_STATE_PAIRING_STATE != dev->event_packet.param_buf[ 0 ] );
    return error_flag;
}

err_t BM83_reset_to_default ( BM83_t *dev )
{
    BM83_clear_events ( dev );
    err_t error_flag = BM83_send_mmi_action ( dev, 0, BM83_MMI_RESET_EEPROM_SETTING );
    BM83_wait_for_event ( dev, BM83_EVT_RESET_TO_DEFAULT );
    return error_flag;
}

err_t BM83_set_device_name ( BM83_t *dev, char *device_name )
{
    uint8_t device_name_size = strlen( device_name );
    if ( device_name_size >= 32 )
    {
        return BM83_ERROR;
    }
    BM83_clear_events ( dev );
    dev->command_packet.opcode = BM83_CMD_CHANGE_DEVICE_NAME;
    memcpy ( dev->command_packet.param_buf, device_name, device_name_size );
    dev->command_packet.param_len = device_name_size;
    err_t error_flag = BM83_send_command_with_ack_check ( dev );
    
    dev->command_packet.opcode = BM83_CMD_CONFIGURE_VENDOR_PARAM;
    dev->command_packet.param_buf[ 0 ] = 0x00;
    dev->command_packet.param_buf[ 1 ] = 0x00;
    dev->command_packet.param_buf[ 2 ] = device_name_size;
    memcpy ( &dev->command_packet.param_buf[ 3 ], device_name, device_name_size );
    dev->command_packet.param_len = device_name_size + 3;
    error_flag |= BM83_send_command_with_ack_check ( dev );
    return error_flag;
}

err_t BM83_music_control ( BM83_t *dev, BM83_music_control_t action )
{
    dev->command_packet.opcode = BM83_CMD_MUSIC_CONTROL;
    dev->command_packet.param_buf[ 0 ] = 0x00;
    dev->command_packet.param_buf[ 1 ] = action;
    dev->command_packet.param_len = 2;
    return BM83_send_command_with_ack_check ( dev );
}

err_t BM83_make_call ( BM83_t *dev, uint8_t database_index, uint8_t *phone_number )
{
    if ( strlen ( phone_number ) > 19 )
    {
        return BM83_ERROR;
    }
    dev->command_packet.opcode = BM83_CMD_MAKE_CALL;
    dev->command_packet.param_buf[ 0 ] = database_index;
    memcpy ( &dev->command_packet.param_buf[ 1 ], phone_number, strlen ( phone_number ) );
    dev->command_packet.param_len = strlen ( phone_number ) + 1;
    return BM83_send_command_with_ack_check ( dev );
}

err_t BM83_volume_up ( BM83_t *dev, uint8_t database_index, uint8_t mask )
{
    dev->command_packet.opcode = BM83_CMD_SET_OVERALL_GAIN;
    dev->command_packet.param_buf[ 0 ] = database_index;
    dev->command_packet.param_buf[ 1 ] = mask;
    dev->command_packet.param_buf[ 2 ] = 0x01;
    dev->command_packet.param_buf[ 3 ] = 0x00;
    dev->command_packet.param_buf[ 4 ] = 0x00;
    dev->command_packet.param_buf[ 5 ] = 0x00;
    dev->command_packet.param_len = 6;
    return BM83_send_command_with_ack_check ( dev );
}

err_t BM83_volume_down ( BM83_t *dev, uint8_t database_index, uint8_t mask )
{
    dev->command_packet.opcode = BM83_CMD_SET_OVERALL_GAIN;
    dev->command_packet.param_buf[ 0 ] = database_index;
    dev->command_packet.param_buf[ 1 ] = mask;
    dev->command_packet.param_buf[ 2 ] = 0x02;
    dev->command_packet.param_buf[ 3 ] = 0x00;
    dev->command_packet.param_buf[ 4 ] = 0x00;
    dev->command_packet.param_buf[ 5 ] = 0x00;
    dev->command_packet.param_len = 6;
    return BM83_send_command_with_ack_check ( dev );
}

err_t BM83_enable_advertising ( BM83_t *dev )
{
    dev->command_packet.opcode = BM83_CMD_LE_SIGNALING;
    dev->command_packet.param_buf[ 0 ] = 0x01;
    dev->command_packet.param_buf[ 1 ] = 0x01;
    dev->command_packet.param_len = 2;
    return BM83_send_command_with_ack_check ( dev );
}

err_t BM83_disable_advertising ( BM83_t *dev )
{
    dev->command_packet.opcode = BM83_CMD_LE_SIGNALING;
    dev->command_packet.param_buf[ 0 ] = 0x01;
    dev->command_packet.param_buf[ 1 ] = 0x00;
    dev->command_packet.param_len = 2;
    return BM83_send_command_with_ack_check ( dev );
}

err_t BM83_set_eq_mode ( BM83_t *dev, BM83_eq_mode_t eq_mode )
{
    dev->command_packet.opcode = BM83_CMD_EQ_MODE_SETTING;
    dev->command_packet.param_buf[ 0 ] = eq_mode;
    dev->command_packet.param_buf[ 1 ] = 0x00;
    return BM83_send_command_with_ack_check ( dev );
}

// ------------------------------------------------------------------------- END