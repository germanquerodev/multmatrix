# MultMatrix
Es un ejercicio para clase de Programacion de sistemas distribuidos.
Se trata de una clase distribuida en C++
### Utilizacion:
La ip de conexion se encuentra en "multmatrix_stub.h" en la clase multMatrix como la variable privada std::string ip
Poner la ip privada del servidor.<br>
Tras eso ejecutar:
```sh
cmake .
make
```
En el servidor hay que ejecutar:
```sh
./serverMultmatrix
```
Enviar al cliente clientMultmatrix y ejecutarlo de la misma forma.<br>
Si se quiere cambiar la secuencia de acciones que debe de realizar el sistema cambiarlo en el main del cliente("clientMultmatrix.cpp").