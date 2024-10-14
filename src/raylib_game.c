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

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 1200;
static const int screenHeight = 800;

Vector2 points[MAXPOINTS];
int numPoints = 0;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateFrame(void);
static void DrawFrame(void);

static void addPoint(Vector2 point);
static void clearPoints(Vector2 point);
static void drawPoint(Vector2 point);

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

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 pointerPos = GetMousePosition();
		TraceLog(LOG_INFO, "Mouse clicked at position: (%f, %f)", pointerPos.x, pointerPos.y);
		addPoint(pointerPos);
	}
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

static void clearPoints(Vector2 point)
{
	numPoints = 0;
}

static void drawPoint(Vector2 point)
{
	const float offset = pointSize / 2;
	DrawRectangle(point.x-offset, point.y-offset, pointSize, pointSize, pointColor);
	/* DrawPixelV(point, pointColor); */
}
