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
 <li>Tiles de 6 x 8 pixels</li>
 <li>256 tiles</li>
 <li>Impresión de texto</li>
</ul>


<br><br>
<h1>Demo controller tester</h1>
Se ha testeado con éxito. Usa el modo de video 3.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/controllertester.gif'></center>
<ul>
 <li>240 x 224</li> 
 <li>Pantalla de 32 x 32 Tiles</li>
 <li>Tiles de 8 x 8 pixels</li>
 <li>256 tiles</li>
 <li>Test del gamepad</li>
</ul>


<br><br>
<h1>Demo Bitmap</h1>
Se ha testeado con éxito. Usa el modo de video 8.
<center>
 <img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo01.gif'>
 <img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo02.gif'>
 <img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo03.gif'>
 <img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo04.gif'>
 <img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo05.gif'>
 <img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo06.gif'> 
 <img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo07.gif'>
 <img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo08.gif'>
</center> 
<ul>
 <li>120 x 96 modo bitmap (framebuffer)</li> 
 <li>4 colores por pixel (2 bits)</li>
 <li>4 paletas de 4 colores de los 256 posibles</li> 
</ul>
 

<br><br>
<h1>Demo chess4uzebox</h1>
Se ha testeado con éxito. Usa el modo de video 5.
<center>
 <img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/chess4uzebox01.gif'>
 <img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/chess4uzebox02.gif'>
</center>
<ul>
 <li>240 x 224 resolution</li> 
 <li>Pantalla de 40 x 28 Tiles</li>
 <li>Tiles de 6 x 8 pixels/li> 
</ul>


<br><br>
