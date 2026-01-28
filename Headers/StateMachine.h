#pragma once

#include <string>
#include <functional>
#include <vector>

enum State { Idle, Walk, Run, Jump, Any };

struct Transition
{
    State from;
    State to;
    std::function<bool()> condition;
};

class StateMachine
{
private:
    State currentState;
    std::vector<Transition> transitions;

public:
    float speed;
    bool isGrounded;

    StateMachine(State _BaseState);
    std::string getCurrentState();
    void update();
};
