package com.example.irobotapplication;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

public class LoadingFragment extends Fragment {

    private LoadingListener loadingListener;

    private ConnectToArduino connect;

    public void setLoadingListener(LoadingListener listener) {
        this.loadingListener = listener;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        // Implementa la UI del tuo fragment di caricamento
        // ...

        // Simula il completamento del caricamento dopo un certo tempo
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                // Notifica la MainActivity che il caricamento è completato
                if (loadingListener != null) {
                    loadingListener.onLoadingFinished();
                }
            }
        }, 3000); // Simula il caricamento per 3 secondi

        return super.onCreateView(inflater, container, savedInstanceState);
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
