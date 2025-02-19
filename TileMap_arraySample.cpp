#include <iostream>
// _getch
#include <conio.h>
// min, max
#include <algorithm>
#include <ctime>

using namespace std;

int tileMap[]
{
    3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 2, 2, 2, 2, 2, 2, 2, 4,
    4, 2, 2, 2, 2, 2, 2, 2, 4,
    4, 2, 2, 2, 2, 2, 2, 2, 4,
    4, 2, 2, 2, 2, 2, 2, 2, 4,
    4, 2, 2, 2, 2, 2, 2, 2, 4,
    4, 2, 2, 2, 2, 2, 2, 2, 4,
    4, 2, 2, 2, 2, 2, 2, 2, 4,
    4, 2, 2, 2, 2, 2, 2, 2, 4,
    4, 2, 2, 2, 2, 2, 2, 2, 4,
    3, 3, 3, 3, 3, 3, 3, 3, 3,

};

const char tileImage[] =
{
    '0', // ���ΰ�
    '&', // ��
    ' ', // road
    '-', // ���� ��
    '|', // ���� ��
};

struct Tile
{
    // Ÿ������
    int tileType; 
    // ����� �̹���
    int imageType;
    // �浹����
    int collision;
};

// Ÿ������
enum ETileType
{
    PLAYER, //0
    ENEMY,  //1
    ROAD,   //2
    HWALL,  //3
    VWALL,  //4
    TILE_END
};
// ����
#define LEFT -1;
#define RIGHT 1;
#define UP -1;
#define DOWN 1;


void main()
{
    // ���� �迭�� ����ϱ� ���� constexpr �� ���
    // ������Ÿ�ӿ� ����� ��ȯ
    constexpr int nTileX = 9;
    constexpr int nTileY = (sizeof(tileMap)/4)/nTileX;
    // ���� ��ġ
    int nPlayerPos = 0;
    int nEnemyPos = 0;
    bool bGameover = false;

    srand((unsigned int)time(0));
    // Player
    int y = rand() % (nTileY -2) + 1;
    int x = rand() % (nTileX -2) + 1;
    tileMap[x + y * nTileX] = PLAYER;

    while (true)
    {
        // Enemy
        int ey = rand() % (nTileY - 2) + 1;
        int ex = rand() % (nTileX - 2) + 1;
        int index = ex + ey * nTileX;
        if (tileMap[index] == PLAYER)
        {
            continue;
        }
        tileMap[index] = ENEMY;
        break;
    }
    // Ÿ�� �Ӽ� �迭
    // ���� �迭�� C++ ������ �������� �ʱ� ������
    // nTileX, Y �� constexpr �� ������ ���
    Tile tiles[nTileX * nTileY] = {};

    // �÷��̾��, �� ��ġ ����
    // Ÿ�� ���
    for (int i = 0; i < nTileY; i++)
    {
        for (int j = 0; j < nTileX; j++)
        {
            int curIndex = i * nTileX + j;
            int type = tileMap[curIndex];
            tiles[curIndex].tileType = type;
            tiles[curIndex].imageType = type;
            switch (type)
            {
                case PLAYER:
                    nPlayerPos = i * nTileX + j;
                    tiles[curIndex].collision = 0;
                    break;
                case ENEMY:
                    nEnemyPos = i * nTileX + j;
                    tiles[curIndex].collision = 0;
                    break;
                case ROAD:
                    tiles[curIndex].collision = 0;
                    break;
                case HWALL:
                    tiles[curIndex].collision = 1;
                    break;
                case VWALL:
                    tiles[curIndex].collision = 1;
                    break;

            }
        }
        cout << endl;
    }
   
    // ���ӷ���
    char cInput;
    while (true)
    {
        system("cls");

        // Ÿ�� ���
        for (int i = 0; i < nTileY; i++)
        {
            for (int j = 0; j < nTileX; j++)
            {
                int curIndex = i * nTileX + j;
                Tile tile = tiles[curIndex];
                cout << tileImage[tile.imageType] << " ";
            }
            cout << endl;
        }
       
        if (bGameover)
        {
            cout << "========================================" << endl;
            cout << "=============== ���ӿ��� ===============" << endl;
            cout << "========================================" << endl;
            cout << endl;
            system("pause");
            break;
        }

        cout << endl;
        cout << "q : ����" << endl;
        cout << "wasd ������ �Է��Ͻÿ� ";

        cInput = _getch();

        if (cInput == 'q')
        {
            break;
        }
        
        // �̵����� ���ϱ�
        int nHorizontal = 0;
        int nVertical = 0;
        switch (cInput)
        {
            case 'w':
                nVertical = UP;
                break;
            case 's':
                nVertical = DOWN;
                break;
            case 'a':
                nHorizontal = LEFT;
                break;
            case 'd':
                nHorizontal = RIGHT;
                break;
            default:
                continue;
                break;
        }
        
        int nNewTilePosX = (nPlayerPos % nTileX) + nHorizontal;
        int nNewTilePosY = nPlayerPos / nTileX + nVertical;

        int nNewPos = nNewTilePosY * nTileX + nNewTilePosX;
        Tile newTile = tiles[nNewPos];
        
        //if (newTile.tileType != ROAD && newTile.tileType != ENEMY)
        if (newTile.collision)
        {
            continue;
        }

        tileMap[nPlayerPos] = tileMap[nNewPos];
        tileMap[nNewPos] = PLAYER;
        // Ÿ�ϱ���ü ������Ʈ
        tiles[nNewPos] = tiles[nPlayerPos];
        tiles[nPlayerPos] = newTile;
        nPlayerPos = nNewPos;

        // ai�� �̵��� ���ⱸ�ϱ�(target ������)
        // clamp �ڸ��� max(-1, min(x, 1))
        int nEnemyX = nEnemyPos % nTileX;
        int nEnemyY = nEnemyPos / nTileX;
        int nHorDist = abs(nNewTilePosX - nEnemyX);
        int nVerDist = abs(nNewTilePosY - nEnemyY);
        // ����, ������ ũ�Ⱑ �� ū ������ �̵� ������ ��´�.
        nHorizontal = nVertical = 0;

        if (nHorDist > nVerDist)
        {
            nHorizontal = max(-1, min(nNewTilePosX - nEnemyX, 1));
        }
        else
        {
            nVertical = max(-1, min(nNewTilePosY - nEnemyY, 1));
        }

        int nNewX = nEnemyX + nHorizontal;
        int nNewY = nEnemyY + nVertical;
        int nNext = nNewY * nTileX + nNewX;
        Tile nextTile = tiles[nNext];
        // ���� �÷��̾ ������ ���ӿ���
        if (tileMap[nPlayerPos] == ENEMY || tileMap[nNext] == PLAYER)
        {
            bGameover = true;
            tileMap[nNext] = ENEMY;
            tileMap[nEnemyPos] = ROAD;
            tiles[nNext].tileType = ENEMY;
            tiles[nNext].imageType = ENEMY;
            tiles[nEnemyPos].tileType = ROAD;
            tiles[nEnemyPos].imageType = ROAD;
            nEnemyPos = nNext;
            continue;
        }
        tileMap[nEnemyPos] = tileMap[nNext];
        tileMap[nNext] = ENEMY;
        // Ÿ�ϱ���ü ������Ʈ
        tiles[nNext] = tiles[nEnemyPos];
        tiles[nEnemyPos] = nextTile;
        nEnemyPos = nNext;
    }
}

