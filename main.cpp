#include "raylib.h"

typedef enum {
    MENU,
    PLAYING,
    GAME_OVER,
    SETTINGS,
    EXIT
} GameState;

typedef enum {
    LEVEL_1_DIALOGUE,  // Dating Sim Choice Phase
    LEVEL_2,           // Progression Stage
    LEVEL_3,           // Final Stage
    LEVEL_COMPLETE     // Victory
} GameLevel;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Raylib - Dating Sim Choice Level");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    GameState currentState = MENU;
    GameLevel currentLevel = LEVEL_1_DIALOGUE;

    // Menu settings
    int selectedOption = 0;
    const int totalOptions = 3;

    // Dialogue Choice Box bounds
    Rectangle btnChar1 = { 80, 350, 300, 50 };
    Rectangle btnChar2 = { 420, 350, 300, 50 };

    int currentScore = 0;
    int scoreToAdvance = 10;

    while (!WindowShouldClose() && currentState != EXIT) {

        // ==========================================
        // UPDATE LOGIC
        // ==========================================

        if (currentState == MENU) {
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                selectedOption = (selectedOption + 1) % totalOptions;
            }
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
            }

            Vector2 mousePos = GetMousePosition();
            for (int i = 0; i < totalOptions; i++) {
                Rectangle btnBounds = { 300, (float)(180 + i * 50), 200, 40 };
                if (CheckCollisionPointRec(mousePos, btnBounds)) {
                    selectedOption = i;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (i == 0) {
                            currentState = PLAYING;
                            currentLevel = LEVEL_1_DIALOGUE;
                            currentScore = 0;
                        }
                        if (i == 1) currentState = SETTINGS;
                        if (i == 2) currentState = EXIT;
                    }
                }
            }

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (selectedOption == 0) {
                    currentState = PLAYING;
                    currentLevel = LEVEL_1_DIALOGUE;
                    currentScore = 0;
                }
                if (selectedOption == 1) currentState = SETTINGS;
                if (selectedOption == 2) currentState = EXIT;
            }
        } 
        else if (currentState == PLAYING) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = MENU;
            }

            if (currentLevel == LEVEL_1_DIALOGUE) {
                Vector2 mousePos = GetMousePosition();
                bool hover1 = CheckCollisionPointRec(mousePos, btnChar1);
                bool hover2 = CheckCollisionPointRec(mousePos, btnChar2);

                // Option 1: Trust Character 1 -> Progress to Level 2
                if (IsKeyPressed(KEY_ONE) || (hover1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
                    currentLevel = LEVEL_2;
                    currentScore = 5;
                }

                // Option 2: Trust Character 2 -> Betrayal / Game Over
                if (IsKeyPressed(KEY_TWO) || (hover2 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
                    currentState = GAME_OVER;
                }
            } 
            else if (currentLevel == LEVEL_2 || currentLevel == LEVEL_3) {
                // Press SPACE to advance through subsequent levels
                if (IsKeyPressed(KEY_SPACE)) {
                    currentScore += 5;
                    if (currentScore >= 10 && currentLevel == LEVEL_2) {
                        currentLevel = LEVEL_3;
                    } else if (currentScore >= 20 && currentLevel == LEVEL_3) {
                        currentLevel = LEVEL_COMPLETE;
                    }
                }
            }
        } 
        else if (currentState == GAME_OVER) {
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                currentState = PLAYING;
                currentLevel = LEVEL_1_DIALOGUE;
                currentScore = 0;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = MENU;
            }
        } 
        else if (currentState == SETTINGS) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = MENU;
            }
        }

        // ==========================================
        // DRAW LOGIC
        // ==========================================

        BeginDrawing();

        if (currentState == MENU) {
            ClearBackground(RAYWHITE);
            DrawText("MAIN MENU", 310, 80, 30, DARKGRAY);

            const char* options[] = { "Start Game", "Settings", "Exit" };

            for (int i = 0; i < totalOptions; i++) {
                Rectangle btnBounds = { 300, (float)(180 + i * 50), 200, 40 };
                bool isSelected = (selectedOption == i);

                DrawRectangleRec(btnBounds, isSelected ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(btnBounds, 2, isSelected ? BLUE : DARKGRAY);
                DrawText(options[i], 330, 190 + i * 50, 20, isSelected ? BLUE : BLACK);
            }

            DrawText("Use UP/DOWN arrows and ENTER, or click with MOUSE", 200, 380, 16, GRAY);
        } 
        else if (currentState == PLAYING) {

            if (currentLevel == LEVEL_1_DIALOGUE) {
                ClearBackground(DARKBLUE);

                // Character Visual Placeholders (Avatars)
                DrawRectangle(150, 100, 140, 180, SKYBLUE);
                DrawText("Char 1", 180, 180, 22, WHITE);

                DrawRectangle(510, 100, 140, 180, PURPLE);
                DrawText("Char 2", 540, 180, 22, WHITE);

                // Dating Sim Dialogue Box Frame
                DrawRectangle(40, 270, 720, 150, BLACK);
                DrawRectangleLinesEx((Rectangle){ 40, 270, 720, 150 }, 3, RAYWHITE);

                // Dialogue Text
                DrawText("LEVEL 1 - THE CROSSROADS", 60, 280, 18, YELLOW);
                DrawText("Two strangers offer to guide you through the fog. Who do you trust?", 60, 305, 18, RAYWHITE);

                Vector2 mousePos = GetMousePosition();
                bool hover1 = CheckCollisionPointRec(mousePos, btnChar1);
                bool hover2 = CheckCollisionPointRec(mousePos, btnChar2);

                // Choice Button 1
                DrawRectangleRec(btnChar1, hover1 ? SKYBLUE : DARKGRAY);
                DrawRectangleLinesEx(btnChar1, 2, hover1 ? WHITE : LIGHTGRAY);
                DrawText("[1] Believe Character 1", btnChar1.x + 20, btnChar1.y + 15, 18, WHITE);

                // Choice Button 2
                DrawRectangleRec(btnChar2, hover2 ? RED : DARKGRAY);
                DrawRectangleLinesEx(btnChar2, 2, hover2 ? WHITE : LIGHTGRAY);
                DrawText("[2] Believe Character 2", btnChar2.x + 20, btnChar2.y + 15, 18, WHITE);

            } 
            else if (currentLevel == LEVEL_2) {
                ClearBackground(BLUE);
                DrawText("LEVEL 2: Following Character 1's Advice", 40, 40, 22, WHITE);
                DrawText(TextFormat("Score: %d / 10", currentScore), 40, 80, 20, LIGHTGRAY);
                DrawText("Press SPACE to advance forward!", 240, 220, 20, RAYWHITE);
            } 
            else if (currentLevel == LEVEL_3) {
                ClearBackground(PURPLE);
                DrawText("LEVEL 3: Approaching the Destination", 40, 40, 22, WHITE);
                DrawText(TextFormat("Score: %d / 20", currentScore), 40, 80, 20, LIGHTGRAY);
                DrawText("Press SPACE to reach the goal!", 250, 220, 20, RAYWHITE);
            } 
            else if (currentLevel == LEVEL_COMPLETE) {
                ClearBackground(DARKGREEN);
                DrawText("YOU WON!", 340, 160, 40, WHITE);
                DrawText("Believing Character 1 brought you to safety.", 200, 220, 20, LIGHTGRAY);
                DrawText("Press ESC to return to Menu", 270, 320, 20, LIGHTGRAY);
            }
        } 
        else if (currentState == GAME_OVER) {
            ClearBackground(MAROON);

            // Dating Sim Game Over Box
            DrawRectangle(100, 100, 600, 250, BLACK);
            DrawRectangleLinesEx((Rectangle){ 100, 100, 600, 250 }, 3, RED);

            DrawText("YOU DIE!", 330, 130, 40, RED);
            DrawText("Character 2 lied to you and led you straight into a trap.", 140, 200, 20, WHITE);

            DrawText("Press SPACE to restart Level 1", 250, 280, 18, LIGHTGRAY);
            DrawText("Press ESC for Main Menu", 280, 310, 18, GRAY);
        } 
        else if (currentState == SETTINGS) {
            ClearBackground(RAYWHITE);
            DrawText("SETTINGS SCREEN", 270, 180, 30, ORANGE);
            DrawText("Press ESC to return to Menu", 270, 240, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}