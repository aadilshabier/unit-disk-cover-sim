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
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//----------------------------------------------------------------------------------
// Constant Definition
//---------------------------------------------------------------------------------
static const int screenWidth = 1200;
static const int screenHeight = 800;

#define MAXPOINTS 128

static const Color pointColor = BLACK;
static const float pointSize = 6;

static const Color approxDiskColor = BLUE;

static const int buttonWidth = 80;
static const int buttonHeight = 40;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Vector2 points[MAXPOINTS];
int numPoints = 0;

Vector2 disks[MAXPOINTS*4];
int numDisks = 0;

float diskRadius = 100.0f;
float currentDiskRadius = 0;

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

static void computeUnitDisksApprox();
static void computeUnitDisksOptim();

static int pointCompare(const void *pA, const void *pB);

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
		numDisks = 0;
		currentDiskRadius = diskRadius;
		computeUnitDisksApprox();
		compute = false;
	}

	Vector2 pointerPos = GetMousePosition();
	if (shouldTrack(pointerPos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		TraceLog(LOG_DEBUG, "Mouse clicked at position: (%f, %f)", pointerPos.x, pointerPos.y);
		addPoint(pointerPos);
	}
}

static void DrawGUI(void)
{
	int baseH = 50;
	int baseW = 50;
	int offset = 10;
	reset = GuiButton((Rectangle){baseW, baseH, buttonWidth, buttonHeight}, "Reset");
	compute = GuiButton((Rectangle){baseW, baseH+offset+buttonHeight, buttonWidth, buttonHeight}, "Compute");

	GuiSlider((Rectangle){baseW, baseH+2*offset+2*buttonHeight, buttonWidth, buttonHeight/2}, "0", "200", &diskRadius, 0.0f, 200.f);
}

// Draw game frame
static void DrawFrame(void)
{

    // Draw
    //----------------------------------------------------------------------------------
	ClearBackground(WHITE);
	DrawText("Click to add points!", screenWidth/2-150, screenHeight/2, 20, LIGHTGRAY);

	// Draw points
	for (int i=0; i<numPoints; i++) {
		drawPoint(points[i]);
	}

	// Draw points
	for (int i=0; i<numDisks; i++) {
		DrawCircleLinesV(disks[i], currentDiskRadius, approxDiskColor);
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
	numDisks = 0;
}

static void drawPoint(Vector2 point)
{
	const float offset = (pointSize+1) / 2;
	DrawRectangle(point.x-offset, point.y-offset, pointSize, pointSize, pointColor);
	/* DrawPixelV(point, pointColor); */
}

// Check if clicked point is somewhere that can contain points. i.e: not on
static int shouldTrack(Vector2 pointerPos)
{
	Rectangle guiArea = {50, 50, 100, 150};
	return !CheckCollisionPointRec(pointerPos, guiArea);
}

static int pointCompare(const void *pA, const void *pB)
{
	Vector2 a = *(Vector2*)pA;
	Vector2 b = *(Vector2*)pB;
	if (a.x == b.x) {
		return a.y - b.y;
	} else {
		return a.x - b.x;
	}
}

// Approximation Algorithm to compute unit disks
static void computeUnitDisksApprox()
{
	qsort(points, numPoints, sizeof(*points), pointCompare);

	bool covered[MAXPOINTS] = {0};
	for (int i=0; i<numPoints; i++) {
		if (covered[i]) continue;

		Vector2 point = points[i];

		// Add disks
		Vector2 dist1 = {sqrt(3)*currentDiskRadius, 0};
		Vector2 dist2 = Vector2Rotate(dist1, +PI/3);
		Vector2 dist3 = Vector2Rotate(dist1, -PI/3);

		disks[numDisks++] = point;
		disks[numDisks++] = Vector2Add(point, dist1);
		disks[numDisks++] = Vector2Add(point, dist2);
		disks[numDisks++] = Vector2Add(point, dist3);

		covered[i] = true;
		for (int j=i+1; j<numPoints; j++) {
			// NOTE: instead check against the 4 disks above for better results
			for (int k=numDisks-4; k<numDisks; k++) {
				if (CheckCollisionPointCircle(points[j], disks[k], currentDiskRadius)) {
					covered[j] = true;
				}
			}
		}
	}
}
