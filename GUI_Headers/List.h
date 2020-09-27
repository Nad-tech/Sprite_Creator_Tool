#pragma once

#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\FrameBuffer.h>
#include<vector>

class List : public FrameBuffer {
public:
	vector<string> items;
	vector<bool> itemsClicked;
	short normalColour;
	short mouseOverColour;
	short clickedColour;
	int maxItemLength;

	List() {}

	List(vector<string> items, int x, int y) : FrameBuffer(0, 0, x, y) {
		normalColour = BG_BLACK | FG_WHITE;
		mouseOverColour = BG_WHITE | FG_BLACK;
		clickedColour = BG_YELLOW | FG_GREEN;
	
		this->items = items;
		
		//Find the longest string to get appropriate frame buffer width
		maxItemLength = 0;
		for (int i = 0; i < int(items.size()); i++) {
			if (int(items[i].length()) > maxItemLength) maxItemLength = items[i].length();
		}
		
		//Set this frame buffer attributes, cant be set correctly in constructor call
		//because at that stage width and height coud not be determined
		this->width = maxItemLength;
		this->height = items.size();//Height is simply the number of list items
		this->size = width * height;
		this->colours = vector<short>(this->width * this->height);
		this->characters = vector<short>(this->width * this->height);
		this->behindFrame_Characters = vector<short>(this->width * this->height);
		this->behindFrame_Colours = vector<short>(this->width * this->height);

		//Add list items characters and colours to frame buffer 
		int offset = 0;
		for (auto item : items) {
			for (int i = 0; i < this->width; i++) {
				this->setCellChrCol(
					i + offset, i < int(item.length()) ? item[i] : ' ',
					normalColour
				);
			}
			offset += this->width;
		}

		itemsClicked = vector<bool>(this->height);
	}

private:
	
};