#include "gpio.h"
#include "usart.h"
#include "hal_target.h"

#define BM83_CMD_MAKE_CALL                                  0x00
#define BM83_CMD_MAKE_EXTENSION_CALL                        0x01
#define BM83_CMD_MMI_ACTION                                 0x02
#define BM83_CMD_EVENT_MASK_SETTING                         0x03
#define BM83_CMD_MUSIC_CONTROL                              0x04
#define BM83_CMD_CHANGE_DEVICE_NAME                         0x05
#define BM83_CMD_CHANGE_PIN_CODE                            0x06
#define BM83_CMD_SET_BTM_PARAM                              0x07
#define BM83_CMD_GET_BTM_VERSION                            0x08
#define BM83_CMD_VENDOR_AT_CMD                              0x0A
#define BM83_CMD_AVRCP_SPEC_CMD                             0x0B
#define BM83_CMD_AVRCP_GROUP_NAVIGATION                     0x0C
#define BM83_CMD_READ_LINK_STATUS                           0x0D
#define BM83_CMD_READ_PAIRING_RECORD                        0x0E
#define BM83_CMD_READ_LOCAL_BD_ADDR                         0x0F
#define BM83_CMD_READ_LOCAL_DEVICE_NAME                     0x10
#define BM83_CMD_SEND_SPP_DATA                              0x12
#define BM83_CMD_BTM_UTILITY_FUNCTION                       0x13
#define BM83_CMD_EVENT_ACK                                  0x14
#define BM83_CMD_ADDITIONAL_PROFILE_LINK_SETUP              0x15
#define BM83_CMD_READ_LINKED_DEVICE_INFO                    0x16
#define BM83_CMD_PROFILE_LINK_BACK                          0x17
#define BM83_CMD_DISCONNECT                                 0x18
#define BM83_CMD_USER_CONFIRM_SPP_REQ_REPLY                 0x1A
#define BM83_CMD_SET_HF_GAIN_LEVEL                          0x1B
#define BM83_CMD_EQ_MODE_SETTING                            0x1C
#define BM83_CMD_DSP_NR_CTRL                                0x1D
#define BM83_CMD_GPIO_CTRL                                  0x1E
#define BM83_CMD_BT_MCU_UART_RX_BUFF_SIZE                   0x1F
#define BM83_CMD_VOICE_PROMPT_CMD                           0x20
#define BM83_CMD_SET_OVERALL_GAIN                           0x23
#define BM83_CMD_READ_BTM_SETTING                           0x24
#define BM83_CMD_READ_BTM_BATTERY_CHARGE_STATUS             0x25
#define BM83_CMD_MCU_UPDATE                                 0x26
#define BM83_CMD_REPORT_BATTERY_CAPACITY                    0x27
#define BM83_CMD_LE_ANCS_SERVICE                            0x28
#define BM83_CMD_LE_SIGNALING                               0x29
#define BM83_CMD_MSPK_VENDOR                                0x2A
#define BM83_CMD_READ_MSPK_LINK_STATUS                      0x2B
#define BM83_CMD_MSPK_SYNC_AUDIO_EFFECT                     0x2C
#define BM83_CMD_LE_GATT                                    0x2D
#define BM83_CMD_LE_APP                                     0x2F
#define BM83_CMD_DSP_RUNTIME_PROGRAM                        0x30
#define BM83_CMD_READ_VENDOR_EEPROM                         0x31
#define BM83_CMD_READ_IC_VERSION                            0x32
#define BM83_CMD_READ_LINK_MODE                             0x34
#define BM83_CMD_CONFIGURE_VENDOR_PARAM                     0x35
#define BM83_CMD_MSPK_EXCHANGE_LINK_INFO                    0x37
#define BM83_CMD_MSPK_SET_GIAC                              0x38
#define BM83_CMD_READ_FEATURE_LIST                          0x39
#define BM83_CMD_PERSONAL_MSPK_GROUP_CTRL                   0x3A
#define BM83_CMD_TEST_DEVICE                                0x3B
#define BM83_CMD_READ_EEPROM                                0x3C
#define BM83_CMD_WRITE_EEPROM                               0x3D
#define BM83_CMD_LE_SIGNALING_2                             0x3E
#define BM83_CMD_PBAPC                                      0x3F
#define BM83_CMD_TWS                                        0x40
#define BM83_CMD_AVRCP_BROWSING                             0x41
#define BM83_CMD_READ_PAIRED_LINK_KEY_INFO                  0x42
#define BM83_CMD_AUDIO_TRANSCEIVER                          0x44
#define BM83_CMD_BUTTON_MMI_SETTING                         0x46
#define BM83_CMD_BUTTON_OPERATION                           0x47
#define BM83_CMD_READ_BUTTON_MMI_SETTING                    0x48
#define BM83_CMD_DFU                                        0x49
#define BM83_CMD_AVRCP_VENDOR_DEPENDENT                     0x4A
#define BM83_CMD_CONCERT_MODE_ENDLESS_GROUPING              0x4B
#define BM83_CMD_READ_RUNTIME_LATENCY                       0x4C
#define BM83_CMD_TOGGLE_AUDIO_SOURCE                        0xCC

/**
 * @brief BM83 events OP code list.
 * @details Specified events OP code list of BM83 Click driver.
 */
#define BM83_EVT_CMD_ACK                                    0x00
#define BM83_EVT_BTM_STATE                                  0x01
#define BM83_EVT_CALL_STATUS                                0x02
#define BM83_EVT_CALL_ID                                    0x03
#define BM83_EVT_SMS_INDICATION                             0x04
#define BM83_EVT_MISS_CALL_INDICATION                       0x05
#define BM83_EVT_PHONE_MAX_BATTERY_LEVEL                    0x06
#define BM83_EVT_PHONE_BATTERY_LEVEL                        0x07
#define BM83_EVT_PHONE_ROAMING_STATUS                       0x08
#define BM83_EVT_PHONE_MAX_SIGNAL_STRENGTH                  0x09
#define BM83_EVT_PHONE_SIGNAL_STRENGTH                      0x0A
#define BM83_EVT_PHONE_SERVICE_STATUS                       0x0B
#define BM83_EVT_BATTERY_LEVEL                              0x0C
#define BM83_EVT_CHARGER_STATUS                             0x0D
#define BM83_EVT_RESET_TO_DEFAULT                           0x0E
#define BM83_EVT_VOLUME_LEVEL                               0x0F
#define BM83_EVT_EQ_MODE                                    0x10
#define BM83_EVT_LINKED_DEVICE_INFO                         0x17
#define BM83_EVT_BTM_VERSION                                0x18
#define BM83_EVT_CALL_LIST_REPORT                           0x19
#define BM83_EVT_AVRCP_SPEC_RSP                             0x1A
#define BM83_EVT_BTM_UTILITY_REQ                            0x1B
#define BM83_EVT_VENDOR_AT_CMD_RSP                          0x1C
#define BM83_EVT_UNKNOW_AT_RESULT                           0x1D
#define BM83_EVT_REPORT_LINK_STATUS                         0x1E
#define BM83_EVT_REPORT_PAIRING_RECORD                      0x1F
#define BM83_EVT_REPORT_LOCAL_BD_ADDR                       0x20
#define BM83_EVT_REPORT_LOCAL_DEVICE_NAME                   0x21
#define BM83_EVT_REPORT_SPP_DATA                            0x22
#define BM83_EVT_REPORT_LINK_BACK_STATUS                    0x23
#define BM83_EVT_RINGTONE_FINISH_INDICATION                 0x24
#define BM83_EVT_USER_CONFIRM_SSP_REQ                       0x25
#define BM83_EVT_REPORT_AVRCP_VOL_CTRL                      0x26
#define BM83_EVT_REPORT_INPUT_SIGNAL_LEVEL                  0x27
#define BM83_EVT_REPORT_IAP_INFO                            0x28
#define BM83_EVT_REPORT_AVRCP_ABS_VOL_CTRL                  0x29
#define BM83_EVT_REPORT_VOICE_PROMPT_STATUS                 0x2A
#define BM83_EVT_REPORT_TYPE_CODEC                          0x2D
#define BM83_EVT_REPORT_TYPE_BTM_SETTING                    0x2E
#define BM83_EVT_REPORT_MCU_UPDATE_REPLY                    0x2F
#define BM83_EVT_REPORT_BTM_INITIAL_STATUS                  0x30
#define BM83_EVT_REPORT_LE_ANCS_EVENT                       0x31
#define BM83_EVT_REPORT_LE_EVENT                            0x32
#define BM83_EVT_REPORT_MSPK_STATUS                         0x33
#define BM83_EVT_REPORT_MSPK_VENDOR_EVENT                   0x34
#define BM83_EVT_REPORT_MSPK_AUDIO_SETTING                  0x35
#define BM83_EVT_REPORT_MSPK_EFFECT_STATUS                  0x36
#define BM83_EVT_REPORT_VENDOR_EEPROM                       0x37
#define BM83_EVT_REPORT_IC_VERSION_INFO                     0x38
#define BM83_EVT_REPORT_CUSTOMER_GATT_ATTRIBUTE_DATA        0x39
#define BM83_EVT_REPORT_LINK_MODE                           0x3A
#define BM83_EVT_REPORT_MSPK_EXCHANGE_LINK_INFO             0x3D
#define BM83_EVT_REPORT_BT_CLK                              0x3F
#define BM83_EVT_REPORT_FEATURE_LIST                        0x40
#define BM83_EVT_REPORT_TEST_RESULT                         0x41
#define BM83_EVT_REPORT_READ_EEPROM_DATA                    0x42
#define BM83_EVT_PBAPC_EVENT                                0x43
#define BM83_EVT_AVRCP_BROWSING_EVENT                       0x44
#define BM83_EVT_REPORT_PAIRED_LINK_KEY_INFO                0x45
#define BM83_EVT_REPORT_TWS_RX_VENDOR_EVENT                 0x53
#define BM83_EVT_REPORT_TWS_LOCAL_DEVICE_STATUS             0x54
#define BM83_EVT_REPORT_TWS_VAD_DATA                        0x55
#define BM83_EVT_REPORT_TWS_RADIO_CONDITION                 0x56
#define BM83_EVT_REPORT_TWS_EAR_BUD_POSITION                0x57
#define BM83_EVT_REPORT_TWS_SECONDARY_DEVICE_STATUS         0x58
#define BM83_EVT_AUDIO_TRANSCEIVER_EVENT_STATUS             0x5A
#define BM83_EVT_READ_BUTTON_MMI_SETTING_REPLY              0x5C
#define BM83_EVT_AVRCP_VENDOR_DEPENDENT_RSP                 0x5D
#define BM83_EVT_RUNTIME_LATENCY                            0x5E

/**
 * @brief BM83 MMI actions list.
 * @details Specified MMI actions list of BM83 Click driver.
 */
#define BM83_MMI_ADD_REMOVE_SCO_LINK                        0x01
#define BM83_MMI_ENABLE_TEST_MODE                           0x03
#define BM83_MMI_ACCEPT_CALL                                0x04
#define BM83_MMI_REJECT_CALL                                0x05
#define BM83_MMI_ENDCALL_OR_TRANSFER_TO_HEADSET             0x06
#define BM83_MMI_MIC_MUTE_TOGGLE                            0x07
#define BM83_MMI_MUTE_MIC                                   0x08
#define BM83_MMI_UNMUTE_MIC                                 0x09
#define BM83_MMI_VOICE_DIAL                                 0x0A
#define BM83_MMI_CANCEL_VOICE_DIAL                          0x0B
#define BM83_MMI_LAST_NUMBER_REDIAL                         0x0C
#define BM83_MMI_ACTIVE_CALL_HOLD_ACCEPT_HELD_CALL          0x0D
#define BM83_MMI_VOICE_TRANSFER                             0x0E
#define BM83_MMI_QUERY_CALL_LIST                            0x0F
#define BM83_MMI_THREE_WAY_CALL                             0x10
#define BM83_MMI_RELEASE_CALL                               0x11
#define BM83_MMI_ACCEPT_WAITING_HOLD_CALL_RLS_ACTIVE_CALL   0x12
#define BM83_MMI_DISCONNECT_HF_LINK                         0x17
#define BM83_MMI_ENABLE_AEC                                 0x1E
#define BM83_MMI_DISABLE_AEC                                0x1F
#define BM83_MMI_SWITCH_AEC                                 0x20
#define BM83_MMI_ENABLE_AEC_RX_NOISE_REDUCTION              0x21
#define BM83_MMI_DISABLE_AEC_RX_NOISE_REDUCTION             0x22
#define BM83_MMI_SWITCH_AEC_RX_NOISE_REDUCTION              0x23
#define BM83_MMI_INC_MIC_GAIN                               0x24
#define BM83_MMI_DEC_MIC_GAIN                               0x25
#define BM83_MMI_SWITCH_PRIMARY_SECONDARY_HF_DEVICE         0x26
#define BM83_MMI_LOCK_BUTTON                                0x3A
#define BM83_MMI_DISCONNECT_A2DP                            0x3B
#define BM83_MMI_NEXT_AUDIO_EFFECT                          0x3C
#define BM83_MMI_PREVIOUS_AUDIO_EFFECT                      0x3D
#define BM83_MMI_REPORT_CURRENT_EQ_MODE                     0x3F
#define BM83_MMI_REPORT_CURRENT_AUDIO_EFFECT_STATUS         0x40
#define BM83_MMI_TOGGLE_AUDIO_PLAYBACK                      0x41
#define BM83_MMI_POWER_ON_BUTTON_PRESS                      0x51
#define BM83_MMI_POWER_ON_BUTTON_RELEASE                    0x52
#define BM83_MMI_POWER_OFF_BUTTON_PRESS                     0x53
#define BM83_MMI_POWER_OFF_BUTTON_RELEASE                   0x54
#define BM83_MMI_RESET_EEPROM_SETTING                       0x56
#define BM83_MMI_FORCE_SPEAKER_GAIN_TOGGLE                  0x57
#define BM83_MMI_TOGGLE_BUTTON_IND                          0x58
#define BM83_MMI_COMBINE_FUNCTION_0                         0x59
#define BM83_MMI_COMBINE_FUNCTION_1                         0x5A
#define BM83_MMI_COMBINE_FUNCTION_2                         0x5B
#define BM83_MMI_COMBINE_FUNCTION_3                         0x5C
#define BM83_MMI_ENTER_PAIRING_MODE                         0x5D
#define BM83_MMI_SWITCH_POWER_OFF                           0x5E
#define BM83_MMI_DISABLE_LED                                0x5F
#define BM83_MMI_BUZZER_TOGGLE                              0x60
#define BM83_MMI_DISABLE_BUZZER                             0x61
#define BM83_MMI_ENABLE_BUZZER                              0x62
#define BM83_MMI_TONE_CHANGE                                0x63
#define BM83_MMI_SHS_SEND_USER_DATA_1                       0x72
#define BM83_MMI_SHS_SEND_USER_DATA_2                       0x73
#define BM83_MMI_SHS_SEND_USER_DATA_3                       0x74
#define BM83_MMI_SHS_SEND_USER_DATA_4                       0x75
#define BM83_MMI_SHS_SEND_USER_DATA_5                       0x76
#define BM83_MMI_REPORT_CURRENT_RX_NR_STATUS                0x77
#define BM83_MMI_REPORT_CURRENT_TX_NR_STATUS                0x78
#define BM83_MMI_CANCEL_ALL_BT_PAGING                       0x7A
#define BM83_MMI_DISCONNECT_SPP_LINK                        0x7D
#define BM83_MMI_ENABLE_A2DP_MIX_LINE_IN                    0x80
#define BM83_MMI_DISABLE_A2DP_MIX_LINE_IN                   0x81
#define BM83_MMI_INCREASE_LINE_IN_INPUT_GAIN                0x82
#define BM83_MMI_DECREASE_LINE_IN_INPUT_GAIN                0x83
#define BM83_MMI_TRIGGER_MSPK_CENTRAL                       0xE0
#define BM83_MMI_TRIGGER_MSPK_PERIPHERAL                    0xE1
#define BM83_MMI_MSPK_ONE_KEY_CONNECT                       0xE2
#define BM83_MMI_CANCEL_MSPK_CREATION                       0xE3
#define BM83_MMI_TERMINATE_MSPK_LINK                        0xE4
#define BM83_MMI_TERMINATE_MSPK_CONNECTION                  0xE5
#define BM83_MMI_MSPK_CENTRAL_ENTER_AUX_IN_44p1K_PCM_MODE   0xE6
#define BM83_MMI_MSPK_CENTRAL_ENTER_AUX_IN_48K_PCM_MODE     0xE7
#define BM83_MMI_MSPK_CENTRAL_EXIT_AUX_IN_PCM_MODE          0xE8
#define BM83_MMI_MSPK_SWITCH_CHANNEL                        0xEC
#define BM83_MMI_MSPK_POWER_OFF_ALL_SPEAKERS                0xED
#define BM83_MMI_MSPK_ERASE_CONCERT_MODE_RECORD             0xEF
#define BM83_MMI_MSPK_ENTER_STEREO_MODE                     0xF4
#define BM83_MMI_MSPK_ENTER_CONCERT_MODE                    0xF5
#define BM83_MMI_MSPK_ADD_THIRD_SPK                         0xF6
#define BM83_MMI_MSPK_SOUND_SYNC                            0xF7
#define BM83_MMI_MSPK_CSB_CONNECTED_MODE_SWITCH             0xF8
#define BM83_MMI_MSPK_BACK_TO_LAST_MODE                     0xF9
#define BM83_MMI_MSPK_ONE_KEY_CONNECT_WITH_LINK_BACK        0xFA
#define BM83_MMI_MSPK_ONE_KEY_OP_STEREO_MODE                0xFB
#define BM83_MMI_MSPK_ONE_KEY_OP_CONCERT_MODE               0xFC

/**
 * @brief BM83 command ACK status bytes.
 * @details Specified command ACK status bytes of BM83 Click driver.
 */
#define BM83_ACK_STATUS_CMD_COMPLETE                        0x00
#define BM83_ACK_STATUS_CMD_DISALLOW                        0x01
#define BM83_ACK_STATUS_CMD_UNKNOWN                         0x02
#define BM83_ACK_STATUS_CMD_PARAM_ERR                       0x03
#define BM83_ACK_STATUS_CMD_BTM_BUSY                        0x04
#define BM83_ACK_STATUS_CMD_BTM_FULL                        0x05

/**
 * @brief BM83 BTM status bytes.
 * @details Specified BTM status bytes of BM83 Click driver.
 */
#define BM83_BTM_STATE_POWER_OFF                            0x00
#define BM83_BTM_STATE_PAIRING_STATE                        0x01
#define BM83_BTM_STATE_POWER_ON                             0x02
#define BM83_BTM_STATE_PAIRING_SUCCESSFULL                  0x03
#define BM83_BTM_STATE_PAIRING_FAILED                       0x04
#define BM83_BTM_STATE_HFHS_LINK_ESTABLISHED                0x05
#define BM83_BTM_STATE_A2DP_LINK_ESTABLISHED                0x06
#define BM83_BTM_STATE_HF_LINK_DISCONNECTED                 0x07
#define BM83_BTM_STATE_A2DP_LINK_DISCONNECTED               0x08
#define BM83_BTM_STATE_SCO_LINK_CONNECTED                   0x09
#define BM83_BTM_STATE_SCO_LINK_DISCONNECTED                0x0A
#define BM83_BTM_STATE_AVRCP_LINK_ESTABLISHED               0x0B
#define BM83_BTM_STATE_AVRCP_LINK_DISCONNECTED              0x0C
#define BM83_BTM_STATE_STANDARD_SPP_CONNECTED               0x0D
#define BM83_BTM_STATE_STANDARD_SPP_IAP_DISCONNECTED        0x0E
#define BM83_BTM_STATE_STANDBY_STATE                        0x0F
#define BM83_BTM_STATE_IAP_CONNECTED                        0x10
#define BM83_BTM_STATE_ACL_DISCONNECTED                     0x11
#define BM83_BTM_STATE_MAP_CONNECTED                        0x12
#define BM83_BTM_STATE_MAP_OPERATION_FORBIDDEN              0x13
#define BM83_BTM_STATE_MAP_DISCONNECTED                     0x14
#define BM83_BTM_STATE_ACL_CONNECTED                        0x15
#define BM83_BTM_STATE_SPP_IAP_DISCONNECTED_NO_PROFILE      0x16
#define BM83_BTM_STATE_LINK_BACK_ACL                        0x17
#define BM83_BTM_STATE_INQUIRY_STATE                        0x18
#define BM83_BTM_STATE_UNKNOWN_AUDIO_SOURCE                 0x80
#define BM83_BTM_STATE_AUX_IN_AUDIO_SOURCE                  0x81
#define BM83_BTM_STATE_A2DP_AUDIO_SOURCE                    0x82

/**
 * @brief BM83 Volume control mask bytes.
 * @details Specified Volume control mask bytes of BM83 Click driver.
 */
#define BM83_VOLUME_MASK_A2DP                               0x01
#define BM83_VOLUME_MASK_HF                                 0x02
#define BM83_VOLUME_MASK_LINE_IN                            0x04
#define BM83_VOLUME_MASK_PCM                                0x08
#define BM83_VOLUME_MASK_LINE_IN_INPUT                      0x10  

/**
 * @brief BM83 header byte.
 * @details Specified header byte of BM83 Click driver.
 */
#define BM83_START_HEADER                                   0xAA

/**
 * @brief BM83 driver buffer size.
 * @details Specified size of driver ring buffer.
 * @note Increase buffer size if needed.
 */
#define DRV_BUFFER_SIZE                                         512

/*! @} */ // BM83_cmd

typedef struct
{
    uint8_t opcode;
    uint8_t param_buf[ 256 ];
    uint8_t param_len;
    
} BM83_packet_t;

/**
 * @brief BM83 Click context object.
 * @details Context object definition of BM83 Click driver.
 */
typedef struct
{
                        /**< System config pin. */

    

    // Buffers
    char uart_rx_buffer[ DRV_BUFFER_SIZE ]; /**< Buffer size. */
    char uart_tx_buffer[ DRV_BUFFER_SIZE ]; /**< Buffer size. */
    
    BM83_packet_t command_packet;
    BM83_packet_t event_packet;

} BM83_t;



/**
 * @brief BM83 Click return value data.
 * @details Predefined enum values for driver return values.
 */
typedef enum
{
    BM83_OK = 0,
    BM83_ERROR = -1

} BM83_return_value_t;

/**
 * @brief BM83 Click music control values.
 * @details Predefined enum values for music control.
 */
typedef enum
{
    BM83_STOP_FAST_FORWARD_OR_REWIND = 0,
    BM83_FAST_FORWARD,
    BM83_FAST_FORWARD_WITH_REPEAT,
    BM83_REWIND,
    BM83_REWIND_WITH_REPEAT,
    BM83_PLAY,
    BM83_PAUSE,
    BM83_PLAY_PAUSE_TOGGLE,
    BM83_STOP,
    BM83_NEXT_SONG,
    BM83_PREVIOUS_SONG

} BM83_music_control_t;

/**
 * @brief BM83 Click equalizer mode selection values.
 * @details Predefined enum values for equalizer mode selection.
 */
typedef enum
{
    BM83_EQ_MODE_OFF = 0,
    BM83_EQ_MODE_SOFT,
    BM83_EQ_MODE_BASS,
    BM83_EQ_MODE_TREBLE,
    BM83_EQ_MODE_CLASSIC,
    BM83_EQ_MODE_ROCK,
    BM83_EQ_MODE_JAZZ,
    BM83_EQ_MODE_POP,
    BM83_EQ_MODE_DANCE,
    BM83_EQ_MODE_RNB,
    BM83_EQ_MODE_USER1

} BM83_eq_mode_t;

/*!
 * @addtogroup BM83 BM83 Click Driver
 * @brief API for configuring and manipulating BM83 Click driver.
 * @{
 */

/**
 * @brief BM83 configuration object setup function.
 * @details This function initializes click configuration structure to initial
 * values.
 * @param[out] cfg : Click configuration structure.
 * See #BM83_cfg_t object definition for detailed explanation.
 * @return Nothing.
 * @note The all used pins will be set to unconnected state.
 */

err_t BM83_default_cfg ( BM83_t *ctx );

/**
 * @brief BM83 data writing function.
 * @details This function writes a desired number of data bytes by using UART serial interface.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] data_buf : Data buffer for sending.
 * @param[in] len : Number of bytes for sending.
 * @return @li @c  >=0 - Success,
 *         @li @c   <0 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_generic_write ( BM83_t *ctx, uint8_t *data_buf, uint16_t len );

/**
 * @brief BM83 data reading function.
 * @details This function reads a desired number of data bytes by using UART serial interface.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[out] data_buf : Output read data.
 * @param[in] max_len : Number of bytes to be read.
 * @return @li @c  >0 - Number of data bytes read,
 *         @li @c <=0 - Error/Empty Ring buffer.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_generic_read ( BM83_t *ctx, uint8_t *data_buf, uint16_t max_len );

/**
 * @brief BM83 set MFB pin function.
 * @details This function sets the MFB pin to a desired logic state.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] state : Pin logic state.
 * @return None.
 * @note None.
 */
void BM83_set_mfb_pin ( BM83_t *ctx, uint8_t state );

/**
 * @brief BM83 set RST pin function.
 * @details This function sets the RST pin to a desired logic state.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] state : Pin logic state.
 * @return None.
 * @note None.
 */
void BM83_set_rst_pin ( BM83_t *ctx, uint8_t state );

/**
 * @brief BM83 set application mode function.
 * @details This function enters the application mode by setting the MOD pin to high logic state.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return None.
 * @note None.
 */
void BM83_set_application_mode ( BM83_t *ctx );

/**
 * @brief BM83 set test mode function.
 * @details This function enters the test mode by setting the MOD pin to low logic state.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return None.
 * @note None.
 */
void BM83_set_test_mode ( BM83_t *ctx );

/**
 * @brief BM83 reset device function.
 * @details This function resets the device via the RST pin.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return None.
 * @note None.
 */
void BM83_reset_device ( BM83_t *ctx );

/**
 * @brief BM83 soft mute output function.
 * @details This function soft mutes output by setting the SM pin to high logic state.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return None.
 * @note None.
 */
void BM83_soft_mute_output ( BM83_t *ctx );

/**
 * @brief BM83 soft unmute output function.
 * @details This function soft unmutes output by setting the SM pin to low logic state.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return None.
 * @note None.
 */
void BM83_soft_unmute_output ( BM83_t *ctx );

/**
 * @brief BM83 send command function.
 * @details This function sends a desired command from the click context object.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return None.
 * @note None.
 */
void BM83_send_command ( BM83_t *ctx );

/**
 * @brief BM83 send command with ack check function.
 * @details This function sends a desired command from the click context object, and waits for
 * the command acknowledge event, if there's no ACK event received it resends the command once again.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_send_command_with_ack_check ( BM83_t *ctx );

/**
 * @brief BM83 read event function.
 * @details This function reads an event from the ring buffer and stores it in the click context object.
 * It waits 200ms for an event, and if there's no event received it returns an error.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return @li @c  0 - Correct event received,
 *         @li @c -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_read_event ( BM83_t *ctx );

/**
 * @brief BM83 wait for event function.
 * @details This function waits for the desired event to occur and stores it in the click context object.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] opcode : Expected event opcode.
 * @return None.
 * @note None.
 */
void BM83_wait_for_event ( BM83_t *ctx, uint8_t opcode );

/**
 * @brief BM83 clear events function.
 * @details This function clears all events from the ring buffer. The last cleared event will be stored in the
 * click context object.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return None.
 * @note None.
 */
void BM83_clear_events ( BM83_t *ctx );

/**
 * @brief BM83 send MMI action function.
 * @details This function sends a desired MMI action to specific index database.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] database_index : Database index.
 * @param[in] action : MMI action byte.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_send_mmi_action ( BM83_t *ctx, uint8_t database_index, uint8_t action );

/**
 * @brief BM83 power ON device function.
 * @details This function powers ON the device and waits for the power ON BTM event to occur.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_power_on_device ( BM83_t *ctx );

/**
 * @brief BM83 power OFF device function.
 * @details This function powers OFF the device and waits for the power OFF BTM event to occur.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_power_off_device ( BM83_t *ctx );

/**
 * @brief BM83 enter pairing mode function.
 * @details This function enters pairing mode and waits for the pairing state BTM event to occur.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_enter_pairing_mode ( BM83_t *ctx );

/**
 * @brief BM83 reset to default function.
 * @details This function resets the device to default state and waits for an appropriate event to occur.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_reset_to_default ( BM83_t *ctx );

/**
 * @brief BM83 set device name function.
 * @details This function sets the local device name.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] device_name : Device name (up to 32 characters).
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_set_device_name ( BM83_t *ctx, uint8_t *device_name );

/**
 * @brief BM83 music control function.
 * @details This function sends a music control command with a desired action.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] action : Action for music control command.
 * See #BM83_music_control_t enum definition for detailed explanation.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_music_control ( BM83_t *ctx, BM83_music_control_t action );

/**
 * @brief BM83 make call function.
 * @details This function makes a call to the specified phone number.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] database_index : Database index.
 * @param[in] phone_number : Phone number (up to 19 characters).
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_make_call ( BM83_t *ctx, uint8_t database_index, uint8_t *phone_number );

/**
 * @brief BM83 volume up function.
 * @details This function increases the volume of a specified gain mask.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] database_index : Database index.
 * @param[in] mask : Gain mask.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_volume_up ( BM83_t *ctx, uint8_t database_index, uint8_t mask );

/**
 * @brief BM83 volume down function.
 * @details This function decreases the volume of a specified gain mask.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] database_index : Database index.
 * @param[in] mask : Gain mask.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_volume_down ( BM83_t *ctx, uint8_t database_index, uint8_t mask );

/**
 * @brief BM83 enable advertising function.
 * @details This function enables advertising of LE signaling.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_enable_advertising ( BM83_t *ctx );

/**
 * @brief BM83 disable advertising function.
 * @details This function disables advertising of LE signaling.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_disable_advertising ( BM83_t *ctx );

/**
 * @brief BM83 set equalizer mode function.
 * @details This function sets the equalizer mode.
 * @param[in] ctx : Click context object.
 * See #BM83_t object definition for detailed explanation.
 * @param[in] eq_mode : Equalizer mode byte.
 * See #BM83_eq_mode_t enum definition for detailed explanation.
 * @return @li @c  >=0 - Command ACK event response,
 *         @li @c   -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t BM83_set_eq_mode ( BM83_t *ctx, BM83_eq_mode_t eq_mode );

#ifdef __cplusplus

#endif

/*! @} */ // BM83

// ------------------------------------------------------------------------ END