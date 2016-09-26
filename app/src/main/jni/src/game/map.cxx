/**
 * @file map.cxx
 * @desc Map module.
 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../game.hxx"
#include "common.hxx"
#include <stdio.h>

Map::~Map()
{
	if (imageData)
		delete[] imageData;
	if (mapData)
		delete[] mapData;
}

Map::Map()
	: width(), height()
	, mapData(), imageData()
{
}

bool Map::load(const char *fileName)
{
	// opens file
	FILE *fp = fopen(fileName, "r");
	if (!fp) {
		SDL_Log("Cannot open file.");
		return false;
	}

	// checks file header, 8 bytes
	char check[8] = { 0x43, 0x41, 0x50, 0x4D, 0x41, 0x50, 0x00, 0x00, };
	char header[8] = { 0 };
	if (fread(header, 1, 8, fp) != 8) {
		SDL_Log("Header is not acceptable.");
		fclose(fp);
		return false;
	}
	for (int i = 0; i < 8; i++) {
		if (header[i] != check[i]) {
			SDL_Log("Header is not acceptable.");
			fclose(fp);
			return false;
		}
	}

	// reads header information
	// 6 int = width, height, divo start width, divo start height, pacman start width, pacman start height
	int32_t buffer[6] = { 0 };
	if (fread(buffer, 1, 6 * sizeof(int32_t), fp) != 6 * sizeof(int32_t)) {
		SDL_Log("Not enough data to read.");
		fclose(fp);
		return false;
	}
	int w = buffer[0];
	int h = buffer[1];
	int size = w * h;

	// reads map data
	char *mapData = new char[size];
	if (fread(mapData, 1, size * sizeof(char), fp) != size * sizeof(char)) {
		SDL_Log("Not enough data to read.");
		fclose(fp);
		return false;
	}

	// reads image data
	int *imageData = new int[size];
	for (int i = 0; i < size; i++) {
		int32_t image;
		if (fread(&image, 1, sizeof(int32_t), fp) != sizeof(int32_t)) {
			SDL_Log("Not enough data to read.");
			fclose(fp);
			return false;
		}
		imageData[i] = (int)image;
	}

	// closes file
	fclose(fp);

	// copying
	this->width = w;
	this->height = h;
	if (this->mapData)
		delete[] this->mapData;
	if (this->imageData)
		delete[] this->imageData;
	this->mapData = mapData;
	this->imageData = imageData;

	return true;
}

