LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/../SDL_image $(LOCAL_PATH)/../SDL_ttf

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	main.cxx \
	game.cxx \
	scenes\scene_win.cxx \
	scenes\scene_lost.cxx \
	scenes\scene_play.cxx \
	scenes\scene_stage.cxx \
	scenes\scene_title.cxx \
	scenes\scene.cxx \
	game\arena.cxx \
	game\map.cxx \
	game\item.cxx \
	game\bullet.cxx \
	game\tankhero.cxx \
	game\tank.cxx \
	game\movable.cxx \
	drawnum.cxx \
	menu.cxx \
	animation.cxx \
	sprite.cxx

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_ttf

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
