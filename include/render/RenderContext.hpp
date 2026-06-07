#ifndef RENDERCONTEXT_HPP
#define RENDERCONTEXT_HPP

#include <vector>
#include <variant>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class RenderContext
{
public:
    using DrawableVariant = std::variant<
        sf::RectangleShape,
        sf::CircleShape,
        sf::Text
    >;

    RenderContext() = default;

    void Render(sf::RenderTarget& target, const sf::Font& renderFont)
    {
        for (auto& drawable : Drawables)
        {
            std::visit([&](auto& shape) {
                if constexpr (std::is_same_v<std::decay_t<decltype(shape)>, sf::Font>)
                {
                    shape.setFont(renderFont);
                }
                target.draw(shape);
            }, drawable);
        }
    };

    std::vector<DrawableVariant> Drawables;
};

#endif // RENDERCONTEXT_HPP
