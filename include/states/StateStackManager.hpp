#ifndef STATESTACKMANAGER_HPP
#define STATESTACKMANAGER_HPP

class IState;

class RenderSnapshot;

class StateStackManager
{
public:
    bool IsEmpty() const;

    void Attach(const std::shared_ptr<IState>& state);

    void QueueAttach(const std::shared_ptr<IState>& state);

    void Detach(const std::shared_ptr<IState>& state);

    void FlushPendingAttachments();

    void UpdateStates(float dt);

    void RenderStates(sf::RenderTarget& target);

    void BuildSnapshot(RenderSnapshot& snapshot);

    void OnWindowResize(const sf::Event::SizeEvent& size);

    void OnKeyPressed(const sf::Event::KeyEvent& key);

    void OnKeyReleased(const sf::Event::KeyEvent& key);

    void OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton);

    void OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton);

    void OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove);

private:
    std::vector<std::shared_ptr<IState>> StateStack;

    std::vector<std::shared_ptr<IState>> PendingAttachments;
};

#endif // STATESTACKMANAGER_HPP
