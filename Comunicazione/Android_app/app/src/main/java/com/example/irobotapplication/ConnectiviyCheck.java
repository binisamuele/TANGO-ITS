package com.example.irobotapplication;

import android.content.Context;
import android.os.Looper;
import android.os.Handler;
import android.util.Log;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

public class ConnectiviyCheck {

    Context context;
    private Handler handler;
    public ConnectiviyCheck(Context context) {
        this.context = context;
        this.handler = new Handler(Looper.getMainLooper());
    }

    public void startPeriodicRequests() {
        // Schedule a periodic task using Handler.postDelayed
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                // Make your periodic request
                sendGetRequest();

                // Schedule the next periodic task after a delay of 2.5s
                handler.postDelayed(this, 2500);
            }
        }, 10000); // Initial delay before the first periodic task
    }

    public void stopPeriodicRequests() {
        handler.removeCallbacksAndMessages(null);
    }

    private void sendGetRequest() {
        try {
            HttpURLConnection connection = (HttpURLConnection) new URL(context.getString(R.string.conn_string) + "connection-check").openConnection();
            connection.setRequestMethod("GET");

            int responseCode = connection.getResponseCode();

            if (responseCode != HttpURLConnection.HTTP_OK) {
                Log.e("ConnectivityCheck", "Connection failed");
            } else {
                Log.i("ConnectivityCheck", "Connection successful");
            }

            connection.disconnect();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
