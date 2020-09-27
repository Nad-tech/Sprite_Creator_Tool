#pragma once

/*
A save frame buffer pop up has:
- save and cancel buttons attributes
- text field attributes to name file
- file handler to create the file with the name from the text field
*/

struct LoadPopUpObjectAttributes {
	string title;
	string id;
	int width;
	int height;
	int x;
	int y;
};

class LoadFrameBufferPopUp : public FrameBuffer {
public:
	LoadPopUpObjectAttributes loadButtonAttributes;
	LoadPopUpObjectAttributes cancelButtonAttributes;
	LoadPopUpObjectAttributes listAttributes;
	LoadPopUpObjectAttributes bannerAttributes;
	List filesList;
	Banner banner;

	LoadFrameBufferPopUp() {}

	LoadFrameBufferPopUp(string title, int x, int y, vector<string> fileNames) : FrameBuffer(0, 0, x, y) {
		filesList = List(fileNames, x + 2, y + 2);
		
		//Set frame buffer attributes not set in constructor call
		this->width = 27;
		this->height = fileNames.size() + 15;
		this->size = this->width * this->height;
		this->colours = vector<short>(this->width * this->height);
		this->characters = vector<short>(this->width * this->height);
		this->behindFrame_Characters = vector<short>(this->width * this->height);
		this->behindFrame_Colours = vector<short>(this->width * this->height);

		//Set load pop up contents
		normalColour = BG_GREY | FG_DARK_GREY;
		textColour = BG_BLUE | FG_WHITE;

		this->setOutSideBorders(1, normalColour);
		for (int i = 1; i < this->width - 1; i++) {
			this->setCellChrCol(i + this->width, i - 1 < int(title.length()) ? title[i - 1] : ' ', textColour);
		}
		this->setRow(2, 1, this->width - 2, HOR, normalColour);

		loadButtonAttributes.title = "Load";
		loadButtonAttributes.id = "loadPopUp_load";
		loadButtonAttributes.width = 6;
		loadButtonAttributes.height = 3;
		loadButtonAttributes.x = this->x + 2;
		loadButtonAttributes.y = this->y + this->height - 5;
		
		cancelButtonAttributes.title = "Close";
		cancelButtonAttributes.id = "loadPopUp_exit";
		cancelButtonAttributes.width = 7;
		cancelButtonAttributes.height = 3;
		cancelButtonAttributes.x = this->x + 9;
		cancelButtonAttributes.y = this->y + this->height - 5;

		listAttributes.title = "";
		listAttributes.id = "loadPopUp_list";
		listAttributes.width = 0;
		listAttributes.height = 0;
		listAttributes.x = this->x + 2;
		listAttributes.y = this->y + 4;

		bannerAttributes.title = "";
		bannerAttributes.id = "loadPopUp_banner";
		bannerAttributes.width = 0;
		bannerAttributes.height = 0;
		bannerAttributes.x = this->x + 2;
		bannerAttributes.y = this->y + this->height - 7;
	}

private:
	string title;
	short normalColour;
	short textColour;
};