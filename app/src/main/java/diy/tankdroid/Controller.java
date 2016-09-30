package diy.tankdroid;

import android.content.Context;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ImageView;

import org.libsdl.app.SDLActivity;

public class Controller {

    private static final String TAG = "SDL";

    private static final int TIME_PER_CLICK = 100;

    private View controller;
    private Handler handler;
    private Runnable runnable;
    private int keyCode;
    private long lastTime;

    public Controller(Context context) {
        LayoutInflater inflater = LayoutInflater.from(context);
        controller = inflater.inflate(R.layout.controller, null, false);

        ImageView buttonCross = (ImageView) controller.findViewById(R.id.button_cross);
        ImageButton buttonSelect = (ImageButton) controller.findViewById(R.id.button_select);
        ImageButton buttonSpace = (ImageButton) controller.findViewById(R.id.button_space);

        buttonCross.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent event) {
                if (System.currentTimeMillis() - lastTime < TIME_PER_CLICK * 2)
                    return true;

                float w = view.getWidth() / 2;
                float h = view.getHeight() / 2;
                float x = event.getX() - w;
                float y = event.getY() - h;
                float ratio = y / x;
                Log.d(TAG, String.format("(x, y) ratio = (%2f, %2f) %2f", x, y, ratio));

                if (Math.abs(ratio) > 1) {
                    // up or down
                    if (y < 0) {
                        SDLActivity.onNativeKeyDown(19);  // up key
                        keyCode = 19;
                    }
                    else {
                        SDLActivity.onNativeKeyDown(20);  // down key
                        keyCode = 20;
                    }
                }
                else {
                    // left or right
                    if (x < 0) {
                        SDLActivity.onNativeKeyDown(21);  // left key
                        keyCode = 21;
                    }
                    else {
                        SDLActivity.onNativeKeyDown(22);  // right key
                        keyCode = 22;
                    }
                }

                handler.removeCallbacks(runnable);
                int action = event.getAction();
                if (action == MotionEvent.ACTION_MOVE) {
                    handler.postDelayed(runnable, TIME_PER_CLICK * 2);
                }

                return true;
            }
        });
        buttonSelect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SDLActivity.onNativeKeyDown(61);  // TAB key
            }
        });
        buttonSpace.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SDLActivity.onNativeKeyDown(66);  // ENTER key
            }
        });

        handler = new Handler();
        runnable = new Runnable() {
            @Override
            public void run() {
                SDLActivity.onNativeKeyDown(keyCode);
                handler.postDelayed(runnable, TIME_PER_CLICK);
            }
        };
        keyCode = 0;
        lastTime = System.currentTimeMillis();
    }

    public void attach(ViewGroup layout) {
        layout.addView(controller);
    }

}
