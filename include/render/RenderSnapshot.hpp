#ifndef RENDERSNAPSHOT_HPP
#define RENDERSNAPSHOT_HPP

#include <vector>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>

#include "render/RenderContext.hpp"

struct RenderSnapshot
{
    RenderSnapshot()
        : RenderContexts{} {}

    void Clear()
    {
        RenderContexts.clear();
    }

    void Render(sf::RenderTarget& target)
    {
        // TODO(siqek): finish
        (void)target;
    }

    std::vector<std::unique_ptr<RenderContext>> RenderContexts;
};

#endif // RENDERSNAPSHOT_HPP
