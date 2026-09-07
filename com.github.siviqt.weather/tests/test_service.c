#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "weather.h"
#include "tanmatsu_plugin.h"
static const char *test_config=NULL;
static FILE *test_fopen(const char *path,const char *mode) {
 (void)mode;assert(!strcmp(path,"/sd/weather/weather_plugin.txt"));
 if(!test_config)return NULL;
 FILE *f=tmpfile(); assert(f);fputs(test_config,f);rewind(f);return f;
}
#define fopen test_fopen
#include "../src/main.c"
#undef fopen
static bool online=true,stop=false;
static unsigned scenario=0, weather_calls=0, ip_calls=0;
static uint32_t ticks=0,stop_at=0;
static const char *ip="{\"city\":\"Test\",\"latitude\":0,\"longitude\":0}";
static const char *data="{\"current\":{\"temperature_2m\":0,\"cloud_cover\":0}}";
asp_err_t asp_net_is_connected(bool *v) { *v=online;return ASP_OK; }
int asp_weather_http_get(plugin_context_t *c,const char *url,char *b,size_t cap,int *http) {
 (void)c;*http=200;
 if(strstr(url,"ipwho")||strstr(url,"ipapi")) { ip_calls++;snprintf(b,cap,"%s",scenario==4?"{}":ip);return 0; }
 if(strstr(url,"example.com")) { if(scenario==3){*http=0;return -1;} strcpy(b,"ok");return 0; }
 weather_calls++;
 if(scenario==6){strcpy(b,"{\"temperature_c\":15}");return 0;}
 if(scenario==1 && strstr(url,"open-meteo")){*http=503;return -1;}
 if(scenario==1 && strstr(url,"wttr")){strcpy(b,"{\"current_condition\":[{\"temp_C\":\"5\"}]}");return 0;}
 if(scenario==2||scenario==3){*http=0;return -1;}
 if(scenario==5){strcpy(b,"{}");return 0;}
 snprintf(b,cap,"%s",data);return 0;
}
int asp_weather_draw(pax_buf_t *b,int r,int y,int h,const char *f,const char *p){(void)b;(void)r;(void)y;(void)h;(void)f;(void)p;return 0;}
void asp_log_warn(const char *tag,const char *fmt,...){(void)tag;(void)fmt;}
int asp_plugin_status_widget_register(plugin_context_t *c,plugin_status_widget_fn f,void *d){(void)c;(void)f;(void)d;return 0;}
void asp_plugin_status_widget_unregister(int i){(void)i;}
bool asp_plugin_should_stop(plugin_context_t *c){(void)c;return stop;}
uint32_t asp_plugin_get_tick_ms(void){return ticks;}
void asp_plugin_delay_ms(uint32_t ms){ticks+=ms;if(stop_at&&weather_due(ticks,stop_at))stop=true;}
int main(void){
 plugin_context_t *c=(void*)1; assert(init(c)==0);
 online=false;update(c);assert(!strcmp(status_text,"No Internet access")&&weather_calls==0);
 online=true;update(c);assert(!*status_text&&latest.temperature==0&&weather_calls==1);
 scenario=1;weather_calls=0;update(c);assert(weather_calls==2&&latest.temperature==5&&!*status_text);
 scenario=2;weather_calls=0;update(c);assert(weather_calls==3&&!strcmp(status_text,"Weather sites not available"));
 scenario=4;update(c);assert(!strcmp(status_text,"No weather info found"));
 scenario=5;weather_calls=0;update(c);assert(weather_calls==3&&!strcmp(status_text,"No weather info found"));
 scenario=6;weather_calls=0;ip_calls=0;test_config="location=Fixed\nlatitude=0\nlongitude=0\ncustom_url=https://custom.example/?lat={lat}\n";
 update(c);assert(weather_calls==1&&ip_calls==0&&latest.temperature==15&&!strcmp(latest.city,"Fixed"));
 test_config="latitude=0\n";weather_calls=0;update(c);assert(weather_calls==0&&!strcmp(status_text,"No weather info found"));
 test_config=NULL;
 scenario=0;weather_calls=0;ticks=0;stop_at=WEATHER_INTERVAL_MS*2+250;run(c);assert(weather_calls==3);
 stop=false;weather_calls=0;ticks=0xffff0000u;started=ticks;stop_at=ticks+WEATHER_INTERVAL_MS+250;run(c);assert(weather_calls==2);
 cleanup(c);puts("Service tests passed: offline, fallback, no data, 30-minute cadence, tick wrap");
}
