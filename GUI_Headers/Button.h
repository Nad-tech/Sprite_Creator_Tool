#pragma once

#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\FrameBuffer.h>

class Button : public FrameBuffer {
public:
	std::string text;
	bool pressed;

	Button() {}

	Button(int width, int height, int x, int y, 
		std::string text, short textColour, 
		short normalColour, short mouseOverColour, short pressedColour) 
		: FrameBuffer(width, height, x, y) 
	{	
		this->normalColour = normalColour;
		this->mouseOverColour = mouseOverColour;
		this->pressedColour = pressedColour;
		this->text = text;
		this->textColour = textColour;
		pressed = false;
		FrameBuffer::setOutSideBorders(1, normalColour);
		for (int i = 1; i < FrameBuffer::width - 1; i++) {
			FrameBuffer::setCellChrCol( 1 * FrameBuffer::width + i, 
										i - 1 < int(text.length()) ? text[i - 1] : ' ', 
										textColour);
		}
	}

	void changeColour(string Event) {
		if (Event == "mouseOver") {
			FrameBuffer::setOutSideBorders(1, mouseOverColour);
		}
		else if (Event == "normal") {
			FrameBuffer::setOutSideBorders(1, normalColour);
		}
		else if (Event == "leftButtonPressed") {
			FrameBuffer::setOutSideBorders(1, normalColour);
		}
	}

private:
	short normalColour;
	short mouseOverColour;
	short pressedColour;
	short textColour;
};