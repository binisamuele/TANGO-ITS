package com.example.irobotapplication;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.fragment.app.Fragment;

public class HomeFragment extends Fragment {

    public HomeFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        requireActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        View view = inflater.inflate(R.layout.fragment_home, container, false);

        // Accedi a TemperatureView e imposta i valori della temperatura e dell'umidità
        TemperatureView temperatureView = view.findViewById(R.id.temperatureView);
        temperatureView.setTemperature("25°C");
        temperatureView.setHumidity("50%");

        return view;
    }
}
