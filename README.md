# Projektowanie-blok-w-funkcjonalnych-kana-w-cyfrowych
Projektowanie bloków funkcjonalnych kanałów cyfrowych  - Projekt PG

Program należy skompilować na windowsie poleceniem:

```ps
g++ -I.\_DSPE_lib_minGW_\include\ -I.\_DSPE_lib_minGW_\include\dbg\ .\client.cpp -o .\client -L.\_DSPE_lib_minGW_\dbg\ -lDSPE -lDSPEsockets -lwinmm -lws2_32
g++ -I.\_DSPE_lib_minGW_\include\ -I.\_DSPE_lib_minGW_\include\dbg\ .\server.cpp -o .\server -L.\_DSPE_lib_minGW_\dbg\ -lDSPE -lDSPEsockets -lwinmm -lws2_32
```