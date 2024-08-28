/**
UNIVERSIDAD DE LAS FUERZAS ARMADAS - ESPE
Autor: Andrés Romero
Materia: Estructura de Datos - 2024
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include "../lib/StrLib.h"
#include "TermInput.h"

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>

    static struct termios old, current;

    void initTermios(int echo)
    {
        tcgetattr(0, &old);         /* grab old terminal i/o settings */
        current = old;              /* make new settings same as old settings */
        current.c_lflag &= ~ICANON; /* disable buffered i/o */
        if (echo)
        {
            current.c_lflag |= ECHO; /* set echo mode */
        }
        else
        {
            current.c_lflag &= ~ECHO; /* set no echo mode */
        }
        tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
    }

    /* Restore old terminal i/o settings */
    void resetTermios(void)
    {
        tcsetattr(0, TCSANOW, &old);
    }

    /* Read 1 character - echo defines echo mode */
    char getch_(int echo)
    {
        char ch;
        initTermios(echo);
        ch = getchar();
        resetTermios();
        return ch;
    }

    /* Read 1 character without echo */
    char getch(void)
    {
        return getch_(0);
    }

    /* Read 1 character with echo */
    char getche(void)
    {
        return getch_(1);
    }
#endif

std::string ingresarCadena(bool o) {
    char ch = '\0';
    std::string str = "";
    do {
        ch = getch();
        // Borrar
        if (ch == 127 || ch == 8) {
            std::cout << "\b \b";
            if (str.length() > 0) {
                str = str.substr(0, str.length() - 1);
            }
            continue;
        }
        if (ch == 10) {
            if (!o || (o && str != "")) {
                break;
            }
        }
        if ((ch < 65 || ch > 90) && (ch < 97 || ch > 122)) {
            continue;
        };
        std::cout << ch;
        str += ch;
    } while (true);
    std::cout << std::endl;
    return str;
}

std::string ingresarNumero() {
    char ch = '\0';
    std::string str = "";
    do {
        ch = getch();
        // Borrar
        if (ch == 127 || ch == 8) {
            std::cout << "\b \b";
            if (str.length() > 0) {
                str = str.substr(0, str.length() - 1);
            }
            continue;
        }
        if (ch == 10) {
            break;
        }
        if (ch < 48 || ch > 57) {
            continue;
        };
        std::cout << ch;
        str += ch;
    } while (true);
    std::cout << std::endl;
    return str;
}

std::string ingresarCedula() {
    char ch = '\0';
    std::string str = "";
    do {
        ch = getch();
        // Borrar
        if (ch == 127 || ch == 8) {
            std::cout << "\b \b";
            if (str.length() > 0) {
                str = str.substr(0, str.length() - 1);
            }
            continue;
        }
        if (ch == 10) {
            break;
        }
        if (ch < 48 || ch > 57) {
            continue;
        };
        str += ch;
    } while (true);
    std::cout << std::endl;
    return str;
}

int ingresarEntero() {
    char ch = '\0';
    std::wstring str = L"";
    do {
        ch = getch();
        // Borrar
        if (ch == 127 || ch == 8) {
            if (str.length() > 0) {
                std::cout << "\b \b";
                str = str.substr(0, str.length() - 1);
            }
            continue;
        }
        if (ch == 10) {
            break;
        }
        if (ch < 48 || ch > 57) {
            continue;
        };
        std::cout << ch;
        str += ch;
    } while (true);
    std::cout << std::endl;
    try {
        return std::stoi(str);
    } catch (const std::invalid_argument&) {
        return 0;
    }
}

float ingresarFlotante() {
    char ch = '\0';
    std::string str = "";
    bool hasDot = false;
    do {
        ch = getch();
        // Borrar
        if (ch == 127 || ch == 8) {
            if (str.length() > 0) {
                std::cout << "\b \b";
                if (str[str.length() - 1] == 46) {
                    hasDot = false;
                }
                str = str.substr(0, str.length() - 1);
            }
            continue;
        }
        if (ch == 10) {
            break;
        }
        // Punto
        if (ch == 46) {
            if (!hasDot) {
                hasDot = true;
                std::cout << ch;
                str += ch;
            }
            continue;
        }
        if (ch < 48 || ch > 57) {
            continue;
        };
        std::cout << ch;
        str += ch;
    } while (true);
    std::cout << std::endl;
    if (hasDot) {
        str = str.replace(str.find("."), 1, ",");
    }
    return std::stof(str);
}
