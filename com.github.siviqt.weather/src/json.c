// SPDX-License-Identifier: MIT
// Bounded, allocation-free JSON traversal. Validate once, then select spans.
#include "weather.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
static const char *ws(const char *p, const char *e) {
    while(p<e && (*p==' ' || *p=='\n' || *p=='\r' || *p=='\t')) ++p;
    return p;
}
static int hex(char c) { return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:c>='A'&&c<='F'?c-'A'+10:-1; }
static const char *strend(const char *p,const char *e) {
    if(p==e || *p++!='"') return NULL;
    while(p<e) {
        unsigned char c=*p++;
        if(c=='"') return p;
        if(c<32) return NULL;
        if(c=='\\') {
            if(p==e) return NULL;
            c=*p++;
            if(c=='u') { for(int i=0;i<4;i++) if(p==e || hex(*p++)<0) return NULL; }
            else if(!strchr("\"\\/bfnrt",c)) return NULL;
        }
    }
    return NULL;
}
static const char *value(const char *p,const char *e,unsigned depth) {
    p=ws(p,e);
    if(p==e || depth>24) return NULL;
    if(*p=='"') return strend(p,e);
    if(*p=='{' || *p=='[') {
        bool object=*p++=='{'; char close=object?'}':']'; p=ws(p,e);
        if(p<e && *p==close) return p+1;
        for(;;) {
            if(object) { p=strend(p,e); if(!p) return NULL; p=ws(p,e); if(p==e || *p++!=':') return NULL; }
            p=value(p,e,depth+1); if(!p) return NULL; p=ws(p,e);
            if(p==e) return NULL;
            if(*p==close) return p+1;
            if(*p++!=',') return NULL;
            p=ws(p,e);
        }
    }
    const char *words[]={"true","false","null"};
    for(unsigned i=0;i<3;i++) { size_t n=strlen(words[i]); if((size_t)(e-p)>=n && !memcmp(p,words[i],n)) return p+n; }
    if(*p=='-') ++p;
    if(p==e) return NULL;
    if(*p=='0') ++p;
    else { if(*p<'1'||*p>'9') return NULL; do { ++p; } while(p<e && *p>='0'&&*p<='9'); }
    if(p<e && *p=='.') { ++p; if(p==e||*p<'0'||*p>'9') return NULL; while(p<e&&*p>='0'&&*p<='9') ++p; }
    if(p<e && (*p=='e'||*p=='E')) { ++p; if(p<e&&(*p=='+'||*p=='-')) ++p; if(p==e||*p<'0'||*p>'9') return NULL; while(p<e&&*p>='0'&&*p<='9') ++p; }
    return p;
}
bool json_document(const char *text,json_span *out) {
    const char *e=text+strlen(text), *p=ws(text,e), *q=value(p,e,0);
    if(!q || ws(q,e)!=e) { *out=(json_span){0}; return false; }
    *out=(json_span){p,q}; return true;
}
json_span json_member(json_span obj,const char *key) {
    if(!obj.p || *obj.p!='{') return (json_span){0};
    const char *p=ws(obj.p+1,obj.end); size_t n=strlen(key);
    while(p<obj.end && *p!='}') {
        const char *k=p, *ke=strend(p,obj.end); if(!ke) break;
        p=ws(ke,obj.end); if(p==obj.end||*p++!=':') break;
        p=ws(p,obj.end); const char *end=value(p,obj.end,0); if(!end) break;
        if((size_t)(ke-k)==n+2 && !memcmp(k+1,key,n)) return (json_span){p,end};
        p=ws(end,obj.end); if(p==obj.end||*p++!=',') break; p=ws(p,obj.end);
    }
    return (json_span){0};
}
json_span json_first(json_span a) {
    if(!a.p || *a.p!='[') return (json_span){0};
    const char *p=ws(a.p+1,a.end); if(p==a.end||*p==']') return (json_span){0};
    const char *e=value(p,a.end,0); return e?(json_span){p,e}:(json_span){0};
}
bool json_number(json_span v,float *out) {
    if(!v.p) return false;
    const char *p=v.p,*e=v.end;
    if(*p=='"') { ++p; --e; }
    size_t n=e-p; if(!n||n>=48) return false;
    char b[48]; memcpy(b,p,n); b[n]=0; char *tail;
    float f=strtof(b,&tail); if(tail!=b+n||tail==b||!isfinite(f)) return false;
    *out=f; return true;
}
bool json_string(json_span v,char *out,size_t cap) {
    if(!v.p || *v.p!='"' || cap<1) return false;
    const char *p=v.p+1,*e=v.end-1; size_t n=0;
    while(p<e) {
        unsigned c=(unsigned char)*p++;
        if(c=='\\') {
            c=(unsigned char)*p++;
            if(c=='u') {
                c=0; for(int i=0;i<4;i++) c=(c<<4)|(unsigned)hex(*p++);
                if(c>=0xd800&&c<=0xdfff) return false;
                if(c>=128) {
                    unsigned need=c<2048?2:3; if(n+need>=cap) return false;
                    if(need==3) out[n++]=(char)(0xe0|(c>>12));
                    out[n++]=(char)((need==2?0xc0:0x80)|((c>>6)&(need==2?31:63)));
                    out[n++]=(char)(0x80|(c&63)); continue;
                }
            } else if(c=='n'||c=='r'||c=='t'||c=='b'||c=='f') c=' ';
        }
        if(c<32 || c==127) c=' ';
        if(n+1>=cap) return false;
        out[n++]=(char)c;
    }
    out[n]=0; return n>0;
}
