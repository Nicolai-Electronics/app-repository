// SPDX-License-Identifier: MIT
#include "weather.h"
#include "weather_host.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static pthread_mutex_t lock;
static int widget=-1;
static weather_data latest;
static char status_text[48];
static uint32_t started;
static const plugin_info_t info={.name="Weather",.slug="com.github.siviqt.weather",.version="0.1.0",.author="siviqt",.description="Weather in the Home header",.api_version=TANMATSU_PLUGIN_API_VERSION,.type=PLUGIN_TYPE_SERVICE,.flags=1u};
static const plugin_info_t *get_info(void) { return &info; }
static void set_status(const char *s) { pthread_mutex_lock(&lock); snprintf(status_text,sizeof(status_text),"%s",s); pthread_mutex_unlock(&lock); }
static int draw(pax_buf_t *b,int right,int y,int h,void *unused) {
    (void)unused;
    char full[256],page[160],status[48]; weather_data w;
    pthread_mutex_lock(&lock); w=latest; strcpy(status,status_text); pthread_mutex_unlock(&lock);
    if(*status) return asp_weather_draw(b,right,y,h,status,status);
    weather_format(&w,0,full,sizeof(full));
    unsigned p=((asp_plugin_get_tick_ms()-started)/8000u)%4;
    if(p==0) {
        weather_data short_city=w;
        // Preserve complete metrics on later pages; only shorten the location here.
        if(strlen(short_city.city)>20) { size_t n=17; while(n && ((unsigned char)short_city.city[n]&0xc0)==0x80) --n; strcpy(short_city.city+n,"..."); }
        char temp[32]; if(w.valid&W_TEMP) snprintf(temp,sizeof(temp),"%.0f°C",w.temperature); else strcpy(temp,"n/a");
        snprintf(page,sizeof(page),"%s %s",short_city.city,temp);
    } else if(p==1) {
        char hi[24],lo[24]; if(w.valid&W_HIGH) snprintf(hi,sizeof(hi),"%.0f°C",w.high); else strcpy(hi,"n/a");
        if(w.valid&W_LOW) snprintf(lo,sizeof(lo),"%.0f°C",w.low); else strcpy(lo,"n/a");
        snprintf(page,sizeof(page),"High:%s Low:%s",hi,lo);
    } else if(p==2) {
        char humidity[20],cloud[20]; if(w.valid&W_HUMID) snprintf(humidity,sizeof(humidity),"%.0f%%",w.humidity); else strcpy(humidity,"n/a");
        if(w.valid&W_CLOUD) snprintf(cloud,sizeof(cloud),"%.0f%%",w.cloud); else strcpy(cloud,"n/a");
        snprintf(page,sizeof(page),"RH:%s %s:%s",humidity,(w.valid&W_CLOUD)?(w.cloud>=90?"Overcast":w.cloud>=60?"Cloudy":w.cloud>=20?"Partly":"Clear"):"Cloud",cloud);
    } else {
        const char *dirs[]={"N","NNE","NE","ENE","E","ESE","SE","SSE","S","SSW","SW","WSW","W","WNW","NW","NNW"};
        char speed[24]; if(w.valid&W_WIND) snprintf(speed,sizeof(speed),"%.0f km/h",w.wind); else strcpy(speed,"n/a");
        snprintf(page,sizeof(page),"Wind:%s %s",w.valid&W_DIR?dirs[(int)(w.direction/22.5f+0.5f)%16]:"n/a",speed);
    }
    return asp_weather_draw(b,right,y,h,full,page);
}
static int init(plugin_context_t *ctx) {
    if(pthread_mutex_init(&lock,NULL)) return -1;
    started=asp_plugin_get_tick_ms(); strcpy(status_text,"Loading weather...");
    widget=asp_plugin_status_widget_register(ctx,draw,NULL);
    if(widget<0) { pthread_mutex_destroy(&lock); return -1; } return 0;
}
static void cleanup(plugin_context_t *ctx) { (void)ctx; if(widget>=0) asp_plugin_status_widget_unregister(widget); widget=-1; pthread_mutex_destroy(&lock); }
static bool fetch(plugin_context_t *ctx,const char *url,char *body,bool *reachable) {
    int status=0; body[0]=0;
    int result=asp_weather_http_get(ctx,url,body,WEATHER_BODY_CAP,&status);
    if(status>0) *reachable=true;
    return result==0 && status==200;
}
static void update(plugin_context_t *ctx) {
    bool connected=false;
    if(asp_net_is_connected(&connected)!=ASP_OK||!connected) { set_status("No Internet access"); return; }
    weather_config *cfg=calloc(1,sizeof(*cfg));
    char *body=malloc(WEATHER_BODY_CAP);
    if(!cfg || !body) { free(cfg); free(body); set_status("No weather info found"); return; }
    FILE *f=fopen("/sd/weather/weather_plugin.txt","r");
    if(f) {
        size_t n=fread(body,1,4096,f); bool bad=ferror(f)||n==4096; fclose(f);
        body[n]=0; weather_config_parse(body,cfg); cfg->invalid|=bad;
    }
    if(cfg->invalid) { asp_log_warn("weather","Invalid weather_plugin.txt"); set_status("No weather info found"); free(cfg); free(body); return; }
    char url[WEATHER_URL_CAP]; bool reachable=false,location_response=false;
    if(!cfg->coordinates) {
        if(cfg->manual) {
            char encoded[192]; size_t k=0;
            for(const unsigned char *p=(unsigned char*)cfg->location;*p;p++) {
                if((*p>='a'&&*p<='z')||(*p>='A'&&*p<='Z')||(*p>='0'&&*p<='9')||*p=='-'||*p=='_') encoded[k++]=*p;
                else { snprintf(encoded+k,sizeof(encoded)-k,"%%%02X",*p); k+=3; }
            }
            encoded[k]=0; snprintf(url,sizeof(url),"https://geocoding-api.open-meteo.com/v1/search?name=%s&count=1&language=en&format=json",encoded);
            if(fetch(ctx,url,body,&reachable)) { location_response=true; weather_location(body,"geocode",cfg); }
        } else {
            const char *ips[]={"https://ipwho.is/","https://ipapi.co/json/"};
            for(unsigned i=0;i<2&&!cfg->coordinates&&!asp_plugin_should_stop(ctx);i++)
                if(fetch(ctx,ips[i],body,&reachable)) { location_response=true; weather_location(body,"ip",cfg); }
        }
    }
    if(!cfg->coordinates) {
        if(!reachable&&!asp_plugin_should_stop(ctx)) { int status=0; asp_weather_http_get(ctx,"https://example.com/",body,WEATHER_BODY_CAP,&status); reachable=status>0; }
        set_status(location_response?"No weather info found":reachable?"Weather sites not available":"No Internet access");
        free(cfg); free(body); return;
    }
    bool success=false,valid_response=false;
    // User endpoints take priority, then the three built-ins in fixed order.
    for(unsigned i=0;i<cfg->custom_count+3&&!asp_plugin_should_stop(ctx);i++) {
        weather_provider provider=i<cfg->custom_count?PROVIDER_CUSTOM:(weather_provider)(i-cfg->custom_count);
        if(!weather_url(provider,cfg,i<cfg->custom_count?cfg->custom[i]:NULL,url,sizeof(url))) continue;
        if(!fetch(ctx,url,body,&reachable)) continue;
        json_span doc; if(json_document(body,&doc)) valid_response=true;
        weather_data w;
        if(weather_parse(provider,body,&w)) {
            strcpy(w.city,cfg->location);
            pthread_mutex_lock(&lock); latest=w; status_text[0]=0; pthread_mutex_unlock(&lock);
            success=true; break;
        }
    }
    if(!success&&!asp_plugin_should_stop(ctx)) {
        // A separate HTTPS probe distinguishes reachable Internet from provider outage.
        // No third-party probe is needed when any location/weather server replied.
        if(!reachable) { int status=0; asp_weather_http_get(ctx,"https://example.com/",body,WEATHER_BODY_CAP,&status); reachable=status>0; }
        set_status(valid_response?"No weather info found":reachable?"Weather sites not available":"No Internet access");
    }
    free(cfg); free(body);
}
static void run(plugin_context_t *ctx) {
    uint32_t next=started;
    bool previous_online=false;
    while(!asp_plugin_should_stop(ctx)) {
        uint32_t now=asp_plugin_get_tick_ms();
        bool online=false;
        asp_net_is_connected(&online);
        if(weather_due(now,next)) {
            update(ctx);
            do { next+=WEATHER_INTERVAL_MS; } while(weather_due(asp_plugin_get_tick_ms(),next));
        } else if(online&&!previous_online) {
            update(ctx); // Recover promptly if WiFi was not ready during autostart.
        } else if(!online&&previous_online) {
            set_status("No Internet access");
        }
        previous_online=online;
        asp_plugin_delay_ms(1000);
    }
}
static const plugin_entry_t entry={.get_info=get_info,.init=init,.cleanup=cleanup,.service_run=run};
TANMATSU_PLUGIN_REGISTER(entry);
