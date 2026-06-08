#include <raylib.h>
#include "BaseScreen.h"

BaseScreen::BaseScreen()
{
    backSpacePositions[0] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 20), 155.0 };
    backSpacePositions[1] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 10), 147.0 };
    backSpacePositions[2] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 10), 163.0 };
    navigateBackward[0] = { 0,12.5 };
    navigateBackward[1] = { 25,25 };
    navigateBackward[2] = { 25,0 };
    x1 = GetScreenWidth() / 3 + 60;
    y1 = 80;
    x2 = GetScreenWidth() / 3 + 60;
    y2 = GetScreenHeight() - GetScreenHeight() / 3 + 30;
}

BaseScreen::BaseScreen(std::vector<std::string>& infoTexts)
{
    this->infoTexts = infoTexts;
    ClearScene();
}

void BaseScreen::ClearScene()
{
    //xPos = width / 3 + 60;
    //yPos = 80;

    //errorButtonApplied = false;
    //infoTexts.clear();
}

void BaseScreen::DrawScreen()
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();
  //  int textWidth = MeasureText(Input.c_str(), 20);
    DrawTriangle(navigateBackward[0], navigateBackward[1], navigateBackward[2], GREEN);
   // DrawText("Parity Bit", width / 2 - MeasureText("Parity Bit", 40) / 2, 0, 40, GRAY);
    DrawRectangleLines(0, 30, width / 3.5, height, GOLD);
    DrawText("Parna parnost", 2, 30, 18, BLACK);
   // DrawCircle((width / 3.5) - 20, 40, 10, ColorRadBtn1);
    DrawText("Neparna parnost", 2, 60, 18, BLACK);
   // DrawCircle((width / 3.5) - 20, 70, 10, ColorRadBtn2);
    DrawText("Greska prenosa", 2, 90, 18, BLACK);
   // DrawCircle((width / 3.5) - 20, 100, 10, ColorRadBtn3);
    DrawText("Ulazni podaci", 2, 120, 18, BLACK);
    DrawRectangleLines(2, 140, width / 3.5 - 5, 30, BLACK);
 //   DrawText(Input.c_str(),2 + (width / 3.5 - 5 - textWidth) / 2,140 + (30 - 20) / 2,20,BLACK);

    DrawRectangleLines(40, 180, width / 3.5 - 80, 30, DARKGREEN);
    DrawText("Testiraj", 40 + (width / 3.5 - 80) / 2 - MeasureText("Testiraj", 30) / 2, 180 + 30 / 2 - 30 / 2, 30, GREEN);
    DrawRectangleLines(40, 220, width / 3.5 - 80, 30, RED);
    DrawText("Resetuj", 40 + (width / 3.5 - 80) / 2 - MeasureText("Resetuj", 30) / 2, 220 + 30 / 2 - 30 / 2, 30, RED);
    DrawTriangleLines(backSpacePositions[0], backSpacePositions[1], backSpacePositions[2], RED);
    DrawRectangleLines(2 + width / 3.5 - 5 - 20 * 2 - 5, 140 + 5, 20, 20, BLACK);
    DrawText("0", 2 + (width / 3.5 - 5 - 20 * 2 - 5) + 20 / 2 - MeasureText("0", 20) / 2, 140 + 5 + 20 / 2 - 20 / 2, 20, RED);
    DrawRectangleLines(2 + width / 3.5 - 5 - 20 - 2, 140 + 5, 20, 20, BLACK);
    DrawText("1", 2 + (width / 3.5 - 5 - 20 - 2) + 20 / 2 - MeasureText("1", 20) / 2, 140 + 5 + 20 / 2 - 20 / 2, 20, RED);
    int x = width / 2;
    int y = height - 60 - 30;
    DrawRectangle(x + 50, y, 200, 60, LIME);
    DrawText(
        "DALJE",
        x + 300 / 2,
        y + (60 - 30) / 2,
        30,
        WHITE
    );
}

void BaseScreen::ShowSenderInfo(std::string info)
{
    int width = GetScreenWidth();

    Rectangle box = {
        (float)width / 3 + 125,
        40,
        float(width)- ((float)width / 3 + 125) - 20,
        340
    };
    DrawRectangleRec(box, SVETLOZELENA);
    DrawRectangleLinesEx(box, 2, DARKGREEN);
    DrawText("INFO", box.x + 10, box.y + 10, 24, DARKGREEN);
    DrawLine(
        box.x + 10,
        box.y + 40,
        box.x + box.width - 10,
        box.y + 40,
        DARKGREEN
    );
    DrawText(
        info.c_str(),
        box.x + 10,
        box.y + 55,
        20,
        BLACK
    );
}

void BaseScreen::ShowReceiverInfo(std::string info)
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    Rectangle box = {
        (float)width / 3 + 125,
        (float)height - height / 3 - 100,
        (float)width - ((float)width / 3 + 125) - 20,
        180
    };

    DrawRectangleRec(box, SVETLOZELENA);
    DrawRectangleLinesEx(box, 2, DARKGREEN);
    DrawText("INFO", box.x + 10, box.y + 10, 24, DARKGREEN);
    DrawLine(
        box.x + 10,
        box.y + 40,
        box.x + box.width - 10,
        box.y + 40,
        DARKGREEN
    );

    DrawText(
        info.c_str(),
        box.x + 10,
        box.y + 55,
        20,
        BLACK
    );
}

void BaseScreen::DrawStaticScene()
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    DrawRectangleLines(width / 3, 40, 120, 80, BLUE);
    DrawRectangleLines(width / 3 + 120 - 25, 40 + 5, 20, 20, BLACK);
    DrawText("?", width / 3 + 120 - 19, 40 + 7, 16, BLACK);
    DrawText("Sender", width / 3 + 25, 70, 20, BLACK);
    DrawRectangleLines( width / 3, height - height / 3, 120, 80, GREEN);
    DrawRectangleLines(width / 3 + 120 - 25, height - height / 3 + 5, 20, 20, BLACK);
    DrawText("?", width / 3 + 120 - 19, height - height / 3 + 7, 16, BLACK);
    DrawText("Receiver", width / 3 + 15, (height - height / 3) + 50, 20, BLACK);

    DrawLine(x1, y1, x2, y2, GREEN);
}

void BaseScreen::AnimateCode()
{
    // ! finished
    float speed = 2.0f;

    float dx = x2 - x1;
    float dy = y2 - y1;

    float length = sqrt(dx * dx + dy * dy);

    xPos += (dx / length) * speed;
    yPos += (dy / length) * speed;

    // if (ypos >= y2 )    receive
}
