#include <X11/X.h>
#include <X11/keysym.h>

#include <unordered_map>

namespace ECS {

    struct Keyboard_t
    {
        using OptIter = std::optional<std::unordered_map<KeySym, bool>::iterator> ;

        explicit Keyboard_t() = default;

        Keyboard_t(const Keyboard_t&) = delete;
        Keyboard_t(const Keyboard_t&&) = delete;
        Keyboard_t& operator = (const Keyboard_t&) = delete;
        Keyboard_t& operator = (const Keyboard_t&&) = delete;


        bool IsKeyPressed (KeySym k) noexcept;

        // I can use 1 function inside with SetValue instead to copy 2 times the code

        void KeyPressed(KeySym k) noexcept{
            if (auto it = GetIterator(k))
                (*it)->second = true;
        };
        void KeyReleased(KeySym k) noexcept{
            if (auto it = GetIterator(k))
                (*it)->second = false;
        };

        private:

        OptIter GetIterator(KeySym k) noexcept;

        std::unordered_map<KeySym, bool> m_pressedKeys {
               // { XK_BlackSpace     , false}
            //,   
                { XK_Tab     , false}
            ,   { XK_Linefeed     , false}
            ,   { XK_Clear     , false}
            ,   { XK_Return     , false}
            ,   { XK_Pause     , false}
            ,   { XK_Scroll_Lock     , false}
            ,   { XK_Sys_Req     , false}
            ,   { XK_Escape     , false}
            ,   { XK_Delete     , false}
            ,   { XK_Home     , false}
            ,   { XK_Left     , false}
            ,   { XK_Up     , false}
            ,   { XK_Right     , false}
            ,   { XK_Down     , false}
            ,   { XK_Prior     , false}
            ,   { XK_Page_Up     , false}
            ,   { XK_Next     , false}
            ,   { XK_Page_Down     , false}
            ,   { XK_End     , false}
            ,   { XK_Begin     , false}
            ,   { XK_Select     , false}
            ,   { XK_Print     , false}
            ,   { XK_Execute     , false}
            ,   { XK_Insert     , false}
            ,   { XK_Undo     , false}
            ,   { XK_Redo     , false}
            ,   { XK_Menu     , false}
            ,   { XK_Find     , false}
            ,   { XK_Cancel    , false}


            ,   { XK_o    , false}
            ,   { XK_p    , false}
            ,   { XK_q    , false}
            ,   { XK_a    , false}

            ,   { XK_KP_Left     , false}
            ,   { XK_KP_Up     , false}
            ,   { XK_KP_Right     , false}
            ,   { XK_KP_Down     , false}


            ,   { XK_Control_L     , false}
            ,   { XK_Control_R     , false}

            ,   { XK_Alt_L     , false}
            ,   { XK_Alt_R     , false}


            //,   { XK_KP_space     , false}
        };

    };
    

    
}