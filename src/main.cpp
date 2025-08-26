#include <raylib.h>
#include <raymath.h>

#include "imgui.h"
#include "rlImGui.h"

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
    FIRE = 5,
    STEAM = 6,
    LAVA = 7,
    OBSIDIAN = 8,
    GLASS = 9
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
    case Cell::STEAM: return "Steam";
    case Cell::LAVA: return "Lava";
    case Cell::OBSIDIAN: return "Obsidian";
    case Cell::GLASS: return "Glass";
    default: return "";
    }
}

Color getCellColor(Cell cell)
{
    switch (cell)
    {
    case Cell::AIR: return BLANK;
    case Cell::SAND: return YELLOW;
    case Cell::WATER: return BLUE;
    case Cell::OIL: return BLACK;
    case Cell::ROCK: return GRAY;
    case Cell::FIRE: return RED;
    case Cell::STEAM: return WHITE;
    case Cell::LAVA: return ORANGE;
    case Cell::OBSIDIAN: return DARKPURPLE;
    case Cell::GLASS: return {100, 150, 200, 120};
    default: return BLANK;
    }
}

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
        m_Next = std::vector<Cell> (m_Cols * m_Rows, Cell::AIR);
    }

    ~Grid() = default;

    void spawnCells(Cell selected, int spawnRadius)
    {
        Vector2 mousePos = GetMousePosition();

        for (int x = (int)mouseCell(mousePos).x - spawnRadius; 
             x < (int)mouseCell(mousePos).x + spawnRadius; ++x)
        {
            for (int y = (int)mouseCell(mousePos).y - spawnRadius;
                 y < (int)mouseCell(mousePos).y + spawnRadius; ++y)
            {
                if (canMove(selected, x, y)) m_Cells[getKey(x, y)] = selected;
            }
        }
    }

    void update()
    {
        m_Next.assign(m_Cells.begin(), m_Cells.end());

        for (int x = 0; x < m_Cols; ++x)
        {
            for (int y = 0; y < m_Rows; ++y)
            {   
                if (m_Cells[getKey(x, y)] == Cell::AIR) continue;

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
                case Cell::STEAM:
                    updateSteam(x, y);
                    break;
                case Cell::LAVA:
                    updateLava(x, y);
                    break;
                default:
                    break;
                }
            }
        }   

        m_Cells.assign(m_Next.begin(), m_Next.end());
    }

    void render()
    {
        for (int x = 0; x < m_Cols; ++x)
        {
            for (int y = 0; y < m_Rows; ++y)
            {
                if (m_Cells[getKey(x, y)] != Cell::AIR) 
                {
                    DrawRectangle(x * m_CellSize, y * m_CellSize, m_CellSize,
                                  m_CellSize, getCellColor(m_Cells[getKey(x, y)]));
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
        int fireSpread = 20;

        int dir = ((rand() % 2) == 0) ? 1 : -1;

        if (m_Cells[getKey(x + dir, y)] == Cell::OIL)
        {
            if (rand() % fireSpread == 0) m_Next[getKey(x + dir, y)] = Cell::FIRE;
        }
        else if (m_Cells[getKey(x - dir, y)] == Cell::OIL)
        {
            if (rand() % fireSpread == 0) m_Next[getKey(x - dir, y)] = Cell::FIRE;
        }
        else if (m_Cells[getKey(x, y + dir)] == Cell::OIL)
        {
            if (rand() % fireSpread == 0) m_Next[getKey(x, y + dir)] = Cell::FIRE;
        }
        else if (m_Cells[getKey(x, y - dir)] == Cell::OIL)
        {
            if (rand() % fireSpread == 0) m_Next[getKey(x, y - dir)] = Cell::FIRE;
        }
        else if (m_Cells[getKey(x + dir, y)] == Cell::WATER)
        {
            m_Next[getKey(x + dir, y)] = Cell::STEAM;
            m_Next[getKey(x, y)] = Cell::AIR;
        }
        else if (m_Cells[getKey(x - dir, y)] == Cell::WATER)
        {
            m_Next[getKey(x - dir, y)] = Cell::STEAM;
            m_Next[getKey(x, y)] = Cell::AIR;
        }
        else if (m_Cells[getKey(x, y + dir)] == Cell::WATER)
        {
            m_Next[getKey(x, y + dir)] = Cell::STEAM;
            m_Next[getKey(x, y)] = Cell::AIR;
        }
        else if (m_Cells[getKey(x, y - dir)] == Cell::WATER)
        {
            m_Next[getKey(x, y - dir)] = Cell::STEAM;
            m_Next[getKey(x, y)] = Cell::AIR;
        }
        else if (m_Cells[getKey(x + dir, y)] == Cell::SAND)
        {
            m_Next[getKey(x + dir, y)] = Cell::GLASS;
            m_Next[getKey(x, y)] = Cell::AIR;
        }
        else if (m_Cells[getKey(x - dir, y)] == Cell::SAND)
        {
            m_Next[getKey(x - dir, y)] = Cell::GLASS;
            m_Next[getKey(x, y)] = Cell::AIR;
        }
        else if (m_Cells[getKey(x, y + dir)] == Cell::SAND)
        {
            m_Next[getKey(x, y + dir)] = Cell::GLASS;
            m_Next[getKey(x, y)] = Cell::AIR;
        }
        else if (m_Cells[getKey(x, y - dir)] == Cell::SAND)
        {
            m_Next[getKey(x, y - dir)] = Cell::GLASS;
            m_Next[getKey(x, y)] = Cell::AIR;
        }
        else if (rand() % 5 == 0) m_Next[getKey(x, y)] = Cell::AIR;
    }

    void updateSteam(int x, int y)
    {
        int steamSpeed = (int)roundf(m_CellSpeed * 0.5);
        if (steamSpeed < 1) steamSpeed = 1;

        int dir = ((rand() % 2) == 0) ? 1 : -1;

        int newX = x;
        int newY = y;

        if (canMove(Cell::STEAM, x, y - 1))
        {
            newY = y - 1;
        }
        else if (canMove(Cell::STEAM, x + dir, y - 1))
        {
            for (int i = 1; i <= steamSpeed; ++i)
            {
                if (canMove(Cell::STEAM, x + i * dir, y - i))
                {
                    newY = y - i;
                    newX = x + i * dir;
                }
                else break;
            } 
        }
        else if (canMove(Cell::STEAM, x - dir, y - 1))
        {
            for (int i = 1; i <= steamSpeed; ++i)
            {
                if (canMove(Cell::STEAM, x - i * dir, y - i))
                {
                    newY = y - i;
                    newX = x - i * dir;
                }
                else break;
            } 
        }
        else if (canMove(Cell::STEAM, x + dir, y))
        {
            for (int i = 1; i <= steamSpeed; ++i)
            {
                if (canMove(Cell::STEAM, x + i * dir, y))
                {
                    newX = x + i * dir;
                }
                else break;
            } 
        }
        else if (canMove(Cell::STEAM, x - dir, y))
        {
            for (int i = 1; i <= steamSpeed; ++i)
            {
                if (canMove(Cell::STEAM, x - i * dir, y))
                {
                    newX = x - i * dir;
                }
                else break;
            } 
        }

        if (rand() % 10000 == 0) m_Next[getKey(x, y)] = Cell::WATER;

        move(x, y, newX, newY); 
    }

    void updateLava(int x, int y)
    {
        int dir = ((rand() % 2) == 0) ? 1 : -1;

        int newX = x;
        int newY = y;


        if (m_Cells[getKey(x, y + dir)] == Cell::WATER)
        {
            m_Next[getKey(x, y + dir)] = Cell::OBSIDIAN;
        }
        else if (m_Cells[getKey(x, y - dir)] == Cell::WATER)
        {
            m_Next[getKey(x, y - dir)] = Cell::OBSIDIAN;
        }
        else if (m_Cells[getKey(x + dir, y)] == Cell::WATER)
        {
            m_Next[getKey(x + dir, y)] = Cell::OBSIDIAN;
        }
        else if (m_Cells[getKey(x - dir, y)] == Cell::WATER)
        {
            m_Next[getKey(x - dir, y)] = Cell::OBSIDIAN;
        }
        else if (m_Cells[getKey(x + dir, y)] == Cell::OIL)
        {
            m_Next[getKey(x + dir, y)] = Cell::FIRE;
        }
        else if (m_Cells[getKey(x - dir, y)] == Cell::OIL)
        {
            m_Next[getKey(x - dir, y)] = Cell::FIRE;
        }
        else if (m_Cells[getKey(x, y + dir)] == Cell::OIL)
        {
            m_Next[getKey(x, y + dir)] = Cell::FIRE;
        }
        else if (m_Cells[getKey(x, y - dir)] == Cell::OIL)
        {
            m_Next[getKey(x, y - dir)] = Cell::FIRE;
        }
        else if (m_Cells[getKey(x + dir, y)] == Cell::SAND)
        {
            m_Next[getKey(x + dir, y)] = Cell::GLASS;
        }
        else if (m_Cells[getKey(x - dir, y)] == Cell::SAND)
        {
            m_Next[getKey(x - dir, y)] = Cell::GLASS;
        }
        else if (m_Cells[getKey(x, y + dir)] == Cell::SAND)
        {
            m_Next[getKey(x, y + dir)] = Cell::GLASS;
        }
        else if (m_Cells[getKey(x, y - dir)] == Cell::SAND)
        {
            m_Next[getKey(x, y - dir)] = Cell::GLASS;
        }
        else if (canMove(Cell::LAVA, x, y + 1))
        {
            for (int i = 1; i <= m_CellSpeed; ++i)
            {
                if (canMove(Cell::LAVA, x, y + i))
                {
                    newY = y + i;
                }
                else break;
            }  
        }
        else if (canMove(Cell::LAVA, x + dir, y + 1))
        {
            if (rand() % 3 == 0)
            {
                newY = y + 1;
                newX = x + dir;
            }
        }
        else if (canMove(Cell::LAVA, x - dir, y + 1))
        {
            if (rand() % 3 == 0)
            {
                newY = y + 1;
                newX = x - dir;
            }
        }
        else if (canMove(Cell::LAVA, x + dir, y))
        {
            if (rand() % 3 == 0) newX = x + dir;
        }
        else if (canMove(Cell::LAVA, x - dir, y))
        {
            if (rand() % 3 == 0) newX = x - dir;
        }

        move(x, y, newX, newY);        
    }

    void move(int x, int y, int nextX, int nextY)
    {   
        Cell movingCell = m_Next[getKey(x, y)];
        Cell nextCell = m_Next[getKey(nextX, nextY)];

        if (nextCell == Cell::WATER || nextCell == Cell::OIL || nextCell == Cell::LAVA) 
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
                                       m_Next[getKey(x, y)] == Cell::FIRE ||
                                       m_Next[getKey(x, y)] == Cell::OIL ||
                                       m_Next[getKey(x, y)] == Cell::LAVA;

        if (cell == Cell::WATER) return m_Next[getKey(x, y)] == Cell::AIR || 
                                        m_Next[getKey(x, y)] == Cell::OIL ||
                                        m_Next[getKey(x, y)] == Cell::FIRE;

        if (cell == Cell::OIL) return m_Next[getKey(x, y)] == Cell::AIR;

        if (cell == Cell::ROCK) return true;        

        if (cell == Cell::FIRE) return m_Next[getKey(x, y)] == Cell::OIL ||
                                       m_Next[getKey(x, y)] == Cell::WATER ||
                                       m_Next[getKey(x, y)] == Cell::AIR;

        if (cell == Cell::STEAM) return m_Next[getKey(x, y)] == Cell::AIR ||
                                        m_Next[getKey(x, y)] == Cell::WATER ||
                                        m_Next[getKey(x, y)] == Cell::OIL ||
                                        m_Next[getKey(x, y)] == Cell::LAVA;

        if (cell == Cell::LAVA) return m_Next[getKey(x, y)] == Cell::AIR || 
                                       m_Next[getKey(x, y)] == Cell::OIL ||
                                       m_Next[getKey(x, y)] == Cell::FIRE ||
                                       m_Next[getKey(x, y)] == Cell::WATER;

        if (cell == Cell::OBSIDIAN) return true;

        if (cell == Cell::GLASS) return true;

        return false;
    }

    int getKey(int x, int y) {return y * m_Cols + x;}
};

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

    const int cellSize = 3;
    const int cols = ceilf((float)screenWidth / (float)cellSize);
    const int rows = ceilf((float)screenHeight / (float)cellSize);
    const int cellSpeed = 5;
    Grid grid(cols, rows, cellSize, cellSpeed);

    std::vector<Cell> cellOptions = {Cell::AIR, Cell::SAND, Cell::WATER, 
                                     Cell::OIL, Cell::ROCK, Cell::FIRE,
                                     Cell::STEAM, Cell::LAVA, Cell::OBSIDIAN,
                                     Cell::GLASS};
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

        // render gui

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