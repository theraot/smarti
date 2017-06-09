#ifndef SMARTI_H
#define SMARTI_H

#include "smarti.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if _WIN32
    #include <conio.h>
    #include <windows.h>
    #define __BACKSPACE '\b'

    void clearTerminal()
    {
        HANDLE hStdOut;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        DWORD cellCount;
        COORD homeCoords = {0, 0};

        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hStdOut == INVALID_HANDLE_VALUE)
        {
            return;
        }
        else
        {
            if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
            {
                cellCount = csbi.dwSize.X *csbi.dwSize.Y;
                if
                (
                    FillConsoleOutputCharacter
                    (
                        hStdOut,
                        (TCHAR) ' ',
                        cellCount,
                        homeCoords,
                        &count
                    )
                )
                {
                    if
                    (
                        FillConsoleOutputAttribute
                        (
                            hStdOut,
                            csbi.wAttributes,
                            cellCount,
                            homeCoords,
                            &count)
                        )
                    {
                        SetConsoleCursorPosition(hStdOut, homeCoords);
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    return;
                }
            }
            else
            {
                return;
            }
        }
    }

    void placeCursor(int x, int y)
    {
        HANDLE hStdOut;
        COORD coords;
		coords.X = x;
		coords.Y = y;
        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hStdOut == INVALID_HANDLE_VALUE)
        {
            return;
        }
        else
        {
            SetConsoleCursorPosition(hStdOut, coords);
        }
    }

    char readch()
    {
        while(1)
        {
			char result = _getch();
            if (result == 27)
            {
                continue;
            }
            else if (result == '\0' || result == -32)
            {
                result = _getch();
                continue;
            }
            else
            {
                return result;
            }
        }
    }

    int readArrow()
    {
        while(1)
        {
			char result = _getch();
            if (result == 27)
            {
                continue;
            }
            else if (result == '\0')
            {
                result = _getch();
                continue;
            }
            else if (result == -32)
            {
                result = _getch();
                if (result == 72)
                {
                    return ARROW_UP;
                }
                else if (result == 75)
                {
                    return ARROW_LEFT;
                }
                else if (result == 77)
                {
                    return ARROW_RIGHT;
                }
                if (result == 80)
                {
                    return ARROW_DOWN;
                }
                continue;
            }
            else
            {
                continue;
            }
        }
    }

#elif __linux__
    #include <termios.h>
    #include <unistd.h>
    #define __BACKSPACE 127

    void disableEcho()
    {
        struct termios t;
        tcgetattr(STDIN_FILENO, &t);
        t.c_lflag &= ~ECHO;
        t.c_lflag &= ~ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
    }

    void enableEcho()
    {
        struct termios t;
        tcgetattr(STDIN_FILENO, &t);
        t.c_lflag |= ECHO;
        t.c_lflag |= ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
    }

    void clearTerminal()
    {
        fprintf(stdout, "\033[2J\033[0;0f");
        fprintf(stdout, "%c[%d;%df",0x1B,0,0);
        fflush(stdout);
    }

    void placeCursor(x, y)
    {
        fprintf(stdout, "%c[%d;%df",0x1B,x,y);
    }

    char readch()
    {
        char result;
        disableEcho();
        while(1)
        {
            fread(&result, 1, 1, stdin);
            while(1)
            {
                if (result == 27)
                {
                    fread(&result, 1, 1, stdin);
                    if (result >= 64 && result <= 95)
                    {
                        if (result == '[')
                        {
                            while(1)
                            {
                                fread(&result, 1, 1, stdin);
                                if (result >= 64 && result <= 126)
                                {
                                    break;
                                }
                            }
                            break;
                        }
                        else if (result == 79)
                        {
                            fread(&result, 1, 1, stdin);
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    /*else if (result == '#' || result == '(' || result == ')' || result == '/' || (result >= '0' && result <= '6'))
                    {
                        fread(&result, 1, 1, stdin);
                        break;
                    }*/
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    enableEcho();
                    return result;
                }
            }
        }
    }

    int readArrow()
    {
        char result;
        disableEcho();
        while(1)
        {
            fread(&result, 1, 1, stdin);
            while(1)
            {
                if (result == 27)
                {
                    fread(&result, 1, 1, stdin);
                    if (result >= 64 && result <= 95)
                    {
                        if (result == '[')
                        {
                            while(1)
                            {
                                fread(&result, 1, 1, stdin);
                                if (result >= 64 && result <= 126)
                                {
                                    if (result == 65)
                                    {
                                        enableEcho();
                                        return ARROW_UP;
                                    }
                                    else if (result == 66)
                                    {
                                        enableEcho();
                                        return ARROW_DOWN;
                                    }
                                    else if (result == 67)
                                    {
                                        enableEcho();
                                        return ARROW_RIGHT;
                                    }
                                    else if (result == 68)
                                    {
                                        enableEcho();
                                        return ARROW_LEFT;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        else if (result == 79)
                        {
                            fread(&result, 1, 1, stdin);
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    /*else if (result == '#' || result == '(' || result == ')' || result == '/' || (result >= '0' && result <= '6'))
                    {
                        fread(&result, 1, 1, stdin);
                        break;
                    }*/
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
#else
    #error UNSUPPORTED PLATFORM
#endif /* BUILD_PLATFORM */

#define __MAX_INT_less(digit) ((2 << ((sizeof(int) * 8) - 2))- (1 + digit))
#define __MIN_INT_less(digit) (-(__MAX_INT_less(-digit))-1)
#define __MAX_INT_10th (((2 << ((sizeof(int) * 8) - 3))- 1) / 5)
#define __MIN_INT_10th (-(__MAX_INT_10th)-1)

int readStringChar(int length, int position, char * value)
{
    for(;;)
    {
        (*value) = readch();
        if ((*value) == '\n' || (*value) == '\r')
        {
            return 0;
        }
        else if ((*value) == __BACKSPACE)
        {
            if (position > 0)
            {
                return 2;
            }
        }
        else if ((*value) < 0 || (*value) > 31)
        {
            if (position < length)
            {
                return 1;
            }
        }
    }
}

int AddDigit (int value, int digit, int * overflow)
{
    (*overflow) = 0;
    if (value < 0)
    {
        if (value >= __MIN_INT_10th)
        {
            value *= 10;
            if (value >= __MIN_INT_less(digit))
            {
                value += digit;
            }
            else
            {
                (*overflow) = 1;
            }
        }
        else
        {
            (*overflow) = 1;
        }
        return value;
    }
    else if (value > 0)
    {
        if (value <= __MAX_INT_10th)
        {
            value *= 10;
            if (value <= __MAX_INT_less(digit))
            {
                value += digit;
            }
            else
            {
                (*overflow) = 1;
            }
        }
        else
        {
            (*overflow) = 1;
        }
        return value;
    }
    else
    {
        return digit;
    }
}

/*----*/

int readChar(char * value)
{
    return readString(1, value);
}

int readInt(int * result)
{
	#define LENGTH 12
    char value;
    int position = 0;
    int found_sign = 0;
    int oldresult = 0;
    int digit = 0;
    char buffer[LENGTH];
    int overflow;
    (*result) = 0;
    for(;;)
    {
        switch(readStringChar(LENGTH, position, &value))
        {
            case 0:
            {
                printf("\n");
                return position;
            }break;
            case 1:
            {
                if (value == '-')
                {
                    if (position == 0)
                    {
                        buffer[position] = value;
                        position++;
                        printf("-");
                        found_sign = 1;
                    }
                }
                else if (value >= '0' && value <= '9')
                {
                    oldresult = (*result);
                    digit = (value - '0');
                    if (found_sign)
                    {
                        digit = -digit;
                    }
                    (*result) = AddDigit(oldresult, digit, &overflow);
                    if (overflow)
                    {
                        (*result) = oldresult;
                        continue;
                    }
                    buffer[position] = value;
                    printf("%c", value);
                    position++;
                }
            }break;
            case 2:
            {
                position--;
                if (buffer[position] == '-')
                {
                    found_sign = 0;
                }
                else
                {
                    (*result) = (*result) / 10;
                }
                printf("\b \b");
            }break;
        }
    }
	#undef LENGTH
}

int readString(int length, char * buffer)
{
    char value;
    int position = 0;
    for(;;)
    {
        switch(readStringChar(length, position, &value))
        {
            case 0:
            {
                if (position < length)
                {
                    buffer[position] = '\0';
                }
                printf("\n");
                return position;
            }break;
            case 1:
            {
                buffer[position] = value;
                position++;
                if (value == '\t')
                {
                    printf("        ");
                }
                else
                {
                    printf("%c", value);
                }
            }break;
            case 2:
            {
                position--;
                if (buffer[position] == '\t')
                {
                    printf("\b\b\b\b\b\b\b");
                }
                printf("\b \b");
            }break;
        }
    }
}

int readStringSilent(int length, char * buffer)
{
    char value;
    int position = 0;
    for(;;)
    {
        switch(readStringChar(length, position, &value))
        {
            case 0:
            {
                if (position < length)
                {
                    buffer[position] = '\0';
                }
                printf("\n");
                return position;
            }break;
            case 1:
            {
                buffer[position] = value;
                position++;
            }break;
            case 2:
            {
                position--;
            }break;
        }
    }
}

int readStringMask(int length, char * buffer, char mask)
{
    char value;
    int position = 0;
    for(;;)
    {
        switch(readStringChar(length, position, &value))
        {
            case 0:
            {
                if (position < length)
                {
                    buffer[position] = '\0';
                }
                printf("\n");
                return position;
            }break;
            case 1:
            {
                buffer[position] = value;
                position++;
                printf("%c", mask);
            }break;
            case 2:
            {
                position--;
                printf("\b \b");
            }break;
        }
    }
}

#undef __MAX_INT_less
#undef __MIN_INT_less
#undef __MAX_INT_10th
#undef __MIN_INT_10th

#endif /* SMARTI_H */
