#include "raylib.h"

typedef enum {
    MENU,
    PLAYING,
    GAME_OVER,
    SETTINGS,
    EXIT
} GameState;

typedef enum {
    LEVEL_1_DIALOGUE,
    LEVEL_2,
    LEVEL_3,
    LEVEL_COMPLETE
} GameLevel;

typedef enum {
    MEET_CHAR1,
    CHAR1_CHOICE,
    PART_WAYS,
    MEET_CHAR2,
    CHAR2_CHOICE
} DialogueStage;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "User Interface");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    GameState currentState = MENU;
    GameLevel currentLevel = LEVEL_1_DIALOGUE;
    DialogueStage dialogueStage = MEET_CHAR1;

    int selectedOption = 0;
    const int totalOptions = 3;
    int currentScore = 0;

    Rectangle btnChar1 = {80, 350, 300, 50};
    Rectangle btnChar2 = {420, 350, 300, 50};

    while (!WindowShouldClose() && currentState != EXIT)
    {
        // UPDATE

        Vector2 mouse = GetMousePosition();

        bool hover1 = CheckCollisionPointRec(mouse, btnChar1);
        bool hover2 = CheckCollisionPointRec(mouse, btnChar2);

        if (currentState == MENU)
        {
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
                selectedOption = (selectedOption + 1) % totalOptions;

            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
                selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;

            for (int i = 0; i < totalOptions; i++)
            {
                Rectangle button = {300, (float)(180 + i * 50), 200, 40};

                if (CheckCollisionPointRec(mouse, button))
                {
                    selectedOption = i;

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        if (i == 0)
                        {
                            currentState = PLAYING;
                            currentLevel = LEVEL_1_DIALOGUE;
                            dialogueStage = MEET_CHAR1;
                            currentScore = 0;
                        }
                        else if (i == 1)
                            currentState = SETTINGS;
                        else
                            currentState = EXIT;
                    }
                }
            }

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
            {
                if (selectedOption == 0)
                {
                    currentState = PLAYING;
                    currentLevel = LEVEL_1_DIALOGUE;
                    dialogueStage = MEET_CHAR1;
                    currentScore = 0;
                }
                else if (selectedOption == 1)
                    currentState = SETTINGS;
                else
                    currentState = EXIT;
            }
        }
        else if (currentState == PLAYING)
        {
            if (IsKeyPressed(KEY_ESCAPE))
            {
                currentState = MENU;
            }
            else if (currentLevel == LEVEL_1_DIALOGUE)
            {
                if (dialogueStage == MEET_CHAR1)
                {
                    if (IsKeyPressed(KEY_ENTER))
                        dialogueStage = CHAR1_CHOICE;
                }
                else if (dialogueStage == CHAR1_CHOICE)
                {
                    if (IsKeyPressed(KEY_ONE) ||
                        (hover1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
                    {
                        currentScore = 5;
                        currentLevel = LEVEL_2;
                    }
                    else if (IsKeyPressed(KEY_TWO) ||
                             (hover2 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
                    {
                        dialogueStage = PART_WAYS;
                    }
                }
                else if (dialogueStage == PART_WAYS)
                {
                    if (IsKeyPressed(KEY_ENTER))
                        dialogueStage = MEET_CHAR2;
                }
                else if (dialogueStage == MEET_CHAR2)
                {
                    if (IsKeyPressed(KEY_ENTER))
                        dialogueStage = CHAR2_CHOICE;
                }
                else if (dialogueStage == CHAR2_CHOICE)
                {
                    if (IsKeyPressed(KEY_ONE) ||IsKeyPressed(KEY_TWO)|| (hover1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
                        || (hover2 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
                    {
                        currentState = GAME_OVER;
                    }
                }
            }
            else if (currentLevel == LEVEL_2)
            {
                if (IsKeyPressed(KEY_SPACE))
                    currentLevel = LEVEL_3;
            }
            else if (currentLevel == LEVEL_3)
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    currentScore = 20;
                    currentLevel = LEVEL_COMPLETE;
                }
            }
            else if (currentLevel == LEVEL_COMPLETE)
            {
                if (IsKeyPressed(KEY_ENTER))
                    currentState = MENU;
            }
        }
        else if (currentState == GAME_OVER)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                currentState = PLAYING;
                currentLevel = LEVEL_1_DIALOGUE;
                dialogueStage = MEET_CHAR1;
                currentScore = 0;
            }
            else if (IsKeyPressed(KEY_ESCAPE))
            {
                currentState = MENU;
            }
        }
        else if (currentState == SETTINGS)
        {
            if (IsKeyPressed(KEY_ESCAPE))
                currentState = MENU;
        }

        // DRAW

        BeginDrawing();

        if (currentState == MENU)
        {
            ClearBackground(RAYWHITE);
            DrawText("MAIN MENU", 310, 80, 30, DARKGRAY);

            const char* options[] = {
                "Start Game", "Settings", "Exit"
            };

            for (int i = 0; i < totalOptions; i++)
            {
                Rectangle button = {300, (float)(180 + i * 50), 200, 40};
                bool selected = (selectedOption == i);

                DrawRectangleRec(button, selected ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(button, 2, selected ? BLUE : DARKGRAY);
                DrawText(options[i], 330, 190 + i * 50, 20, selected ? BLUE : BLACK);
            }

            DrawText("Use UP/DOWN and ENTER, or click", 220, 380, 16, GRAY);
        }
        else if (currentState == PLAYING)
        {
            if (currentLevel == LEVEL_1_DIALOGUE)
            {
                ClearBackground(DARKBLUE);

                // Character 1 appears only during the first meeting and choice
                if (dialogueStage == MEET_CHAR1 ||
                    dialogueStage == CHAR1_CHOICE)
                    {
                        DrawRectangle(150, 40, 140, 150, SKYBLUE);
                        DrawText("CHAR 1", 180, 100, 22, WHITE);
                    }
                    
                    // Character 2 appears after the player parts ways with Character 1
                    if (dialogueStage == MEET_CHAR2 ||
                        dialogueStage == CHAR2_CHOICE)
                        {
                            DrawRectangle(510, 40, 140, 150, PURPLE);
                            DrawText("CHAR 2", 540, 100, 22, WHITE);
                        }

                DrawRectangle(40, 210, 720, 220, BLACK);
                DrawRectangleLinesEx((Rectangle){40, 210, 720, 220}, 3, RAYWHITE );

                switch (dialogueStage)
                {
                    case MEET_CHAR1:
                        DrawText("Person 1: hello, Come with me!", 60, 240, 22, WHITE);
                        DrawText("Press ENTER to continue", 60, 290, 18, YELLOW);
                        break;

                    case CHAR1_CHOICE:
                        DrawText("Will you follow Person 1?", 60, 230, 22, WHITE);

                        DrawRectangleRec(btnChar1, hover1 ? SKYBLUE : DARKGRAY);
                        DrawRectangleLinesEx(btnChar1, 2, WHITE);
                        DrawText("1. Follow Person 1", 100, 365, 18, WHITE);

                        DrawRectangleRec(btnChar2, hover2 ? RED : DARKGRAY);
                        DrawRectangleLinesEx(btnChar2, 2, WHITE);
                        DrawText("2. Refuse and leave", 440, 365, 18, WHITE);
                        break;

                    case PART_WAYS:
                        DrawText("You part ways with Person 1.", 60, 240, 22, WHITE);
                        DrawText("You walk alone into the darkness...", 60, 280, 18, LIGHTGRAY);
                        DrawText("Press ENTER to continue", 60, 330, 18, YELLOW);
                        break;

                    case MEET_CHAR2:
                        DrawText("Person 2: Hey! Are you lost?", 60, 240, 22, WHITE);
                        DrawText("Press ENTER to continue", 60, 290, 18, YELLOW);
                        break;

                    case CHAR2_CHOICE:
                        DrawText("Will you trust Person 2?", 60, 230, 22, WHITE);

                        DrawRectangleRec(btnChar1, hover1 ? SKYBLUE : DARKGRAY);
                        DrawRectangleLinesEx(btnChar1, 2, WHITE);
                        DrawText("1. Trust Person 2", 100, 365, 18, WHITE);
                        DrawRectangleRec(btnChar2, hover2 ? RED : DARKGRAY);
                        DrawRectangleLinesEx(btnChar2, 2, WHITE);
                        DrawText("2. Refuse", 440, 365, 18, WHITE);
                        break;
                }
            }
            else if (currentLevel == LEVEL_2)
            {
                ClearBackground(BLUE);
                DrawText("LEVEL 2: Your Journey Continues", 40, 40, 22, WHITE);
                DrawText(TextFormat("Score: %d / 20", currentScore), 40, 80, 20, LIGHTGRAY);
            }
            else if (currentLevel == LEVEL_3)
            {
                ClearBackground(PURPLE);
                DrawText("LEVEL 3: Approaching the Destination", 40, 40, 22, WHITE);
                DrawText(TextFormat("Score: %d / 20", currentScore), 40, 80, 20, LIGHTGRAY);
            }
            else if (currentLevel == LEVEL_COMPLETE)
            {
                ClearBackground(DARKGREEN);
                DrawText("You have reached the destination!", 230, 220, 20, LIGHTGRAY);
                DrawText("Press ENTER for the menu", 260, 320, 20, WHITE);
            }
        }
        else if (currentState == GAME_OVER)
        {
            ClearBackground(MAROON);
            DrawRectangle(100, 100, 600, 250, BLACK);
            DrawRectangleLinesEx(
                (Rectangle){100, 100, 600, 250}, 3, RED
            );

            DrawText("YOU DIED!", 310, 130, 40, RED);
            DrawText("You made the wrong choice.",
                     210, 200, 20, WHITE);
            DrawText("Press SPACE to restart",
                     260, 270, 18, LIGHTGRAY);
            DrawText("Press ESC for Main Menu",
                     260, 310, 18, GRAY);
        }
        else if (currentState == SETTINGS)
        {
            ClearBackground(RAYWHITE);
            DrawText("SETTINGS SCREEN", 270, 180, 30, ORANGE);
            DrawText("Press ESC to return to Menu",
                     250, 240, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}