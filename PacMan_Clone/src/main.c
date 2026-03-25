#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> // For atoi()

// Defining constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define USERS_FILE "users.txt"
#define MAX_USERS 200
#define NAME_LEN 25 // Max Name length
#define PASS_LEN 25 // Max password length
#define TILE_SIZE 22
#define MAP_ROWS 32
#define MAP_COLS 28
#define RIGHT_OFFSET 92
#define DOWN_OFFSET 72
#define QUEUE_MAX (MAP_ROWS * MAP_COLS)

// Initializing Map of the game 
int OGMap[MAP_ROWS][MAP_COLS] = {
    {91, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 51, 52, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 92}, // 9-->Vertical Double Line Border
    {9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9},                             // 91-->Up-Left Double Line Border
    {9, 1, 81, 80, 80, 82, 1, 81, 80, 80, 80, 82, 1, 8, 8, 1, 81, 80, 80, 80, 82, 1, 81, 80, 80, 82, 1, 9},           // 92-->Up-Right Double Line Border
    {9, 1, 8, 0, 0, 8, 1, 8, 0, 0, 0, 8, 1, 8, 8, 1, 8, 0, 0, 0, 8, 1, 8, 0, 0, 8, 1, 9},                             // 93-->Down-Left Double Line Border
    {9, 1, 83, 80, 80, 84, 1, 83, 80, 80, 80, 84, 1, 83, 84, 1, 83, 80, 80, 80, 84, 1, 83, 80, 80, 84, 1, 9},         // 94-->Down-Right Double Line Border
    {9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9},                             // 90-->Horiontal Double Line Border
    {9, 1, 81, 80, 80, 82, 1, 81, 82, 1, 81, 80, 80, 80, 80, 80, 80, 82, 1, 81, 82, 1, 81, 80, 80, 82, 1, 9},
    {9, 1, 83, 80, 80, 84, 1, 8, 8, 1, 83, 80, 80, 82, 81, 80, 80, 84, 1, 8, 8, 1, 83, 80, 80, 84, 1, 9},     // 8-->Vertical Single Line Border
    {9, 1, 1, 1, 1, 1, 1, 8, 8, 1, 1, 1, 1, 8, 8, 1, 1, 1, 1, 8, 8, 1, 1, 1, 1, 1, 1, 9},                     // 80-->Horizontal Single Line Border
    {93, 90, 90, 90, 90, 92, 1, 8, 83, 80, 80, 82, 0, 8, 8, 0, 81, 80, 80, 84, 8, 1, 91, 90, 90, 90, 90, 94}, // 81-->Up-Left Single Line Border
    {0, 0, 0, 0, 0, 9, 1, 8, 81, 80, 80, 84, 0, 83, 84, 0, 83, 80, 80, 82, 8, 1, 9, 0, 0, 0, 0, 0},           // 82-->Up-Right Single Line Border
    {0, 0, 0, 0, 0, 9, 1, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 1, 9, 0, 0, 0, 0, 0},                     // 83-->Down-Left Single Line Border
    {0, 0, 0, 0, 0, 9, 1, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 1, 9, 0, 0, 0, 0, 0},                     // 84-->Down-Right Single Line Border
    {90, 90, 90, 90, 90, 94, 1, 83, 84, 0, 71, 90, 90, 7, 7, 90, 90, 72, 0, 83, 84, 1, 93, 90, 90, 90, 90, 90},
    {10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 1, 0, 0, 0, 0, 0, 10}, // 6-->Gateway
    {10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 1, 0, 0, 0, 0, 0, 10},
    {90, 90, 90, 90, 90, 92, 1, 81, 82, 0, 73, 90, 90, 90, 90, 90, 90, 74, 0, 81, 82, 1, 91, 90, 90, 90, 90, 90}, // 0-->Blank Space
    {0, 0, 0, 0, 0, 9, 1, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 1, 9, 0, 0, 0, 0, 0},                         // 1-->Pallete
    {0, 0, 0, 0, 0, 9, 1, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 1, 9, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 9, 1, 8, 8, 0, 81, 80, 80, 80, 80, 80, 80, 82, 0, 8, 8, 1, 9, 0, 0, 0, 0, 0},
    {91, 90, 90, 90, 90, 94, 1, 83, 84, 0, 83, 80, 80, 82, 81, 80, 80, 84, 0, 83, 84, 1, 93, 90, 90, 90, 90, 92}, // 51 to 56-->Special Double Line Border Cases
    {9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9},
    {9, 1, 81, 80, 80, 82, 1, 81, 80, 80, 80, 82, 1, 8, 8, 1, 81, 80, 80, 80, 82, 1, 81, 80, 80, 82, 1, 9},
    {9, 1, 83, 80, 82, 8, 1, 83, 80, 80, 80, 84, 1, 83, 84, 1, 83, 80, 80, 80, 84, 1, 8, 81, 80, 84, 1, 9},
    {9, 1, 1, 1, 8, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 8, 1, 1, 1, 9},
    {53, 80, 82, 1, 8, 8, 1, 81, 82, 1, 81, 80, 80, 80, 80, 80, 80, 82, 1, 81, 82, 1, 8, 8, 1, 81, 80, 55}, // 7-->Ghost Door
    {54, 80, 84, 1, 83, 84, 1, 8, 8, 1, 83, 80, 80, 82, 81, 80, 80, 84, 1, 8, 8, 1, 83, 84, 1, 83, 80, 56}, // 70-->Vertical Double Line
    {9, 1, 1, 1, 1, 1, 1, 8, 8, 1, 1, 1, 1, 8, 8, 1, 1, 1, 1, 8, 8, 1, 1, 1, 1, 1, 1, 9},
    {9, 1, 81, 80, 80, 80, 80, 84, 83, 80, 80, 82, 1, 8, 8, 1, 81, 80, 80, 84, 83, 80, 80, 80, 80, 82, 1, 9},
    {9, 1, 83, 80, 80, 80, 80, 80, 80, 80, 80, 84, 1, 83, 84, 1, 83, 80, 80, 80, 80, 80, 80, 80, 80, 84, 1, 9},
    {9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9},
    {93, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 94}};
int Map[MAP_ROWS][MAP_COLS];

// Checking State of Game
typedef enum
{
    LogIn,      //0
    SignUp,
    Menu,
    Playing,
    LeaderBoard,
    Pause,
    SettingsM,
    SettingsP,
    GameOver
} State;

typedef struct
{
    Vector2 q[QUEUE_MAX];
    int front, rear;
} Queue;

typedef enum
{
    Right,
    Left,
    Up,
    Down
} Direction;

typedef struct
{
    Vector2 position;
    const float radius;
    const int Segments;
    const float MaxMouthAngle;
    const float AnimationSpeed;
    const float speed;
    const float EyeRadius;
    float currentMouthAngle;
    Direction direct;
    Color color;
} Pacman;

// Pacman Initialization
Pacman pac = {(Vector2){RIGHT_OFFSET + (13.5f * TILE_SIZE), DOWN_OFFSET + (24.5f * TILE_SIZE)}, TILE_SIZE / 2 + 5, 100, 30, 10, 150, 3, 0.0f, Right, YELLOW};
// pac.position = (Vector2){RIGHT_OFFSET + (13.5f * TILE_SIZE), DOWN_OFFSET + (24.5f * TILE_SIZE)};
// pac.radius = TILE_SIZE / 2;
// pac.Segments = 100;
// pac.MaxMouthAngle = 30; // In degrees
// pac.AnimationSpeed = 10; // Radians per second
// pac.speed = 150;         // pixels per second
// pac.EyeRadius = 3;
// pac.currentMouthAngle = 0.0f;
// pac.direct = Right;
// pac.color = YELLOW;

typedef struct
{
    Vector2 position;
    const float speed;              // = 120
    const float ghostHeadRadius;    // =  TILE_SIZE/2
    const float ghostBodyHeight;    // = 13
    const float ghostWidth;         // = TILE_SIZE
    const float legWiggleSpeed;     // = 5
    const float legWiggleMagnitude; // = 3
    const float legRadius;          // = ghostHeadRadius / 3.0f
    const float eyeRadius;          // = 3
    const float pupilRadius;        // = 1.5
    bool isBlinking;
    float blinkTimer;
    const float blinkDuration;      // = 0.15
    float blinkInterval;      // = 3
    Color ghostColor;
} Ghost;
//Ghost Initialisation
Ghost g1 = {(Vector2){RIGHT_OFFSET + 13*TILE_SIZE,DOWN_OFFSET+ 12.5*TILE_SIZE},120,TILE_SIZE/2,13,TILE_SIZE,5,3,TILE_SIZE/6.0f,3,1.5,false,0,0.15,3,RED};

typedef struct
{
    char username[NAME_LEN];
    char password[PASS_LEN];
    int score;
} User;

User users[MAX_USERS];
int userCount = 0;
int loggedInUserIndex = -1;
int framesCounter = 0;      // Used for blinking cursor

int score = 0;

State GameState = LogIn;
bool shouldQuit = false;

// --- UI Buffers ---
// Login Screen
char loginUserBuffer[NAME_LEN] = {0};
char loginPassBuffer[PASS_LEN] = {0};
int loginUserLetters = 0;
int loginPassLetters = 0;
bool loginUserActive = false;
bool loginPassActive = false;
const char *loginMessage = "";

// Register Screen
char regUserBuffer[NAME_LEN] = {0};
char regPassBuffer[PASS_LEN] = {0};
int regUserLetters = 0;
int regPassLetters = 0;
bool regUserActive = false;
bool regPassActive = false;
const char *regMessage = "";

typedef struct {
    Sound intro;
    Music bgmusic;
    Sound eating;
    Sound dead;
} Audio;
int introSound = 0;
float masterVolume = 1.0;
bool isDragging = false;

//--------------Function Prototypes----------------

// Load Users from txt file
void LoadUsers();

// Saves new user in txt file
void SaveNewUser(User user);

// Sorts all Users in txt file
void SortUsers();

// Checks if a username already exists
int FindUser(const char *username);

// Checks for a valid login
int CheckLogin(const char *username, const char *password);

// Function to Render Map
void RenderMap(Font myFont);

//Function to load Login Page
void LoadLoginPage(Font myFont,Font textFont);

//Function to load SignUp Page
void LoadSignUpPage(Font myFont,Font textFont);

// Function to load Menu
void LoadMenu(Font myFont,Font textFont);

//LeaderBoard of Menu
void LoadLeaderBoard(Font myFont,Font textFont);

// To Render Pacman
void RenderPac();

//To Render Ghost
void RenderGhost();

// For Movement of Pacman
void PacMovement(Audio *sounds);

// To get Target tile for Ghost to chase
void GetPacmanTargetTile(int *targetX, int *targetY);

// For Ghost Movement
void GhostMovement(); 

//For Score Updation
void UpdateScore(Sound *sound);

// Function for paused game
void GamePause(Font myFont);

// Checks collision with walls
bool checkWall(Vector2 position);

//Function that shows GameOver msg and returns score to update in file
void Quit(Font myFont);

//Function to check If Pacman and Ghost Collides
void Collision(Audio *sounds);

//Function to Reset Everything
void Reset();

//To Load all Audio files
void LoadAllSounds(Audio *sounds);

//Handle BackgroundSounds
void HandleSound(Audio *sounds);

//Unload all audio files
void UnloadAllSounds(Audio *sounds);

//Change volume
void VolumeSettings(Font myFont,State previousState);

//----------------------------Main---------------------------------

int main()
{
    // Initialization
    memcpy(Map,OGMap,sizeof(OGMap));
    // Initialize the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My Game");
    InitAudioDevice();
    // Comes immediately after InitWindow and ensures VSync
    SetTargetFPS(60);

    Audio sounds;
    LoadAllSounds(&sounds);
    LoadUsers();
    Font myFont = LoadFont("Resources/myFont.ttf");
    Font textFont = LoadFont("Resources/textFont.otf");

    SetExitKey(KEY_NULL);
    // Main game loop
    while (!WindowShouldClose() && !shouldQuit) // Detect window close button or ESC key
    {
        switch (GameState)
        {
            case LogIn:
            {
                BeginDrawing();
                ClearBackground(BLACK);
                LoadLoginPage(myFont,textFont);
                EndDrawing();
            }
            break;

            case SignUp:
            {
                BeginDrawing();
                ClearBackground(BLACK);
                LoadSignUpPage(myFont,textFont);
                EndDrawing();
            }
            break;

            case Menu:
            {
                BeginDrawing();
                ClearBackground(BLACK);
                LoadMenu(myFont,textFont);
                EndDrawing();
            }
            break;

            case LeaderBoard:
            {
                BeginDrawing();
                ClearBackground(BLACK);
                LoadLeaderBoard(myFont,textFont);
                EndDrawing();
            }
            break;

            case Playing:
            {
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    GameState = Pause;
                }
                BeginDrawing();
                ClearBackground(BLACK);
                RenderMap(myFont);
                HandleSound(&sounds);
                RenderPac();
                RenderGhost();
                PacMovement(&sounds);
                GhostMovement();
                Collision(&sounds);
                EndDrawing();
            }
            break;

            case Pause:
            {
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    GameState = Playing;
                }
                BeginDrawing();
                ClearBackground(BLACK);
                RenderMap(myFont);
                RenderPac();
                RenderGhost();
                GamePause(myFont);
                EndDrawing();
                break;
            }

            case SettingsM:
            {
                BeginDrawing();
                ClearBackground(BLACK);
                VolumeSettings(myFont,Menu);
                EndDrawing();
                break;
            }

            case SettingsP:
            {
                BeginDrawing();
                ClearBackground(BLACK);
                VolumeSettings(myFont,Pause);
                EndDrawing();
                break;
            }

            case GameOver:
            {
                BeginDrawing();
                ClearBackground(BLACK);
                Quit(myFont);
                EndDrawing();
                break;
            }
        }
    }

    UnloadFont(myFont);
    UnloadFont(textFont);
    UnloadAllSounds(&sounds);
    // De-Initialization
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void LoadAllSounds(Audio *sounds){
    sounds->intro = LoadSound("Resources/pacman_intro.wav");
    sounds->bgmusic = LoadMusicStream("Resources/pacman.ogg");
    sounds->bgmusic.looping = true;
    SetMusicVolume(sounds->bgmusic,0.7);
    sounds->eating = LoadSound("Resources/pacman_eating.wav");
    SetSoundVolume(sounds->eating,0.4);
    sounds->dead = LoadSound("Resources/pacman_dead.wav");
}

void HandleSound(Audio *sounds){
    if(introSound == 0){
        PlaySound(sounds->intro);
        introSound = 1;
    }
    if(!IsSoundPlaying(sounds->intro) && !IsMusicStreamPlaying(sounds->bgmusic)){
        PlayMusicStream(sounds->bgmusic);
    }
    if(IsMusicStreamPlaying(sounds->bgmusic)){
        UpdateMusicStream(sounds->bgmusic);
    }
}

void UnloadAllSounds(Audio *sounds){
    UnloadSound(sounds->intro);
    UnloadMusicStream(sounds->bgmusic);
    UnloadSound(sounds->eating);
    UnloadSound(sounds->dead);
}

void VolumeSettings(Font myFont,State previousState){
    Vector2 mousePos = GetMousePosition();

    Vector2 MeasureBack = MeasureTextEx(myFont,"Back",30,1.5);
    Rectangle backButton = { SCREEN_WIDTH/2 - MeasureBack.x/2, 600, MeasureBack.x, MeasureBack.y };

    // The long background bar
    Rectangle sliderTrack = { 100, 200, 600, 10 }; 
    
    // The handle that moves
    Rectangle sliderKnob = { sliderTrack.x, sliderTrack.y - 10, 20, 30 };
    
    // A larger, invisible-but-clickable area for the track
    Rectangle trackClickArea = { sliderTrack.x, sliderTrack.y - 10, sliderTrack.width, 30 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, trackClickArea))
        {
            isDragging = true;
        }

        // Stop dragging when mouse is released
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            isDragging = false;
        }

        // Update volume if dragging
        if (isDragging)
        {
            // 1. Get mouse X, clamped within the track's bounds
            float newMouseX = Clamp(mousePos.x, sliderTrack.x, sliderTrack.x + sliderTrack.width);
            
            // 2. Convert the X position to a 0.0f - 1.0f volume value
            masterVolume = (newMouseX - sliderTrack.x) / sliderTrack.width;
            
            // 3. Set the master volume
            SetMasterVolume(masterVolume);
        }

        // Update the knob's visual position based on the volume
        // We center the knob on the correct position
        sliderKnob.x = sliderTrack.x + (masterVolume * sliderTrack.width) - (sliderKnob.width / 2);
        sliderKnob.y = sliderTrack.y + (sliderTrack.height / 2) - (sliderKnob.height / 2);
        
        DrawTextEx(myFont,"Volume Settings",(Vector2){SCREEN_WIDTH/2 - MeasureTextEx(myFont,"Volume Settings",40,1.5).x/2,50},40,1.5,YELLOW);

        // Draw the slider track
        DrawRectangleRec(sliderTrack, GRAY);
        
        // Draw a "filled" track to show current volume
        DrawRectangle(sliderTrack.x, sliderTrack.y, sliderKnob.x + (sliderKnob.width / 2) - sliderTrack.x, sliderTrack.height, BLUE);

        // Draw the knob
        DrawRectangleRec(sliderKnob, isDragging ? RED : DARKGRAY);
            
        // Draw the volume value
        DrawText(TextFormat("%.0f%%", masterVolume * 100), sliderKnob.x, sliderTrack.y - 30, 20, GREEN);

        // Back Button
        bool backHover = CheckCollisionPointRec(mousePos, backButton);
        // Check "Back" button
        DrawTextEx(myFont,"Back",(Vector2){backButton.x,backButton.y},30,1.5,backHover ? GREEN : YELLOW);
        if (CheckCollisionPointRec(mousePos, backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            GameState = previousState;
        }
}

void LoadUsers()
{
    FILE *file = fopen(USERS_FILE, "r");
    if (file != NULL)
    {
        // Read users until EOF or array is full
        while (userCount < MAX_USERS && fscanf(file, "%s %s %d",
                                               users[userCount].username,
                                               users[userCount].password,
                                               &users[userCount].score) == 3)
        {
            userCount++;
        }
        fclose(file);
    }
}

void SaveNewUser(User user)
{
    FILE *file = fopen(USERS_FILE, "a");
    if (file != NULL)
    {
        fprintf(file, "%s %s %d\n", user.username, user.password, user.score);
        fclose(file);
    }
}

void SortUsers()
{
    for (int i = 0; i < userCount - 1; i++)
    {
        for (int j = 0; j < userCount - i - 1; j++)
        {
            if (users[j].score < users[j + 1].score)
            {
                User tmp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = tmp;

                if(loggedInUserIndex == j){
                    loggedInUserIndex++;
                }
                if(loggedInUserIndex == j+1){
                    loggedInUserIndex--;
                }
            }
        }
    }

    FILE *file = fopen(USERS_FILE, "w");
    if (file != NULL)
    {
        for (int i = 0; i < userCount; i++)
        {
            fprintf(file, "%s %s %d\n",
                    users[i].username,
                    users[i].password,
                    users[i].score);
        }
        fclose(file);
    }
}

int FindUser(const char *username)
{
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            return i; // Found at index i
        }
    }
    return -1; // Not found
}

int CheckLogin(const char *username, const char *password)
{
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0)
        {
            return i; // Found, return user index
        }
    }
    return -1; // Invalid login
}

bool checkWall(Vector2 position)
{
    int col = (int)((position.x - RIGHT_OFFSET) / TILE_SIZE);
    int row = (int)((position.y - DOWN_OFFSET) / TILE_SIZE);

    if (Map[row][col] > 6)
    {
        switch (pac.direct)
        {
        case Right:
            if (position.x < ((col * TILE_SIZE) + RIGHT_OFFSET + 4))
                return false;
            break;

        case Left:
            if (position.x > ((col + 1) * TILE_SIZE + RIGHT_OFFSET - 4))
                return false;
            break;

        case Down:
            if (position.y < (row * TILE_SIZE + DOWN_OFFSET + 4))
                return false;
            break;

        case Up:
            if (position.y > ((row + 1) * TILE_SIZE + DOWN_OFFSET - 4))
                return false;
            break;
        }
    }
    else
    {
        return false;
    }
    return true;
}

void LoadSignUpPage(Font myFont , Font textFont)
{
    framesCounter++; // For blinking cursor
    Vector2 mousePos = GetMousePosition(); // Get mouse position once per frame

    const Vector2 MeasureSign_Up = MeasureTextEx(myFont,"Sign-Up",40,1.5);
    const Vector2 MeasureLog_In = MeasureTextEx(myFont,"Log-In",30,1.5);
    const Vector2 MeasureSignUp = MeasureTextEx(myFont,"Sign Up",30,1);

    const float boxWidth = 300;
    const float boxHeight = 40;
    const float fontSizeTitle = 40.0f;
    const float fontSizeMenu = 30.0f;
    const float fontSizeText = 20.0f;

    Rectangle regUserBox = { SCREEN_WIDTH/2 - boxWidth/2, 250, boxWidth, boxHeight };
    Rectangle regPassBox = { SCREEN_WIDTH/2 - boxWidth/2, 370, boxWidth, boxHeight };
    Rectangle registerButton = { SCREEN_WIDTH/2 - MeasureSignUp.x/2, 500, MeasureSignUp.x, MeasureSignUp.y };
    Rectangle Login = { SCREEN_WIDTH/2 - MeasureLog_In.x/2, 570, MeasureLog_In.x, MeasureLog_In.y };

    DrawTextEx(myFont, "Sign-Up", (Vector2){ SCREEN_WIDTH/2 - MeasureSign_Up.x/2, 100 }, fontSizeTitle, 1.5, YELLOW);
    Color LoginColor = YELLOW;
    if(CheckCollisionPointRec(mousePos, Login)) LoginColor = BLUE;
    DrawTextEx(myFont, "Log-In", (Vector2){ SCREEN_WIDTH/2 - MeasureLog_In.x/2, Login.y }, fontSizeMenu, 1.5, LoginColor);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        regUserActive = CheckCollisionPointRec(mousePos, regUserBox);
        regPassActive = CheckCollisionPointRec(mousePos, regPassBox);
    }

    if (regUserActive)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (regUserLetters < NAME_LEN - 1))
            {
                regUserBuffer[regUserLetters] = (char)key;
                regUserBuffer[regUserLetters + 1] = '\0';
                regUserLetters++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (regUserLetters > 0)
                regUserLetters--;
            regUserBuffer[regUserLetters] = '\0';
        }
    }

    if (regPassActive)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (regPassLetters < PASS_LEN - 1))
            {
                regPassBuffer[regPassLetters] = (char)key;
                regPassBuffer[regPassLetters + 1] = '\0';
                regPassLetters++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (regPassLetters > 0)
                regPassLetters--;
            regPassBuffer[regPassLetters] = '\0';
        }
    }

    if (CheckCollisionPointRec(mousePos, registerButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (FindUser(regUserBuffer) != -1)
        {
            regMessage = "USERNAME ALREADY EXISTS";
        }
        else if (regUserLetters == 0 || regPassLetters == 0)
        {
            regMessage = "USERNAME/PASSWORD CANNOT BE EMPTY";
        }
        else if (userCount >= MAX_USERS)
        {
            regMessage = "USER DATABASE FULL";
        }
        else
        {
            User newUser;
            strcpy(newUser.username, regUserBuffer);
            strcpy(newUser.password, regPassBuffer);
            newUser.score = 0;

            users[userCount] = newUser;
            userCount++;

            SaveNewUser(newUser);

            regMessage = "REGISTRATION SUCCESSFUL!";
        }
    }

    if (CheckCollisionPointRec(mousePos, Login) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        GameState = LogIn;
        regMessage = "";
        loginMessage = "";
    }

    DrawTextEx(myFont, "NEW USERNAME", (Vector2){regUserBox.x, regUserBox.y - 30}, fontSizeMenu, 1, YELLOW);
    DrawRectangleRec(regUserBox, BLACK);
    DrawRectangleLinesEx(regUserBox, 2, regUserActive ? BLUE : DARKGRAY);
    DrawTextEx(textFont, regUserBuffer, (Vector2){regUserBox.x + 10, regUserBox.y + 10}, fontSizeText, 1.5, WHITE);

    char passMask[PASS_LEN] = {0};
    for (int i = 0; i < regPassLetters; i++) passMask[i] = '*';
    DrawTextEx(myFont, "NEW PASSWORD", (Vector2){regPassBox.x, regPassBox.y - 30}, fontSizeMenu, 1, YELLOW);
    DrawRectangleRec(regPassBox, BLACK);
    DrawRectangleLinesEx(regPassBox, 2, regPassActive ? BLUE : DARKGRAY);
    DrawTextEx(GetFontDefault(), passMask, (Vector2){regPassBox.x + 10, regPassBox.y + 10}, fontSizeText, 1.5, WHITE);

    // --- CURSOR CHANGE 3 ---
    if (regUserActive && ((framesCounter / 30) % 2 == 0))
    {
        Vector2 textSize = MeasureTextEx(textFont, regUserBuffer, fontSizeText, 1.5);
        DrawTextEx(textFont, "|", (Vector2){regUserBox.x + 10 + (int)textSize.x, regUserBox.y + 10}, fontSizeText, 1.5, WHITE);
    }
    // --- CURSOR CHANGE 4 ---
    if (regPassActive && ((framesCounter / 30) % 2 == 0))
    {
        Vector2 textSize = MeasureTextEx(GetFontDefault(), passMask, fontSizeText, 1.5);
        DrawTextEx(GetFontDefault(), "|", (Vector2){regPassBox.x + 10 + (int)textSize.x, regPassBox.y + 10}, fontSizeText, 1.5, WHITE);
    }

    bool SignUpHover = CheckCollisionPointRec(mousePos, registerButton);
    DrawTextEx(myFont, "Sign Up", (Vector2){SCREEN_WIDTH/2 - MeasureSignUp.x/2, registerButton.y}, 30, 1, SignUpHover ? BLUE : YELLOW);

    bool isSuccess = (strcmp(regMessage, "REGISTRATION SUCCESSFUL!") == 0);
    DrawText(regMessage, SCREEN_WIDTH/2 - MeasureText(regMessage, 20)/2, 460, 20, isSuccess ? GREEN : RED);
}

void LoadLoginPage(Font myFont, Font textFont)
{
    framesCounter++; // For blinking cursor
    Vector2 mousePos = GetMousePosition(); // Get mouse position once per frame

    const Vector2 MeasureLog_in = MeasureTextEx(myFont,"Log-In",40,1.5);
    const Vector2 MeasureSign_Up = MeasureTextEx(myFont,"Sign-Up",30,1.5);
    const Vector2 MeasureLogIn = MeasureTextEx(myFont,"Log In",30,1);

    //int offset = 25;

    const float boxWidth = 300;
    const float boxHeight = 40;
    const float fontSizeTitle = 40.0f;
    const float fontSizeMenu = 30.0f;
    const float fontSizeText = 20.0f;

    Rectangle loginUserBox = { SCREEN_WIDTH/2 - boxWidth/2, 250, boxWidth, boxHeight };
    Rectangle loginPassBox = { SCREEN_WIDTH/2 - boxWidth/2, 370, boxWidth, boxHeight };
    Rectangle loginButton = { SCREEN_WIDTH/2 - MeasureLogIn.x/2, 500, MeasureLogIn.x, MeasureLogIn.y };
    Rectangle Sign_Up = {  SCREEN_WIDTH/2 - MeasureSign_Up.x/2, 570, MeasureSign_Up.x, MeasureSign_Up.y };

    DrawTextEx(myFont, "Log-In", (Vector2){ SCREEN_WIDTH/2 - MeasureLog_in.x/2, 100 }, 40, 1.5, ORANGE);
    Color SignUpColor = YELLOW;
    if(CheckCollisionPointRec(mousePos, Sign_Up)) SignUpColor = BLUE;
    DrawTextEx(myFont, "Sign-Up", (Vector2){ SCREEN_WIDTH/2 - MeasureSign_Up.x/2, Sign_Up.y }, 30, 1.5, SignUpColor);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        loginUserActive = CheckCollisionPointRec(mousePos, loginUserBox);
        loginPassActive = CheckCollisionPointRec(mousePos, loginPassBox);
    }

    DrawTextEx(myFont, "USERNAME", (Vector2){loginUserBox.x, loginUserBox.y - 30}, 30, 1, YELLOW);
    DrawRectangleRec(loginUserBox, BLACK);
    DrawRectangleLinesEx(loginUserBox, 2, loginUserActive ? BLUE : DARKGRAY);
    DrawTextEx(textFont, loginUserBuffer, (Vector2){loginUserBox.x + 10, loginUserBox.y + 10}, 20, 1.5, WHITE);

    char passMask[PASS_LEN] = {0};
    for (int i = 0; i < loginPassLetters; i++) passMask[i] = '*';
    DrawTextEx(myFont, "PASSWORD", (Vector2){loginPassBox.x, loginPassBox.y - 30}, 30, 1, YELLOW);
    DrawRectangleRec(loginPassBox, BLACK);
    DrawRectangleLinesEx(loginPassBox, 2, loginPassActive ? BLUE : DARKGRAY);
    DrawTextEx(GetFontDefault(), passMask, (Vector2){loginPassBox.x + 10, loginPassBox.y + 10}, 20, 1.5, WHITE);

    // --- CURSOR CHANGE 1 ---
    if (loginUserActive && ((framesCounter / 30) % 2 == 0)){
        Vector2 textSize = MeasureTextEx(textFont, loginUserBuffer, 20, 1.5);
        DrawTextEx(textFont, "|", (Vector2){loginUserBox.x + 10 + (int)textSize.x + 2, loginUserBox.y + 10}, 20, 1.5, WHITE);
    }
    // --- CURSOR CHANGE 2 ---
    if (loginPassActive && ((framesCounter / 30) % 2 == 0)){
        Vector2 textSize = MeasureTextEx(GetFontDefault(), passMask, 20, 1.5);
        DrawTextEx(textFont, "|", (Vector2){loginPassBox.x + 10 + (int)textSize.x + 2, loginPassBox.y + 10}, 20, 1.5, WHITE);
    }

    if (loginUserActive){
        int key = GetCharPressed();
        while (key > 0){
            if ((key >= 32) && (key <= 125) && (loginUserLetters < NAME_LEN - 1)){
                loginUserBuffer[loginUserLetters] = (char)key;
                loginUserBuffer[loginUserLetters + 1] = '\0';
                loginUserLetters++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)){
            if (loginUserLetters > 0) loginUserLetters--;
            loginUserBuffer[loginUserLetters] = '\0';
        }
    }

    if (loginPassActive){
        int key = GetCharPressed();
        while (key > 0){
            if ((key >= 32) && (key <= 125) && (loginPassLetters < PASS_LEN - 1)){
                loginPassBuffer[loginPassLetters] = (char)key;
                loginPassBuffer[loginPassLetters + 1] = '\0';
                loginPassLetters++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)){
            if (loginPassLetters > 0) loginPassLetters--;
            loginPassBuffer[loginPassLetters] = '\0';
        }
    }

    if (CheckCollisionPointRec(mousePos, loginButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        loggedInUserIndex = CheckLogin(loginUserBuffer, loginPassBuffer);
        if (loggedInUserIndex != -1)
        {
            GameState = Menu;
            loginMessage = "";
        }
        else
        {
            loginMessage = "INVALID USERNAME OR PASSWORD";
        }
    }

    if (CheckCollisionPointRec(mousePos, Sign_Up) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        GameState = SignUp;
        loginMessage = "";
        regMessage = "";
    }

    bool loginHover = CheckCollisionPointRec(mousePos, loginButton);
    DrawTextEx(myFont, "Log In", (Vector2){SCREEN_WIDTH/2 - MeasureLogIn.x/2, loginButton.y}, 30, 1, loginHover ? BLUE : YELLOW);

    DrawText(loginMessage, SCREEN_WIDTH/2 - MeasureText(loginMessage, 20)/2, 460, 20, RED);
}

void GamePause(Font myFont)
{
    // Draw the dimming overlay
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 200});

    // Get mouse position for hover checks
    Vector2 mousePos = GetMousePosition();

    // Define text and measure
    Vector2 Txtmeasure = MeasureTextEx(myFont, "Pause", 60, 1.5);
    Vector2 resumeMeasure = MeasureTextEx(myFont, "Resume", 40, 1);
    Vector2 settingsMeasure = MeasureTextEx(myFont, "Settings", 40, 1);
    Vector2 quitMeasure = MeasureTextEx(myFont, "Quit", 40, 1);

    // Define button rectangles
    float pauseTextY = (SCREEN_HEIGHT / 2) - (Txtmeasure.y / 2) - 120; // Moved up
    float buttonHeight = resumeMeasure.y;
    float buttonSpacing = 20;

    Rectangle resumeButton = {(SCREEN_WIDTH / 2) - (resumeMeasure.x / 2), pauseTextY + Txtmeasure.y + 60, resumeMeasure.x, buttonHeight};
    Rectangle settingsButton = {(SCREEN_WIDTH / 2) - (settingsMeasure.x / 2), resumeButton.y + resumeButton.height + buttonSpacing, settingsMeasure.x, buttonHeight}; // <-- NEW
    Rectangle quitButton = {(SCREEN_WIDTH / 2) - (quitMeasure.x / 2), settingsButton.y + settingsButton.height + buttonSpacing, quitMeasure.x, buttonHeight};         // <-- Adjusted Y

    // Check for button clicks (Update logic)
    if (CheckCollisionPointRec(mousePos, resumeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        GameState = Playing; // Resume the game
    }
    if (CheckCollisionPointRec(mousePos, settingsButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        GameState = SettingsP;
    }
    if (CheckCollisionPointRec(mousePos, quitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        GameState = GameOver; // Quit the game
    }

    // --- Draw all elements ---

    // Draw "Pause" title
    DrawTextEx(myFont, "Pause", (Vector2){(SCREEN_WIDTH / 2) - (Txtmeasure.x / 2), pauseTextY}, 60, 1.5, WHITE);

    // Draw "Resume" Button
    Color resumeTxtColor = GREEN;
    if (CheckCollisionPointRec(mousePos, resumeButton))
        resumeTxtColor = ORANGE;
    DrawTextEx(myFont, "Resume", (Vector2){resumeButton.x, resumeButton.y + (resumeButton.height - 20) / 2}, 40, 1, resumeTxtColor);

    // Draw "Settings" Button // <-- NEW
    Color settingsTxtColor = GREEN;
    if (CheckCollisionPointRec(mousePos, settingsButton))
        settingsTxtColor = ORANGE;
    DrawTextEx(myFont, "Settings", (Vector2){settingsButton.x, settingsButton.y + (settingsButton.height - 20) / 2}, 40, 1, settingsTxtColor);

    // Draw "Quit Game" Button
    Color quitTxtColor = GREEN;
    if (CheckCollisionPointRec(mousePos, quitButton))
        quitTxtColor = RED;
    DrawTextEx(myFont, "Quit", (Vector2){quitButton.x, quitButton.y + (quitButton.height - 20) / 2}, 40, 1, quitTxtColor);
}

void PacMovement(Audio *sounds)
{
    float deltaTime = GetFrameTime();

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        pac.direct = Right;
        if (!checkWall((Vector2){pac.position.x + pac.radius + pac.speed * deltaTime, pac.position.y}))
        {
            pac.position.x += pac.speed * deltaTime;
        }
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        pac.direct = Left;
        if (!checkWall((Vector2){pac.position.x - pac.radius - pac.speed * deltaTime, pac.position.y}))
        {
            pac.position.x -= pac.speed * deltaTime;
        }
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        pac.direct = Down;
        if (!checkWall((Vector2){pac.position.x, pac.position.y + pac.radius + pac.speed * deltaTime}))
        {
            pac.position.y += pac.speed * deltaTime;
        }
    }
    else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        pac.direct = Up;
        if (!checkWall((Vector2){pac.position.x, pac.position.y - pac.radius - pac.speed * deltaTime}))
        {
            pac.position.y -= pac.speed * deltaTime;
        }
    }
    UpdateScore(&(sounds->eating));
}

//Functions for BFS(Ghost Movement)
void QueueInit(Queue *qu) { qu->front = qu->rear = 0; }
int QueueIsEmpty(Queue *qu) { return qu->front == qu->rear; }
void QueuePush(Queue *qu, Vector2 val)
{
    if (qu->rear < QUEUE_MAX)
        qu->q[qu->rear++] = val;
}
Vector2 QueuePop(Queue *qu) { return qu->q[qu->front++]; }

void GetPacmanTargetTile(int *targetX, int *targetY)
{
    int currX = (pac.position.x - RIGHT_OFFSET) / TILE_SIZE;
    int currY = (pac.position.y - DOWN_OFFSET) / TILE_SIZE;
    int lookahead = 0; // Number of tiles ahead to target

    // int dirX = pacDirX;
    // int dirY = pacDirY;
    int dirX,dirY;

    if(pac.direct == Right){
        dirX = 1;
        dirY = 0;
    } 
    else if(pac.direct == Left){
        dirX = -1;
        dirY = 0;
    }
    else if(pac.direct == Up){
        dirX = 0;
        dirY = -1;
    } 
    else if(pac.direct == Down){
        dirX = 0;
        dirY = 1;
    }
    else{
        dirX = 0;
        dirY = 0;
    }
    *targetX = currX;
    *targetY = currY;
    for (int step = 0; step < lookahead; step++)
    {
        int nx = *targetX + dirX;
        int ny = *targetY + dirY;
        if (nx >= 0 && nx < MAP_COLS && ny >= 0 && ny < MAP_ROWS &&
            (Map[ny][nx] == 0 || Map[ny][nx] == 1))
        {
            *targetX = nx;
            *targetY = ny;
        }
        else
        {
            break;
        }
    }
}

void GhostMovement(){
    float deltaTime = GetFrameTime();

    int ghostTileX = (g1.position.x - RIGHT_OFFSET) / TILE_SIZE;
    int ghostTileY = (g1.position.y - DOWN_OFFSET) / TILE_SIZE;

    int targetTileX, targetTileY;
    GetPacmanTargetTile(&targetTileX, &targetTileY);

    int visited[MAP_ROWS][MAP_COLS] = {0};
    Vector2 parent[MAP_ROWS][MAP_COLS];
    Queue qu;
    QueueInit(&qu);

    QueuePush(&qu, (Vector2){ghostTileX, ghostTileY});
    visited[ghostTileY][ghostTileX] = 1;
    parent[ghostTileY][ghostTileX] = (Vector2){-1, -1};

    int found = 0;
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    while (!QueueIsEmpty(&qu) && !found)
    {
        Vector2 cur = QueuePop(&qu);
        if (cur.x == targetTileX && cur.y == targetTileY)
        {
            found = 1;
            break;
        }
        for (int di = 0; di < 4; di++)
        {
            int nx = cur.x + dirs[di][0];
            int ny = cur.y + dirs[di][1];
            if (nx >= 0 && nx < MAP_COLS && ny >= 0 && ny < MAP_ROWS && !visited[ny][nx] && (Map[ny][nx] == 0 || Map[ny][nx] == 1))
            {
                QueuePush(&qu, (Vector2){nx, ny});
                visited[ny][nx] = 1;
                parent[ny][nx] = cur;
            }
        }
    }

    int nextGX = ghostTileX, nextGY = ghostTileY;
    if (found)
    {
        int tx = targetTileX, ty = targetTileY;
        Vector2 prev;
        while (!(tx == ghostTileX && ty == ghostTileY))
        {
            prev = parent[ty][tx];
            if (prev.x == ghostTileX && prev.y == ghostTileY)
                break;
            tx = prev.x;
            ty = prev.y;
        }
        nextGX = tx;
        nextGY = ty;
    }
    int tX = nextGX * TILE_SIZE + RIGHT_OFFSET + (TILE_SIZE / 2);
    int tY = nextGY * TILE_SIZE + DOWN_OFFSET + (TILE_SIZE / 2);
    if (g1.position.x < tX)
        g1.position.x += g1.speed*deltaTime;
    if (g1.position.x > tX)
        g1.position.x -= g1.speed*deltaTime;
    if (g1.position.y < tY)
        g1.position.y += g1.speed*deltaTime;
    if (g1.position.y > tY)
        g1.position.y -= g1.speed*deltaTime;
} 

void UpdateScore(Sound *sound){
    int col = (int)((pac.position.x - RIGHT_OFFSET) / TILE_SIZE);
    int row = (int)((pac.position.y - DOWN_OFFSET) / TILE_SIZE);

    if(Map[row][col] == 1){
        score+=10;
        Map[row][col] = 0;
        PlaySound(*sound);
    }
}

void RenderPac()
{
    float time = (float)GetTime();
    if (GameState != Pause)
    {
        pac.currentMouthAngle = ((sinf(time * pac.AnimationSpeed) + 1.0f) / 2.0f) * pac.MaxMouthAngle;
    }
    else
    {
        pac.currentMouthAngle = pac.MaxMouthAngle;
    }

    float baseRoation = 0;
    switch (pac.direct)
    {
    case Right:
        baseRoation = 0.0f;
        break;

    case Left:
        baseRoation = 180.0f;
        break;

    case Up:
        baseRoation = 270.0f;
        break;

    case Down:
        baseRoation = 90.0f;
        break;
    }
    float startAngle = baseRoation + pac.currentMouthAngle;
    float endAngle = baseRoation + 360.0f - pac.currentMouthAngle;

    // Draw the Pac-Man circle sector
    DrawCircleSector(pac.position, pac.radius, startAngle, endAngle, pac.Segments, pac.color);

    float eyeOffsetX = pac.radius * 0.5f * cosf(DEG2RAD * (baseRoation + 270));
    float eyeOffsetY = pac.radius * 0.5f * sinf(DEG2RAD * (baseRoation + 270));

    DrawCircle(pac.position.x + eyeOffsetX, pac.position.y + eyeOffsetY, pac.EyeRadius, BLACK);
}

void RenderGhost(){
    float time = (float)GetTime();
    float deltaTime = GetFrameTime();

    float wiggleOffset1 = ((sinf(time * g1.legWiggleSpeed) + 1.0f) / 2.0f) * g1.legWiggleMagnitude;
    float wiggleOffset2 = ((sinf(time * g1.legWiggleSpeed + 2.0f) + 1.0f) / 2.0f) * g1.legWiggleMagnitude;
    float wiggleOffset3 = ((sinf(time * g1.legWiggleSpeed + 4.0f) + 1.0f) / 2.0f) * g1.legWiggleMagnitude;

    g1.blinkTimer += deltaTime;

    if (g1.isBlinking)
    {
        // If eyes are "closed", check if it's time to "open" them
        if (g1.blinkTimer >= g1.blinkDuration)
        {
            g1.isBlinking = false;
            g1.blinkTimer = 0.0f; // Reset timer
            // Set a new random time for the next blink
            g1.blinkInterval = (float)GetRandomValue(200, 500) / 100.0f; // 2.0 to 5.0 seconds
        }
    }
    else
    {
        // If eyes are "open", check if it's time to "close" them
        if (g1.blinkTimer >= g1.blinkInterval)
        {
            g1.isBlinking = true;
            g1.blinkTimer = 0.0f; // Reset timer
        }
    }

    // 1. Draw Head (Circle)
    DrawCircle(g1.position.x, g1.position.y, g1.ghostHeadRadius, g1.ghostColor);

    // 2. Draw Body (Rectangle)
    DrawRectangle(g1.position.x - g1.ghostHeadRadius, g1.position.y,g1.ghostWidth, g1.ghostBodyHeight, g1.ghostColor);

    // 3. Draw Wiggling Legs (Circles)
    float legBaseY = g1.position.y + g1.ghostBodyHeight;
    DrawCircle(g1.position.x - g1.ghostWidth / 2 + g1.legRadius, legBaseY + wiggleOffset1, g1.legRadius, g1.ghostColor);
    DrawCircle(g1.position.x, legBaseY + wiggleOffset2, g1.legRadius, g1.ghostColor);
    DrawCircle(g1.position.x + g1.ghostWidth / 2 - g1.legRadius, legBaseY + wiggleOffset3, g1.legRadius, g1.ghostColor);

    // --- 4. NEW: Draw Eyes (Blinking) ---
    Vector2 leftEyeCenter = {g1.position.x - g1.ghostHeadRadius / 2, g1.position.y - g1.ghostHeadRadius / 2.5f};
    Vector2 rightEyeCenter = {g1.position.x + g1.ghostHeadRadius / 2, g1.position.y - g1.ghostHeadRadius / 2.5f};

    if (g1.isBlinking)
    {
        // Draw "closed" eyes (as simple horizontal lines)
        DrawLineEx((Vector2){leftEyeCenter.x - g1.eyeRadius / 1.5f, leftEyeCenter.y},(Vector2){leftEyeCenter.x + g1.eyeRadius / 1.5f, leftEyeCenter.y},1.0f, BLACK);

        DrawLineEx((Vector2){rightEyeCenter.x - g1.eyeRadius / 1.5f, rightEyeCenter.y},(Vector2){rightEyeCenter.x + g1.eyeRadius / 1.5f, rightEyeCenter.y},1.0f, BLACK);
    }
    else
    {
        // Draw "open" eyes
        DrawCircleV(leftEyeCenter, g1.eyeRadius, WHITE);
        DrawCircleV(leftEyeCenter, g1.pupilRadius, BLACK); // Pupil

        DrawCircleV(rightEyeCenter, g1.eyeRadius, WHITE);
        DrawCircleV(rightEyeCenter, g1.pupilRadius, BLACK); // Pupil
    }
}

void LoadMenu(Font myFont,Font textFont)
{
    const Vector2 MeasureMGM = MeasureTextEx(myFont, "MY GAME MENU", 60, 1.5);
    const Vector2 MeasureSG = MeasureTextEx(myFont, "Start Game", 40, 1);
    const Vector2 MeasureLB = MeasureTextEx(myFont, "LeaderBoard", 40, 1);
    const Vector2 MeasureSt = MeasureTextEx(myFont, "Settings", 40, 1);
    const Vector2 MeasureQG = MeasureTextEx(myFont, "Quit Game", 40, 1);

    const int NumberOfButton = 4;
    const float buttonSpacing = 40;
    const float buttonHeight = MeasureSG.y;
    const float Totalheight = (buttonHeight * NumberOfButton) + (buttonSpacing * (NumberOfButton - 1));
    const float StartY = (SCREEN_HEIGHT - Totalheight) * 0.67;

    Rectangle StartGame = {(SCREEN_WIDTH / 2) - (MeasureSG.x / 2), StartY, MeasureSG.x, buttonHeight};
    Rectangle Setting = {(SCREEN_WIDTH / 2) - (MeasureSt.x / 2), StartY + (buttonHeight + buttonSpacing), MeasureSt.x, buttonHeight};
    Rectangle Leader_Board = {(SCREEN_WIDTH / 2) - (MeasureLB.x / 2), StartY + 2*(buttonHeight + buttonSpacing), MeasureLB.x, buttonHeight};
    Rectangle QuitGame = {(SCREEN_WIDTH / 2) - (MeasureQG.x / 2), StartY + 3 * (buttonHeight + buttonSpacing), MeasureQG.x, buttonHeight};

    Vector2 mousePos = GetMousePosition();

    // Check "Start Game" button
    if (CheckCollisionPointRec(mousePos, StartGame) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        GameState = Playing;
    }

    // Check "Leader Board" button
    if (CheckCollisionPointRec(mousePos, Leader_Board) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        GameState = LeaderBoard;
    }

    // Check "Settings" button
    if (CheckCollisionPointRec(mousePos, Setting) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        GameState = SettingsM;
    }

    // Check "Quit Game" button
    if (CheckCollisionPointRec(mousePos, QuitGame) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        shouldQuit = true;
    }

    // Title
    DrawTextEx(myFont, "MY GAME MENU", (Vector2){(SCREEN_WIDTH / 2) - MeasureMGM.x / 2, 150}, 60, 1.5, YELLOW);

    // StartGame Button
    Color StarttxtColor = YELLOW;
    if (CheckCollisionPointRec(mousePos, StartGame))
        StarttxtColor = ORANGE;
    DrawRectangleRec(StartGame, BLACK);
    DrawTextEx(myFont, "Start Game", (Vector2){(SCREEN_WIDTH / 2) - (MeasureSG.x / 2), StartGame.y + (StartGame.height - 20) / 2}, 40, 1, StarttxtColor);

    // LeaderBoard Button
    Color LeaderboardColor = YELLOW;
    if (CheckCollisionPointRec(mousePos, Leader_Board))
        LeaderboardColor = ORANGE;
    DrawRectangleRec(Leader_Board, BLACK);
    DrawTextEx(myFont, "LeaderBoard", (Vector2){(SCREEN_WIDTH / 2) - (MeasureLB.x / 2), Leader_Board.y + (StartGame.height - 20) / 2}, 40, 1, LeaderboardColor);

    // Settings Button
    Color SettingtxtColor = YELLOW;
    if (CheckCollisionPointRec(mousePos, Setting))
        SettingtxtColor = ORANGE;
    DrawRectangleRec(Setting, BLACK);
    DrawTextEx(myFont, "Settings", (Vector2){(SCREEN_WIDTH / 2) - (MeasureSt.x / 2), Setting.y + (Setting.height - 20) / 2}, 40, 1, SettingtxtColor);

    // Quit Game Button
    Color QuittxtColor = YELLOW;
    if (CheckCollisionPointRec(mousePos, QuitGame))
        QuittxtColor = RED;
    DrawRectangleRec(QuitGame, BLACK);
    DrawTextEx(myFont, "Quit Game", (Vector2){(SCREEN_WIDTH / 2) - (MeasureQG.x / 2), QuitGame.y + (QuitGame.height - 20) / 2}, 40, 1, QuittxtColor);
}

void LoadLeaderBoard(Font myFont,Font textFont){
    Vector2 mousePos = GetMousePosition();
    Vector2 MeasureBack = MeasureTextEx(myFont,"Back",30,1.5);
    Rectangle backButton = { SCREEN_WIDTH/2 - MeasureBack.x/2, 600, MeasureBack.x, MeasureBack.y };

    DrawTextEx(myFont, "LeaderBoard", (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(myFont, "LeaderBoard", 40, 1.5).x / 2, 80}, 40, 1.5, YELLOW);

    int displayCount = (userCount > 10) ? 10 : userCount;
    for (int i = 0; i < displayCount; i++)
    {
        // Format: "1. USERNAME   10000"
        const char *entry = TextFormat("%2d. %s", i + 1, users[i].username);
        DrawTextEx(textFont, entry, (Vector2){RIGHT_OFFSET , 150 + i * 40}, 25, 1.5, WHITE);
        DrawTextEx(textFont,TextFormat("%d", users[i].score),(Vector2){SCREEN_WIDTH/2 + RIGHT_OFFSET,150 + i * 40},25,1.5,WHITE);
    }

    if (userCount == 0)
    {
        DrawTextEx(myFont, "NO SCORES YET", (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(myFont, "NO SCORES YET", 30, 1.5).x / 2, 150}, 30, 1.5, YELLOW);
    }

    // Back Button
    bool backHover = CheckCollisionPointRec(mousePos, backButton);
    // Check "Back" button
    DrawTextEx(myFont,"Back",(Vector2){backButton.x,backButton.y},30,1.5,backHover ? GREEN : YELLOW);
    if (CheckCollisionPointRec(mousePos, backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        GameState = Menu;
    }
}

void Collision(Audio *sounds){
    int colP = (int)((pac.position.x - RIGHT_OFFSET) / TILE_SIZE);
    int rowP = (int)((pac.position.y - DOWN_OFFSET) / TILE_SIZE);

    int colG = (int)((g1.position.x - RIGHT_OFFSET) / TILE_SIZE);
    int rowG = (int)((g1.position.y - DOWN_OFFSET) / TILE_SIZE);

    if(colP == colG && rowP == rowG){
         GameState = GameOver;
         PlaySound(sounds->dead);
    }
}

void Quit(Font myFont){
    Vector2 mousePos = GetMousePosition();
    DrawTextEx(myFont,"GAMEOVER!!!",(Vector2){SCREEN_WIDTH/2 - MeasureTextEx(myFont,"GAMEOVER!!!",60,2).x/2,100},60,2,RED);

    DrawTextEx(myFont,TextFormat("Final Score : %d",score),(Vector2){SCREEN_WIDTH/2 - MeasureTextEx(myFont,TextFormat("Final Score : %d",score),40,1.5).x/2,300},40,1.5,WHITE);

    if(score >= users[loggedInUserIndex].score){
        users[loggedInUserIndex].score = score;
        DrawTextEx(myFont,"NEW HIGH SCORE!",(Vector2){SCREEN_WIDTH/2 - MeasureTextEx(myFont,"NEW HIGH SCORE!",30,1).x/2,400},30,1,GREEN);
        SortUsers();
    }

    Vector2 MeasureMenu = MeasureTextEx(myFont,"Menu",30,1.5);
    Rectangle MenuButton = { SCREEN_WIDTH/2 - MeasureMenu.x/2, 600, MeasureMenu.x, MeasureMenu.y};

    bool menuHover = CheckCollisionPointRec(mousePos, MenuButton);
    DrawTextEx(myFont,"Menu",(Vector2){MenuButton.x,MenuButton.y},30,1.5,menuHover ? BLUE : YELLOW);
    if (CheckCollisionPointRec(mousePos, MenuButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Reset();
        GameState = Menu;
    }
}

void Reset(){
    pac.position = (Vector2){RIGHT_OFFSET + (13.5f * TILE_SIZE), DOWN_OFFSET + (24.5f * TILE_SIZE)};
    g1.position = (Vector2){RIGHT_OFFSET + 13*TILE_SIZE,DOWN_OFFSET+ 12.5*TILE_SIZE};
    score = 0;
    memcpy(Map,OGMap,sizeof(OGMap));
    introSound = 0;
}

void RenderMap(Font myFont)
{
    DrawTextEx(myFont, "Score", (Vector2){RIGHT_OFFSET, 6}, 30, 1, WHITE);
    DrawTextEx(myFont,TextFormat("%d",score),(Vector2){RIGHT_OFFSET,35},30,1,WHITE);

    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLS; j++)
        {
            int Y = i * TILE_SIZE + DOWN_OFFSET;
            int X = j * TILE_SIZE + RIGHT_OFFSET;

            switch (Map[i][j])
            {
            // All Double Line Borders
            case 9:
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 3, Y}, (Vector2){X + (TILE_SIZE / 2) - 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 3, Y}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                break;
            case 90:
                DrawLineEx((Vector2){X, Y + (TILE_SIZE / 2) - 3}, (Vector2){X + TILE_SIZE, Y + (TILE_SIZE / 2) - 3}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X, Y + (TILE_SIZE / 2) + 3}, (Vector2){X + TILE_SIZE, Y + (TILE_SIZE / 2) + 3}, 3, (Color){0, 255, 255, 255});
                break;
            case 91:
                DrawRing((Vector2){X + TILE_SIZE, Y + TILE_SIZE}, TILE_SIZE / 2 - 1 - 3, TILE_SIZE / 2 + 1 - 3, 180, 270, 100, (Color){0, 255, 255, 255});
                DrawRing((Vector2){X + TILE_SIZE, Y + TILE_SIZE}, TILE_SIZE / 2 - 1 + 3, TILE_SIZE / 2 + 1 + 3, 180, 270, 100, (Color){0, 255, 255, 255});
                break;
            case 92:
                DrawRing((Vector2){X, Y + TILE_SIZE}, TILE_SIZE / 2 - 1 - 3, TILE_SIZE / 2 + 1 - 3, 270, 360, 100, (Color){0, 255, 255, 255});
                DrawRing((Vector2){X, Y + TILE_SIZE}, TILE_SIZE / 2 - 1 + 3, TILE_SIZE / 2 + 1 + 3, 270, 360, 100, (Color){0, 255, 255, 255});
                break;
            case 93:
                DrawRing((Vector2){X + TILE_SIZE, Y}, TILE_SIZE / 2 - 1 - 3, TILE_SIZE / 2 + 1 - 3, 90, 180, 100, (Color){0, 255, 255, 255});
                DrawRing((Vector2){X + TILE_SIZE, Y}, TILE_SIZE / 2 - 1 + 3, TILE_SIZE / 2 + 1 + 3, 90, 180, 100, (Color){0, 255, 255, 255});
                break;
            case 94:
                DrawRing((Vector2){X, Y}, TILE_SIZE / 2 - 1 - 3, TILE_SIZE / 2 + 1 - 3, 0, 90, 100, (Color){0, 255, 255, 255});
                DrawRing((Vector2){X, Y}, TILE_SIZE / 2 - 1 + 3, TILE_SIZE / 2 + 1 + 3, 0, 90, 100, (Color){0, 255, 255, 255});
                break;

            // All Single Line Border
            case 8:
                DrawLineEx((Vector2){X + TILE_SIZE / 2, Y}, (Vector2){X + TILE_SIZE / 2, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                break;
            case 80:
                DrawLineEx((Vector2){X, Y + TILE_SIZE / 2}, (Vector2){X + TILE_SIZE, Y + TILE_SIZE / 2}, 3, (Color){0, 255, 255, 255});
                break;
            case 81:
                DrawRing((Vector2){X + TILE_SIZE, Y + TILE_SIZE}, TILE_SIZE / 2 - 1, TILE_SIZE / 2 + 1, 180, 270, 100, (Color){0, 255, 255, 255});
                break;
            case 82:
                DrawRing((Vector2){X, Y + TILE_SIZE}, TILE_SIZE / 2 - 1, TILE_SIZE / 2 + 1, 270, 360, 100, (Color){0, 255, 255, 255});
                break;
            case 83:
                DrawRing((Vector2){X + TILE_SIZE, Y}, TILE_SIZE / 2 - 1, TILE_SIZE / 2 + 1, 90, 180, 100, (Color){0, 255, 255, 255});
                break;
            case 84:
                DrawRing((Vector2){X, Y}, TILE_SIZE / 2 - 1, TILE_SIZE / 2 + 1, 0, 90, 100, (Color){0, 255, 255, 255});
                break;

            // All Special Cases
            case 51:
                DrawLineEx((Vector2){X, Y + (TILE_SIZE / 2) - 3}, (Vector2){X + TILE_SIZE, Y + (TILE_SIZE / 2) - 3}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X, Y + (TILE_SIZE / 2) + 3}, (Vector2){X + 3, Y + (TILE_SIZE / 2) + 3}, 3, (Color){0, 255, 255, 255});
                DrawRing((Vector2){X + 3, Y + TILE_SIZE}, TILE_SIZE / 2 - 1 - 3, TILE_SIZE / 2 + 1 - 3, 270, 360, 100, (Color){0, 255, 255, 255});
                break;
            case 52:
                DrawLineEx((Vector2){X, Y + (TILE_SIZE / 2) - 3}, (Vector2){X + TILE_SIZE, Y + (TILE_SIZE / 2) - 3}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + TILE_SIZE - 3, Y + (TILE_SIZE / 2) + 3}, (Vector2){X + TILE_SIZE, Y + (TILE_SIZE / 2) + 3}, 3, (Color){0, 255, 255, 255});
                DrawRing((Vector2){X + TILE_SIZE - 3, Y + TILE_SIZE}, TILE_SIZE / 2 - 1 - 3, TILE_SIZE / 2 + 1 - 3, 180, 270, 100, (Color){0, 255, 255, 255});
                break;
            case 53:
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 3, Y}, (Vector2){X + (TILE_SIZE / 2) - 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 3, Y}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + 3}, 3, (Color){0, 255, 255, 255});
                DrawRing((Vector2){X + TILE_SIZE, Y + 3}, TILE_SIZE / 2 - 1 - 3, TILE_SIZE / 2 + 1 - 3, 90, 180, 100, (Color){0, 255, 255, 255});
                break;
            case 54:
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 3, Y}, (Vector2){X + (TILE_SIZE / 2) - 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 3, Y + TILE_SIZE - 3}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawRing((Vector2){X + TILE_SIZE, Y + TILE_SIZE - 3}, TILE_SIZE / 2 - 1 - 3, TILE_SIZE / 2 + 1 - 3, 180, 270, 100, (Color){0, 255, 255, 255});
                break;
            case 55:
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 3, Y}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 3, Y}, (Vector2){X + (TILE_SIZE / 2) - 3, Y+3}, 3, (Color){0, 255, 255, 255});
                DrawRing((Vector2){X, Y+3}, TILE_SIZE / 2 - 1 - 3, TILE_SIZE / 2 + 1 - 3, 0, 90, 100, (Color){0, 255, 255, 255});
                break;
            case 56:
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 3, Y}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 3, Y + TILE_SIZE - 3}, (Vector2){X + (TILE_SIZE / 2) - 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawRing((Vector2){X, Y + TILE_SIZE - 3}, TILE_SIZE / 2 - 1 - 3, TILE_SIZE / 2 + 1 - 3, 270, 360, 100, (Color){0, 255, 255, 255});
                break;

            // Pallete
            case 1:
                DrawCircle(X + TILE_SIZE / 2, Y + TILE_SIZE / 2, 4, GOLD);
                break;

            // Ghost Door
            case 7:
                DrawRectangle(X, Y + (TILE_SIZE / 2) - 3, TILE_SIZE, 4, PURPLE);
                break;

            // Ghost Corner Walls
            case 71:
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 3, Y + (TILE_SIZE / 2) - 5}, (Vector2){X + (TILE_SIZE / 2) - 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 3, Y + (TILE_SIZE / 2) + 1}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 5, Y + (TILE_SIZE / 2) - 3}, (Vector2){X + TILE_SIZE, Y + (TILE_SIZE / 2) - 3}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 1, Y + (TILE_SIZE / 2) + 3}, (Vector2){X + TILE_SIZE, Y + (TILE_SIZE / 2) + 3}, 3, (Color){0, 255, 255, 255});
                break;
            case 72:
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 3, Y + (TILE_SIZE / 2) + 1}, (Vector2){X + (TILE_SIZE / 2) - 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 3, Y + (TILE_SIZE / 2) - 5}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + TILE_SIZE}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X, Y + (TILE_SIZE / 2) - 3}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + (TILE_SIZE / 2) - 3}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X, Y + (TILE_SIZE / 2) + 3}, (Vector2){X + (TILE_SIZE / 2) - 3, Y + (TILE_SIZE / 2) + 3}, 3, (Color){0, 255, 255, 255});
                break;
            case 73:
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 3, Y}, (Vector2){X + (TILE_SIZE / 2) - 3, Y + (TILE_SIZE / 2) + 4}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 3, Y}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + (TILE_SIZE / 2) - 2}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 3, Y + (TILE_SIZE / 2) - 3}, (Vector2){X + TILE_SIZE, Y + (TILE_SIZE / 2) - 3}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 3, Y + (TILE_SIZE / 2) + 3}, (Vector2){X + TILE_SIZE, Y + (TILE_SIZE / 2) + 3}, 3, (Color){0, 255, 255, 255});
                break;
            case 74:
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) - 3, Y}, (Vector2){X + (TILE_SIZE / 2) - 3, Y + (TILE_SIZE / 2) - 2}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X + (TILE_SIZE / 2) + 3, Y}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + (TILE_SIZE / 2) + 4}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X, Y + (TILE_SIZE / 2) - 3}, (Vector2){X + (TILE_SIZE / 2) - 3, Y + (TILE_SIZE / 2) - 3}, 3, (Color){0, 255, 255, 255});
                DrawLineEx((Vector2){X, Y + (TILE_SIZE / 2) + 3}, (Vector2){X + (TILE_SIZE / 2) + 3, Y + (TILE_SIZE / 2) + 3}, 3, (Color){0, 255, 255, 255});
                break;

            default:
                break;
            }
        }
    }
}