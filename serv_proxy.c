#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <strings.h>
#include <errno.h>
#include <netdb.h>
#include "cabeceras.h"
#include <errno.h>

#define MAXLINE 512

extern int errno;

void analizar_peticion(char *peticion, char *peticion2 , char *nombre_host);

int  main()
{
    int sock_cli; //socket cliente
    int sock_ser; //socvket servidor
    int escucha; // socket escucha
    int reuse = 1;
    int n ;
    int i = 0;//contador auxiliar
    int k = 0;//contador auxiliar
    int addrlen;//longitud de sockaddr_in
    char peticion[1024];//peticion del cliente
    char peticion2[1024];//peticion del servidor-proxy
    //char *respuesta;//  [100]//respuesta de�l ssservidor
    char respuesta[4096]; //1024      2048    4096
    char nombre_host[100];//cadena para el nombre de host
    struct sockaddr_in dirserv; //direccion del sevidor
    struct sockaddr_in dircli;  //direccion del cliente
    struct sockaddr_in dir_escucha;  //direccion del mia de escucha
    struct hostent *Host; //estructura que guarda el host
    int longitud = 1024; //longitud de la cadena  peticion y peticion2
    int Max_Cli = 5;
    CLIENTE Vec_cli[Max_Cli];

    fd_set setuno;
    fd_set p_leer;     /*conjunto de descriptores en lectura*/
    fd_set p_escribir; /*conjunto de descriptores en escritura*/
    fd_set excepcion; /*conjunto de descriptores en excepci�n */
    struct timeval uno; 
    struct timeval *p_tiempo; /* temporizaci�n (timeout) */


   /* p_leer = &setuno;
    p_escribir = &setuno;
    excepcion  = &setuno;  */
    p_tiempo= &uno;    


     //inicializo el timer
     p_tiempo->tv_sec=5;
     p_tiempo->tv_usec=0;
    

     addrlen = sizeof(struct sockaddr_in);

    
/*Socket Escucha************************/     
printf( "\n*********************** SockertEscucha \n  " );
        bzero(&dir_escucha, sizeof(struct sockaddr_in));//pone a cero los bytes de dir 
	dir_escucha.sin_family = AF_INET;
        dir_escucha.sin_port = htons (8080);//puerto de escucha
        dir_escucha.sin_addr.s_addr = INADDR_ANY; 


      escucha = socket(AF_INET, SOCK_STREAM, 0);//(int dominio, int tipo, int protocolo);
       // PF_INET            Protocolos de Internet IPv4
       // SOCK_STREAM  Para conceciones confiables y bidireccionales
       //0 xk solo hayu protooclo que admita ese conector


    n = setsockopt( escucha, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
     /*Hacemos el bind, para dar un nombre a un conector*/
    n = bind (escucha, (struct sockaddr *)&dir_escucha, sizeof(dir_escucha));
        //se da al coneector sock la direccion dircli

/***Socket Escucha*********************************/


/***Socket Cliente*********************************/
 printf( "\n*********************** Sockert Cliente \n  " );     
	/* Rellenamos la estructura de direccion*/
	bzero(&dircli, sizeof(struct sockaddr_in));//pone a cero los bytes de dir

	dircli.sin_family = AF_INET;
        dircli.sin_addr.s_addr = INADDR_ANY; 


//***********sacado de socket cliente
 // sock_cli = socket(AF_INET, SOCK_STREAM, 0);




        //(int dominio, int tipo, int protocolo);
       // PF_INET: Protocolos de Internet IPv4|| SOCK_STREAM :Para conceciones confiables y bidireccionales
       //0 xk solo hayu protooclo que admita ese conector
 
     /*Hacemos el bind, para dar un nombre a un conector*/
  //  n = bind (sock_cli, (struct sockaddr *)&dircli, sizeof(dircli));
        //se da al coneector sock la direccion dircli

/*** Fin Socket Cliente*********************************/

 //printf( "\n*********************** Escuchando \n  " );
      

//(1)
//Empezamos con k =0
  while(k < Max_Cli)
 {
     /*conjunto de descriptores en lectura*/
   // fd_set *p_escribir; /*conjunto de descriptores en escritura*/
   // fd_set *excepcion; /*conjunto de descriptores en excepci�n */

           printf( "\n *********** Dentro del while grandote "); 
           printf( "\n *********** El valor de k es  %i \n", k); 

        FD_ZERO (&p_leer);
        FD_SET(escucha,&p_leer); 
         
        listen (escucha,5);// Se usa listen ????   
        //n clientes por 2 sockets + escucha +1?????              escucha
        n= select((Max_Cli * 2)+escucha+1,&p_leer, NULL, NULL, p_tiempo);

         

	       if (errno == EWOULDBLOCK)
            {  
	          printf("Se agoto el timer han pasado  seg\n");
			}

        printf( "\n *********** El valor de n es  %i \n", n); 
    if (n > 0)
    { 
               if (FD_ISSET(escucha,&p_leer)== 1) 
               { 
                       //***********sacado de socket cliente
                    Vec_cli[k].sock_cli = socket(AF_INET, SOCK_STREAM, 0);// Vec_cli[k].sock_cli = socket..... ;
                    Vec_cli[k].sock_cli = accept(escucha,(struct sockaddr *)&dircli, &addrlen );
                    printf( "\n \n*** Se acepta la coneccion en el sock_cli \n ");
                    FD_SET(Vec_cli[k].sock_cli,&p_leer); // Vec_cli[k].sock_cli 
        /*
      int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
      s: conector creado con socket|| addr:puntero a una estructura sockaddr||addrlen:tamanno de la estructura apuntada por addr;
	*/
                    k ++; //Aumento los clientes 
                }//fin if(FD_ISSET(escucha))
        }

// }  //while 

       /*    for (i=0; i< Max_Cli; i++)
            {
                FD_SET(sock_cli,&p_leer); 
             }*/

/********FOR UNO***************/
        for (i=0; i<1/* Max_Cli*/; i++)
        {
            if (FD_ISSET(Vec_cli[k].sock_cli,&p_leer) ==1 )  
	        {
              printf( "\n**********Dentro del primer for %i \n  ",i );
                  //longitud = 1024
                  n = read(Vec_cli[k].sock_cli,peticion,longitud);
	/*Aux = read (Descriptor, Datos,  Longitud );
           Descriptor : del fichero / socket del que se quiere leer|| Datos Puntero a char donde se almacenar�
           los datos le�os|| Longitud: Nmero de caracteres que se quieren leer.
	*/

     // printf( "\n*********************** llegue a anallizar peticion   \n  " );
                  analizar_peticion(peticion,peticion2 ,nombre_host);
                     printf( "\n \n********* El nombre del host es  ===%s*** \n ", nombre_host );
       	/*Se hara el cambio de "Proxy-Connection" de peticion  por  Connection
	 y se guardara en peticion2. */
                     printf( "\n \n*** longitud nombre_host   %i = 14 \n ",strlen( nombre_host) );
           
                  Host = (struct hostent * )gethostbyname(nombre_host);/*("www.google.com") */;//dirserv
 //Da error porque en /etc/ en al archivo hosts  falta la direccion de www.google.com
                   if (Host == NULL)
                    {
		       perror ("Error en la obtencion del host \n");

		    }
    /************** Socket Servidor ***********************/
    /* Rellenamos la estructura de direccion    Se puede llevar fuera?????     */
	bzero(&dirserv, sizeof(struct sockaddr_in));//pone a cero los bytes de dirserv

	dirserv.sin_family = AF_INET;
                                  //cambiar puerto para cada servidor ??? 
        dirserv.sin_port = htons (80);//puerto de envio y recepcion de datos 

        dirserv.sin_addr.s_addr = INADDR_ANY;

//dirserv.sin_addr.s_addr =  ((struct in_addr*)(Host->h_addr))->s_addr;
   // dirserv.sin_addr.s_addr = inet_addr(inet_ntoa( *( struct in_addr*)( Host -> h_addr)));
       
                       bcopy(Host->h_addr, &dirserv.sin_addr, Host->h_length);
                      printf( "\n**********Despues de copiar el Host en dirser \n  " );


                    Vec_cli[k].sock_ser = socket(AF_INET, SOCK_STREAM, 0);// Vec_cli[i].sock_ser 

        	    n = setsockopt( Vec_cli[k].sock_ser, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
     /*Hacemos el bind, para dar un nombre a un conector*/
        	    n = bind (Vec_cli[k].sock_ser, (struct sockaddr *)&dirserv, sizeof(dirserv));
        //se da al conector sock la direccion dirserv
    /*Nos conectamos con el servidor*/
        	    if (connect (Vec_cli[k].sock_ser, (struct sockaddr *)&dirserv,addrlen) == -1)
        	    {
                      perror ("Error en connect\n");
        	    }
        /*
	int connect (Descriptor, (struct sockaddr *)&Direccion,addrlen)
	Descriptor: conector  sido creado con socket|| Direccion:puntero a una estructura sockaddr
        addrlen:el tama� de la  estructura apuntada por addr
	*/      
                                      

       // printf( "\n**********Se ha conectado con el HOST \n  " );
        	longitud = strlen(peticion2);// error con paginas de fotos

                n= write (Vec_cli[k].sock_ser, peticion2, longitud);

                if (n <=0)
                 {
                   printf ("Paso si es 0 no escribio y si es -1 es error = %i",n );
                      }

                printf( "\n**********Se escribio la peticion2 al Host \n  " );
                 FD_SET(Vec_cli[k].sock_ser,&p_leer); 

                }//if (FD_ISSET(sock_cli))   
                } //for 

             printf( "\n**********Antres del bucle For  \n  " );
//pas peticion y peti3 2 y respuesta
                 //   memset(argu->paquete->data,'\0',sizeof(argu->paquete->data));

          // for (i=0; i< Max_Cli; i++)
           // {
 //               FD_SET(sock_ser,&p_leer); 
           //  }
/********FOR DOS***************/
           for (i=0; i< 1/*Max_Cli*/; i++) 
            {
                printf( "\n**********Dentro del segundo for %i \n  ",i );

             if (FD_ISSET(Vec_cli[k].sock_ser,&p_leer))  
	          {         //longitud = 1024
   
    
             while (n>0)
               {  	
        	longitud = 4096;//2048;//1024;
        
        // printf( "\n**********Antes de leer respuesta \n  " );
        	 n = read(Vec_cli[k].sock_ser,respuesta,longitud);// Vec_cli[i].sock_ser
                     printf("\n \n=========================================================== \n");
                      printf ("\n Se recibio en read %i bytes \n",n );
                   if (n >0)
                    {

                      longitud = n;//strlen(respuesta);// error con paginas de fotos usar n???
                      n= write(Vec_cli[k].sock_cli, respuesta,n);// longitud);
         	      printf ("\n Se escribieron al cliente %i bytes \n",n );

                      printf("\n Esta es larespuesta del host : %s \n \n",respuesta);
                    }

                } //fin while
               }//             if (FD_ISSET(sock_ser))  

              }  //for 
  }  //while    aqui el while ???


//cerrar sockets con bucle
    close(Vec_cli[k].sock_cli);
    close(Vec_cli[k].sock_ser);
         printf("\n Cierro las conexiones \n");
     return (0);
	}
	
	
 void analizar_peticion(char *peticion, char *peticion2 , char *nombre_host)
	{
	  char *aux;
          //char aux[100];
	  int i=0;
          int j=0;
          int k =0;
           int longitud;
	  
          printf( "\n*********************** estoy en anallizar peticion   \n  " );
	  //*nombre_host =""; // error?
	  aux = strstr(peticion,"Host:" );//devuelvce un puntero de la palbara Host en peticion
	  //i = strcspn(":",aux);//devuelve la posicion de : en aux
          aux = strstr(aux,":" );
           //nombre_host[0]="";
          i++; // xk quiero el siguiente caracter a ":"
         // borrar el espacio en balnco
            i++;
	 printf( "\n******El bvalor de i es  %i  \n  ",i );
         //   \n
	   while(aux[i] !='\r')
	   {
            
            
            nombre_host[j]=aux[i]; 
            j++;
	    //nombre_host= strncat(nombre_host,aux[i],1);//termina la cadena con cero
            i++; 
	   }
           nombre_host[j]= '\0';

           //printf( "\n******sali del bucle  \n  " ); 
           printf("\n \n Esta es el nombre delservidor === %s \n",nombre_host);
    	aux=NULL;
    	//verifico si la cadena "Proxy..." esta en peticion
    	aux = strstr(peticion,"Proxy-Connection:");
	printf("\n \n En peticion esta esto ====  %s \n",peticion);

	if (aux != NULL)
    	{    	//i = (int) strcspn( peticion, "Proxy-Connection:");//devuelve posicion de Prox..
		// "Proxy-Connection:"
                j=strlen(aux);
                i= strlen(peticion);
                i= i - j;//Posicion de Proxy.... en la cadebna peticion
 
               strncpy(peticion2,peticion,i);//se copian los i caracteres de peticion
               printf( "\n******Dentro del if el valor de i es  %i  \n  ",i ); 
               //peticion2[i]= '\0';//cierro la cadena
                //
                
		 longitud = strlen("Proxy-Connection:");
 	          i = i + longitud;//longitud despues de Proxy....
                 printf("\n \n En peticion2 esta esto ====  %s \n",peticion2);
                //Annado Connection a mi cadena 

                 j= strlen (peticion2);
                 aux = "Connection:";// \n ???
                 longitud = strlen(aux);

                 while( longitud > 0)
	   	  { peticion2[j] = aux[k];
          	    j++;
                    k++;
                    longitud--;
                 printf( "\n****** 1Dentro del while longitud= %i,j= %i , k= %i  \n",longitud,j,k );
                  //printf( "\n******La longitud de peticion2 es %i \n",strlen (peticion2));

                 //printf("\n \n En peticion2 con Connection esta esto ====  %s \n",peticion2);  
      	    	  }  
                   printf("\n \n fuera del bucle %s \n \n");
                     printf( "\n******Antes del 2 strcat %i  \n  ",i );
                    peticion2[j] = '\0';
		 //strcat(peticion2,"Connection:");
               
                 printf("\n \n En peticion2 con Connection esta esto ====\n  %s \n \n",peticion2);

   //Annado Connection a mi cadena 

                 j= strlen (peticion2);
                 aux = &peticion[i];// \n ???
                 printf("\n \n En aux tenemos  %s \n", aux);
                 longitud = strlen(aux);
                 k=0;  
                 while( longitud > 0)
	   	  { peticion2[j] = aux[k];
          	    j++;
                    k++;
                    longitud--;   
                // printf( "\n****** 2Dentro del while longitud= %i,j= %i , k= %i  \n",longitud,j,k );
                 // printf( "\n******La longitud de peticion2 es %i \n",strlen (peticion2));

                 //printf("\n \n En peticion2 con Connection esta esto ====  %s \n",peticion2);  
  
	    	  }  
                   peticion2[j] = '\0';
		 //strcat(peticion2,peticion[i]);// da un error?? copiar con bucle while??
                 printf("\n \n En peticion2 finalmente  esta esto ==== \n %s \n",peticion2); 
  		} 
		 else
		 {
		 printf("Desde el navegador viene conecttion \n");
		 }
	 
		 printf("\n \n Esta es el nombre del host : %suno \n",nombre_host);
		 printf("Esta es la peticion original :\n %s \n",peticion);	 
		 printf("Esta es la peticion2 :\n %s \n",peticion2);
             
	}
/*void analizar_peticion(char *peticion, char *peticion2 , char *nombre_host)
{
}*/