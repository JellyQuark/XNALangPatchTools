#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include <cstdlib>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
using namespace std;

struct image {
	unsigned char* data;
	int W, H, N;
	image(int w = 0, int h = 0, int n = 0) {
		data = (unsigned char*)malloc(w * h * n);
		memset(data, 0, sizeof(unsigned char) * w * h * n);
		W = w, H = h, N = n;
	}
	image(const char* path) {
		data = stbi_load(path, &W, &H, &N, 0);
	}
	void read_file(const char* path) {
		free(data);
		data = stbi_load(path, &W, &H, &N, 0);
	}
	void write_file(const char* path) {
		stbi_write_png(path, W, H, N, data, 0);
	}
	image subimage(int x, int y, int w, int h) {
		image ret(w, h, N);
		for (int i = 0; i < h; i++)
			memcpy(ret.data + i * w * N, data + (i + x) * W * N + y * N, sizeof(unsigned char) * w * N);
		return ret;
	}
	void replace(int x, int y, const image& from) {
		for (int i = 0; i < from.H; i++)
			memcpy(data + (i + x) * W * N + y * N, from.data + i * from.W * N, sizeof(unsigned char) * from.W * N);
	}
};

struct fontelem {
	image img;
	wchar_t c;
	fontelem() {}
	struct rec {
		int x, y, width, height;
		rec(int a = 0, int b = 0, int c = 0, int d = 0) {
			x = a, y = b, width = c, height = d;
		}
	}glyphs, cropping;
	struct vec {
		int x, y, z;
		vec(int a = 0, int b = 0, int c = 0) {
			x = a, y = b, z = c;
		}
	}kerning;
};

void read_xnbfont(map<wchar_t, fontelem>& mp, const char* imgpath, const wchar_t* yamlpath) {
	image img(imgpath);
	FILE* wfin = _wfopen(yamlpath, L"r,ccs=UTF-8");
	wchar_t wstr[201];
	vector<fontelem> elems;
	do fgetws(wstr, 200, wfin); while (!wcsstr(wstr, L"glyphs"));
	while (fgetws(wstr, 200, wfin), *wstr != L'\n') {
		int x, y, w, h;
		fgetws(wstr, 200, wfin);
		x = _wtoi(wstr + 15);
		fgetws(wstr, 200, wfin);
		y = _wtoi(wstr + 15);
		fgetws(wstr, 200, wfin);
		w = _wtoi(wstr + 19);
		fgetws(wstr, 200, wfin);
		h = _wtoi(wstr + 20);
		fgetws(wstr, 200, wfin);
		elems.push_back(fontelem());
		elems.back().img = img.subimage(y, x, w, h);
	}
	free(img.data);
	do fgetws(wstr, 200, wfin); while (!wcsstr(wstr, L"cropping"));
	for (fontelem& e : elems) {
		fgetws(wstr, 200, wfin);
		fgetws(wstr, 200, wfin);
		e.cropping.x = _wtoi(wstr + 15);
		fgetws(wstr, 200, wfin);
		e.cropping.y = _wtoi(wstr + 15);
		fgetws(wstr, 200, wfin);
		e.cropping.width = _wtoi(wstr + 19);
		fgetws(wstr, 200, wfin);
		e.cropping.height = _wtoi(wstr + 20);
		fgetws(wstr, 200, wfin);
	}
	do fgetws(wstr, 200, wfin); while (!wcsstr(wstr, L"characterMap"));
	for (fontelem& e : elems) {
		fgetws(wstr, 200, wfin);
		e.c = wstr[11] == L'\\' ? wstr[12] : wstr[11];
	}
	do fgetws(wstr, 200, wfin); while (!wcsstr(wstr, L"kerning"));
	for (fontelem& e : elems) {
		fgetws(wstr, 200, wfin);
		fgetws(wstr, 200, wfin);
		e.kerning.x = _wtoi(wstr + 15);
		fgetws(wstr, 200, wfin);
		e.kerning.y = _wtoi(wstr + 15);
		fgetws(wstr, 200, wfin);
		e.kerning.z = _wtoi(wstr + 15);
		fgetws(wstr, 200, wfin);
	} 
	fclose(wfin);
	for (fontelem& e : elems)
		mp[e.c] = e;
}

const int LEN = 2048;
int test_write(map<wchar_t, fontelem>& mp) {
	int x = 0, y = 0, maxh = 0, n = 4;
}

void write_xnbfont(map<wchar_t, fontelem>& mp, const char* imgpath, const wchar_t* yamlpath) {
	int x = 0, y = 0, maxh = 0, n = 4;
	image img(LEN, LEN, n);
	FILE* wfout = _wfopen(yamlpath, L"w,ccs=UTF-8");
	fputws(L"\nxnbData: \n    target: \"w\"\n    compressed: true\n    hiDef: false\n    readerData: \n        - \n            type: \"Microsoft.Xna.Framework.Content.SpriteFontReader, Microsoft.Xna.Framework.Graphics, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553\"\n            version: 0\n\n        - \n            type: \"Microsoft.Xna.Framework.Content.Texture2DReader, Microsoft.Xna.Framework.Graphics, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553\"\n            version: 0\n\n        - \n            type: \"Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Rectangle, Microsoft.Xna.Framework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553]]\"\n            version: 0\n\n        - \n            type: \"Microsoft.Xna.Framework.Content.RectangleReader\"\n            version: 0\n\n        - \n            type: \"Microsoft.Xna.Framework.Content.ListReader`1[[System.Char, mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089]]\"\n            version: 0\n\n        - \n            type: \"Microsoft.Xna.Framework.Content.CharReader\"\n            version: 0\n\n        - \n            type: \"Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector3, Microsoft.Xna.Framework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=842cf8be1de50553]]\"\n            version: 0\n\n        - \n            type: \"Microsoft.Xna.Framework.Content.Vector3Reader\"\n            version: 0\n\n\n    numSharedResources: 0\n\ncontent:  #!SpriteFont\n    texture:  #!Texture2D\n        format: 0\n\n    glyphs:  #!List<Rectangle>\n", wfout);
	for (auto duo : mp) {
		fontelem& e = duo.second;
		if (y + e.img.W > LEN) {
			x += maxh;
			maxh = 0;
			y = 0;
		}
		img.replace(x, y, e.img);
		fwprintf(wfout, L"        -  #!Rectangle\n            x: %d\n            y: %d\n            width: %d\n            height: %d\n\n",
			y, x, e.img.W, e.img.H);
		y += e.img.W;
		if (e.img.H > maxh) maxh = e.img.H;
	}
	img.write_file(imgpath);
	free(img.data);
	fputws(L"\n    cropping:  #!List<Rectangle>\n", wfout);
	for (auto duo : mp) {
		fontelem& e = duo.second;
		fwprintf(wfout, L"        -  #!Rectangle\n            x: %d\n            y: %d\n            width: %d\n            height: %d\n\n",
			e.cropping.x, e.cropping.y, e.cropping.width, e.cropping.height);
	}
	fputws(L"\n    characterMap:  #!List<Char>\n", wfout);
	for (auto duo : mp) {
		const wchar_t& wc = duo.first;
		fputws(L"        - \"", wfout);
		if (wc == L'\"' || wc == L'\\') fputwc(L'\\', wfout);
		fputwc(wc, wfout);
		fputws(L"\" #!Char\n", wfout);
	}
	fputws(L"\n    verticalSpacing: 20\n    horizontalSpacing: 0\n    kerning:  #!List<Vector3>\n", wfout);
	for (auto duo : mp) {
		fontelem& e = duo.second;
		fwprintf(wfout, L"        -  #!Vector3\n            x: %d\n            y: %d\n            z: %d\n\n",
			e.kerning.x, e.kerning.y, e.kerning.z);
	}
	fputws(L"\n    defaultCharacter:  #!Nullable<Char>\n        data: null\n\n\nextractedImages: \n    - \n        path: \"texture\"\n\n", wfout);
	fclose(wfout);
}

map<wchar_t, fontelem> font;

int main() {

// read_xnbfont(font, "0.texture.png", L"0.yaml");
//	read_xnbfont(font, "1.texture.png", L"1.yaml");
//	read_xnbfont(font, "2.texture.png", L"2.yaml");
//	read_xnbfont(font, "3.texture.png", L"3.yaml");
//	read_xnbfont(font, "4.texture.png", L"4.yaml");
//	read_xnbfont(font, "5.texture.png", L"5.yaml");
//	read_xnbfont(font, "6.texture.png", L"6.yaml");
//	read_xnbfont(font, "7.texture.png", L"7.yaml");
//	read_xnbfont(font, "8.texture.png", L"8.yaml");
//	read_xnbfont(font, "9.texture.png", L"9.yaml");
//	read_xnbfont(font, "10.texture.png", L"10.yaml");
//	for(auto &duo : font){
//		fontelem &e = duo.second;
//		free(e.img.data);
//		e.img = image(0, 0, 4);
//	}
/*
	for(auto &duo : font){
		fontelem &e = duo.second;
		image img(e.img.W + 2, e.img.H + 2, e.img.N);
		img.replace(0, 0, e.img);
		free(e.img.data);
		for(int i = 0; i < e.img.H; i++)
			for(int j = 0; j < e.img.W; j++){
				int p = i * img.W * img.N + j * img.N;
				if(img.data[p] == 255 && img.data[p + 3] == 255){
					img.data[p + img.N + 3] = 255;
					img.data[p + 2 * img.N + 3] = 255;
					img.data[p + img.W * img.N + 3] = 255;
					img.data[p + img.W * img.N + img.N + 3] = 255;
					img.data[p + img.W * img.N + 2 * img.N + 3] = 255;
					img.data[p + 2 * img.W * img.N + 3] = 255;
					img.data[p + 2 * img.W * img.N + img.N + 3] = 255;
					img.data[p + 2 * img.W * img.N + 2 * img.N + 3] = 255;
				}
			}
		e.img = img;
	}
	for(auto &duo : font){
		fontelem &e = duo.second;
		e.kerning.x = 0;
		e.kerning.y = e.img.W + 4;
		e.kerning.z = 0;
		e.cropping.x = 2;
		e.cropping.y = 22 - e.img.H;
		e.cropping.width = e.img.W + 4;
		e.cropping.height = 24;
	}
	for(auto &duo : font){
		fontelem &e = duo.second;
		image img(e.img.W + 2, e.img.H + 2, e.img.N);
		img.replace(2, 2, e.img);
		free(e.img.data);
		for(int i = 2; i < e.img.H; i++)
			for(int j = 2; j < e.img.W; j++){
				int p = i * img.W * img.N + j * img.N;
				if(img.data[p] == 255 && img.data[p + 3] == 255){
					img.data[p - img.N + 3] = 255;
					img.data[p - 2 * img.N + 3] = 255;
					img.data[p - img.N + img.W * img.N + 3] = 255;
					img.data[p - 2 * img.N + img.W * img.N + 3] = 255;
					img.data[p - img.N + 2 * img.W * img.N + 3] = 255;
					img.data[p - 2 * img.N + 2 * img.W * img.N + 3] = 255;
					img.data[p - img.W * img.N + 3] = 255;
					img.data[p - 2 * img.W * img.N + 3] = 255;
					img.data[p - img.W * img.N + img.N + 3] = 255;
					img.data[p - 2 * img.W * img.N + img.N + 3] = 255;
					img.data[p - img.W * img.N + 2 * img.N + 3] = 255;
					img.data[p - 2 * img.W * img.N + 2 * img.N + 3] = 255;
				}
			}
		e.img = img;
		e.cropping.x -= 2;
		e.cropping.y -= 2;
	}
*/
	read_xnbfont(font, "Atari ST 8x16 System Font 24px.texture.png", L"Atari ST 8x16 System Font 24px.yaml");
	read_xnbfont(font, "Ori_Atari ST 8x16 System Font 24px.texture.png", L"Ori_Atari ST 8x16 System Font 24px.yaml");
	write_xnbfont(font, "out.texture.png", L"out.yaml");
	return 0;
}
