/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//----------------------------------------------------------------------------------
// Constant Definition
//---------------------------------------------------------------------------------
static const Color pointColor = BLACK;
static const float pointSize = 10;
#define MAXPOINTS 128

static const int buttonWidth = 80;
static const int buttonHeight = 40;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 1200;
static const int screenHeight = 800;

Vector2 points[MAXPOINTS];
int numPoints = 0;

int reset = false;
int compute = false;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void DrawGUI(void);
static void UpdateFrame(void);
static void DrawFrame(void);

static void addPoint(Vector2 point);
static void clearPoints(void);
static void drawPoint(Vector2 point);
/* float TextToFloat(const char *text); */

static int shouldTrack(Vector2 pointerPos);

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib game template");

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateFrame();

		BeginDrawing();
		DrawFrame();
		DrawGUI();
		EndDrawing();
    }

    // Unload global data loaded

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Update game frame
static void UpdateFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
	if (reset) {
		clearPoints();
		reset = false;
	}

	if (compute) {
		// compute result
	}

	Vector2 pointerPos = GetMousePosition();
	if (shouldTrack(pointerPos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		TraceLog(LOG_INFO, "Mouse clicked at position: (%f, %f)", pointerPos.x, pointerPos.y);
		addPoint(pointerPos);
	}
}

static void DrawGUI(void)
{
	int baseH = 50;
	int offset = 10;
	reset = GuiButton((Rectangle){50, baseH, buttonWidth, buttonHeight}, "Reset");
	compute = GuiButton((Rectangle){50, baseH+offset+buttonHeight, buttonWidth, buttonHeight}, "Compute");

	return;
}

// Draw game frame
static void DrawFrame(void)
{

    // Draw
    //----------------------------------------------------------------------------------
	ClearBackground(WHITE);
	DrawText("Click to add points!", screenWidth/2-150, screenHeight/2, 20, GRAY);

	// Draw points
	for (int i=0; i<numPoints; i++) {
		drawPoint(points[i]);
	}
	//----------------------------------------------------------------------------------
}

static void addPoint(Vector2 point)
{
	if (numPoints >= MAXPOINTS) {
		TraceLog(LOG_ERROR, "Maximum number of points reached! (%d)", MAXPOINTS);
	} else {
		points[numPoints++] = point;
	}
}

static void clearPoints(void)
{
	numPoints = 0;
}

static void drawPoint(Vector2 point)
{
	const float offset = pointSize / 2;
	DrawRectangle(point.x-offset, point.y-offset, pointSize, pointSize, pointColor);
	/* DrawPixelV(point, pointColor); */
}

// Check if clicked point is somewhere that can contain points. i.e: not on
static int shouldTrack(Vector2 pointerPos)
{
	return 1;
}
