#ifndef _USB2P_H_
#define _USB2P_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
    uint8_t all;
    struct {
        uint8_t cmd:3;
        uint8_t encry_flg:1;
        uint8_t port_num;
    }
}port_ops_t;

typedef struct {
    uint8_t port_type;
    port_ops_t port_ops;
    uint16_t port_len;
    void *pdata;
}usb2p_t;

#ifdef __cplusplus
}
#endif

#endif /* _USB2P_H_ */