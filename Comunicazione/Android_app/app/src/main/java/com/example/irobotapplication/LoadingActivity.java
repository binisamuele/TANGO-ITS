package com.example.irobotapplication;

import android.content.pm.ActivityInfo;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

public class LoadingActivity extends AppCompatActivity {
    private ConnectToArduino connect;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_loading);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        connect = new ConnectToArduino();
        do {
            // prova a connetterti all'Arduino e scamabia l'IP
            GlobalVars.arduinoIP = connect.tryConnection();
        } while (GlobalVars.arduinoIP == null);
    }
}
