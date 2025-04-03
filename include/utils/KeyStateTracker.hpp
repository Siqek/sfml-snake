#ifndef KEYSTATETRACKER_HPP
#define KEYSTATETRACKER_HPP

struct KeyState {
    bool current; // false = IDLE; true = PRESSED
    bool previous;
};

class KeyStateTracker
{
public:
    KeyStateTracker(const std::unordered_map<std::string, int>& keys);

    void updateKeyStates();

    bool isKeyPressed(const std::string& key) const;
    bool isKeyDown(const std::string& key) const;
    bool isKeyUp(const std::string& key) const;

private:
    const std::unordered_map<std::string, int>& keys;
    std::unordered_map<int, KeyState> keyStates;

    void initKeyStates();
};

#endif