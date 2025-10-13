#include <stdio.h>
#include <string.h>
#include "usb2p.h"
#include "usb2p_cfg.h"

typedef struct {
    uint16_t id; /**< must be(usb2p_type_t<<8)& port_num. */
    uint16_t len;
    char *pdata;
} usb2p_pkg_t;


typedef struct usb2p {
    struct usb2p *next;
    uint16_t id;       /**< must be(usb2p_type_t<<8)& port_num. */
    uint16_t max_size; /**< max pkg size, not include header. */
    data_process pfn;
    usb2p_pkg_t pkg; // 修改为结构体而非指针
} usb2p_t;

typedef struct {
    usb2p_t *head;
    uint16_t active_num;
} usb2p_manager_t;

static usb2p_manager_t usb2p_mgr = { 0 };

static void send_data(char *pdata, uint16_t len)
{
    usb2p_printf("send data:");
    for (int i = 0; i < len; i++) {
        usb2p_printf("%c ", (uint8_t)pdata[i]);
    }
    usb2p_printf("\r\n");
}

int32_t usb2p_insert(uint16_t id, uint16_t max_size, data_process pfn)
{
    printf("insert id = %d\r\n", id);
    usb2p_t *node = usb2p_mgr.head;
    while (node) {
        if (node->id == id) {
            return usb2p_error_busy;
        }
        node = node->next;
    }
    node = (usb2p_t *)malloc(sizeof(usb2p_t));
    if (!node)
        return usb2p_error_init_failed;
    node->id       = id;
    node->max_size = max_size;
    node->pfn      = pfn;
    node->next     = usb2p_mgr.head;
    usb2p_mgr.head = node;
    usb2p_mgr.active_num++;
    return usb2p_error_none;
}

int32_t usb2p_remove(uint16_t id)
{
    usb2p_t *prev = NULL, *node = usb2p_mgr.head;
    while (node) {
        if (node->id == id) {
            if (prev)
                prev->next = node->next;
            else
                usb2p_mgr.head = node->next;
            if (node->pkg.pdata)
                free(node->pkg.pdata);
            free(node);
            usb2p_mgr.active_num--;
            return usb2p_error_none;
        }
        prev = node;
        node = node->next;
    }
    return usb2p_error_none;
}

int32_t usb2p_send(uint16_t id, char *pdata, uint16_t len)
{
    send_data(pdata, len);

    return usb2p_error_none;
}

int32_t usb2p_recv(uint16_t id, char *pdata, uint16_t len)
{
    usb2p_t *node = usb2p_mgr.head;
    printf("total id: %d\r\n", usb2p_mgr.active_num);
    printf("active id: %d\r\n", node->id);
    printf("active id max size: %d\r\n", node->max_size);
    while (node) {
        if (node->id == id) {
            if(node->pfn) {

                return node->pfn(pdata, len);
            }
        }
        node = node->next;
    }
    return usb2p_error_invalid_param;
}

int32_t demo_recv(char *pdata, uint16_t len)
{
    usb2p_printf("recv data:");
    for (int i = 0; i < len; i++) {
        usb2p_printf("%c ", (uint8_t)pdata[i]);
    }
    usb2p_printf("\r\n");
    return 0;
}


#define HELLO_WORLD "Hello world"
#define DEMO_STR_LEN (sizeof(HELLO_WORLD)+1)
int main(void)
{
    usb2p_printf("usb2p demo start\r\n");

    usb2p_insert(USB2P_ID_GEN(usb2p_type_none, 1), DEMO_STR_LEN, demo_recv);

    char send_buf[DEMO_STR_LEN] = HELLO_WORLD;

    uint8_t id = USB2P_ID_GEN(usb2p_type_none, 1);
    printf("new id = %d\r\n", id);

    usb2p_send(id, send_buf, DEMO_STR_LEN);
    usb2p_recv(id, send_buf, DEMO_STR_LEN);

    return 0;
}
