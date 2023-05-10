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



/* Para un float aleatorio en el intervalo [min, max]*/


float floatrand(float min, float max){
    return min + (float)rand()/(float)(RAND_MAX/(max-min));
}

/*  
    Funciones que simulan el Acceloremeter_Sensor

    El M_PI revisar si funciona**

    Devuelve un array con los 200 datos. 
*/

int16_t* acc_sensor_acc_x(){
    int16_t* arr = malloc(2000* sizeof(int16_t));
    for (int i =0; i <2000; i++){
        arr[i] = (int16_t)2 * sin(2 * M_PI * 0.001 * floatrand(-8000, 8000));
    }
    return arr;
}

int16_t* acc_sensor_acc_y(){
    int16_t* arr = malloc(2000* sizeof(int16_t));
    for (int i =0; i <2000; i++){
        arr[i] = (int16_t)3 * cos(2 * M_PI * 0.001 * floatrand(-8000, 8000));
    }
    return arr;
}

int16_t* acc_sensor_acc_z(){
    int16_t* arr = malloc(2000* sizeof(int16_t));
    for (int i =0; i <2000; i++){
        arr[i] = (int16_t)10 * sin(2 * M_PI * 0.001 * floatrand(-8000, 8000));
    }
    return arr;
}


/*  
    Funciones que simulan el THPC_Sensor
    Existe una para cada parametro.
    - temperatura
    - humedad
    - presion
    - co2
*/

char THPC_sensor_temp(){
    char n =(char) 5 + (rand() %26);
    return n;
}

char THPC_sensor_hum(){
    float n =(float) floatrand(30, 80)
    // (float) 30 + (rand() %51);  <-- Esta no genera decimales creo.
    return n;
}

char THPC_sensor_pres(){
    float n =(char) 1000 + (rand() %201);
    return n;
}

char THPC_sensor_co2(){
    float n =(float) floatrand(30, 200)
    return n;
}

/*  
    Funciones que simulan el Batt_sensor
    nivel de bateria del aparato.
*/

char batt_sensor(){
    char n =(char) rand() %101
    return n;
}

float acelerometer_kpi() {
    float amp_x = (float) floatrand(0.0059, 0.12)
    float frec_x = (float) floatrand(0.0059, 0.12)
    float amp_y = (float) floatrand(29, 31)
    float frec_y = (float) floatrand(0.0041, 0.11)
    float amp_z = (float) floatrand(59, 61)
    float frec_z = (float) floatrand(89, 91)
    float RMS = (float) sqrt(pow(amp_x, 2) + pow(amp_y, 2) + pow(amp_z, 2));
    return RMS;
}