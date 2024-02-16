package com.example.irobotapplication;

import android.util.Log;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class ConnectToArduino {

    private static final int UDP_PORT = 8888;
    private static final String BROADCAST_IP = "192.168.1.255";

    public static void main(String[] args) {
        try {
            DatagramSocket socket = new DatagramSocket();

            // broadcast address
            InetAddress broadcastAddress = InetAddress.getByName(BROADCAST_IP);

            String message = "start";
            byte[] buffer = message.getBytes();
            DatagramPacket packet = new DatagramPacket(
                    buffer,
                    buffer.length,
                    broadcastAddress,
                    UDP_PORT
            );

            // sending the message on broadcast address
            socket.send(packet);

            // receive Arduino's IP address
            byte[] receiveBuffer = new byte[1024];
            DatagramPacket receivePacket = new DatagramPacket(receiveBuffer, receiveBuffer.length);
            socket.receive(receivePacket);
            String arduinoIP = new String(receivePacket.getData(), 0, receivePacket.getLength());
            System.out.println("Arduino's IP address: " + arduinoIP);

            byte[] androidIPBytes = InetAddress.getLocalHost().getAddress();
            DatagramPacket androidIPPacket = new DatagramPacket(
                    androidIPBytes,
                    androidIPBytes.length,
                    InetAddress.getByName(arduinoIP),
                    UDP_PORT
            );
            socket.send(androidIPPacket);

            // Close the socket
            socket.close();

            Log.d("UDP Broadcast", "Broadcasted message received: " + message);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
