package com.example.irobotapplication;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

public class LoadingFragment extends Fragment {
    
    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {

        View loadingView = inflater.inflate(R.layout.activity_loading, container, false);

        return loadingView;
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        requireActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        connect = new ConnectToArduino();
        do {
            // prova a connetterti all'Arduino e scamabia l'IP
            GlobalVars.arduinoIP = connect.tryConnection();
        } while (GlobalVars.arduinoIP == null);

        // chiudi il fragment
        requireActivity().getSupportFragmentManager().beginTransaction().remove(this).commit();
    }
}
