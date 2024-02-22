package com.example.irobotapplication;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatImageButton;

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
                do {
                    GlobalVars.arduinoIP = connect.tryConnection();
                } while (GlobalVars.arduinoIP == null);
                // Una volta che la connessione è stabilita, avvia MainActivity
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        startActivity(new Intent(LoadingActivity.this, MainActivity.class));
                        finish(); // Chiudi la LoadingActivity per evitare di tornarci con il tasto indietro
                    }
                });
            }
        }).start(); //start() avvia l'esecuzione del thread, in modo che run() sia asincrono rispetto al thread principale dell'UI
    }
}
