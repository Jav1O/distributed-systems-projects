# Práctica Final Sistemas Distribuidos - Servicio de envío de mensajes

> Nota para Cursor: la Parte 1 es obligatoria y debe implementarse primero. La Parte 2 amplía la Parte 1 con SENDATTACH/GETFILE, servicio web y RPC. No implementar la Parte 2 hasta que la Parte 1 esté terminada, compilando y probada.


---

# PARTE 1 - Servicio de envío de mensajes


## Página 1


Universidad Carlos III de Madrid
Departamento de Inform´atica
´Area de Arquitectura y Tecnolog´ıa de Computadores
Sistemas Distribuidos
Pr´actica. Servicio de env´ıo de mensajes
Parte 1
Grado de Ingenier´ıa en Inform´atica
Grupo docente de Sistemas Distribuidos
Curso 2025-2026


## Página 2


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
´Indice
1
Objetivo
3
2
Descripci´on de la funcionalidad
3
3
Primera parte
4
3.1
Desarrollo del servicio
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
4
4
Prerrequisitos
4
5
Ejecuci´on y uso de la interfaz
4
6
Desarrollo del cliente
5
6.1
Finalizar la ejecuci´on del cliente . . . . . . . . . . . . . . . . . . . . . . . . . . .
5
6.2
Registro en el sistema . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
5
6.3
Darse de baja en el sistema
. . . . . . . . . . . . . . . . . . . . . . . . . . . . .
6
6.4
Conectarse al sistema . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
6
6.5
Desconectarse del sistema
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
8
6.6
Env´ıo de un mensaje . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
8
6.7
Recepci´on de mensajes . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
10
6.8
Petici´on de usuarios conectados . . . . . . . . . . . . . . . . . . . . . . . . . . .
10
7
Desarrollo del servidor
11
7.1
Uso del servidor . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
11
7.2
Registro de un cliente . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
11
7.3
Baja de un cliente . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
12
7.4
Conexi´on de un cliente . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
12
7.5
Desconexi´on de un cliente
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
13
7.6
Env´ıo de un mensaje . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
14
7.7
Solicitud de usuarios conectados . . . . . . . . . . . . . . . . . . . . . . . . . . .
14
8
Protocolo de comunicaci´on
15
8.1
Registro . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
15
8.2
Baja . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
16
8.3
Conexi´on . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
16
8.4
Desconexi´on . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
16
8.5
Env´ıo de un mensaje cliente-servidor
. . . . . . . . . . . . . . . . . . . . . . . .
17
8.6
Env´ıo de un mensaje servidor cliente
. . . . . . . . . . . . . . . . . . . . . . . .
17
8.7
Solicitud de usuarios conectados . . . . . . . . . . . . . . . . . . . . . . . . . . .
18
1


## Página 3


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
9
Normas generales
18
9.1
Calificaci´on de la pr´actica
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
19
2


## Página 4


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
1
Objetivo
El objetivo de esta pr´actica es que el alumno conozca y practique los principales conceptos
relacionados con el dise˜no e implementaci´on de una aplicaci´on distribuida que utiliza distintas
tecnolog´ıas para el desarrollo de aplicaciones distribuidas (Sockets, RPC y Servicios Web) con
diferentes lenguajes de programaci´on (C y Python).
2
Descripci´on de la funcionalidad
El objetivo de la pr´actica es desarrollar un servicio de notificaci´on de mensajes entre
usuarios conectados a Internet, de forma parecida, aunque con una funcionalidad mucho m´as
simplificada, a lo que ocurre con la aplicaci´on WhatsApp. Se podr´an enviar mensajes de texto
de un tama˜no m´aximo de 256 bytes (incluyendo el c´odigo 0 que indica fin de cadena, es decir,
como mucho la cadena almacenada en el mensaje tendr´a una longitud m´axima de 255 caracteres)
y de forma opcional se podr´a tambi´en enviar archivos adjuntos de cualquier tama˜no. El esquema
final de la aplicaci´on es el que se muestra en la Figura 1.
Fig. 1: Interfaz de Usuario
Los componentes de la aplicaci´on final son los siguientes:
• Servicio de mensajer´ıa. Es el servidor encargado de la funcionalidad global de mensajer´ıa.
• Usuarios. Son los usuarios del servicio de mensajer´ıa.
• Servicio de registro. Es un servidor desarrollado utilizando RPC que registrar´a las
operaciones que van realizando los diferentes usuarios.
3


## Página 5


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
• Conversor de mensajes. Es un servicio web que se encargar´a de normalizar los mensajes
que se env´ıan los usuarios. Cada vez que un usuario redacta un mensaje, se lo env´ıa a este
conversor de mensajes para que elimine del mensaje los espacios en blanco repetidos. El
objetivo es que en los mensajes, las diferentes palabras est´en separadas solo por un espacio
en blanco.
3
Primera parte
El/La estudiante deber´a dise˜nar, codificar y probar, utilizando el lenguaje C y sobre un
sistema operativo Linux, un servidor que gestione la funcionalidad del sistema y, por otro
lado, deber´a dise˜nar, codificar y probar, utilizando el lenguaje Python, el c´odigo de los clientes.
Toda la pr´actica tendr´a que desarrollarse y funcionar correctamente en las aulas
de laboratorio utilizadas en la asignatura o el entorno Docker disponible en Aula
Global.
En esta primera parte no se desarrollar´a la funcionalidad relacionada con el env´ıo opcional
de ficheros entre usuarios. Tampoco se desarrollar´a el servicio de registro ni el conversor de
mensajes. Todas estas funcionalidades se desarrollar´an en la segunda parte de la pr´actica.
A continuaci´on se detallan las caracter´ısticas del sistema. En esta parte del enunciado se
va a describir el protocolo a seguir entre el servidor y el cliente. Este protocolo permitir´a a
cualquier cliente que lo siga comunicarse con el servidor implementado. Esto hace que, diferentes
alumnos puedan probar sus clientes con los servidores desarrollados por otros.
Para el almacenamiento de los usuarios y de los mensajes se podr´a utilizar la implementaci´on
que se desee: listas en memoria o ficheros.
3.1
Desarrollo del servicio
El objetivo es dise˜nar y desarrollar los dos siguientes programas:
• Un servidor concurrente multihilo que proporciona el servicio de comunicaci´on entre
los distintos clientes registrados en el sistema, gestiona las conexiones de los mismos y el
almacenamiento de los mensajes enviados a un cliente no conectado en el sistema.
• Un cliente concurrente multihilo que se comunica con el servidor y es capaz de enviar
y recibir mensajes. Uno de los hilos se utilizar´a para enviar mensajes al servidor y el otro
para recibirlos.
4
Prerrequisitos
Como material de apoyo se proporciona el c´odigo Python de un programa que permite
interactuar con el servidor y donde se desarrollar´a todo el c´odigo necesario de la funcionalidad
que ejecutan los usuarios.
5
Ejecuci´on y uso de la interfaz
Para ejecutar el programa cliente se invocar´a en la l´ınea de comandos:
4


## Página 6


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
$ python3 ./client.py -s <IP> -p <PUERTO>
Al comienzo de la pr´actica, como no se tendr´a ning´un servidor preparado, se puede utilizar
la interfaz indicando que la IP sea “localhost” y el PUERTO sea el que el usuario desee:
$ python3 ./client.py -s localhost -p 8888
Este programa permite interactuar a trav´es de los siguientes comandos:
• REGISTER.
• UNREGISTER.
• CONNECT.
• DISCONNECT.
• USERS.
• SEND.
• SENDATTACH.
• QUIT.
En esta primera parte de la pr´actica no se desarrollar´a el c´odigo asociado a SENDATTACH,
que es el que se utiliza para enviar mensajes y archivos de texto adjuntos.
6
Desarrollo del cliente
6.1
Finalizar la ejecuci´on del cliente
Para finalizar la ejecuci´on del programa cliente de usuario se introducir´a en la consola de
la aplicaci´on cliente:
c> QUIT
6.2
Registro en el sistema
Para registrar a un usuario en el sistema se introducir´a en la consola de la aplicaci´on
cliente:
c> REGISTER <userName>
Cada vez que se realiza una operaci´on, su resultado se mostrar´a en la consola. El servicio
REGISTER, una vez ejecutado en el servidor, puede devolver tres resultados (cuyos valores se
describen detalladamente en la secci´on destinada a describir el protocolo de comunicaci´on): 0 si
la operaci´on se ejecut´o con ´exito, 1 si ya existe un usuario registrado con el mismo nombre y 2
en cualquier otro caso. Si la operaci´on se realiza correctamente, el cliente recibir´a por parte del
servidor un mensaje con c´odigo 0 y mostrar´a por pantalla el siguiente mensaje:
5


## Página 7


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
c> REGISTER OK
Si el usuario ya est´a registrado se mostrar´a en la consola del cliente:
c> USERNAME IN USE
En este caso el servidor no realizar´a ning´un registro.
En caso de que no se pueda realizar la operaci´on de registro, bien porque el servidor este
ca´ıdo o bien porque se devuelva el c´odigo 2, se mostrar´a el siguiente mensaje en la consola del
programa de usuario:
c> REGISTER FAIL
6.3
Darse de baja en el sistema
Para dar de baja a un usuario del sistema se introducir´a en la consola del cliente:
c> UNREGISTER <userName>
Este servicio una vez ejecutado en el servidor, puede devolver tres resultados: 0 si la
operaci´on se realiza con ´exito, 1 si el usuario no existe y 2 en cualquier otro caso. Si el usuario
que se quiere dar de baja del servicio no existe, el servidor retornar´a un 1 (descrito en la secci´on
destinada a describir el protocolo de comunicaci´on). En este caso se mostrar´a el siguiente
mensaje en la consola del cliente:
c> USER DOES NOT EXIST
Si la operaci´on de baja se realiza correctamente y el usuario es borrado en el servidor, el
servidor devolver´a un 0 y el cliente mostrar´a:
c> UNREGISTER OK
En caso de que no se pueda realizar esta operaci´on, bien porque el servidor este ca´ıdo y no
se pueda establecer la conexi´on con ´el o bien porque devuelva el c´odigo 2, se mostrar´a en la
consola del cliente el siguiente mensaje:
c> UNREGISTER FAIL
6.4
Conectarse al sistema
La funcionalidad de conexi´on permitir´a al usuario conectarse al servidor para poder
establecer conversaciones con otros usuarios registrados en el sistema y recibir los mensajes que
estuviesen esperando a ser enviados al usuario reci´en conectado.
Se va a considerar que desde una interfaz de usuario (consola) solo puede haber un
´unico usuario conectado a la vez, es decir, no se contemplar´a la conexi´on de dos clientes a la
vez desde el mismo programa cliente. Cada programa cliente est´a destinado a la conexi´on de un
´unico usuario.
Una vez que un cliente est´a registrado en el sistema de mensajer´ıa, este puede conectarse
y desconectarse del servicio tantas veces como desea. Para conectarse debe enviar (utilizando
6


## Página 8


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
el protocolo descrito en la Secci´on 7) al servidor su direcci´on IP y puerto para que ´este pueda
enviarle los mensajes de otros usuarios. La estructura de un proceso cliente conectado al servicio
se muestra en la Figura 2.
Servidor
de men-
sajer´ıa
Servidor
de men-
sajer´ıa
Interfaz
de
usuario
Interfaz
de
usuario
Thread
que
recibe
mens
Usuario 1
Usuario 2
3. Enviar mensaje de
conexi´on
1. Buscar
puerto libre
2. Crear
thread
Antes de
la conexi´onn
Proceso de
conexi´on
Fig. 2:
Estructura de un proceso cliente conectado al servicio de mensajer´ıa
Para ello el cliente introducir´a en la interfaz:
c> CONNECT <userName>
Internamente el cliente buscar´a un puerto v´alido libre (1).
Una vez obtenido el puerto, y antes de enviar el mensaje al servidor, el cliente debe crear
un hilo (2) que ser´a el encargado de escuchar (en la IP y puerto seleccionado) y atender los
env´ıos de mensajes de otros usuarios procedentes del servidor.
A continuaci´on, el cliente enviar´a (3) la solicitud de conexi´on al servidor.
Una vez establecida la conexi´on en el sistema, el servidor devolver´a un byte que codificar´a
el resultado de la operaci´on: 0 en caso de ´exito, 1 si el usuario no existe, 2 si el usuario ya est´a
conectado y 3 en cualquier otro caso.
Si todo ha ido bien, se mostrar´a en el cliente:
c> CONNECT OK
En caso de c´odigo 1 (usuario no est´a registrado en el sistema), el cliente mostrar´a el
siguiente error:
7


## Página 9


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
c> CONNECT FAIL, USER DOES NOT EXIST
En caso de que el cliente ya estuviera conectado en el sistema (c´odigo 2), el cliente mostrar´a:
c> USER ALREADY CONNECTED
En caso de que no se pueda realizar la operaci´on de conexi´on, bien porque el servidor este
ca´ıdo, se produzca un error en las comunicaciones o se devuelva el c´odigo 3, se mostrar´a el
siguiente mensaje:
c> CONNECT FAIL
6.5
Desconectarse del sistema
Cuando un usuario desea desconectarse del sistema se escribir´a en la consola asociada al
usuario:
c> DISCONNECT <userName>
Internamente el cliente debe parar la ejecuci´on del hilo creado en la operaci´on CONNECT.
El servidor ante esta operaci´on puede devolver 4 valores: 0 si se ejecut´o con ´exito, 1 si el usuario
no existe, 2 si el usuario no est´a conectado y 3 en caso de error.
Si todo ha ido correctamente, el servidor devolver´a un 0 y el cliente mostrar´a el siguiente
mensaje por pantalla:
c> DISCONNECT OK
Si el usuario no existe, se mostrar´a el siguiente mensaje:
c> DISCONNECT FAIL, USER DOES NOT EXIST
Si el usuario existe pero no se conect´o previamente, se mostrar´a el siguiente mensaje:
c> DISCONNECT FAIL, USER NOT CONNECTED
En caso de que no se pueda realizar la operaci´on con el servidor, porque ´este est´e ca´ıdo,
hay un error en las comunicaciones o el servidor devuelve un 3, se mostrar´a por la consola el
siguiente mensaje:
c> DISCONNECT FAIL
En caso de que se produzca un error en la desconexi´on, el cliente de igual forma parar´a la
ejecuci´on del hilo creado en la operaci´on CONNECT, actuando a todos los efectos como si se
hubiera realizado la desconexi´on.
6.6
Env´ıo de un mensaje
Las funcionalidades SEND y SENDATTACH se usar´an para establecer conversaciones
con el resto de usuarios registrados en el sistema. La primera se utilizar´a para enviar mensajes
que no incluye un archivo adjunto y la segunda se utilizar´a cuando se quiera enviar un mensaje
8


## Página 10


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
y un archivo adjunto (solo se enviar´a un ´unico archivo adjunto). En esta parte del proyecto solo
se contemplar´a la funcionalidad SEND, dejando la funcionalidad SENDATTACH para la
segunda parte
Para enviar un mensaje a un destinatario se escribir´a en la consola:
c> SEND <userName> <message>
donde <userName> indica el alias del usuario destinatario.
Para implementar esta funcionalidad, el servidor asociar´a a cada mensaje enviado por un
usuario un n´umero entero como identificador y llevar´a siempre el registro de cu´al ha sido el
´ultimo identificador asignado a un mensaje de un usuario. Cuando un usuario se registra por
primera vez en el sistema, este identificador se pone a 0, de forma que el primer mensaje que se
env´ıa toma como identificador el valor 1, el segundo el valor 2, y as´ı sucesivamente. Cuando se
llegue al m´aximo n´umero de identificadores posibles, el nuevo identificador a asignar volver´a a
ser el 1, y se proceder´a de forma similar. El identificador debe almacenarse en una variable de
tipo unsigned int, cuando se llegue al n´umero m´aximo representable en una variable de este
tipo y se le sume 1, la variable volver´a a tomar valor 0 y se continuar´a el proceso, de forma que
el siguiente identificador volver´a a ser el 1.
Cuando se env´ıa un mensaje al servidor, ´este devuelve un byte con tres posibles valores (se
describen con detalle en la Secci´on 7): un 0 en caso de ´exito, un 1 si el usuario no existe y 2 en
cualquier otro caso. En caso de ´exito (c´odigo 0), adem´as devolver´a el identificador asociado al
mensaje enviado (un n´umero entero) y se mostrar´a el siguiente mensaje:
c> SEND OK - MESSAGE <id>
En caso de que se env´ıe un mensaje a un usuario no registrado, el servidor indicar´a el error
(c´odigo 1) y mostrar´a en la consola del cliente:
c> SEND FAIL, USER DOES NOT EXIST
En caso de que se produzca un error (servidor ca´ıdo, error de comunicaciones, error por
problemas de almacenamiento de mensaje o se devuelva un error de tipo 2) se mostrar´a:
c> SEND FAIL
Una vez que el servidor almacena un mensaje para un usuario y ha respondido con el
c´odigo correspondiente al usuario remitente, si el usuario est´a conectado en ese momento le
enviar´a el mensaje. En caso de que se haya enviado con ´exito, el servidor enviar´a al remitente del
mensaje la confirmaci´on de que el mensaje con el identificador asignado se ha enviado al usuario
correctamente (se describe con detalle en la Secci´on 7). Cada vez que un cliente remitente de
un mensaje recibe del servidor un mensaje de entrega de mensaje a otro proceso mostrar´a:
c> SEND MESSAGE <id> OK
Indicando que el mensaje con identificador <id> se ha entregado correctamente.
En caso de que el usuario no est´e conectado, el servidor almacenar´a el mensaje. Posterior-
mente cuando el cliente destinatario se conecte el servidor se encargar´a de enviarle todos los
mensajes pendientes (uno a uno). Cada vez que se env´ıa con ´exito un mensaje a un usuario, se
notifica el remitente del mensaje, el cual mostrar´a por pantalla:
9


## Página 11


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
c> SEND MESSAGE <id> OK
Como se ver´a posteriormente, siempre que el servidor env´ıa con ´exito un mensaje a un
usuario, descarta el mensaje elimin´andolo del servidor. Es decir, el servidor solo almacena los
mensajes pendientes de entrega, cada vez que se entrega con ´exito un mensaje se borra del
servidor.
6.7
Recepci´on de mensajes
Cada vez que el cliente reciba un mensaje a trav´es del hilo creado para ello, deber´a mostrar
por pantalla el siguiente mensaje:
s> MESSAGE <id> FROM <userName>
<message>
END
Donde <userName> indica el alias del usuario.
Como se ver´a en la Secci´on de protocolo de comunicaci´on, el mensaje de recepci´on llevar´a el
remitente (nombre de usuario), el mensaje y un identificador (n´umero entero) que lo identifica.
6.8
Petici´on de usuarios conectados
Por ´ultimo, la funcionalidad USERS permitir´a al usuario solicitar al servidor cu´ales
de los usuarios registrados en el sistema est´an conectados actualmente y as´ı poder entablar
conversaci´on con ellos.
Cuando un cliente se ha conectado podr´a saber si hay m´as usuarios conectados para poder
hablar con ellos. Para ello, el cliente tendr´a que enviar un mensaje al servidor con la operaci´on
propiamente dicha:
c> USERS
Para implementar esta funcionalidad, el servidor puede devolver tres posibles resultados:
0 si la operaci´on se ha realizado correctamente, 1 si el usuario que hace la petici´on no est´a
conectado en el servidor y 2 en caso de error.
Si todo ha ido correctamente, el servidor devolver´a un 0 y, adem´as, la lista de todos los
usuarios conectados. Estos usuarios conectados se mostrar´an en la consola del cliente de la
siguiente forma:
c> CONNECTED USERS (N users connected) OK
<user1>
<user2>
...
<userN>
Se indica entre par´entesis el n´umero de usuarios conectados (el valor N en el ejemplo
anterior) y a continuaci´on los nombres de usuario, uno por l´ınea.
10


## Página 12


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
En caso de que el usuario que env´ıe la operaci´on no est´a previamente conectado al servidor,
el servidor indicar´a el error (c´odigo 1) y se mostrar´a en la consola del cliente:
c> CONNECTED USERS FAIL, USER IS NOT CONNECTED
En caso de que se produzca un error (servidor ca´ıdo, error de comunicaciones, o se devuelva
un error de tipo 2) se mostrar´a:
c> CONNECTED USERS FAIL
7
Desarrollo del servidor
El objetivo del servidor es ofrecer un servicio de comunicaci´on entre clientes. Para ello
los clientes deber´an registrarse con un nombre determinado en el sistema y a continuaci´on
conectarse, indicando para ello su IP y puerto. El servidor debe mantener una lista con todos
los clientes registrados, el nombre, estado y direcci´on de los mismos, as´ı como una lista de los
mensajes pendientes de entrega a cada cliente. Adem´as se encargar´a de asociar un identificador
a cada mensaje recibido de un cliente.
El servidor debe ser capaz de gestionar varias conexiones simult´aneamente (debe ser
concurrente) mediante el uso de m´ultiples hilos (multithread). El servidor utilizar´a sockets TCP
orientados a conexi´on
7.1
Uso del servidor
Se ejecutar´a de la siguiente manera:
$ ./server -p <port>
Al iniciar el servidor se mostrar´a el siguiente mensaje:
s> init server <localIP>:<port>
Antes de recibir peticiones por parte de los clientes mostrar´a:
s>
El programa terminar´a al recibir una se˜nal SIGINT (Ctrl+C).
7.2
Registro de un cliente
Cuando un cliente quiera registrarse enviar´a el mensaje correspondiente indicando el
nombre de usuario. Cuando este mensaje es recibido, el servidor deber´a hacer lo siguiente:
• Verificar que no existe ning´un otro usuario registrado con el mismo nombre.
• Si no existe el usuario, se almacena la informaci´on con el nombre del usuario y se env´ıa el
c´odigo 0 al cliente. Se pone a 0 el valor asociado al identificador de mensaje.
• Si existe un usuario con el mismo nombre, se env´ıa una notificaci´on al cliente indic´andolo.
11


## Página 13


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
La informaci´on asociada a cada cliente incluir´a ´unicamente:
• Nombre de usuario.
Una vez registrado un cliente, el servidor mostrar´a el siguiente mensaje por consola en
caso de ´exito:
s> REGISTER <userName> OK
Donde <userName> indica el alias del usuario registrado.
En caso de que se haya producido un error en el registro se mostrar´a:
s> REGISTER <userName> FAIL
Donde <userName> indica el alias del usuario registrado.
7.3
Baja de un cliente
Cuando un cliente quiera darse de baja del servicio de mensajer´ıa debe enviar el mensaje
correspondiente indicando en ´el el nombre de usuario que se quiere borrar. Cuando el servidor
recibe el mensaje har´a lo siguiente:
• Verificar que el usuario est´a registrado.
• Si el usuario existe, se borra su entrada de la lista y se env´ıa un 0 al cliente.
• Si no existe, se env´ıa una notificaci´on de error al cliente (c´odigo con valor 1).
Cuando se realice con ´exito el borrado del usuario se mostrar´a en la consola del servidor el
siguiente mensaje
s> UNREGISTER <userName> OK
Donde <userName> indica el alias del usuario registrado.
En caso de fallo se mostrar´a:
s> UNREGISTER <userName> FAIL
Donde <userName> indica el alias del usuario registrado.
Cuando un usuario se da de baja del sistema, se borrar´an todos los mensajes (en caso de
no estar conectado) que todav´ıa no se le han entregado.
7.4
Conexi´on de un cliente
Cuando un cliente se conecta al servicio, debe indicar su puerto en un mensaje (la IP
se obtendr´a a trav´es de la llamada accept). Cuando este mensaje se recibe, el servidor debe
realizar lo siguiente:
• Buscar el nombre de usuario indicado entre todos los usuarios registrados (usando el nombre
de usuario).
• Si el usuario existe y su estado es “Desconectado”:
12


## Página 14


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
– Se rellena el campo IP y puerto del usuario.
– Se cambia su estado a “Conectado”.
– Se devuelve el c´odigo de la operaci´on (0).
Si una vez conectado, existen mensajes pendientes de enviar para este usuario, se enviar´an
todos los mensajes al usuario uno a uno. Si el usuario no existe, se devuelve un 1, si ya est´a
conectado un 2, y en cualquier otro caso un 3.
Cuando la operaci´on de conexi´on finaliza con ´exito en el servidor se debe mostrar el
siguiente mensaje en la consola del servidor:
s> CONNECT <userName> OK
Donde <userName> indica el alias del usuario registrado.
En caso de fallo se mostrar´a:
s> CONNECT <userName> FAIL
Donde <userName> indica el alias del usuario registrado.
Si existen mensajes pendientes para el usuario que se ha conectado se mostrar´a el siguiente
mensaje por cada uno de ellos que se env´ıe:
s> SEND MESSAGE <id> FROM <userNameS> TO <userNameR>
Siendo userNameS el usuario que envi´o el mensaje originalmente, userNameR el usuario
que destinatario del mensaje, y id el identificador asociado al mensaje que se env´ıa. <userNameS>
y <userNameR> hacen referencia a los nombres de usuario de los usuarios.
El env´ıo de cada uno de los mensajes debe hacerse siguiendo el protocolo
descrito en la secci´on 8.6.
7.5
Desconexi´on de un cliente
Cuando un cliente quiere dejar de recibir mensajes del servicio debe enviar el mensaje
correspondiente indicando el nombre de usuario. Cuando el servidor reciba este mensaje realizar´a
lo siguiente:
• Buscar el nombre de usuario indicado entre los usuarios registrados.
• Si el usuario existe y su estado es “Conectado”:
– Borra los campos IP y puerto del usuario.
– Se cambia su estado a “Desconectado”.
– Se env´ıa el c´odigo 0 al cliente.
• Si no existe, se env´ıa una notificaci´on de error al cliente.
Cuando la operaci´on finaliza con ´exito, se debe mostrar por consola lo siguiente:
s> DISCONNECT <userName> OK
13


## Página 15


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
Donde <userName> indica el nombre del usuario registrado.
En caso de error, se mostrar´a:
s> DISCONNECT <userName> FAIL
Donde <userName> indica el nombre del usuario registrado.
7.6
Env´ıo de un mensaje
Cuando un cliente quiere enviar un mensaje a otro cliente registrado deber´a enviar el
mensaje correspondiente al servidor indicando el usuario de destino, su nombre y el mensaje.
Una vez recibido el mensaje en el servidor, ´este realizar´a lo siguiente:
• Buscar el nombre de ambos usuarios entre los usuarios registrados.
• Si uno de los dos usuarios no existe se env´ıa un mensaje de error al cliente (ver secci´on 5).
• Se almacena en la lista de mensajes pendientes del usuario destino el mensaje junto con el
usuario que lo env´ıa y el identificador asignado.
• Se devuelve un mensaje al remitente con el identificador de mensaje asignado (c´odigo 0,
cuando todo ha ido bien).
Una vez realizadas estas acciones, si el usuario destino existe y su estado es ”Conectado”:
• Se env´ıa el mensaje a la IP:puerto indicado en la entrada del usuario.
• Se env´ıa al cliente remitente del mensaje un mensaje indicando que el mensaje con el
identificador correspondiente se ha enviado.
Una vez finalizado el env´ıo se muestra por la consola del servidor el siguiente mensaje:
s> SEND MESSAGE <id> FROM <userNameS> TO <userNameR>
Donde <userNameS> y <userNameR> hacen referencia a los nombre de los usuarios.
Si el usuario destino existe y su estado es “Desconectado”, no realizar´a ninguna acci´on.
Los mensajes se enviar´a en el momento en el que el proceso destinatario del mensaje se conecte.
En este caso se mostrar´a por pantalla:
s> MESSAGE <id> FROM <userNameS> TO <userNameR> STORED
<userNameS> y <userNameR> hacen referencia a los nombres de los usuarios.
Los mensajes almacenados se enviar´an posteriormente (uno a uno) cuando el cliente
destinatario se conecte al sistema
7.7
Solicitud de usuarios conectados
Cuando un cliente conectado quiere saber qui´enes est´an conectados en el servicio de
mensajer´ıa, deber´a enviar un mensaje con la operaci´on propiamente dicha. Cuando el servidor
recibe la operaci´on har´a lo siguiente:
• Verificar que el usuario est´a registrado. En caso contrario, devuelve un error de tipo 2.
14


## Página 16


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
• Verificar que el usuario est´a conectado. En caso contrario, devuelve un error de tipo 1.
• Si est´a conectado, obtendr´a todos los usuarios conectados en el servicio en ese momento.
• Enviar´a al cliente un c´odigo de tipo 0, y la cantidad de usuarios conectados al servidor.
• Enviar´a los usuarios conectados al servicio.
Cuando se realice con ´exito la obtenci´on de los usuarios conectados, se mostrar´a en la
consola del servidor el siguiente mensaje:
s> CONNECTEDUSERS OK
En caso de fallo se mostrar´a:
s> CONNECTEDUSERS FAIL
8
Protocolo de comunicaci´on
En este apartado se especificar´an los mensajes que se intercambiar´an el servidor y los
clientes. Estos mensajes no se pueden modificar y se deben usar tal y como se describen y en el
orden en el que se describen. En todo el protocolo se establece una conexi´on por cada operaci´on.
IMPORTANTE
Todos los campos enviados se codificar´an como cadenas de caracteres. Se recuerda que
las cadenas finalizan con el c´odigo ASCII ‘\0’.
Todos los c´odigos de error que devuelve el servidor como respuesta se codificar´an como un
byte (valor 0 si la operaci´on se ejecut´o con ´exito, valor 1 si ya existe un usuario registrado
con el mismo nombre y valor 2 en cualquier otro caso).
8.1
Registro
Cuando un cliente quiere registrarse realiza las siguientes operaciones:
1. Se conecta al servidor, de acuerdo a la IP y puerto pasado en la l´ınea de mandatos al
programa.
2. Se env´ıa la cadena “REGISTER” indicando la operaci´on.
3. Se env´ıa una cadena de caracteres con el nombre del usuario que se quiere registrar y que
identifica al usuario.
4. Se recibe el resultado (un byte) de la operaci´on.
5. Cierra la conexi´on
15


## Página 17


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
8.2
Baja
Cuando un cliente quiere darse de baja env´ıa se realizan las siguientes operaciones:
1. Se conecta al servidor, de acuerdo a la IP y puerto pasado en la l´ınea de mandatos al
programa.
2. Se env´ıa la cadena “UNREGISTER” indicando la operaci´on.
3. Se env´ıa una cadena con el nombre del usuario que se quiere dar de baja.
4. Recibe del servidor un byte que codifica el resultado de la operaci´on: 0 en caso de ´exito, 1
si el usuario no existe, 2 en cualquier otro caso.
5. Cierra la conexi´on.
8.3
Conexi´on
Cuando un cliente se quiere conectar al servicio debe realizar las siguientes operaciones:
1. Se conecta al servidor, de acuerdo a la IP y puerto pasado en la l´ınea de mandatos al
programa.
2. Se env´ıa la cadena “CONNECT” indicando la operaci´on.
3. Se env´ıa una cadena con el nombre del usuario.
4. Se env´ıa una cadena de caracteres que codifica el n´umero de puerto de escucha del cliente.
As´ı, para el puerto 456, esta cadena ser´a ”456”.
5. Recibe del servidor un byte que codifica el resultado de la operaci´on: 0 en caso de ´exito, 1
si el usuario no existe, 2 si el usuario ya est´a conectado y 3 en cualquier otro caso.
6. Cierra la conexi´on
8.4
Desconexi´on
Cuando el cliente quiera dejar de recibir los mensajes deber´a realizar las siguientes acciones:
1. Se conecta al servidor, de acuerdo a la IP y puerto pasado en la l´ınea de mandatos al
programa.
2. Se env´ıa la cadena “DISCONNECT” indicando la operaci´on.
3. Se env´ıa una cadena con el nombre del usuario que se desea desconectar.
4. Recibe del servidor un byte que codifica el resultado de la operaci´on: 0 en caso de ´exito, 1
si el usuario no existe, 2 si el usuario no est´a conectado y 3 en cualquier otro caso.
5. Cierra la conexi´on.
Tenga en cuenta que un usuario solo se puede desconectar si la operaci´on se env´ıa desde la
IP desde la que se registr´o.
16


## Página 18


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
8.5
Env´ıo de un mensaje cliente-servidor
Cuando el cliente quiere enviarle a otro usuario un mensaje realizar´a las siguientes acciones:
1. Se conecta al servidor, de acuerdo a la IP y puerto pasado en la l´ınea de mandatos al
programa.
2. Se env´ıa la cadena “SEND” indicando la operaci´on.
3. Se env´ıa una cadena con el nombre que identifica al usuario que env´ıa el mensaje.
4. Se env´ıa una cadena con el nombre que identifica al usuario destinatario del mensaje.
5. Se env´ıa una cadena en la que se codifica el mensaje a enviar (como mucho 256 caracteres
incluido el c´odigo ’0’, es decir la cadena tendr´a como mucho una longitud de 255 caracteres).
6. Recibe del servidor un byte que codifica el resultado de la operaci´on: 0 en caso de ´exito.
En este caso recibir´a a continuaci´on una cadena de caracteres que codificar´a el identificador
num´erico asignado al mensaje, ”132” para el mensaje con n´umero 132. Si no se ha realizado
la operaci´on con ´exito se recibe 1 si el usuario no existe y 2 en cualquier otro caso. En
estos dos casos no se recibir´a ning´un identificador.
7. Cierra la conexi´on.
8.6
Env´ıo de un mensaje servidor cliente
Cuando el servidor quiere enviarle a un usuario registrado y conectado un mensaje de otro
usuario realizar´a las siguientes acciones (por cada mensaje a enviar):
1. Se conecta al thread de escucha del cliente (de acuerdo a la IP y puerto almacenado para
ese cliente).
2. Se env´ıa la cadena “SEND MESSAGE” indicando la operaci´on.
3. Se env´ıa una cadena con el nombre que identifica al usuario que env´ıa el mensaje.
4. Se env´ıa una cadena codificando en ella el identificador asociado al mensaje.
5. Se env´ıa una cadena con el mensaje (todos los mensajes tendr´an como mucho 256 bytes
incluido el c´odigo ’0’, este tama˜no lo controlar´a el cliente).
6. Cierra la conexi´on.
Si se produce alg´un error durante esta operaci´on, el mensaje se considerar´a no entregado y
se seguir´a almacenando en el servidor como pendiente de entrega, hasta que se pueda entregar.
Si se produce alg´un error durante la conexi´on a un cliente, el servidor asumir´a que el cliente se
ha desconectado y lo marcar´a como desconectado.
Una vez enviado el mensaje, el servidor tiene que notificar al usuario que env´ıo el mensaje
(remitente) del mensaje de esta recepci´on. Para ello el servidor realiza las siguientes acciones:
1. Se conecta al thread de escucha del cliente remitente del mensaje (de acuerdo a la IP y
puerto almacenado para ese cliente).
17


## Página 19


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
2. Se env´ıa la cadena “SEND MESS ACK” indicando la operaci´on.
3. Se env´ıa una cadena codificando en ella el identificador asociado al mensaje que se ha
entregado.
4. Cierra la conexi´on.
En caso de que el usuario remitente no estuviera conectado, se descartar´a este
mensaje y no se realizar´an las acciones anteriores.
8.7
Solicitud de usuarios conectados
Cuando un cliente quiere saber los usuarios conectados en el servicio de mensajer´ıa se
realizan las siguientes operaciones:
1. Se conecta al servidor, de acuerdo a la IP y puerto pasado en la l´ınea de mandatos al
programa.
2. Se env´ıa la cadena “USERS” indicando la operaci´on.
3. Se env´ıa una cadena con el nombre que identifica al usuario que env´ıa el mensaje.
4. Recibe del servidor un byte que codifica el resultado de la operaci´on: 0 en caso de ´exito, 1
si el usuario no est´a conectado, 2 en cualquier otro caso.
5. En caso de ´exito (valor devuelto 0), el cliente recibi´ra del servidor una cadena de caracteres
que codifica la cantidad de clientes conectados al servidor. As´ı, para 18 clientes conectados
recibir´a la cadena de texto “18”
6. Recibe del servidor tantas cadenas como clientes haya conectados. Una cadena de caracteres
por usuario
7. Cierra la conexi´on.
Recuerde que en el protocolo utilizado todas las cadenas de caracteres finalizan con el
c´odigo ASCII 0 (’\0’).
9
Normas generales
Para el desarrollo de las dos partes que constituyen la pr´actica han de seguirse las siguientes
normas:
1. Las pr´acticas que no compilen o no se ajusten a la funcionalidad y requisitos planteados,
obtendr´an una calificaci´on de 0.
2. Las pr´acticas que tengan warnings ser´an penalizadas.
3. Un programa no comentado, obtendr´a una calificaci´on de 0.
4. La entrega de la pr´actica se realizar´a a trav´es de los entregadores habilitados. No se
permite la entrega a trav´es de correo electr´onico.
18


## Página 20


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 1
5. Se prestar´a especial atenci´on a detectar funcionalidades copiadas entre dos pr´acticas. En
caso de detectar copia, ambos grupos perder´an la evaluaci´on continua.
6. Toda la pr´actica tendr´a que desarrollarse y funcionar correctamente en las aulas
de laboratorio utilizadas en la asignatura.
7. El sistema debe funcionar con clientes y servidores ejecutando en contenedores con direc-
ciones IP distintas.
8. La memoria debe tener una longitud m´axima de 15 p´aginas. No se incluir´an capturas
de pantalla en la secci´on de pruebas.
M´as adelante se indicar´a el contenido de la memoria asociado a la pr´actica y el procedimiento
de entrega. Solo se har´a una ´unica entrega para todas las partes que compone la pr´actica. La
fecha de entrega es el 10 de mayo.
9.1
Calificaci´on de la pr´actica
S´olo debe hacerse una entrega que podr´a contener la funcionalidad completa de todas las
partes o de solo la parte 1. La pr´actica se calificar´a de la siguiente forma:
• La parte 1 de la pr´actica se puntuar´a sobre 6 puntos.
• El env´ıo de ficheros adjuntos (en la parte 2) se puntuar´a sobre 2 puntos.
• El servicio web (parte 2) se puntuar´a sobre 1 punto.
• El servicio RPC (parte 2) se puntuar´a sobre 1 punto.
De esta forma si solo se entrega la parte 1, como m´aximo se obtendr´an 6 puntos. La
entrega de las partes 1 y 2 permitir´ıa obtener hasta 10 puntos. En todo caso, ser´a obligatorio
entregar la parte 1. Es opcional entregar la parte 2. Se puede entregar solo la parte 1 o la parte
1 y 2. Dentro de la parte 2 se pueden entregar de forma opcional cualquiera de estas partes:
• El env´ıo de ficheros adjuntos.
• El servicio web.
• El servicio RPC.
19


---

# PARTE 2 - Ampliación: ficheros, servicio web y RPC


## Página 1


Universidad Carlos III de Madrid
Departamento de Inform´atica
´Area de Arquitectura y Tecnolog´ıa de Computadores
Sistemas Distribuidos
Pr´actica. Servicio de env´ıo de mensajes
Parte 2
Grado de Ingenier´ıa en Inform´atica
Grupo docente de Sistemas Distribuidos
Curso 2025-2026


## Página 2


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
´Indice
1
Objetivo
2
2
Parte 1. Transferencia de ficheros entre usuarios
2
2.1
Env´ıo de un mensaje . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
2
2.2
Protocolo de env´ıo de mensaje con fichero adjunto cliente-servidor . . . . . . . .
4
2.3
Env´ıo de un mensaje servidor-cliente
. . . . . . . . . . . . . . . . . . . . . . . .
4
2.4
Modificaci´on de la operaci´on de solicitud de usuarios conectados . . . . . . . . .
5
2.5
Solicitud de transferenca de ficheros . . . . . . . . . . . . . . . . . . . . . . . . .
6
3
Parte 2. Servicio Web
7
4
Parte 3. RPC
7
5
Normas generales
8
5.1
Calificaci´on de la pr´actica
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
9
6
Documentaci´on a entregar
9
6.1
Fichero a entregar . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
11
1


## Página 3


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
1
Objetivo
El objetivo de esta parte de la pr´actica es completar la funcionalidad del servicio de env´ıo
de mensajes, incorporando el env´ıo de ficheros entre usuarios y el empleo de servicios web y
RPC.
El esquema final de la aplicaci´on es el que se muestra en la Figura 1.
Fig. 1: Interfaz de Usuario
2
Parte 1. Transferencia de ficheros entre usuarios
En esta parte de la pr´actica se implementar´a el servicio de transferencia de ficheros
entre usuarios. En la transferencia de ficheros no interviene para nada el servidor. Para los
nombres de los ficheros se utilizar´an siempre nombres con path absoluto, como por ejemplo:
/tmp/datos.txt.
2.1
Env´ıo de un mensaje
La funcionalidad SENDATTACH se usar´a para establecer conversaciones con el resto de
usuarios registrados en el sistema para enviar un mensaje y un fichero.
Para enviar un mensaje a un destinatario incluyendo un fichero se escribir´a en la consola:
c> SENDATTACH <userName> <message> <fileName>
donde:
2


## Página 4


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
• <userName> indica el nombre del usuario destinatario.
• <message> es el mensaje a enviar.
• <filName> indica el nombre del fichero a enviar.
La funcionalidad de esta operaci´on es id´entica a la funcionalidad SEND de la parte 1, con
la ´unica diferencia que el programa cliente env´ıa tambi´en el nombre del fichero.
De esta forma, el servidor asociar´a a cada mensaje enviado por un usuario un n´umero entero
como identificador y llevar´a siempre el registro de cu´al ha sido el ´ultimo identificador asignado
a un mensaje de un usuario. Cuando un usuario se registra por primera vez en el sistema, este
identificador se pone a 0, de forma que el primer mensaje que se env´ıa toma como identificador
el valor 1, el segundo el valor 2, y as´ı sucesivamente. Cuando se llegue al m´aximo n´umero de
identificadores posibles, el nuevo identificador a asignar volver´a a ser el 1, y se proceder´a de forma
similar. El identificador debe almacenarse en una variable de tipo unsigned int, cuando se llegue
al n´umero m´aximo representable en una variable de este tipo y se le sume 1, la variable volver´a
a tomar valor 0 y se continuar´a el proceso, de forma que el siguiente identificador volver´a a ser el 1.
Cuando se env´ıa un mensaje con fichero al servidor, ´este devuelve un byte con tres posibles
valores (se describen con en la parte 1 de la pr´actica): un 0 en caso de ´exito, un 1 si el usuario no
existe y 2 en cualquier otro caso. En caso de ´exito (c´odigo 0), adem´as devolver´a el identificador
asociado al mensaje enviado (un n´umero entero) y se mostrar´a el siguiente mensaje:
c> SENDATTACH OK - MESSAGE <id>
En caso de que se env´ıe un mensaje a un usuario no registrado, el servidor indicar´a el error
(c´odigo 1) y mostrar´a en la consola del cliente:
c> SENDATTACH FAIL, USER DOES NOT EXIST
En caso de que se produzca un error (servidor ca´ıdo, error de comunicaciones, error por
problemas de almacenamiento de mensaje o se devuelva un error de tipo 2) se mostrar´a:
c> SENDATTACH FAIL
Una vez que el servidor almacena un mensaje y el nombre de fichero para un usuario y
ha respondido con el c´odigo correspondiente al usuario remitente, si el usuario est´a conectado
en ese momento le enviar´a el mensaje junto con el nombre del fichero. En caso de que se haya
enviado con ´exito, el servidor enviar´a al remitente del mensaje la confirmaci´on de que el mensaje
con el identificador asignado se ha enviado al usuario correctamente, junto con el nombre del
fichero. Cada vez que un cliente remitente de un mensaje recibe del servidor un mensaje de
entrega de mensaje a otro proceso junto con un nombre de fichero mostrar´a:
c> SENDATTACH MESSAGE <id> <fileName> OK
Indicando que el mensaje con identificador <id> se ha entregado correctamente, y que el
nombre de fichero asociado es fileName.
En caso de que el usuario no est´e conectado, el servidor almacenar´a el mensaje y el nombre
de fichero. Posteriormente cuando el cliente destinatario se conecte el servidor se encargar´a de
enviarle todos los mensajes pendientes (uno a uno), junto con los nombres de fichero. Cada vez
3


## Página 5


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
que se env´ıa con ´exito un mensaje a un usuario, se notifica el remitente del mensaje, el cual
mostrar´a por pantalla:
c> SENDATTACH MESSAGE <id> <fileName> OK
Siempre que el servidor env´ıa con ´exito un mensaje a un usuario, descarta el mensaje
elimin´andolo del servidor. Es decir, el servidor solo almacena los mensajes pendientes de entrega,
cada vez que se entrega con ´exito un mensaje se borra del servidor.
2.2
Protocolo de env´ıo de mensaje con fichero adjunto cliente-servidor
Cuando el cliente quiere enviarle a otro usuario un mensaje con fichero adjunto realizar´a
las siguientes acciones:
1. Se conecta al servidor, de acuerdo a la IP y puerto pasado en la l´ınea de mandatos al
programa.
2. Se env´ıa la cadena “SENDATTACH” indicando la operaci´on.
3. Se env´ıa una cadena con el nombre que identifica al usuario que env´ıa el mensaje.
4. Se env´ıa una cadena con el nombre que identifica al usuario destinatario del mensaje.
5. Se env´ıa una cadena en la que se codifica el mensaje a enviar (como mucho 256 caracteres
incluido el c´odigo ’0’, es decir la cadena tendr´a como mucho una longitud de 255 caracteres).
6. Se env´ıa una cadena con el nombre del fichero adjunto. Se considerar´a que el nombre del
fichero tiene como mucho una longitud de 255 caracteres.
7. Recibe del servidor un byte que codifica el resultado de la operaci´on: 0 en caso de ´exito.
En este caso recibir´a a continuaci´on una cadena de caracteres que codificar´a el identificador
num´erico asignado al mensaje, ”132” para el mensaje con n´umero 132. Si no se ha realizado
la operaci´on con ´exito se recibe 1 si el usuario no existe y 2 en cualquier otro caso. En
estos dos casos no se recibir´a ning´un identificador.
8. Cierra la conexi´on.
2.3
Env´ıo de un mensaje servidor-cliente
Cuando el servidor quiere enviarle a un usuario registrado y conectado un mensaje, con
fichero adjunto, de otro usuario realizar´a las siguientes acciones (por cada mensaje a enviar):
1. Se conecta al thread de escucha del cliente (de acuerdo a la IP y puerto almacenado para
ese cliente).
2. Se env´ıa la cadena “SEND MESSAGE ATTACH” indicando la operaci´on.
3. Se env´ıa una cadena con el nombre que identifica al usuario que env´ıa el mensaje.
4. Se env´ıa una cadena codificando en ella el identificador asociado al mensaje.
4


## Página 6


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
5. Se env´ıa una cadena con el mensaje (todos los mensajes tendr´an como mucho 256 bytes
incluido el c´odigo ’0’, este tama˜no lo controlar´a el cliente).
6. Se env´ıa una cadena con el nombre del fichero adjunto.
7. Cierra la conexi´on.
Si se produce alg´un error durante esta operaci´on, el mensaje se considerar´a no entregado y
se seguir´a almacenando en el servidor como pendiente de entrega, hasta que se pueda entregar.
Si se produce alg´un error durante la conexi´on a un cliente, el servidor asumir´a que el cliente se
ha desconectado y lo marcar´a como desconectado.
Una vez enviado el mensaje, el servidor tiene que notificar al usuario que lo envıo (remitente)
sobre su recepci´on. Para ello el servidor realiza las siguientes acciones:
1. Se conecta al thread de escucha del cliente remitente del mensaje (de acuerdo a la IP y
puerto almacenado para ese cliente).
2. Se env´ıa la cadena “SEND MESS ATTACH ACK” indicando la operaci´on.
3. Se env´ıa una cadena codificando en ella el identificador asociado al mensaje que se ha
entregado.
4. Se env´ıa una cadena codificando en ella el nombre del fichero adjunto.
5. Cierra la conexi´on.
En caso de que el usuario remitente no estuviera conectado, se descartar´a este
mensaje y no se realizar´an las acciones anteriores.
2.4
Modificaci´on de la operaci´on de solicitud de usuarios conectados
Para que un usuario pueda solicitar la transferencia de un fichero a otro, es necesario
que el primero conozca la direcci´on IP y el puerto del segundo. Para ello, se modificar´a la
funcionalidad de solicitud de usuarios conectados de forma que el servidor devuelva por cada
usuario conectado, la IP y el puerto de escucha del thread creado en la conexi´on.
De esta forma, cuando un cliente quiere saber los usuarios conectados en el servicio de
mensajer´ıa se realizan las siguientes operaciones:
1. Se conecta al servidor, de acuerdo a la IP y puerto pasado en la l´ınea de mandatos al
programa.
2. Se env´ıa la cadena “USERS” indicando la operaci´on.
3. Se env´ıa una cadena con el nombre que identifica al usuario que env´ıa el mensaje.
4. Recibe del servidor un byte que codifica el resultado de la operaci´on: 0 en caso de ´exito, 1
si el usuario no est´a conectado, 2 en cualquier otro caso.
5. En caso de ´exito (valor devuelto 0), el cliente recibir´a del servidor una cadena de caracteres
que codifica la cantidad de clientes conectados al servidor. As´ı, para 18 clientes conectados
recibir´a la cadena de texto “18”.
5


## Página 7


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
6. Recibe del servidor tantas cadenas como clientes haya conectados. Una cadena de caracteres
por usuario. Cada cadena incluir´a la IP y el puerto y estar´a codificada de la siguiente
forma: usuario :: IP :: puerto.
7. Cierra la conexi´on.
La informaci´on recibida con los usuarios conectados se almacenar´a en el cliente Python en
una estructura de datos que permitir´a conocer para cada usuario conectado, la IP y el puerto
asociado al thread creado en la conexi´on.
2.5
Solicitud de transferenca de ficheros
Cuando un cliente recibe un mensaje de servidor con fichero adjunto, el thread que recibe
la petici´on muestra por pantalla:
c> MESSAGE <id> FROM <userName>
<message>
END
FILE <fileName>
Donde <userName> indica el nombre del usuario y <fileName> el nombre del fichero
adjunto.
Para poder recuperar el contenido del fichero con nombre <fileName> ha de modificarse
el c´odigo del cliente escrito en Python a˜nadiendo una nueva funcionalidad:
c> GETFILE <userName> <fileName> <localFileName>
donde:
• <userName> indica el nombre del usuario.
• <fileName> indica el nombre del fichero remoto que hay que transferir.
• <localFileName> indica el nombre del fichero local en el que se ha de copiar el contenido
del fichero remoto.
Para poder llevar a cabo la operaci´on, el c´odigo del cliente buscar´a en la estructura de
datos creada en la secci´on anterior, la direcci´on IP y el puerto asociado al thread de escucha del
usuario que posee el fichero. En caso de no encontrarse en esa estructura de datos, se volver´a
a realizar de forma interna una solicitud de petici´on de usuarios conectados para refrescar la
informaci´on.
En caso de que el usuario no se encuentre en la estructura anterior por estar desconectado,
se mostrar´a en la interfaz del cliente el siguiente mensaje:
c> FILE TRANSFER FAILED, user not connected.
Si se dispone de la IP y del puerto se realizar´an las siguientes acciones:
1. Se conecta al thread de escucha del cliente (de acuerdo a la IP y puerto almacenado para
ese cliente).
6


## Página 8


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
2. Se env´ıa la cadena “GET FILE” indicando la operaci´on.
3. Se env´ıa una cadena con el nombre que identifica al usuario que env´ıa la operaci´on.
4. Se env´ıa una cadena codificando en ella el nombre del fichero a transferir.
5. Se recibe del cliente el contenido del fichero y se almacena de forma local en <localFileName>.
6. Cierra la conexi´on.
El proceso para transferir el contenido del fichero entre el usuario remoto y el fichero local
(acci´on 5 anterior) es totalmente libre y se puede implementar como se desee, teniendo en cuenta
que el contenido ha de transferirse obligatoriamente a tr´aves de sockets.
3
Parte 2. Servicio Web
Para el servicio web, se desarrollar´a y desplegar´a un servicio web desarrollado en Python
siguiendo el material presentado en la asignatura.
Este servicio web se encargar´a de normalizar los mensajes que se env´ıan los usuarios. Cada
vez que un usuario redacta un mensaje, se lo env´ıa a este conversor de mensajes para que elimine
del mensaje los espacios en blanco repetidos. El objetivo es que en los mensajes, las diferentes
palabras est´en separadas solo por un espacio en blanco.
El servicio se desplegar´a, por simplicidad, en la m´aquina local donde ejecuta el cliente
desarrollado en Python.
4
Parte 3. RPC
Esta parte de la pr´actica pretende ampliar la aplicaci´on desarrollada en la Parte 1 para
a˜nadir un servicio, basado en RPC, que se encargue de imprimir por pantalla las operaciones
que realizan los usuarios del sistema.
Cada vez que el servidor reciba una operaci´on de un usuario enviar´a al servidor RPC el
nombre del usuario que realiza la operaci´on y la operaci´on que realiza. Toda esta informaci´on se
enviar´a como cadenas de caracteres.
De esta forma, cada vez que el servidor reciba una petici´on remota imprimir´a la siguiente
informaci´on:
Nombre_usuario
OPERACION
Donde OPERACION puede tomar los siguientes valores:
• REGISTER
• UNREGISTER
• CONNECT
• DISCONNECT
• USERS
7


## Página 9


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
• SEND
• SENDATTACH
En el caso de SENDATTACH tambi´en se imprimir´a el nombre del fichero enviar, como se
puede ver en el siguiente ejemplo:
Nombre_usuario
SENDATTACH
/tmp/file.txt
Se deja total libertad para definir la interfaz (fichero .x) que se considere m´as adecuada.
En todo caso, es necesario que se justifique la interfaz definida.
Tenga en cuenta que solo se env´ıa el nombre de la operaci´on y el nombre del fichero para
SENDATTACH. No hay que enviar el texto de los mensajes que se env´ıan los usuarios.
Para el desarrollo de esta parte se utilizar´a el lenguaje de programaci´on C y el modelo
ONC-RPC.
El proceso servidor de la parte 1 es cliente del servicio RPC implementado en esta parte,
mientras que el servidor ONC-RPC desarrollado ofrece el servicio. El servidor de la parte 1
acceder´a a esta variable de entorno para poder localizar el servicio.
Para poder ejecutar correctamente el servidor de la parte 1, este necesitar´a conocer la
direcci´on IP o nombre del computador donde ejecuta el servidor RPC. Para ello se definir´a una
variable de entorno (denominado LOG RPC IP) de forma similar a como se hizo en los ejercicios
evaluables 2 y 3.
Para poder probar toda la funcionalidad ser´a necesario:
• Ejecutar el servicio web (en la m´aquina local del cliente).
• Ejecutar el servidor RPC.
• Ejecutar el servidor de la parte 1 con la variable de entorno LOG RPC IP correctamente
definida.
• Ejecutar los clientes que considere necesario para probar el proyecto. En caso de que
se pruebe con varios clientes en varias m´aquinas, cada m´aquina cliente tendr´a su propio
servicio web desplegado.
5
Normas generales
Para el desarrollo de las dos partes que constituyen la pr´actica han de seguirse las siguientes
normas:
1. Las pr´acticas que no compilen o no se ajusten a la funcionalidad y requisitos planteados,
obtendr´an una calificaci´on de 0.
2. Las pr´acticas que tengan warnings ser´an penalizadas.
3. Un programa no comentado, obtendr´a una calificaci´on de 0.
4. La entrega de la pr´actica se realizar´a a trav´es de los entregadores habilitados. No se
permite la entrega a trav´es de correo electr´onico.
8


## Página 10


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
5. Se prestar´a especial atenci´on a detectar funcionalidades copiadas entre dos pr´acticas. En
caso de detectar copia, ambos grupos perder´an la evaluaci´on continua.
6. Toda la pr´actica tendr´a que desarrollarse y funcionar correctamente en las aulas
de laboratorio utilizadas en la asignatura.
7. El sistema debe funcionar con clientes y servidores ejecutando en contenedores con direc-
ciones IP distintas.
8. La memoria debe tener una longitud m´axima de 15 p´aginas. No se incluir´an capturas
de pantalla en la secci´on de pruebas.
5.1
Calificaci´on de la pr´actica
S´olo debe hacerse una entrega que podr´a contener la funcionalidad completa de todas las
partes o de solo la parte 1. La pr´actica se calificar´a de la siguiente forma:
• La parte 1 de la pr´actica se puntuar´a sobre 6 puntos.
• El env´ıo de ficheros adjuntos (en la parte 2) se puntuar´a sobre 2 puntos.
• El servicio web (parte 2) se puntuar´a sobre 1 punto.
• El servicio RPC (parte 2) se puntuar´a sobre 1 punto.
De esta forma si solo se entrega la parte 1, como m´aximo se obtendr´an 6 puntos. La
entrega de las partes 1 y 2 permitir´ıa obtener hasta 10 puntos. En todo caso, ser´a obligatorio
entregar la parte 1. Es opcional entregar la parte 2. Se puede entregar solo la parte 1 o la parte
1 y 2. Dentro de la parte 2 se pueden entregar de forma opcional cualquiera de estas partes:
• El env´ıo de ficheros adjuntos.
• El servicio web.
• El servicio RPC.
6
Documentaci´on a entregar
La pr´actica se desarrollar´a en grupos de dos alumnos como m´aximo. La pr´actica s´olo
deber´a ser entregada por un ´unico integrante del grupo de pr´acticas en su grupo docente. No se
debe entregar la misma pr´actica de forma repetida por todos los integrantes del grupo.
El plazo de entrega de toda la pr´actica en su conjunto finaliza el 10 de mayo de 2026.
La entrega se realizar´a mediante Aula Global, a trav´es de un entregador que se habilitar´a
a tal efecto.
Se debe entregar un fichero comprimido en formato zip con el nombre ssdd proyecto A B.zip
donde A y B son los NIA de los integrantes que realizan la entrega.
El fichero en formato zip debe contener:
9


## Página 11


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
• autores.txt, con los nombres y NIA de los integrantes del grupo.
• memoria.pdf
• client.py
• server.c y/o todos los ficheros fuentes que necesite el servidor para su compilaci´on.
• Fichero Makefile utilizado para compilar todos los archivos .c.
• Ficheros Python necesarios para el desarrollo del servicio web.
• Fichero con la interfaz (.x) del servidor RPC y todos los ficheros necesarios para la
compilaci´on y ejecuci´on.
• Fichero de texto de nombre README con instrucciones detalladas para la compilaci´on y
despliegue de todos los procesos involucrados en la aplicaci´on.
• Cualquier otro fichero fuente que se considere necesario para la compilaci´on o evaluaci´on
de la pr´actica.
Los ficheros entregados deben incluir la funcionalidad de todas las partes que se hayan
completado.
Deben incluirse todos los archivos fuente necesarios para la compilaci´on y un fichero de
texto con nombre README, que incluir´a instrucciones detalladas para la compilaci´on y despliegue
de todos los procesos involucrados en la aplicaci´on.
La memoria de la pr´actica debe comentar los aspectos del desarrollo de la misma que
considere m´as relevantes. Del mismo modo, puede exponer los comentarios personales que
considere oportunos. Se deber´a entregar un documento en formato PDF.
No descuide la calidad de la memoria de su pr´actica. Aprobar la memoria es imprescindible
para aprobar la pr´actica, tanto como el correcto funcionamiento de la misma. Si al evaluarse
la memoria de su pr´actica, se considera que no alcanza el m´ınimo admisible, su
pr´actica estar´a suspensa.
La memoria tendr´a que contener al menos los siguientes apartados:
• Portada donde figuren los autores (incluyendo nombre completo, NIA y direcci´on de
correo electr´onico).
• ´Indice de contenidos.
• Descripci´on del c´odigo detallando las principales funciones implementadas. No incluir
c´odigo fuente de la pr´actica en este apartado.
• Descripci´on de la forma de compilar y obtener el ejecutable de todos los procesos
involucrados. Adem´as, se debe describir la forma de ejecutarlos.
• Bater´ıa de pruebas utilizadas y resultados obtenidos. Se dar´a mayor puntuaci´on a
pruebas avanzadas, casos extremos, y en general a aquellas pruebas que garanticen el
correcto funcionamiento de la pr´actica en todos los casos.
Hay que tener en cuenta:
10


## Página 12


Departamento de Inform´atica
Sistemas Distribuidos
Pr´actica - Servicio de env´ıo de mensajes - Parte 2
– Que el programa compile correctamente y sin warnings a ser posible.
– Evite pruebas duplicadas que eval´uan los mismo flujos de programa. La puntuaci´on de
este apartado no se mide en funci´on del n´umero de pruebas, sino del grado de cobertura
de las mismas. Es mejor pocas pruebas que eval´uan diferentes casos, a muchas que
eval´uan siempre el mismo caso.
• Conclusiones, problemas encontrados, c´omo se han solucionado, y opiniones personales.
Se puntuar´an tambi´en los siguientes aspectos relativos a la presentaci´on:
• La memoria debe tener n´umeros de p´agina en todas las p´aginas (menos en la portada).
• El texto de la memoria debe estar justificado.
6.1
Fichero a entregar
Para crear el fichero a entregar se deben seguir los siguientes pasos:
• Se crea el directorio para preparar los materiales a entregar y se comprueba que se encuentra
en el directorio de la entrega:
$ cd
$ mkdir
ssdd_proyecto_AAAAAAAAAA_BBBBBBBBB
$ cd
ssdd_proyecto_AAAAAAAAA_BBBBBBBBB
• Despu´es se proceder´a a copiar todos los ficheros con los programas desarrollados al directorio
de la entrega y se procede a generar el fichero zip a ser entregado:
$ cd ..
$ ls
...
ssdd_proyecto_AAAAAAAAAA_BBBBBBBBB
...
$ zip -r ssdd_proyecto_AAAAAAAAAA_BBBBBBBBB .zip
ssdd_p2_AAAAAAAAAA_BBBBBBBBB /
Solo se har´a una ´unica entrega para todas las partes que compone la pr´actica. La fecha
tope de entrega es el 10 de mayo de 2026 a las 23:55.
11

