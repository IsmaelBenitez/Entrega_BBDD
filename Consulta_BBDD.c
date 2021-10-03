#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char identificador[20]="1";
	char consulta [80];
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexiￃﾳn: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "BBDD",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexiￃﾳn: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	// consulta SQL para obtener una tabla con todos los datos
	// de la base de datos
	strcpy(consulta,"SELECT Jugador.Partidas_ganadas, Jugador.Partidas_jugadas FROM Jugador WHERE Identificador = '");
	strcat (consulta, identificador);
	strcat (consulta,"'");
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de un jugador.
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	// En una fila hay tantas columnas como datos tiene un
	// jugador. En nuestro caso hay dos columnas: Partidas_ganadas(row[0]) y
	// Partidas_jugadas(row[1]) .
	if (row == NULL)
		printf ("No se han obtenido datos en la consulta\n");
	else
	{
		int Partidas_ganadas=atoi(row[0]);
		int Partidas_jugadas=atoi(row[1]);
		float resultado=(Partidas_ganadas/Partidas_jugadas)*100;
		printf("Ganadas: %d \n",Partidas_ganadas);
		printf("Jugadas: %d \n",Partidas_jugadas);
		printf("El ratio de victorias es: %f \n",resultado);
	}
	// cerrar la conexion con el servidor MYSQL 
	mysql_close (conn);
	exit(0);
	
}

