package de.fre4242.WifiTemperatureSensor;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.webkit.WebView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.preference.PreferenceManager;

public class MainActivity extends AppCompatActivity {

    private WebView webView;

    @SuppressLint("SetJavaScriptEnabled")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        webView = findViewById(R.id.webView);
        webView.getSettings().setJavaScriptEnabled(true);
        SharedPreferences myPreference = PreferenceManager.getDefaultSharedPreferences(this);
        String sig = myPreference.getString("ipAddressSetting", "http://192.168.178.1");
        if (!sig.startsWith("http")) {
            sig = "http://" + sig;
        }
        webView.loadUrl(sig);
    }

    @Override
    protected void onResume() {
        super.onResume();
        webView = findViewById(R.id.webView);
        SharedPreferences myPreference = PreferenceManager.getDefaultSharedPreferences(this);
        String sig = myPreference.getString("ipAddressSetting", "http://192.168.178.1");
        if (!sig.startsWith("http")) {
            sig = "http://" + sig;
        }
        webView.loadUrl(sig);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.settings_menu, menu);
        return true;
    }

    @SuppressLint("NonConstantResourceId")
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
            case R.id.open_settings:
                Intent myIntent = new Intent(this, SettingsActivity.class);
                this.startActivity(myIntent);
                return true;
            case R.id.reload:
                webView.reload();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }


}