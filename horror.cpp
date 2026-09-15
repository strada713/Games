#include <iostream>
#include <vector>
#include "raylib.h"
int main()
{
    InitWindow(800, 600, "Horror Surival");
    SetTargetFPS(60);
    int ammo = 1;
    float battery = 30.0f;
    bool isLightOn = true;
    float monsterDistance = 100.0f;
    float ammoResparsTimer = 0.0f;
    float batteryRespawnTimer = 0.0f;
    bool isAmmoTrap = false;
    bool isBatteryTrap = false;
    bool ifPlayerAlive = true;
    bool hasAmmoBox = true;
    bool hasBatteryBox = true;
    int animFrames = 0;
    int currentRoom = 0;
    int MonsterRoom = 0;
    float ventTimer = 0.0f;
    InitAudioDevice();
    Sound shotSound = LoadSound("resources/shot.mp3");
    Sound flashSound = LoadSound("resources/flash.mp3");
    Music scaryMusic = LoadMusicStream("resources/musicreac.mp3");
    Image gifImage = LoadImageAnim("resources/eye.gif", &animFrames);
    Sound SoundFnaf = LoadSound("resources/fnaf4.mp3");
    Sound Soundwhosh = LoadSound("resources/sound4.mp3");
    Sound sonic = LoadSound("resources/sonic_laugh.mp3");
    Texture2D monsterTexture = LoadTextureFromImage(gifImage);
    int currentAnimFrame = 0;
    float frameDelay = 0.05f;
    float frameTimer = 0.0f;
    PlayMusicStream(scaryMusic);
    while (!WindowShouldClose())
    {
        UpdateMusicStream(scaryMusic);
        if (IsKeyPressed(KEY_W))
        {
            if (ammo > 0 && ifPlayerAlive)
            {
                ammo = ammo - 1;
                PlaySound(shotSound);
                if (currentRoom == MonsterRoom)
                {
                    monsterDistance = monsterDistance + 25.0f;

                    if (monsterDistance > 100.0f)
                        monsterDistance = 100.0f;
                    std::cout << "SHOT. monster pushed back!" << std::endl;
                }
                else
                {
                    std::cout << " shoot into dark space" << std::endl;
                }
            }
            else if (ammo <= 0 && ifPlayerAlive)
            {
                std::cout << "click*   out of ammo!" << std::endl;
            }
        }
        if (IsKeyPressed(KEY_F))
        {
            isLightOn = !isLightOn;

            if (isLightOn)
            {
                std::cout << "--- FLASHLIGHT TURNED ON ---" << std::endl;
                PlaySound(flashSound);
            }
            else
            {
                std::cout << "--- FLASHLIGHT TURNED OFF ---" << std::endl;
                PlaySound(flashSound);
            }
        }
        if (IsKeyPressed(KEY_D) && currentRoom < 1)
        {
            currentRoom = currentRoom + 1;
            PlaySound(Soundwhosh);
        }
        if (IsKeyPressed(KEY_A) && currentRoom > -1)
        {
            currentRoom = currentRoom - 1;
            PlaySound(Soundwhosh);
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            if (currentRoom == 1 && hasAmmoBox)
            {
                if (isAmmoTrap)
                {
                    ammo = ammo - 2;
                    if (ammo < 0)
                        ammo = 0;
                    monsterDistance = monsterDistance - 15.0f;
                    PlaySound(sonic);
                    std::cout << " trap:) -2 ammo" << std::endl;
                }
                else
                {
                    int randomAmmo = GetRandomValue(1, 2);
                    ammo = ammo + randomAmmo;
                    std::cout << " picked up +" << randomAmmo << "ammo" << std::endl;
                }
                hasAmmoBox = false;
            }
            if (currentRoom == -1 && hasBatteryBox)
            {
                if (isBatteryTrap)
                {
                    battery = battery - 30.0f;
                    if (battery < 0)
                        battery = 0;
                    monsterDistance = monsterDistance - 15.0f;
                    PlaySound(sonic);
                    std::cout << "trap:) -30% battery" << std::endl;
                }
                else
                {
                    int randomBattery = GetRandomValue(10, 20);
                    battery = battery + randomBattery;
                    if (battery > 100.0f)
                        battery = 100.0f;
                    std::cout << " picked up" << randomBattery << "%% battery" << std::endl;
                }
                hasBatteryBox = false;
            }
        }
        if (!hasAmmoBox)
        {
            ammoResparsTimer = ammoResparsTimer + GetFrameTime();
            if (ammoResparsTimer >= 15.0f)
            {
                hasAmmoBox = true;
                ammoResparsTimer = 0.0f;
                isAmmoTrap = (GetRandomValue(1, 10) <= 3);
                std::cout << "a new ammo box spawned on the shelf(Trap: " << isAmmoTrap << ")" << std::endl;
            }
        }
        if (!hasBatteryBox)
        {
            batteryRespawnTimer = batteryRespawnTimer + GetFrameTime();
            if (batteryRespawnTimer >= 20.0f)
            {
                hasBatteryBox = true;
                batteryRespawnTimer = 0.0f;
                isBatteryTrap = (GetRandomValue(1, 10) <= 3);
                std::cout << " a new battery spawned in the shield (Trap: " << isBatteryTrap << ")" << std::endl;
            }
        }
        if (isLightOn && ifPlayerAlive && battery > 0.0f)
        {
            battery = battery - GetFrameTime() * 2.0f;

            std::cout << "Battery level: " << battery << "%" << std::endl;
            if (battery < 0.0f)
            {
                battery = 0.0f;
            }
        }
        if (ifPlayerAlive)
        {
            float monsterSpeed = isLightOn ? 1.5f : 5.0f;

            monsterDistance = monsterDistance - GetFrameTime() * monsterSpeed;
            std::cout << "MONSTER DISTANCE: " << monsterDistance << "meters" << std::endl;
            if (monsterDistance <= 0.0f)
            {
                PlaySound(SoundFnaf);
                WaitTime(0.5f);
                CloseAudioDevice();
                CloseWindow();
                exit(0);
            }
        }
        if (ifPlayerAlive)
        {
            frameTimer = frameTimer + GetFrameTime();
            if (frameTimer >= frameDelay)
            {
                frameTimer = 0.0f;
                currentAnimFrame = currentAnimFrame + 1;
                if (currentAnimFrame >= animFrames)
                    currentAnimFrame = 0;
                int nextFrameOffset = gifImage.width * gifImage.height * 4 * currentAnimFrame;
                UpdateTextureRec(
                    monsterTexture,
                    Rectangle{0, 0, (float)monsterTexture.width, (float)monsterTexture.height},
                    (unsigned char *)gifImage.data + nextFrameOffset);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        if (ifPlayerAlive)
        {

            if (currentRoom == 0)
            {
                if (isLightOn && battery > 0.0f)
                {
                    DrawCircle(400, 300, 250, GetColor(0x3a3a00ff));
                }
                if (ifPlayerAlive)
                {
                    float monsterRadius = (100.0f - monsterDistance) * 1.5;
                    if (monsterRadius < 5.0f)
                        monsterRadius = 5.0f;
                    float ImgSize = monsterRadius * 2.0f;
                    DrawTexturePro(
                        monsterTexture,
                        Rectangle{0, 0, (float)monsterTexture.width, (float)monsterTexture.height},
                        Rectangle{400.0f - (ImgSize / 2.0f), 300.0f - (ImgSize / 2.0f), ImgSize, ImgSize},
                        Vector2{0, 0}, 0.0f, WHITE);
                }
            }
            else if (currentRoom == -1)
            {
                ClearBackground(GetColor(0x1a1a1aff));
                DrawText(" ELECTRICAL ROOM ", 240, 50, 24, RED);

                if (hasBatteryBox)
                {
                    Color batColor = isBatteryTrap ? MAROON : YELLOW;
                    DrawText("[SPACE BATTERY IS HERE]", 250, 280, 20, YELLOW);
                    DrawText("PRESS SPACE TO PICK UP", 280, 320, 16, GRAY);
                }
                else
                {
                    DrawText("( EMPTY SHIELD)", 320, 280, 20, DARKGRAY);
                }
            }
            else if (currentRoom == 1)
            {
                ClearBackground(GetColor(0x1a1a1aff));
                DrawText(" AMMO STORAGE", 270, 50, 24, RED);
                if (hasAmmoBox)
                {
                    Color ammoColor = isAmmoTrap ? MAROON : GREEN;
                    DrawText("[AMMO BOX ON THE SHELF]", 240, 280, 20, GREEN);
                    DrawText("PRESS SPACE TO PICK UP", 280, 320, 16, GRAY);
                }
                else
                {
                    DrawText("(EMPTY SHELF)", 330, 280, 20, DARKGRAY);
                }
            }
            else
            {
                ClearBackground(MAROON);
                DrawText("YOU DEAD", 180, 260, 40, WHITE);
                DrawText("GAME OVER", 300, 320, 30, BLACK);
            }
            DrawText(TextFormat("AMMO: %d", ammo), 20, 20, 20, WHITE);
            DrawText(TextFormat("BATTERY: %d%%", (int)battery), 20, 50, 20, isLightOn ? YELLOW : GRAY);
            EndDrawing();
        }
    }
    UnloadTexture(monsterTexture);
    UnloadImage(gifImage);
    UnloadSound(shotSound);
    UnloadSound(flashSound);
    UnloadMusicStream(scaryMusic);
    UnloadSound(SoundFnaf);
    CloseWindow();
    return 0;
}