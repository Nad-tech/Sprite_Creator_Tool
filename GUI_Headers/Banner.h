#pragma once

#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\FrameBuffer.h>

class Banner : public FrameBuffer {
public:
	Banner() {}

	Banner(std::string banner, int  width, int height, int x, int y) : FrameBuffer(width, height, x, y) {
		for (int i = 0; i < FrameBuffer::size; i++) {
			FrameBuffer::setCellChrCol(i, i < int(banner.length()) ? banner[i] : ' ', BG_WHITE | FG_DARK_GREEN);
		}
	}

	void setText(string text) {
		for (int i = 0; i < width; i++) {
			characters[i] = i < int(text.length()) ? text[i] : ' ';
		}
	}
private:
};