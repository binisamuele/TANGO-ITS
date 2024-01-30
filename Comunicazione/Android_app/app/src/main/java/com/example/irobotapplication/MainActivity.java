package com.example.irobotapplication;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.text.SpannableString;
import android.text.style.ForegroundColorSpan;
import android.view.Menu;
import android.view.MenuItem;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.content.ContextCompat;
import androidx.core.graphics.drawable.DrawableCompat;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.google.android.material.navigation.NavigationView;

public class MainActivity extends AppCompatActivity {

    DrawerLayout drawerLayout;
    Toolbar toolbar;
    NavigationView navigationView;

    // Variabile di stato per tracciare la visibilità della toolbar
    private boolean isToolbarHidden = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        toolbar = findViewById(R.id.toolbar);
        drawerLayout = findViewById(R.id.drawerLayout);
        navigationView = findViewById(R.id.nav);

        setSupportActionBar(toolbar);

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
                    setToolbarTitle("Home");
                    handleMenuItemSelection(item);
                } else if (itemId == R.id.controller) {
                    fragmentR(new ControllerFragment());
                    drawerLayout.closeDrawer(GravityCompat.START);
                    handleMenuItemSelection(item);
                } else if (itemId == R.id.lidar || itemId == R.id.telecamere) {
                    Fragment fragment;
                    String title;

                    if (itemId == R.id.lidar) {
                        fragment = new LidarFragment();
                        title = "Lidar";
                    } else {
                        fragment = new FotocamereFragment();
                        title = "Telecamere";
                    }

                    fragmentR(fragment);
                    drawerLayout.closeDrawer(GravityCompat.START);
                    setToolbarTitle(title);
                    handleMenuItemSelection(item);
                }

                return true;
            }
        });

        // Imposta il menu selezionato inizialmente
        setInitialMenuItem();

        // Carica il fragment iniziale solo se savedInstanceState è nullo
        if (savedInstanceState == null) {
            fragmentR(new HomeFragment());
            setToolbarTitle("Tango");
        }
    }

    private void fragmentR(Fragment fragment) {
        FragmentManager fragmentManager = getSupportFragmentManager();
        FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
        fragmentTransaction.replace(R.id.frameLayout, fragment);
        fragmentTransaction.commit();
    }

    private void setToolbarTitle(String title) {
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setTitle(title);
        }
    }

    private void setInitialMenuItem() {
        Menu menu = navigationView.getMenu();

        // Deseleziona manualmente tutti gli elementi del menu
        for (int i = 0; i < menu.size(); i++) {
            menu.getItem(i).setChecked(false);
        }

        // Imposta l'elemento del menu inizialmente selezionato
        MenuItem homeItem = menu.findItem(R.id.home);
        if (homeItem != null) {
            homeItem.setChecked(true);
            navigationView.setCheckedItem(R.id.home);

            // Carica la ColorStateList dal file di risorse
            int selectedTextColor = ContextCompat.getColor(this, R.color.selected_text_color);

            // Imposta la ColorStateList per l'elemento del menu
            tintMenuItemIcon(homeItem, selectedTextColor);
            setMenuItemTextColor(homeItem, selectedTextColor);

            // Imposta il colore di default per gli altri elementi del menu
            for (int i = 0; i < menu.size(); i++) {
                if (menu.getItem(i) != homeItem) {
                    tintMenuItemIcon(menu.getItem(i), ContextCompat.getColor(this, R.color.default_text_color));
                    setMenuItemTextColor(menu.getItem(i), ContextCompat.getColor(this, R.color.default_text_color));
                }
            }
        }
    }

    private void handleMenuItemSelection(MenuItem selectedItem) {
        Menu menu = navigationView.getMenu();

        // Deseleziona manualmente tutti gli elementi del menu
        for (int i = 0; i < menu.size(); i++) {
            menu.getItem(i).setChecked(false);
            tintMenuItemIcon(menu.getItem(i), ContextCompat.getColor(this, R.color.default_text_color));
            setMenuItemTextColor(menu.getItem(i), ContextCompat.getColor(this, R.color.default_text_color));
        }

        // Imposta la ColorStateList per l'elemento selezionato
        int selectedTextColor = ContextCompat.getColor(this, R.color.selected_text_color);
        tintMenuItemIcon(selectedItem, selectedTextColor);
        setMenuItemTextColor(selectedItem, selectedTextColor);
    }

    private void tintMenuItemIcon(MenuItem menuItem, int color) {
        Drawable icon = menuItem.getIcon();
        if (icon != null) {
            icon = DrawableCompat.wrap(icon);
            DrawableCompat.setTint(icon, color);
            menuItem.setIcon(icon);
        }
    }

    private void setMenuItemTextColor(MenuItem menuItem, int color) {
        SpannableString spanString = new SpannableString(menuItem.getTitle().toString());
        spanString.setSpan(new ForegroundColorSpan(color), 0, spanString.length(), 0);
        menuItem.setTitle(spanString);
    }


}
