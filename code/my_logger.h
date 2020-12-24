static const char* MyKeys[] = { 
    "\0", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", 
    "9", "0", "-", "=", "BACKSPACE", "TAB", "q", "w", "e", "r", 
    "t", "y", "u", "i", "o", "p", "SPACE", "SPACE", "ENTER", "lCTRL", 
    "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", 
    "'", "`", "lSHIFT", "\\", "z", "x", "c", "v", "b", "n", 
    "m", ",", ".", "/", "rSHIFT", "\0", "lALT", "SPACE", "CAPSLOCK", "F1", 
    "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUMLOCK", // 60 - 69 
    "SCROLLLOCK", "HOME", "UP", "PGUP", "-", "LEFT", "5", "RTARROW", "+", "END", 
    "DOWN", "PGDN", "INS", "DELETE", "\0", "\0", "\0", "F11", "F12", "\0", // 80 - 89 
    "\0", "\0", "\0", "\0", "\0", "\0", "rENTER", "rCTRL", "/", "PRTSCR", 
    "rALT", "\0", "HOME", "UP", "PGUP", "LEFT", "RIGHT", "END", "DOWN", "PGDN", // 100 - 109 
    "INSERT", "DEL", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "PAUSE", 
    "\0", "\0", "\0", "\0", "\0", "WIN", "\0", "\0", "\0", "\0"
};

static const int MyKeysMax = 129;

struct info_item
{
    int my_event_type;
    char btns;
    char dx;
    char dy;
    char wheel;
};
