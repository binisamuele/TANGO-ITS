package com.example.irobotapplication;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatImageButton;

import java.util.Objects;

public class LoadingActivity extends AppCompatActivity {
    private ConnectToArduino connect;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_loading);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        connect = new ConnectToArduino();
        new Thread(new Runnable() {
            @Override
            public void run() {
                connect.startListening();
                connect.startBroadcasting();
                while (Objects.equals(GlobalVars.arduinoIP, "") || !GlobalVars.isArduinoConnected) {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                // Una volta che la connessione è stabilita, avvia MainActivity
                startActivity(new Intent(LoadingActivity.this, MainActivity.class));
                finish(); // Chiudi la LoadingActivity per evitare di tornarci con il tasto indietro
            }
        }).start(); //start() avvia l'esecuzione del thread, in modo che run() sia asincrono rispetto al thread principale dell'UI
    }
}
