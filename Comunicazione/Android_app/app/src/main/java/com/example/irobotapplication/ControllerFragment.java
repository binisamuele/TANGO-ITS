package com.example.irobotapplication;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatImageButton;
import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Switch;
import android.widget.TextView;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.RetryPolicy;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;

public class ControllerFragment extends Fragment {
    private ActionBar actionBar;
    private static final long CHECK_INTERVAL = 1000;
    private final Handler handler = new Handler();
    private final Runnable periodicTask = new Runnable() {
        @Override
        public void run() {
            if (!GlobalVars.isArduinoConnected) {
                startActivity(new Intent(getActivity(), LoadingActivity.class));
            }
            // Schedule the task to run again after CHECK_INTERVAL
            handler.postDelayed(this, CHECK_INTERVAL);
        }
    };

    @Override
    public void onResume() {
        super.onResume();
        // Start the periodic task when the fragment resumes
        handler.postDelayed(periodicTask, CHECK_INTERVAL);
    }

    @Override
    public void onPause() {
        super.onPause();
        // Stop the periodic task when the fragment is paused to prevent memory leaks
        handler.removeCallbacks(periodicTask);
    }

    public ControllerFragment() {
        // Required empty public constructor
    }

    private Context fragmentContext;

    @Override
    public void onAttach(@NonNull Context context) {
        super.onAttach(context);
        fragmentContext = context;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("FragmentController", "onCreate");
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_controller, container, false);
    }

    @SuppressLint("ClickableViewAccessibility")
    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        /* Test UpdateSpeed
        float currentSpeed = 2.5f;
        updateSpeed(currentSpeed);
         */

        // Nasconde la toolbar nell'activity associata al fragment
        AppCompatActivity activity = (AppCompatActivity) getActivity();
        if (activity != null) {
            actionBar = activity.getSupportActionBar();
            if (actionBar != null) {
                actionBar.hide();
            }
        }

        requireActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        AppCompatImageButton buttonLights = requireView().findViewById(R.id.btnLights);
        AppCompatImageButton buttonForward = requireView().findViewById(R.id.bntUp);
        AppCompatImageButton buttonBackwards = requireView().findViewById(R.id.btnDown);
        AppCompatImageButton buttonRotSx = requireView().findViewById(R.id.btnRotSX);
        AppCompatImageButton buttonRotDx = requireView().findViewById(R.id.btnRotDX);
        Switch switchOnOff = requireView().findViewById(R.id.switch_OnOff);
        AppCompatImageButton btnStop = requireView().findViewById(R.id.btnStop);
        switchOnOff.setChecked(false);

        //using this boolean to prevent multiple buttons from being pressed at the same time
        AtomicBoolean anyButtonPressed = new AtomicBoolean(false);

        buttonForward.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_UP && anyButtonPressed.get() && switchOnOff.isChecked()) {
                // when button is released
                anyButtonPressed.set(false);
                postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "released");
                buttonForward.setImageResource(R.drawable.vettoreup_bianco__removebg_preview);
                return true;
            } else if (event.getAction() == MotionEvent.ACTION_DOWN && !anyButtonPressed.get() && switchOnOff.isChecked()) {
                // when button is pressed
                anyButtonPressed.set(true);
                postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "up");
                buttonForward.setImageResource((R.drawable.vettoreup_rosso__removebg_preview));
                return true;
            }
            return false;
        });

        // TODO postToServer delle luci, però per ora non sono state montate
        buttonLights.setOnClickListener(new View.OnClickListener() {
            boolean lights_status = false; // Stato iniziale
            @Override
            public void onClick(View v) {
                if (!lights_status) {
                    // Se le luci sono spente, accendile
                    buttonLights.setImageResource(R.drawable.lights_on);
                    lights_status = true;
                } else {
                    // Se le luci sono accese, spegnile
                    buttonLights.setImageResource(R.drawable.lights_off);
                    lights_status = false;
                }
            }
        });

        buttonBackwards.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_UP && anyButtonPressed.get() && switchOnOff.isChecked()) {
                // when button is released
                anyButtonPressed.set(false);
                postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "released");
                buttonBackwards.setImageResource(R.drawable.vettoredown_bianco__removebg_preview);
                return true;
            } else if (event.getAction() == MotionEvent.ACTION_DOWN && !anyButtonPressed.get() && switchOnOff.isChecked()) {
                // when button is pressed
                anyButtonPressed.set(true);
                postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "down");
                buttonBackwards.setImageResource((R.drawable.vettoredown_rosso__removebg_preview));
                return true;
            }
            return false;
        });

        buttonRotSx.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_UP && anyButtonPressed.get() && switchOnOff.isChecked()) {
                // when button is released
                anyButtonPressed.set(false);
                postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "released");
                buttonRotSx.setImageResource(R.drawable.rotsx_bianco_2);
                return true;
            } else if (event.getAction() == MotionEvent.ACTION_DOWN && !anyButtonPressed.get() && switchOnOff.isChecked()) {
                // when button is pressed
                anyButtonPressed.set(true);
                postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "left");
                buttonRotSx.setImageResource(R.drawable.rotsx_rosso__removebg_preview);
                return true;
            }
            return false;
        });

        buttonRotDx.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_UP && anyButtonPressed.get() && switchOnOff.isChecked()) {
                // when button is released
                anyButtonPressed.set(false);
                postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "released");
                buttonRotDx.setImageResource(R.drawable.rotdx_bianco);
                return true;
            } else if (event.getAction() == MotionEvent.ACTION_DOWN && !anyButtonPressed.get() && switchOnOff.isChecked()) {
                // when button is pressed
                anyButtonPressed.set(true);
                postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "right");
                buttonRotDx.setImageResource(R.drawable.rotdx_rosso__removebg_preview);
                return true;
            }
            return false;
        });

        //CONTROLLO SWITCH
        switchOnOff.setOnCheckedChangeListener((v, event) -> {
            if (!switchOnOff.isChecked()) {
                postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "commStop");
            }
            if (switchOnOff.isChecked()) {

            }
        });

        AtomicBoolean stopClicked = new AtomicBoolean(false);
        btnStop.setOnClickListener(v -> {
            if(!stopClicked.get()){
                // se il bottone viene cliccato e stopclicked = false cambia immagine e stopclicked = true
                postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "emergencyStop");
                switchOnOff.setChecked(false);
                btnStop.setImageResource(R.drawable.stop_button_clicked);
                stopClicked.set(true);
            }
            else{
                btnStop.setImageResource(R.drawable.stop_button_default);
                stopClicked.set(false);
            }
        });
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();

        // Ripristina la toolbar quando il fragment viene distrutto
        if (actionBar != null) {
            actionBar.show();
        }
        postToServer("http://" + GlobalVars.arduinoIP + GlobalVars.arduinoPort, "commStop");
    }

    private void postToServer(String url, String direction) {

        RequestQueue queue = Volley.newRequestQueue(requireContext());
        Log.d("PostToServer", "URL: " + url);
        StringRequest stringRequest = new StringRequest(Request.Method.POST, url,
                response -> Log.d("HTTP-POST", "Response: " + response),
                error -> {
                    // Handle errors
                    //Toast.makeText(fragmentContext, "Errore di connessione", Toast.LENGTH_SHORT).show();
                    Log.e("HTTP-POST", "Error: " + error.toString());
                })
        {
            @Override
            public byte[] getBody() {
                // define the POST parameters as JSON.
                JSONObject jsonBody = new JSONObject();
                try {
                    jsonBody.put("direction", direction);
                } catch (JSONException e) {
                    Log.e("JSON in POST", "JSON Error: " + e.getMessage());
                }
                return jsonBody.toString().getBytes(StandardCharsets.UTF_8);
            }

            @Override
            public Map<String, String> getHeaders() {
                // set headers, including Content-Type.
                Map<String, String> headers = new HashMap<>();
                headers.put("Content-Type", "application/json");
                return headers;
            }

            @Override
            public String getBodyContentType() {
                return "application/json";
            }

            @Override
            public RetryPolicy getRetryPolicy() {
                return new DefaultRetryPolicy(
                        1000,
                        0,
                        1
                );
            }
        };
        queue.add(stringRequest);
    }

    //TODO (bisogna richiamare il metodo in modo continuo per aggiornare la velocità.
    // la si chiama tramite i button forward e backwards o ????)


    private void updateSpeed(float currentSpeed) {
        
        // Aggiornamento della velocità
        TextView speedTextView = requireView().findViewById(R.id.Speed);
        speedTextView.setText(Float.toString(currentSpeed));

        // Cambio colori in base alla velocità
        if (currentSpeed == 0) {
            speedTextView.setTextColor(getResources().getColor(R.color.Panna));
        } else if (currentSpeed > 0 && currentSpeed <= 2) {
            speedTextView.setTextColor(getResources().getColor(R.color.Green));
        } else if (currentSpeed > 2 && currentSpeed <= 4) {
            speedTextView.setTextColor(getResources().getColor(R.color.Yellow));
        } else {
            speedTextView.setTextColor(getResources().getColor(R.color.red));
        }
    }

}