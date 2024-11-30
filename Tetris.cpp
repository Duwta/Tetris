#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <string>
#include <queue>
#include <map>
using namespace std;

#define MAX 20

int rd_block(){
    return rand()%19;
}

int rd_color(){
    return rand()%5+1;
}

class Data{
public:
    int type_block[19][4][4] = {
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,1}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,1,0,0},
            {0,1,1,1}
        },
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,1,1,1},
            {0,0,0,1}
        },
        {
            {0,0,0,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,1,1,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,1},
            {0,1,1,1}
        },
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,1,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,1,1,1},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,1,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,0,1,1},
            {0,1,1,0}
        },
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,1,1,0},
            {0,0,1,1}
        },
        {
            {0,0,0,0},
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,0,1,0},
            {0,1,1,1}
        },
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,1,0,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,1,1,1},
            {0,0,1,0}
        },
        {
            {0,0,0,0},
            {0,0,1,0},
            {0,1,1,0},
            {0,0,1,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0}
        }
    };
    void fill_block(int x, int y, int t, int t_c, int (&board)[25][14], int (&board_color)[25][12]){
        for (int i = 3; i > -1; i--){
            for (int j = 0; j < 4; j++){
                if (type_block[t][i][j]){
                    if (t == 0){
                        board[x-(4-i-1)][y+j] = type_block[t][i][j];
                        board_color[x-(4-i-1)][y+j] = t_c;
                    }
                    else{
                        board[x-(4-i-1)][y+j-1] = type_block[t][i][j];
                        board_color[x-(4-i-1)][y+j-1] = t_c;
                    }
                }
            }
        }
    }
    bool check(int board[25][14], queue<int> &row_full){
        for (int i = 23; i > 2; i--){
            int cnt = 0;
            for (int j = 1; j < 11; j++){
                if (board[i][j]) cnt++;
            }
            if (cnt == 10) row_full.push(i);
        }
        if (row_full.empty()){
            return false;
        }
        return true;
    }
    void delete_oblock(int x, int y, int t, int (&board)[25][14], int (&board_color)[25][12]){
        for (int i = 3; i > -1; i--){
            for (int j = 0; j < 4; j++){
                if (type_block[t][i][j]){
                    if (t == 0){
                        board[x-(4-i-1)][y+j] = board_color[x-(4-i-1)][y+j] = 0;
                    }
                    else{
                        board[x-(4-i-1)][y+j-1] = board_color[x-(4-i-1)][y+j-1] = 0;
                    }
                }
            }
        }
    }
    int delete_row(int x, int y, int t, int t_c, int (&board)[25][14], int (&board_color)[25][12], queue<int> &q){
        delete_oblock(x,y,t,board,board_color);
        int cnt = 0; //Đếm số dòng đã xóa
        while (!q.empty()){
            int row = q.front();
            q.pop();
            for (int i = row; i > 0; i--){
                int k = i+cnt;
                for (int j = 1; j < 11; j++){
                    board[k][j] = board[k-1][j];
                    board_color[k][j] = board_color[k-1][j];
                }
            }
            cnt++;
        }
        fill_block(x,y,t,t_c,board,board_color);
        return cnt;
    }
    bool loss(int board[25][14]){
        for (int i = 1; i < 11; i++){
            if (board[3][i]){
                return true;
            }
        }
        return false;
    }
};

class One : public Data{
public:
    int board[25][14], board_color[25][12] = {0};
    int X, Y;
    int index_type;
    int color_type;
    queue<int> row_full;
    queue<int> list_index;
    queue<int> list_color;

    void create(){
        for (int i = 0; i < 24; i++){
            board[i][0] = board[i][11] = board[i][12] = board[i][13] = 1;
            for (int j = 1; j < 11; j++){
                board[i][j] = 0;
                board_color[i][j] = 0;
            }
        }
        for (int i = 0; i < 14; i++) board[24][i] = 1;
        index_type = rd_block();
        color_type = rd_color();
        list_index.push(rd_block());
        list_color.push(rd_color());
        X = 3;
        Y = (index_type == 0? 4 : 5);
        fill_block(X,Y,index_type,color_type,board,board_color);
    }

    void reset(){
        index_type = list_index.front();
        color_type = list_color.front();
        list_index.push(rd_block());
        list_color.push(rd_color());
        list_index.pop();
        list_color.pop();
        X = 3;
        Y = (index_type == 0? 4 : 5);
        fill_block(X,Y,index_type,color_type,board,board_color);
    }
};

class Two : public Data{
public:
    int board1[25][14], board2[25][14];
    int board_color1[25][12] = {0};
    int board_color2[25][12] = {0};
    int X1, Y1, X2, Y2;
    int index_type1, index_type2;
    int color_type1, color_type2;
    queue<int> list_index1, list_index2;
    queue<int> list_color1, list_color2;
    queue<int> row_full1, row_full2;
    
    void create(){
        for (int i = 0; i < 24; i++){
            board1[i][0] = board1[i][11] = board1[i][12] = board1[i][13] = 1;
            board2[i][0] = board2[i][11] = board2[i][12] = board2[i][13] = 1;
            for (int j = 1; j < 11; j++){
                board1[i][j] = board2[i][j] = 0;
                board_color1[i][j] = board_color2[i][j] = 0;
            }
        }
        for (int i = 0; i < 14; i++) board1[24][i] = board2[24][i] = 1;
        int temp = rd_block();
        index_type1 = index_type2 = temp;
        temp = rd_color();
        color_type1 = color_type2 = temp;
        temp = rd_block();
        list_index1.push(temp);
        list_index2.push(temp);
        temp = rd_color();
        list_color1.push(temp);
        list_color2.push(temp);
        X1 = X2 = 3;
        Y1 = Y2 = (index_type1 == 0? 4 : 5);
        fill_block(X1,Y1,index_type1,color_type1,board1,board_color1);
        fill_block(X2,Y2,index_type2,color_type2,board2,board_color2);
    }

    void push(int t, int t_c){
        list_index1.push(t);
        list_index2.push(t);
        list_color1.push(t_c);
        list_color2.push(t_c);
    }
    void reset1(){
        index_type1 = list_index1.front();
        color_type1 = list_color1.front();
        push(rd_block(),rd_color());
        list_index1.pop();
        list_color1.pop();
        X1 = 3;
        Y1 = (index_type1 == 0? 4 : 5);
        fill_block(X1,Y1,index_type1,color_type1,board1,board_color1);
    }

    void reset2(){
        index_type2 = list_index2.front();
        color_type2 = list_color2.front();
        push(rd_block(),rd_color());
        list_index2.pop();
        list_color2.pop();
        X2 = 3;
        Y2 = (index_type2 == 0? 4 : 5);
        fill_block(X2,Y2,index_type2,color_type2,board2,board_color2);
    }
};

class Update : public Data{
public:
    void turn(int &x, int &y, int &t, int t_c, int (&board)[25][14], int (&board_color)[25][12]){
        delete_oblock(x,y,t,board,board_color);
        int arr1[] = {1,11,13};
        int arr2[] = {5,9,17};
        int arr3[] = {3,5,7,9,11,13,15,17};

        if (t == 0) y += 1;
        else if (t == 1){
            if (y >= 9) y = 7;
            else if (y != 1) y -= 1;
        }
        else if ((find(begin(arr3),end(arr3),t) != end(arr3)) && y == 9) y -= 1;

        if (find(begin(arr1),end(arr1),t) != end(arr1)) t -= 1;
        else if (find(begin(arr2),end(arr2),t) != end(arr2)) t -= 3;
        else if (t != 18) t += 1;
        fill_block(x,y,t,t_c,board,board_color);
    }
    void fall(int &x, int &y, int t, int t_c, int (&board)[25][14], int (&board_color)[25][12]){
        delete_oblock(x,y,t,board,board_color);
        x++;
        fill_block(x,y,t,t_c,board,board_color);
    }
    void move_l(int &x, int &y, int t, int t_c, int (&board)[25][14], int (&board_color)[25][12]){
        delete_oblock(x,y,t,board,board_color);
        y--;
        fill_block(x,y,t,t_c,board,board_color);
    }
    void move_r(int &x, int &y, int t, int t_c, int (&board)[25][14], int (&board_color)[25][12]){
        delete_oblock(x,y,t,board,board_color);
        y++;
        fill_block(x,y,t,t_c,board,board_color);
    }
};

class Check_update : public Data{
public:
    bool turn(int x, int y, int t, int board[25][14]){
        int arr[MAX] = {-1};
        int k = 0;
        if (t == 0){
            for (int i = x-3; i < x; i++){
                for (int j = y; j < y+2; j++){
                    arr[k] = board[i][j];
                    k++;
                }
            }
        }
        else if (t == 1){
            if (y >= 9){
                for (int i = x-3; i < x; i++){
                    for (int j = y+1; j < min(y+2,11); j++){
                        arr[k] = board[i][j];
                        k++;
                    }
                }
                for (int j = 7; j < 11; j++){
                    if (j != y){
                        arr[k] = board[x][j];
                        k++;
                    }
                }
            }
            else if (y == 1){
                arr[0] = board[x-3][y+1];
                arr[1] = board[x-1][y+3];
                arr[2] = board[x][y+3];
                k = 3;
                for (int i = x-2; i < x+1; i++){
                    for (int j = y+1; j < y+3; j++){
                        arr[k] = board[i][j];
                        k++;
                    }
                }
            }
            else{
                arr[0] = board[x][y-1];
                k = 1;
                for (int i = x-3; i < x+1; i++){
                    for (int j = y+1; j < y+3; j++){
                        arr[k] = board[i][j];
                        k++;
                    }
                }
            }
        }
        else if (t == 2){
            arr[0] = board[x-2][y];
            arr[1] = board[x-2][y+1];
            arr[2] = board[x-1][y+1];
        }
        else if (t == 3){
            if (y == 9){
                for (int i = x-1; i < x+1; i++){
                    for (int j = y-1; j < y+2; j += 2){
                        arr[k] = board[i][j];
                        k++;
                    }
                }
            }
            else{
                arr[0] = board[x-2][y+2];
                arr[1] = board[x-1][y+1];
                arr[2] = board[x-1][y+2];
                arr[3] = board[x][y+2];
            }
        }
        else if (t == 4){
            for (int i = x-2; i < x+1; i += 2){
                for (int j = y; j < y+2; j++){
                    arr[k] = board[i][j];
                    k++;
                }
            }
        }
        else if (t == 5){
            if (y == 9){
                arr[0] = board[x-1][y-1];
                arr[1] = board[x-1][y];
                arr[2] = board[x][y-1];
            }
            else{
                arr[0] = board[x-1][y];
                k = 1;
                for (int i = x-2; i < x+1; i++){
                    arr[k] = board[i][y+2];
                    k++;
                }
            }
        }
        else if (t == 6){
            arr[0] = board[x-2][y];
            arr[1] = board[x-1][y];
            arr[2] = board[x-1][y+1];
        }
        else if (t == 7){
            if (y == 9){
                arr[0] = board[x-1][y-1];
                arr[1] = board[x][y-1];
                arr[2] = board[x-2][y+1];
                arr[3] = board[x-1][y+1];
            }
            else{
                arr[0] = board[x-2][y+1];
                arr[1] = board[x-1][y+1];
                arr[2] = board[x-1][y+2]; 
            }
        }
        else if (t == 8){
            arr[0] = board[x-2][y];
            arr[1] = board[x-2][y+1];
            arr[2] = board[x][y+1];
            arr[3] = board[x][y+2];
        }
        else if (t == 9){
            if (y == 9){
                arr[0] = board[x-1][y];
                arr[1] = board[x][y-1];
                arr[2] = board[x][y];
            }
            else{
                arr[0] = board[x][y];
                k = 1;
                for (int i = x-2; i < x+1; i++){
                    arr[k] = board[i][y+2];
                    k++;
                }
            }
        }
        else if (t == 10){
            arr[0] = board[x-2][y];
            arr[1] = board[x-1][y];
            arr[2] = board[x][y+2];
        }
        else if (t == 11){
            if (y == 9){
                arr[0] = board[x-2][y+1];
                arr[1] = board[x][y-1];
                arr[2] = board[x][y];
            }
            else{
                arr[0] = board[x][y];
                arr[1] = board[x-2][y+1];
                arr[2] = board[x-1][y+2];
            }
        }
        else if (t == 12){
            arr[0] = board[x-2][y];
            arr[1] = board[x-2][y+1];
            arr[2] = board[x][y];
        }
        else if (t == 13){
            if (y == 9){
                arr[0] = board[x-1][y-1];
                arr[1] = board[x][y-1];
                arr[2] = board[x][y+1];
            }
            else{
                arr[0] = board[x][y+1];
                k = 1;
                for (int i = x-2; i < x+1; i++){
                    arr[k] = board[i][y+2];
                    k++;
                }
            }
        }
        else if (t == 14){
            arr[0] = board[x-2][y];
            arr[1] = board[x-1][y];
        }
        else if (t == 15){
            if (y == 9){
                arr[0] = board[x-2][y+1];
                arr[1] = board[x-1][y-1];
                arr[2] = board[x][y-1];
                arr[3] = board[x][y+1];
            }
            else{
                arr[0] = board[x-2][y+1];
                arr[1] = board[x][y+1];
            }
        }
        else if (t == 16){
            arr[0] = board[x-2][y];
            arr[1] = board[x-2][y+1];
            arr[2] = board[x][y];
            arr[3] = board[x][y+2];
        }
        else if (t == 17){
            if (y == 9){
                arr[0] = board[x][y-1];
                arr[1] = board[x][y];
            }
            else{
                arr[0] = board[x][y];
                k = 1;
                for (int i = x-2; i < x+1; i++){
                    arr[k] = board[i][y+2];
                    k++;
                }
            }
        }
        if (find(begin(arr),end(arr),1) != end(arr)){
            return false;
        }
        return true;
    }
    bool move_l(int x, int y, int t, int board[25][14]){
        int arr[MAX] = {-1};
        int k = 0;
        if (t == 0){
            arr[0] = board[x][y-1];
        }
        else if (t == 1){
            for (int i = x-3; i < x+1; i++){
                arr[k] = board[i][y-1];
                k++;
            }
        }
        else if (t == 2 || t == 8 || t == 18){
            for (int i = x-1; i < x+1; i++){
                arr[k] = board[i][y-1];
                k++;
            }
        }
        else if (t == 3 || t == 7 || t == 15){
            for (int i = x-2; i < x+1; i++){
                arr[k] = board[i][y-1];
                k++;
            }
        }
        else if (t == 10 || t == 14){
            arr[0] = board[x][y-1];
            arr[1] = board[x-1][y];
        }
        else if (t == 12 || t == 16){
            arr[0] = board[x][y];
            arr[1] = board[x-1][y-1];
        }
        else if (t == 4){
            arr[0] = board[x][y+1];
            arr[1] = board[x-1][y-1];
        }
        else if (t == 5){
            arr[0] = board[x][y-1];
            arr[1] = board[x-1][y];
            arr[2] = board[x-2][y];
        }
        else if (t == 6){
            arr[0] = board[x][y-1];
            arr[1] = board[x-1][y+1];
        }
        else if (t == 9){
            arr[0] = board[x][y];
            arr[1] = board[x-1][y];
            arr[2] = board[x][y-1];
        }
        else if (t == 11){
            arr[0] = board[x][y];
            arr[1] = board[x-1][y-1];
            arr[2] = board[x-2][y-1];
        }
        else if (t == 13){
            arr[0] = board[x][y-1];
            arr[1] = board[x-1][y-1];
            arr[2] = board[x-2][y];
        }
        else if (t == 17){
            arr[0] = board[x][y];
            arr[1] = board[x-1][y-1];
            arr[2] = board[x-2][y];
        }
        if (find(begin(arr),end(arr),1) != end(arr)){
            return false;
        }
        return true;
    }
    bool move_r(int x, int y, int t, int board[25][14]){
        int arr[MAX] = {-1};
        int k = 0;
        if (t == 0){
            arr[0] = board[x][y+4];
        }
        else if (t == 1){
            for (int i = x-3; i < x+1; i++){
                arr[k] = board[i][y+1];
                k++;
            }
        }
        else if (t == 4 || t == 6){
            arr[0] = board[x-1][y+3];
            arr[1] = board[x][y+3];
        }
        else if (t == 5 || t == 9 || t == 17){
            for (int i = x-2; i < x+1; i++){
                arr[k] = board[i][y+2];
                k++;
            }
        }
        else if (t == 10 || t == 16){
            arr[0] = board[x][y+2];
            arr[1] = board[x-1][y+3];
        }
        else if (t == 12 || t == 14){
            arr[0] = board[x-1][y+2];
            arr[1] = board[x][y+3];
        }
        else if (t == 2){
            arr[0] = board[x-1][y+1];
            arr[1] = board[x][y+3];
        }
        else if (t == 3){
            arr[0] = board[x-2][y+2];
            arr[1] = board[x-1][y+1];
            arr[2] = board[x][y+1];
        }
        else if (t == 7){
            arr[0] = board[x-2][y+1];
            arr[1] = board[x-1][y+1];
            arr[2] = board[x][y+2];
        }
        else if (t == 8){
            arr[0] = board[x-1][y+3];
            arr[1] = board[x][y+1];
        }
        else if (t == 11){
            arr[0] = board[x-2][y+1];
            arr[1] = board[x-1][y+2];
            arr[2] = board[x][y+2];
        }
        else if (t == 13){
            arr[0] = board[x-2][y+2];
            arr[1] = board[x-1][y+2];
            arr[2] = board[x][y+1];
        }
        else if (t == 15){
            arr[0] = board[x-2][y+1];
            arr[1] = board[x-1][y+2];
            arr[2] = board[x][y+1];
        }
        else if (t == 18){
            arr[0] = board[x-1][y+2];
            arr[1] = board[x][y+2];
        }
        if (find(begin(arr),end(arr),1) != end(arr)){
            return false;
        }
        return true;
    }
    bool fall(int x, int y, int t, int board[25][14]){
        int arr[MAX] = {-1};
        int k = 0;
        if (t == 0){
            for (int j = y; j < y+4; j++){
                arr[k] = board[x+1][j];
                k++;
            }
        }
        else if (t == 1){
            arr[0] = board[x+1][y];
        }
        else if (t == 2 || t == 6 || t == 14){
            for (int j = y; j < y+3; j++){
                arr[k] = board[x+1][j];
                k++;
            }
        }
        else if (t == 5 || t == 7 || t == 18){
            arr[0] = board[x+1][y];
            arr[1] = board[x+1][y+1];
        }
        else if (t == 11 || t == 17){
            arr[0] = board[x][y];
            arr[1] = board[x+1][y+1];
        }
        else if (t == 13 || t == 15){
            arr[0] = board[x+1][y];
            arr[1] = board[x][y+1];
        }
        else if (t == 3){
            arr[0] = board[x+1][y];
            arr[1] = board[x-1][y+1];
        }
        else if (t == 4){
            arr[0] = board[x][y];
            arr[1] = board[x][y+1];
            arr[2] = board[x+1][y+2];
        }
        else if (t == 8){
            arr[0] = board[x+1][y];
            arr[1] = board[x][y+1];
            arr[2] = board[x][y+2];
        }
        else if (t == 9){
            arr[0] = board[x-1][y];
            arr[1] = board[x+1][y+1];
        }
        else if (t == 10){
            arr[0] = board[x+1][y];
            arr[1] = board[x+1][y+1];
            arr[2] = board[x][y+2];
        }
        else if (t == 12){
            arr[0] = board[x][y];
            arr[1] = board[x+1][y+1];
            arr[2] = board[x+1][y+2];
        }
        else if (t == 16){
            arr[0] = board[x][y];
            arr[1] = board[x+1][y+1];
            arr[2] = board[x][y+2];
        }
        if (find(begin(arr),end(arr),1) != end(arr)){
            return false;
        }
        return true;
    }
};

class Interaction{
private:
    //Khai báo biến màn hình
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_DisplayMode displayMode;

    //Bảng và khối
    SDL_Texture* board;
    SDL_Texture* block[6];
    //Bảng next block và score
    SDL_Texture* next1;
    SDL_Texture* next2;

    SDL_Texture* scoreboard1;
    SDL_Texture* scoreboard2;
    //Bảng tỉ số mode 2 player
    SDL_Texture* win2;
    //Numbers
    SDL_Texture* number[10];
    //Bảng thua
    SDL_Texture* end1;
    SDL_Texture* end2;
    //Bảng menu
    SDL_Texture* menu;
    SDL_Texture* selectMode;

    //Tên game
    SDL_Texture* name;

    //Các nút
    SDL_Texture* pause[2];
    SDL_Texture* try_again[2];
    SDL_Texture* exit[2];
    SDL_Texture* cntinue[2];
    SDL_Texture* play[2];
    SDL_Texture* settings[2];
    SDL_Texture* onePlayer[2];
    SDL_Texture* twoPlayers[2];

    //Kích thước
    int w_board, h_board;
    int w_block, h_block;

    int w_next1, h_next1;
    int w_next2, h_next2;
    int w_scoreboard1, h_scoreboard1;
    int w_scoreboard2, h_scoreboard2;

    int w_win2, h_win2;

    int w_number[10], h_number[10];

    int w_end1, h_end1;
    int w_end2, h_end2;

    int w_menu, h_menu;
    int w_selectMode, h_selectMode;

    int w_name, h_name;

    int w_pause, h_pause;
    int w_try_again, h_try_again;
    int w_exit, h_exit;
    int w_cntinue, h_cntinue;
    int w_play, h_play;
    int w_settings, h_settings;
    int w_onePlayer, h_onePlayer;
    int w_twoPlayers, h_twoPlayers;

    //Điểm của player
    int score = 0;
    int score1 = 0;
    int score2 = 0;

    //Đếm số lần win
    int cnt_win[3] = {0};

    //Tọa độ 
    SDL_Rect crd_board1;
    SDL_Rect crd_board2;
    SDL_Rect crd_board3;
    SDL_Rect crd_block1[25][12];
    SDL_Rect crd_block2[25][12];
    SDL_Rect crd_block3[25][12];

    SDL_Rect crd_next1;
    SDL_Rect crd_next2;

    SDL_Rect crd_scoreboard1;
    SDL_Rect crd_scoreboard2;

    SDL_Rect crd_win2;

    SDL_Rect crd_next_block[3][4][4];
    SDL_Rect crd_next_block1[3][4][4];
    SDL_Rect crd_next_block2[3][4][4];

    SDL_Rect crd_end1;
    SDL_Rect crd_end2;

    SDL_Rect crd_menu;
    SDL_Rect crd_selectMode;

    SDL_Rect crd_name;

    SDL_Rect crd_pause;
    SDL_Rect crd_try_again[2];
    SDL_Rect crd_exit[4];
    SDL_Rect crd_cntinue;
    SDL_Rect crd_play;
    SDL_Rect crd_settings;
    SDL_Rect crd_onePlayer;
    SDL_Rect crd_twoPlayers;

    //Lưu thời gian nhấn key
    map<SDL_Scancode, Uint32> key_hold;

    //Thời gian rơi và thời gian nhấn giữ key
    Uint32 hold_time = 100;
    Uint32 fall_levels[7] = {1000,800,500,200,100,50,20};
    int level = 0;
    int level1 = 0;
    int level2 = 0;

    //Lấy dữ liệu key đang nhấn trên bàn phím
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    //Danh sách các key hợp lệ
    SDL_Scancode keyList[6] = {
        SDL_SCANCODE_A,
        SDL_SCANCODE_S,
        SDL_SCANCODE_D,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_RIGHT,
    };

    //val bool thua
    bool loss = false;
    bool loss1 = false;
    bool loss2 = false;

    //bool thoát game
    bool hard_exit = false;

    //chọn ảnh button khi click nút
    map<string, int> clicking;
    //kiểm tra button vừa được click
    map<string, bool> isPressed;

    //Xử lí khung hình
    Uint32 frameTime; //Khoảng thời gian trong một khung hình
    Uint32 frameStart; //Thời gian bắt đầu 1 khung hình
    Uint32 frameDelay = 4; //240fps

public:
    //Hàm tạo
    Interaction(){
        //Tạo màn hình
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            cerr << "Could not initialize SDL_image: " << IMG_GetError() << endl;
            SDL_Quit();
        }

        window = SDL_CreateWindow("Tetris",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    0, 0, 
                                    SDL_WINDOW_FULLSCREEN_DESKTOP);
        if (!window) {
            cerr << "Could not create window: " << SDL_GetError() << endl;
            IMG_Quit();
            SDL_Quit();
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            cerr << "Could not create renderer: " << SDL_GetError() << endl;
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
        }
        //Khởi tạo background
        backgroundTexture = IMG_LoadTexture(renderer, "graphic/background/background.png");
        if (!backgroundTexture) {
            cerr << "Could not load background image: " << IMG_GetError() << endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
        }

        // Lấy chế độ hiển thị hiện tại
        if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
            std::cout << "Could not get current display mode! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }

        //Lấy ảnh tử folder
        board = loadTexture("graphic/gameplay/board.png");
        block[0] = nullptr;
        block[1] = loadTexture("graphic/gameplay/red.png");
        block[2] = loadTexture("graphic/gameplay/blue.png");
        block[3] = loadTexture("graphic/gameplay/yellow.png");
        block[4] = loadTexture("graphic/gameplay/green.png");
        block[5] = loadTexture("graphic/gameplay/purple.png");
        next1 = loadTexture("graphic/gameplay1/next1.png");
        next2 = loadTexture("graphic/gameplay2/next2.png");
        scoreboard1 = loadTexture("graphic/gameplay1/score1.png");
        scoreboard2 = loadTexture("graphic/gameplay2/score2.png");
        win2 = loadTexture("graphic/gameplay2/win2.png");
        for (int i = 0; i < 10; i++){
            number[i] = loadTexture("graphic/numbers/"+to_string(i)+".png");
        }
        end1 = loadTexture("graphic/gameplay1/your_score.png");
        end2 = loadTexture("graphic/gameplay2/player's_scores.png");
        menu = loadTexture("graphic/menu/settings_window.png");
        selectMode = loadTexture("graphic/menu/select_mode.png");
        name = loadTexture("graphic/menu/name.png");

        loadTextureButton(pause,"pause");
        loadTextureButton(try_again,"try_again");
        loadTextureButton(exit,"exit");
        loadTextureButton(cntinue,"continue");
        loadTextureButton(play,"play");
        loadTextureButton(settings,"settings");
        loadTextureButton(onePlayer,"1_player");
        loadTextureButton(twoPlayers,"2_players");

        //Kích thước bảng, khối
        cal_size(w_board,h_board,78,148);
        cal_size(w_block,h_block,7,7);
        //Kích thước bảng next block
        cal_size(w_next1,h_next1,51,51);
        cal_size(w_next2,h_next2,80,50);

        //Kích thước bảng score
        cal_size(w_scoreboard1,h_scoreboard1,51,29);
        cal_size(w_scoreboard2,h_scoreboard2,72,28);

        //Kích thước bảng end
        cal_size(w_win2,h_win2,64,28);

        //Kích thước các số
        for (int i = 0; i < 10; i++){
            if (i == 1){
                cal_size(w_number[i],h_number[i],3,5);
            }
            else{
                cal_size(w_number[i],h_number[i],4,5);
            }
        }

        //Kích thước bảng end
        cal_size(w_end1,h_end1,96,51);
        cal_size(w_end2,h_end2,150,100);

        //Kích thước menu
        cal_size(w_menu,h_menu,150,75);
        cal_size(w_selectMode,h_selectMode,150,75);

        //Kích thước tên
        cal_size(w_name,h_name,158,28);

        //Kích thước các nút
        cal_size(w_pause,h_pause,12,11);
        cal_size(w_try_again,h_try_again,53,15);
        cal_size(w_exit,h_exit,31,15);
        cal_size(w_cntinue,h_cntinue,51,15);
        cal_size(w_play,h_play,32,15);
        cal_size(w_settings,h_settings,52,15);
        cal_size(w_onePlayer,h_onePlayer,47,15);
        cal_size(w_twoPlayers,h_twoPlayers,53,15);

        //Tọa độ, thuộc tính của bảng, khối
        crd_board1 = {
            (displayMode.w-w_board)/2,
            (displayMode.h-h_board)/2,
            w_board,
            h_board
        };
        crd_board2 = {
            displayMode.w*15/266,
            (displayMode.h-h_board)/2,
            w_board,
            h_board
        };
        crd_board3 = {
            displayMode.w*175/266,
            (displayMode.h-h_board)/2,
            w_board,
            h_board
        };

        for (int i = 4; i < 24; i++){
            for (int j = 1; j < 11; j++){
                int x = i-3;
                crd_block1[i][j] = {
                    displayMode.w/2+w_block*(-5+j-1),
                    displayMode.h/2+h_block*(-10+x-1),
                    w_block,
                    h_block
                };
                crd_block2[i][j] = {
                    displayMode.w*19/266+w_block*(j-1),
                    displayMode.h/2+h_block*(-10+x-1),
                    w_block,
                    h_block
                };
                crd_block3[i][j] = {
                    displayMode.w*179/266+w_block*(j-1),
                    displayMode.h/2+h_block*(-10+x-1),
                    w_block,
                    h_block
                };
            }
        }
        //Tọa độ bảng next block và khối next block
        get_crd(crd_next1,w_next1,h_next1,23,78);
        get_crd(crd_next2,w_next2,h_next2,94,51);

        for (int i = 0; i < 4; i++){//Tọa độ khối next của 3 nhóm block để căn giữa từng loại
            for (int j = 0; j < 4; j++){
                crd_next_block[0][i][j] = { //Khối lệch về bên phải
                    displayMode.w*31/266+w_block*j,
                    displayMode.h*88/150+h_block*i,
                    w_block,
                    h_block
                };
                crd_next_block[1][i][j] = { //Khối I đứng
                    displayMode.w*38/266+w_block*j,
                    displayMode.h*92/150+h_block*i,
                    w_block,
                    h_block
                };
                crd_next_block[2][i][j] = { //Khối nằm giữa
                    displayMode.w*35/266+w_block*j,
                    displayMode.h*88/150+h_block*i,
                    w_block,
                    h_block                    
                };
            }
        }

        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                crd_next_block1[0][i][j] = {
                    displayMode.w*97/266+w_block*j,
                    displayMode.h*105/266+h_block*i,
                    w_block,
                    h_block
                };
                crd_next_block1[1][i][j] = {
                    displayMode.w*104/266+w_block*j,
                    displayMode.h*112/266+h_block*i,
                    w_block,
                    h_block
                };
                crd_next_block1[2][i][j] = {
                    displayMode.w*100/266+w_block*j,
                    displayMode.h*108/266+h_block*i,
                    w_block,
                    h_block
                };
            }
        }

        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                crd_next_block2[0][i][j] = {
                    displayMode.w*136/266+w_block*j,
                    displayMode.h*105/266+h_block*i,
                    w_block,
                    h_block
                };
                crd_next_block2[1][i][j] = {
                    displayMode.w*143/266+w_block*j,
                    displayMode.h*112/266+h_block*i,
                    w_block,
                    h_block
                };
                crd_next_block2[2][i][j] = {
                    displayMode.w*139/266+w_block*j,
                    displayMode.h*108/266+h_block*i,
                    w_block,
                    h_block
                };
            }
        }

        //Tọa độ bảng scoreboard
        get_crd(crd_scoreboard1,w_scoreboard1,h_scoreboard1,195,22);
        get_crd(crd_scoreboard2,w_scoreboard2,h_scoreboard2,98,22);

        //Tọa độ bảng win
        get_crd(crd_win2,w_win2,h_win2,102,102);

        //tọa độ bảng end
        get_crd_setting(crd_end1,w_end1,h_end1);
        get_crd_setting(crd_end2,w_end2,h_end2);

        //Tọa độ menu
        get_crd_setting(crd_menu,w_menu,h_menu);
        get_crd_setting(crd_selectMode,w_selectMode,h_selectMode);

        //Tọa độ name
        get_crd(crd_name,w_name,h_name,55,37);

        //Tọa độ các nút
        get_crd(crd_pause,w_pause,h_pause,1,1);
        get_crd(crd_try_again[0],w_try_again,h_try_again,91,80);
        get_crd(crd_try_again[1],w_try_again,h_try_again,81,94);
        get_crd(crd_exit[0],w_exit,h_exit,145,80);
        get_crd(crd_exit[1],w_exit,h_exit,155,86);
        get_crd(crd_exit[2],w_exit,h_exit,155,94);
        get_crd(crd_exit[3],w_exit,h_exit,118,80);
        get_crd(crd_cntinue,w_cntinue,h_cntinue,80,86);
        get_crd(crd_play,w_play,h_play,118,68);
        get_crd(crd_settings,w_settings,h_settings,108,84);
        get_crd(crd_onePlayer,w_onePlayer,h_onePlayer,75,68);
        get_crd(crd_twoPlayers,w_twoPlayers,h_twoPlayers,140,68);
    }
    //Tạo texture ảnh bất kì
    SDL_Texture* loadTexture(const string& path){
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == nullptr) {
            std::cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);

        if (texture == nullptr) {
            std::cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }

        return texture;
    }
    //Tạo texture ảnh nút bất kì
    void loadTextureButton(SDL_Texture* button[2], string name){
        button[0] = loadTexture("graphic/menu/"+name+".png");
        button[1] = loadTexture("graphic/menu/"+name+"_hold.png");
    }
    //Vẽ các khối ra màn hình
    void draw_block(int board_color[25][12], SDL_Rect crd_block[25][12]){
        for (int i = 4; i < 24; i++){
            for (int j = 1; j < 11; j++){
                if (board_color[i][j] != 0){
                    SDL_RenderCopy(renderer,block[board_color[i][j]],nullptr,&crd_block[i][j]);
                }
            }
        }
    }

    //Vẽ thông báo khối tiếp theo
    void draw_next_block(Data data, int t, int t_c, SDL_Rect crd_n[3][4][4]){
        int arr1[] = {0,3,5,7,9,11,13,15,17,18};

        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                if (data.type_block[t][i][j]){
                    if (find(begin(arr1),end(arr1),t) != end(arr1)){
                        SDL_RenderCopy(renderer,block[t_c],nullptr,&crd_n[2][i][j]);
                    }
                    else if (t == 1){
                        SDL_RenderCopy(renderer,block[t_c],nullptr,&crd_n[1][i][j]);
                    }
                    else{
                        SDL_RenderCopy(renderer,block[t_c],nullptr,&crd_n[0][i][j]);
                    }
                }
            }
        }
    }

    //Tính kích thước các ảnh
    void cal_size(int &w, int &h, int w_r, int h_r){
        w = displayMode.h*w_r/150;
        h = displayMode.h*h_r/150;
    }

    //Tính tọa độ các màn hình setting
    void get_crd_setting(SDL_Rect &name,int w, int h){
        name = {
            (displayMode.w-w)/2,
            (displayMode.h-h)/2,
            w,
            h,
        };
    }
    //Tính tọa độ các ảnh khác
    void get_crd(SDL_Rect &name, int w, int h, int crdx, int crdy){
        name = {
            displayMode.w*crdx/266,
            displayMode.h*crdy/150,
            w,
            h
        };
    }

    //Tính tọa độ các chữ số
    void cal_crd_num(SDL_Rect *crd_num, int n, string score, int x, int y, int w){
        int range_x = 0;
        for (int i = 0; i < n; i++){
            if (score[i] == '1'){
                range_x += 3;
            }
            else{
                range_x += 4;
            }
            range_x += (i < n-1? 1 : 0);
        }
        int st_crdx = x+(w-range_x)/2;
        int crdy = y;
        for (int i = 0; i < n; i++){
            string str(1, score[i]);
            crd_num[i] = {
                displayMode.w*st_crdx/266,
                displayMode.h*crdy/150,
                w_number[stoi(str)],
                h_number[stoi(str)]
            };
            st_crdx += (score[i] == '1'? 4 : 5);
        }
    }

    //Viết điểm
    void write_score(int score, int x, int y, int w){
        string sscore = to_string(score);
        int n = sscore.size();
        SDL_Rect crd_number[n];
        cal_crd_num(crd_number,n,sscore,x,y,w);
        for (int i = 0; i < n; i++){
            string str(1, sscore[i]);
            SDL_RenderCopy(renderer,number[stoi(str)],nullptr,&crd_number[i]);
        }
    }

    //Tính điểm
    int scoring(int cnt){
        if (cnt == 1){
            return 100;
        }
        else if (cnt == 2){
            return 250;
        }
        else if (cnt == 3){
            return 500;
        }
        else{
            return 800;
        }
    }

    //Tính level
    int cal_level(int score){
        for (int i = 0, p = 3000; i < 6; i++, p += 3000+1000*i){
            if (score < p){
                return i;
            }
        }
        return 6;
    }

    //kiểm tra click button
    bool contains(int x, int y, SDL_Rect crd){
        return (x >= crd.x && x <= crd.x+crd.w && y >= crd.y && y <= crd.y+crd.h);
    }

    //Mode 1 player
    void mode_one_player(Data data, One one, Update update, Check_update check_update){
        //Khởi tạo game
        one.create();
        Uint32 fall_time = SDL_GetTicks();

        bool running = true;
        while (running){
            frameStart = SDL_GetTicks();
            SDL_Event event;
            while (SDL_PollEvent(&event)){
                // Kiểm tra sự kiện thoát
                if (event.type == SDL_QUIT){
                    running = false;
                    hard_exit = true;
                }
                // Kiểm tra phím nhấn
                else if (event.type == SDL_KEYDOWN){
                    if (find(begin(keyList),end(keyList),event.key.keysym.scancode) != end(keyList)){
                        key_hold[event.key.keysym.scancode] = SDL_GetTicks();
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_W || event.key.keysym.scancode == SDL_SCANCODE_UP){
                        if (check_update.turn(one.X,one.Y,one.index_type,one.board)){
                            update.turn(one.X,one.Y,one.index_type,one.color_type,one.board,one.board_color);
                        }
                    }
                }
                //Kiểm tra phím thả
                else if (event.type == SDL_KEYUP){
                    if (event.key.keysym.scancode == keyList[0] || event.key.keysym.scancode == keyList[3]){
                        if (check_update.move_l(one.X,one.Y,one.index_type,one.board) && SDL_GetTicks()-key_hold[event.key.keysym.scancode] < hold_time){
                            update.move_l(one.X,one.Y,one.index_type,one.color_type,one.board,one.board_color);
                        }
                        key_hold.erase(event.key.keysym.scancode);
                    }
                    else if (event.key.keysym.scancode == keyList[2] || event.key.keysym.scancode == keyList[5]){
                        if (check_update.move_r(one.X,one.Y,one.index_type,one.board) && SDL_GetTicks()-key_hold[event.key.keysym.scancode] < hold_time){
                            update.move_r(one.X,one.Y,one.index_type,one.color_type,one.board,one.board_color);
                        }
                        key_hold.erase(event.key.keysym.scancode);
                    }
                    else if (event.key.keysym.scancode == keyList[1] || event.key.keysym.scancode == keyList[4]){
                        key_hold.erase(event.key.keysym.scancode);
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN){
                    if (loss){
                        if (contains(event.button.x,event.button.y,crd_try_again[0])){
                            clicking["try_again"] = 1;
                        }
                        else if (contains(event.button.x,event.button.y,crd_exit[0])){
                            clicking["exit"] = 1;
                        }
                    }
                    else if (!isPressed["pause"] && contains(event.button.x,event.button.y,crd_pause)){
                        clicking["pause"] = 1;
                    }
                    else if (isPressed["pause"]){
                        if (contains(event.button.x,event.button.y,crd_exit[1])){
                            clicking["exit"] = 1;
                        }
                        else if (contains(event.button.x,event.button.y,crd_cntinue)){
                            clicking["continue"] = 1;
                        }
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONUP){
                    if (clicking["try_again"] == 1){
                        clicking["try_again"] = 0;
                        isPressed["try_again"] = true;
                    }
                    if (clicking["exit"] == 1){
                        clicking["exit"] = 0;
                        isPressed["exit"] = true;
                    }
                    if (clicking["pause"] == 1){
                        clicking["pause"] = 0;
                        isPressed["pause"] = true;
                    }
                    if (clicking["continue"] == 1){
                        clicking["continue"] = 0;
                        isPressed["continue"] = true;
                    }
                }
            }
            //Kiểm tra phím đang được giữ
            if (!loss && !isPressed["pause"]){
                if ((keyState[keyList[0]] || keyState[keyList[3]]) && check_update.move_l(one.X,one.Y,one.index_type,one.board)){
                    SDL_Scancode key = (keyState[keyList[0]]? keyList[0] : keyList[3]);
                    if (SDL_GetTicks()-key_hold[key] >= hold_time){
                        update.move_l(one.X,one.Y,one.index_type,one.color_type,one.board,one.board_color);
                        key_hold[key] = SDL_GetTicks();
                    }
                }
                if ((keyState[keyList[2]] || keyState[keyList[5]]) && check_update.move_r(one.X,one.Y,one.index_type,one.board)){
                    SDL_Scancode key = (keyState[keyList[2]]? keyList[2] : keyList[5]);
                    if (SDL_GetTicks()-key_hold[key] >= hold_time){
                        update.move_r(one.X,one.Y,one.index_type,one.color_type,one.board,one.board_color);
                        key_hold[key] = SDL_GetTicks();
                    }
                }

                if ((keyState[keyList[1]] || keyState[keyList[4]]) && check_update.fall(one.X,one.Y,one.index_type,one.board)){
                    SDL_Scancode key = (keyState[keyList[1]]? keyList[1] : keyList[4]);
                    if (SDL_GetTicks()-key_hold[key] >= fall_levels[6]){
                        update.fall(one.X,one.Y,one.index_type,one.color_type,one.board,one.board_color);
                        key_hold[key] = SDL_GetTicks();
                    }
                }
                //Kiểm tra thời gian rơi
                if (SDL_GetTicks()-fall_time >= fall_levels[level]){
                    if (check_update.fall(one.X,one.Y,one.index_type,one.board)){
                        if (!(keyState[keyList[1]] || keyState[keyList[4]])){
                            update.fall(one.X,one.Y,one.index_type,one.color_type,one.board,one.board_color);
                        }
                    }
                    else{
                        if (data.loss(one.board)){
                            loss = true;
                        }
                        else{
                            one.reset();
                            if (data.check(one.board,one.row_full)){
                                int cnt_row = data.delete_row(one.X,one.Y,one.index_type,one.color_type,one.board,one.board_color,one.row_full);
                                score += scoring(cnt_row);
                                level = cal_level(score);
                            }
                        }
                    }
                    fall_time = SDL_GetTicks();
                }
            }

            //Xóa màn hình cũ
            SDL_RenderClear(renderer);

            //Vẽ màn hình mới
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
            SDL_RenderCopy(renderer, board, nullptr, &crd_board1);
            draw_block(one.board_color,crd_block1);

            SDL_RenderCopy(renderer, next1, nullptr, &crd_next1);
            draw_next_block(data,one.list_index.front(),one.list_color.front(),crd_next_block);

            SDL_RenderCopy(renderer,scoreboard1,nullptr,&crd_scoreboard1);

            SDL_RenderCopy(renderer,pause[clicking["pause"]],nullptr,&crd_pause);

            if (loss){
                SDL_RenderCopy(renderer,end1,nullptr,&crd_end1);
                SDL_RenderCopy(renderer,try_again[clicking["try_again"]],nullptr,&crd_try_again[0]);
                SDL_RenderCopy(renderer,exit[clicking["exit"]],nullptr,&crd_exit[0]);
                write_score(score,86,71,96);
                write_score(0,195,37,51);
            }
            else{
                write_score(score,195,37,51);
            }
            if (isPressed["pause"]){
                SDL_RenderCopy(renderer,menu,nullptr,&crd_menu);
                SDL_RenderCopy(renderer,cntinue[clicking["continue"]],nullptr,&crd_cntinue);
                SDL_RenderCopy(renderer,exit[clicking["exit"]],nullptr,&crd_exit[1]);
            }

            //Update màn hình mới
            SDL_RenderPresent(renderer);

            if (isPressed["try_again"]){
                one.create();
                score = 0;
                level = 0;
                isPressed["try_again"] = false;
                loss = false;
            }
            if (isPressed["exit"]){
                running = false;
                isPressed["exit"] = false;
                isPressed["pause"] = false;
                loss = false;
            }
            if (isPressed["continue"]){
                isPressed["pause"] = false;
                isPressed["continue"] = false;
            }

            frameTime = SDL_GetTicks()-frameStart;
            if (frameTime < frameDelay){
                SDL_Delay(frameDelay-frameTime);
            }
        }
    }

    void mode_two_players(Data data, Two two, Update update, Check_update check_update){
        //Khởi tạo game
        two.create();
        Uint32 fall_time1 = SDL_GetTicks();
        Uint32 fall_time2 = SDL_GetTicks();

        bool running = true;
        while (running){
            frameStart = SDL_GetTicks();
            SDL_Event event;
            while (SDL_PollEvent(&event)){
                // Kiểm tra sự kiện thoát
                if (event.type == SDL_QUIT){
                    running = false;
                    hard_exit = true;
                }
                // Kiểm tra phím nhấn
                else if (event.type == SDL_KEYDOWN){
                    if (find(begin(keyList),end(keyList),event.key.keysym.scancode) != end(keyList)){
                        key_hold[event.key.keysym.scancode] = SDL_GetTicks();
                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_W){
                        if (check_update.turn(two.X1,two.Y1,two.index_type1,two.board1)){
                            update.turn(two.X1,two.Y1,two.index_type1,two.color_type1,two.board1,two.board_color1);
                        }
                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_UP){
                        if (check_update.turn(two.X2,two.Y2,two.index_type2,two.board2)){
                            update.turn(two.X2,two.Y2,two.index_type2,two.color_type2,two.board2,two.board_color2);
                        }
                    }
                }
                //Kiểm tra phím thả
                else if (event.type == SDL_KEYUP){
                    if (event.key.keysym.scancode == keyList[0]){
                        if (!loss1 && check_update.move_l(two.X1,two.Y1,two.index_type1,two.board1) && SDL_GetTicks()-key_hold[event.key.keysym.scancode] < hold_time){
                            update.move_l(two.X1,two.Y1,two.index_type1,two.color_type1,two.board1,two.board_color1);
                        }
                        key_hold.erase(event.key.keysym.scancode);
                    }
                    else if (event.key.keysym.scancode == keyList[2]){
                        if (!loss1 && check_update.move_r(two.X1,two.Y1,two.index_type1,two.board1) && SDL_GetTicks()-key_hold[event.key.keysym.scancode] < hold_time){
                            update.move_r(two.X1,two.Y1,two.index_type1,two.color_type1,two.board1,two.board_color1);
                        }
                        key_hold.erase(event.key.keysym.scancode);
                    }
                    else if (event.key.keysym.scancode == keyList[1]){
                        key_hold.erase(event.key.keysym.scancode);
                    }
                    if (event.key.keysym.scancode == keyList[3]){
                        if (!loss2 && check_update.move_l(two.X2,two.Y2,two.index_type2,two.board2) && SDL_GetTicks()-key_hold[event.key.keysym.scancode] < hold_time){
                            update.move_l(two.X2,two.Y2,two.index_type2,two.color_type2,two.board2,two.board_color2);
                        }
                        key_hold.erase(event.key.keysym.scancode);
                    }
                    else if (event.key.keysym.scancode == keyList[5]){
                        if (!loss2 && check_update.move_r(two.X2,two.Y2,two.index_type2,two.board2) && SDL_GetTicks()-key_hold[event.key.keysym.scancode] < hold_time){
                            update.move_r(two.X2,two.Y2,two.index_type2,two.color_type2,two.board2,two.board_color2);
                        }
                        key_hold.erase(event.key.keysym.scancode);
                    }
                    else if (event.key.keysym.scancode == keyList[4]){
                        key_hold.erase(event.key.keysym.scancode);
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN){
                    if (loss){
                        if (contains(event.button.x,event.button.y,crd_try_again[1])){
                            clicking["try_again"] = 1;
                        }
                        else if (contains(event.button.x,event.button.y,crd_exit[2])){
                            clicking["exit"] = 1;
                        }
                    }
                    else if (!isPressed["pause"] && contains(event.button.x,event.button.y,crd_pause)){
                        clicking["pause"] = 1;
                    }
                    else if (isPressed["pause"]){
                        if (contains(event.button.x,event.button.y,crd_exit[1])){
                            clicking["exit"] = 1;
                        }
                        else if (contains(event.button.x,event.button.y,crd_cntinue)){
                            clicking["continue"] = 1;
                        }
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONUP){
                    if (clicking["try_again"] == 1){
                        clicking["try_again"] = 0;
                        isPressed["try_again"] = true;
                    }
                    if (clicking["exit"] == 1){
                        clicking["exit"] = 0;
                        isPressed["exit"] = true;
                    }
                    if (clicking["pause"] == 1){
                        clicking["pause"] = 0;
                        isPressed["pause"] = true;
                    }
                    if (clicking["continue"] == 1){
                        clicking["continue"] = 0;
                        isPressed["continue"] = true;
                    }
                }
            }
            //Kiểm tra phím đang được giữ
            if (!loss && !isPressed["pause"]){
                if (!loss1){
                    if ((keyState[keyList[0]]) && check_update.move_l(two.X1,two.Y1,two.index_type1,two.board1)){
                        if (SDL_GetTicks()-key_hold[keyList[0]] >= hold_time){
                            update.move_l(two.X1,two.Y1,two.index_type1,two.color_type1,two.board1,two.board_color1);
                            key_hold[keyList[0]] = SDL_GetTicks();
                        }
                    }
                    if ((keyState[keyList[2]]) && check_update.move_r(two.X1,two.Y1,two.index_type1,two.board1)){
                        if (SDL_GetTicks()-key_hold[keyList[2]] >= hold_time){
                            update.move_r(two.X1,two.Y1,two.index_type1,two.color_type1,two.board1,two.board_color1);
                            key_hold[keyList[2]] = SDL_GetTicks();
                        }
                    }
                    if ((keyState[keyList[1]]) && check_update.fall(two.X1,two.Y1,two.index_type1,two.board1)){
                        if (SDL_GetTicks()-key_hold[keyList[1]] >= fall_levels[6]){
                            update.fall(two.X1,two.Y1,two.index_type1,two.color_type1,two.board1,two.board_color1);
                            key_hold[keyList[1]] = SDL_GetTicks();
                        }
                    }
                    //Kiểm tra thời gian rơi
                    if (SDL_GetTicks()-fall_time1 >= fall_levels[level1]){
                        if (check_update.fall(two.X1,two.Y1,two.index_type1,two.board1)){
                            if (!keyState[keyList[1]]){
                                update.fall(two.X1,two.Y1,two.index_type1,two.color_type1,two.board1,two.board_color1);
                            }
                        }
                        else{
                            if (data.loss(two.board1)){
                                loss1 = true;
                            }
                            else{
                                two.reset1();
                                if (data.check(two.board1,two.row_full1)){
                                    int cnt_row = data.delete_row(two.X1,two.Y1,two.index_type1,two.color_type1,two.board1,two.board_color1,two.row_full1);
                                    score1 += scoring(cnt_row);
                                    level1 = cal_level(score1);
                                }
                            }
                        }
                        fall_time1 = SDL_GetTicks();
                    }
                }
                if (!loss2){
                    if ((keyState[keyList[3]]) && check_update.move_l(two.X2,two.Y2,two.index_type2,two.board2)){
                        if (SDL_GetTicks()-key_hold[keyList[3]] >= hold_time){
                            update.move_l(two.X2,two.Y2,two.index_type2,two.color_type2,two.board2,two.board_color2);
                            key_hold[keyList[3]] = SDL_GetTicks();
                        }
                    }
                    if ((keyState[keyList[5]]) && check_update.move_r(two.X2,two.Y2,two.index_type2,two.board2)){
                        if (SDL_GetTicks()-key_hold[keyList[5]] >= hold_time){
                            update.move_r(two.X2,two.Y2,two.index_type2,two.color_type2,two.board2,two.board_color2);
                            key_hold[keyList[5]] = SDL_GetTicks();
                        }
                    }
                    if ((keyState[keyList[4]]) && check_update.fall(two.X2,two.Y2,two.index_type2,two.board2)){
                        if (SDL_GetTicks()-key_hold[keyList[4]] >= fall_levels[6]){
                            update.fall(two.X2,two.Y2,two.index_type2,two.color_type2,two.board2,two.board_color2);
                            key_hold[keyList[4]] = SDL_GetTicks();
                        }
                    }
                    //Kiểm tra thời gian rơi
                    if (SDL_GetTicks()-fall_time2 >= fall_levels[level2]){
                        if (check_update.fall(two.X2,two.Y2,two.index_type2,two.board2)){
                            if (!keyState[keyList[4]]){
                                update.fall(two.X2,two.Y2,two.index_type2,two.color_type2,two.board2,two.board_color2);
                            }
                        }
                        else{
                            if (data.loss(two.board2)){
                                loss2 = true;
                            }
                            else{
                                two.reset2();
                                if (data.check(two.board2,two.row_full2)){
                                    int cnt_row = data.delete_row(two.X2,two.Y2,two.index_type2,two.color_type2,two.board2,two.board_color2,two.row_full2);
                                    score2 += scoring(cnt_row);
                                    level2 = cal_level(score2);
                                }
                            }
                        }
                        fall_time2 = SDL_GetTicks();
                    }
                }
                if (loss1 && loss2){
                    loss = true;
                    if (score1 > score2) cnt_win[1]++;
                    else if (score2 > score1) cnt_win[2]++;
                }
            }

            //Xóa màn hình cũ
            SDL_RenderClear(renderer);

            //Vẽ màn hình mới
            SDL_RenderCopy(renderer,backgroundTexture,nullptr,nullptr);
            SDL_RenderCopy(renderer,board,nullptr, &crd_board2);
            SDL_RenderCopy(renderer,board,nullptr,&crd_board3);
            draw_block(two.board_color1,crd_block2);
            draw_block(two.board_color2,crd_block3);

            SDL_RenderCopy(renderer,next2,nullptr,&crd_next2);
            SDL_RenderCopy(renderer,scoreboard2,nullptr,&crd_scoreboard2);
            SDL_RenderCopy(renderer,win2,nullptr,&crd_win2);

            draw_next_block(data,two.list_index1.front(),two.list_color1.front(),crd_next_block1);
            draw_next_block(data,two.list_index2.front(),two.list_color2.front(),crd_next_block2);

            write_score(score1,99,37,35);
            write_score(score2,134,37,34);
            write_score(cnt_win[1],103,116,31);
            write_score(cnt_win[2],134,116,30);

            SDL_RenderCopy(renderer,pause[clicking["pause"]],nullptr,&crd_pause);
            if (loss){
                SDL_RenderCopy(renderer,end2,nullptr,&crd_end2);
                SDL_RenderCopy(renderer,try_again[clicking["try_again"]],nullptr,&crd_try_again[1]);
                SDL_RenderCopy(renderer,exit[clicking["exit"]],nullptr,&crd_exit[2]);
                write_score(score1,87,44,58);
                write_score(score2,146,44,58);
                write_score(cnt_win[1],87,64,58);
                write_score(cnt_win[2],146,64,58);
            }
            if (isPressed["pause"]){
                SDL_RenderCopy(renderer,menu,nullptr,&crd_menu);
                SDL_RenderCopy(renderer,cntinue[clicking["continue"]],nullptr,&crd_cntinue);
                SDL_RenderCopy(renderer,exit[clicking["exit"]],nullptr,&crd_exit[1]);
            }
            //Update màn hình mới
            SDL_RenderPresent(renderer);

            if (isPressed["try_again"]){
                two.create();
                score1 = score2 = 0;
                level1 = level2 = 0;
                isPressed["try_again"] = false;
                loss = loss1 = loss2 = false;
            }
            if (isPressed["exit"]){
                running = false;
                isPressed["exit"] = false;
                isPressed["pause"] = false;
                loss = loss1 = loss2 = false;
                cnt_win[1] = cnt_win[2] = 0;
            }
            if (isPressed["continue"]){
                isPressed["pause"] = false;
                isPressed["continue"] = false;
            }

            frameTime = SDL_GetTicks()-frameStart;
            if (frameTime < frameDelay){
                SDL_Delay(frameDelay-frameTime);
            }
        }
    }
    //Màn hình chính
    int home(){
        bool running = true;
        while (running){
            SDL_Event event;
            while (SDL_PollEvent(&event)){
                if (event.type == SDL_QUIT){
                    running = false;
                    hard_exit = true;
                    return 3;
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN){
                    if (!isPressed["settings"] && !isPressed["play"] && contains(event.button.x,event.button.y,crd_play)){
                        clicking["play"] = 1;
                    }
                    else if (!isPressed["play"] && !isPressed["settings"] && contains(event.button.x,event.button.y,crd_settings)){
                        clicking["settings"] = 1;
                    }
                    else if (!contains(event.button.x,event.button.y,crd_menu)){
                        isPressed["play"] = false;
                        isPressed["settings"] = false;
                    }
                    else if (isPressed["play"]){
                        if (contains(event.button.x,event.button.y,crd_onePlayer)){
                            clicking["onePlayer"] = 1;
                        }
                        else if (contains(event.button.x,event.button.y,crd_twoPlayers)){
                            clicking["twoPlayers"] = 1;
                        }
                    }
                    else if (isPressed["settings"]){
                        if (contains(event.button.x,event.button.y,crd_exit[3])){
                            clicking["exit"] = 1;
                        }
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONUP){
                    if (clicking["play"] == 1){
                        clicking["play"] = 0;
                        isPressed["play"] = true;
                    }
                    if (clicking["settings"] == 1){
                        clicking["settings"] = 0;
                        isPressed["settings"] = true;
                    }
                    if (clicking["onePlayer"] == 1){
                        clicking["onePlayer"] = 0;
                        isPressed["onePlayer"] = true;
                    }
                    if (clicking["twoPlayers"] == 1){
                        clicking["twoPlayers"] = 0;
                        isPressed["twoPlayers"] = true;
                    }
                    if (clicking["exit"] == 1){
                        clicking["exit"] = 0;
                        isPressed["exit"] = true;
                    }
                }
            }

            //Xóa màn hình cũ
            SDL_RenderClear(renderer);

            //Vẽ màn hình mới
            SDL_RenderCopy(renderer,backgroundTexture,nullptr,nullptr);
            SDL_RenderCopy(renderer,name,nullptr,&crd_name);

            SDL_RenderCopy(renderer,play[clicking["play"]],nullptr,&crd_play);
            SDL_RenderCopy(renderer,settings[clicking["settings"]],nullptr,&crd_settings);

            if (isPressed["play"]){
                SDL_RenderCopy(renderer,selectMode,nullptr,&crd_selectMode);
                SDL_RenderCopy(renderer,onePlayer[clicking["onePlayer"]],nullptr,&crd_onePlayer);
                SDL_RenderCopy(renderer,twoPlayers[clicking["twoPlayers"]],nullptr,&crd_twoPlayers);
            }

            if (isPressed["settings"]){
                SDL_RenderCopy(renderer,menu,nullptr,&crd_menu);
                SDL_RenderCopy(renderer,exit[clicking["exit"]],nullptr,&crd_exit[3]);
            }

            //Update màn hình mới
            SDL_RenderPresent(renderer);

            if (isPressed["exit"]){
                running = false;
                isPressed["exit"] = false;
                isPressed["settings"] = false;
                return 3;
            }
            if (isPressed["onePlayer"]){
                running = false;
                isPressed["onePlayer"] = false;
                isPressed["play"] = false;
                return 1;
            }
            if (isPressed["twoPlayers"]){
                running = false;
                isPressed["twoPlayers"] = false;
                isPressed["play"] = false;
                return 2;
            }

            frameTime = SDL_GetTicks()-frameStart;
            if (frameTime < frameDelay){
                SDL_Delay(frameDelay-frameTime);
            }
        }
    }

    //Kiểm tra thoát hẳn
    bool get_exit(){
        return hard_exit;
    }

    //Thoát SDL
    ~Interaction(){
        //Xóa các các ảnh
        for (int i = 0; i < 6; i++){
            SDL_DestroyTexture(block[i]);
        }
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyTexture(next1);
        SDL_DestroyTexture(next2);
        SDL_DestroyTexture(scoreboard1);
        SDL_DestroyTexture(scoreboard2);
        SDL_DestroyTexture(win2);
        for (int i = 0; i < 10; i++){
            SDL_DestroyTexture(number[i]);
        }
        SDL_DestroyTexture(end1);
        SDL_DestroyTexture(end2);
        SDL_DestroyTexture(menu);
        SDL_DestroyTexture(selectMode);
        SDL_DestroyTexture(name);
        SDL_DestroyTexture(pause[0]);
        SDL_DestroyTexture(pause[1]);
        SDL_DestroyTexture(try_again[0]);
        SDL_DestroyTexture(try_again[1]);
        SDL_DestroyTexture(exit[0]);
        SDL_DestroyTexture(exit[1]);
        SDL_DestroyTexture(cntinue[0]);
        SDL_DestroyTexture(cntinue[1]);
        SDL_DestroyTexture(play[0]);
        SDL_DestroyTexture(play[1]);
        SDL_DestroyTexture(settings[0]);
        SDL_DestroyTexture(settings[1]);
        SDL_DestroyTexture(onePlayer[0]);
        SDL_DestroyTexture(onePlayer[1]);
        SDL_DestroyTexture(twoPlayers[0]);
        SDL_DestroyTexture(twoPlayers[1]);

        //Xóa cửa sổ màn hình
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
    }
};

int main(int argc, char* argv[]){
    Interaction interaction;
    Data data;
    One one;
    Two two;
    Update update;
    Check_update check_update;

    bool running = true;
    while (running){
        int mode = interaction.home();
        if (mode == 1){
            interaction.mode_one_player(data,one,update,check_update);
        }
        else if (mode == 2){
            interaction.mode_two_players(data,two,update,check_update);
        }
        if (interaction.get_exit() || mode == 3){
            running = false;
        }
    }

    return 0;
}
