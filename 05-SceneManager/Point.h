#include "GameObject.h"

#define SCORE_SPRITE_100_ID		50049
#define SCORE_SPRITE_200_ID		50050
#define SCORE_SPRITE_400_ID		50051
#define SCORE_SPRITE_800_ID		50052
#define SCORE_SPRITE_1000_ID	50053
#define SCORE_SPRITE_2000_ID	50054
#define SCORE_SPRITE_4000_ID	50055
#define SCORE_SPRITE_8000_ID	50056
#define SCORE_SPRITE_1UP_ID		50057


#define SCORE_SPEED 0.05f

#define SCORE_TIME 500

class Point : public CGameObject
{
    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
public:
    Point(int score = 100);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    int IsBlocking() { return 0; }
    int IsBlocking(bool isBlocking) { return isBlocking; }
};


