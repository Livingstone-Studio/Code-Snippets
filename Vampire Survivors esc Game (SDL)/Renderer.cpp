#include "Renderer.h"

#include "SDL.h"
#include "../GameObjects/IRenderableObject.h"
#include "GameWindow.h" // Game::GetWindow

#include "../GameObjects/ExampleGameObject.h"
#include "TileMap.h"

#include "../Core/CollisionManager.h"
#include "../Components/Collider2D.h"
#include "../Core/InputManager.h"
#include "../Core/DeltaTime.h"

#include <iostream> // std::cout, std::endl
#include <SDL.h> // SDL_CreateRenderer, SDL_DestroyRenderer, SDL_RENDERER_ACCELERATED, SDL_RENDERER_PRESENTVSYNC
#include <cmath>
#include <algorithm>

GameRenderer::GameRenderer(SDL_Window* pWindow)
{
    _pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (_pRenderer == nullptr)
    {
        std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
    }
    std::cout << "Game renderer created" << std::endl;

    _position.x = 0;
    _position.y = 0;

    InputManager::instance().BindKey(SDL_SCANCODE_F10, InputManager::KEYDOWN, std::bind(&GameRenderer::ToggleDebugGraphics, this));
    InputManager::instance().BindKey(SDL_SCANCODE_F11, InputManager::KEYDOWN, std::bind(&GameRenderer::ToggleFullscreen, this));
}

GameRenderer::~GameRenderer()
{
    SDL_DestroyRenderer(_pRenderer);
    std::cout << "Game renderer destroyed" << std::endl;
}

SDL_Rect GameRenderer::WorldToScreenSpace(float x, float y, float w, float h)
{
    int wi = DEFAULT_SCREEN_WIDTH, he = DEFAULT_SCREEN_HEIGHT;
    SDL_GetWindowSize(GameWindow::instance().GetWindow(), &wi, &he);

    float resolutionScale[2] = {(float)wi /DEFAULT_SCREEN_WIDTH, (float)he/DEFAULT_SCREEN_HEIGHT };// 1 1

    float scale[2] = { _unitsOnScreen[0] * _scale, _unitsOnScreen[1] * _scale }; // 10 5
    float offset[2] = { x - _position.x, y - _position.y }; // 0 0
    float range[2] = { scale[0] - (-scale[0]), scale[1] - (-scale[1]) }; // 20 10

    offset[0] = offset[0] - (-scale[0] * resolutionScale[0]); // 0 - (-10 * 1) = 10
    offset[1] = offset[1] - (-scale[1] * resolutionScale[1]); // 0 - (-5 * 1) = 5

    offset[0] = offset[0] / range[0]; // 10 / 20 = 0.5
    offset[1] = offset[1] / range[1]; // 5 / 10 = 0.5

    offset[0] = offset[0] / resolutionScale[0]; // 0.5 / 1
    offset[1] = offset[1] / resolutionScale[1]; // 0.5 / 1

    float normalDimensions[2] = { w / range[0], h / range[1] };
    normalDimensions[0] = normalDimensions[0] / resolutionScale[0];
    normalDimensions[1] = normalDimensions[1] / resolutionScale[1];

    SDL_Rect rect =
    { 
        (int)((float)wi* offset[0]), // 800 * 0.5 = 400
        (int)((float)he - (he * offset[1])), // 600 * 0.5 = 300
        (int)((float)wi * normalDimensions[0]), // 5 / 1 = 5
        (int)((float)he * normalDimensions[1])  // 5 / 1 = 5
    };
    return rect;
}

SDL_Rect GameRenderer::UIToScreenSpace(float x, float y, float w, float h)
{
    int wi = DEFAULT_SCREEN_WIDTH, he = DEFAULT_SCREEN_HEIGHT;
    SDL_GetWindowSize(GameWindow::instance().GetWindow(), &wi, &he);

    float resolutionScale[2] = { (float)wi / DEFAULT_SCREEN_WIDTH, (float)he / DEFAULT_SCREEN_HEIGHT };

    float scale[2] = { (float)wi * _scale, (float)he * _scale };
    float offset[2];
    float range[2] = { scale[0] - (-scale[0]), scale[1] - (-scale[1]) };

    float pos[2] = { range[0],range[1] };
    float window_scaling[2] = {pos[0]/(_scale * 2),pos[1]/(_scale * 2)};

    if (window_scaling[0] < 0)
        window_scaling[0] *= -1.0f;
    else if (window_scaling[0] == 0)
        window_scaling[0] = 1;
    if (window_scaling[1] < 0)
        window_scaling[1] *= -1.0f;
    else if (window_scaling[1] == 0)
        window_scaling[1] = 1;

    offset[0] = x * window_scaling[0] * resolutionScale[0] * _scale;
    offset[1] = y * window_scaling[1] * resolutionScale[1] * _scale;

    offset[0] = offset[0] - (-scale[0] * resolutionScale[0]);
    offset[1] = offset[1] - (-scale[1] * resolutionScale[1]);

    offset[0] = offset[0] / range[0];
    offset[1] = offset[1] / range[1];

    offset[0] = offset[0] / resolutionScale[0];
    offset[1] = offset[1] / resolutionScale[1];

    float resScaleAvg = (resolutionScale[0] + resolutionScale[1]) / 2;
    SDL_Rect rect =
    {
        (int)((float)wi * offset[0]), (int)((float)he - ((float)he * offset[1])),
        (int)(w * resScaleAvg), (int)(h * resScaleAvg)
    };
    return rect;
}

void GameRenderer::Track()
{
    float tX = _target->GetX();
    float tY = _target->GetY();

    float dir = tX - _position.x;
    if (abs(dir) > 0.01f)
        _position.x += (dir * _moveSpeed * DeltaTime::GetDeltaTime());
    dir = tY - _position.y;
    if (abs(dir) > 0.01f)
        _position.y += (dir * _moveSpeed * DeltaTime::GetDeltaTime());
}

void GameRenderer::SetDrawColor(SDL_Color color)
{
    SDL_SetRenderDrawColor(_pRenderer, color.r, color.g, color.b, color.a);
}

void GameRenderer::SetDrawColor(int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(_pRenderer, r, g, b, a);
}

void GameRenderer::Draw()
{
    if (_target) 
        Track();

    if (_staticRenderList.size() == 0 && _dynamicRenderList.size() == 0)
        return;
    
    Clear();

    std::vector<RenderInfo> drawList;

    for (const RenderInfo& renderable : _staticRenderList)
    {
        Vector2 target_position = _position;
        if (_target)
            target_position = { _target->GetX(), _target->GetY() };
        float distance = abs(target_position.distance(renderable.position));
        if (distance > _maxRenderDistance)
            continue;
        drawList.push_back(renderable);
    }


    std::vector<IRenderableObject*> uiObjects;
    for (IRenderableObject* renderable : _dynamicRenderList)
    {
        if (!renderable)
            continue;
        if (!renderable->GetEnabled())
            continue;

        Vector2 target_position = _position;
        if (_target)
            target_position = { _target->GetX(), _target->GetY() };
        float distance = abs(target_position.distance({ renderable->GetX() , renderable->GetY() }));
        if (distance > _maxRenderDistance)
            continue;

        switch (renderable->GetRenderSpace())
        {
        case WORLD:
        {
            RenderInfo info = renderable->GetRenderInfo();
            info.src = renderable->GetSrc();
            drawList.push_back(info);
            break;
        }
        case UI:
            uiObjects.push_back(renderable);
            break;
        }
    }

    if (_layers)
    {
        std::stable_sort(drawList.begin(), drawList.end(),
            [](const RenderInfo& g1, const RenderInfo& g2) {
                return g1.sortingLayer - g1.position.y < g2.sortingLayer - g2.position.y; });
    }


    RenderInfo renderable;
    SDL_Rect rect;
    // Render World Objects
    for (int i = 0; i < drawList.size(); ++i)
    {
        renderable = drawList[i];     
                
        SDL_Point rot_center;
        rot_center.x = renderable.size.x / 2;
        rot_center.y = renderable.size.y / 2;

        rect = WorldToScreenSpace(renderable.position.x, renderable.position.y, renderable.size.x, renderable.size.y);
        rect.x = rect.x - rect.w / 2; rect.y = rect.y - rect.h / 2;

        Vector2 target_position = _position;
        if (_target)
            target_position = { _target->GetX(), _target->GetY() };
        float distance = abs(target_position.distance(renderable.position));

        SDL_Color color = renderable.color;

        if (_lighting && distance > 1)
        {
            float norm_distance = (_maxRenderDistance+1-distance) / _maxRenderDistance;
            color.r = renderable.color.r * norm_distance;
            color.g = renderable.color.g * norm_distance;
            color.b = renderable.color.b * norm_distance;
            color.a = renderable.color.a * norm_distance;
        }

        SDL_Texture* texture = renderable.texture;
        if (texture)
        {
            SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
            SDL_Rect src = renderable.src;
            if (renderable.flipped)
                SDL_RenderCopyEx(_pRenderer, texture, &src, &rect, 0.0f, &rot_center, SDL_FLIP_HORIZONTAL);
            else
                SDL_RenderCopyEx(_pRenderer, texture, &src, &rect, 0.0f, &rot_center, SDL_FLIP_NONE);
        }
        else
        {
            SetDrawColor(color);
            SDL_RenderFillRect(_pRenderer, &rect);
        }
    }


    // Render Debug
    if (_drawWorldDebug)
        DrawWorldDebug();



    // Render UI Objects
    for (int i = 0; i < uiObjects.size(); ++i)
    {
        rect = UIToScreenSpace(uiObjects[i]->GetX(), uiObjects[i]->GetY(), uiObjects[i]->GetWidth(), uiObjects[i]->GetHeight());
        rect.x = rect.x - rect.w / 2; rect.y = rect.y - rect.h / 2;
        SetDrawColor(uiObjects[i]->GetColor());
        SDL_Texture* texture = uiObjects[i]->GetTexture();
        if (texture)
        {
            SDL_Rect src = uiObjects[i]->GetSrc();
            SDL_RenderCopy(_pRenderer, texture, &src, &rect);
        }
        else
        {
            SDL_RenderFillRect(_pRenderer, &rect);
        }
    }

    SetDrawColor(_defaultColor);
    Present();
}

bool GameRenderer::AddToRenderList(IRenderableObject* go)
{
    if (FindInRenderList(go))
        return false;
    _dynamicRenderList.push_back(go);
    return true;
}

bool GameRenderer::AddToRenderList(RenderInfo info)
{
    _staticRenderList.push_back(info);
    return true;
}

IRenderableObject* GameRenderer::FindInRenderList(IRenderableObject* go)
{
    std::vector<IRenderableObject*>::iterator it = std::find(_dynamicRenderList.begin(), _dynamicRenderList.end(), go);
    if (it == _dynamicRenderList.end())
        return nullptr;
    return _dynamicRenderList[std::distance(_dynamicRenderList.begin(), it)];
}

bool GameRenderer::RemoveFromRenderList(IRenderableObject* go)
{
    IRenderableObject* obj = FindInRenderList(go);
    if (!obj)
        return false;
    _dynamicRenderList.erase(std::remove(_dynamicRenderList.begin(), _dynamicRenderList.end(), go), _dynamicRenderList.end());
    return true;
}

void GameRenderer::ToggleFullscreen()
{
    if (_fullscreen)
    {
        SDL_SetWindowFullscreen(GameWindow::instance().GetWindow(), 0);
        _fullscreen = false;
    }
    else
    {
        SDL_SetWindowFullscreen(GameWindow::instance().GetWindow(), SDL_WINDOW_FULLSCREEN_DESKTOP);
        _fullscreen = true;
    }
}

void GameRenderer::Clear()
{
    SDL_RenderClear(_pRenderer);
}

void GameRenderer::Present()
{
    SDL_RenderPresent(_pRenderer);
}

void GameRenderer::DrawWorldDebug()
{
    // Get All Colliders
    std::vector<Collider2D*> colliders = CollisionManager::GetColliders();

    // Render Colliders
    for (int i = 0; i < colliders.size(); ++i)
    {
        if (!colliders[i])
            continue;
        // check shape
        switch (colliders[i]->GetColliderType())
        {
        case ColliderType::RECTANGLE:
        {
            Vector2 pos = colliders[i]->GetPosition();
            Vector2 dim = colliders[i]->GetDimensions();
            SDL_Rect rect = WorldToScreenSpace(pos.x, pos.y, dim.x, dim.y);
            rect.x = rect.x - rect.w / 2; rect.y = rect.y - rect.h / 2;
            SetDrawColor({ 0,255,255,255 });
            SDL_RenderDrawRect(_pRenderer, &rect);
            break;
        }
        case ColliderType::CIRCLE:
        {
            Vector2 pos = colliders[i]->GetPosition();
            Vector2 dim = { colliders[i]->GetRadius(), colliders[i]->GetRadius() };
            SDL_Rect rect = WorldToScreenSpace(pos.x + dim.x/2, pos.y - dim.y/2, dim.x, dim.y);
            rect.x = rect.x - rect.w / 2; rect.y = rect.y - rect.h / 2;
            
            
            SetDrawColor({ 0,255,255,255 });
            int x = 0;
            int y = rect.w;
            int d = 3 - 2 * rect.w;
            while (y >= x) {
                SDL_RenderDrawPoint(_pRenderer, rect.x + x, rect.y + y);
                SDL_RenderDrawPoint(_pRenderer, rect.x + y, rect.y + x);
                SDL_RenderDrawPoint(_pRenderer, rect.x - x, rect.y + y);
                SDL_RenderDrawPoint(_pRenderer, rect.x - y, rect.y + x);
                SDL_RenderDrawPoint(_pRenderer, rect.x + x, rect.y - y);
                SDL_RenderDrawPoint(_pRenderer, rect.x + y, rect.y - x);
                SDL_RenderDrawPoint(_pRenderer, rect.x - x, rect.y - y);
                SDL_RenderDrawPoint(_pRenderer, rect.x - y, rect.y - x);
                if (d < 0) {
                    d += 4 * x + 6;
                }
                else {
                    d += 4 * (x - y) + 10;
                    y--;
                }
                x++;
            }
            break;
        }
        default:
            break;
        }
    } 
}
