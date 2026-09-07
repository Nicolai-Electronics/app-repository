#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define WEATHER_INTERVAL_MS 1800000u
#define WEATHER_BODY_CAP 65536u
#define WEATHER_CUSTOM_MAX 4
#define WEATHER_URL_CAP 768

typedef struct { const char *p, *end; } json_span;
bool json_document(const char *text, json_span *out);
json_span json_member(json_span obj, const char *key);
json_span json_first(json_span array);
bool json_number(json_span v, float *out);
bool json_string(json_span v, char *out, size_t cap);

typedef struct {
    char city[64];
    float temperature, high, low, humidity, cloud, wind, direction;
    unsigned valid;
} weather_data;
enum { W_TEMP=1, W_HIGH=2, W_LOW=4, W_HUMID=8, W_CLOUD=16, W_WIND=32, W_DIR=64 };
typedef struct {
    char location[64];
    float latitude, longitude;
    bool manual, coordinates, invalid;
    char custom[WEATHER_CUSTOM_MAX][WEATHER_URL_CAP];
    unsigned custom_count;
} weather_config;
typedef enum { PROVIDER_OPEN_METEO, PROVIDER_WTTR, PROVIDER_MET_NO, PROVIDER_CUSTOM } weather_provider;
void weather_config_parse(char *text, weather_config *cfg);
bool weather_parse(weather_provider provider, const char *body, weather_data *out);
bool weather_location(const char *body, const char *kind, weather_config *cfg);
bool weather_url(weather_provider provider, const weather_config *cfg, const char *custom, char *out, size_t cap);
void weather_format(const weather_data *data, unsigned page, char *out, size_t cap);
bool weather_due(uint32_t now, uint32_t deadline);
