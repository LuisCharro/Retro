#pragma once

#include <stack>
#include <memory>
#include <type_traits>

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