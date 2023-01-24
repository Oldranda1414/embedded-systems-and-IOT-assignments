package com.example.remoteblinkapp;

import androidx.appcompat.app.AppCompatActivity;

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
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Timer;
import java.util.TimerTask;

public class LedSwitchEmulatedActivity extends AppCompatActivity {

    private Button ledButton;
    private SeekBar rollerBlindsSlider;
    private boolean ledState;
    private int sliderState;
    private OutputStream emulatedBluetoothOutputStream;
    private EmulatedClientConnectionThread connectionThread;
    //private Timer timer = new Timer();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_led_switch);
        initUI();
    }

    private void initUI() {
        ledButton = findViewById(R.id.ledButton);
        rollerBlindsSlider = findViewById((R.id.rollerBlindsSlider));
        ledButton.setOnClickListener((v) -> {
            ledState = !ledState;
            ledButton.setBackgroundColor(ledState? Color.GREEN : Color.RED);
            sendMessage();
        });
        rollerBlindsSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                sliderState = i;
                sendMessage();
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
                String ledMessage = ledState ? "ON" : "OFF";
                String message = ledMessage + "/" + sliderState + "\n";
                emulatedBluetoothOutputStream.write(message.getBytes(StandardCharsets.UTF_8));
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
    }

    @Override
    protected void onStart() {
        super.onStart();
        connectionThread = new EmulatedClientConnectionThread(this::manageConnectedSocket);
        connectionThread.start();
        /*timer.scheduleAtFixedRate(new TimerTask(){
            @Override
            public void run(){
                sendMessage();
            }
        },0,500);*/
    }

    private void manageConnectedSocket(Socket socket) {
        try {
            emulatedBluetoothOutputStream = socket.getOutputStream();
            Log.i(C.TAG, "Connection successful!");
        } catch (IOException e) {
            Log.e(C.TAG, "Error occurred when creating output stream", e);
        }
        runOnUiThread(() -> {
            ledButton.setEnabled(true);
            rollerBlindsSlider.setEnabled(true);
            ledButton.setBackgroundColor(Color.RED);
        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        //timer.cancel();
        connectionThread.cancel();
    }
}