#pragma once

#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\globals.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\FrameBuffer.h>

class Palette: public FrameBuffer {
public:
	vector<short> BG_colours;
	
	Palette(int x, int y) : FrameBuffer(2, 8, x, y) {
		BG_colours.push_back(BG_BLACK);
		BG_colours.push_back(BG_DARK_BLUE);
		BG_colours.push_back(BG_DARK_GREEN);
		BG_colours.push_back(BG_DARK_CYAN);
		BG_colours.push_back(BG_DARK_RED);
		BG_colours.push_back(BG_DARK_MAGENTA);
		BG_colours.push_back(BG_DARK_YELLOW);
		BG_colours.push_back(BG_GREY);
		BG_colours.push_back(BG_DARK_GREY);
		BG_colours.push_back(BG_BLUE);
		BG_colours.push_back(BG_GREEN);
		BG_colours.push_back(BG_CYAN);
		BG_colours.push_back(BG_RED);
		BG_colours.push_back(BG_MAGENTA);
		BG_colours.push_back(BG_YELLOW);
		BG_colours.push_back(BG_WHITE);

		for (int i = 0; i < 16; i++) {
			setCellChrCol(i, ' ', BG_colours[i]);
		}	
	}
};

class Swatch : public FrameBuffer {
public:
	
	Swatch(int x, int y) : FrameBuffer(2, 2, x, y) {}
	
	void setChrCol(short ch, short co) {
		for (int i = 0; i < int(colours.size()); i++) {
			this->setCellChrCol(i, ch, co);
		}
	}
};

class ShadePalette : public FrameBuffer {
public:
	vector<short> FG_colours;

	ShadePalette(int x, int y) : FrameBuffer(2, 8, x, y) {
		FG_colours.push_back(FG_BLACK);
		FG_colours.push_back(FG_DARK_BLUE);
		FG_colours.push_back(FG_DARK_GREEN);
		FG_colours.push_back(FG_DARK_CYAN);
		FG_colours.push_back(FG_DARK_RED);
		FG_colours.push_back(FG_DARK_MAGENTA);
		FG_colours.push_back(FG_DARK_YELLOW);
		FG_colours.push_back(FG_GREY);
		FG_colours.push_back(FG_DARK_GREY);
		FG_colours.push_back(FG_BLUE);
		FG_colours.push_back(FG_GREEN);
		FG_colours.push_back(FG_CYAN);
		FG_colours.push_back(FG_RED);
		FG_colours.push_back(FG_MAGENTA);
		FG_colours.push_back(FG_YELLOW);
		FG_colours.push_back(FG_WHITE);

		for (int i = 0; i < 16; i++) {
			setCellChrCol(i, ' ', FG_colours[i]);
		}
	}

	void setChrBGCol(short chr, short BGcol) {
		for (int i = 0; i < 16; i++) {
			setCellChrCol(i, chr, FG_colours[i] | BGcol);
		}
	}
};