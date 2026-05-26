#include "raylib.h"
#include <vector>
#include <iostream>

#include "Button.h"
#include "BaseScreen.h"
#include "ParityBit.h"
#include "ParityScreen.h"
#include "HammingScreen.h"
#include "LRCScreen.h"
#include "CRCScreen.h"
#include "ScreenNavigation.h"
#include "Hamming.h"
#include "LRC.h"
#include "CRC.h"
int main()
{
    const int screenWidth = 1080;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "ECC Vizuelizacija");

    SetTargetFPS(60);
    
    BaseScreen baseScreen;
    ParityScreen ps;
    HammingScreen hs;
    LRCScreen lrcs;
    CRCscreen crcs;
    ScreenNavigation currentScreen = Menu;
    Rectangle recCRCBttn = { screenWidth/2 - 100,120, 200, 50 };
    Rectangle recParityBttn = { screenWidth / 2 - 100, 170, 200, 50 };
    Rectangle recHammingBttn = { screenWidth / 2 - 100, 220, 200, 50 };
    Rectangle recLRCBttn = { screenWidth / 2 - 100, 270, 200, 50 };
    Rectangle recGolayBttn = { screenWidth / 2 - 100, 320, 200, 50 };
    Button parityBtn(recParityBttn, "Parity bit");
    Button hammingBtn(recHammingBttn, "Hamming code");
    Button crcBtn(recCRCBttn, "CRC");
    Button lrcBtn(recLRCBttn, "LRC");
    Button golayBtn(recGolayBttn, "Golay");


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
            
        switch (currentScreen)
        {
        case Menu:
            crcBtn.DrawButton();
            parityBtn.DrawButton();
            hammingBtn.DrawButton();
            lrcBtn.DrawButton();
            golayBtn.DrawButton();
            break;
        case Parity:
            ps.DrawParityScreen();
            if (ps.GetInfoTextLenght()>0)
                ps.DrawInfoText();
            if (ps.test && ps.GetInputLength() >= 5 && !ps.reset)
            {
                ps.DrawScene();
            }
            break;
        case Hamming:
            hs.DrawHammingScreen();
            if (hs.test && hs.GetInputLength() >= 4 && !hs.reset)
            {
                hs.DrawScene();
            }
            break;
        case Lrc:
            lrcs.DrawLRCScreen();
            if (lrcs.test && !lrcs.reset)
            {
                lrcs.DrawScene();
            }
            break;
        case Crc:
            crcs.DrawCRCscreen();
            if (crcs.test && !crcs.reset)
            {
                crcs.DrawScene();
            }
            break;
        case Golay:
            break;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos;
            mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, recHammingBttn) && currentScreen == Menu)
            {
                currentScreen = Hamming;
            }
            if (CheckCollisionPointRec(mousePos, recCRCBttn) && currentScreen == Menu)
            {
                currentScreen = Crc;
            }
            if (CheckCollisionPointRec(mousePos, recLRCBttn) && currentScreen == Menu)
            {
                currentScreen = Lrc;
            }
            if (CheckCollisionPointRec(mousePos, recGolayBttn) && currentScreen == Menu)
            {
                currentScreen = Golay;
            }
            if (CheckCollisionPointRec(mousePos, recParityBttn) && currentScreen == Menu)
            {
                currentScreen = Parity;
            }
            if (CheckCollisionPointTriangle(mousePos, baseScreen.navigateBackward[0], baseScreen.navigateBackward[1], baseScreen.navigateBackward[2]) && currentScreen != Menu)
            {
                currentScreen = Menu;
            }
#pragma region CRC
            if (currentScreen == Crc && CheckCollisionPointRec(mousePos, Rectangle{ 40, 180, screenWidth / 3.5 - 80, 30 }))
            {
                if (crcs.test)
                    crcs.test = false;
                else
                    crcs.test = true;
            }
            if (currentScreen == Crc && CheckCollisionPointRec(mousePos, Rectangle{ 40, 220, screenWidth / 3.5 - 80, 30 }))
            {
                crcs.ClearScene();
                crcs.reset = true;
            }
            if (currentScreen == Crc && CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 100 }, 10.0))
            {
                crcs.CheckRadioButton();
            }
            if (currentScreen == Crc && CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 * 2 - 5, 140 + 5, 20, 20 }))
            {
                crcs.CheckButton('0');
            }
            if (currentScreen == Crc && CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 - 2, 140 + 5, 20, 20 }))
            {
                crcs.CheckButton('1');
            }
            if (currentScreen == Crc && CheckCollisionPointTriangle(mousePos, baseScreen.backSpacePositions[0], baseScreen.backSpacePositions[1], baseScreen.backSpacePositions[2]))
            {
                crcs.CheckButton('-');
            }
#pragma endregion CRC
#pragma region LRC
            if (currentScreen == Lrc && CheckCollisionPointRec(mousePos, Rectangle{ 40, 180, screenWidth / 3.5 - 80, 30 }))
            {
                if (lrcs.test)
                    lrcs.test = false;
                else
                    lrcs.test = true;
            }
            if (currentScreen == Lrc && CheckCollisionPointRec(mousePos, Rectangle{ 40, 220, screenWidth / 3.5 - 80, 30 }))
            {
                lrcs.ClearScene();
                lrcs.reset = true;
            }
            if (currentScreen == Lrc && CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 40 }, 10.0))
            {
                lrcs.CheckRadioButton();
            }
            if (currentScreen == Lrc && CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 * 2 - 5, 140 + 5, 20, 20 }))
            {
                lrcs.CheckButton('0');
            }
            if (currentScreen == Lrc && CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 - 2, 140 + 5, 20, 20 }))
            {
                lrcs.CheckButton('1');
            }
            if (currentScreen == Lrc && CheckCollisionPointTriangle(mousePos, baseScreen.backSpacePositions[0], baseScreen.backSpacePositions[1], baseScreen.backSpacePositions[2]))
            {
                lrcs.CheckButton('-');
            }
            
#pragma endregion LRC
#pragma region Hamming
            if (currentScreen == Hamming && CheckCollisionPointRec(mousePos, Rectangle{ 40, 180, screenWidth / 3.5 - 80, 30 }))
            {
                if (hs.test)
                    hs.test = false;
                else
                    hs.test = true;
            }
            if (currentScreen == Hamming && CheckCollisionPointRec(mousePos, Rectangle{ 40, 220, screenWidth / 3.5 - 80, 30 }))
            {
                hs.ClearScene();
                hs.reset = true;
            }
            if (currentScreen == Hamming && CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 40 }, 10.0))
            {
                hs.CheckRadioButton();
            }
            if (currentScreen == Hamming && CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 * 2 - 5, 140 + 5, 20, 20 }))
            {
                hs.CheckButton('0');
            }
            if (currentScreen == Hamming && CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 - 2, 140 + 5, 20, 20 }))
            {
                hs.CheckButton('1');
            }
            if (currentScreen == Hamming && CheckCollisionPointTriangle(mousePos, baseScreen.backSpacePositions[0], baseScreen.backSpacePositions[1], baseScreen.backSpacePositions[2]))
            {
                hs.CheckButton('-');
            }


#pragma endregion Hamming
#pragma region Parity
            if (currentScreen == Parity && CheckCollisionPointCircle(mousePos, Vector2{ screenWidth / 3.5 - 20, 100.0 }, 10.0))
            {
                ps.CheckRadioButton(ERROR_SIM);
            }
            if (currentScreen == Parity && CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 70.0 }, 10.0))
            {
                ps.CheckRadioButton(ODD_PARITY);
            }
            if (currentScreen == Parity && CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 40.0 }, 10.0))
            {
                ps.CheckRadioButton(EVEN_PARITY);
            }
            if (currentScreen == Parity && CheckCollisionPointRec(mousePos, Rectangle{ 40, 220, screenWidth / 3.5 - 80, 30 }))
            {
                ps.ClearScene();
                ps.reset = true;
            }
            if (currentScreen == Parity && CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 * 2 - 5, 140 + 5, 20, 20 }))
            {
                ps.CheckButton('0');
            }
            if (currentScreen == Parity && CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 - 2, 140 + 5, 20, 20 }))
            {
                ps.CheckButton('1');
            }
            if (currentScreen == Parity && CheckCollisionPointTriangle(mousePos, baseScreen.backSpacePositions[0], baseScreen.backSpacePositions[1], baseScreen.backSpacePositions[2]))
            {
                ps.CheckButton('-');
            }
            if (currentScreen == Parity && CheckCollisionPointRec(mousePos, Rectangle{ 40, 180, screenWidth / 3.5 - 80, 30 }))
            {
                if (ps.IsRadBtn1Active() == true || ps.IsRadBtn2Active() == true)
                {
                    std::cout << "TEST";
                    ps.test = true;
                    ps.reset = false;
                }
                else
                    ps.test = false;
            }
#pragma endregion Parity
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}