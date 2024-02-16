package com.example.irobotapplication;

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
}
