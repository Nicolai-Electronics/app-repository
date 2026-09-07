#include "weather.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void) {
    json_span j;
    const char *bad[]={"", "{", "{\"x\":}", "{\"x\":1,}", "[1,]", "[01]", "[1.]", "[1e]", "true false", "{\"a\":1}junk", "[NaN]", "{\"a\":\"\\uXY00\"}", "{\"a\":\"\n\"}"};
    for(unsigned i=0;i<sizeof(bad)/sizeof(*bad);i++) assert(!json_document(bad[i],&j));
    assert(json_document("{\"x\": [{\"v\": 0}],\"name\":\"M\\u00fcnchen\"}",&j));
    float f=1; assert(json_number(json_member(json_first(json_member(j,"x")),"v"),&f)&&f==0);
    char b[256]; assert(json_string(json_member(j,"name"),b,sizeof(b))&&!strcmp(b,"München"));
    weather_data w;
    assert(weather_parse(PROVIDER_OPEN_METEO,"{\"current\":{\"temperature_2m\":0,\"relative_humidity_2m\":0,\"cloud_cover\":100,\"wind_speed_10m\":0,\"wind_direction_10m\":360},\"daily\":{\"temperature_2m_max\":[2],\"temperature_2m_min\":[-5]}}",&w));
    assert(w.valid==127&&w.temperature==0&&w.low==-5);
    strcpy(w.city,"Test"); weather_format(&w,0,b,sizeof(b)); assert(strstr(b,"0°C")&&strstr(b,"Overcast")&&strstr(b,"N 0km/h"));
    assert(weather_parse(PROVIDER_WTTR,"{\"current_condition\":[{\"temp_C\":\"-2\",\"humidity\":\"80\",\"windspeedKmph\":\"10\"}],\"weather\":[{\"maxtempC\":\"2\",\"mintempC\":\"-5\"}]}",&w));
    assert(w.temperature==-2&&w.wind==10&&!(w.valid&W_CLOUD));
    assert(weather_parse(PROVIDER_MET_NO,"{\"properties\":{\"timeseries\":[{\"data\":{\"instant\":{\"details\":{\"air_temperature\":12,\"wind_speed\":10}}}}]}}",&w));
    assert(w.wind==36&&!(w.valid&(W_HIGH|W_LOW)));
    assert(weather_parse(PROVIDER_CUSTOM,"{\"temperature_c\":null,\"humidity_pct\":101,\"wind_kmh\":-2,\"cloud_pct\":0}",&w));
    assert(w.valid==W_CLOUD); weather_format(&w,0,b,sizeof(b)); assert(strstr(b,"n/a"));
    assert(!weather_parse(PROVIDER_CUSTOM,"{\"error\":\"bad\"}",&w));
    assert(!weather_parse(PROVIDER_CUSTOM,"{\"temperature_c\":\"nan\"}",&w));
    assert(weather_parse(PROVIDER_CUSTOM,"{\"temperature_c\":1,\"high_c\":-5,\"low_c\":10}",&w)&&w.valid==W_TEMP);
    weather_config c; char config[]="location=Test\r\nlatitude=0\nlongitude=0\ncustom_url=https://example.com/{lat}/{lon}\n";
    weather_config_parse(config,&c); assert(c.coordinates&&c.manual&&!c.invalid&&c.latitude==0);
    assert(weather_url(PROVIDER_CUSTOM,&c,c.custom[0],b,sizeof(b))&&!strcmp(b,"https://example.com/0.0000/0.0000"));
    assert(!weather_url(PROVIDER_CUSTOM,&c,c.custom[0],b,8));
    char invalid[]="latitude=80\n"; weather_config_parse(invalid,&c); assert(c.invalid);
    char invalid2[]="latitude=nan\nlongitude=0\n"; weather_config_parse(invalid2,&c); assert(c.invalid);
    char auto_cfg[]="location=auto\n"; weather_config_parse(auto_cfg,&c); assert(!c.invalid&&!c.manual);
    assert(!weather_location("{\"city\":\"Test\",\"latitude\":100,\"longitude\":0}","ip",&c));
    assert(weather_location("{\"city\":\"Test\",\"latitude\":0,\"longitude\":0}","ip",&c));
    assert(c.coordinates&&!strcmp(c.location,"Test"));
    assert(!weather_location("{\"results\":[]}","geocode",&c));
    assert(weather_due(0x10,0xfffffff0)); assert(!weather_due(0xfffffff0,0x10));
    assert(weather_due(WEATHER_INTERVAL_MS,WEATHER_INTERVAL_MS));
    puts("Core tests passed");
}
