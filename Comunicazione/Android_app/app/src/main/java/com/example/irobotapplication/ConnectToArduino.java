package com.example.irobotapplication;

import android.util.Log;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;

public class ConnectToArduino {

    private static final int SEND_PORT = 12345; // Change to your desired port for sending
    private static final int RECEIVE_PORT = 8787; // Change to your desired port for receiving
    private static final String BROADCAST_ADDRESS = "255.255.255.255"; // Broadcast address

    public void startBroadcasting() {
        new Thread(() -> {
            try {
                DatagramSocket socket = new DatagramSocket();
                while (true) {
                    String message = "Hello";
                    DatagramPacket packet = new DatagramPacket(message.getBytes(), message.length(),
                            InetAddress.getByName(BROADCAST_ADDRESS), SEND_PORT);
                    socket.send(packet);
                    Thread.sleep(1000); // adjust the interval as needed
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }).start();
    }

    public void startListening() {
        new Thread(() -> {
            while (true){
            try {
                DatagramSocket socket = new DatagramSocket(8787);
                socket.setSoTimeout(30000);
                byte[] buffer = new byte[1024];
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                socket.receive(packet);
                String received = new String(packet.getData(), 0, packet.getLength());
                Log.d("ConnectToArduino","Received: " + received);
                GlobalVars.arduinoIP = received;
                socket.close();
            } catch (Exception e) {
                if (e instanceof SocketTimeoutException) {
                    Log.d("ConnectToArduino", "Timeout occurred");
                } else if (e instanceof SocketException) {
                    Log.d("ConnectToArduino", "SocketException occurred");
                } else {
                    e.printStackTrace();
                }

            }
        }
        }).start();
    }
}
