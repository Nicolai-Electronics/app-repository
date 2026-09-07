#include "weather.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char **argv) {
 if(argc!=3)return 2; FILE *f=fopen(argv[2],"rb"); if(!f)return 2;
 char *b=calloc(1,WEATHER_BODY_CAP); size_t n=fread(b,1,WEATHER_BODY_CAP-1,f); fclose(f);
 weather_data w; int ok=weather_parse((weather_provider)atoi(argv[1]),b,&w); free(b);
 if(!ok)return 1; printf("%s: %zu bytes, metrics 0x%x, %.1f C, wind %.1f km/h\n",argv[2],n,w.valid,w.temperature,w.wind); return 0;
}
