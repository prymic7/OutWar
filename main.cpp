
#include "includes/raylib.h"
#include "includes/raymath.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
int screenWidth = 1600;
int screenHeight = 800;

using std::cout;
using std::endl;

#undef main


class Terrain {
public:
    Image skyImg = LoadImage("img/sky.png");
    Image lavaImg = LoadImage("img/lava.png");
    Texture2D skyTexture = LoadTextureFromImage(skyImg);
    Texture2D lavaTexture = LoadTextureFromImage(lavaImg);
    int oneRectWidth = 25;
    int terrainRectsCount = screenWidth/oneRectWidth;
    int terrainRectMaxY = 650;
    int terrainRectMinY = 200;
    int terrainYDiff = 15;
    int terrainYNow = 0;
    int terrainXNow = 0;
    int terrainStartY = 0;
    int lastYMovement = 0;
    int straightYs = 0;
    int straightYsMax = 20;
    bool lowRepeatY = false;
    std::vector<int> continueYMovement = {0,1};
    int continueYMovementInt = 2;
    Color customBrown = {138, 51, 36, 255};
    std::vector<int> possibleFirstY = {250,300,350,400,450,500,550};
    std::vector<Rectangle> terrainRects = {};

    Terrain(){
        createTerrain();
    }

    int returnOneRectWidth(){
        return oneRectWidth;
    }

    int returnTerrainYDiff(){
        return terrainYDiff;
    }

    // void showRects(){
    //     for(Rectangle r : terrainRects)
    // }

    void restartTerrain(){
        terrainRects.clear();
        oneRectWidth = 25;
        terrainRectsCount = screenWidth/oneRectWidth;
        terrainRectMaxY = 650;
        terrainRectMinY = 200;
        terrainYDiff = 15;
        terrainYNow = 0;
        terrainXNow = 0;
        terrainStartY = 0;
        lastYMovement = 0;
        straightYs = 0;
        straightYsMax = 20;
        lowRepeatY = false;
        continueYMovement = {0,1};
        continueYMovementInt = 2;
        createTerrain();
    }

    void createTerrain(){

        for(int i = 0; i < terrainRectsCount; i++){
            
            if(i == 0){
                terrainYNow = possibleFirstY[rand() % possibleFirstY.size()]; 
            } else if(i == 1){
                int luckyNum = rand() % 50;
                int ted = terrainYNow - 25 + luckyNum;
                if(ted < terrainYNow){
                    lastYMovement = 0;
                    terrainYNow = terrainYNow - terrainYDiff;
                } else if(ted >= terrainYNow){
                    lastYMovement = 1;
                    terrainYNow = terrainYNow + terrainYDiff;
                }
            } else if(i != 0){
                int luckyNumContinueYMovement = rand() % continueYMovement.size();
                if(terrainYNow > terrainRectMaxY){
                    terrainYNow = terrainYNow - terrainYDiff;
                    lastYMovement = 0;
                } else if(terrainYNow < terrainRectMinY){
                    terrainYNow = terrainYNow + terrainYDiff;
                    lastYMovement = 1;
                }
                
                else if(straightYs == straightYsMax){
                    straightYs = 0;
                    continueYMovement.clear();
                    continueYMovement.push_back(0);
                    continueYMovement.push_back(1);
                    if(lastYMovement == 0){
                        terrainYNow = terrainYNow + terrainYDiff;
                        lastYMovement = 1;
                    } else if(lastYMovement == 1){
                        terrainYNow = terrainYNow - terrainYDiff;
                        lastYMovement = 0;
                    }
                }
                else if(continueYMovement[luckyNumContinueYMovement] == 1){
                    straightYs = 0;
                    if(lastYMovement == 0){
                        terrainYNow = terrainYNow + terrainYDiff;
                        lastYMovement = 1;
                    } else if(lastYMovement == 1){
                        terrainYNow = terrainYNow - terrainYDiff;
                        lastYMovement = 0;
                    }
                    continueYMovement.clear();
                    continueYMovement.push_back(0);
                    continueYMovement.push_back(1);

                   
                } else if(continueYMovement[luckyNumContinueYMovement] != 1){
                    straightYs++;
                    if(lastYMovement == 0){
                        terrainYNow = terrainYNow - terrainYDiff;
                    } else if(lastYMovement == 1){
                        terrainYNow = terrainYNow + terrainYDiff;
                    }
                    continueYMovement.push_back(continueYMovementInt);
                    continueYMovementInt++;  
                }
            }
            Rectangle rectangle = {(float)terrainXNow, (float)terrainYNow, (float)oneRectWidth, (float)screenHeight - terrainYNow};
            terrainRects.push_back(rectangle);
            
            terrainXNow += oneRectWidth;
        }
    }

    void drawTerrain(){
        DrawTexture(skyTexture, -100, 0, WHITE);
        for(Rectangle rect : terrainRects){
            DrawRectangle((float)rect.x, (float)rect.y, (float)rect.width, (float)oneRectWidth * 2, GREEN);
            DrawRectangle((float)rect.x, (float)rect.y + oneRectWidth * 2, (float)rect.width, (float)rect.height, BROWN); 
            
        }
        // DrawTexture(lavaTexture, 0, 750, WHITE);
    }

    std::vector<Rectangle> returnTerrain(){
        return terrainRects;
    }

    void update(){

    }

    void draw(){
        drawTerrain();
    }
};

struct Bullet{
    Vector2 startPoint = {0,0};
    Vector2 position = {0,0};
    Vector2 velocity = {0,0};
    Color color = {0,0,0,0};
    int gunInHandak = 0;
    float angle = 0.0f;

    Bullet(Vector2 pos, Vector2 vel, int gun, float ang, Color c)
    : startPoint(pos), position(pos), velocity(vel), color(c), gunInHandak(gun), angle(ang) {
    }
};

struct Granade{
    Vector2 startPoint = {0,0};
    Vector2 position = {0,0};
    Vector2 velocity = {0,0};
    float granadeTrajectory;
    float angle = 0.0f;
    Color color = {0,0,0,0};
    int gunInHandak = 2;
    bool falling = false;
    float finalDeltaX = 0.0f;
    float finalDeltaY = 0.0f;

    Granade(Vector2 pos, Vector2 vel, float traj, float ang, Color c)
    : startPoint(pos), position(pos), velocity(vel), granadeTrajectory(traj), angle(ang), color(c) {
    }
};

class Fighter {
public:
    Rectangle fighterRect;
    Terrain terrain;
    int oneRectWidth = 0;
    int posX = 0;
    int posY = 0;
    Image postavicka;
    int posRectX = 0 ;
    int posRectY = 0;
    int health = 100;
    int gunInHand = 0;
    Vector2 aimPoint;
    Vector2 startBulletPoint;
    Vector2 bulletImagination;
    float cursorAngle = 0.0f;
    int speedOfFighter = 1;
    int jumpSpeed = 2;
    int macSpeed = 1500;
    int m4Speed = 1000;
    int granadeSpeed = 600;
    float foundRectY = 0.0f;
    int granadesLeft = 3;
    float granadeTrajectory = 200.0f;
    float granadeCircleRadius = 0.0f;
    float granadeTrajectoryFinishAngle = 180.0f;
    float granadeTrajectoryNowAngle;
    float granadeStraightLine = 0;
    float circleAngleNum = -45;
    float circleAngleNum2 = -45;
    float highestGranadePointX = 0.0f;
    float highestGranadePointY = 0.0f;
    std::chrono::system_clock::time_point granadeCircleChecked;
    std::chrono::system_clock::time_point granadeDetonated;
    float macRange = 400.0;
    float m4Range = 800.0;
    float granadeRange;
    int macBulletLength = 10;
    int m4BulletLength = 20;
    std::vector<Vector2> positions;
    std::vector<Rectangle> terrainRects;
    std::vector<Rectangle> currentPosRects;
    int fighterFootDif = 0;
    bool isJumping = false;
    int startJumpPos = 0;
    float standartGravity = 3;
    float jumpGravity = -2.4;
    float gravity = standartGravity;
    std::chrono::system_clock::time_point jumpTimer;
    Texture2D texturePostavicka;
    std::vector<Bullet> bullets;
    std::vector<Granade> granades;
    std::vector<Vector2> shots;
    Color aimImaginColor = {54, 69, 79, 255};

    Fighter(int x, int y, Image postavicka, std::vector<Rectangle> terrainRectsVec, int fighterDif)
    : posX(x), posY(y),  postavicka(postavicka), terrainRects(terrainRectsVec), fighterFootDif(fighterDif) {
    texturePostavicka = LoadTextureFromImage(postavicka);
    posY -= texturePostavicka.height;
    oneRectWidth = terrain.returnOneRectWidth();
    fighterFootDif = fighterDif;
    terrainRects = terrainRectsVec;
    // Other constructor logic...
    }

    void setRectangle(Rectangle rect){
        fighterRect = rect;
    }

    void deleteRects(){
        terrainRects.clear();
    }


    void updatePos(int x, int y){
        
        posX += x;
        posY += y;
    }

    void restartHealth(){
        health = 100;
    }

    void updateBulletImagination(Vector2 vec){
        bulletImagination.x = vec.x;
        bulletImagination.y = vec.y;

        
    }

    std::vector<Rectangle> returnTerrain(){
        return terrainRects;
    }

    void updateCursorAngle(float num){
        
        if(cursorAngle < 90.0 && cursorAngle > -90.0){
            cursorAngle += num;
        } else if(cursorAngle >= 90.0 && num < 0){
            cursorAngle += num;
        } else if(cursorAngle <= -90.0 && num > 0){
            cursorAngle += num;
        }   
    }

    void updateShots(Vector2 vec){
        shots.push_back(vec);
    }

    void reduceHealth(int num){
        health += num;
        
    }

    void updateGranadeTrajectory(int num){
        if(num < 0 && granadeTrajectory >= 152){
            granadeTrajectory += num;
        }
        if(num > 0 && granadeTrajectory <= 400){
            granadeTrajectory += num;
        }
        
    }

    float returnGranadeTrajectory(){
        return granadeTrajectory;
    }

    int returnBulletLength(){
        if(gunInHand == 0){
            return macBulletLength;
        } else if(gunInHand == 1){
            return m4BulletLength;
        }
        return 0;
    }

    Vector2 returnAimPoint(){
        return aimPoint;
    }

    void updateAimPoint(int x, int y){
        aimPoint.x = x;
        aimPoint.y = y;
    }

    Vector2 returnStartBulletPoint(){
        return startBulletPoint;
    }

    void updateStartBulletPoint(int x, int y){
        startBulletPoint.x = x;
        startBulletPoint.y = y;
    }

    void deleteBullets(){
        
        
        auto one = bullets.begin();
        while (one != bullets.end()) {
            // cout<<bullets.size();
            if (one->position.x < 0 || one->position.x > screenWidth || 
                one->position.y < 0 || one->position.y > screenHeight) {
                one = bullets.erase(one);
            } else {
                one++;
            }
        };
        auto two = granades.begin();
        while (two != granades.end()) {
            // cout<<bullets.size();
            if (two->position.x < 0 || two->position.x > screenWidth || 
                two->position.y < 0 || two->position.y > screenHeight) {
                two = granades.erase(two);
            } else {
                two++;
            }
        }

    }

    Texture2D returnTexture(){
        return texturePostavicka;
    }

    float reuturnCursorAngle(){
        return cursorAngle;
    }

    virtual void moveGranade(Vector2 pos, int Diff, Fighter& fighter){
        //
    }


    void setJumpTimer(){
        jumpTimer = std::chrono::system_clock::now();
    }


    int returnMovingSpeed(){
        return speedOfFighter;
    }

    int returnGunInHand(){
        return gunInHand;
    }

    void changeGunInHand(){
        if(gunInHand == 2){
            gunInHand = 0;
        } else {
            gunInHand++;
        }
    }

    virtual void calculateGranadeCircleRadius(){
        //
    }

    int returnBulletSpeed(){
        if(gunInHand == 0){
            return macSpeed;
        } else if(gunInHand == 1){
            return m4Speed;
        } else if(gunInHand == 2){
            return granadeSpeed;
        }
        
        return 0;
    }

    int returnGunRange(int num){
        if(num == 0){
            return macRange;
        } else if(num == 1){
            return m4Range;
        } else if(num == 2){
            return granadeRange;
        }
        
        return 0;
    }

    void setJumpBool(bool co){
        isJumping = co;
    }

    bool returnIsJumping(){
        return isJumping;
    }

    int returnHealth(){
        return health;
    }

    void jump(){
        std::chrono::milliseconds jumpDuration;
        if(isJumping){
            jumpDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - jumpTimer);
            gravity = jumpGravity;

        }
        
        if(jumpDuration.count() > 200 && isJumping){
            isJumping = false;
            gravity = standartGravity;
        }
    }

    Vector2 returnPos(){
        Vector2 position = {(float)posX, (float)posY};
        return position;
    }

    void draw(){
        
        drawHealth();
        DrawTexture(texturePostavicka, posX, posY, WHITE);
        aim();
        calculateAimPoint();
        drawAim();
    }

    void update(){
        
        calculateGranadeCircleRadius();
        stayOnGround();
        jump(); 
    }

    virtual void stayOnGround(){
        //  
    }

    virtual void continueMoving(int x, std::string direction){
        //
    }

    virtual int returnFighterFootDiff(){
        return 0;
    } 

    virtual void drawAim(){
        //
    }

    virtual void aim(){
        //
    }

    virtual void drawHealth(){
        //
    }

    virtual void calculateAimPoint(){
        //
    }

    virtual void shoot(){
        //
    }

    virtual void moveShot(Vector2 pos, int Diff, Fighter& fighter){
        deleteBullets();
    }
    
};

class Soldier : public Fighter{
public:
    
    bool onGround;
    float soldierFootDiff = 18.0f;
    int terroristFootDiff = 13.0f;
    int textureWidth = 30;
    int textureHeight = 60;
   
    
    
    Soldier(int x, int y, Image postavicka , std::vector<Rectangle> terrainRects, int fighterFootDif) : Fighter(x, y, postavicka, terrainRects, fighterFootDif) {
       
    };

    void stayOnGround() override{
        Rectangle postavickaRect = {(float)posX, (float)posY, (float)soldierFootDiff, (float)postavicka.height};
        onGround = false;
        for(Rectangle rect : terrainRects){            
            
            if(CheckCollisionRecs(postavickaRect, rect)){
                onGround = true;
            }  
        }
        if(!onGround || isJumping){
            posY += gravity;
        }
    }

    bool isOnGround(){
        return onGround;
    }

    void continueMoving(int x, std::string direction) override{
        int rectWidth = terrain.returnOneRectWidth();
        Rectangle postavickaRect = {(float)posX, (float)posY, (float)soldierFootDiff, (float)postavicka.height};
        
        // DrawRectangle(postavickaRect.x, postavickaRect.y, postavickaRect.width, postavickaRect.height, sui);
        bool canMove = true;
        for(Rectangle rect : terrainRects){
            if(direction == "left"){
                if(rect.x + rectWidth * 1.5 > postavickaRect.x && rect.x + rectWidth * 0.5 < postavickaRect.x && std::abs(rect.y - postavickaRect.y) < postavickaRect.height - 4){
                    // DrawRectangle(rect.x, rect.y, rectWidth, rectHeight, ORANGE);
                    if(CheckCollisionRecs(postavickaRect, rect)){
                        canMove = false;
                        break;
                    }
                }
            } else if(direction == "right"){
                if(rect.x + rectWidth * 0.5 > postavickaRect.x + soldierFootDiff && rect.x - rectWidth * 0.5 < postavickaRect.x + soldierFootDiff && std::abs(rect.y - postavickaRect.y) < postavickaRect.height - 4){
                    // DrawRectangle(rect.x, rect.y, rectWidth, rectHeight, RED);
                    if(CheckCollisionRecs(postavickaRect, rect)){
                        canMove = false;
                        break;
                    }
                }
            } 
        }
        if(canMove){
            posX += x;
        }
    }

    void aim() override{
        
    }

    void calculateAimPoint() override{
        
        Vector2 pos = returnPos();
        float cursorAngle = reuturnCursorAngle();
        int gunInHandak = returnGunInHand();

        float aimStartX = pos.x + textureWidth;
        float aimStartY = pos.y + 10;
        float distanceFromGun = 0;
        if(gunInHandak == 0){
            distanceFromGun = macRange;
        } else if(gunInHandak == 1){
            distanceFromGun = m4Range;
        } else if(gunInHandak == 2){
            distanceFromGun = granadeRange;
        }
        

        float cursorX = aimStartX + distanceFromGun * cos(DEG2RAD  * cursorAngle);
        float cursorY = aimStartY + distanceFromGun * sin(DEG2RAD  * cursorAngle);
        

        // DrawCircle(cursorX, cursorY, 5, RED);
        updateAimPoint(cursorX, cursorY);
        updateStartBulletPoint(aimStartX, aimStartY);
    }

    void drawHealth() override {
        DrawRectangle((float)20, (float)20, (float)3 * returnHealth(), (float)20, RED);
    }

    void shoot() override {
        int gunInHandak = returnGunInHand();
        deleteBullets();
        Vector2 startBullet = returnStartBulletPoint();
        float angle = reuturnCursorAngle();
        Vector2 bulletVelocity = {0,0};
        // float ang = 0.0f;
        if(gunInHandak == 1 || gunInHandak == 0){

            Vector2 poiting = returnAimPoint();
            int bulletSpeed = returnBulletSpeed();
            bulletVelocity = Vector2Subtract(poiting, startBullet);
            bulletVelocity = Vector2Normalize(bulletVelocity);
            bulletVelocity = Vector2Scale(bulletVelocity,bulletSpeed);
            bullets.push_back(Bullet(startBullet, bulletVelocity, gunInHandak, angle, RED));
        }
        else if(gunInHandak == 2 && granadesLeft > 0){
            int bulletSpeed = returnBulletSpeed();
            bulletVelocity = Vector2Normalize(bulletVelocity);
            bulletVelocity = Vector2Scale(bulletVelocity,bulletSpeed);
            granades.push_back(Granade(startBullet, bulletVelocity, granadeTrajectory, 180.0, RED));
            granadesLeft--;
        } 
    }

    void moveGranade(Vector2 posEnemy, int diff, Fighter& fighter) override{
        std::vector<Rectangle> rects = returnTerrain();
        // float startPointX = startBulletPoint.x;
        // float startPointY = startBulletPoint.y;
        

        for(Granade& granade : granades){
            float angleNum = granade.angle;
            float deltaX = granade.finalDeltaX;
            float deltaY = granade.finalDeltaY;
            
            
            if(!granade.falling) {
                float circleRadAngle = DEG2RAD * angleNum;
                // float angleDeg = circleRadAngle * (180/PI);
                
                Vector2 granadePosNow = granade.position;
                float trajectory = granade.granadeTrajectory;
                deltaX = granadePosNow.x + trajectory + cos(circleRadAngle) * trajectory;
                deltaY = granadePosNow.y + sin(circleRadAngle) * trajectory;
                angleNum -= -2;
                granade.angle = angleNum;
                if(granade.angle > 360){
                    granade.falling = true;
                    granade.finalDeltaX = deltaX;
                    granade.finalDeltaY = deltaY;
                }
                
            } else if(granade.falling){
                granade.finalDeltaY += 7;
            }
            
            for(Rectangle rect : terrainRects){
                std::chrono::milliseconds granadeDetonatedDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - granadeDetonated);
                if(rect.x < deltaX && rect.x + rect.width > deltaX && rect.y < deltaY && rect.y + rect.height > deltaY && granadeDetonatedDuration.count() > 1000){
                    
                    
                    int rangeFromEnemyX = std::abs(deltaX - posEnemy.x + texturePostavicka.width/2);
                    if(std::abs(rangeFromEnemyX) < 100){
                        fighter.reduceHealth(-((100-rangeFromEnemyX)/2));
                        granadeDetonated = std::chrono::system_clock::now();
                        cout << "now1" << endl;
                    } 
                    granade.position.x = 2000;
                    granade.position.y = 2000;
                    granade.finalDeltaX = 2000;
                    granade.finalDeltaY = 2000;
                    break; 
                   
                }
                
                if(deltaX > posEnemy.x + terroristFootDiff && deltaX < posEnemy.x + textureWidth
                    && deltaY > posEnemy.y && deltaY < posEnemy.y + texturePostavicka.height && granadeDetonatedDuration.count() > 200){
                    
                    granade.position.x = 2000;
                    granade.position.y = 2000;
                    granade.finalDeltaX = 2000;
                    granade.finalDeltaY = 2000;
                    fighter.reduceHealth(-70);
                    granadeDetonated = std::chrono::system_clock::now();
                    cout << "now2" <<endl;
                    break;
                }
            }   
            DrawCircleV((Vector2){deltaX,deltaY}, 4, RED);
        }  
    }

    void moveShot(Vector2 posEnemy, int diff, Fighter& fighter) override {
        std::vector<Rectangle> rects = returnTerrain();
        int minusHealth = 0;
        int bulletLength1 = 0;
        
        for(Bullet& bullet : bullets){
            
            float angleRad = DEG2RAD * bullet.angle;
            bullet.position = Vector2Add(bullet.position, Vector2Scale(bullet.velocity, GetFrameTime()));
            int gunInHandak = bullet.gunInHandak;
            
            int deltaX = bullet.position.x - bullet.startPoint.x;
            int deltaY = bullet.position.y - bullet.startPoint.y;
            float gunRange = returnGunRange(gunInHandak);
            float totalDistance = sqrt(deltaX * deltaX + deltaY * deltaY);
            if(gunInHandak == 0){
                minusHealth = -2;
                bulletLength1 = 10;
            } else if(gunInHandak == 1){
                minusHealth = -15;
                bulletLength1 = 20;
            }
            // cout << "soldier" << totalDistance << endl;
            if(totalDistance <= gunRange){

                Vector2 novy = {bullet.position.x + cos(angleRad) * bulletLength1, bullet.position.y + sin(angleRad
                ) * bulletLength1};
                DrawLineV(bullet.position, novy, RED);
                if(novy.x > posEnemy.x + diff && novy.x < posEnemy.x + textureWidth && novy.y > posEnemy.y && novy.y < posEnemy.y + textureHeight){
                    fighter.reduceHealth(minusHealth);
                    bullet.position.x = 2000;
                    bullet.position.y = 2000;
                }
                for(Rectangle rect : rects){
                    if(novy.x > rect.x && novy.x < rect.x + rect.width && novy.y > rect.y && novy.y < rect.y + rect.height){
                        bullet.position.x = 2000;
                        bullet.position.y = 2000;
                    }
                }
                
            }
        }
    }


    void calculateGranadeCircleRadius() override {
        float circleRadAngle = DEG2RAD * circleAngleNum;
        
        float startPointX = startBulletPoint.x;
        float startPointY = startBulletPoint.y;
        
        
        float deltaX = startPointX + granadeTrajectory + cos(circleRadAngle) * granadeTrajectory;
        float deltaY = startPointY + sin(circleRadAngle) * granadeTrajectory;
        // DrawCircleV((Vector2){deltaX,deltaY}, 20, YELLOW);
       
        float angleDeg = circleRadAngle * (180/PI);
        for(Rectangle rect : terrainRects){
            if(rect.x < deltaX && rect.x + oneRectWidth > deltaX && rect.y < deltaY && rect.y + rect.width + 5 > deltaY){
                granadeTrajectoryFinishAngle = angleDeg - 10;
                break;
            }    
        }
        // cout << "Good" << angleDeg << endl;

        circleAngleNum -= -4;
        if(circleAngleNum > 90){
            circleAngleNum = -45;
        }
    }

    void drawAim() override {
        // cout << gunInHand;
        Vector2 startBulletPoint = returnStartBulletPoint();
        float cursorAngle = reuturnCursorAngle();
        float angleRad = DEG2RAD * cursorAngle;
        float startPointX = startBulletPoint.x;
        float startPointY = startBulletPoint.y;
        Vector2 startPoint = {startPointX, startPointY};
        Vector2 middlePoint1;
        Vector2 middlePoint2;
        Vector2 middlePoint3;
        Vector2 middlePoint4;
        Vector2 middlePoint5;
        Vector2 middlePoint6;
        Vector2 middlePoint7;
        float space = 10.0f;
        int bulletLength = returnBulletLength();
        
    
        middlePoint1 = {
            (float)startPoint.x + cos(angleRad) * bulletLength,
            (float)startPoint.y + sin(angleRad) * bulletLength
        };

        middlePoint2 = {
            (float)middlePoint1.x + cos(angleRad) * space,
            (float)middlePoint1.y + sin(angleRad) * space
        };

        middlePoint3 = {
            (float)middlePoint2.x + cos(angleRad) * bulletLength,
            (float)middlePoint2.y + sin(angleRad) * bulletLength
        };

        middlePoint4 = {
            (float)middlePoint3.x + cos(angleRad) * space,
           (float) middlePoint3.y + sin(angleRad) * space
        };

        middlePoint5 = {
            (float)middlePoint4.x + cos(angleRad) * bulletLength,
            (float)middlePoint4.y + sin(angleRad) * bulletLength
        };

        middlePoint6 = {
            (float)middlePoint5.x + cos(angleRad) * space,
            (float)middlePoint5.y + sin(angleRad) * space
        };

        middlePoint7 = {
            (float)middlePoint6.x + cos(angleRad) * bulletLength,
            (float)middlePoint6.y + sin(angleRad) * bulletLength
        };
        int gunInHandak = returnGunInHand();
        
        if(gunInHandak == 0){
            // cout << 0;
            DrawLineV(startPoint, middlePoint1, aimImaginColor);
            DrawLineV(middlePoint2, middlePoint3, aimImaginColor);
            DrawLineV(middlePoint4, middlePoint5, aimImaginColor);
        } else if(gunInHandak == 1){
            
            DrawLineV(startPoint, middlePoint1, aimImaginColor);
            DrawLineV(middlePoint2, middlePoint3, aimImaginColor);
            DrawLineV(middlePoint4, middlePoint5, aimImaginColor);
            DrawLineV(middlePoint6, middlePoint7, aimImaginColor);
        } 
            
            bool rectFound = false;
            float finishAngle = 90.0 - granadeTrajectoryFinishAngle;
            if(finishAngle < 90){
                finishAngle = 90;
            }
      
            for(Rectangle rect : terrainRects){
            
                if(rect.y < startPointY + granadeStraightLine && rect.y + rect.height> startPointY + granadeStraightLine && rect.x + rect.width > startPointX + granadeTrajectory * 2 && rect.x < startPointX + granadeTrajectory * 2){
                    rectFound = true;
                    foundRectY = rect.y;
                    break;

                } 
            }

            

            int diffY = std::abs(foundRectY - startPointY);

            if(rectFound){

                granadeStraightLine = diffY;
            } else if(!rectFound){
                granadeStraightLine += 2;
            }
            
            float granadeTraj = returnGranadeTrajectory();
            if(gunInHand == 2){
                DrawRing((Vector2) {startPointX + granadeTraj, startPointY}, granadeTrajectory-1, granadeTrajectory, 270, finishAngle, 200, aimImaginColor);
            
                if(finishAngle == 90){
                    DrawLine(startPointX + granadeTrajectory * 2, startPointY, startPointX + granadeTrajectory * 2, startPointY + diffY, aimImaginColor);
                }  
            }
            
            
        
    }
};

class Terrorist : public Fighter{
public:
    int textureWidth = 30;
    int textureHeight = 60;
    bool onGround;
    int terroristFootDiff = 13.0f;
    float soldierFootDiff = 18.0f;
    Rectangle terroristRect = {(float)posX + terroristFootDiff, (float)posY, (float)postavicka.width - terroristFootDiff, (float)postavicka.height};
    Terrorist(int x, int y, Image postavicka, std::vector<Rectangle> terrainRects, int fighterFootDif) : Fighter(x, y, postavicka, terrainRects, fighterFootDif) {
        setRectangle(terroristRect);
    };  

    void stayOnGround() override{
        
        Rectangle postavickaRect = {(float)posX + terroristFootDiff, (float)posY, (float)postavicka.width - terroristFootDiff, (float)postavicka.height};
        onGround = false;
        for(Rectangle rect : terrainRects){
            if(CheckCollisionRecs(postavickaRect, rect)){
                onGround = true;
                
            }
        }
        if(!onGround || isJumping){
            posY += gravity;
            
        }
    }

    bool isOnGround(){
        return onGround;
    }

    void continueMoving(int x, std::string direction) override{
        int rectWidth = terrain.returnOneRectWidth();
        Rectangle postavickaRect = {(float)posX + terroristFootDiff, (float)posY, (float)postavicka.width - terroristFootDiff, (float)postavicka.height};
        
        // DrawRectangle(postavickaRect.x, postavickaRect.y, postavickaRect.width, postavickaRect.height, sui);
        bool canMove = true;

        for(Rectangle rect : terrainRects){
            if(direction == "left"){
                if(rect.x + rectWidth * 1.2 > postavickaRect.x && rect.x + rectWidth * 0.2 < postavickaRect.x && std::abs(rect.y - postavickaRect.y) < postavickaRect.height - 4){
                    // DrawRectangle(rect.x, rect.y, rectWidth, rectHeight, ORANGE);
                    if(CheckCollisionRecs(postavickaRect, rect)){
                        canMove = false;
                        break;
                    }
                }
            } else if(direction == "right"){
                if(rect.x + rectWidth * 0.2 > posX +  postavicka.width - terroristFootDiff && rect.x - rectWidth * 0.2 < posX +  postavicka.width && ::abs(rect.y - postavickaRect.y) < postavickaRect.height - 4){
                    // DrawRectangle(rect.x, rect.y, rectWidth, rectHeight, RED);
                    if(CheckCollisionRecs(postavickaRect, rect)){
                        canMove = false;
                        break;
                    }
                }
            } 
        }
        if(canMove){
            posX += x;
        }
    }

    void calculateAimPoint() override{
        
        Vector2 pos = returnPos();
        float cursorAngle = reuturnCursorAngle();
        int gunInHandak = returnGunInHand();
        
        float aimStartX = pos.x; 
        float aimStartY = pos.y + 18;

        float distanceFromGun = 0;
        if(gunInHandak == 0){
            distanceFromGun = -macRange;
        } else if(gunInHandak == 1){
            distanceFromGun = -m4Range;
        } else if(gunInHandak == 2){
            distanceFromGun = -granadeRange;
        }

        float cursorX = aimStartX + distanceFromGun * cos(DEG2RAD  * cursorAngle);
        float cursorY = aimStartY + distanceFromGun * sin(DEG2RAD  * cursorAngle);
        
        updateAimPoint((float)cursorX, (float)cursorY);
        updateStartBulletPoint((float)aimStartX, (float)aimStartY);
    }

    void drawHealth() override {
        DrawRectangle((float)screenWidth - 3 * returnHealth() - 20, (float)20, (float)3 * returnHealth(), (float)20, RED);
    }

    void shoot() override {
        
        deleteBullets();
        int gunInHandak = returnGunInHand();
        Vector2 startBullet = returnStartBulletPoint();
        float angle = reuturnCursorAngle();
        // float ang = 0.0f;
        Vector2 bulletVelocity = {0,0};

        if(gunInHandak == 1 || gunInHandak == 0){

            Vector2 poiting = returnAimPoint();
            int bulletSpeed = returnBulletSpeed();
            bulletVelocity = Vector2Subtract(poiting, startBullet);
            bulletVelocity = Vector2Normalize(bulletVelocity);
            bulletVelocity = Vector2Scale(bulletVelocity,bulletSpeed);
            bullets.push_back(Bullet(startBullet, bulletVelocity, gunInHandak, angle, RED));
        }
        else if(gunInHandak == 2 && granadesLeft > 0){
            int bulletSpeed = returnBulletSpeed();
            bulletVelocity = Vector2Normalize(bulletVelocity);
            bulletVelocity = Vector2Scale(bulletVelocity,bulletSpeed);
            granades.push_back(Granade(startBullet, bulletVelocity, granadeTrajectory, 180.0, RED));
            granadesLeft--;
        }    
    }

    void moveGranade(Vector2 posEnemy, int Diff, Fighter& fighter) override{
        std::vector<Rectangle> rects = returnTerrain();
        
        for(Granade& granade : granades){
            float angleNum = granade.angle;
            float deltaX = granade.finalDeltaX;
            float deltaY = granade.finalDeltaY;
            
            
            if(!granade.falling) {
                float circleRadAngle = DEG2RAD * angleNum;
                // float angleDeg = circleRadAngle * (180/PI);
                
                Vector2 granadePosNow = granade.position;
                float trajectory = granade.granadeTrajectory;
                deltaX = granadePosNow.x - trajectory - cos(circleRadAngle) * trajectory;
                deltaY = granadePosNow.y + sin(circleRadAngle) * trajectory;
                angleNum -= -2;
                granade.angle = angleNum;
                if(granade.angle > 360){
                    granade.falling = true;
                    granade.finalDeltaX = deltaX;
                    granade.finalDeltaY = deltaY;
                }
                
            } else if(granade.falling){
                granade.finalDeltaY += 7;
            }
            for(Rectangle rect : terrainRects){
                    std::chrono::milliseconds granadeDetonatedDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - granadeDetonated);
                if(rect.x < deltaX && rect.x + rect.width > deltaX && rect.y < deltaY && rect.y + rect.height > deltaY && granadeDetonatedDuration.count() > 1000){
                    
                    
                    int rangeFromEnemyX = std::abs(deltaX - posEnemy.x + texturePostavicka.width/2);
                    if(std::abs(rangeFromEnemyX) < 100){
                        fighter.reduceHealth(-((100-rangeFromEnemyX)/2));
                        granadeDetonated = std::chrono::system_clock::now();
                        cout << "now1" << endl;
                    } 
                    granade.position.x = 2000;
                    granade.position.y = 2000;
                    granade.finalDeltaX = 2000;
                    granade.finalDeltaY = 2000;
                    break; 
                   
                }
                
                if(deltaX > posEnemy.x && deltaX < posEnemy.x + soldierFootDiff
                    && deltaY > posEnemy.y && deltaY < posEnemy.y + texturePostavicka.height && granadeDetonatedDuration.count() > 200){
                    
                    granade.position.x = 2000;
                    granade.position.y = 2000;
                    granade.finalDeltaX = 2000;
                    granade.finalDeltaY = 2000;
                    fighter.reduceHealth(-70);
                    granadeDetonated = std::chrono::system_clock::now();
                    cout << "now2" <<endl;
                    break;
                }
                }   
            DrawCircleV((Vector2){deltaX,deltaY}, 4, RED);
        }  
    }

    void moveShot(Vector2 posEnemy, int diff, Fighter& fighter) override {
        std::vector<Rectangle> rects = returnTerrain();
        Rectangle postavickaRect = {(float)posX + diff, (float)posY, (float)postavicka.width - diff, (float)postavicka.height};
    
        int minusHealth = 0;
        int bulletLength1 = 0;
        for(Bullet& bullet : bullets){
            
            float angleRad = DEG2RAD * bullet.angle;
            bullet.position = Vector2Add(bullet.position, Vector2Scale(bullet.velocity, GetFrameTime()));
            int gunInHandak = bullet.gunInHandak;
            
            int deltaX = bullet.position.x - bullet.startPoint.x;
            int deltaY = bullet.position.y - bullet.startPoint.y;
            float gunRange = returnGunRange(gunInHandak);
            float totalDistance = sqrt(deltaX * deltaX + deltaY * deltaY);
            if(gunInHandak == 0){
                minusHealth = -2;
                bulletLength1 = 10;
            } else if(gunInHandak == 1){
                minusHealth = -15;
                bulletLength1 = 20;
            }
            if(totalDistance <= gunRange){
                
                Vector2 novy = {(float)bullet.position.x + cos(angleRad) * bulletLength1, (float)bullet.position.y + sin(angleRad
                ) * bulletLength1};
                DrawLineV(bullet.position, novy, RED);
                if(novy.x > posEnemy.x + diff && novy.x < posEnemy.x + postavickaRect.width && novy.y > posEnemy.y && novy.y < posEnemy.y + postavickaRect.height){
                    fighter.reduceHealth(minusHealth);
                    bullet.position.x = 2000;
                    bullet.position.y = 2000;
                }
                for(Rectangle rect : rects){
                    if(novy.x > rect.x && novy.x < rect.x + rect.width && novy.y > rect.y && novy.y < rect.y + rect.height){
                        bullet.position.x = 2000;
                        bullet.position.y = 2000;
                    }
                }
            }
            
        }
    }

    void calculateGranadeCircleRadius() override {
        float circleRadAngle = DEG2RAD * circleAngleNum2;
        
        float startPointX = startBulletPoint.x;
        float startPointY = startBulletPoint.y;
        // cout << circleAngleNum;
        
        float deltaX = startPointX - granadeTrajectory - cos(circleRadAngle) * granadeTrajectory;
        float deltaY = startPointY + sin(circleRadAngle) * granadeTrajectory;
        float angleDeg = circleRadAngle * (180/PI);
        for(Rectangle rect : terrainRects){
            if(rect.x < deltaX && rect.x + oneRectWidth > deltaX && rect.y < deltaY && rect.y + rect.width + 5 > deltaY){
                granadeTrajectoryFinishAngle = angleDeg - 10;
                break;
            }    
        }
        circleAngleNum2 -= -4;
        if(circleAngleNum2 > 90){
            circleAngleNum2 = -45;
        }
    }

    void drawAim() override {
        int gunInHandak = returnGunInHand();
        Vector2 startBulletPoint = returnStartBulletPoint();
        float cursorAngle = reuturnCursorAngle();
        float angleRad = DEG2RAD * cursorAngle;
        float startPointX = startBulletPoint.x;
        float startPointY = startBulletPoint.y;
        Vector2 startPoint = {startPointX, startPointY};
        Vector2 middlePoint1;
        Vector2 middlePoint2;
        Vector2 middlePoint3;
        Vector2 middlePoint4;
        Vector2 middlePoint5;
        Vector2 middlePoint6;
        Vector2 middlePoint7;
        float space = 10.0f;
        int bulletLength1 = returnBulletLength();
    
        middlePoint1 = {
            startPoint.x - cos(angleRad) * bulletLength1,
            startPoint.y - sin(angleRad) * bulletLength1
        };

        middlePoint2 = {
            middlePoint1.x - cos(angleRad) * space,
            middlePoint1.y - sin(angleRad) * space
        };

        middlePoint3 = {
            middlePoint2.x - cos(angleRad) * bulletLength1,
            middlePoint2.y - sin(angleRad) * bulletLength1
        };

        middlePoint4 = {
            middlePoint3.x - cos(angleRad) * space,
            middlePoint3.y - sin(angleRad) * space
        };

        middlePoint5 = {
            middlePoint4.x - cos(angleRad) * bulletLength1,
            middlePoint4.y - sin(angleRad) * bulletLength1
        };

        middlePoint6 = {
            middlePoint5.x - cos(angleRad) * space,
            middlePoint5.y - sin(angleRad) * space
        };

        middlePoint7 = {
            middlePoint6.x - cos(angleRad) * bulletLength1,
            middlePoint6.y - sin(angleRad) * bulletLength1
        };

        if(gunInHandak == 0){
            DrawLineV(startPoint, middlePoint1, aimImaginColor);
            DrawLineV(middlePoint2, middlePoint3, aimImaginColor);
            DrawLineV(middlePoint4, middlePoint5, aimImaginColor);
        }
        if(gunInHandak == 1){
            DrawLineV(startPoint, middlePoint1, aimImaginColor);
            DrawLineV(middlePoint2, middlePoint3, aimImaginColor);
            DrawLineV(middlePoint4, middlePoint5, aimImaginColor);
            DrawLineV(middlePoint6, middlePoint7, aimImaginColor);
        } 
            
            bool rectFound = false;
            float finishAngle1 = 270.0 + granadeTrajectoryFinishAngle;
            if(finishAngle1 > 270){
                finishAngle1 = 270;
                
            }
            
            for(Rectangle rect : terrainRects){
            
                if(rect.y < startPointY + granadeStraightLine && rect.y + rect.height > startPointY + granadeStraightLine && rect.x < startPointX - granadeTrajectory * 2 && rect.x + rect.width > startPointX - granadeTrajectory * 2){
                    rectFound = true;
                    foundRectY = rect.y;
                    break;

                } 
            }

            int diffY = std::abs(foundRectY - startPointY);
            // cout << foundRectY <<endl;
            if(rectFound){

                granadeStraightLine = diffY;
            } else if(!rectFound){
                granadeStraightLine += 2;
            }
            
            float granadeTraj = returnGranadeTrajectory();
            if(gunInHandak == 2){
                DrawRing((Vector2) {startPointX - granadeTraj, startPointY}, granadeTrajectory-1, granadeTrajectory, 90, finishAngle1, 200, aimImaginColor);
            
                if(finishAngle1 == 270){
                    DrawLine(startPointX - granadeTrajectory * 2, startPointY, startPointX - granadeTrajectory * 2, startPointY + diffY, aimImaginColor);
                }  
            }
            
            
        
        
    }
};

class Game {
public:
    bool gameRunning = false;
    std::chrono::system_clock::time_point shotAgainPlayer1_1 = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point shotAgainPlayer2_1 = std::chrono::system_clock::now();

    Image soldierImg = LoadImage("img/soldier.png");
    Image terroristImg = LoadImage("img/terrorist.png");
    Image macImg = LoadImage("img/mac.png");
    Image m4Img = LoadImage("img/m4.png");
    Image granadeImg = LoadImage("img/granade.png");

    Texture2D macTexture = LoadTextureFromImage(macImg);
    Texture2D m4Texture = LoadTextureFromImage(m4Img);
    Texture2D granadeTexture = LoadTextureFromImage(granadeImg);
    
    Terrain terrain = Terrain();
    std::vector<Rectangle> terrainRects = terrain.returnTerrain();
    // Soldier soldier = Soldier((int)terrainRects[2].x, (int)terrainRects[2].y - 40, soldierImg, terrainRects, 12);
    // Terrorist terrorist = Terrorist((int)terrainRects[terrainRects.size()-2].x - 9, (int)terrainRects[terrainRects.size()-2].y - 40, terroristImg, terrainRects, 12);
    Soldier soldier = Soldier((int)terrainRects[2].x, (int)terrainRects[2].y - 40, soldierImg, terrainRects, 12);
    Terrorist terrorist = Terrorist((int)terrainRects[terrainRects.size()-2].x - 9, (int)terrainRects[terrainRects.size()-2].y - 40, terroristImg, terrainRects, 12);
    

    void returnTerrain(){
        terrainRects = terrain.returnTerrain();
    }

    void drawGunMenu(){

        DrawTexture(macTexture, 350, 20, WHITE);
        DrawTexture(m4Texture, 440, 20, WHITE);
        DrawTexture(granadeTexture, 530, 20, WHITE);

        DrawTexture(macTexture, screenWidth-430, 20, WHITE);
        DrawTexture(m4Texture, screenWidth-520, 20, WHITE);
        DrawTexture(granadeTexture, screenWidth-610, 20, WHITE);


        DrawRectangleLinesEx((Rectangle){(float)350, (float)20, (float)80, (float)80}, 5, BLACK);
        DrawRectangleLinesEx((Rectangle){(float)440, (float)20, (float)80, (float)80}, 5, BLACK);
        DrawRectangleLinesEx((Rectangle){530, 20, 80, 80}, 5, BLACK);

        DrawRectangleLinesEx((Rectangle){(float)screenWidth-430, (float)20, (float)80, (float)80}, 5, BLACK);
        DrawRectangleLinesEx((Rectangle){(float)screenWidth-520, (float)20, (float)80, (float)80}, 5, BLACK);
        DrawRectangleLinesEx((Rectangle){(float)screenWidth-610, (float)20, (float)80, (float)80}, 5, BLACK);

        switch(soldier.returnGunInHand()){
            case 0:
                DrawRectangleLinesEx((Rectangle){(float)350, (float)20, (float)80, (float)80}, 5, GREEN);
                break;
            case 1:
                DrawRectangleLinesEx((Rectangle){(float)440, (float)20, (float)80, (float)80}, 5, GREEN);
                break;
            case 2:
                DrawRectangleLinesEx((Rectangle){(float)530, (float)20, (float)80, (float)80}, 5, GREEN);
        }

        switch(terrorist.returnGunInHand()){
            case 0: 
                DrawRectangleLinesEx((Rectangle){(float)screenWidth-430, (float)20, (float)80,(float) 80}, 5, GREEN);
                break;
            case 1:
                DrawRectangleLinesEx((Rectangle){(float)screenWidth-520, (float)20, (float)80, (float)80}, 5, GREEN);
                break;
            case 2:
                DrawRectangleLinesEx((Rectangle){(float)screenWidth-610, (float)20, (float)80, (float)80}, 5, GREEN);
        }
        
    }

    void handleShots(){
        //
    }

    void checkGameOver(){
        if(soldier.returnHealth() <= 0 || terrorist.returnHealth() <= 0){
            gameRunning = false;
        }
    }

    void restartGame(){
        gameRunning = true;
        terrain.restartTerrain();
        terrainRects = terrain.returnTerrain(); 
        terrorist = Terrorist((int)terrainRects[terrainRects.size()-2].x - 9, (int)terrainRects[terrainRects.size()-2].y - 40, terroristImg, terrainRects, 12);  
        soldier = Soldier((int)terrainRects[2].x, (int)terrainRects[2].y - 40, soldierImg, terrainRects, 12);
    }

    void drawTextToPlay(){
        float rectWidth = 700;
        float rectHeight = 80;
        float rectX = screenWidth / 2 - rectWidth / 2;
        float rectY = screenHeight / 2 - rectHeight / 2;
        DrawRectangle((float)rectX, (float)rectY, (float)rectWidth, (float)rectHeight, BLACK);
        DrawText("Press ENTER to start the game", 470, 385, 40, WHITE);
    }

    void update(){
        if(gameRunning){
            
            soldier.update();
            terrorist.update();
            soldier.moveShot(terrorist.returnPos(), terrorist.returnFighterFootDiff(), terrorist);
            terrorist.moveShot(soldier.returnPos(), soldier.returnFighterFootDiff(), soldier);
            soldier.moveGranade(terrorist.returnPos(), terrorist.returnFighterFootDiff(), terrorist);
            terrorist.moveGranade(soldier.returnPos(), soldier.returnFighterFootDiff(), soldier);
            checkGameOver();
        }
        
        checkUserInput();
    }

    void draw(){
        
        terrain.draw();
        if(gameRunning){
            soldier.draw();
            terrorist.draw();
            drawGunMenu();
        }
        if(!gameRunning){
            drawTextToPlay();
        }
        
    }

    void checkUserInput(){
        std::chrono::milliseconds durationTime1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - shotAgainPlayer1_1);
        std::chrono::milliseconds durationTime2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - shotAgainPlayer2_1);


        //RESTART
        if(IsKeyPressed(KEY_ENTER) && !gameRunning){
            restartGame();
            gameRunning = true;
        }


        //PLAYER 1
        if(IsKeyPressed(KEY_W) && soldier.isOnGround()){
            soldier.setJumpTimer();
            soldier.setJumpBool(true);
            soldier.jump();
        }
        if(IsKeyDown(KEY_A)){
            soldier.continueMoving(-soldier.returnMovingSpeed(), "left");
        }
        if(IsKeyDown(KEY_D)){
            soldier.continueMoving(soldier.returnMovingSpeed(), "right");
        }
        if(IsKeyDown(KEY_C)){
            soldier.updateCursorAngle(2);
            soldier.updateGranadeTrajectory(-2);
        }
        if(IsKeyDown(KEY_V)){
            soldier.updateCursorAngle(-2);
            soldier.updateGranadeTrajectory(2);
        }

        if(IsKeyPressed(KEY_E)){
            soldier.changeGunInHand();
        }
        if(IsKeyPressed(KEY_SPACE) && soldier.returnGunInHand() == 1){
            soldier.shoot();
        }
        if(IsKeyDown(KEY_SPACE) && soldier.returnGunInHand() == 0 && durationTime1.count() > 50){
            soldier.shoot();
            shotAgainPlayer1_1 = std::chrono::system_clock::now();
        }
        if(IsKeyPressed(KEY_SPACE) && soldier.returnGunInHand() == 2){
            soldier.shoot();
        }

        //PLAYER 2
        if(IsKeyPressed(KEY_UP) && terrorist.isOnGround()){
            terrorist.setJumpTimer();
            terrorist.setJumpBool(true);
            terrorist.jump();
        }
        if(IsKeyDown(KEY_LEFT)){
            terrorist.continueMoving(-soldier.returnMovingSpeed(), "left");
        }
        if(IsKeyDown(KEY_RIGHT)){
            terrorist.continueMoving(soldier.returnMovingSpeed(), "right");
        }
        if(IsKeyDown(KEY_M)){
            terrorist.updateCursorAngle(-2);
            terrorist.updateGranadeTrajectory(-2);
        }
        if(IsKeyDown(KEY_K)){
            terrorist.updateCursorAngle(2);
            terrorist.updateGranadeTrajectory(2);
        }
        if(IsKeyPressed(KEY_L)){
            terrorist.changeGunInHand();
        }
        if(IsKeyPressed(KEY_COMMA) && soldier.returnGunInHand() == 1){
            terrorist.shoot();
        }
        if(IsKeyDown(KEY_COMMA) && soldier.returnGunInHand() == 0 && durationTime2.count() > 50){
            terrorist.shoot();
            shotAgainPlayer2_1 = std::chrono::system_clock::now();
        }
        if(IsKeyPressed(KEY_COMMA) && soldier.returnGunInHand() == 2){
            terrorist.shoot();
        }
    }
};



int main(){
    InitAudioDevice();
    InitWindow(screenWidth, screenHeight, "Geometry Worms");
    Game game = Game();
    SetTargetFPS(60);
    while(!WindowShouldClose())
    { 
        BeginDrawing();
        
        ClearBackground(WHITE);
        game.draw();
        game.update();
    
        EndDrawing();
    }
    
    CloseWindow();


    return 0;
}