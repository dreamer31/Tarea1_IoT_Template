#include <math.h>
#include <stdlib.h>
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_log.h"

/*

Aqui generamos los 5 tipos de protocolos con sus datos.
Las timestamps en realidad siempre mandamos 0, y por comodidad 
guardamos la timestampo del tiempo de llegada en el servidor de la raspberry.


En general los "mensajes" los creamos copiando a la mala (con memcpy) la memoria de los datos en un char*.
No es muy elegante pero funciona.

Al final lo único que se usa fuera de este archivo es:

message: dado un protocolo, crea un mensaje (con header y datos) codificado en un array de bytes (char*).
messageLength: dado un protocolo, entrega el largo del mensaje correspondiente

*/




float floatrand(float min, float max){
    return min + (float)rand()/(float)(RAND_MAX/(max-min));
}


int16_t* acc_sensor_acc_x(){
    int16_t* arr = malloc(2000* sizeof(int16_t));
    for (int i =0; i <2000; i++){
        arr[i] = (int16_t)floatrand(-8000, 8000);
    }
    return arr;

char THPC_sensor_temp(){
    char n =(char) 15 + (rand() %16);
    return n;
}