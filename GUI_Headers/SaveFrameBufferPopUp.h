#pragma once

struct SavePopUpObjectAttributes {
	string title;
	string id;
	int width;
	int height;
	int x;
	int y;
};

class SaveFrameBufferPopUp : public FrameBuffer{
public:
	SavePopUpObjectAttributes saveButtonAttributes;
	SavePopUpObjectAttributes cancelButtonAttributes;
	SavePopUpObjectAttributes textBoxAttributes;

	SaveFrameBufferPopUp() {}

	SaveFrameBufferPopUp(string title, int x, int y) : FrameBuffer(0, 0, x, y) {
		//Set frame buffer attributes not set in constructor call
		this->width = SAVE_FRAME_BUFFER_WIDTH;
		this->height = SAVE_FRAME_BUFFER_HEIGHT;
		this->size = this->width * this->height;
		this->colours = vector<short>(this->width * this->height);
		this->characters = vector<short>(this->width * this->height);
		this->behindFrame_Characters = vector<short>(this->width * this->height);
		this->behindFrame_Colours = vector<short>(this->width * this->height);
		
		//Set save pop up contents
		normalColour = BG_GREY | FG_DARK_GREY;
		textColour = BG_BLUE | FG_WHITE;

		this->setOutSideBorders(1, normalColour);
		for (int i = 1; i < this->width - 1; i++) {
			this->setCellChrCol(i + this->width, i - 1 < int(title.length()) ? title[i - 1] : ' ', textColour);
		}
		this->setRow(2, 1, this->width - 2, HOR, normalColour);

		saveButtonAttributes.title = "Save";
		saveButtonAttributes.id = "savePopUp_save";
		saveButtonAttributes.width = 6;
		saveButtonAttributes.height = 3;
		saveButtonAttributes.x = this->x + 2;
		saveButtonAttributes.y = this->y + 6;

		cancelButtonAttributes.title = "Close";
		cancelButtonAttributes.id = "savePopUp_close";
		cancelButtonAttributes.width = 7;
		cancelButtonAttributes.height = 3;
		cancelButtonAttributes.x = this->x + 9;
		cancelButtonAttributes.y = this->y + 6;

		textBoxAttributes.title = "";
		textBoxAttributes.id = "savePopUp_textBox";
		textBoxAttributes.width = 15;
		textBoxAttributes.height = 1;
		textBoxAttributes.x = this->x + 2;
		textBoxAttributes.y = this->y + 4;
	}

private:
	string title;
	short normalColour;
	short textColour;
};