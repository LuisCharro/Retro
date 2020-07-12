#pragma once

#include <stack>
#include <memory>
#include <type_traits>

struct GameManager_t;

struct StateBase_t{
    virtual void Update() = 0;
    virtual bool Alive() = 0;
    virtual ~StateBase_t() = default;
};

struct StateManager_t{
    explicit StateManager_t() = default;

    template <typename State_t, typename... Ts>
    void PushState(Ts&&... args)
    {
        //Stop at compilation time --> If the type used is not base 
        static_assert(std::is_base_of<StateBase_t, State_t>::value,"Error invalid State_t");
        m_States.push( std::make_unique<State_t>( std::forward<decltype(args)>(args)...) );
    }

    void Update()
    {
        if (Alive())
        {
            auto& state { m_States.top() };
            if (state && state->Alive())
                state->Update();
            else
                m_States.pop();
        }
    }

    bool Alive() {return !m_States.empty();}

    private:
        std::stack<std::unique_ptr<StateBase_t>> m_States;
};

struct MenuState_t : StateBase_t{
    explicit MenuState_t(StateManager_t& sm) : SM {sm} {}

    void Update() final {
        std::cout << R"(
            MENU:
            1. Play
            2. Options
            0. Exit
            [Select Option])";

            int opt;
            std::cin >> opt;
            switch(opt)
            {
                case 1 : SM.PushState<GameManager_t>(SM); break;
                //case 2 : options(); break;
                case 0 :  m_alive = false;                
            }
    }

    bool Alive() final {
        return m_alive;
    }

    private:
        bool m_alive {true};
        StateManager_t& SM;
};

struct PauseState_t : StateBase_t{
    explicit PauseState_t() = default;

    void Update() final {
        std::cout << R"(
            GAME PAUSED            
            [ TYPE IN SOMETHING ])";

            int opt;
            std::cin >> opt;
            std::cin.ignore(10000,'\n');
            std::cin.clear();

            m_alive = false;
    }

    bool Alive() final { return m_alive; }

    private:
        bool m_alive {true};
};