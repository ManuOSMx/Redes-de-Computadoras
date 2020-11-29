#!/bin/bash
clear
gcc Servidor.c -o Servidor
gcc Costumer.c -o Costumer
clear
gnome-terminal -- ./Costumer
./Servidor
