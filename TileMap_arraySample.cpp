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
    '0', // 주인공
    '&', // 적
    ' ', // road
    '-', // 가로 벽
    '|', // 세로 벽
};

struct Tile
{
    // 타일종류
    int tileType; 
    // 사용할 이미지
    int imageType;
    // 충돌여부
    int collision;
};

// 타일종류
enum ETileType
{
    PLAYER, //0
    ENEMY,  //1
    ROAD,   //2
    HWALL,  //3
    VWALL,  //4
    TILE_END
};
// 방향
#define LEFT -1;
#define RIGHT 1;
#define UP -1;
#define DOWN 1;


void main()
{
    // 동적 배열을 사용하기 위해 constexpr 을 사용
    // 컴파일타임에 상수로 변환
    constexpr int nTileX = 9;
    constexpr int nTileY = (sizeof(tileMap)/4)/nTileX;
    // 현재 위치
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
    // 타일 속성 배열
    // 동적 배열은 C++ 에서는 지원하지 않기 때문에
    // nTileX, Y 를 constexpr 로 선언해 사용
    Tile tiles[nTileX * nTileY] = {};

    // 플래이어와, 적 위치 설정
    // 타일 출력
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
   
    // 게임루프
    char cInput;
    while (true)
    {
        system("cls");

        // 타일 출력
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
            cout << "=============== 게임오버 ===============" << endl;
            cout << "========================================" << endl;
            cout << endl;
            system("pause");
            break;
        }

        cout << endl;
        cout << "q : 종료" << endl;
        cout << "wasd 방향을 입력하시오 ";

        cInput = _getch();

        if (cInput == 'q')
        {
            break;
        }
        
        // 이동방향 구하기
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
        // 타일구조체 업데이트
        tiles[nNewPos] = tiles[nPlayerPos];
        tiles[nPlayerPos] = newTile;
        nPlayerPos = nNewPos;

        // ai가 이동할 방향구하기(target 쪽으로)
        // clamp 자르기 max(-1, min(x, 1))
        int nEnemyX = nEnemyPos % nTileX;
        int nEnemyY = nEnemyPos / nTileX;
        int nHorDist = abs(nNewTilePosX - nEnemyX);
        int nVerDist = abs(nNewTilePosY - nEnemyY);
        // 수직, 수평중 크기가 더 큰 곳으로 이동 방향을 잡는다.
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
        // 적과 플래이어가 만나면 게임오버
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
        // 타일구조체 업데이트
        tiles[nNext] = tiles[nEnemyPos];
        tiles[nEnemyPos] = nextTile;
        nEnemyPos = nNext;
    }
}

