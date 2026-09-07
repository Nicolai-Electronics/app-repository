// SPDX-License-Identifier: MIT
#include "weather.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M json_member
#define F json_first
static bool numeric(const char *s,float *v) {
    if(!*s) return false;
    char *e; *v=strtof(s,&e); return e!=s && !*e && isfinite(*v);
}
static char *trim(char *s) {
    while(*s==' '||*s=='\t'||*s=='\r') ++s;
    size_t n=strlen(s); while(n && (s[n-1]==' '||s[n-1]=='\t'||s[n-1]=='\r')) s[--n]=0;
    return s;
}
void weather_config_parse(char *text,weather_config *c) {
    memset(c,0,sizeof(*c)); bool lat=false,lon=false;
    while(text && *text) {
        char *line=text, *next=strchr(text,'\n'); if(next) *next++=0; text=next;
        line=trim(line); if(!*line||*line=='#') continue;
        char *eq=strchr(line,'='); if(!eq) { c->invalid=true; continue; }
        *eq++=0; char *key=trim(line),*v=trim(eq);
        if(!strcmp(key,"location")) {
            if(!strcmp(v,"auto")) c->manual=false;
            else { c->manual=true; if(!*v||strlen(v)>=sizeof(c->location)) c->invalid=true; else strcpy(c->location,v); }
        } else if(!strcmp(key,"latitude")) { lat=numeric(v,&c->latitude)&&c->latitude>=-90&&c->latitude<=90; if(!lat) c->invalid=true; }
        else if(!strcmp(key,"longitude")) { lon=numeric(v,&c->longitude)&&c->longitude>=-180&&c->longitude<=180; if(!lon) c->invalid=true; }
        else if(!strcmp(key,"custom_url")) {
            if(c->custom_count>=WEATHER_CUSTOM_MAX||strlen(v)>=WEATHER_URL_CAP||strncmp(v,"https://",8)) c->invalid=true;
            else strcpy(c->custom[c->custom_count++],v);
        } else c->invalid=true;
    }
    if(lat!=lon) c->invalid=true;
    c->coordinates=lat&&lon;
    if(c->coordinates) { c->manual=true; if(!c->location[0]) snprintf(c->location,sizeof(c->location),"%.3f,%.3f",c->latitude,c->longitude); }
}
static void metric(json_span s,float min,float max,float *out,unsigned bit,weather_data *w) {
    float f; if(json_number(s,&f)&&f>=min&&f<=max) { *out=f; w->valid|=bit; }
}
bool weather_parse(weather_provider p,const char *body,weather_data *w) {
    json_span root; memset(w,0,sizeof(*w)); if(!json_document(body,&root)) return false;
    json_span t={0},hi={0},lo={0},h={0},cl={0},wind={0},dir={0};
    if(p==PROVIDER_OPEN_METEO) {
        json_span c=M(root,"current"), d=M(root,"daily");
        t=M(c,"temperature_2m"); h=M(c,"relative_humidity_2m"); cl=M(c,"cloud_cover");
        wind=M(c,"wind_speed_10m"); dir=M(c,"wind_direction_10m");
        hi=F(M(d,"temperature_2m_max")); lo=F(M(d,"temperature_2m_min"));
    } else if(p==PROVIDER_WTTR) {
        json_span c=F(M(root,"current_condition")), d=F(M(root,"weather"));
        t=M(c,"temp_C"); h=M(c,"humidity"); cl=M(c,"cloudcover");
        wind=M(c,"windspeedKmph"); dir=M(c,"winddirDegree"); hi=M(d,"maxtempC"); lo=M(d,"mintempC");
    } else if(p==PROVIDER_MET_NO) {
        json_span c=M(M(M(F(M(M(root,"properties"),"timeseries")),"data"),"instant"),"details");
        t=M(c,"air_temperature"); h=M(c,"relative_humidity"); cl=M(c,"cloud_area_fraction");
        wind=M(c,"wind_speed"); dir=M(c,"wind_from_direction");
        // MET compact has no full local-day extrema. Do not substitute partial-day samples.
    } else {
        t=M(root,"temperature_c"); hi=M(root,"high_c"); lo=M(root,"low_c"); h=M(root,"humidity_pct");
        cl=M(root,"cloud_pct"); wind=M(root,"wind_kmh"); dir=M(root,"wind_degrees");
    }
    metric(t,-100,70,&w->temperature,W_TEMP,w); metric(hi,-100,70,&w->high,W_HIGH,w); metric(lo,-100,70,&w->low,W_LOW,w);
    metric(h,0,100,&w->humidity,W_HUMID,w); metric(cl,0,100,&w->cloud,W_CLOUD,w);
    metric(wind,0,p==PROVIDER_MET_NO?150:540,&w->wind,W_WIND,w); metric(dir,0,360,&w->direction,W_DIR,w);
    if(p==PROVIDER_MET_NO && (w->valid&W_WIND)) w->wind*=3.6f;
    if((w->valid&(W_HIGH|W_LOW))==(W_HIGH|W_LOW)&&w->high<w->low) w->valid&=~(W_HIGH|W_LOW);
    return w->valid!=0;
}
bool weather_location(const char *body,const char *kind,weather_config *cfg) {
    json_span r; if(!json_document(body,&r)) return false;
    json_span success=M(r,"success"),error=M(r,"error");
    if((success.p && success.end-success.p==5 && !memcmp(success.p,"false",5)) ||
       (error.p && error.end-error.p==4 && !memcmp(error.p,"true",4))) return false;
    if(!strcmp(kind,"geocode")) r=F(M(r,"results"));
    float lat,lon; char city[64];
    if(!json_number(M(r,"latitude"),&lat)||!json_number(M(r,"longitude"),&lon)||lat < -90||lat>90||lon < -180||lon>180) return false;
    if(!json_string(M(r,!strcmp(kind,"geocode")?"name":"city"),city,sizeof(city))) return false;
    cfg->latitude=lat; cfg->longitude=lon; cfg->coordinates=true; strcpy(cfg->location,city); return true;
}
bool weather_url(weather_provider p,const weather_config *c,const char *custom,char *out,size_t cap) {
    int n;
    if(p==PROVIDER_OPEN_METEO) n=snprintf(out,cap,"https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current=temperature_2m,relative_humidity_2m,cloud_cover,wind_speed_10m,wind_direction_10m&daily=temperature_2m_max,temperature_2m_min&forecast_days=1&timezone=auto&temperature_unit=celsius&wind_speed_unit=kmh",c->latitude,c->longitude);
    else if(p==PROVIDER_WTTR) n=snprintf(out,cap,"https://wttr.in/%.4f,%.4f?format=j1&num_of_days=1",c->latitude,c->longitude);
    else if(p==PROVIDER_MET_NO) n=snprintf(out,cap,"https://api.met.no/weatherapi/locationforecast/2.0/compact?lat=%.4f&lon=%.4f",c->latitude,c->longitude);
    else {
        if(!custom||strncmp(custom,"https://",8)) return false;
        size_t used=0;
        while(*custom) {
            char b[32]; const char *piece=b; size_t consume=1;
            if(!strncmp(custom,"{lat}",5)) { snprintf(b,sizeof(b),"%.4f",c->latitude); consume=5; }
            else if(!strncmp(custom,"{lon}",5)) { snprintf(b,sizeof(b),"%.4f",c->longitude); consume=5; }
            else { b[0]=*custom; b[1]=0; }
            size_t len=strlen(piece); if(used+len>=cap) return false; memcpy(out+used,piece,len); used+=len; custom+=consume;
        }
        out[used]=0; return true;
    }
    return n>0&&(size_t)n<cap;
}
static void fmt(char *b,size_t n,float v,bool valid,const char *unit) { if(valid) snprintf(b,n,"%.0f%s",v,unit); else snprintf(b,n,"n/a"); }
void weather_format(const weather_data *w,unsigned page,char *out,size_t cap) {
    char t[20],h[20],l[20],hum[20],wind[20],dir[12],cloud[32];
    fmt(t,sizeof(t),w->temperature,w->valid&W_TEMP,"°C"); fmt(h,sizeof(h),w->high,w->valid&W_HIGH,"°"); fmt(l,sizeof(l),w->low,w->valid&W_LOW,"°");
    fmt(hum,sizeof(hum),w->humidity,w->valid&W_HUMID,"%"); fmt(wind,sizeof(wind),w->wind,w->valid&W_WIND,"km/h");
    const char *dirs[]={"N","NNE","NE","ENE","E","ESE","SE","SSE","S","SSW","SW","WSW","W","WNW","NW","NNW"};
    snprintf(dir,sizeof(dir),"%s",w->valid&W_DIR?dirs[(int)(w->direction/22.5f+0.5f)%16]:"n/a");
    if(w->valid&W_CLOUD) snprintf(cloud,sizeof(cloud),"%s %.0f%%",w->cloud>=90?"Overcast":w->cloud>=60?"Cloudy":w->cloud>=20?"Partly cloudy":"Clear",w->cloud);
    else strcpy(cloud,"n/a");
    if(page==0) snprintf(out,cap,"%s %s H:%s L:%s RH:%s %s %s %s",w->city,t,h,l,hum,cloud,dir,wind);
    else if(page==1) snprintf(out,cap,"%s %s H:%s L:%s",w->city,t,h,l);
    else snprintf(out,cap,"RH:%s %s %s %s",hum,cloud,dir,wind);
}
bool weather_due(uint32_t now,uint32_t deadline) { return (int32_t)(now-deadline)>=0; }
