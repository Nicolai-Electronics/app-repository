#pragma once
#include "gui_style.h"
#include "tanmatsu_plugin.h"
void weather_host_init(void);
void weather_host_prepare(gui_theme_t *theme,int left,bool home);
int asp_weather_http_get(plugin_context_t *ctx,const char *url,char *body,size_t capacity,int *status);
int asp_weather_draw(pax_buf_t *buffer,int x_right,int y,int height,const char *full,const char *page);
