package com.example.irobotapplication;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.content.ContextCompat;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import android.os.Bundle;
import android.view.MenuItem;
import android.widget.Toast;

import com.google.android.material.navigation.NavigationView;
import android.content.res.ColorStateList;


public class MainActivity extends AppCompatActivity {

    DrawerLayout drawerLayout;
    Toolbar toolbar;
    NavigationView navigationView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        toolbar = findViewById(R.id.toolbar);
        drawerLayout = findViewById(R.id.drawerLayout);
        navigationView = findViewById(R.id.nav);

        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawerLayout, toolbar, R.string.open, R.string.close);
        drawerLayout.addDrawerListener(toggle);
        toggle.syncState();
        toggle.getDrawerArrowDrawable().setColor(getResources().getColor(R.color.white));

        navigationView.setNavigationItemSelectedListener(new NavigationView.OnNavigationItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                int itemId = item.getItemId();
                if (itemId == R.id.home) {
                    drawerLayout.closeDrawer(GravityCompat.START);
                    fragmentR(new HomeFragment());
                } else if (itemId == R.id.controller) {
                    fragmentR(new ControllerFragment());
                    drawerLayout.closeDrawer(GravityCompat.START);
                } else if (itemId == R.id.lidar) {
                    fragmentR(new LidarFragment());
                    drawerLayout.closeDrawer(GravityCompat.START);
                } else if (itemId == R.id.telecamere) {
                    fragmentR(new FotocamereFragment());
                    drawerLayout.closeDrawer(GravityCompat.START);
                }

                ColorStateList selectedTextColor = new ColorStateList(
                        new int[][]{{android.R.attr.state_checked}, {}},
                        new int[]{ContextCompat.getColor(MainActivity.this, R.color.selected_text_color),
                                ContextCompat.getColor(MainActivity.this, R.color.default_text_color)});

                navigationView.setItemTextColor(selectedTextColor);
                navigationView.setItemIconTintList(selectedTextColor);


                return true;
            }
        });
        MenuItem homeItem = navigationView.getMenu().findItem(R.id.home);
        if (homeItem != null) {
            homeItem.setChecked(true); // Imposta l'elemento "Home" come selezionato
            navigationView.setCheckedItem(R.id.home); // Imposta l'elemento "Home" come selezionato visivamente

            // Cambia il colore del testo e delle icone per l'elemento "Home"
            ColorStateList initialTextColor = new ColorStateList(
                    new int[][]{{android.R.attr.state_checked}, {}},
                    new int[]{ContextCompat.getColor(MainActivity.this, R.color.selected_text_color),
                            ContextCompat.getColor(MainActivity.this, R.color.default_text_color)});

            navigationView.setItemTextColor(initialTextColor);
            navigationView.setItemIconTintList(initialTextColor);
        }

        if (savedInstanceState == null) {
            fragmentR(new HomeFragment());
            navigationView.setCheckedItem(R.id.home); // Imposta l'elemento di menu Home come selezionato
        }
    }

    private void fragmentR(Fragment fragment) {
        FragmentManager fragmentManager = getSupportFragmentManager();
        FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
        fragmentTransaction.replace(R.id.frameLayout, fragment);
        fragmentTransaction.commit();
    }
}