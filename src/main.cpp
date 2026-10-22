#include "raylib.h"

// Define the game states
enum GameState {
    MENU,
    PLAYING,
    SETTINGS,
    EXIT
};

int main() {
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Raylib - Player Choice Menu");
    SetTargetFPS(60);

    GameState currentState = MENU;
    int selectedOption = 0; // 0: Start Game, 1: Settings, 2: Exit
    const int totalOptions = 3;

    while (!WindowShouldClose() && currentState != EXIT) {        // UPDATE LOGIC

        if (currentState == MENU) {
            // Keyboard navigation
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                selectedOption = (selectedOption + 1) % totalOptions;
            }
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
            }

            // Mouse hover & click detection
            Vector2 mousePos = GetMousePosition();
            for (int i = 0; i < totalOptions; i++) {
                Rectangle btnBounds = { 300, (float)(180 + i * 50), 200, 40 };
                if (CheckCollisionPointRec(mousePos, btnBounds)) {
                    selectedOption = i;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (i == 0) currentState = PLAYING;
                        if (i == 1) currentState = SETTINGS;
                        if (i == 2) currentState = EXIT;
                    }
                }
            }

            // Select option with ENTER or SPACE
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (selectedOption == 0) currentState = PLAYING;
                if (selectedOption == 1) currentState = SETTINGS;
                if (selectedOption == 2) currentState = EXIT;
            }
        } 
        else if (currentState == PLAYING || currentState == SETTINGS) {
            // Return to menu with ESCAPE key
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = MENU;
            }
        }

        // DRAW LOGIC

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentState == MENU) {
            DrawText("MAIN MENU", 310, 80, 30, DARKGRAY);

            const char* options[] = { "Start Game", "Settings", "Exit" };

            for (int i = 0; i < totalOptions; i++) {
                Rectangle btnBounds = { 300, (float)(180 + i * 50), 200, 40 };
                bool isSelected = (selectedOption == i);

                // Draw button background
                DrawRectangleRec(btnBounds, isSelected ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(btnBounds, 2, isSelected ? BLUE : DARKGRAY);

                // Draw button label
                DrawText(options[i], 330, 190 + i * 50, 20, isSelected ? BLUE : BLACK);
            }

            DrawText("Use UP/DOWN arrows and ENTER, or click with MOUSE", 200, 380, 16, GRAY);
        } 
        else if (currentState == PLAYING) {
            DrawText("GAME SCREEN", 300, 200, 30, GREEN);
            DrawText("Press ESC to return to Menu", 270, 240, 20, DARKGRAY);
        } 
        else if (currentState == SETTINGS) {
            DrawText("SETTING SCREEN", 270, 180, 30, ORANGE);
            DrawText("Press ESC to return to Menu", 270, 240, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}