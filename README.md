# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

* Valenzuela Tomás Agustín 
* Paredes Alonso Francisco
* Capiel Sofia

## Verificación del timestep

Se pudo obtener un valor adecuado de timeStep al realizar diversas simulaciones dejando fijo el valor de fps en 60 y variando la magnitud del factor que multiplica a la constante SECONDS_PER_DAY en la inicializacion de la variable timeMultiplier. Se noto principalmente que valores mayores a 50 en este factor generaban movimientos inusuales en los cuerpos como por ejemplo inclinaciones en sus orbitas y trayectorias bruscamente impulsadas, mientras que valores menores a 50 dieron lugar a simulaciones poco fluidas. Es por estos motivos que se tomo un valor de 50 para el factor, lo que resulto en un valor de (SECONDS_PER_DAY * 5/6) asignado a la variable timeStep.

## Verificación del tipo de datos float

[completar]

## Complejidad computacional con asteroides

[completar]

## Mejora de la complejidad computacional

Se pudo mejorar la complejidad computacional al reducir el cuello de botella generado en la renderizacion de los graficos, esto se logro al implementar una funcion similar a DrawSphere() dentro de la libreria raylib llamada DrawSphereEx() la cual al otorgarle dos parametros extra (cantidad de anillos y de caras para realizar la esfera) brindo una notable mejora en el rendimiento del programa, esto se debe a la reduccion de los calculos internos en la libreria para formar la superficie esferica basandose en una cantidad limitada de los parametros dados. Tambien se logro reducir el cuello de botella dado en los calculos fisicos del programa al implementar el famoso algoritmo de la raiz cuadrada inversa presente en el desarrollo del video juego Quake III Arena.

## Bonus points

[completar]
