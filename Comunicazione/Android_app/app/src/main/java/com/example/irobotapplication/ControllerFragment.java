package com.example.irobotapplication;

import android.annotation.SuppressLint;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class ControllerFragment extends Fragment {

    public ControllerFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_controller, container, false);
    }

    @SuppressLint("ClickableViewAccessibility")
    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Button buttonForward = requireView().findViewById(R.id.btnForward);
        Button buttonBackwards = requireView().findViewById(R.id.btnBackwards);
        Button buttonRotSx = requireView().findViewById(R.id.btnRotSx);
        Button buttonRotDx = requireView().findViewById(R.id.btnRotDx);

        buttonForward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_UP) {
                    //do something here when button is released
                    postToServer("stop");
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    //do something here when button is first clicked
                    postToServer("up");
                    return true;
                }

                return false;
            }
        });

        buttonBackwards.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_UP) {
                    //do something here when button is released
                    postToServer("stop");
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    //do something here when button is first clicked
                    postToServer("down");
                    return true;
                }

                return false;
            }
        });

        buttonRotSx.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_UP) {
                    //do something here when button is released
                    postToServer("stop");
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    //do something here when button is first clicked
                    postToServer("left");
                    return true;
                }

                return false;
            }
        });

        buttonRotDx.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_UP) {
                    //do something here when button is released
                    postToServer("stop");
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    //do something here when button is first clicked
                    postToServer("right");
                    return true;
                }

                return false;
            }
        });
    }
    private void postToServer(String direction) {
        String conn_string = "http://192.168.0.4:3000/control";

        RequestQueue queue = Volley.newRequestQueue(requireActivity().getApplicationContext());  // make sure that this thing works

        StringRequest stringRequest = new StringRequest(Request.Method.POST, conn_string,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        Log.d("HTTP-POST", "Response: " + response);
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        // Handle errors here.
                        Log.e("HTTP-POST", "Error: " + error.toString());
                    }
                })
        {
            @Override
            public byte[] getBody() {
                // Define the POST parameters as JSON.
                JSONObject jsonBody = new JSONObject();
                try {
                    jsonBody.put(direction, "up");
                } catch (JSONException e) {
                    Log.e("JSON in POST", "JSON Error: " + e.getMessage());
                }
                return jsonBody.toString().getBytes(StandardCharsets.UTF_8);
            }
            @Override
            public Map<String, String> getHeaders() {
                // Set headers, including Content-Type.
                Map<String, String> headers = new HashMap<>();
                headers.put("Content-Type", "application/json");
                return headers;
            }

            @Override
            public String getBodyContentType() {
                return "application/json";
            }
        };
        queue.add(stringRequest);
    }
}