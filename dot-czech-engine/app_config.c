#include "app_config.h"

static const app_config_t def_app_config = {
    .lang = {
        .support = 2,
        .major = 3,
        .item = {
            { .code = 0x05, .rule = 2, .grade = 1 },
            { .code = 0x0A, .rule = 1, .grade = 1 },
        },
    },
};
app_config_t* m_app_config = (app_config_t*)&def_app_config;




