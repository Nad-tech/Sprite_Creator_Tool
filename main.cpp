#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\GUI.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\GUI_Headers\globals.h>
#include<\Users\dexte\source\repos\new projects\Sprite_Creation_Tool\Sprite_Creation_Tool\Palette.h>
#include<Windows.h>
#include<iostream>


//Creates a start page frame buffer for start up
void addStartPage(GUI &gui) {
	gui.addFrameBuffer("startUp", 31, 31, SCREEN_WIDTH / 2 - 15, SCREEN_HEIGHT / 2 - 18);
	gui.frameBuffers["startUp"].setAll(' ', BG_WHITE | FG_WHITE);
	gui.frameBuffers["startUp"].setOutSideBorders(0, BG_BLUE | FG_WHITE);

	//Program usage instructions
	string s = "";
	s += "     Dans Sprite Creator     ";
	s += "                             ";
	s += "        Instructions:        ";
	s += "                             ";
	s += "* Press A to enter draw mode ";
	s += "* Press C to enter copy mode ";
	s += "* Press K to enter paste mode";
	s += "* Press P to paste           ";
	s += "                             ";
	s += "                             ";
	s += "                             ";
	s += "     Choose canvas size:     ";

	gui.addBanner("startUp", s, 29, 12, SCREEN_WIDTH / 2 - 14, SCREEN_HEIGHT / 2 - 16);
}

//Creates banners and buttons to modify width and height of the canvas for start up window
vector<int> getCanvasWidthHeight (GUI &gui) {
	vector<int> widthHeight(2);

	//Get the start up window dimensions
	int startWindowX = gui.frameBuffers["startUp"].x;
	int startWindowY = gui.frameBuffers["startUp"].y;
	int startWindowW = gui.frameBuffers["startUp"].width;
	int startWindowH = gui.frameBuffers["startUp"].height;

	//Default canvas size
	int canvasWidth = 20;
	int canvasHeight = 20;
	
	//Width banner/buttons
	int widthBannerWidth = 10;
	int widthBannerHeight = 1;
	int widthBannerX = startWindowX + 5;
	int widthBannerY = startWindowY + 18;
	int widthAddX = widthBannerX + 3;
	int widthAddY = widthBannerY - 3;
	int widthSubX = widthBannerX + 3;
	int widthSubY = widthBannerY + 1;

	gui.addBanner(
		"width", 
		"Width:" + to_string(canvasWidth), 
		widthBannerWidth, widthBannerHeight, 
		widthBannerX, widthBannerY
	);

	gui.addButton("widthAdd", "+", 3, 3, widthAddX, widthAddY);
	gui.buttons["widthAdd"].active = true;
	gui.addButton("widthSub", "-", 3, 3, widthSubX, widthSubY);
	gui.buttons["widthSub"].active = true;
	//**End: Width banner/buttons

	//Height banner/buttons
	int heightBannerWidth = 10;
	int heightBannerHeight = 1;
	int heightBannerX = startWindowX + 16;
	int heightBannerY = startWindowY + 18;
	int heightAddX = heightBannerX + 3;
	int heightAddY = heightBannerY - 3;
	int heightSubX = heightBannerX + 3;
	int heightSubY = heightBannerY + 1;
	
	gui.addBanner(
		"height", 
		"Height:" + to_string(canvasHeight), 
		heightBannerWidth, heightBannerHeight, 
		heightBannerX, heightBannerY
	);

	gui.addButton("heightAdd", "+", 3, 3, heightAddX, heightAddY);
	gui.buttons["heightAdd"].active = true;
	gui.addButton("heightSub", "-", 3, 3, heightSubX, heightSubY);
	gui.buttons["heightSub"].active = true;
	//**End: Height banner/buttons

	//Start program button
	gui.addButton("start", "Start", 7, 3, 27, 28);
	gui.buttons["start"].active = true;
	//**End: Start program button

	//Start page program loop
	int numEventsRead = 0;
	bool quit = false;
	while (!quit) {
		if (gui.buttons["start"].pressed) {
			quit = true;
		}

		numEventsRead = gui.cWindow.getInput();

		for (int i = 0; i < numEventsRead; i++) {
			INPUT_RECORD input = gui.cWindow.eventBuffer[i];
			if (input.EventType == MOUSE_EVENT) {
				MOUSE_EVENT_RECORD mouseEvent = gui.cWindow.eventBuffer[i].Event.MouseEvent;
				gui.checkForMouseOverButton(mouseEvent);
			}
		}

		gui.addFrameBufferToWindow("startUp", "frameBuffer");
		gui.addFrameBufferToWindow("startUp", "banner");
		
		//Width add button pressed
		if (gui.buttons["widthAdd"].pressed) {
			canvasWidth++;
			gui.banners["width"].setText("Width:" + to_string(canvasWidth));
			gui.buttons["widthAdd"].pressed = false;
		}
		
		//Width sub button pressed
		if (gui.buttons["widthSub"].pressed) {
			canvasWidth--;
			gui.banners["width"].setText("Width:" + to_string(canvasWidth));
			gui.buttons["widthSub"].pressed = false;
		}
		
		//Height add button pressed
		if (gui.buttons["heightAdd"].pressed) {
			canvasHeight++;
			gui.banners["height"].setText("Height:" + to_string(canvasHeight));
			gui.buttons["heightAdd"].pressed = false;
		}

		//Height sub button pressed
		if (gui.buttons["heightSub"].pressed) {
			canvasHeight--;
			gui.banners["height"].setText("Height:" + to_string(canvasHeight));
			gui.buttons["heightSub"].pressed = false;
		}
		
		gui.addFrameBufferToWindow("widthAdd", "button");
		gui.addFrameBufferToWindow("widthSub", "button");
		gui.addFrameBufferToWindow("width", "banner");
		gui.addFrameBufferToWindow("heightAdd", "button");
		gui.addFrameBufferToWindow("heightSub", "button");
		gui.addFrameBufferToWindow("height", "banner");
		gui.addFrameBufferToWindow("start", "button");

		gui.cWindow.upDateWindow();
	}
	//**End: Start page program loop

	//Clear the screen ready for the main program
	gui.cWindow.clearScreen(' ', BG_DARK_GREY | FG_DARK_GREY);
	
	//Finally get the chosen width and height
	widthHeight[0] = canvasWidth;
	widthHeight[1] = canvasHeight;

	return widthHeight;
}

void populateCopyBox(FrameBuffer &copyBox, FrameBuffer canvas) {
	//set the copy box characters/colours to the canvas characters/colours at copy box coords dynamically
	//as user moves the box around canvas
	for (int x = 0; x < copyBox.width; x++) {
		for (int y = 0; y < copyBox.height; y++) {
			int canvasIndex = (y + copyBox.y - canvas.y)*canvas.width + (x + copyBox.x - canvas.x);
			copyBox.setCellChrCol(y*copyBox.width + x,
				canvas.characters[canvasIndex] == ' ' ? QUARTER : canvas.characters[canvasIndex],
				canvas.colours[canvasIndex]
			);
		}
	}
}

void copy(FrameBuffer &copyBox, FrameBuffer canvas) {
	//set the copy box characters/colours to the canvas characters/colours at copy box coords permanently
	for (int x = 0; x < copyBox.width; x++) {
		for (int y = 0; y < copyBox.height; y++) {
			int canvasIndex = (y + copyBox.y - canvas.y)*canvas.width + (x + copyBox.x - canvas.x);
			copyBox.setCellChrCol(y*copyBox.width + x,
				canvas.characters[canvasIndex] == ' ' ? QUARTER : canvas.characters[canvasIndex],
				canvas.colours[canvasIndex]
			);
		}
	}
}

void paste(FrameBuffer copyBox, FrameBuffer &canvas) {
	//set the canvas characters/colours to the copyBox characters/colours at copy box coords permantely
	for (int x = 0; x < copyBox.width; x++) {
		for (int y = 0; y < copyBox.height; y++) {
			int canvasIndex = (y + copyBox.y - canvas.y)*canvas.width + (x + copyBox.x - canvas.x);
			int copyIndex = y * copyBox.width + x;
			canvas.setCellChrCol(canvasIndex,
				copyBox.characters[copyIndex] == QUARTER ? ' ' : copyBox.characters[copyIndex],
				copyBox.colours[copyIndex]
			);
		}
	}
}

int main() {

	//Create window
	const char* title = "Dans Sprite Creator";
	short screenWidth = SCREEN_WIDTH;
	short screenHeight = SCREEN_HEIGHT;
	int fontWidth = FONT_WIDTH;
	int fontHeight = FONT_HEIGHT;
	GUI gui = GUI(title, screenWidth, screenHeight, fontWidth, fontHeight);
	//**End: create window
	

	//Add the save/load frame buffer object to gui
	gui.addSaveFrameBufferPopUp("savePopUp", "SAVE", 0, 0);

	string savePopUpSaveButtonID = gui.saveFrameBufferPopUp["savePopUp"].saveButtonAttributes.id;
	string savePopUpCancelButtonID = gui.saveFrameBufferPopUp["savePopUp"].cancelButtonAttributes.id;
	string savePopUpTextBoxID = gui.saveFrameBufferPopUp["savePopUp"].textBoxAttributes.id;

	gui.addLoadFrameBufferPopUp("loadPopUp", 20, 0);

	string loadPopUpSaveButtonID = gui.loadFrameBufferPopUp["loadPopUp"].loadButtonAttributes.id;
	string loadPopUpCancelButtonID = gui.loadFrameBufferPopUp["loadPopUp"].cancelButtonAttributes.id;
	string loadPopUpListID = gui.loadFrameBufferPopUp["loadPopUp"].listAttributes.id;
	//**End: Add the save/load frame buffer object to gui


	//For paintbrush
	short paintColour = 0;
	short shade = ' ';


	//Create palette, shade picker, shade colour picker
	Palette palette = Palette(1, 3);
	gui.addPreMadeFrameBuffer("palette", palette);
	int paletteXmin = gui.frameBuffers["palette"].x;
	int paletteXmax = gui.frameBuffers["palette"].x + gui.frameBuffers["palette"].width;
	int paletteYmin = gui.frameBuffers["palette"].y;
	int paletteYmax = gui.frameBuffers["palette"].y + gui.frameBuffers["palette"].height;
	
	Swatch colourSwatch = Swatch(1, gui.frameBuffers["palette"].height + 4);
	colourSwatch.setChrCol(' ', BG_BLACK | FG_BLACK);
	gui.addPreMadeFrameBuffer("colourSwatch", colourSwatch);
	
	Swatch shadePicker = Swatch(1, gui.frameBuffers["palette"].height + 
		gui.frameBuffers["colourSwatch"].height + 5
	);
	shadePicker.setCellChrCol(0, QUARTER, BG_WHITE | FG_BLACK);
	shadePicker.setCellChrCol(1, HALF, BG_WHITE | FG_BLACK);
	shadePicker.setCellChrCol(2, THREEQUARTERS, BG_WHITE | FG_BLACK);
	shadePicker.setCellChrCol(3, SOLID, BG_WHITE | FG_BLACK);
	gui.addPreMadeFrameBuffer("shadePicker", shadePicker);
	int shadePickXmin = gui.frameBuffers["shadePicker"].x;
	int shadePickXmax = gui.frameBuffers["shadePicker"].x + gui.frameBuffers["shadePicker"].width;
	int shadePickYmin = gui.frameBuffers["shadePicker"].y;					  
	int shadePickYmax = gui.frameBuffers["shadePicker"].y + gui.frameBuffers["shadePicker"].height;
	
	ShadePalette shadePalette = ShadePalette(1, 
		gui.frameBuffers["palette"].height + 
		gui.frameBuffers["colourSwatch"].height + 
		gui.frameBuffers["shadePicker"].height + 6
	);
	gui.addPreMadeFrameBuffer("shadePalette", shadePalette);
	int shadePallXmin = gui.frameBuffers["shadePalette"].x;
	int shadePallXmax = gui.frameBuffers["shadePalette"].x + gui.frameBuffers["shadePalette"].width;
	int shadePallYmin = gui.frameBuffers["shadePalette"].y;							
	int shadePallYmax = gui.frameBuffers["shadePalette"].y + gui.frameBuffers["shadePalette"].height;
	//**End: Create palette, shade picker, shade colour picker


	//Create load/save menu bar
	gui.addMenuBar("menu", 0, 0);


	//Create copy/paste frame buffer
	gui.addFrameBuffer("copy", 3, 3, 10, 10);
	

	//Create status bar
	gui.addBanner("status", "Draw mode", 10, 1, 50, 0);


	//Display start page and create canvas
	addStartPage(gui);
	vector<int> canvasWH = getCanvasWidthHeight(gui);
	gui.addFrameBuffer("canvas", canvasWH[0], canvasWH[1], 8, 4);
	gui.frameBuffers["canvas"].setAll(' ', BG_WHITE);
	//**End: Display start page and create canvas


	//Main program loop
	int numEventsRead = 0;
	bool quit = false;
	bool copyMode = false;
	bool pasteMode = false;
	//Draw mode is implied, if not in copy/paste mode we have to be in draw mode

	while (!quit) {
		//Handle input
		numEventsRead = gui.cWindow.getInput();
		
		for (int i = 0; i < numEventsRead; i++) {
			INPUT_RECORD input = gui.cWindow.eventBuffer[i];
			
			//Keyboard events
			if (input.Event.KeyEvent.bKeyDown) {
				short keyCode = input.Event.KeyEvent.wVirtualKeyCode;

				//press A key to get back to draw mode from copy/paste mode
				if (keyCode == VK_A) { 
					shade = ' '; 
					colourSwatch.setChrCol(shade, paintColour);
					gui.frameBuffers["colourSwatch"] = colourSwatch;
					copyMode = false;
					pasteMode = false;
				}

				//press C key to enable copy mode
				if (keyCode == VK_C) {
					copyMode = true;
					pasteMode = false;
				}

				//Press up/down/left/right to make copy box bigger/smaller in x/y direction
				//Press K to copy canvas to copy box
				if (copyMode) {
					int copyW = gui.frameBuffers["copy"].width;
					int copyH = gui.frameBuffers["copy"].height;
					int copyX = gui.frameBuffers["copy"].x;
					int copyY = gui.frameBuffers["copy"].y;
					
					int canvasW = gui.frameBuffers["canvas"].width;
					int canvasH = gui.frameBuffers["canvas"].height;
					int canvasX = gui.frameBuffers["canvas"].x;
					int canvasY = gui.frameBuffers["canvas"].y;

					//Make copy box bigger/smaller but not bigger than canvas and ensure
					//copy box cannot be smaller than a 1 X 1 pixel 
					if (keyCode == VK_DOWN && copyY + copyH < canvasY + canvasH) {
						gui.frameBuffers["copy"] = FrameBuffer(copyW, copyH + 1, copyX, copyY);
						populateCopyBox(gui.frameBuffers["copy"], gui.frameBuffers["canvas"]);
					}
					if (keyCode == VK_UP && copyH > 1) {
						gui.frameBuffers["copy"] = FrameBuffer(copyW, copyH - 1, copyX, copyY);
						populateCopyBox(gui.frameBuffers["copy"], gui.frameBuffers["canvas"]);
					}
					if (keyCode == VK_LEFT && copyW > 1) {
						gui.frameBuffers["copy"] = FrameBuffer(copyW - 1, copyH, copyX, copyY);
						populateCopyBox(gui.frameBuffers["copy"], gui.frameBuffers["canvas"]);
					}
					if (keyCode == VK_RIGHT && copyX + copyW < canvasX + canvasW) {
						gui.frameBuffers["copy"] = FrameBuffer(copyW + 1, copyH, copyX, copyY);
						populateCopyBox(gui.frameBuffers["copy"], gui.frameBuffers["canvas"]);
					}
					
					//Press K to enter paste mode and copy the canvas to the copy box
					if (keyCode == VK_K) {
						copyMode = false;
						pasteMode = true;
						copy(gui.frameBuffers["copy"], gui.frameBuffers["canvas"]);
					}
				}

				//Paste the copy box to the canvas at the current mouse coords
				if (keyCode == VK_P) {
					paste(gui.frameBuffers["copy"], gui.frameBuffers["canvas"]);
				}
			}
			//End: Keyboard events

			//Mouse events
			if (input.EventType == MOUSE_EVENT) {
				if (gui.menuBar["menu"].exitClicked) { quit = true; }//Quit

				MOUSE_EVENT_RECORD mouseEvent = gui.cWindow.eventBuffer[i].Event.MouseEvent;
				int mouseX = mouseEvent.dwMousePosition.X;
				int mouseY = mouseEvent.dwMousePosition.Y;
				
				//Handle load pop up events
				if (gui.loadFrameBufferPopUp["loadPopUp"].active) {
					gui.checkForLoadPopUpEvents("loadPopUp", mouseEvent, gui.frameBuffers["canvas"]);
					gui.cWindow.clearScreen(' ', BG_DARK_GREY | FG_DARK_GREY);
				}

				//Handle save pop up events
				if (gui.saveFrameBufferPopUp["savePopUp"].active) {
					gui.checkForSavePopUpEvents("savePopUp", "loadPopUp", mouseEvent, gui.frameBuffers["canvas"]);
				}

				//Handle menu bar events
				gui.checkForMouseOverMenuBar(mouseEvent);

				//User painting/erasing or copying/pasting on canvas
				int canvasXmin = gui.frameBuffers["canvas"].x;
				int canvasXmax = gui.frameBuffers["canvas"].x + gui.frameBuffers["canvas"].width;
				int canvasYmin = gui.frameBuffers["canvas"].y;
				int canvasYmax = gui.frameBuffers["canvas"].y + gui.frameBuffers["canvas"].height;
				
				if (
					mouseX >= canvasXmin && mouseX < canvasXmax &&
					mouseY >= canvasYmin && mouseY < canvasYmax
					) 
				{
					//Painting
					if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && !copyMode && !pasteMode) {
						gui.frameBuffers["canvas"].setCellChrCol(
							(mouseY - canvasYmin)*gui.frameBuffers["canvas"].width + mouseX - canvasXmin,
							shade,
							paintColour
						);
					}

					//Erasing
					if (mouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
						gui.frameBuffers["canvas"].setCellChrCol(
							(mouseY - canvasYmin)*gui.frameBuffers["canvas"].width + mouseX - canvasXmin,
							' ',
							BG_WHITE | FG_WHITE
						);
					}

					//User moving copy box around with mouse
					if (copyMode || pasteMode) {
						if (mouseX < gui.frameBuffers["canvas"].x + gui.frameBuffers["canvas"].width - gui.frameBuffers["copy"].width + 1 &&
							mouseY < gui.frameBuffers["canvas"].y + gui.frameBuffers["canvas"].height - gui.frameBuffers["copy"].height + 1 
							) 
						{
							if (copyMode) {
								int w = gui.frameBuffers["copy"].width;
								int h = gui.frameBuffers["copy"].height;
								gui.frameBuffers["copy"] = FrameBuffer(w, h, mouseX, mouseY);
								populateCopyBox(gui.frameBuffers["copy"], gui.frameBuffers["canvas"]);
							}
							
							if (pasteMode) {
								gui.frameBuffers["copy"].x = mouseX;
								gui.frameBuffers["copy"].y = mouseY;
							}
						}
					}
					//**End: User moving copy box around with mouse
				}
				//End: User painting on canvas

				//User choosing a colour
				if (
					mouseX >= paletteXmin && mouseX < paletteXmax &&
					mouseY >= paletteYmin && mouseY < paletteYmax
					)
				{
					if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						int pWidth = gui.frameBuffers["palette"].width;
						int index = (mouseY - paletteYmin)*gui.frameBuffers["palette"].width + mouseX - paletteXmin;

						//Choose colour
						if ((mouseY - paletteYmin) * pWidth + (mouseX - paletteXmin) < 16) {
							paintColour = gui.frameBuffers["palette"].colours[index];
							
							//Set colour swatch colour
							colourSwatch.setChrCol(shade, paintColour);
							gui.frameBuffers["colourSwatch"] = colourSwatch;

							//Set shade palette colour
							shadePalette.setChrBGCol(shade, paintColour);
							gui.frameBuffers["shadePalette"] = shadePalette;
						}
					}
				}
				//**End: User choosing a colour

				//User choosing shade 
				if (
					mouseX >= shadePickXmin && mouseX < shadePickXmax &&
					mouseY >= shadePickYmin && mouseY < shadePickYmax
					)
				{
					if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
						int sWidth = gui.frameBuffers["shadePicker"].width;
						int index = (mouseY - shadePickYmin)*gui.frameBuffers["shadePicker"].width + mouseX - shadePickXmin;

						//Choose shade
						if ((mouseY - shadePickYmin) * sWidth + (mouseX - shadePickXmin) < 4) {
							shade = gui.frameBuffers["shadePicker"].characters[index];
							
							//Set colour swatch shade
							colourSwatch.setChrCol(shade, paintColour);
							gui.frameBuffers["colourSwatch"] = colourSwatch;

							//Set shade palette shade
							shadePalette.setChrBGCol(shade, paintColour);
							gui.frameBuffers["shadePalette"] = shadePalette;
						}
					}
				}
				//**End: User choosing shade

				//User choosing shade colour
				if (
					mouseX >= shadePallXmin && mouseX < shadePallXmax &&
					mouseY >= shadePallYmin && mouseY < shadePallYmax
					)
				{
					if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {

						int sWidth = gui.frameBuffers["shadePalette"].width;
						int index = (mouseY - shadePallYmin)*gui.frameBuffers["shadePalette"].width + mouseX - shadePallXmin;

						//Choose shade
						if ((mouseY - shadePallYmin) * sWidth + (mouseX - shadePallXmin) < 16) {
							paintColour = gui.frameBuffers["shadePalette"].colours[index];

							//Set colour swatch shade
							colourSwatch.setChrCol(shade, paintColour);
							gui.frameBuffers["colourSwatch"] = colourSwatch;
						}
					}
				}
				//**End: User choosing shade colour
			}
			//**End: Mouse events
		}
		//**End: Handle input

		if (gui.menuBar["menu"].saveClicked) {
			//Enable the save frame pop up
			gui.saveFrameBufferPopUp["savePopUp"].active = true;
			gui.buttons[savePopUpSaveButtonID].active = true;
			gui.buttons[savePopUpCancelButtonID].active = true;
			gui.textBoxes[savePopUpTextBoxID].active = true;
			gui.menuBar["menu"].saveClicked = false;
		}

		if (gui.menuBar["menu"].loadClicked) {
			//Enable the load frame pop up
			gui.loadFrameBufferPopUp["loadPopUp"].active = true;
			gui.buttons[loadPopUpSaveButtonID].active = true;
			gui.buttons[loadPopUpCancelButtonID].active = true;
			gui.lists[loadPopUpListID].active = true;
			gui.menuBar["menu"].loadClicked = false;
		}

		//Add all the object buffers to the window buffer, must be added in this order to display correctly
		gui.addFrameBufferToWindow("shadePalette", "frameBuffer");
		gui.addFrameBufferToWindow("shadePicker", "frameBuffer"); 
		gui.addFrameBufferToWindow("colourSwatch", "frameBuffer");
		gui.addFrameBufferToWindow("palette", "frameBuffer");
		gui.addFrameBufferToWindow("canvas", "frameBuffer");
		gui.addMenuBarToWindow("menu");
		gui.addFrameBufferToWindow("status", "banner");

		//Add copy box
		if (copyMode || pasteMode) {
			gui.addFrameBufferToWindow("copy", "frameBuffer");
		}
		
		//Set the status banner
		if (copyMode) {
			gui.banners["status"].setText("Copy mode");
		}
		else if (pasteMode) {
			gui.banners["status"].setText("Paste mode");
		}
		else {
			gui.banners["status"].setText("Draw mode");
		}
		
		//Add the save/load frame buffers pop up to window buffer
		if (gui.saveFrameBufferPopUp["savePopUp"].active) {
			gui.addSaveFrameBufferPopUpToWindow("savePopUp");
		}

		if (gui.loadFrameBufferPopUp["loadPopUp"].active) {
			gui.addLoadFrameBufferPopUpToWindow("loadPopUp");
		}

		gui.cWindow.upDateWindow();//Finally write all the buffers to the console window
	}
	//**End: main program loop

	return 0;
}