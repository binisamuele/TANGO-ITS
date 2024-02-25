package com.example.irobotapplication;

import android.util.Log;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;

public class ConnectToArduino {
    private static final int RECEIVE_PORT = 8989;
    private static final int SEND_PORT = 7979;
    private static final String BROADCAST_ADDRESS = "192.168.1.255"; // Broadcast address

    public void startBroadcasting() {
        new Thread(() -> {
            DatagramSocket socket = null;
            int threadFrequency;
            try {
                while (true) {
                    threadFrequency = 1000;
                    socket = new DatagramSocket();
                    String message = "Android IP";
                    if (!GlobalVars.arduinoIP.equals("")) {
                        message = "ok";   // Notify arduino that its IP has been received
                        threadFrequency = 450;
                    }
                    DatagramPacket packet = new DatagramPacket(
                        message.getBytes(),
                        message.length(),
                        InetAddress.getByName(BROADCAST_ADDRESS),
                        SEND_PORT
                    );
                    socket.send(packet);
                    Thread.sleep(threadFrequency);
                    socket.close();
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (socket != null) {
                    socket.close();
                }
            }
        }).start();
    }

    public void startListening() {
        new Thread(() -> {
            int retries = 0;
            while (true){
                DatagramSocket socket = null;
                if (retries > 5) {
                    GlobalVars.isArduinoConnected = false;
                    GlobalVars.arduinoIP = "";
                    retries = 0;
                }
                try {
                    socket = new DatagramSocket(RECEIVE_PORT);
                    socket.setSoTimeout(1000);
                    byte[] buffer = new byte[1024];
                    DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                    socket.receive(packet);
                    String received = new String(packet.getData(), 0, packet.getLength());
                    //get the IP address of the arduino
                    GlobalVars.arduinoIP = packet.getAddress().getHostAddress();
                    if (received.contains("ok")){
                        GlobalVars.isArduinoConnected = true;
                    }
                    if (GlobalVars.isArduinoConnected && !GlobalVars.arduinoIP.equals("") && !received.contains("ok")) {
                        retries++;
                    }
                    socket.close();
                } catch (Exception e) {
                    if (e instanceof SocketTimeoutException) {
                        Log.d("ConnectToArduino", "Timeout occurred");
                        if (GlobalVars.isArduinoConnected && !GlobalVars.arduinoIP.equals("")) {
                            retries++;
                        }
                    } else if (e instanceof SocketException) {
                        Log.d("ConnectToArduino", "SocketException occurred");
                    } else {
                        e.printStackTrace();
                    }

                }
                finally {
                    if (socket != null) {
                        socket.close();
                    }
                }
        }
        }).start();
    }
}
