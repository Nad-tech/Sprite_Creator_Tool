#pragma once

#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\ConsoleWindow.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\FrameBuffer.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\PopUp.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\TextBox.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\Button.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\Banner.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\FileHandler.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\List.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\SaveFrameBufferPopUp.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\LoadFrameBufferPopUp.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\MenuBar.h>

#include<vector>
#include<map>

class GUI {
public:
	ConsoleWindow cWindow;
	
	FileHandler fileHandler;
	
	map<string, FrameBuffer> frameBuffers;
	map<string, Button> buttons;
	map<string, TextBox> textBoxes;
	map<string, PopUp> popUps;
	map<string, Banner> banners;
	map<string, List> lists;
	map<string, SaveFrameBufferPopUp> saveFrameBufferPopUp;
	map<string, LoadFrameBufferPopUp> loadFrameBufferPopUp;
	map<string, MenuBar> menuBar;

	GUI(const char* title, short screenWidth, short screenHeight, int fontW, int fontH) {
		this->title = title;
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->fontWidth = fontW;
		this->fontHeight = fontH;
		cWindow = ConsoleWindow(title, screenWidth, screenHeight, fontW, fontH);
		fileHandler = FileHandler();
		normalButtonColour = BG_GREY | FG_DARK_GREY;
		pressedButtonColour = BG_RED | FG_MAGENTA;
		mouseOverButtonColour = BG_DARK_GREEN | FG_DARK_YELLOW;
		buttonTextColour = BG_GREY | FG_BLACK;
		
		popUps["errorMessage"] = PopUp(
			(screenWidth / 2) - POP_UP_WIDTH / 2 - 1, screenHeight /2 - POP_UP_HEIGHT/2,
			"Default pop up window", "Default pop up window"
		);
	}

	//**Handle frame buffers----------------------------------------------------------------------------------------
	void addPreMadeFrameBuffer(string id, FrameBuffer f) {
		frameBuffers[id] = f;
	}
	
	void addFrameBuffer(string id, int width, int height, int x, int y) {
		frameBuffers[id] = FrameBuffer(width, height, x, y);
	}
	
	void addFrameBufferToWindow(string id, string objectType) {
		//NOTE: Everything you can see and print to the screen is a essentially just a frame 
		//buffer eg. TextBox object is just a specialised FrameBuffer object. That is why we can 
		//use addFrameBufferToWindow() and deleteFrameBufferFromWindow() on all objects.
		
		FrameBuffer *f = nullptr;//Remember everything is a child of a frame buffer object

		//We still need to choose the right object though!
		if (objectType == "frameBuffer") {
			//add error checking 
			f = &frameBuffers[id];
		}
		else if (objectType == "button") {
			//add error checking
			f = &buttons[id];
		}
		else if (objectType == "textBox") {
			if (textBoxes.find(id) == textBoxes.end()) { 
				error("Error: addFrameBufferToWindow()", "textBoxes["+id+"] not found"); 
			}
			f = &textBoxes[id];
		}
		else if (objectType == "popUp") {
			//add error checking
			f = &popUps[id];
		}
		else if (objectType == "banner") {
			//add error checking
			f = &banners[id];
		}
		else if (objectType == "list") {
			if (lists.find(id) == lists.end()) {
				error("Error: addFrameBufferToWindow()", "lists[" + id + "] not found");
			}
			f = &lists[id];
		}
		else if (objectType == "saveFrameBufferPopUp") {
			if (saveFrameBufferPopUp.find(id) == saveFrameBufferPopUp.end()) {
				error("Error: addFrameBufferToWindow()", "saveFrameBufferPopUp[" + id + "] not found");
			}
			f = &saveFrameBufferPopUp[id];
		}
		else if (objectType == "loadFrameBufferPopUp") {
			if (loadFrameBufferPopUp.find(id) == loadFrameBufferPopUp.end()) {
				error("Error: addFrameBufferToWindow()", "loadFrameBufferPopUp[" + id + "] not found");
			}
			f = &loadFrameBufferPopUp[id];
		}
		else {
			error(
				"Error in GUI.h: addFrameBufferToWindow()", 
				"objectType: " + objectType + " not found!"
			);
		}

		if (f != nullptr) {
			int xOffset = f->x;
			int yOffset = f->y;

			//Store what will be behind the frame buffer so we can delete the frame later 
			if (!f->behindFrame_CharactersAndColoursSaved) {
				for (int x = 0; x < f->width; x++) {
					for (int y = 0; y < f->height; y++) {
						int windowIdx = (y + yOffset) * screenWidth + (x + xOffset);
						int fBufferIdx = y * f->width + x;
						f->behindFrame_Characters[fBufferIdx] = cWindow.getCharAt(windowIdx);
						f->behindFrame_Colours[fBufferIdx] = cWindow.getColourAt(windowIdx);
					}
				}
			}

			f->behindFrame_CharactersAndColoursSaved = true;

			//Add the frame buffer to console window buffer
			for (int x = 0; x < f->width; x++) {
				for (int y = 0; y < f->height; y++) {
					int windowIdx = (y + yOffset) * screenWidth + (x + xOffset);
					int fBufferIdx = y * f->width + x;
					cWindow.setPixel(windowIdx, f->characters[fBufferIdx], f->colours[fBufferIdx]);
				}
			}
		}
	}

	void deleteFrameBufferFromWindow(string id, string objectType) {
		//See note in addFrameBufferToWindow()
		FrameBuffer* f = nullptr;
		
		//add error checking
		if (objectType == "frameBuffer") {
			f = &frameBuffers[id];
		}
		else if (objectType == "button") {
			f = &buttons[id];
		}
		else if (objectType == "textBox") {
			f = &textBoxes[id];
		}
		else if (objectType == "popUp") {
			f = &popUps[id];
		}
		else if (objectType == "banner") {
			f = &banners[id];
		}
		else if (objectType == "list") {
			if (lists.find(id) == lists.end()) {
				error("Error: deleteFrameBufferFromWindow()", "lists[" + id + "] not found");
			}
			f = &lists[id];
		}
		else if (objectType == "saveFrameBufferPopUp") {
			if (saveFrameBufferPopUp.find(id) == saveFrameBufferPopUp.end()) {
				error("Error: addFrameBufferToWindow()", "saveFrameBufferPopUp[" + id + "] not found");
			}
			f = &saveFrameBufferPopUp[id];
		}
		else if (objectType == "loadFrameBufferPopUp") {
			if (loadFrameBufferPopUp.find(id) == loadFrameBufferPopUp.end()) {
				error("Error: addFrameBufferToWindow()", "loadFrameBufferPopUp[" + id + "] not found");
			}
			f = &loadFrameBufferPopUp[id];
		}

		if (f != nullptr && f->behindFrame_CharactersAndColoursSaved) {
			int xOffset = f->x;
			int yOffset = f->y;

			for (int x = 0; x < f->width; x++) {
				for (int y = 0; y < f->height; y++) {
					int windowIdx = (y + yOffset) * screenWidth + (x + xOffset);
					int fBufferIdx = y * f->width + x;

					//Restore what was behind the frame buffer
					cWindow.setPixel(
						windowIdx,
						f->behindFrame_Characters[fBufferIdx],
						f->behindFrame_Colours[fBufferIdx]
					);
				}
			}
		}
	}

	void addFrameBuffersToWindow() {
		for (auto i : frameBuffers) {
			addFrameBufferToWindow(i.first, "frameBuffer");
		}

		for (auto i : textBoxes) {
			addFrameBufferToWindow(i.first, "textBox");
		}
	}

	void copyFrameBufferTo(FrameBuffer &source, FrameBuffer &destination) {
		for (int i = 0; i < destination.size; i++) {
			destination.characters[i] = source.characters[i];
			destination.colours[i] = source.colours[i];
		}
	}
	//**END Handle frame buffers----------------------------------------------------------------------------------------


	//**Handle buttons--------------------------------------------------------------------------------------------------
	void addButton(string id, string text, int width, int height, int x, int y) {
		buttons[id] = Button (
			width, height, x, y, text,
			buttonTextColour, normalButtonColour,
			mouseOverButtonColour, pressedButtonColour
		);
	}

	void checkForMouseOverButton(MOUSE_EVENT_RECORD mouseEvent) {
		int mouseX = mouseEvent.dwMousePosition.X;
		int mouseY = mouseEvent.dwMousePosition.Y;
		
		//Is the mouse cursor on a button?
		for (auto &i : buttons) {
			if (i.second.active) {
				if (mouseX >= i.second.x && mouseX < i.second.x + i.second.width &&
					mouseY >= i.second.y && mouseY < i.second.y + i.second.height)
				{
					i.second.changeColour("mouseOver");
					if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						i.second.changeColour("leftButtonPressed");
						buttons[i.first].pressed = true;
					}
				}
				else {
					i.second.changeColour("normal");
				}
			}
		}
	}
	//**END Handle buttons--------------------------------------------------------------------------------------------------


	//**Handle text boxes---------------------------------------------------------------------------------------------------
	void addTextBox(string id, int width, int height, int x, int y) {
		textBoxes[id] = TextBox(width, height, x, y);
	}

	void checkForMouseOverTextBox(MOUSE_EVENT_RECORD mouseEvent) {
		int mouseX = mouseEvent.dwMousePosition.X;
		int mouseY = mouseEvent.dwMousePosition.Y;
		vector<short> result;

		//Is the mouse cursor on the text box?
		for (auto &i : textBoxes) {
			if (i.second.active) {
				if (mouseX >= i.second.x && mouseX < i.second.x + i.second.width &&
					mouseY >= i.second.y && mouseY < i.second.y + i.second.height)
				{
					i.second.changeColour("hover");

					//Text box clicked
					if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						i.second.changeColour("focus");
						textBoxPressed(i.first);
					}
				}
				else {
					i.second.changeColour("notFocus");
				}
			}
		}
	}

	void textBoxPressed(string id) {
		TextBox* t = &textBoxes[id];

		//Set the cursor colour
		t->setCellChrCol(t->cursorPos, t->cursorChar, BG_WHITE | FG_BLACK);
		addFrameBufferToWindow(id, "textBox");
		cWindow.upDateWindow();

		//Handle typing events inside the textbox
		bool quit = false;
		while (!quit) {
			int numEventsRead = cWindow.getInput();

			for (int i = 0; i < numEventsRead; i++) {
				INPUT_RECORD input = cWindow.eventBuffer[i];
				if (input.EventType == KEY_EVENT) {
					//Exit text box
					if (input.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						quit = true;
						free(cWindow.eventBuffer);
						t->setCellChrCol(t->cursorPos, t->cursorChar, BG_BLACK | FG_WHITE);
						addFrameBufferToWindow(id, "textBox");
						cWindow.upDateWindow();
					}
					//User is typing stuff inside text box
					else if (input.Event.KeyEvent.bKeyDown) {
						t->empty = false;
						if (t->cursorPos < t->size) {
							short chr = input.Event.KeyEvent.wVirtualKeyCode;
							if (chr != VK_BACK) {
								if (t->cursorPos < t->size - 1) {
									t->setCellChrCol(t->cursorPos, chr, BG_BLACK | FG_WHITE);
									t->setChar(t->cursorPos, chr);
									t->setCellChrCol(t->cursorPos + 1, t->cursorChar, BG_WHITE | FG_BLACK);
									addFrameBufferToWindow(id, "textBox");
									cWindow.upDateWindow();
									t->cursorPos++;
								}
							}
							//User is pressing backspace and deleting characters
							else {
								if (t->cursorPos > 0) {
									t->setCellChrCol(t->cursorPos, ' ', BG_WHITE | FG_BLACK);
									t->cursorPos--;
									t->setCellChrCol(t->cursorPos, t->cursorChar, BG_WHITE | FG_BLACK);
									addFrameBufferToWindow(id, "textBox");
									cWindow.upDateWindow();
								}
							}
						}
					}
				}
			}
		}
	}
	//**END Handle text boxes---------------------------------------------------------------------------------------------------


	//**Handle banners----------------------------------------------------------------------------------------------------------
	void addBanner(string id, string banner, int width, int height, int x, int y) {
		banners[id] = Banner(banner, width, height, x, y);
	}
	//**END Handle banners----------------------------------------------------------------------------------------------------------

	
	//**Handle lists (create a selectable list)---------------------------------------------------------------------------------------
	void addList(string id, vector<string> list, int x, int y) {
		lists[id] = List(list, x, y);
	}

	void checkForMouseOverList(MOUSE_EVENT_RECORD mouseEvent) {
		int mouseX = mouseEvent.dwMousePosition.X;
		int mouseY = mouseEvent.dwMousePosition.Y;

		//Is the mouse cursor on a list?
		for (auto &i : lists) {
			if (i.second.active) {
				if (mouseX >= i.second.x && mouseX < i.second.x + i.second.width &&
					mouseY >= i.second.y && mouseY < i.second.y + i.second.height)
				{
					//Highlight the row that the mouse pointer is on
					for (int y = i.second.y; y < i.second.height + i.second.y; y++) {
						if (mouseY == y) {
							i.second.setRowColour(y - i.second.y, i.second.mouseOverColour);
							//Item clicked
							if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
								i.second.setRowColour(y - i.second.y, i.second.clickedColour);
								i.second.itemsClicked[y - i.second.y] = true;
							}
						}
						else {
							i.second.setRowColour(y - i.second.y, i.second.normalColour);
						}

					}
				}
				//Mouse pointer not on list, reset all row colours
				else {
					for (int y = 0; y < i.second.height; y++) {
						i.second.setRowColour(y, i.second.normalColour);
					}
				}

				//DEBUG: Show which list items have been clicked (debug, see bottom left of screen)
				/*for (unsigned int a = 0; a < i.second.itemsClicked.size(); a++) {
					char c = ' ';
					if (i.second.itemsClicked[a]) {
						c = '1';
					}
					else {
						c = '0';
					}
					cWindow.setPixel((screenHeight - 1) * screenWidth + a, c, BG_CYAN | FG_MAGENTA);
				}*/
			}
		}
	}
	//**END Handle lists (make a selectable list)---------------------------------------------------------------------------------------

	
	//**Handle save frame buffer pop up-------------------------------------------------------------------------------------------------
	void addSaveFrameBufferPopUp(string id, string title, int x, int y) {
		saveFrameBufferPopUp[id] = SaveFrameBufferPopUp(title, x, y);
		
		SaveFrameBufferPopUp* s = &saveFrameBufferPopUp[id];
		
		addButton(s->saveButtonAttributes.id, 
			s->saveButtonAttributes.title, 
			s->saveButtonAttributes.width,
			s->saveButtonAttributes.height,
			s->saveButtonAttributes.x,
			s->saveButtonAttributes.y);

		addButton(s->cancelButtonAttributes.id,
			s->cancelButtonAttributes.title,
			s->cancelButtonAttributes.width,
			s->cancelButtonAttributes.height,
			s->cancelButtonAttributes.x,
			s->cancelButtonAttributes.y);

		addTextBox(s->textBoxAttributes.id,
			s->textBoxAttributes.width,
			s->textBoxAttributes.height,
			s->textBoxAttributes.x,
			s->textBoxAttributes.y);
	}

	void addSaveFrameBufferPopUpToWindow(string id) {
		string sb_ID = saveFrameBufferPopUp[id].saveButtonAttributes.id;
		string cb_ID = saveFrameBufferPopUp[id].cancelButtonAttributes.id;
		string tb_ID = saveFrameBufferPopUp[id].textBoxAttributes.id;

		addFrameBufferToWindow(id, "saveFrameBufferPopUp");
		addFrameBufferToWindow(sb_ID, "button");
		addFrameBufferToWindow(cb_ID, "button");
		addFrameBufferToWindow(tb_ID, "textBox");
	}
	
	void checkForSavePopUpEvents(string savePopUpId, string loadPopUpId, MOUSE_EVENT_RECORD mouseEvent, FrameBuffer frameBuffer) {
		//Use the save pop up to save a frame buffer to a file using the supplied file name in the textbox

		checkForMouseOverButton(mouseEvent);
		checkForMouseOverTextBox(mouseEvent);
		
		//Save button pressed Save the frame buffer, naming it with the contents of the textbox
		if (buttons[saveFrameBufferPopUp[savePopUpId].saveButtonAttributes.id].pressed == true) {
			string fileName = textBoxes[saveFrameBufferPopUp[savePopUpId].textBoxAttributes.id].getText();
			fileHandler = FileHandler();//See FileHandler class for why I do this here
			
			//Create a new file
			if (fileHandler.saveFrameBuffer(fileName, frameBuffer)) { //create the file see FileHandler class
				error("WARNING", "File name already exists!");
			}
			else {
				//We need to update the load frame buffer so the new file will be on the file list
				//when the load pop up is next activated
				addLoadFrameBufferPopUp(
					loadPopUpId, 
					loadFrameBufferPopUp[loadPopUpId].x, loadFrameBufferPopUp[loadPopUpId].y);
			}
		
			buttons[saveFrameBufferPopUp[savePopUpId].saveButtonAttributes.id].pressed = false;
		}
		//Cancel button pressed disable pop up window and buttons and textbox
		else if (buttons[saveFrameBufferPopUp[savePopUpId].cancelButtonAttributes.id].pressed == true) {
			deleteFrameBufferFromWindow(savePopUpId, "saveFrameBufferPopUp");
			saveFrameBufferPopUp[savePopUpId].active = false;
			buttons[saveFrameBufferPopUp[savePopUpId].saveButtonAttributes.id].active = false;
			buttons[saveFrameBufferPopUp[savePopUpId].cancelButtonAttributes.id].active = false;
			textBoxes[saveFrameBufferPopUp[savePopUpId].textBoxAttributes.id].active = false;
			buttons[saveFrameBufferPopUp[savePopUpId].cancelButtonAttributes.id].pressed = false;
		}
	}
	//**END Handle save frame buffer pop up-------------------------------------------------------------------------------------------------


	//**Handle load frame buffer pop up-----------------------------------------------------------------------------------------------------
	void addLoadFrameBufferPopUp(string id, int x, int y) {
		loadFrameBufferPopUp[id] = LoadFrameBufferPopUp("LOAD", x, y, fileHandler.getFileNames());
		
		LoadFrameBufferPopUp* l = &loadFrameBufferPopUp[id];

		addButton(l->loadButtonAttributes.id,
			l->loadButtonAttributes.title,
			l->loadButtonAttributes.width,
			l->loadButtonAttributes.height,
			l->loadButtonAttributes.x,
			l->loadButtonAttributes.y);

		addButton(l->cancelButtonAttributes.id,
			l->cancelButtonAttributes.title,
			l->cancelButtonAttributes.width,
			l->cancelButtonAttributes.height,
			l->cancelButtonAttributes.x,
			l->cancelButtonAttributes.y);

		addList(l->listAttributes.id, l->filesList.items, l->listAttributes.x, l->listAttributes.y);

		string defaultMessage = "Select a file to load";

		addBanner(
			l->bannerAttributes.id,
			defaultMessage,
			defaultMessage.length(), 1, 
			l->bannerAttributes.x, l->bannerAttributes.y
		);
	}

	void addLoadFrameBufferPopUpToWindow(string id) {
		string lb_ID = loadFrameBufferPopUp[id].loadButtonAttributes.id;
		string cb_ID = loadFrameBufferPopUp[id].cancelButtonAttributes.id;
		string list_ID = loadFrameBufferPopUp[id].listAttributes.id;
		string banner_ID = loadFrameBufferPopUp[id].bannerAttributes.id;
		
		addFrameBufferToWindow(id, "loadFrameBufferPopUp");
		addFrameBufferToWindow(lb_ID, "button");
		addFrameBufferToWindow(cb_ID, "button");
		addFrameBufferToWindow(list_ID, "list");
		addFrameBufferToWindow(banner_ID, "banner");
	}

	void checkForLoadPopUpEvents(string loadPopUpId, MOUSE_EVENT_RECORD mouseEvent, FrameBuffer &frameBuffer) {
		checkForMouseOverButton(mouseEvent);
		checkForMouseOverList(mouseEvent);

		//Make sure list is updated at run time after save is used
		fileHandler = FileHandler();
		lists[loadFrameBufferPopUp[loadPopUpId].listAttributes.id].items = fileHandler.getFileNames();

		//Handle when a file is clicked in the list
		vector<string> fileNames = lists[loadFrameBufferPopUp[loadPopUpId].listAttributes.id].items;
		vector<bool> clickedFile = lists[loadFrameBufferPopUp[loadPopUpId].listAttributes.id].itemsClicked;
		string selectedFile;

		//Store the selected files name, put it in the banner and then unclick it
		for (int i = 0; i < int(fileNames.size()); i++) {
			if (clickedFile[i]) {
				selectedFile = fileNames[i];

				//Put the selected file name into the banner
				int bannerSize = banners[loadFrameBufferPopUp[loadPopUpId].bannerAttributes.id].size;

				for (int i = 0; i < bannerSize; i++) {
					banners[loadFrameBufferPopUp[loadPopUpId].bannerAttributes.id].setCellChrCol(
						i, i < int(selectedFile.length()) ? selectedFile[i] : ' ', BG_WHITE | FG_BLACK
					);
				}

				lists[loadFrameBufferPopUp[loadPopUpId].listAttributes.id].itemsClicked[i] = false;

				break;
			}
		}
		
		//Load button pressed, load the selected frame buffer file whose name is in 
		//the banner into the supplied frame buffer
		if (buttons[loadFrameBufferPopUp[loadPopUpId].loadButtonAttributes.id].pressed == true) {
			//get filename from banner
			vector<short> chars = banners[loadFrameBufferPopUp[loadPopUpId].bannerAttributes.id].characters;
			
			//Get full file name
			string extendedFileName = "";
			for (int i = 0; i < int(chars.size()); i++) {
				extendedFileName += char(chars[i]);
			}
			
			//Load the selected file
			if (extendedFileName != "Select a file to load") {
				//remove the "data/" and ".dan" strings to give to loadFrameBuffer() see FileHandler class
				string shortenedFileName = "";
				int i = 5;
				while (extendedFileName[i] != '.') {
					shortenedFileName += extendedFileName[i];
					i++;
				}

				//DEBUG: Print shortened filename
				/*for (int i = 0; i < shortenedFileName.length(); i++) {
					cWindow.setPixel(300 + i, shortenedFileName[i], BG_BLACK | FG_WHITE);
				}*/

				frameBuffer = loadFrameBuffer(shortenedFileName, frameBuffer.x, frameBuffer.y);
			}
			else { error("WARNING", "No file selected"); }

			buttons[loadFrameBufferPopUp[loadPopUpId].loadButtonAttributes.id].pressed = false;
		}
		//Cancel button pressed disable pop up window and buttons and list and banner
		else if (buttons[loadFrameBufferPopUp[loadPopUpId].cancelButtonAttributes.id].pressed == true) {
			deleteFrameBufferFromWindow(loadPopUpId, "loadFrameBufferPopUp");
			loadFrameBufferPopUp[loadPopUpId].active = false;
			buttons[loadFrameBufferPopUp[loadPopUpId].loadButtonAttributes.id].active = false;
			buttons[loadFrameBufferPopUp[loadPopUpId].cancelButtonAttributes.id].active = false;
			lists[loadFrameBufferPopUp[loadPopUpId].listAttributes.id].active = false;
			banners[loadFrameBufferPopUp[loadPopUpId].bannerAttributes.id].active = false;
			buttons[loadFrameBufferPopUp[loadPopUpId].cancelButtonAttributes.id].pressed = false;
		}
	}
	//**End Handle load frame buffer pop up-------------------------------------------------------------------------------------------------


	//**Handle Menu Bar-------------------------------------------------------------------------------------------------------------------------------
	void addMenuBar(string id, int x, int y) {
		menuBar[id] = MenuBar(x, y);
		banners["menu"] = menuBar[id].menuItems;
	}

	void addMenuBarToWindow(string id) {
		addFrameBufferToWindow("menu", "banner");
	}

	void checkForMouseOverMenuBar(MOUSE_EVENT_RECORD mouseEvent) {
		int mouseX = mouseEvent.dwMousePosition.X;
		int mouseY = mouseEvent.dwMousePosition.Y;
		vector<int> loadPos = { menuBar["menu"].load_start, menuBar["menu"].load_end };
		vector<int> savePos = { menuBar["menu"].save_start, menuBar["menu"].save_end };
		vector<int> exitPos = { menuBar["menu"].exit_start, menuBar["menu"].exit_end };
		vector<int> menuPos = { menuBar["menu"].x, menuBar["menu"].y };

		//Mouse pointer is on load menu button
		if (
			mouseX >= loadPos[0] + menuPos[0] && mouseX <= loadPos[1] + menuPos[0] &&
			mouseY >= menuPos[1] && mouseY < menuPos[1] + 1
		)
		{	
			//Highlight load
			for (int i = loadPos[0]; i <= loadPos[1]; i++) {
				banners["menu"].setColour(i, BG_RED | FG_GREEN);
			}
			
			//Load clicked
			if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				menuBar["menu"].loadClicked = true;
			}
		}
		else {
			for (int i = loadPos[0]; i <= loadPos[1]; i++) {
				banners["menu"].setColour(i, BG_BLUE | FG_WHITE);
			}
		}

		//Mouse pointer is on save menu button
		if (
			mouseX >= savePos[0] + menuPos[0] && mouseX <= savePos[1] + menuPos[0] &&
			mouseY >= menuPos[1] && mouseY < menuPos[1] + 1
			)
		{	
			//Highlight save
			for (int i = savePos[0]; i <= savePos[1]; i++) {
				banners["menu"].setColour(i, BG_RED | FG_GREEN);
			}

			//Save clicked
			if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				menuBar["menu"].saveClicked = true;
			}
		}
		else {
			for (int i = savePos[0]; i <= savePos[1]; i++) {
				banners["menu"].setColour(i, BG_BLUE | FG_WHITE);
			}
		}

		//Mouse pointer is on exit menu button
		if (
			mouseX >= exitPos[0] + menuPos[0] && mouseX <= exitPos[1] + menuPos[0] &&
			mouseY >= menuPos[1] && mouseY < menuPos[1] + 1
			)
		{
			//Highlight exit
			for (int i = exitPos[0]; i <= exitPos[1]; i++) {
				banners["menu"].setColour(i, BG_RED | FG_GREEN);
			}

			//Exit clicked
			if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				menuBar["menu"].exitClicked = true;
			}
		}
		else {
			for (int i = exitPos[0]; i <= exitPos[1]; i++) {
				banners["menu"].setColour(i, BG_BLUE | FG_WHITE);
			}
		}
	}
	//**END Handle Menu Bar-------------------------------------------------------------------------------------------------------------------------------

	
	//**Handle frame buffer files-----------------------------------------------------------------------------------------------------------
	void saveFrameBuffer(string fileName, FrameBuffer frameBuffer) {
		fileHandler.saveFrameBuffer(fileName, frameBuffer);
	}

	FrameBuffer loadFrameBuffer(string fileName, int x, int y) {
		FrameBuffer frameBuffer = FrameBuffer(1, 1, 0, 0);
		fileHandler.loadFrameBuffer(fileName, frameBuffer, x, y);
		return frameBuffer;
	}

	FrameBuffer createFrameBufferUsingFileNameList(int x, int y) {
		vector<string> fileNames = fileHandler.getFileNames();
		int width = 20;
		int height = fileNames.size();
		
		FrameBuffer f = FrameBuffer(width, height, x, y);

		int offSet = 0;
		for (int i = 0; i < int(fileNames.size()); i++) {
			string s = fileNames[i];
			
			for (int j = 0; j < f.width; j++) {
				f.characters[j + offSet] = j < int(s.length()) ? s[j] : ' ';
			}
			
			offSet += f.width;
		}

		return f;
	}
	//**END Handle frame buffer files-----------------------------------------------------------------------------------------------------------


	//**Error pop up box------------------------------------------------------------------------------------------------------------------------
	void error(string title, string message) {
		popUps["errorMessage"].setTitleText(title, BG_RED | FG_WHITE);
		popUps["errorMessage"].setBodyText(message + " **Escape to close**");
		addFrameBufferToWindow("errorMessage", "popUp");
		cWindow.upDateWindow();
		bool close = false;
		while (!close) {
			int numEvents = cWindow.getInput();
			for (int i = 0; i < numEvents; i++) {
				INPUT_RECORD input = cWindow.eventBuffer[i];
				if (
					input.EventType == KEY_EVENT &&
					input.Event.KeyEvent.bKeyDown &&
					input.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE
					)
				{
					close = true;
				}
			}
		}
		
		deleteFrameBufferFromWindow("errorMessage", "popUp");
	}
	//**END Error pop up box------------------------------------------------------------------------------------------------------------------------

private:
	const char* title;
	short screenWidth;
	short screenHeight;
	int fontWidth;
	int fontHeight;

	//button colours
	short normalButtonColour;
	short pressedButtonColour;
	short mouseOverButtonColour;
	short buttonTextColour;
};