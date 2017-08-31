#include "request.h"
#include "cJSON.h"

typedef struct {
    double sell;
    double buy;
} price_item;

price_item *load_price(char *typeIds[], int count) {
    int i;
    int len;
    price_item items[count];

    int data_size = 0;
    for (i = 0; i < count; i++) {
        data_size += strlen("typeid=");
        data_size += strlen(typeIds[i]);
        data_size += strlen("&");
    }
    data_size += strlen("usesystem=30000142");

    char *data = malloc(data_size + 1);
    for (i = 0; i < count; i++) {
        strcat(data, "typeid=");
        strcat(data, typeIds[i]);
        strcat(data, "&");
    }
    strcat(data, "usesystem=30000142");
    
    char *res = http_get(
        "api.eve-central.com",
        "/api/marketstat/json",
        data,
        "Content-Type: application/json"
    );
    len = strlen(res);

    char *body;
    int body_len = 0;
    for (i = 0; i < len; i++) {
        if (res[i] == '\r' 
        && res[i + 1] == '\n' 
        && res[i + 2] == '\r'
        && res[i + 3] == '\n') {
            body_len = len - i - 3;
            body = malloc(body_len);
            memcpy(body, &res[i + 4], body_len);
            body[body_len - 1] = '\0';
            break;
        }
    }

    cJSON *root = cJSON_Parse(body);
    int size = cJSON_GetArraySize(root);
    if (!size) {
        return NULL;
    }

    for (i = 0 ; i < size; i++)
    {
        cJSON *subItem = cJSON_GetArrayItem(root, i);
        cJSON *buy = cJSON_GetObjectItem(subItem, "buy");
        cJSON *sell = cJSON_GetObjectItem(subItem, "sell");
        items[i].buy = cJSON_GetObjectItem(buy, "max")->valuedouble;
        items[i].sell = cJSON_GetObjectItem(sell, "min")->valuedouble;
        printf("buy: %f, sell: %f\n", 
            items[i].buy,
            items[i].sell);
    }

    return items;
}