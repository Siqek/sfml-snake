#ifndef RENDERSNAPSHOT_HPP
#define RENDERSNAPSHOT_HPP

#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>

#include "render/RenderContext.hpp"

class RenderSnapshot
{
public:
    RenderSnapshot(const sf::Font& renderFont)
        : RenderContexts{}, RenderFont(renderFont) {}

    void Clear()
    {
        RenderContexts.clear();
    }

    void Render(sf::RenderTarget& target)
    {
        for (auto& context : RenderContexts)
        {
            context.Render(target, RenderFont);
        }
    }

    [[nodiscard]] RenderContext& CreateContext()
    {
        return RenderContexts.emplace_back();
    }

private:
    std::vector<RenderContext> RenderContexts;

    const sf::Font& RenderFont;
};

#endif // RENDERSNAPSHOT_HPP
