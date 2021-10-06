# tp1
## Amando Tomás Civini
## https://github.com/ArmandoCivini/tp1.git
Para este TP se usó c en su totalidad. Haciendo uso de sus structs se crearon 5 tipos de datos abstractos con sus respectivas funciones para simplificar y construir el trabajo por bloques. 3 de estos tipos de datos abstractos cumplen la funcion de administrar la comunicacion entre el servidor y el cliente haciendo uso de sockets. En el siguiente diagrama se muestra los tipo de datos abstractos usados para esto y en donde son dependencia.

![diagrama estructural](diagrama_dep.png)

### TDA Ahorcado
La primera parte del trabajo práctico que, se contruyó y se testeó, fue el TDA del ahorcado. Este comenzó simplemente como una palabra y la cantidad de intentos guardados en la estructura la cual mediante que se escribía el codigo se fueron agregando campos necesarios como el largo de la palabra, las letras reveladas actualmente.  Ambas palabras que se guardan en el dato yacen en el heap por lo cual cuenta con una función de destrucción. Finalmente éste cuenta con una función para probar un caracter a la vez, la cual devuelve si ha ganado o no, lo que va revelado de la palabra(y en el caso de perder la palabra entera) y la cantidad de intentos restantes. Tambien este TDA tiene la opcion comenzar otro juego y esto lo hace mediante la funcion nueva_palabra() la cual guarda la nueva palabra en el struct y reinicia el estado del juego. De esto hace uso el servidor al leer una nueva linea del archivo de texto.
### TDA Servidor
Este TDA contiene toda la informacion que requiere el servidor para operar y se encarga de manejar los recursos como el Sockt y el Ahorcado y de manejar la logica del juego. Cuenta con una funcion de destruccion que se encarga tambien de destruir los TDA que Servidor maneja.
### TDA Sockt
El TDA sockt esta diseñado para uso generico de un socket, por esto implementa solamente un inizializador, una funcion para enviar un mensaje otra para recivirlo y un destructor. Este TDA se usa tanto en el cliente como en el servidor para enviar y recivir mensajes entre estos.
### TDA Srv_sockt
Este TDA se encarga de crear el soccket del servidor que sera usado para escuchar conexiones y aceptarlas. El servidor hace uso de este TDA para hacerse presente y a medida que va podiendo procesar clientes usa la funcion de aceptar de este TDA para formar conexiones con ellos. La funcion de aceptar devuelve un file descriptor para ser usado en un TDA Sockt. Este TDA tambien cuenta con funcion de destruccion.
### TDA Sockt_connect
El TDA Sockt_connect es usado por el cliente para inicializar la informacion de adress del servidor al que se quiere conectar para asi conectarse con este. la funcion sockt_connect_conection() tambien devuelve un file descriptor para ser usado con un TDA Sockt y asi intercambiar informacion con el servidor. 
