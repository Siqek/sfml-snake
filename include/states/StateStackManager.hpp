#ifndef STATESTACKMANAGER_HPP
#define STATESTACKMANAGER_HPP

class IState;

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

    void OnWindowResize();

private:
    std::vector<std::shared_ptr<IState>> StateStack;

    std::vector<std::shared_ptr<IState>> PendingAttachments;
};

#endif // STATESTACKMANAGER_HPP
