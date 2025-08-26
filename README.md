# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

* Valenzuela Tomás Agustín 
* Paredes Alonso Francisco
* Capiel Sofia

## Verificación del timestep

Se pudo obtener un valor adecuado de timeStep al realizar diversas simulaciones dejando fijo el valor de fps en 60 y variando la magnitud del factor que multiplica a la constante SECONDS_PER_DAY en la inicializacion de la variable timeMultiplier. Se noto principalmente que valores mayores a 50 en este factor generaban movimientos inusuales en los cuerpos como por ejemplo inclinaciones en sus orbitas y trayectorias bruscamente impulsadas, mientras que valores menores a 50 dieron lugar a simulaciones poco fluidas, es por estos motivos que se tomo un valor de 50 para el timeMultiplier lo que resulto en un valor de (SECONDS_PER_DAY * 5/6) en la variable de timeStep.

## Verificación del tipo de datos float

[completar]

## Complejidad computacional con asteroides

[completar]

## Mejora de la complejidad computacional

[completar]

## Bonus points

[completar]
