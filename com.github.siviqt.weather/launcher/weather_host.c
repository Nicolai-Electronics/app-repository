// SPDX-License-Identifier: MIT
#include "weather_host.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "kbelf.h"
#include "pax_gfx.h"
#include "esp_log.h"
static gui_theme_t *header_theme;
static int header_left;
static bool is_home;
void weather_host_prepare(gui_theme_t *theme,int left,bool home) { header_theme=theme; header_left=left; is_home=home; }
int asp_weather_draw(pax_buf_t *b,int right,int y,int height,const char *full,const char *page) {
    if(!is_home||!header_theme) return 0;
    gui_element_style_t *s=&header_theme->header;
    int available=right-header_left-8;
    if(available<32) return 0;
    int icon=16, gap=5;
    const char *text=full;
    float width=pax_text_size(s->text_font,s->text_height,text).x;
    if(width+icon+gap>available) { text=page; width=pax_text_size(s->text_font,s->text_height,text).x; }
    char shortened[256];
    if(width+icon+gap>available) {
        snprintf(shortened,sizeof(shortened),"%s",text);
        size_t n=strlen(shortened);
        while(n && pax_text_size(s->text_font,s->text_height,shortened).x+icon+gap>available) {
            --n; while(n && ((unsigned char)shortened[n]&0xc0)==0x80) --n;
            shortened[n]=0;
        }
        text=shortened; width=pax_text_size(s->text_font,s->text_height,text).x;
    }
    int used=(int)width+icon+gap+4;
    if(used>available) used=available;
    int x=right-used, cy=y+height/2;
    uint32_t fg=s->palette.color_foreground;
    // A compact monochrome sun/cloud glyph inherits the header foreground.
    pax_draw_circle(b,fg,x+5,cy-3,4);
    pax_draw_circle(b,fg,x+5,cy+3,3);
    pax_draw_circle(b,fg,x+10,cy+1,4);
    pax_draw_circle(b,fg,x+14,cy+4,2);
    pax_draw_rect(b,fg,x+5,cy+3,9,3);
    pax_draw_text(b,fg,s->text_font,s->text_height,x+icon+gap,y+(height-s->text_height)/2.0f,text);
    return used;
}
int asp_weather_http_get(plugin_context_t *ctx,const char *url,char *body,size_t cap,int *status) {
    if(!ctx||!url||strncmp(url,"https://",8)||!body||cap<2||!status) return -1;
    *status=0; body[0]=0;
    if(asp_plugin_should_stop(ctx)) return -1;
    esp_http_client_config_t config={.url=url,.timeout_ms=1200,.crt_bundle_attach=esp_crt_bundle_attach,
        .user_agent="TanmatsuWeather/0.1 (+https://github.com/siviqt/app-repository)",.disable_auto_redirect=true};
    esp_http_client_handle_t client=esp_http_client_init(&config);
    if(!client) return -1;
    int result=-1; size_t used=0;
    uint32_t start=asp_plugin_get_tick_ms();
    if(esp_http_client_open(client,0)!=ESP_OK || asp_plugin_should_stop(ctx)) goto done;
    int64_t len=esp_http_client_fetch_headers(client);
    *status=esp_http_client_get_status_code(client);
    if(len<0 || len>=(int64_t)cap || *status!=200) goto done;
    while(!asp_plugin_should_stop(ctx) && asp_plugin_get_tick_ms()-start<12000u) {
        if(used==cap-1) break;
        int n=esp_http_client_read(client,body+used,cap-1-used);
        if(n<0) break;
        used+=(size_t)n;
        if(esp_http_client_is_complete_data_received(client)) { result=0; break; }
        if(n==0) break;
    }
    body[used]=0;
done:
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    if(result) body[0]=0;
    return result;
}
void weather_host_init(void) {
    // Extend the existing libbadge descriptor once. No managed component edits.
    static kbelf_builtin_lib extended;
    if(extended.symbols) return;
    for(size_t i=0;i<kbelfx_builtin_libs_len;i++) {
        const kbelf_builtin_lib *old=kbelfx_builtin_libs[i];
        if(strcmp(old->path,"libbadge.so")) continue;
        kbelf_builtin_sym *symbols=malloc((old->symbols_len+2)*sizeof(*symbols));
        if(!symbols) { ESP_LOGE("weather_host","Cannot register weather extension"); return; }
        memcpy(symbols,old->symbols,old->symbols_len*sizeof(*symbols));
        symbols[old->symbols_len]=(kbelf_builtin_sym){"asp_weather_http_get",(size_t)asp_weather_http_get};
        symbols[old->symbols_len+1]=(kbelf_builtin_sym){"asp_weather_draw",(size_t)asp_weather_draw};
        extended=*old; extended.symbols=symbols; extended.symbols_len+=2;
        kbelfx_builtin_libs[i]=&extended;
        return;
    }
}
