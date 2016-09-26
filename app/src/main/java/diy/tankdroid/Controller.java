package diy.tankdroid;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import org.libsdl.app.SDLActivity;

public class Controller {

    private static final String TAG = "SDL";

    private View controller;

    public Controller(Context context) {
        LayoutInflater inflater = LayoutInflater.from(context);
        controller = inflater.inflate(R.layout.controller, null, false);

        Button buttonEsc = (Button) controller.findViewById(R.id.button_esc);
        Button buttonLeft = (Button) controller.findViewById(R.id.button_left);
        Button buttonUp = (Button) controller.findViewById(R.id.button_up);
        Button buttonDown = (Button) controller.findViewById(R.id.button_down);
        Button buttonRight = (Button) controller.findViewById(R.id.button_right);
        Button buttonEnter = (Button) controller.findViewById(R.id.button_enter);

        buttonEsc.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d(TAG, "ESC click");
                SDLActivity.onNativeKeyDown(111);
            }
        });
        buttonLeft.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d(TAG, "left click");
                //SDLActivity.onNativeKeyDown(21);
                SDLActivity.onNativeKeyDown(40);
            }
        });
        buttonUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d(TAG, "up click");
                SDLActivity.onNativeKeyDown(19);
            }
        });
        buttonDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d(TAG, "down click");
                SDLActivity.onNativeKeyDown(20);
            }
        });
        buttonRight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d(TAG, "right click");
                //SDLActivity.onNativeKeyDown(22);
                SDLActivity.onNativeKeyDown(51);
            }
        });
        buttonEnter.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d(TAG, "ENTER click");
                SDLActivity.onNativeKeyDown(66);
            }
        });
    }

    public void attach(ViewGroup layout) {
        layout.addView(controller);
    }

}
