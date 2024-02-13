package com.example.irobotapplication;

import static android.view.View.MeasureSpec.getMode;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.ColorStateList;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatImageButton;
import androidx.fragment.app.Fragment;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Switch;
import android.widget.Toast;

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

    private ConnectivityCheck check;
    private ActionBar actionBar;

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

        check = new ConnectivityCheck(requireActivity().getApplicationContext());
        check.startSending();

        //using this boolean to prevent multiple buttons from being pressed at the same time
        AtomicBoolean anyButtonPressed = new AtomicBoolean(false);

        buttonForward.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_UP && anyButtonPressed.get()) {
                // when button is released
                anyButtonPressed.set(false);
                postToServer(getString(R.string.conn_string) + "control", "released");
                buttonForward.setImageResource(R.drawable.vettoreup_bianco__removebg_preview);
                return true;
            } else if (event.getAction() == MotionEvent.ACTION_DOWN && !anyButtonPressed.get()) {
                // when button is pressed
                anyButtonPressed.set(true);
                postToServer(getString(R.string.conn_string) + "control", "up");
                buttonForward.setImageResource((R.drawable.vettoreup_rosso__removebg_preview));
                return true;
            }
            return false;
        });

        buttonLights.setOnClickListener(new View.OnClickListener() {
            boolean lights_state = false; // Stato iniziale
            @Override
            public void onClick(View v) {
                if (!lights_state) {
                    // Se le luci sono spente, accendile
                    buttonLights.setImageResource(R.drawable.lights_on);
                    lights_state = true;
                } else {
                    // Se le luci sono accese, spegnile
                    buttonLights.setImageResource(R.drawable.lights_off);
                    lights_state = false;
                }
            }
        });

        buttonBackwards.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_UP && anyButtonPressed.get()) {
                // when button is released
                anyButtonPressed.set(false);
                postToServer(getString(R.string.conn_string) + "control", "released");
                buttonBackwards.setImageResource(R.drawable.vettoredown_bianco__removebg_preview);
                return true;
            } else if (event.getAction() == MotionEvent.ACTION_DOWN && !anyButtonPressed.get()) {
                // when button is pressed
                anyButtonPressed.set(true);
                postToServer(getString(R.string.conn_string) + "control", "down");
                buttonBackwards.setImageResource((R.drawable.vettoredown_rosso__removebg_preview));
                return true;
            }
            return false;
        });

        buttonRotSx.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_UP && anyButtonPressed.get()) {
                // when button is released
                anyButtonPressed.set(false);
                postToServer(getString(R.string.conn_string) + "control", "released");
                buttonRotSx.setImageResource(R.drawable.rotsx_bianco_2);
                return true;
            } else if (event.getAction() == MotionEvent.ACTION_DOWN && !anyButtonPressed.get()) {
                // when button is pressed
                anyButtonPressed.set(true);
                postToServer(getString(R.string.conn_string) + "control", "left");
                buttonRotSx.setImageResource(R.drawable.rotsx_rosso__removebg_preview);
                return true;
            }
            return false;
        });

        buttonRotDx.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_UP && anyButtonPressed.get()) {
                // when button is released
                anyButtonPressed.set(false);
                postToServer(getString(R.string.conn_string) + "control", "released");
                buttonRotDx.setImageResource(R.drawable.rotdx_bianco);
                return true;
            } else if (event.getAction() == MotionEvent.ACTION_DOWN && !anyButtonPressed.get()) {
                // when button is pressed
                anyButtonPressed.set(true);
                postToServer(getString(R.string.conn_string) + "control", "right");
                buttonRotDx.setImageResource(R.drawable.rotdx_rosso__removebg_preview);
                return true;
            }
            return false;
        });

        //CONTROLLO SWITCH
        switchOnOff.setOnCheckedChangeListener((v, event) -> {

        });
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();

        // Ripristina la toolbar quando il fragment viene distrutto
        if (actionBar != null) {
            actionBar.show();
        }

        check.stopSending();
        postToServer(getString(R.string.conn_string) + "control", "commStop");
    }

    private void postToServer(String url, String direction) {

        RequestQueue queue = Volley.newRequestQueue(requireContext());

        StringRequest stringRequest = new StringRequest(Request.Method.POST, url,
                response -> Log.d("HTTP-POST", "Response: " + response),
                error -> {
                    // Handle errors
                    Toast.makeText(fragmentContext, "Errore di connessione", Toast.LENGTH_SHORT).show();
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
}