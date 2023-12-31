#include "Gameloop.h"
#include "PlayerUI.h" //includes player too
#include "StateMachine.h"
#include "Enemy.h"

Gameloop::Gameloop(SDL_Renderer* r, const int windowWidth, const int windowHeight)
    :g_renderer(r), windowWidth(windowWidth), windowHeight(windowHeight)
{}

int Gameloop::init()
{
    Time = new deltaTime();
    //create a new player
    player = new Player(this->g_renderer, windowWidth, windowHeight, &camera, Time);
    //create tiled map - (this will be moved in the future to cater for map changing)
    g_tiledMap = std::shared_ptr<TileMap>(new TileMap(g_renderer, LoadMap(MapOne), player, windowWidth, windowHeight));
    //defualt return to continue execution
    golfBall = new GolfBall(g_renderer, windowWidth, windowHeight, &camera, player, g_tiledMap->GetFlagLocation(), Time, g_tiledMap);
    g_audioPlayer = new AudioPlayer();
    g_audioPlayer->TrackSelect("./content/sounds/game_music.wav");
    //seed random number for enemies
    srand((unsigned)time(NULL));
    int enemyCount = (rand() % 3 + 1); // between 1 and 3 enemies

    for (int i = 1; i <= enemyCount; ++i)
    {
        //add enemies to the list that is pre allocated
        enemyList.push_back(new Enemy(this->g_renderer, windowWidth, windowHeight, &camera, player, i, Time,g_tiledMap));
    }

    return 0;
}
//function to map select at a later date
std::string Gameloop::LoadMap(const int mapNum)
{
    switch(mapNum)
    {
    case MapOne:
        return "./content/maps/map1.txt";
    case MapTwo:
        return "./content/maps/map2.txt";
    }
    return"";
}

bool Gameloop::processInput()
{
    //check for an event 
    SDL_Event e;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    while (SDL_PollEvent(&e))
    {
        //check for the key presses specific to movement
        const Uint8* state = SDL_GetKeyboardState(NULL);

        //use this as the flag to check the movement of the player
        player->movingUp = state[SDL_SCANCODE_W] > 0;
        player->movingDown = state[SDL_SCANCODE_S] > 0;
        player->movingLeft = state[SDL_SCANCODE_A] > 0;
        player->movingRight = state[SDL_SCANCODE_D] > 0;
        //checking if the player has quit the game
        if (e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            g_state = 5;
            return false;
        }
        //mouse events
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                if (golfBall->GetInRangeOfPlayer() && !golfBall->GetMoition() && !player->hasThrownClub)
                {
                    player->swingClub(mousePos.x, mousePos.y);
                }
            }
            if (e.button.button == SDL_BUTTON_RIGHT)
            {
                if (g_usingClub)
                {
                    player->throwProjectile(mousePos.x, mousePos.y, 0);
                }
                else
                {
                    player->throwProjectile(mousePos.x, mousePos.y, 1);
                }
            }
        }
        if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                if (golfBall->GetInRangeOfPlayer() && !player->hasThrownClub && !golfBall->GetMoition())
                {
                    g_audioPlayer->PlaySound(AudioPlayer::swing_club);
                    golfBall->SetVelocityAndDirection(true,mousePos.x,mousePos.y);
                }
            }
        }
        //select ammo
        if (e.type == SDL_MOUSEWHEEL)
        {
            if (e.wheel.y > 0) // scroll up
            {
                if (g_usingClub)
                    g_usingClub = false;
                else
                {
                    g_usingClub = true;
                }
            }
            else if (e.wheel.y < 0) // scroll down
            {
                if (g_usingClub)
                    g_usingClub = false;
                else
                {
                    g_usingClub = true;
                }
            }
        }

    //add processing input code here
    }
    if (g_state == 2)
    {
        return false;
    }
    if (g_state == 3)
    {
        return false;
    }
    //pass that key press to the players processing
    player->processInput(e);
    //default return to continue playing
    return true;
}

void Gameloop::CameraUpdate()
{
    camera.x = player->p_positionDest.x - (windowWidth/2);
    camera.y = player->p_positionDest.y - (windowHeight/2);

    //stop camera from going too far left
    if (camera.x < 0)
        camera.x = 0;
    //stop camera from going too far up
    if (camera.y < 0)
        camera.y = 0;
    //stop camera from going too far right
    if (camera.x > ((windowWidth/10) * mapWidth) - windowWidth)
        camera.x = ((windowWidth / 10) * mapWidth) - windowWidth;
    //stop camera from going too far down
    if (camera.y > ((windowHeight / 10) * mapHeight) - windowHeight)
        camera.y = ((windowHeight / 10) * mapHeight) - windowHeight;
}

void Gameloop::update()
{
    Time->Update();
    draw();
    if (!player->update(enemyList))
    {
        g_state = 2;
    }
    if (!golfBall->Update())
    {
        g_mapComplete++;
        g_tiledMap->UpdateMap(LoadMap(MapTwo));
    }
    if (g_mapComplete == TotalMaps)
    {
        g_state = 3;
    }
    for (auto& enemy : enemyList)
    {
        enemy->update();
    }
    CameraUpdate();
    //(other class updates go here)
}

void Gameloop::draw()
{
    //clear the render before drawing the next frame
    SDL_RenderClear(g_renderer);

    //Draw routines go here
    SDL_Delay(80);
    g_tiledMap->draw(camera);
    golfBall->Draw();
    player->draw();
    for (auto& enemy : enemyList)
    {
        enemy->draw();
    }
    //render the new frames that happened since the last call
    SDL_RenderPresent(g_renderer);
}

void Gameloop::clean()
{
    //clear player status
    player->clean();
    if (player)
    {
        //clear player from memory and set to NULL
        delete player;
        player = nullptr;
    }
    if (golfBall)
    {
        delete golfBall;
        golfBall = nullptr;
    }
    if (g_audioPlayer)
    {
        //clear audio from memory and set to NULL
        delete g_audioPlayer;
        g_audioPlayer = nullptr;
    }
    if (Time)
    {
        delete Time;
        Time = nullptr;
    }
    for (auto& enemy : enemyList)
    {
        enemy->clean();
        delete enemy;
        enemy = nullptr;
    }
    enemyList.clear();
}

bool Gameloop::gameRunning()
{
    return processInput();
}

int Gameloop::gameStateUpdate()
{
    return g_state;
}
