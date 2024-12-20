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
    - 0x01 reserve
    - 0x00 reserve
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

typedef struct {
    uint8_t type;
    port_header_t header;
    uint16_t len;
    void *pdata;
} usb2p_t;

typedef int (*usb2p_init)(uint8_t channel, void *param, uint16_t len,
                          uint16_t timeout);
typedef int (*usb2p_deinit)(uint8_t channel);
typedef int (*usb2p_read)(uint8_t channel, void *data, uint16_t len);
typedef int (*usb2p_write)(uint8_t channel, void *data, uint16_t len);
typedef int (*usb2p_status)(uint8_t channel, void *data, uint16_t len);

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