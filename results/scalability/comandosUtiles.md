# Idea para el experimento app-scalability
Programar un "ping" todos contra todos (cada nodo le manda un mensaje a todos los nodos). Sobre una red con 100, 1000, 10000 nodos generada aleatoriamente. (se escuchan ideas).

Tener implementaciones tanto en C como en JAVA

## Propuesta
Tener el actor Peer
El actor Peer:
* va a tener un mailbox asociado
* va a saber su nro
* va a saber la cantidad de peers en la red
* le va a enviar un "ping" asincronico a todos los que no sean el
* va a esperar a recibir asincronicament pings de todos sus otros peers

# Checkout
```bash
git clone experiments git@github.com:kovin/simgrid.git
```

# Compile
```bash
simgrid$ cmake -DCMAKE_INSTALL_PREFIX=/opt/simgrid -Denable_java=ON .
simgrid$ make
simgrid$ sudo make install
simgrid$ ctest # to test that everything went smoothly
```

# Build & Run
## Build C
```bash
simgrid$ make app-scalability
```

## Ejecucion C
```bash
simgrid/results/scalability$ ../../examples/msg/app-scalability/app-scalability exponential_25_nodes_.5_alpha.xml 25_nodes_deployment_for_c.xml
```

### Ejecucion sin log
```bash
simgrid/results/scalability$ ../../examples/msg/app-scalability/app-scalability exponential_25_nodes_.5_alpha.xml 25_nodes_deployment_for_c.xml --log=root.thres:critical
```

### Ejecucion con memory profile de valgrind
```bash
simgrid/results/scalability$ valgrind --tool=massif ../../examples/msg/app-scalability/app-scalability exponential_25_nodes_.5_alpha.xml 25_nodes_deployment_for_c.xml --log=root.thres:critical

ms_print $(ls -t | head -n1)
```

## Build Java
```bash
simgrid/results/scalability$ javac -cp ../../simgrid.jar:../../examples/java ../../examples/java/app/scalability/*java
```

## Ejecucion Java
```bash
simgrid/results/scalability$ java -cp ../../simgrid.jar:../../examples/java app.scalability.Main exponential_25_nodes_.5_alpha.xml 25_nodes_deployment_for_java.xml
```

# Comando top con filtrado por nombre de proceso
```bash
top -c -p $(pgrep -d',' -f scalability)
```

# Comandos para obtener resultados y xmls de ejemplo
En results/scalability se incluyen varios archivos de plataforma `exponential_[node_count]_nodes_.5_alpha.xml` que fueron creados con `utils/simulacrum-1.1.jar` y luego adaptados con `utils/adaptPlaftormXml`
También se incluyen varios archivos de deployment:
* `[nodes_count]_nodes_deployment_for_c.xml`
* `[nodes_count]_nodes_deployment_for_java.xml`

para las implementacion de app-scalability en C y JAVA respectivamente

En results/ también se incluyen:
* `testCImplementation.sh`
* `testJavaImplementation.sh`

estos scripts van a probar las implementacions de C y JAVA con los archivos de plataforma disponibles en results/ y los archivos de deployment correspondientes a la cantidad de nodos y tipo de ejecutable que corresponda.

En results/ también se incluyen:
* `resultsForC`
* `resultsForJava`

que corresponden al output de los scripts previamente mencionados
