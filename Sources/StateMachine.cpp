#include "../Headers/StateMachine.h"

#include <iostream>

StateMachine::StateMachine(State _BaseState) : currentState(_BaseState), speed(0.0f), isGrounded(false)
{
    transitions.push_back({ Idle, Walk, [this] { return speed > 0; } });
    transitions.push_back({ Walk, Run, [this] { return speed > 5; } });
    transitions.push_back({ Run, Walk, [this] { return speed <= 5; } });
    transitions.push_back({ Walk, Idle, [this] { return speed == 0; } });
    transitions.push_back({ Any, Jump, [this] { return isGrounded == false; } });
    transitions.push_back({ Jump, Idle, [this] { return isGrounded == true; } });
}

std::string StateMachine::getCurrentState()
{
    switch (currentState)
    {
    case Idle:
        return "Idle";
    case Walk:
        return "Walk";
    case Run:
        return "Run";
    case Jump:
        return "Jump";
    }
    return "Unknown";
}

void StateMachine::update()
{
    for (Transition transition : transitions)
    {
        if ((transition.from == currentState || transition.from == Any) && transition.condition())
        {
            currentState = transition.to;
            return;
        }
    }
}
