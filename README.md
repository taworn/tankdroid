**tankdroid**
---------

A battle city clone game.

**Tools and Library Usage**

 - Android Studio v.2.2
 - SDL2 v.2.0.4
 - SDL2_image v.2.0.1
 - SDL2_ttf v.2.0.14
 - SDL2_mixer v.2.0.1

**Build**

 - download and unzip SDL2, SDL2_image, SDL2_ttf, SDL2_mixer
 - move downloaded files to tankdroid/app/src/main/jni
 - rename them to SDL, SDL_image, SDL_ttf, SDL_mixer
 - for SDL_mixer, you need a workaround, open SDL_mixer/Android.mk in editor and change "?= true" to ":= false"
 - move current directory to tankdroid/app/src/main/jni
 - run "ndk-build NDK_LIBS_OUT=../jniLibs"
 - open Android Studio and build Java part
 - that's all ^_^

**How to Play**

 - key circle (left) to select menu or shoot
 - key circle (right) to choose menu
 - key cross to move tank
 - if you get shot, your HP decrease
 - if you HP decrease to zero, you die and game over
 - you have to shoot and destroy all eagle(s) to win stage

**Thank**

 - github.com - to host this project
 - google.com - to search every time when you want to learn any new things
 - stackoverflow.com - good community
 - www.sitepoint.com/using-c-and-c-code-in-an-android-app-with-the-ndk/ - for building SDL on Android
 - www.dinomage.com/2013/01/howto-sdl-on-android/ - for building SDL_image and SDL_ttf on Android
 - www.spriters-resource.com - provide graphics, thank to Zephiel87 and Superjustinbros
 - www.sounds-resource.com - provide sounds, thank to J-Sinn
 - and Namco Limited (www.namco.co.jp) to build this original game

---------
Taworn T.

My English is very poor, I'm not English native, sorry. T_T

