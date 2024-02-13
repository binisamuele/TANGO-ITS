package com.example.irobotapplication;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;

import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;

public class ConnectivityCheck {
    Context context;
    private static final int INTERVAL_MILLISECONDS = 3000;

    private Handler handler;
    private Runnable apiRunnable;

    public ConnectivityCheck(Context context) {
        this.context = context;

        handler = new Handler(Looper.getMainLooper());
        apiRunnable = new Runnable() {
            @Override
            public void run() {
                // Create a new thread for API call
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        // Call your API sending method here
                        sendPostRequest(context.getString(R.string.conn_string) + "connection-check");
                    }
                }).start();

                // Schedule the next execution
                handler.postDelayed(this, INTERVAL_MILLISECONDS);
            }
        };
    }

    public void startSending() {
        // Start sending API requests
        handler.post(apiRunnable);
    }

    public void stopSending() {
        // Stop sending API requests
        handler.removeCallbacks(apiRunnable);
    }

    private void sendPostRequest(String apiEndpoint) {
        try {
            URL url = new URL(apiEndpoint);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("POST");
            connection.setDoOutput(true);
            connection.setRequestProperty("Content-Type", "application/json");

            // Your payload data goes here
            String postData = "{\"androidCheck\":\"ok\"}";

            try (OutputStream os = connection.getOutputStream();
                 OutputStreamWriter osw = new OutputStreamWriter(os, StandardCharsets.UTF_8)) {

                osw.write(postData);
                osw.flush();
            }

            int responseCode = connection.getResponseCode();

            connection.disconnect();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
