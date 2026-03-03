#ifndef RENDERCONTEXT_HPP
#define RENDERCONTEXT_HPP

#include <SFML/Graphics/RenderTarget.hpp>

struct RenderContext
{
    virtual ~RenderContext() = default;

    virtual void Render(sf::RenderTarget& target) = 0;
};

#endif // RENDERCONTEXT_HPP
