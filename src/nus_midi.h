/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef BT_NUS_H_
#define BT_NUS_H_

/**
 * @file
 * @defgroup bt_nus Nordic UART (NUS) GATT Service
 * @{
 * @brief Nordic UART (NUS) GATT Service API.
 */

#include <zephyr/types.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#ifdef __cplusplus
extern "C" {
#endif
/** @brief UUID of the NUS Service. **/
#define MIDI_BT_UUID_NUS_VAL \
	BT_UUID_128_ENCODE(0x03b80e5a, 0xede8, 0x4b33, 0xa751, 0x6ce34ec4c700)

/** @brief UUID of the TX Characteristic. **/
#define MIDI_BT_UUID_NUS_TX_VAL \
	BT_UUID_128_ENCODE(0x7772e5db, 0x3868, 0x4112, 0xa1a9, 0xf2669d106bf3)

/** @brief UUID of the RX Characteristic. **/
#define MIDI_BT_UUID_NUS_RX_VAL \
	BT_UUID_128_ENCODE(0x6e400002, 0xb5a3, 0xf393, 0xe0a9, 0xe50e24dcca9e)
	// BT_UUID_128_ENCODE(0x7772e5db, 0x3868, 0x4112, 0xa1a9, 0xf2669d106bf3)

#define MIDI_BT_UUID_NUS_SERVICE   BT_UUID_DECLARE_128(MIDI_BT_UUID_NUS_VAL)
#define MIDI_BT_UUID_NUS_TX        BT_UUID_DECLARE_128(MIDI_BT_UUID_NUS_TX_VAL)
#define MIDI_BT_UUID_NUS_RX        BT_UUID_DECLARE_128(MIDI_BT_UUID_NUS_RX_VAL)

/* Channel messages.
   4 most significant bytes is the type, 4 least significant is the channel.
   Example: 0x83 is note off channel 4*/
#define MIDI_CH_NOTE_OFF                  0x80
#define MIDI_CH_NOTE_ON                   0x90
#define MIDI_CH_POLY_AFTER_TOUCH          0xA0
#define MIDI_CH_CTRL_CHANGE               0xB0
#define MIDI_CH_PRGM_CAHNGE               0xC0
#define MIDI_CH_CHANNEL_AFTER_TOUCH       0xD0
#define MIDI_CH_PITCH_WHEEL               0xE0

/* System Exclusive. */
#define MIDI_SYS_SYSEX_START      0xF0
#define MIDI_SYS_SYSEX_END        0xF7

/* System Common. */
#define MIDI_SYS_CMN_FRAME        0xF1
#define MIDI_SYS_CMN_SONG_POINTER 0xF2
#define MIDI_SYS_CMN_SONG_SELECT  0xF3
#define MIDI_SYS_CMN_TUNE_REQUEST 0xF6

/* System Real-Time. */
#define MIDI_SYS_RT_TIMING        0xF8
#define MIDI_SYS_RT_START         0xFA
#define MIDI_SYS_RT_CONTINUE      0xFB
#define MIDI_SYS_RT_STOP          0xFC
#define MIDI_SYS_RT_SENSE         0xFE
#define MIDI_SYS_RT_RESET         0xFF

/* Midi channels
   4 most significant bytes is the type, 4 least significant is the channel.
   Example: 0x83 is note off channel 4 */

#define MIDI_CHANNEL_1  0x00
#define MIDI_CHANNEL_2  0x01
#define MIDI_CHANNEL_3  0x02
#define MIDI_CHANNEL_4  0x03
#define MIDI_CHANNEL_5  0x04
#define MIDI_CHANNEL_6  0x05
#define MIDI_CHANNEL_7  0x06
#define MIDI_CHANNEL_8  0x07
#define MIDI_CHANNEL_9  0x08
#define MIDI_CHANNEL_10 0x09
#define MIDI_CHANNEL_11 0x0A
#define MIDI_CHANNEL_12 0x0B
#define MIDI_CHANNEL_13 0x0C
#define MIDI_CHANNEL_14 0x0D
#define MIDI_CHANNEL_15 0x0E
#define MIDI_CHANNEL_16 0x0F

#define MIDI_CHANNEL_MIN  MIDI_CHANNEL_1
#define MIDI_CHANNEL_MAX  MIDI_CHANNEL_16

#define MIDI_VELOCITY_MAX 0x7F
#define MIDI_NOTE_MAX     0x7F
#define MIDI_DATA_MAX     0x7F

/*Example note values as defines. Common to have Middle C as 0x3C. 128 Notes in total. */
#define MIDI_NOTE_C8          0x78
#define MIDI_NOTE_C7          0x6C
#define MIDI_NOTE_C6          0x60
#define MIDI_NOTE_C5          0x54
#define MIDI_NOTE_C4          0x48
#define MIDI_NOTE_C3          0x3C
#define MIDI_NOTE_C2          0x30
#define MIDI_NOTE_C1          0x24
#define MIDI_NOTE_C0          0x18
#define MIDI_NOTE_C_MINUS_1   0x0C
#define MIDI_NITE_C_MINUS_2   0x00

/*Example MIDI Data Package */
#define MIDI_Package_C_Note 0x80 0x80 0x90 0x3C 0x7F
//Header-0x80 Timestamp-0x80 Status-0x90 MIDI_NOTE_C3-0x3C MIDI_VELOCITY_MAX-0x7F
#define MIDI_Package_C_Chord 0x80 0x80 0x90 0x3C 0x7F 0x80 0x90 0x40 0x7F 0x44 0x7F //C, E and G Notes
//Header-0x80 Timestamp-0x80 Status-0x90 MIDI_NOTE_C3-0x3C MIDI_VELOCITY_MAX-0x7F
//Timestamp-0x80 Status-0x90 MIDI_NOTE_E3-0x40 MIDI_VELOCITY_MAX-0x7F MIDI_NOTE_G3-0x44 MIDI_VELOCITY_MAX-0x7F

/** @brief NUS send status. */
enum bt_nus_send_status {
	/** Send notification enabled. */
	BT_NUS_SEND_STATUS_ENABLED,
	/** Send notification disabled. */
	BT_NUS_SEND_STATUS_DISABLED,
};

/** @brief Pointers to the callback functions for service events. */
struct bt_nus_cb {
	/** @brief Data received callback.
	 *
	 * The data has been received as a write request on the NUS RX
	 * Characteristic.
	 *
	 * @param[in] conn  Pointer to connection object that has received data.
	 * @param[in] data  Received data.
	 * @param[in] len   Length of received data.
	 */
	void (*received)(struct bt_conn *conn,
			 const uint8_t *const data, uint16_t len);

	/** @brief Data sent callback.
	 *
	 * The data has been sent as a notification and written on the NUS TX
	 * Characteristic.
	 *
	 * @param[in] conn Pointer to connection object, or NULL if sent to all
	 *                 connected peers.
	 */
	void (*sent)(struct bt_conn *conn);

	/** @brief Send state callback.
	 *
	 * Indicate the CCCD descriptor status of the NUS TX characteristic.
	 *
	 * @param[in] status Send notification status.
	 */
	void (*send_enabled)(enum bt_nus_send_status status);

};

/**@brief Initialize the service.
 *
 * @details This function registers a GATT service with two characteristics,
 *          TX and RX. A remote device that is connected to this service
 *          can send data to the RX Characteristic. When the remote enables
 *          notifications, it is notified when data is sent to the TX
 *          Characteristic.
 *
 * @param[in] callbacks  Struct with function pointers to callbacks for service
 *                       events. If no callbacks are needed, this parameter can
 *                       be NULL.
 *
 * @retval 0 If initialization is successful.
 *           Otherwise, a negative value is returned.
 */
int bt_nus_init(struct bt_nus_cb *callbacks);

/**@brief Send data.
 *
 * @details This function sends data to a connected peer, or all connected
 *          peers.
 *
 * @param[in] conn Pointer to connection object, or NULL to send to all
 *                 connected peers.
 * @param[in] data Pointer to a data buffer.
 * @param[in] len  Length of the data in the buffer.
 *
 * @retval 0 If the data is sent.
 *           Otherwise, a negative value is returned.
 */
int bt_nus_send(struct bt_conn *conn, const uint8_t *data, uint16_t len);

/**@brief Get maximum data length that can be used for @ref bt_nus_send.
 *
 * @param[in] conn Pointer to connection Object.
 *
 * @return Maximum data length.
 */
static inline uint32_t bt_nus_get_mtu(struct bt_conn *conn)
{
	/* According to 3.4.7.1 Handle Value Notification off the ATT protocol.
	 * Maximum supported notification is ATT_MTU - 3 */
	return bt_gatt_get_mtu(conn) - 3;
}

#ifdef __cplusplus
}
#endif

/**
 *@}
 */

#endif /* BT_NUS_H_ */
