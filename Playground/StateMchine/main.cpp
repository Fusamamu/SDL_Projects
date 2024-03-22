#include <iostream>

class StateMachine;

class State
{
public:
    State() = default;
    virtual ~State() = default;

    void SetStateMachine(StateMachine* stateMachine)
    {
        m_StateMchine = stateMachine;
    }

    virtual void Enter() = 0;
    virtual void Exit()  = 0;

private:
    StateMachine* m_StateMchine { };
};

class StateMachine
{
public:
    StateMachine() : m_CurrentState(nullptr)
    {

    }

    void TransitionTo(State* _state)
    {
        if(m_CurrentState){
            m_CurrentState->Exit();
            delete m_CurrentState;
        }

        m_CurrentState = _state;
        if(m_CurrentState)
            m_CurrentState->Enter();
    }

private:
    State* m_CurrentState;
};

class ConcreteStateA : public State
{
public:
    void Enter() override
    {
        std::cout << "Enter concrete state A" << std::endl;
    }

    void Exit() override
    {
        std::cout << "Exit concrete state A" << std::endl;
    }
};

class ConcreteStateB : public State
{
public:
    void Enter() override
    {
        std::cout << "Enter concrete state B" << std::endl;
    }

    void Exit() override
    {
        std::cout << "Exit concrete state B" << std::endl;
    }
};

int main()
{
    StateMachine _stateMachine;

    auto* _concreteStateA = new ConcreteStateA;
    auto* _concreteStateB = new ConcreteStateB;

    _concreteStateA->SetStateMachine(&_stateMachine);
    _concreteStateB->SetStateMachine(&_stateMachine);

    _stateMachine.TransitionTo(_concreteStateA);
    _stateMachine.TransitionTo(_concreteStateB);

    return 0;
}
