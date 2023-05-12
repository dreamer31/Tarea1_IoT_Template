#include <sensors.c>
#include <math.h>
#include <stdlib.h>
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_log.h"

//Genera el header de un mensaje, con la MAC, el protocolo, status, y el largo del mensaje.
char* header(char protocol, char transportLayer){
	char* head = malloc(12);

    char ID = 'D1'
    memcpy((void*) &(head[0]), (void*) ID, 2);
	uint8_t* MACaddrs = malloc(6);
	esp_efuse_mac_get_default(MACaddrs);
	memcpy((void*) &(head[2]), (void*) MACaddrs, 6);//consigue el MACaddrs
    head[8]= transportLayer;
	head[9]= protocol;
	unsigned short dataLen = dataLength(protocol); //revisa el tamano del protocolo
	memcpy((void*) &(head[10]), (void*) &dataLen, 2);
	free(MACaddrs);
	return head;
}

unsigned short lengmsg[6] = {2, 6, 16, 20, 44, 12016};
unsigned short dataLength(char protocol){
    return lengmsg[ (unsigned int) protocol]-1;
}

unsigned short messageLength(char protocol){ //calcula tamano del mensaje
    return 1+12+dataLength(protocol);
}

char* mensaje (char protocol, char transportLayer){
	char* mnsj = malloc(messageLength(protocol));
	mnsj[messageLength(protocol)-1]= '\0';
	char* hdr = header(protocol, transportLayer);//crea el mensaje completo
	char* data;
	switch (protocol) { //crea cada uno de los protocolos, cambiar
		case 0:
			data = dataprotocol00();
			break;
		case 1:
			data = dataprotocol0();
			break;
		case 2:
			data = dataprotocol1();
			break;
		case 3:
			data = dataprotocol2();
			break;
        case 4:
			data = dataprotocol3();
			break;
        case 5:
			data = dataprotocol4();
			break;
		default:
			data = dataprotocol0();
			break;
	}
	memcpy((void*) mnsj, (void*) hdr, 12);
	memcpy((void*) &(mnsj[12]), (void*) data, dataLength(protocol));
	free(hdr);
	free(data);
	return mnsj;
}

// Arma un paquete para el protocolo de inicio, que busca solo respuesta
char* dataprotocol00(){
    char* msg = malloc(dataLength(0));
    msg[0] = 1;
    return msg;
}

// Arma un paquete para el protocolo 0, con la bateria
char* dataprotocol0(){
    //seteamos el largo de la data (6 bytes)
    char* msg = malloc(dataLength(6));
	msg[0] = 1; //1 byte

    float batt = batt_sensor();
    msg[1] = batt; //1 byte
    // long t = 0;
    // memcpy((void*) &(msg[1]), (void*) &t, 4);

	time_t time = gettimeofday();
	msg[2] = time; //4 bytes
	memcpy((void*) &(msg[1]), (void*) &time, 4);

    return msg;


}

char* dataprotocol1(){
    //seteamos el largo de la data (16 bytes)
    char* msg = malloc(dataLength(16));
	msg[0] = 1; //1 byte

    float batt = batt_sensor();
    msg[1] = batt; //1 byte

	time_t time = gettimeofday();
	memcpy((void*) &(msg[2]), (void*) &time, 4); //4 bytes

	char temp = THPC_sensor_temp(); //1 byte
	msg[6] = temp;

	float press = THPC_sensor_pres();
    memcpy((void*) &(msg[7]), (void*) &press, 4); //4 bytes

	char hum = THPC_sensor_hum();
    msg[11] = hum;

	float co = THPC_sensor_co();
    memcpy((void*) &(msg[12]), (void*) &co, 4); //4 bytes

    return msg;

}

char* dataprotocol2(){
    //seteamos el largo de la data (20 bytes)
    char* msg = malloc(dataLength(20));
	msg[0] = 1; //1 byte

    float batt = batt_sensor();
    msg[1] = batt; //1 byte

	time_t time = gettimeofday();
	memcpy((void*) &(msg[2]), (void*) &time, 4); //4 bytes

	char temp = THPC_sensor_temp(); //1 byte
	msg[6] = temp;

	float press = THPC_sensor_pres();
    memcpy((void*) &(msg[7]), (void*) &press, 4); //4 bytes

	char hum = THPC_sensor_hum();
    msg[11] = hum;

	float co = THPC_sensor_co();
    memcpy((void*) &(msg[12]), (void*) &co, 4); //4 bytes

	float rms =  acc_kpi_rms();
	memcpy((void*) &(msg[16]), (void*) &rms, 4); //4 bytes

    return msg;

}

char* dataprotocol3(){
    //seteamos el largo de la data (44 bytes)
    char* msg = malloc(dataLength(44));
	msg[0] = 1; //1 byte

    float batt = batt_sensor();
    msg[1] = batt; //1 byte

	time_t time = gettimeofday();
	memcpy((void*) &(msg[2]), (void*) &time, 4); //4 bytes

	char temp = THPC_sensor_temp(); //1 byte
	msg[6] = temp;

	float press = THPC_sensor_pres();
    memcpy((void*) &(msg[7]), (void*) &press, 4); //4 bytes

	char hum = THPC_sensor_hum();
    msg[11] = hum;

	float co = THPC_sensor_co();
    memcpy((void*) &(msg[12]), (void*) &co, 4); //4 bytes

	float rms =  acc_kpi_rms();
	memcpy((void*) &(msg[16]), (void*) &rms, 4); //4 bytes

	float amp_x = acc_kpi_amp_x();
	memcpy((void*) &(msg[20]), (void*) &amp_x, 4); //4 bytes

	float frec_x = acc_kpi_frec_x();
	memcpy((void*) &(msg[24]), (void*) &frec_x, 4); //4 bytes

	float amp_y = acc_kpi_amp_y();
	memcpy((void*) &(msg[28]), (void*) &amp_y, 4); //4 bytes

	float frec_y = acc_kpi_frec_y();
	memcpy((void*) &(msg[32]), (void*) &frec_y, 4); //4 bytes

	float amp_z = acc_kpi_amp_z();
	memcpy((void*) &(msg[36]), (void*) &amp_z, 4); //4 bytes

	float frec_z = acc_kpi_frec_z();
	memcpy((void*) &(msg[40]), (void*) &frec_z, 4); //4 bytes


    return msg;

}


char* dataprotocol4(){
    //seteamos el largo de la data (24016 bytes)
    char* msg = malloc(dataLength(24016));
	msg[0] = 1; //1 byte

    float batt = batt_sensor();
    msg[1] = batt; //1 byte

	time_t time = gettimeofday();
	memcpy((void*) &(msg[2]), (void*) &time, 4); //4 bytes

	char temp = THPC_sensor_temp(); //1 byte
	msg[6] = temp;

	float press = THPC_sensor_pres();
    memcpy((void*) &(msg[7]), (void*) &press, 4); //4 bytes

	char hum = THPC_sensor_hum();
    msg[11] = hum;

	float co = THPC_sensor_co();
    memcpy((void*) &(msg[12]), (void*) &co, 4); //4 bytes

	int16_t* acc_x = acc_sensor_acc_x();
	memcpy((void*) &(msg[16]), (void*) &acc_x, 8000); //8000 bytes

	int16_t* acc_y = acc_sensor_acc_y();
	memcpy((void*) &(msg[8016]), (void*) &acc_y, 8000); //8000 bytes

	int16_t* acc_z = acc_sensor_acc_z();
	memcpy((void*) &(msg[16016]), (void*) &acc_z, 8000); //8000 bytes

    return msg;

}



'''
char* dataprotocol1(){
    
    char* msg = malloc(dataLength(2));

    float batt = batt_sensor();
    msg[0] = batt;


    int t = 0;
    memcpy((void*) &(msg[1]), (void*) &t, 4);

    char temp = THPC_sensor_temp();
    msg[5] = temp;


    float press = THPC_sensor_pres();
    memcpy((void*) &(msg[6]), (void*) &press, 4);

    char hum = THPC_sensor_hum();
    msg[10] = hum;

    float co = THPC_sensor_co();
    memcpy((void*) &(msg[11]), (void*) &co, 4);

    return msg;
}
'''