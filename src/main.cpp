#include <raylib.h>
#include <raymath.h>

#include "imgui.h"
#include "rlImGui.h"

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

enum class Cell : uint8_t
{
    AIR = 0,
    SAND = 1,
    WATER = 2,
    OIL = 3,
    ROCK = 4,
    FIRE = 5

    // to add: steam, glass, lava, obsidian
};

class Grid
{
private:
    int m_Cols;
    int m_Rows;
    int m_CellSize;
    int m_CellSpeed; 

    std::vector<Cell> m_Cells;
    std::vector<Cell> m_Next;

public:
    Grid(int cols, int rows, int cellSize, int cellSpeed)
    {
        m_Cols = cols;
        m_Rows = rows;
        m_CellSize = cellSize;
        m_CellSpeed = cellSpeed;

        m_Cells = std::vector<Cell> (m_Cols * m_Rows, Cell::AIR);
    }

    ~Grid() = default;

    void spawnCells(Cell selected, int spawnRadius)
    {
        Vector2 mousePos = GetMousePosition();

        for (int x = (int)mouseCell(mousePos).x - spawnRadius; x < (int)mouseCell(mousePos).x + spawnRadius; ++x)
        {
            for (int y = (int)mouseCell(mousePos).y - spawnRadius; y < (int)mouseCell(mousePos).y + spawnRadius; ++y)
            {
                if (canMove(selected, x, y)) m_Cells[getKey(x, y)] = selected;
            }
        }
    }

    void update()
    {
        m_Next = m_Cells;

        for (int x = 0; x < m_Cols; ++x)
        {
            for (int y = 0; y < m_Rows; ++y)
            { 
                switch (m_Cells[getKey(x, y)])
                {
                case Cell::SAND:
                    updateSand(x, y);
                    break;
                case Cell::WATER:
                    updateWater(x, y);
                    break;
                case Cell::OIL:
                    updateOil(x, y);
                    break;
                case Cell::FIRE:
                    updateFire(x, y);
                    break;
                default:
                    break;
                }
            }
        }   

        m_Cells = m_Next;
    }

    void render()
    {
        for (int x = 0; x < m_Cols; ++x)
        {
            for (int y = 0; y < m_Rows; ++y)
            {
                switch (m_Cells[getKey(x, y)])
                {
                case Cell::SAND:
                    DrawRectangle(x * m_CellSize, y * m_CellSize, m_CellSize, m_CellSize, YELLOW);
                    break;
                case Cell::WATER:
                    DrawRectangle(x * m_CellSize, y * m_CellSize, m_CellSize, m_CellSize, BLUE);
                    break;
                case Cell::ROCK:
                    DrawRectangle(x * m_CellSize, y * m_CellSize, m_CellSize, m_CellSize, GRAY);
                    break;
                case Cell::OIL:
                    DrawRectangle(x * m_CellSize, y * m_CellSize, m_CellSize, m_CellSize, BLACK);
                    break;
                case Cell::FIRE:
                    DrawRectangle(x * m_CellSize, y * m_CellSize, m_CellSize, m_CellSize, ORANGE);
                    break;
                default:
                    break;                        
                }
            }
        }
    }


private:
    void updateSand(int x, int y)
    {   
        int dir = ((rand() % 2) == 0) ? 1 : -1;

        if (canMove(Cell::SAND, x, y+1))
        {
            int newY = y;
            for (int i = 1; i <= m_CellSpeed; ++i)
            {
                if (canMove(Cell::SAND, x, y + i))
                {
                    newY = y + i;
                }
                else break;
            }    
            move(x, y, x, newY);  
        }
        else if (canMove(Cell::SAND, x+dir, y+1))
        {
            move(x, y, x+dir, y+1);
        }
        else if (canMove(Cell::SAND, x-dir, y+1))
        {
            move(x, y, x-dir, y+1);
        }
    }

    void updateWater(int x, int y)
    {   
        int dir = ((rand() % 2) == 0) ? 1 : -1;

        int newX = x;
        int newY = y;

        if (canMove(Cell::WATER, x, y + 1))
        {
            for (int i = 1; i <= m_CellSpeed; ++i)
            {
                if (canMove(Cell::WATER, x, y + i))
                {
                    newY = y + i;
                }
                else break;
            }  
        }
        else if (canMove(Cell::WATER, x + dir, y + 1))
        {
            for (int i = 1; i <= m_CellSpeed; ++i)
            {
                if (canMove(Cell::WATER, x + i * dir, y + i))
                {
                    newY = y + i;
                    newX = x + i * dir;
                }
                else break;
            } 
        }
        else if (canMove(Cell::WATER, x - dir, y + 1))
        {
            for (int i = 1; i <= m_CellSpeed; ++i)
            {
                if (canMove(Cell::WATER, x - i * dir, y + i))
                {
                    newY = y + i;
                    newX = x - i * dir;
                }
                else break;
            } 
        }
        else if (canMove(Cell::WATER, x + dir, y))
        {
            for (int i = 1; i <= m_CellSpeed; ++i)
            {
                if (canMove(Cell::WATER, x + i * dir, y))
                {
                    newX = x + i * dir;
                }
                else break;
            } 
        }
        else if (canMove(Cell::WATER, x - dir, y))
        {
            for (int i = 1; i <= m_CellSpeed; ++i)
            {
                if (canMove(Cell::WATER, x - i * dir, y))
                {
                    newX = x - i * dir;
                }
                else break;
            } 
        }

        move(x, y, newX, newY);
    }

    void updateOil(int x, int y)
    {   
        int oilSpeed = (int)roundf(m_CellSpeed * 0.2);
        if (oilSpeed < 1) oilSpeed = 1;
    
        int dir = ((rand() % 2) == 0) ? 1 : -1;

        int newX = x;
        int newY = y;

        if (canMove(Cell::OIL, x, y + 1))
        {
            for (int i = 1; i <= (int)roundf(m_CellSpeed * 0.5f); ++i)
            {
                if (canMove(Cell::OIL, x, y + i))
                {
                    newY = y + i;
                }
                else break;
            }  
        }
        else if (canMove(Cell::OIL, x + dir, y + 1))
        {
            for (int i = 1; i <= oilSpeed; ++i)
            {
                if (canMove(Cell::OIL, x + i * dir, y + i))
                {
                    newY = y + i;
                    newX = x + i * dir;
                }
                else break;
            } 
        }
        else if (canMove(Cell::OIL, x - dir, y + 1))
        {
            for (int i = 1; i <= oilSpeed; ++i)
            {
                if (canMove(Cell::OIL, x - i * dir, y + i))
                {
                    newY = y + i;
                    newX = x - i * dir;
                }
                else break;
            } 
        }
        else if (canMove(Cell::OIL, x + dir, y))
        {
            for (int i = 1; i <= oilSpeed; ++i)
            {
                if (canMove(Cell::OIL, x + i * dir, y))
                {
                    newX = x + i * dir;
                }
                else break;
            } 
        }
        else if (canMove(Cell::OIL, x - dir, y))
        {
            for (int i = 1; i <= oilSpeed; ++i)
            {
                if (canMove(Cell::OIL, x - i * dir, y))
                {
                    newX = x - i * dir;
                }
                else break;
            } 
        }

        move(x, y, newX, newY);
    }

    void updateFire(int x, int y)
    {
        int fireSpread = 100;

        int dir = ((rand() % 2) == 0) ? 1 : -1;

        if (canMove(Cell::FIRE, x + dir, y))
        {
            if (rand() % fireSpread) m_Next[getKey(x + dir, y)] = Cell::FIRE;
        }
        else if (canMove(Cell::FIRE, x - dir, y))
        {
            if (rand() % fireSpread) m_Next[getKey(x - dir, y)] = Cell::FIRE;
        }
        else if (canMove(Cell::FIRE, x, y + dir))
        {
            if (rand() % fireSpread) m_Next[getKey(x, y + dir)] = Cell::FIRE;
        }
        else if (canMove(Cell::FIRE, x, y - dir))
        {
            if (rand() % fireSpread) m_Next[getKey(x, y - dir)] = Cell::FIRE;
        }
        else m_Next[getKey(x, y)] = Cell::AIR;
    }

    void move(int x, int y, int nextX, int nextY)
    {   
        Cell movingCell = m_Next[getKey(x, y)];
        Cell nextCell = m_Next[getKey(nextX, nextY)];

        if (nextCell == Cell::WATER || nextCell == Cell::OIL) 
        {
            m_Next[getKey(x, y)] = nextCell;
        }
        else m_Next[getKey(x, y)] = Cell::AIR;

        m_Next[getKey(nextX, nextY)] = movingCell;
    }

    Vector2 mouseCell(Vector2 mousePos)
    {
        return {mousePos.x / m_CellSize, mousePos.y / m_CellSize};
    }

    bool inCols(int x) {return x >= 0 && x < m_Cols;}
    bool inRows(int y) {return y >= 0 && y < m_Rows;}
    bool inGrid(int x, int y) {return inCols(x) && inRows(y);}

    bool canMove(Cell cell, int x, int y)
    {   
        if (!inGrid(x, y)) return false;

        if (cell == Cell::AIR) return true;

        if (cell == Cell::SAND) return m_Next[getKey(x, y)] == Cell::AIR || 
                                       m_Next[getKey(x, y)] == Cell::WATER ||
                                       m_Next[getKey(x, y)] == Cell::FIRE;

        if (cell == Cell::WATER) return m_Next[getKey(x, y)] == Cell::AIR || 
                                        m_Next[getKey(x, y)] == Cell::OIL ||
                                        m_Next[getKey(x, y)] == Cell::FIRE;

        if (cell == Cell::OIL) return m_Next[getKey(x, y)] == Cell::AIR;

        if (cell == Cell::ROCK) return true;

        if (cell == Cell::FIRE) return m_Next[getKey(x, y)] == Cell::OIL;

        return false;
    }

    int getKey(int x, int y) {return y * m_Cols + x;}
};

std::string cellToString(Cell cell)
{
    switch (cell)
    {
    case Cell::AIR: return "Air";
    case Cell::SAND: return "Sand";
    case Cell::WATER: return "Water";
    case Cell::OIL: return "Oil";
    case Cell::ROCK: return "Rock";
    case Cell::FIRE: return "Fire";
    default: return "";
    }
}

int main(int argc, char* argv[])
{
    // initialization

    int screenWidth = 1920;
    int screenHeight = 1080;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "SandSim");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    // setup grid

    const int cellSize = 2;
    const int cols = ceilf((float)screenWidth / (float)cellSize);
    const int rows = ceilf((float)screenHeight / (float)cellSize);
    const int cellSpeed = 6;
    Grid grid(cols, rows, cellSize, cellSpeed);

    std::vector<Cell> cellOptions = {Cell::AIR, Cell::SAND, Cell::WATER, 
                                     Cell::OIL, Cell::ROCK, Cell::FIRE};
    size_t cellSelected = 1;
    int spawnRadius = 2;

    //game loop

    while (!WindowShouldClose())
    {   
        // input

        if (IsMouseButtonDown(0)) grid.spawnCells(cellOptions[cellSelected], spawnRadius);

        // update

        grid.update();

        // render

        BeginDrawing();
        ClearBackground(SKYBLUE);

        grid.render();

        rlImGuiBegin();

        static bool open = true;

        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({1.f/5.f * (float)screenWidth, (float)screenHeight});

        ImGui::Begin("Falling Sand Simulation", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        ImGui::SliderInt("Spawn Radius", &spawnRadius, 0, 20);

        if (ImGui::BeginCombo("Material", cellToString(cellOptions[cellSelected]).c_str()))
        {
            for (size_t i = 0; i < cellOptions.size(); ++i)
            {
                const bool selected = (cellSelected == i);
                if (ImGui::Selectable(cellToString(cellOptions[i]).c_str(), selected))
                {
                    cellSelected = i;
                }
                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::End();
        rlImGuiEnd();

        EndDrawing();
    } 

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}