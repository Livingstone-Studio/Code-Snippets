#pragma once

#include "SDL.h"
#include "../Rendering/Renderer.h"
#include "../Core/AssetManager.h"

#include <string>

class IRenderableObject {
protected:
    SDL_Texture* _texture{ nullptr };
    SDL_Rect _src;
    SDL_Color _color = { 255,255,255,255 };
    GameRenderer::RenderSpace _renderSpace{ GameRenderer::RenderSpace::WORLD };
    bool _enabled{ true };
    bool _flipped{ false };
    int _sortingLayer = 0;

public:
    virtual ~IRenderableObject() { std::cout << "Rendereable Object Destructor Called" << std::endl; }
    virtual float GetX() const = 0;
    virtual float GetY() const = 0;
    virtual float GetWidth() const = 0;
    virtual float GetHeight() const = 0;

    void Flip(bool flip) { if (_flipped != flip) _flipped = flip; }
    bool Flipped() { return _flipped; }

    void SetSortingLayer(int layer) { _sortingLayer = layer; }
    int GetSortingLayer() { return _sortingLayer; }
    
    virtual RenderInfo GetRenderInfo() const = 0;

    /// <summary>
    /// Set whether or not this object is currently rendering.
    /// </summary>
    /// <param name="e">The state of if the object should render.</param>
    void SetEnabled(bool e) { _enabled = e; }
    /// <summary>
    /// </summary>
    /// <returns>Whether or not this object renders.</returns>
    bool GetEnabled() { return _enabled; }
    /// <summary>
    /// </summary>
    /// <returns>The view space the object is rendered to.</returns>
    GameRenderer::RenderSpace GetRenderSpace() { return _renderSpace; }
    /// <summary>
    /// </summary>
    /// <returns>The color of the object.</returns>
    SDL_Color GetColor() { return _color; }
    /// <summary>
    /// Sets the renderable object's texture.
    /// </summary>
    /// <param name="file">The filepath for the new texture.</param>
    /// <param name="src">The x,y,w,h of the file you want to render. Will probably be changed later. </param>
    void SetTexture(std::string file, SDL_Rect src) { _texture = AssetManager::instance().LoadImage(file); _src = src; }
    /// <summary> 
    /// </summary>
    /// <returns>The current texture of the renderable object.</returns>
    SDL_Texture* GetTexture() { return _texture; }
    /// <summary>
    /// </summary>
    /// <returns>The x,y,w,h of the texture being rendered.</returns>
    virtual SDL_Rect GetSrc() { return _src; }



};