# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

* Valenzuela Tomás Agustín 
* Paredes Alonso Francisco 
* Capiel Sofia 

# Puntos principales

## Verificación del time step (delta time)

Se implemento un algoritmo, el cual inicialmente verifica cual es la cantidad de veces que la computadora puede actualizar la simulacion por fotograma, con el objetivo de conseguir la maxima precision para asegurar los fps pedidos por el usuario con `+fps_target` (si no se usa este parametro, los fps por defecto son 60) es por esto que al principio, la simulacion se encuentra quieta, hasta que se logra encontrar un valor preciso para `sim->dt` (variable utilizada como time step) y como consecuencia se estabilizan los fotogramas por segundo. Los beneficios de esta implementacion son notorios en relacion al metodo inicial ya que el valor del time step es independiente de la cantidad de dias que pasan en la simulacion por segundo.

## Verificación del tipo de datos float

Para almacenar las masas, aceleraciones, velocidades y posiciones se optó por el tipo de dato double en lugar de float. Esto permitió evitar casteos implícitos y errores de redondeo, ya que las propiedades de los cuerpos se utilizan en cálculos junto con otras variables de tipo double. Es esencial evitar dichos casteos, dado que en algunas funciones, como por ejemplo `calculateAcceleration` y `updateSpeedAndPosition`, se requiere de precisión para evitar la acumulación de errores durante las iteraciones de updateOrbitalSim. La falta de exactitud en los cálculos puede causar órbitas inestables y movimientos erráticos de los cuerpos en la simulación.

## Complejidad computacional con asteroides

Inicialmente el programa se veia compuesto por un algoritmo en el cual los calculos de las aceleraciones de cada cuerpo se realizaban con respecto al resto (incluyendo todos los asteroides y todos los planetas), al realizar el conteo de dichas revisiones se pudo notar lo siguiente:
- El primer cuerpo calculaba su aceleracion con respecto al segundo, al tercero, ... al ultimo. (n-1 revisiones).
- El segundo cuerpo calculaba su aceleracion con respecto al tercero, al cuarto, ... al ultimo. (n-2 revisiones).<br>
...
- El ultimo no calculaba su aceleracion con respecto a ningun cuerpo. (n-n = 0 revisiones). 
Al sumar todos estos casos (n casos), se obtuvo la expresion: `(n.(n+1))/2` la cual muestra un polinomio de grado 2, por lo que se pudo considerar que la complejidad computacional del programa inicialmente estaba dada por una cota `O(n²)`.

## Mejora de la complejidad computacional

Se pudo mejorar la complejidad computacional al reducir el cuello de botella generado en la renderizacion de los graficos, esto se logro al implementar una funcion similar a `DrawSphere()` dentro de la libreria raylib llamada `DrawSphereEx()` la cual al otorgarle dos parametros extra (cantidad de anillos y de caras para realizar la esfera) brindo una notable mejora en el rendimiento del programa, esto se debe a la reduccion de los calculos internos en la libreria para formar la superficie esferica basandose en una cantidad limitada de los parametros dados, ademas se añadio la posibilidad de cambiar el modo de renderizado entre `QUALITY` (utilizando esta ultima funcion) y `PERFORMANCE` (utilizando `DrawPoint3D()`) con las teclas:
- `F6` Para la nave espacial.
- `F7` Para los planetas (o estrellas).
- `F8` Para los asteroides.
Tambien se logro reducir el cuello de botella dado en los calculos fisicos del programa al implementar un algoritmo en el cual se calcula la aceleracion de cada asteroide con respecto a cada planeta en lugar de a cada cuerpo (es decir sin incluir a los demas asteroides), de esta forma se pudo pasar desde la cota inicial `O(n²)` a una cota mucho mas eficiente de `O(n)`.

# Bonus points

## Simulacion con Jupiter 1000 veces mas masivo

Al simular a Júpiter mil veces más masivo (1,898 × 10<sup>30</sup> kg), su masa se aproxima a la del Sol (1,989 × 10<sup>30</sup> kg). Esto hace que Júpiter comience a comportarse como un cuerpo comparable en fuerza gravitatoria al Sol. Uno de los efectos observados en la simulación fue que las órbitas de los planetas y asteroides se deformaron, y algunos cuerpos previamente ligados al Sol pasaron a gravitar alrededor de Júpiter. Como el Sol dejó de ser el cuerpo dominante, el centro de masa del sistema se desplazó hacia Júpiter, y ambos comenzaron a orbitar alrededor de este nuevo centro de masa. Antes de este cambio, la masa de Júpiter era mucho menor que la del Sol, por lo que las contribuciones a la aceleración calculadas en `calculateAccelerations` eran despreciables, haciendo que el Sol esté prácticamente fijo.
Con un agujero negro ocurriria algo similar, pero con efectos mucho mas drasticos, ya que la masa de un agujero negro típico puede ser decenas de miles de millones de veces la del Sol.
Se puede probar este fenomeno al ejecutar el programa con el siguiente comando:
`out\orbitalSim.exe -massive_jupiter`

## Simulacion con estrellas del sistema Alpha Centauri

Se puede activar la simulacion en relacion al sistema Alpha Centauri utilizando el siguiente comando para ejecutar el programa:
`out\orbitalSim.exe -system 1`

## Easter egg

En el archivo `orbitalSim.cpp` se incluye un Easter Egg en las siguientes lineas de codigo:
```
// Surprise!
//phi = 0;
```
Si estas lineas se descomentan, phi pasa a valer cero. Phi representa el angulo polar en el plano XZ para ubicar a un asteroide alrededor del centro de masa, por lo cual, si phi es cero todos los asteroides se alinearan sobre el eje X y su velocidad inicial sera perpendicular al eje X.
Para poder visualizar el easter egg dentro de la simulacion, se debera ejecutar el programa añadiendo el siguiente parametro de la forma:
`out\orbitalSim.exe -easter_egg`

## Nave espacial manejada por el usuario

Se añadio a la simulacion un cuerpo de color verde el cual representa a una nave espacial que puede ser controlada con las teclas U, I y O para moverla en direcciones positivas por cada eje y J, K y L para moverlo en las direcciones opuestas.

## Modos de Camara

Fue implementada la posibilidad de cambiar el modo de la camara entre "libre" y "tercera persona" oprimiendo la tecla `F4`, a su vez se puede cambiar el planeta al que se esta enfocando con la tecla `T`.

## Visualizacion de los vectores de cada cuerpo

Se añadieron opciones para activar y desactivar la visualizacion de los vectores de velocidad y aceleracion de cada cuerpo en tiempo real. Esto se puede realizar de dos modos, el primero es al ejecutar el programa desde la carpeta principal añadiendo los parametros de la siguiente manera: 
`out\orbitalSim.exe -show_velocity_vectors -show_acceleration_vectors`
El segundo metodo es presionando la tecla F9 (para activar/desactivar los vectores de velocidad) o la tecla F10 (para activar/desactivar los vectores de aceleracion) dentro de la simulacion.

## Rewind de la Simulacion

Fue implementada la posibilidad de invertir el flujo de la simulacion presionando la tecla `R`.

## Parametros extra en la ejecucion del programa

Fueron añadidos parametros que se pueden utilizar al momento de ejecutar el programa, algunos ya fueron mencionados antes, sin embargo aqui hay una lista completa:
- `+fps_target <numero>` Permite cambiar la cantidad de fps para la simulacion (minimo: 15, maximo: 240), el valor por defecto es 60.
- `-fullscreen` Indica que se inicie la simulacion en pantalla completa
- `-w <numero>` Permite cambiar el ancho de la ventana (minimo: 400, maxmimo: 7680), el valor por defecto es 1280.
- `-h <numero>` Permite cambiar el alto de la ventana (minimo: 400, maxmimo: 4320), el valor por defecto es 720.
- `-days_per_simulation_second <numero>` Permite cambiar la cantidad de dias que pasan dentro de la simulacion por cada segundo (minimo: 1, maximo: 365), el valor por defecto es 10.
- `-asteroids_ammount <numero>` Permite agregar la cantidad de asteroides especificada (minimo: 0, maximo: 20000), el valor por defecto es 0.
- `-show_velocity_vectors` Permite visualizar los vectores de velocidad en cada cuerpo.
- `-show_acceleration_vectors` Permite visualizar los vectores de aceleracion en cada cuerpo.
- `-massive_jupiter` Permite simular el fenomeno en el cual jupiter es 1000 veces mas masivo.
- `-spawn_blackhole` Permite simular la aparicion de un agujero negro en la simulacion.
- `-easter_egg` Permite simular el easter egg (phi = 0).
- `-system <1/0>` Permite seleccionar el sistema que se desea simular, `1` equivale al sistema Alpha Centauri, `0` (valor por defecto) equivale al sistema solar.
