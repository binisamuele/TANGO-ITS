package com.example.irobotapplication;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

// HomeFragment.java
public class HomeFragment extends Fragment {

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

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_home, container, false);
        return rootView;
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        requireActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    }

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
}
