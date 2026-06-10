#include "raylib.h"
#include <vector>
#include <bitset>
#include <iostream>

#include "Button.h"
#include "BaseScreen.h"
#include "ParityBit.h"
#include "ParityScreen.h"
#include "HammingScreen.h"
#include "LRCScreen.h"
#include "CRCScreen.h"
#include "GolayScreen.h"
#include "ScreenNavigation.h"
#include "Hamming.h"
#include "LRC.h"
#include "CRC.h"
#include "Golay.h"
int main()
{
    //const int monitor = GetCurrentMonitor();
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "ECC Vizuelizacija");
    SetConfigFlags(FLAG_FULLSCREEN_MODE);

    SetTargetFPS(60);
    
    BaseScreen baseScreen;
    ParityScreen ps;
    HammingScreen hs;
    LRCScreen lrcs;
    CRCscreen crcs;
    GolayScreen gscr;
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
            if (ps.test && ps.GetInputLength() >= 5)
            {
                ps.DrawScene();
            }
            break;
        case Hamming:
            hs.DrawHammingScreen();
            if (hs.test && hs.GetInputLength() >= 4)
            {
                hs.DrawScene();
            }
            break;
        case Lrc:
            lrcs.DrawLRCScreen();
            if (lrcs.test)
            {
                lrcs.DrawScene();
            }
            break;
        case Crc:
            crcs.DrawCRCscreen();
            if (crcs.test && crcs.GetInputLength() >=5)
            {
                if (crcs.canSend)
                    crcs.DrawScene();
                else
                {
                    crcs.prepareBits();
                    crcs.DrawSendersSteps();
                }
            }   
            break;
        case GOLAY:
            gscr.DrawGolayScreen();
            if(gscr.test && gscr.GetInputLength() == 12)
                gscr.DrawScene();
            break;
        }
#pragma region mouse_click_event
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
                currentScreen = GOLAY;
            }
            if (CheckCollisionPointRec(mousePos, recParityBttn) && currentScreen == Menu)
            {
                currentScreen = Parity;
            }
            if (CheckCollisionPointTriangle(mousePos, baseScreen.navigateBackward[0], baseScreen.navigateBackward[1], baseScreen.navigateBackward[2]) && currentScreen != Menu)
            {
                currentScreen = Menu;
            }
#pragma endregion mouse_click_event
#pragma region CRC
            if (currentScreen == Crc)
            {
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 180, screenWidth / 3.5 - 80, 30 }))
                {
                    if (crcs.test)
                    {
                        crcs.test = false;
                    }
                    else
                    {
                        crcs.test = true;
                    }
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 220, screenWidth / 3.5 - 80, 30 }))
                {
                    crcs.ClearScene();
                }
                if (CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 100 }, 10.0))
                {
                    crcs.CheckRadioButton();
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 * 2 - 5, 140 + 5, 20, 20 }))
                {
                    crcs.CheckButton('0');
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 - 2, 140 + 5, 20, 20 }))
                {
                    crcs.CheckButton('1');
                }
                if (CheckCollisionPointTriangle(mousePos, baseScreen.backSpacePositions[0], baseScreen.backSpacePositions[1], baseScreen.backSpacePositions[2]))
                {
                    crcs.CheckButton('-');
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ screenWidth / 3 + 120 - 25, 40 + 5, 20, 20 }))
                {
                    if (crcs.senderInfoBtn)
                        crcs.senderInfoBtn = false;
                    else
                        crcs.senderInfoBtn = true;
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ screenWidth / 3 + 120 - 25, screenHeight - screenHeight / 3 + 5, 20, 20 }))
                {
                    if (crcs.receiverInfoBtn)
                        crcs.receiverInfoBtn = false;
                    else
                        crcs.receiverInfoBtn = true;
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ (float)GetScreenWidth() / 2 , (float)GetScreenHeight() - 60 - 30, 200, 60 }))
                {
                    crcs.CheckBtnDalje();
                }
            }
#pragma endregion CRC
#pragma region LRC
            if (currentScreen == Lrc)
            {
                if (CheckCollisionPointCircle(mousePos, Vector2{ screenWidth / 3.5 - 20, 100.0 }, 10.0))
                {
                    lrcs.CheckRadioButton(ERROR_SIM);
                }
                if (CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 70.0 }, 10.0))
                {
                    lrcs.CheckRadioButton(ODD_PARITY);
                }
                if (CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 40.0 }, 10.0))
                {
                    lrcs.CheckRadioButton(EVEN_PARITY);
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 180, screenWidth / 3.5 - 80, 30 }))
                {
                    if (lrcs.test)
                        lrcs.test = false;
                    else
                        lrcs.test = true;
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 220, screenWidth / 3.5 - 80, 30 }))
                {
                    lrcs.ClearScene();
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 300, screenWidth / 3.5 - 80, 30 }))
                {
                    if (lrcs.generate)
                        lrcs.generate = false;
                    else
                    {
                        lrcs.generate = true;
                    }
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 * 2 - 5, 140 + 5, 20, 20 }))
                {
                    lrcs.CheckButton('0');
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 - 2, 140 + 5, 20, 20 }))
                {
                    lrcs.CheckButton('1');
                }
                if (CheckCollisionPointTriangle(mousePos, baseScreen.backSpacePositions[0], baseScreen.backSpacePositions[1], baseScreen.backSpacePositions[2]))
                {
                    lrcs.CheckButton('-');
                }
                if (CheckCollisionPointRec(mousePos, lrcs.bitButtons[0]))
                {
                    lrcs.SelectedBitOption = 8;
                    lrcs.CheckSelectedBitButton();
                }
                else if (CheckCollisionPointRec(mousePos, lrcs.bitButtons[1]))
                {
                    lrcs.SelectedBitOption = 16;
                    lrcs.CheckSelectedBitButton();
                }
                else if (CheckCollisionPointRec(mousePos, lrcs.bitButtons[2]))
                {
                    lrcs.SelectedBitOption = 32;
                    lrcs.CheckSelectedBitButton();
                }
                else if (CheckCollisionPointRec(mousePos, lrcs.bitButtons[3]))
                {
                    lrcs.SelectedBitOption = 64;
                    lrcs.CheckSelectedBitButton();
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ screenWidth / 3 + 120 - 25, 40 + 5, 20, 20 }))
                {
                    if (lrcs.senderInfoBtn)
                        lrcs.senderInfoBtn = false;
                    else
                        lrcs.senderInfoBtn = true;
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ screenWidth / 3 + 120 - 25, screenHeight - screenHeight / 3 + 5, 20, 20 }))
                {
                    if (lrcs.receiverInfoBtn)
                        lrcs.receiverInfoBtn = false;
                    else
                        lrcs.receiverInfoBtn = true;
                }
            }
#pragma endregion LRC
#pragma region Hamming
            if (currentScreen == Hamming)
            {
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 180, screenWidth / 3.5 - 80, 30 }))
                {
                    if (hs.test)
                        hs.test = false;
                    else
                        hs.test = true;
                }
                if (CheckCollisionPointCircle(mousePos, Vector2{ screenWidth / 3.5 - 20, 100.0 }, 10.0))
                {
                    hs.CheckRadioButton();
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 220, screenWidth / 3.5 - 80, 30 }))
                {
                    hs.ClearScene();
                }
                if (CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 40 }, 10.0))
                {
                    hs.CheckRadioButton();
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 * 2 - 5, 140 + 5, 20, 20 }))
                {
                    hs.CheckButton('0');
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 - 2, 140 + 5, 20, 20 }))
                {
                    hs.CheckButton('1');
                }
                if (CheckCollisionPointTriangle(mousePos, baseScreen.backSpacePositions[0], baseScreen.backSpacePositions[1], baseScreen.backSpacePositions[2]))
                {
                    hs.CheckButton('-');
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ (float)GetScreenWidth() / 2 , (float)GetScreenHeight() - 60 - 30, 200, 60 }))
                {
                    hs.CheckButtonNext();
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ screenWidth / 3 + 120 - 25, 40 + 5, 20, 20 }))
                {
                    if (hs.senderInfoBtn)
                        hs.senderInfoBtn = false;
                    else
                        hs.senderInfoBtn = true;
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ screenWidth / 3 + 120 - 25, screenHeight - screenHeight / 3 + 5, 20, 20 }))
                {
                    if (hs.receiverInfoBtn)
                        hs.receiverInfoBtn = false;
                    else
                        hs.receiverInfoBtn = true;
                }
            }

#pragma endregion Hamming
#pragma region Parity
            if (currentScreen == Parity)
            {
                if (CheckCollisionPointCircle(mousePos, Vector2{ screenWidth / 3.5f - 20.0f, 100.0f }, 10.0))
                {
                    ps.CheckRadioButton(ERROR_SIM);
                }
                if (CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 70.0 }, 10.0))
                {
                    ps.CheckRadioButton(ODD_PARITY);
                }
                if (CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 40.0 }, 10.0))
                {
                    ps.CheckRadioButton(EVEN_PARITY);
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 220, screenWidth / 3.5 - 80, 30 }))
                {
                    ps.ClearScene();
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 * 2 - 5, 140 + 5, 20, 20 }))
                {
                    ps.CheckButton('0');
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 - 2, 140 + 5, 20, 20 }))
                {
                    ps.CheckButton('1');
                }
                if (CheckCollisionPointTriangle(mousePos, baseScreen.backSpacePositions[0], baseScreen.backSpacePositions[1], baseScreen.backSpacePositions[2]))
                {
                    ps.CheckButton('-');
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 180, screenWidth / 3.5 - 80, 30 }))
                {
                    if (ps.IsRadBtn1Active() == true || ps.IsRadBtn2Active() == true)
                    {
                        ps.test = true;
                    }
                    else
                        ps.test = false;
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ screenWidth / 3 + 120 - 25, 40 + 5, 20, 20 }))
                {
                    if (ps.senderInfoBtn)
                        ps.senderInfoBtn = false;
                    else
                        ps.senderInfoBtn = true;
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ screenWidth / 3 + 120 - 25, screenHeight - screenHeight / 3 + 5, 20, 20 }))
                {
                    if (ps.receiverInfoBtn)
                        ps.receiverInfoBtn = false;
                    else
                        ps.receiverInfoBtn = true;
                }
            }
#pragma endregion Parity
#pragma region Golay
            if (currentScreen == GOLAY)
            {
                if (CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 * 2 - 5, 140 + 5, 20, 20 }))
                {
                    gscr.CheckButton('0');
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 2 + screenWidth / 3.5 - 5 - 20 - 2, 140 + 5, 20, 20 }))
                {
                    gscr.CheckButton('1');
                }
                if (CheckCollisionPointTriangle(mousePos, baseScreen.backSpacePositions[0], baseScreen.backSpacePositions[1], baseScreen.backSpacePositions[2]))
                {
                    gscr.CheckButton('-');
                }
                if (CheckCollisionPointCircle(mousePos, Vector2{ (screenWidth / 3.5) - 20, 100 }, 10.0))
                {
                    gscr.CheckRadioButton();
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 180, screenWidth / 3.5 - 80, 30 }))
                {
                    if (gscr.test == false)
                    {
                        gscr.test = true;
                    }
                    else
                        gscr.test = false;
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 220, screenWidth / 3.5 - 80, 30 }))
                {
                    gscr.ClearScene();
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 260, screenWidth / 3.5 - 80, 30 }))
                {
                    gscr.CheckPrikazBtn(btnBC);
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 300, screenWidth / 3.5 - 80, 30 }))
                {
                    gscr.CheckPrikazBtn(btnB);
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 340, screenWidth / 3.5 - 80, 30 }))
                {
                    gscr.CheckPrikazBtn(btnG);
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 380, (float)screenWidth / 3.5 - 80, 30 }))
                {
                    gscr.CheckPrikazBtn(btnH);
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ 40, 420, (float)(screenWidth / 3.5 - 80.f), 30 }))
                {
                    gscr.CheckPrikazBtn(btnV);
                }
                if (CheckCollisionPointRec(mousePos, Rectangle{ (float)GetScreenWidth() / 2 , (float)GetScreenHeight() - 60 - 30, 200, 60 }))
                {
                    gscr.CheckPrikazBtn(btnNext);
                }
            }
#pragma endregion Golay
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}