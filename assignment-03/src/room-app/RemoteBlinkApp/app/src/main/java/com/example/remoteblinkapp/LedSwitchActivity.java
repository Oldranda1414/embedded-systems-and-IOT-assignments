package com.example.remoteblinkapp;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.SeekBar;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.Timer;
import java.util.TimerTask;

@SuppressLint("MissingPermission")
public class LedSwitchActivity extends AppCompatActivity {

    private OutputStream bluetoothOutputStream;
    private Button ledButton;
    private SeekBar rollerBlindsSlider;
    private boolean ledState = false;
    private int sliderState = 0;
    private BluetoothClientConnectionThread connectionThread;
    private final Timer timer = new Timer();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_led_switch);
        initUI();
    }

    private void initUI() {
        ledButton = findViewById(R.id.ledButton);
        rollerBlindsSlider = findViewById((R.id.rollerBlindsSlider));
        ledButton = findViewById(R.id.ledButton);
        rollerBlindsSlider = findViewById((R.id.rollerBlindsSlider));
        ledButton.setOnClickListener((v) -> {
            ledState = !ledState;
            ledButton.setBackgroundColor(ledState? Color.GREEN : Color.RED);
        });
        rollerBlindsSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                sliderState = i;
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
    }

    private void sendMessage() {
        new Thread(() -> {
            try {
                String ledMessage = ledState ? "on" : "off";
                String message = ledMessage + "/" + sliderState + "\n";
                bluetoothOutputStream.write(message.getBytes(StandardCharsets.UTF_8));
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
    }

    @Override
    protected void onStart() {
        super.onStart();
        Intent intent = getIntent();
        BluetoothDevice bluetoothDevice = intent.getParcelableExtra(ScanActivity.X_BLUETOOTH_DEVICE_EXTRA);
        BluetoothAdapter btAdapter = getSystemService(BluetoothManager.class).getAdapter();
        Log.i(C.TAG, "Connecting to " + bluetoothDevice.getName());
        connectionThread = new BluetoothClientConnectionThread(bluetoothDevice, btAdapter, this::manageConnectedSocket);
        connectionThread.start();
    }

    private void manageConnectedSocket(BluetoothSocket socket) {
        try {
            bluetoothOutputStream = socket.getOutputStream();
            Log.i(C.TAG, "Connection successful!");
        } catch (IOException e) {
            Log.e(C.TAG, "Error occurred when creating output stream", e);
        }
        runOnUiThread(() -> {
            rollerBlindsSlider.setEnabled(true);
            ledButton.setEnabled(true);
            ledButton.setBackgroundColor(Color.RED);
            timer.schedule(new TimerTask(){
                @Override
                public void run(){
                    sendMessage();
                }
            },100,500);
        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        timer.cancel();
        connectionThread.cancel();
    }

}