#include <iostream>
#include <game/man/game.hpp>

int main(int argc, char const *argv[])
{
    try
    {
        GameManager_t gm;
        while (gm.Alive())
        {
            gm.Update();
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

