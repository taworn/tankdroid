LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_SRC_FILES := \
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
	game\tankhero.cxx \
	game\tank.cxx \
	game\movable.cxx \
	drawnum.cxx \
	menu.cxx \
	animation.cxx \
	sprite.cxx

LOCAL_STATIC_LIBRARIES := SDL2_static

include $(BUILD_SHARED_LIBRARY)
$(call import-module,SDL)LOCAL_PATH := $(call my-dir)
