# Communication Module

This sub repo contains the code that allows the communication between the **Android app controller** and **Arduino MEGA**. Arduino MEGA will send all the inputs recieved by the android app to another Arduino which controls the two engines of the car.


## Android App
This is the UI that gives a simply way to control the car. The Android app has been developed using **Android Studio** by JetBrains. All the inputs from this application are sent to a Node.js server via HTTP POST requests.   

[![Static Badge](https://img.shields.io/badge/Android%20Studio-%233DDC84?style=for-the-badge&logo=androidstudio&logoColor=white)](#)
[![Java](https://img.shields.io/badge/java-%23ED8B00.svg?style=for-the-badge&logo=openjdk&logoColor=white)](#)

## Node.js Server
We developed this Node.js server to handle all the inputs received from the Android app. This server forwards all those inputs to Arduino MEGA.

[![Static Badge](https://img.shields.io/badge/Node.js-%23339933?style=for-the-badge&logo=nodedotjs&logoColor=white)](#)

## Arduino MEGA
Arduino MEGA is the last node of this communication module. We added an Ethernet shield on Arduino to allow the communication with all the other nodes.  ***ArduinoServer.ino*** file contains the code to configure Arduino to work as a server using the library *Ethernet.h*
<p>

[![Static Badge](https://img.shields.io/badge/Arduino-%2300878F?style=for-the-badge&logo=arduino)](#)[
![Static Badge](https://img.shields.io/badge/C%2B%2B-%2300599C?style=for-the-badge&logo=cplusplus)](#)
</p>
