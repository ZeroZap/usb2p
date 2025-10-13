#ifndef _USB2P_H_
#define _USB2P_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USB2P_ID_GEN(port_type, port_num) (((port_type) << 8) | (port_num))
#define USB2P_ID_TYPE(id) ((id) >> 8)
#define USB2P_ID_NUM(id)  ((id) & 0xff)

typedef enum {
    usb2p_error_none = 0,
    usb2p_error_busy,
    usb2p_error_supported,
    usb2p_error_invalid_param,
    usb2p_error_over_length,
    usb2p_error_no_buffer_space,
    usb2p_error_init_failed,
} usb2p_error_t;

typedef enum {
    usb2p_type_none = 0,
    usb2p_type_gpio,
    usb2p_type_uart,
    usb2p_type_i2c,
    usb2p_type_spi,
    usb2p_type_pwm,
    usb2p_type_adc,
    usb2p_type_sensor,
    usb2p_log = 0xfe,
    usb2p_cfg = 0xff, /**< usb2p_cfg: register id. */
} usb2p_type_t;

typedef int32_t (*data_process)(char *pdata, uint16_t len);
int32_t usb2p_insert(uint16_t id, uint16_t max_size, data_process pfn);
int32_t usb2p_remove(uint16_t id);

int32_t usb2p_send(uint16_t id, char *pdata, uint16_t len);

int32_t usb2p_recv(uint16_t id, char *pdata, uint16_t len);

int32_t usb2p_status(void);

#ifdef __cplusplus
}
#endif

#endif /* _USB2P_H_ */