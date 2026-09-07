#pragma once
#include "tanmatsu_plugin.h"
// Companion launcher extension, version 1. All drawing stays on the UI task.
int asp_weather_http_get(plugin_context_t *ctx,const char *url,char *body,size_t capacity,int *status);
int asp_weather_draw(pax_buf_t *buffer,int x_right,int y,int height,const char *full,const char *page);
