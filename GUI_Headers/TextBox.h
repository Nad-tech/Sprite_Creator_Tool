#pragma once

#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\FrameBuffer.h>
#include<string>

class TextBox : public FrameBuffer {
public:
	int cursorPos;
	short cursorChar;
	short cursorColour;
	bool empty;

	TextBox() {}

	TextBox(int width, int height, int x, int y) : FrameBuffer(width, height, x, y) {
		empty = true;
		focusColour = BG_WHITE | FG_BLACK;
		hoverColour = BG_WHITE | FG_BLACK;
		notFocusColour = BG_WHITE | FG_BLACK;
		cursorPos = 0;
		cursorChar = 0x25CA;
		cursorColour = BG_BLACK | FG_WHITE;
		changeColour("notFocus");
		FrameBuffer::setCellChrCol(cursorPos, cursorChar, cursorColour);
	}

	void changeColour(string status) {
		if (status == "focus" && empty) {
			setAll(' ', focusColour);
			FrameBuffer::setCellChrCol(cursorPos, cursorChar, BG_WHITE | FG_BLACK);
		}
		else if (status == "notFocus" && empty) {
			setAll(THREEQUARTERS, notFocusColour);
			FrameBuffer::setCellChrCol(cursorPos, cursorChar, cursorColour);
		}
		else if (status == "hover" && empty) {
			setAll(HALF, hoverColour);
			FrameBuffer::setCellChrCol(cursorPos, cursorChar, cursorColour);
		}
	}

	string getText() {
		string contents = "";
		//Get the test up to the cursor char
		for (auto i : characters) {
			if (i != 0x25CA) {
				contents += char(i);
			}
			else {
				break;
			}
		}
		return contents;
	}

private:
	short notFocusColour;
	short focusColour;
	short hoverColour;
};