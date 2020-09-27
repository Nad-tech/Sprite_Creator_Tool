#pragma once

#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\Banner.h>
#include<vector>

class MenuBar : public Banner {
public:
	bool saveClicked;
	bool loadClicked;
	bool exitClicked;
	Banner menuItems;
	string menuItemsString; 
	//Indexes in menuItemsString
	int load_start;
	int load_end;
	int save_start;
	int save_end;
	int exit_start;
	int exit_end;

	MenuBar() {}

	MenuBar(int x, int y) {
		loadClicked = false;
		saveClicked = false;
		exitClicked = false;
		menuItemsString = "|Load|Save|Exit|";
		load_start = 1;
		load_end = 4;
		save_start = 6;
		save_end = 9;
		exit_start = 11;
		exit_end = 14;
		menuItems = Banner(menuItemsString, SCREEN_WIDTH, 2, x + 0, y + 0);
		menuItems.setRowColour(0, BG_BLUE | FG_WHITE);
		menuItems.setRow(1,0,SCREEN_WIDTH -1, HOR, BG_BLUE | FG_WHITE);
	}

private:
};