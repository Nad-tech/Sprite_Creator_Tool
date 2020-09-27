#pragma once

#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\FrameBuffer.h>

class PopUp : public FrameBuffer {
public:
	PopUp() {}
	
	PopUp(int x, int y, string title, string bodyContents) : FrameBuffer(POP_UP_WIDTH, POP_UP_HEIGHT, x, y) {
		this->x = x;
		this->y = y;
		this->title = title;
		this->bodyText = bodyContents;
		
		FrameBuffer::setOutSideBorders(0, BG_DARK_GREY | FG_GREY);
		
		setTitleText(title, BG_BLUE | FG_WHITE);

		//separator
		FrameBuffer::setRow(2, 1, POP_UP_WIDTH - 2, D_HOR, BG_DARK_GREY | FG_GREY);

		setBodyText(bodyText);
	}

	void setTitleText(string title, short colour) {	
		for (int i = 1; i < POP_UP_WIDTH - 1; i++) {
			FrameBuffer::setCellChrCol(1 * FrameBuffer::width + i,
				i - 1 < int(title.length()) ? title[i - 1] : ' ', colour);
		}
	}

	void setBodyText(string bodyText) {
		for (int i = 1; i < FrameBuffer::width - 1; i++) {
			for (int j = 3; j < FrameBuffer::height - 1; j++) {
				int k = (j - 3) * FrameBuffer::width + (i - 1);
				bool b = k <= int(bodyText.length());
				FrameBuffer::setCellChrCol(i, j, b ? bodyText[k] : ' ', BG_WHITE);
			}
		}
	}

private:
	int x, y;
	string title;
	string bodyText;
};