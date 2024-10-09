#include <windows.h>
#include <conio.h>
#include <iostream>
#include "clui.h"
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;

int const width = 60;
int const height = 20;
char show_page[height][width];
bool game_over;
int score1, score2;

int rows = get_window_rows();
int colm = get_window_cols();
int level = 0;
struct position
{
    int x, y;
} user1, fruit, bomb, user2;
int rest_of_snake1_x[height * width], rest_of_snake1_y[height * width];
int rest_of_snake2_x[height * width], rest_of_snake2_y[height * width];
int count_of_tail1, count_of_tail2;
// int check_for_kill_itself1, check_for_kill_itselt2;
int repeat_game = 0;
int check_for_save = 0;
int seconds, minutes, hours;
int user_seconds = 0, user_minutes = 0, user_hours = 0;
int check_for_time;
string user1_name, user2_name;

enum adress
{
    Stop,
    Up,
    Down,
    Left,
    Right
};
adress ad_direction1, ad_direction2;

struct leaderboard
{
    string name;
    int score;
};
leaderboard show_league_table[1000]; // array for saving name and score from players

void setup();
void logic();
void menu();
void draw();
void input();
void choose_level();
void game_load();
void load_snake1();
void load_snake2();
void lost_game(int user);
void replay_game();
void Hi();
void get_name();
void make_table();
void hide_cursor();
void time();
void mosavi();


//first things that need to reset after game
void setup()
{
    game_over = false;
    ad_direction1 = Stop;
    ad_direction2 = Stop;
    user1.x = 5;
    user1.y = 5;
    user2.x = 25;
    user2.y = 5;
    fruit.x = rand() % (width - 2) + 1;
    fruit.y = rand() % (height - 2) + 1;
    bomb.x = rand() % (width - 2) + 1;
    bomb.y = rand() % (height - 2) + 1;
    score1 = 0;
    score2 = 0;
    count_of_tail1 = 3;
    count_of_tail2 = 3;
    
    seconds = user_seconds;
    minutes = user_minutes;
    hours = user_hours;
    check_for_time = 0;
}

//a function for show array that contain fruit bomb players and walls
void draw()
{
    clear_screen();
    if (ad_direction1 == Stop)
        load_snake1();
    if (ad_direction2 == Stop)
        load_snake2();

    for (int i = 0; i < width; i++)
    {
        show_page[0][i] = '.';
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0 || j == width - 1)
            {
                show_page[i][j] = '.';
            }
            else if (j == user1.x && i == user1.y)
            {
                show_page[i][j] = 'O';
            }
            else if (j == user2.x && i == user2.y)
            {
                show_page[i][j] = 'X';
            }
            else if (j == fruit.x && i == fruit.y)
            {
                show_page[i][j] = 'F';
            }
            else if (j == bomb.x && i == bomb.y)
            {
                show_page[i][j] = 'B';
            }
            else
            {
                if (i == 0)
                    continue;
                bool res = false;
                for (int m = 0; m < count_of_tail1; m++)
                {
                    if (j == rest_of_snake1_x[m] && i == rest_of_snake1_y[m])
                    {
                        show_page[i][j] = 'o';
                        res = true;
                    }
                }
                for (int n = 0; n < count_of_tail2; n++)
                {
                    if (j == rest_of_snake2_x[n] && i == rest_of_snake2_y[n])
                    {
                        show_page[i][j] = 'x';
                        res = true;
                    }
                }
                if (!res)
                {
                    show_page[i][j] = ' ';
                }
            }
        }
    }

    for (int i = 0; i < width; i++)
    {
        show_page[height - 1][i] = '.';
    }

    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {
            cout << show_page[i][j];
        }
        cout << endl;
    }

    
    cout << "-----------------------" << endl;
    change_color_rgb(222, 75, 75);
    cout << "|" << user1_name << ":" << score1 << endl;
    reset_color();

    change_color_rgb(184, 224, 38);
    cout << "|" << user2_name << ":" << score2 << endl;
    reset_color();

    cout << "-----------------------" << endl;

    time();
    cout << "Hours:" << hours << " Minutes:" << minutes << " Seconds:" << seconds << endl;

   
}

//this function related to keys that user press to say some direction
void input()
{
    if (is_keyboard_hit())
    {
        switch (getch())
        {
        case 'w':
            ad_direction1 = Up;
            break;
        case 'a':
            ad_direction1 = Left;
            break;
        case 's':
            ad_direction1 = Down;
            break;
        case 'd':
            ad_direction1 = Right;
            break;

        case 72:
            ad_direction2 = Up;
            break;
        case 75:
            ad_direction2 = Left;
            break;
        case 80:
            ad_direction2 = Down;
            break;
        case 77:
            ad_direction2 = Right;
            break;

        default:
            break;
        }
    }
}

//some logical things put in this function like condition for losing or condition for increase point
void logic()
{

    // change x,y of snakes tail

    for(int i = count_of_tail1 ; i>= 1  ; i--){
        rest_of_snake1_x[i] = rest_of_snake1_x[i-1];
        rest_of_snake1_y[i] = rest_of_snake1_y[i-1];
    }
    rest_of_snake1_x[0] = user1.x;
    rest_of_snake1_y[0] = user1.y;

    // change x,y of snakes tail
    for(int i = count_of_tail2 ; i>= 1  ; i--){
        rest_of_snake2_x[i] = rest_of_snake2_x[i-1];
        rest_of_snake2_y[i] = rest_of_snake2_y[i-1];
    }
    rest_of_snake2_x[0] = user2.x;
    rest_of_snake2_y[0] = user2.y;

    switch (ad_direction1)
    {
    case Left:
        user1.x -= 1;
        break;
    case Right:
        user1.x += 1;
        break;
    case Up:
        user1.y -= 1;
        break;
    case Down:
        user1.y += 1;
        break;
    }
    switch (ad_direction2)
    {
    case Left:
        user2.x -= 1;
        break;
    case Right:
        user2.x += 1;
        break;
    case Up:
        user2.y -= 1;
        break;
    case Down:
        user2.y += 1;
        break;
    }

    //condition for walls
    if (user1.x == width || user1.x == 0 || user1.y == height || user1.y == -1)
    {
        
        lost_game(1);
    }
    if (user2.x == width || user2.x == 0 || user2.y == height || user2.y == -1)
    {
        
        lost_game(2);
    }

    //condition for bomb
    if (user1.x == bomb.x && user1.y == bomb.y)
    {
        
        lost_game(1);
    }
    if (user2.x == bomb.x && user2.y == bomb.y)
    {
        
        lost_game(2);
    }

    //condition for eat fruits
    if (user1.x == fruit.x && user1.y == fruit.y)
    {
        score1 += 10;
        fruit.x = rand() % (width - 2) + 1;
        fruit.y = rand() % (height - 2) + 1;
        count_of_tail1++;
        
    }
    if (user2.x == fruit.x && user2.y == fruit.y)
    {
        score2 += 10;
        fruit.x = rand() % (width - 2) + 1;
        fruit.y = rand() % (height - 2) + 1;
        count_of_tail2++;
        
    }


    //condition for snake to eat itselt
    for (int i = 1; i < count_of_tail1; i++)
    {

        if (user1.x == rest_of_snake1_x[i] && user1.y == rest_of_snake1_y[i])
        {
            lost_game(1);
            delay(2000);
            menu();
        }
    }

    for (int i = 1; i < count_of_tail2; i++)
    {

        if (user2.x == rest_of_snake2_x[i] && user2.y == rest_of_snake2_y[i])
        {
            lost_game(1);
            delay(2000);
            menu();
        }
    }

    //condition for snake to eat other snake's body
    for (int i = 0; i < count_of_tail2; i++)
    {

        if (user1.x == rest_of_snake2_x[i] && user1.y == rest_of_snake2_y[i])
        {
            lost_game(1);
        }
    }
    for (int i = 0; i < count_of_tail1; i++)
    {

        if (user2.x == rest_of_snake1_x[i] && user2.y == rest_of_snake1_y[i])
        {
            lost_game(2);
        }
    }
    if (seconds == 0 && minutes == 0 && hours == 0)
    {
        if (score1 > score2)
        {
            lost_game(2);
        }
        else if (score2 > score1)
        {
            lost_game(1);
        }
        else
        {
            mosavi();
        }
    }
}

//for draw
void mosavi()
{
    clear_screen();
    make_table();
    game_over = true;
    repeat_game++;
    cursor_to_pos(rows /2 , colm/2 -2);
    cout << "Draw!!" << endl;
    cursor_to_pos(rows / 2 + 2, colm / 2 - 4);
    cout << user1_name << " score is:" << score1 << endl;
    cursor_to_pos(rows / 2 + 3, colm / 2 - 4);
    cout << user2_name << " score is:" << score2 << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 - 12);
    cout << "------------------------------------" << endl;
    delay(2000);
    clear_screen();
    menu();
}

//for choose with delay is suitable 
void choose_level()
{
    clear_screen();
    change_color_rgb(66, 239, 245);
    cursor_to_pos(rows / 2, colm / 2 - 12);
    cout << "----------------------------" << endl;
    for (int i = 0; i < 5; i++)
    {
        cursor_to_pos(rows / 2 + i + 1, colm / 2 - 12);
        cout << "|" << endl;
    }
    for (int i = 0; i < 5; i++)
    {
        cursor_to_pos(rows / 2 + i + 1, colm / 2 + 15);
        cout << "|" << endl;
    }
    cursor_to_pos(rows / 2 + 1, colm / 2 - 10);
    cout << "please choose one level :" << endl;
    cursor_to_pos(rows / 2 + 2, colm / 2 - 10);
    cout << "1-easy" << endl;
    cursor_to_pos(rows / 2 + 3, colm / 2 - 10);
    cout << "2-medium" << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 - 10);
    cout << "3-hard" << endl;
    cursor_to_pos(rows / 2 + 5, colm / 2 - 12);
    cout << "----------------------------" << endl;
    cursor_to_pos(rows / 2 + 6, colm / 2 - 10);
    cin >> level;
    reset_color();
}

//for call setup input logic draw function
void game_load()
{
    srand(time(0));
    setup();

    //we need to call these 3 function until one user lost or the game will draw
    while (game_over == false)
    {
        draw();
        input();
        logic();
        check_for_time++;

        //choose delay for level that user select
        if (level == 1)
            delay(30);
        if (level == 2)
            delay(15);
        if (level == 3)
            delay(5);
        hide_cursor();
    }
}

//for managing the time that i want to show in screen 
void time()
{
    if (level == 3)
    {
        if (check_for_time % 10 == 0)
        {
            seconds--;
        }
        if (seconds == -1)
        {
            minutes--;
            seconds = 60;
        }
    }
    else if (level == 2)
    {
        if (check_for_time % 7 == 0)
        {
            seconds--;
        }
        if (seconds == 0)
        {
            minutes--;
            seconds = 60;
        }
    }
    else if (level == 1)
    {
        if (check_for_time % 7 == 0)
        {
            seconds--;
        }
        if (seconds == 0)
        {
            minutes--;
            seconds = 60;
        }
    }
}

//for time that user want to select 
void select_time()
{
    clear_screen();
    change_color_rgb(38, 224, 221);
    cursor_to_pos(rows / 2, colm / 2 - 10);
    cout << "please write the times that you can play:" << endl;
    // decorate for hours
    cursor_to_pos(rows / 2 + 3, colm / 2 - 10);
    cout << "-----" << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 - 10);
    cout << "|" << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 - 6);
    cout << "|" << endl;
    cursor_to_pos(rows / 2 + 5, colm / 2 - 10);
    cout << "-----" << endl;
    cursor_to_pos(rows / 2 + 2, colm / 2 - 10);
    cout << "Hours";

    // decorate for minutes
    cursor_to_pos(rows / 2 + 3, colm / 2 + 6);
    cout << "-------" << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 + 6);
    cout << "|" << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 + 12);
    cout << "|" << endl;
    cursor_to_pos(rows / 2 + 5, colm / 2 + 6);
    cout << "-------" << endl;
    cursor_to_pos(rows / 2 + 2, colm / 2 + 6);
    cout << "Minutes";

    // decorate for seconds
    cursor_to_pos(rows / 2 + 3, colm / 2 + 23);
    cout << "-------" << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 + 23);
    cout << "|" << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 + 29);
    cout << "|" << endl;
    cursor_to_pos(rows / 2 + 5, colm / 2 + 23);
    cout << "-------" << endl;
    cursor_to_pos(rows / 2 + 2, colm / 2 + 23);
    cout << "Seconds";
    reset_color();

    cursor_to_pos(rows / 2 + 4, colm / 2 - 9);
    cin >> user_hours;
    cursor_to_pos(rows / 2 + 4, colm / 2 + 7);
    cin >> user_minutes;

    //if user select the seconds more than 60 or minutes mote than 60 that put me in the trouble
    if (user_minutes > 60)
    {
        while (user_minutes > 60)
        {
            cursor_to_pos(rows / 2 + 4, colm / 2 + 7);
            cin >> user_minutes;
        }
    }
    cursor_to_pos(rows / 2 + 4, colm / 2 + 24);
    cin >> user_seconds;
    if (user_seconds > 60)
    {
        while (user_seconds > 60)
        {
            cursor_to_pos(rows / 2 + 4, colm / 2 + 24);
            cin >> user_seconds;
        }
    }
}

//show snake in the start
void load_snake1()
{
    rest_of_snake1_x[0] = user1.x;
    rest_of_snake1_x[1] = user1.x;
    rest_of_snake1_x[2] = user1.x;
    rest_of_snake1_x[3] = user1.x;
    rest_of_snake1_y[0] = user1.y - 1;
    rest_of_snake1_y[1] = user1.y - 2;
    rest_of_snake1_y[2] = user1.y - 3;
    rest_of_snake1_y[3] = user1.y - 4;
}

//show snake in the start
void load_snake2()
{
    rest_of_snake2_x[0] = user2.x;
    rest_of_snake2_x[1] = user2.x;
    rest_of_snake2_x[2] = user2.x;
    rest_of_snake2_x[3] = user2.x;
    rest_of_snake2_y[0] = user2.y - 1;
    rest_of_snake2_y[1] = user2.y - 2;
    rest_of_snake2_y[2] = user2.y - 3;
    rest_of_snake2_y[3] = user2.y - 4;
}

//to check name that user write is exist in table or not
bool is_exist(leaderboard a[], int size, string name)
{
    for (int i = 0; i < size; i++)
    {
        if (name == a[i].name)
            return true;
    }
    return false;
}

//to create the table
void make_table()
{
    if (repeat_game == 0)
    {
        show_league_table[0].name = user1_name;
        show_league_table[0].score = score1;
        show_league_table[1].name = user2_name;
        show_league_table[1].score = score2;
    }
    else
    {
        if (is_exist(show_league_table, check_for_save, user1_name) == true)
        {
            for (int i = 0; i < check_for_save; i++)
            {
                if (user1_name == show_league_table[i].name)
                {
                    show_league_table[i].score += score1;
                }
            }
        }
        if (is_exist(show_league_table, check_for_save, user2_name) == true)
        {
            for (int i = 0; i < check_for_save; i++)
            {
                if (user2_name == show_league_table[i].name)
                {
                    show_league_table[i].score += score2;
                }
            }
        }
        if (is_exist(show_league_table, check_for_save, user1_name) == false)
        {
            show_league_table[check_for_save].name = user1_name;
            show_league_table[check_for_save].score = score1;
            check_for_save++;
        }
        if (is_exist(show_league_table, check_for_save, user2_name) == false)
        {
            show_league_table[check_for_save].name = user2_name;
            show_league_table[check_for_save].score = score2;
            check_for_save++;
        }
    }
}

//to sort the table with their score
void sort_table(leaderboard a[], int size)
{

    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - 1 - i; j++)
        {
            if (a[j].score < a[j + 1].score)
            {
                swap(a[j], a[j + 1]);
            }
        }
    }
    clear_screen();
    cursor_to_pos(rows / 2 - 6, colm / 2 - 11);
    cout << "----------" << endl;
    cursor_to_pos(rows / 2 - 5, colm / 2 - 10);
    cout << "username" << endl;
    for (int i = 0; i < 15; i++)
    {
        // for user_name
        cursor_to_pos(rows / 2 + i - 5, colm / 2 - 11);
        cout << "|" << endl;
        cursor_to_pos(rows / 2 + i - 5, colm / 2 - 2);
        cout << "|" << endl;

        // for score
        cursor_to_pos(rows / 2 + i - 5, colm / 2 + 9);
        cout << "|" << endl;
        cursor_to_pos(rows / 2 + i - 5, colm / 2 + 15);
        cout << "|" << endl;
    }
    cursor_to_pos(rows / 2 - 4, colm / 2 - 10);
    cout << "--------" << endl;
    cursor_to_pos(rows / 2 + 9, colm / 2 - 11);
    cout << "----------" << endl;
    for (int i = 0; i < size; i++)
    {
        cursor_to_pos(rows / 2 + i - 3, colm / 2 - 10);
        cout << a[i].name << endl;
        cursor_to_pos(rows / 2 + i - 3, colm / 2 + 11);
        cout << a[i].score << endl;
    }

    cursor_to_pos(rows / 2 - 6, colm / 2 + 9);
    cout << "-------" << endl;
    cursor_to_pos(rows / 2 - 5, colm / 2 + 10);
    cout << "score" << endl;
    cursor_to_pos(rows / 2 - 4, colm / 2 + 10);
    cout << "-----" << endl;
    cursor_to_pos(rows / 2 + 9, colm / 2 + 9);
    cout << "-------" << endl;
    cursor_to_pos(rows / 2 + 11, colm / 2 - 10);
    cout << "press any key to back menu" << endl;

    getch();
    clear_screen();
    menu();
}


//show cursor is not good so i copy this function to hide  the cursor
void hide_cursor()
{ // get from stackoverflow to hide cursor
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(console, &info);
}

//when one user lost i call this function
void lost_game(int user)
{

    make_table();
    
    delay(5000);
    repeat_game++;
    game_over = true;
    clear_screen();
    if (user == 1)
    {
        cursor_to_pos(rows / 2, colm / 2);
        cout << user1_name << " lost!" << endl;
    }

    else if (user == 2)
    {
        cursor_to_pos(rows / 2 + 1, colm / 2);
        cout << user2_name << " lost!" << endl;
    }
    cursor_to_pos(rows / 2 + 2, colm / 2 - 4);
    cout << user1_name << " score is:" << score1 << endl;
    cursor_to_pos(rows / 2 + 3, colm / 2 - 4);
    cout << user2_name << " score is:" << score2 << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 - 12);
    cout << "------------------------------------" << endl;
    delay(2000);
    clear_screen();
    menu();
    
}


void replay_game()
{
    clear_screen();
    delay(2000);
    get_name();
    game_load();
}
void Hi()
{
    cursor_to_pos(rows / 2, colm / 2);
    change_color_rgb(136, 36, 158);
    cout << "Hi !" << endl;
    reset_color();
}


//for get name from users and then save them in user1_name and user2_name
void get_name()
{
    clear_screen();
    change_color_rgb(222, 75, 75);
    cursor_to_pos(rows / 2, colm / 2 - 40);
    cout << "player1 please write your name:" << endl;
    cursor_to_pos(rows / 2 + 1, colm / 2 - 40);
    cout << "-------------------------------" << endl;
    cursor_to_pos(rows / 2 + 2, colm / 2 - 40);
    cout << "|" << endl;
    cursor_to_pos(rows / 2 + 2, colm / 2 - 10);
    cout << "|" << endl;
    cursor_to_pos(rows / 2 + 3, colm / 2 - 40);
    cout << "-------------------------------" << endl;
    reset_color();
    change_color_rgb(184, 224, 38);
    cursor_to_pos(rows / 2, colm / 2);
    cout << "player2 please write your name:" << endl;
    cursor_to_pos(rows / 2 + 1, colm / 2);
    cout << "-------------------------------" << endl;
    cursor_to_pos(rows / 2 + 2, colm / 2);
    cout << "|" << endl;
    cursor_to_pos(rows / 2 + 2, colm / 2 + 30);
    cout << "|" << endl;
    cursor_to_pos(rows / 2 + 3, colm / 2);
    cout << "-------------------------------" << endl;
    reset_color();

    cursor_to_pos(rows / 2 + 2, colm / 2 - 39);
    cin >> user1_name;
    cursor_to_pos(rows / 2 + 2, colm / 2 + 1);
    cin >> user2_name;
    if (repeat_game == 0)
    {
        check_for_save += 2;
    }
    if (user1_name == user2_name)
    {
        while (user1_name == user2_name)
        {
            clear_screen();
            cout << "names are same please choose different!" << endl;
            cout << "player1 please write your name:" << endl;
            cin >> user1_name;
            cout << "player2 please write your name:" << endl;
            cin >> user2_name;
        }
    }
}

//to show some points that user need to read
void hint()
{
    clear_screen();
    cursor_to_pos(rows / 2, colm / 2 - 20);
    cout << "these points can help you :" << endl;
    change_color_rgb(222, 75, 75);
    cursor_to_pos(rows / 2 + 1, colm / 2 - 20);
    cout << "player 1 should use <w> <a> <s> <d> to move" << endl;
    reset_color();
    change_color_rgb(184, 224, 38);
    cursor_to_pos(rows / 2 + 2, colm / 2 - 20);
    cout << "player2 should use arrow keys to move" << endl;
    reset_color();
    change_color_rgb(159, 38, 224);
    cursor_to_pos(rows / 2 + 3, colm / 2 - 20);
    cout << "'F' means fruit and if you eat it you will achieve 10 score" << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 - 20);
    cout << "'B' means bombs and you must try to don't eat it" << endl;
    cursor_to_pos(rows / 2 + 5, colm / 2 - 20);
    reset_color();
    cout << "press ant key to start the game ..." << endl;

    getch();
}

//to show menu and options that user can select
void menu()
{
    cursor_to_pos(rows / 2 + 1, colm / 2 - 12);
    change_color_rgb(15, 217, 113);
    cout << "please choose one number!" << endl;
    cursor_to_pos(rows / 2 + 2, colm / 2 - 8);
    cout << "1-start new game" << endl;
    cursor_to_pos(rows / 2 + 3, colm / 2 - 8);
    cout << "2-league table" << endl;
    cursor_to_pos(rows / 2 + 4, colm / 2 - 8);
    cout << "3-exit" << endl;
    reset_color();
    cursor_to_pos(rows / 2 + 5, colm / 2 - 8);
    int user_choose = 0;
    cin >> user_choose;
    if (user_choose < 1 || user_choose > 3)
    {
        while (user_choose < 1 || user_choose > 3)
        {
            menu();
        }
    }
    switch (user_choose)
    {
    case 1:
        select_time();
        choose_level();
        get_name();
        hint();
        game_load();
        break;
    case 2:
        sort_table(show_league_table, check_for_save);
        break;
    case 3:
        cursor_to_pos(rows / 2 + 6, colm / 2 - 8);
        change_color_rgb(170, 217, 15);
        cout << "Good bye !!";
        reset_color();
        delay(2000);
        quit();
    }
}

int main()
{
    init_clui();
    Hi();
    menu();

    getch();
    return 0;
}
