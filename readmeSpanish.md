# Tiny ESP32 UZEBOX
Hace un año comencé el port del emulador UZEM (PC x86) al ESP32. Eliminé el soporte SD y el sonido, siendo muy fácil el port.<br>
Se conseguía una velocidad ridícula de 1/2 fps. Optimizando y ajustando, se consiguió aumentar los fps, pero seguía siendo muy lento.<br>
Decidí eliminar la interrupción del timer y del KERNEL, reimplementando algún modo de video, logrando 20 fps, pero emulando un AVR a 4 Mhz, en lugar de los 28 Mhz, por lo que no era muy prometedor.<br>
Fue cuando decidí encaminar el proyecto por otro camino, que consiste en realizar el port de la API, logrando no sólo el tiempo real, sino superar los 28 Mhz con creces.

<br><br>
<h1>Características</h1>
Es una versión de prueba de concepto con 5 demos, que aún está muy inestable.
<ul>
 <li>No se usa PSRAM, funcionando en ESP32 de 520 KB de RAM (TTGO VGA32 v1.0, v1.1, v1.2 y v1.4)</li>
 <li>Uso de un sólo core</li>
 <li>OSD de bajos recursos (Tecla F1)</li>
 <li>Menú de velocidad de CPU (normal, Turbo)</li>
 <li>Modo 64 colores</li>
 <li>VGA 320x240</li>
 <li>Cursores arriba, abajo, izquierda, derecha</li>
 <li>Tecla X (botón A), tecla Z (botón B)</li>
 <li>Tecla C (botón SELECT), tecla V (botón START)</li>
</ul>


<br><br>
<h1>Requerimientos</h1>
Se requiere:
<ul>
 <li>TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4)</li>
 <li>Visual Studio 1.66.1 PLATFORMIO 2.4.3 Espressif32 v3.5.0 (python 3.6)</li>
 <li>El proyecto no está aún preparado para Arduino IDE</li>
 <li>Librería reducida Arduino bitluni 0.3.3 (ya incluida en el proyecto)</li>
</ul>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/ttgovga32v12.jpg'></center>


<br><br>
<h1>Demo tutorial</h1>
Se ha testeado con éxito. Usa el modo de video 1.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/tutorial.gif'></center>
<ul>
 <li>240 x 224</li> 
 <li>Tiles de 6 x 8 pixels</li>
 <li>256 tiles</li>
 <li>64 bytes por Tile</li>
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
 <li>64 bytes por Tile</li>
 <li>Test del gamepad</li>
</ul>


<br><br>
<h1>Demo Bitmap</h1>
Se ha testeado con éxito. Usa el modo de video 8.
<center>
 <table>
  <tr>
   <th></th>
   <th></th>
   <th></th>
  </tr>
  <tr>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo01.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo02.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo03.gif'></td>
  </tr>
  <tr>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo04.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo05.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo06.gif'> </td>
  </tr>
  <tr>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo07.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/bitmapdemo08.gif'></td>
   <td></td>
  </tr>
 </table> 
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
 <table>
  <tr>
   <th>Menú</th>
   <th>Juego</th>
   <th>SetRenderingParameters</th>
  </tr>
  <tr>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/chess4uzebox01.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/chess4uzebox02.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/chess4uzebox03.gif'></td>
  </tr>
 </table>
</center> 
<ul>
 <li>240 x 224 resolución</li> 
 <li>Pantalla de 40 x 28 Tiles</li>
 <li>Tiles de 6 x 8 pixels</li> 
 <li>48 bytes por Tile</li>
</ul>


<br><br>
<h1>Demo MegaSokoban</h1>
Se ha testeado con éxito. Usa el modo de video 10.
<center>
 <table>
  <tr>
   <th>Creditos</th>
   <th>Tutorial</th>
   <th>Juego</th>
   <th>Nivel</th>
   <th>Juego</th>
  </tr>
  <tr>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/megasokoban01.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/megasokoban02.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/megasokoban03.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/megasokoban04.gif'></td>
   <td><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TinyUzebox/main/preview/demos/megasokoban05.gif'></td>
  </tr>
 </table>
</center>
<ul>
 <li>192 x 192 resolución</li> 
 <li>Pantalla de 16 x 12 Tiles</li>
 <li>Tiles de 12 x 16 pixels</li> 
 <li>192 bytes por Tile</li>
</ul>
 
 
<br><br>
<h1>API</h1>
Se han portado las siguientes funciones al ESP32 TTGO VGA 1.x (1.0, 1.1, 1.2, 1.4):
<ul>
 <li>WaitVsync</li> 
 <li>ClearVram</li>
 <li>DrawMap2</li>
 <li>FadeIn</li>
 <li>FadeOut</li>
 <li>Fill</li>
 <li>SetFontTable</li>
 <li>PrintChar</li>
 <li>Print</li>
 <li>SetFontTilesIndex</li>
 <li>SetFont</li>
 <li>SetTileTable</li>
 <li>SetSpritesTileTable</li>
 <li>SetTile</li>
 <li>SetRenderingParameters</li>
 <li>ReadJoypad</li>
 <li>DetectControllers</li>
 <li>EepromReadBlock</li>
 <li>EepromWriteBlock</li>
 <li>pgm_read_byte</li>
</ul>

El sonido lo dejo siempre para el final. Las siguientes funciones estás creadas para compilar proyectos, pero no reproducen ningun sonido.
<ul>
 <li>InitMusicPlayer</li>
 <li>ResumeSong</li>
 <li>SetMasterVolume</li>
 <li>StartSong</li>
 <li>StopSong</li>
 <li>TriggerFx</li>
 <li>TriggerNote</li>
 <li>EnableSoundEngine</li>
 <li>DisableSoundEngine</li>
</ul>
También se ha implementado toda la estructura de <b>vram</b>, de manera que si en tiempo real escribimos en el buffer de vram, se verá por pantalla los cambios, es decir, recrea el comportamiento de la <b>UZEBOX</b>.
<br><br>
