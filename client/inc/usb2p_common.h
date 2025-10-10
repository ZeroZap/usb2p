#ifndef _USB2P_COMMON_H_
#define _USB2P_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief usb 2 port ops
 * - cmd
    - 0x7 init
    - 0x6 deinit
    - 0x5 ack
    - 0x4 no ack
    - 0x3 read/response
    - 0x2 w/request
    - 0x1 reserve
    - 0x0 reserve
 *
 */
typedef union {
    uint8_t all;
    struct {
        uint8_t cmd : 3;
        uint8_t encrypt : 1;
        uint8_t port_num : 4;
    };
} port_header_t;

typedef enum {
    usb2p_none = 0,
    usb2p_gpio,
    usb2p_uart,
    usb2p_i2c,
    usb2p_spi,
    usb2p_pwm,
    usb2p_adc,
    usb2p_sensor,
    usb2p_log = 0xfe,
    usb2p_all = 0xff, /**< broadcast. */
} usb2p_type_t;

typedef struct {
    uint8_t port_type;
    uint8_t port_num;
    uint16_t len;
    void *pdata;
} usb2p_t;

// 因为 USB 有了 CRC 校验，所以这里不需要再加 crc 校验了

typedef int (*usb2p_init)(uint8_t channel, void *param, uint16_t len,
                          uint16_t timeout);
typedef int (*usb2p_deinit)(uint8_t channel);
typedef int (*usb2p_read)(uint8_t channel, void *data, uint16_t len,
                          uint16_t timeout);
typedef int (*usb2p_write)(uint8_t channel, void *data, uint16_t len,
                           uint16_t timeout);
typedef int (*usb2p_status)(uint8_t channel, void *data, uint16_t len,
                            uint16_t timeout);

struct usb2p_ops {
    usb2p_init init;
    usb2p_deinit deinit;
    usb2p_read read;
    usb2p_write write;
    usb2p_status status;
};

typedef struct usb2p_ops *usb2p_ops_t

#ifdef __cplusplus
}
#endif
#endif