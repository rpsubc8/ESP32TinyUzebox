# Tiny ESP32 UZEBOX
Hace un año comencé el port del emulador UZEM (PC x86) al ESP32. Eliminé el soporte SD y el sonido, siendo muy fácil el port.<br>
Se conseguía una velocidad ridícula de 1/2 fps. Optimizando y ajustando, se consiguió aumentar los fps, pero seguía siendo muy lento.<br>
Decidí eliminar la interrupción del timer y del KERNEL, reimplementando algún modo de video, logrando 20 fps, pero emulando un AVR a 4 Mhz, en lugar de los 28 Mhz.<br>
Fue cuando decidí realizar el port de la API, logrando no sólo el tiempo real, sino superar los 28 Mhz con creces.

<br><br>
<h1>Demo tutorial</h1>
Se ha testeado con éxito. Usa el modo de video 1.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/tutorial.gif'></center>
<ul>
 <li>240 x 224</li> 
 <li>Tiles de 6 x 8</li>
 <li>256 tiles</li>
 <li>Impresión de texto</li>
</ul>
