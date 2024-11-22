#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <random>
#include <algorithm>
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
    void fill_block(int x, int y, int t, int t_c, int (&board)[24][14], int (&board_color)[24][12]){
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
    bool check(int board[24][14], queue<int> &row_full){
        for (int i = 22; i > 2; i--){
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
    void delete_oblock(int x, int y, int t, int (&board)[24][14], int (&board_color)[24][12]){
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
    void delete_row(int x, int y, int t, int t_c, int (&board)[24][14], int (&board_color)[24][12], queue<int> &q){
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
    }
    bool loss(int board[24][14]){
        for (int i = 1; i < 11; i++){
            if (board[2][i]){
                return true;
            }
        }
        return false;
    }
};

class One : public Data{
public:
    int board[24][14], board_color[24][12] = {0};
    int X, Y;
    int index_type;
    int color_type;
    queue<int> row_full;
    queue<int> list_index;
    queue<int> list_color;

    void create(){
        for (int i = 0; i < 23; i++){
            board[i][0] = board[i][11] = board[i][12] = board[i][13] = 1;
            for (int j = 1; j < 11; j++){
                board[i][j] = 0;
                board_color[i][j] = 0;
            }
        }
        for (int i = 0; i < 14; i++) board[23][i] = 1;
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
    int board1[24][14], board2[24][14];
    int board_color1[24][12] = {0};
    int board_color2[24][12] = {0};
    int X1, Y1, X2, Y2;
    int index_type1, index_type2;
    int color_type1, color_type2;
    queue<int> list_index1, list_index2;
    queue<int> list_color1, list_color2;
    queue<int> row_full1, row_full2;
    
    void create(){
        for (int i = 0; i < 23; i++){
            board1[i][0] = board1[i][11] = board1[i][12] = board1[i][13] = 1;
            board2[i][0] = board2[i][11] = board2[i][12] = board2[i][13] = 1;
            for (int j = 1; j < 11; j++) board1[i][j] = board2[i][j] = 0;
        }
        for (int i = 0; i < 14; i++) board1[23][i] = board2[23][i] = 1;
        X1 = X2 = 3;
        Y1 = Y2 = 5;
        int temp = rd_block();
        list_index1.push(temp);
        list_index2.push(temp);
        index_type1 = index_type2 = temp;
        temp = rd_color();
        list_color1.push(temp);
        list_color2.push(temp);
        color_type1 = color_type2 = temp;
    }

    void push(int t, int t_c){
        list_index1.push(t);
        list_index2.push(t);
        list_color1.push(t_c);
        list_color2.push(t_c);
    }
    void reset1(){
        X1 = 3;
        Y1 = 5;
        list_index1.pop();
        list_color1.pop();
        if (list_index1.empty()){
            index_type1 = rd_block();
            color_type1 = rd_color();
            push(index_type1,color_type1);
        }
        else{
            index_type1 = list_index1.front();
            color_type1 = list_color1.front();
            push(rd_block(),rd_color());
        }
        fill_block(X1,Y1,index_type1,color_type1,board1,board_color1);
    }

    void reset2(){
        X2 = 3;
        Y2 = 5;
        list_index2.pop();
        list_color2.pop();
        if (list_index2.empty()){
            index_type2 = rd_block();
            color_type2 = rd_color();
            push(index_type2,color_type2);
        }
        else{
            index_type2 = list_index2.front();
            color_type2 = list_color2.front();
            push(rd_block(),rd_color());
        }
        fill_block(X2,Y2,index_type2,color_type2,board2,board_color2);
    }
};

class Update : public Data{
public:
    void turn(int &x, int &y, int &t, int t_c, int (&board)[24][14], int (&board_color)[24][12]){
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
    void fall(int &x, int &y, int t, int t_c, int (&board)[24][14], int (&board_color)[24][12]){
        delete_oblock(x,y,t,board,board_color);
        x++;
        fill_block(x,y,t,t_c,board,board_color);
    }
    void move_l(int &x, int &y, int t, int t_c, int (&board)[24][14], int (&board_color)[24][12]){
        delete_oblock(x,y,t,board,board_color);
        y--;
        fill_block(x,y,t,t_c,board,board_color);
    }
    void move_r(int &x, int &y, int t, int t_c, int (&board)[24][14], int (&board_color)[24][12]){
        delete_oblock(x,y,t,board,board_color);
        y++;
        fill_block(x,y,t,t_c,board,board_color);
    }
};

class Check_update : public Data{
public:
    bool turn(int x, int y, int t, int board[24][14]){
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
    bool move_l(int x, int y, int t, int board[24][14]){
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
    bool move_r(int x, int y, int t, int board[24][14]){
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
            for (int i = x-3; i < x+1; i++){
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
    bool fall(int x, int y, int t, int board[24][14]){
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

    SDL_Texture* score1;
    SDL_Texture* score2;
    //Bảng tỉ số mode 2 player
    SDL_Texture* win2;
    //Pause game
    SDL_Texture* pause;

    int w_board, h_board;
    int w_block, h_block;

    int w_next1, h_next1;
    int w_next2, h_next2;
    int w_score1, h_score1;
    int w_score2, h_score2;

    int w_win2, h_win2;

    int w_pause, h_pause;

    //Tọa độ các bảng và các khối
    SDL_Rect crd_board1;
    SDL_Rect crd_board2;
    SDL_Rect crd_board3;
    SDL_Rect crd_block1[24][12];
    SDL_Rect crd_block2[24][12];
    SDL_Rect crd_block3[24][12];

    SDL_Rect crd_next1;
    SDL_Rect crd_next2;

    SDL_Rect crd_score1;
    SDL_Rect crd_score2;

    SDL_Rect crd_win2;
    //Tọa độ khối next
    SDL_Rect crd_next_block[3][4][4];
    SDL_Rect crd_next_block1[3][4][4];
    SDL_Rect crd_next_block2[3][4][4];

    //Tọa độ nút pause
    SDL_Rect crd_pause;

    //Lưu thời gian nhấn key
    map<SDL_Scancode, Uint32> key_hold;

    //Thời gian rơi và thời gian nhấn giữ key
    Uint32 hold_time = 100;
    Uint32 fall_levels[7] = {1000,800,500,200,100,50,20};
    int level = 0;

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

        score1 = loadTexture("graphic/gameplay1/score1.png");

        pause = loadTexture("graphic/menu/pause.png");

        //Kích thước bảng, khối
        h_board = displayMode.h*148/150;
        w_board = h_board*78/148;
        w_block = h_block = h_board*7/148;
        //Kích thước bảng next block
        w_next1 = h_next1 = displayMode.h*51/150;

        //Kích thước bảng score
        w_score1 = displayMode.h*51/150;
        h_score1 = displayMode.h*29/150;

        //Kích thước nút pause
        w_pause = displayMode.h*12/150;
        h_pause = displayMode.h*11/150;

        //Tọa độ, thuộc tính của bảng, khối
        crd_board1 = {
            (displayMode.w-w_board)/2,
            (displayMode.h-h_board)/2,
            w_board,
            h_board
        };
        crd_board2 = {
            (displayMode.w/2-w_board)/2,
            (displayMode.h-h_board)/2,
            w_board,
            h_board
        };
        crd_board3 = {
            (displayMode.w*3/2-w_board)/2,
            (displayMode.h-h_board)/2,
            w_board,
            h_board
        };

        for (int i = 3; i < 23; i++){
            for (int j = 1; j < 11; j++){
                int x = i-2;
                crd_block1[i][j] = {
                    displayMode.w/2+w_block*(-5+j-1),
                    displayMode.h/2+h_block*(-10+x-1),
                    w_block,
                    h_block
                };
                crd_block2[i][j] = {
                    displayMode.w/4+w_block*(-5+j-1),
                    displayMode.h/2+h_block*(-10+x-1),
                    w_block,
                    h_block
                };
                crd_block3[i][j] = {
                    displayMode.w*3/4+w_block*(-5+j-1),
                    displayMode.h/2+h_block*(-10+x-1),
                    w_block,
                    h_block
                };
            }
        }
        //Tọa độ bảng next block và khối next block
        crd_next1 = {
            displayMode.h*23/150,
            displayMode.h*78/150,
            w_next1,
            h_next1
        };
        for (int i = 0; i < 4; i++){//Tọa độ khối next của 3 nhóm block để căn giữa từng loại
            for (int j = 0; j < 4; j++){
                crd_next_block[0][i][j] = {
                    displayMode.h*31/150+w_block*j,
                    displayMode.h*88/150+h_block*i,
                    w_block,
                    h_block
                };
                crd_next_block[1][i][j] = {
                    displayMode.h*38/150+w_block*j,
                    displayMode.h*92/150+h_block*i,
                    w_block,
                    h_block
                };
                crd_next_block[2][i][j] = {
                    displayMode.h*35/150+w_block*j,
                    displayMode.h*88/150+h_block*i,
                    w_block,
                    h_block                    
                };
            }
        }

        //Tọa độ bảng score
        crd_score1 = {
            displayMode.w*216/300,
            displayMode.h*22/150,
            w_score1,
            h_score1
        };

        //Tọa độ nút pause
        crd_pause = {
            displayMode.w/150,
            displayMode.w/150,
            w_pause,
            h_pause
        };
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
    //Vẽ các khối ra màn hình
    void draw_block(int board_color[24][12], SDL_Rect crd_block[24][12]){
        for (int i = 3; i < 23; i++){
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
    //Mode 1 player
    void mode_one_player(Data data, One one, Update update, Check_update check_update){
        //Khởi tạo game
        one.create();
        Uint32 fall_time = SDL_GetTicks();

        bool running = true;
        while (running) {
            Uint32 frameStart = SDL_GetTicks();
            SDL_Event event;
            while (SDL_PollEvent(&event)){
                // Kiểm tra sự kiện thoát
                if (event.type == SDL_QUIT){
                    running = false;
                }
                // Kiểm tra phím nhấn
                else if (event.type == SDL_KEYDOWN){
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                        running = false;
                    }
                    else if (find(begin(keyList),end(keyList),event.key.keysym.scancode) != end(keyList)){
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
            }
            //Kiểm tra phím đang được giữ
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
                        one.create();
                    }
                    else{
                        one.reset();
                        if (data.check(one.board,one.row_full)){
                            data.delete_row(one.X,one.Y,one.index_type,one.color_type,one.board,one.board_color,one.row_full);
                        }
                    }
                }
                fall_time = SDL_GetTicks();
            }

            //Xóa màn hình cũ
            SDL_RenderClear(renderer);

            //Vẽ màn hình mới
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
            SDL_RenderCopy(renderer, board, nullptr, &crd_board1);
            draw_block(one.board_color,crd_block1);

            SDL_RenderCopy(renderer, next1, nullptr, &crd_next1);
            draw_next_block(data,one.list_index.front(),one.list_color.front(),crd_next_block);

            SDL_RenderCopy(renderer,score1,nullptr,&crd_score1);

            SDL_RenderCopy(renderer,pause,nullptr,&crd_pause);

            //Update màn hình mới
            SDL_RenderPresent(renderer);

            SDL_Delay(8); //120fps
        }
    }
    //Thoát SDL
    ~Interaction(){
        //Xóa các block
        for (int i = 0; i < 6; i++){
            SDL_DestroyTexture(block[i]);
        }
        //Xóa background
        SDL_DestroyTexture(backgroundTexture);

        //Xóa các ảnh khác
        SDL_DestroyTexture(next1);
        SDL_DestroyTexture(next2);

        SDL_DestroyTexture(score1);
        SDL_DestroyTexture(score2);

        SDL_DestroyTexture(win2);

        SDL_DestroyTexture(pause);

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

    interaction.mode_one_player(data,one,update,check_update);
    return 0;
}
