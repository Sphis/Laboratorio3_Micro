#!/usr/bin/env python3
import serial

ser = serial.Serial("/tmp/ttyS1", 9600) # Crear objeto para la comunicacion

archivo = open("medidas.csv", 'w') # Crear el archivo csv en modo escritura

i  = 0 # Contador para ver si se han registrado los 4 datos de cada canal

while (1):
    medidas = str(ser.readline())
    datos = medidas[2:][:-5] # Eliminar caracteres especiales como \n de la impresion
    print(datos) # Imprimir valores en terminal

    if i == 3:
    	# Datos por fila
        archivo = open("medidas.csv", "a")
        archivo.write(datos + "\n")
        i = 0 # Resetear contador
    else:
    	# Datos por columna
        archivo.write(datos + ";")
        i+=1
