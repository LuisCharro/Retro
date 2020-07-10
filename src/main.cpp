#include <iostream>
#include <game/man/game.hpp>
#include <game/man/state.hpp>


struct MenuState_t : StateBase_t{
    explicit MenuState_t(StateManager_t& sm) : SM {sm} {}

    void Update() final {
        std::cout << R"(
            MENU:
            1. Play
            2. Options
            -. Exit
            [Select Option])";

            int opt;
            std::cin >> opt;
            switch(opt)
            {
                case 1 : SM.PushState<GameManager_t>(SM); break;
                //case 2 : options(); break;
                default : m_alive = false;
            }
    }

    bool Alive() final {
        return m_alive;
    }

    private:
        bool m_alive {true};
        StateManager_t& SM;
};

int main(int argc, char const *argv[])
{
    try
    {
        StateManager_t States;

        States.PushState<MenuState_t>(States);

        while (States.Alive())
        {
            States.Update();
        }
    }
    catch (const char* s)
    {
        std::cout << "Capturada: " << s << std::endl;
    }
    catch (std::exception e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    catch (const std::system_error& e) {
        std::clog << e.what() << " (" << e.code() << ")" << std::endl;
    }    
 
    return 0;
}

