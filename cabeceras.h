#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>




 
 
typedef struct
{
     int sock_cli; /*4 bytes*/
     int sock_serv; /*4 bytes*/
     char activo[50]; /* 50 bytes ?*/
     //int activo=0; ????
     /*54 bytes*/
     /*
     int n_op;
     char opciones[100];
     //158bytes          
       */  
}CLIENTE;














typedef struct
{
int tipo;
int sec_trozo;
char datos[512]; /*mirar char datos[512]; 513???*/
/*8+512 = 520*/
}DATOS;

typedef struct
{
int tipo;
int sec_trozo;
/*8 bytes*/
}ACK;

typedef struct
{
int tipo;
int cod_err;
char msje[50];
/*8+50=58*/

}ERROR_PAQ;


typedef struct
{
int tipo;
int n_op;// n de opciones
char opciones[100];  // cadena de opciones y numero
/*8+100= 108*/
}OACK;
