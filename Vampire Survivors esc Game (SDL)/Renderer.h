#pragma once

#include "../Templates/Singleton.h"
#include "../GameObjects/BaseGameObject.h"

#include "SDL.h"

#include <vector>

struct SDL_Window;
struct SDL_Renderer;
class IRenderableObject;
class ExampleGameObject;
class TileMap;

struct RenderInfo
{
    RenderInfo()
        : position{ 0,0 }, size{ 0,0 }, texture{ nullptr }, src{ 0,0,0,0 }, flipped{ false }, sortingLayer{ 0 }, color{ 255,255,255,255 }
    {
    }

    RenderInfo(Vector2 Position, Vector2 Size, SDL_Texture* Texture, SDL_Rect Src, bool Flipped, int SortingLayer, SDL_Color Color)
        : position{ Position }, size{ Size }, texture{ Texture }, src{ Src }, flipped{ Flipped }, sortingLayer{ SortingLayer }, color{ Color }
    {}

    Vector2 position;
    Vector2 size;
    SDL_Texture* texture = nullptr;
    SDL_Rect src;
    bool flipped = false;
    int sortingLayer;
    SDL_Color color = { 255, 255, 255, 255 };
};

class GameRenderer
{
public:
    enum RenderSpace { WORLD, UI };
private:
    bool _layers{ true };
    bool _lighting{ true };
    bool _drawWorldDebug{ false };

    Vector2 _position;

    BaseGameObject* _target{ nullptr };
    std::vector<RenderInfo> _staticRenderList;
    std::vector<IRenderableObject*> _dynamicRenderList;
    float _width = 800;
    float _height = 600;
    Uint8 _scale{ 4 };
    float _unitsOnScreen[2]{ 10,7.5f };
    float _moveSpeed{ 5.0f };
    bool _fullscreen{ false };
    int _maxRenderDistance{ 70 };
    SDL_Renderer* _pRenderer = nullptr;
    SDL_Color _defaultColor = { 0, 0, 0, 255 };

    /// <summary>
    /// Converts world space transform units into SDL screen pixel coordinates.
    /// </summary>
    /// <param name="transform">The world space transform.</param>
    /// <returns>The screen pixel space coordinates.</returns>
    SDL_Rect WorldToScreenSpace(float x, float y, float w, float h);
    /// <summary>
    /// Converts UI space transform units into SDL screen pixel coordinates.
    /// </summary>
    /// <param name="x">The X position</param>
    /// <param name="y">The Y position</param>
    /// <param name="w">The object's width</param>
    /// <param name="h">The object's height</param>
    /// <returns>The screen pixel space coordinates.</returns>
    SDL_Rect UIToScreenSpace(float x, float y, float w, float h);

    /// <summary>
    /// Tracks the target if it's set - NOT FINAL
    /// </summary>s
    void Track();
    /// <summary>
    /// Sets the default colour the renderer clears with.
    /// </summary>
    /// <param name="color"></param>
    void SetDrawColor(SDL_Color color);
    /// <summary>
    /// Sets the default colour the renderer clears with.
    /// </summary>
    /// <param name="r">Red</param>
    /// <param name="g">Green</param>
    /// <param name="b">Blue</param>
    /// <param name="a">Alpha</param>
    void SetDrawColor(int r, int g, int b, int a);
    /// <summary>
    /// Clears the renderer.
    /// </summary>
    void Clear();
    /// <summary>
    /// Presents the renderer.
    /// </summary>
    void Present();
    void DrawWorldDebug();
    void ToggleDebugGraphics() { _drawWorldDebug = !_drawWorldDebug; }
public:
    GameRenderer(SDL_Window* pWindow);
    ~GameRenderer();
    void ToggleDebugDraw(bool state) { _drawWorldDebug = state; }
    void SetMoveSpeed(float speed) { _moveSpeed = speed; }
    float GetMoveSpeed() { return _moveSpeed; }
    /// <summary>
    /// Sets the scale of the camera (Higher values zoom out, Lower values zoom in). 
    /// </summary>
    /// <param name="scale">The new renderer scale.</param>
    void SetScale(Uint8 scale) { _scale = scale; }
    /// <summary>
    /// Sets the default colour the renderer clears with.
    /// </summary>
    /// <param name="color"></param>
    void SetDefaultColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) { _defaultColor = { r,g,b,a }; }
    /// <summary>
    /// Draws every object passed in to the renderer.
    /// </summary>
    /// <param name="gameobjects">A list of all objects wanting to be rendered in the renderer.</param>
    void Draw();
    /// <summary>
    /// Gets the Renderer's SDL_Renderer.
    /// </summary>
    /// <returns></returns>
    SDL_Renderer* GetRenderer() const { return _pRenderer; }
    /// <summary>
    /// Set an example object for this renderer to track, set nullptr to not track anything. - NOT FINAL
    /// </summary>
    /// <param name="go">The target object.</param>
    void SetObjectToTrack(BaseGameObject* go) { _target = go; }
    /// <summary>
    /// Adds a game object to the internal render list.
    /// </summary>
    /// <param name="go">The target object.</param>
    bool AddToRenderList(IRenderableObject* go);
    bool AddToRenderList(RenderInfo info);
    /// <summary>
    /// Tries to find a game object in the internal render list.
    /// </summary>
    /// <param name="go">The target object.</param>
    IRenderableObject* FindInRenderList(IRenderableObject* go);
    /// <summary>
    /// Tries to remove a game object from the internal render list.
    /// </summary>
    /// <param name="go">The target object.</param>
    bool RemoveFromRenderList(IRenderableObject* go);
    void ToggleFullscreen();
};