# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

* Valenzuela Tomás Agustín 
* Paredes Alonso Francisco
* Capiel Sofia

# Puntos principales

## Verificación del timestep

Se pudo obtener un valor adecuado de timeStep al realizar diversas simulaciones dejando fijo el valor de fps en 60 y variando la magnitud del factor que multiplica a la constante SECONDS_PER_DAY en la inicializacion de la variable timeMultiplier. Se noto principalmente que valores mayores a 50 en este factor generaban movimientos inusuales en los cuerpos como por ejemplo inclinaciones en sus orbitas y trayectorias bruscamente impulsadas, mientras que valores menores a 50 dieron lugar a simulaciones poco fluidas. Es por estos motivos que se tomo un valor de 50 para el factor, lo que resulto en un valor de (SECONDS_PER_DAY * 5/6) asignado a la variable timeStep.

## Verificación del tipo de datos float

Para almacenar las masas, aceleraciones, velocidades y posiciones se optó por el tipo de dato double en lugar de float. Esto permitió evitar casteos implícitos y errores de redondeo, ya que las propiedades de los cuerpos se utilizan en cálculos junto con otras variables de tipo double. Es esencial evitar dichos casteos, dado que en algunas funciones, como por ejemplo `calculateAcceleration` y `updateSpeedAndPosition`, se requiere de precisión para evitar la acumulación de errores durante las iteraciones de updateOrbitalSim. La falta de exactitud en los cálculos puede causar órbitas inestables y movimientos erráticos de los cuerpos en la simulación.


## Complejidad computacional con asteroides

Se identifico la complejidad computacional con asteroides como una cota superior de O(n) dado a que se pudieron reducir los calculos fisicos dentro de un solo for loop dependiente del numero de entradas, el cual termina siendo el mas significativo de todo el algoritmo y muestra un crecimiento lineal de la ineficiencia del programa frente a una cantidad n de entradas (numero de asteroides).

## Mejora de la complejidad computacional

Se pudo mejorar la complejidad computacional al reducir el cuello de botella generado en la renderizacion de los graficos, esto se logro al implementar una funcion similar a DrawSphere() dentro de la libreria raylib llamada DrawSphereEx() la cual al otorgarle dos parametros extra (cantidad de anillos y de caras para realizar la esfera) brindo una notable mejora en el rendimiento del programa, esto se debe a la reduccion de los calculos internos en la libreria para formar la superficie esferica basandose en una cantidad limitada de los parametros dados. Tambien se logro reducir el cuello de botella dado en los calculos fisicos del programa al implementar el famoso algoritmo de la raiz cuadrada inversa presente en el desarrollo del video juego Quake III Arena.

# Bonus points

## Simulacion con Jupiter 1000 veces mas masivo

Al simular a Júpiter mil veces más masivo (1,898 × 10<sup>30</sup> kg), su masa se aproxima a la del Sol (1,989 × 10<sup>30</sup> kg). Esto hace que Júpiter comience a comportarse como un cuerpo comparable en fuerza gravitatoria al Sol. Uno de los efectos observados en la simulación fue que las órbitas de los planetas y asteroides se deformaron, y algunos cuerpos previamente ligados al Sol pasaron a gravitar alrededor de Júpiter. Como el Sol dejó de ser el cuerpo dominante, el centro de masa del sistema se desplazó hacia Júpiter, y ambos comenzaron a orbitar alrededor de este nuevo centro de masa. Antes de este cambio, la masa de Júpiter era mucho menor que la del Sol, por lo que las contribuciones a la aceleración calculadas en `calculateAccelerations` eran despreciables, haciendo que el Sol esté prácticamente fijo.
Con un agujero negro ocurriria algo similar, pero con efectos mucho mas drasticos, ya que la masa de un agujero negro típico puede ser decenas de miles de millones de veces la del Sol.

## Simulacion con estrellas del sistema Alpha Centauri

## Easter egg

En el archivo `orbitalSim.cpp` se incluye un Easter Egg en las siguientes lineas de codigo:
```
// Surprise!
//phi = 0;
```
Si estas lineas se descomentan, phi pasa a valer cero. Phi representa el angulo polar en el plano XZ para ubicar a un asteroide alrededor del centro de masa, por lo cual, si phi es cero todos los asteroides se alinearan sobre el eje X y su velocidad inicial sera perpendicular al eje X.

## Nave espacial manejada por el usuario

Se añadio a la simulacion un cuerpo de color verde el cual representa a una nave espacial que puede ser controlada con las teclas U, I y O para moverla en direcciones positivas por cada eje y J, K y L para moverlo en las direcciones opuestas.

## Implementacion de colisiones plasticas entre los cuerpos

## Visualizacion de los vectores de cada cuerpo

Se añadieron opciones para activar y desactivar la visualizacion de los vectores de velocidad y aceleracion de cada cuerpo en tiempo real. Esto se puede realizar de dos modos, el primero es al ejecutar el programa desde la carpeta principal añadiendo los parametros de la siguiente manera: `out\orbitalSim.exe -show_velocity_vectors -show_acceleration_vectors`, el segundo metodo es presionando la tecla F9 (para activar/desactivar los vectores de velocidad) o la tecla F10 (para activar/desactivar los vectores de aceleracion) dentro de la simulacion.
