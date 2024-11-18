#include "Level.h"

LEVEL::LEVEL(const int level) {
    char buffer[50];

    setLevel(level);
}

LEVEL::~LEVEL()
{
    for(int i=0;i < NumOfGrid;i++)
    {
        levelMap[i].roadPoint = false;
    }
}

void
LEVEL::setLevel(const int level)
{
    char buffer[50];
    FILE *file;

    sprintf(buffer, "./Level/Level%d.txt", level);
    file = fopen(buffer, "r");

    this->level = level;
    this->road_grid.clear();

    for(int i = 0; i < NumOfGrid; i++)
    {
        this->levelMap[i].roadPoint = false;
    }

    while(fscanf(file, "%s", buffer) != EOF) {
        this->levelMap[atoi(buffer)-1].roadPoint = true;
        road_grid.push_back(atoi(buffer)-1);
    }

    sprintf(buffer, "./Level/Crossroad%d.txt", level);
    file = fopen(buffer, "r");
    this->crossroad.clear();
    while(fscanf(file, "%s", buffer) != EOF) {
        crossroad.insert(atoi(buffer)-1);
    }

    fclose(file);
}
