#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <sstream>
#include <string>

//-1:B
//-2:L
//-3:X
//-4:T
//-5:Q
//-6:K
//-7:Z not moved towerT
//-8:Y not moved kingK
//-9:F en passant B (just moved 2 forw)

//1:b
//2:l
//3:x
//4:t
//5:q
//6:k
//7:z not moved towert
//8:y not moved kingk
//9:f en passant b (just moved 2 forw)

//

bool is_int(int value) {
    std::string input = std::to_string(value);
    std::istringstream iss(input);
    int extracted_value;
    return (iss >> extracted_value) && (iss.eof());
}

int generate_random_int(int min, int max) {
    int random_number=min+rand()%(max-min+1);
    return random_number;
}

std::vector<std::vector<int>> deepcopy(const std::vector<std::vector<int>>& board) {
    std::vector<std::vector<int>> board_copy;
    for (const std::vector<int>& row : board){
        std::vector<int> new_y;
        for (const int& x : row){
            new_y.push_back(x);
        }
        board_copy.push_back(new_y);
    }
    return board_copy;
}

bool verloren(const std::vector<std::vector<int>>& board, int playerk) {
    int player_2 = (playerk == 6) ? 8 : -8;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] == playerk || board[i][j] == player_2) {
                return false;
            }
        }
    }
    return true;
}

std::vector<std::vector<int>> matrix_minus(std::vector<std::vector<int>> matrix) {
    for (int i=0; i<matrix.size(); ++i) {
        for (int j=0; j<matrix[i].size(); ++j) {
            matrix[i][j]=-matrix[i][j];
        }
    }
    return matrix;
}

//

void print_board(const std::vector<std::vector<int>>& board) {
    std::cout <<"    1   2   3   4   5   6   7   8\n";
    std::cout <<"  ---------------------------------\n";
    for (int i=0; i<8; ++i) {
        std::cout << i+1 <<" I";
        for (int j=0; j<8; ++j) {
            std::cout << " ";
            if (board[i][j]==1) {std::cout << 'b';}
            else if (board[i][j]==9) {std::cout <<'f';}
            else if (board[i][j]==2) {std::cout <<'l';}
            else if (board[i][j]==3) {std::cout <<'x';}
            else if (board[i][j]==4) {std::cout <<'t';}
            else if (board[i][j]==7) {std::cout <<'z';}
            else if (board[i][j]==5) {std::cout <<'q';}
            else if (board[i][j]==6) {std::cout <<'k';}
            else if (board[i][j]==8) {std::cout <<'y';}
            else if (board[i][j]==-1) {std::cout <<'B';}
            else if (board[i][j]==-9) {std::cout <<'F';}
            else if (board[i][j]==-2) {std::cout <<'L';}
            else if (board[i][j]==-3) {std::cout <<'X';}
            else if (board[i][j]==-4) {std::cout <<'T';}
            else if (board[i][j]==-7) {std::cout <<'Z';}
            else if (board[i][j]==-5) {std::cout <<'Q';}
            else if (board[i][j]==-6) {std::cout <<'K';}
            else if (board[i][j]==-8) {std::cout <<'Y';}
            else if (board[i][j]==0) {std::cout <<' ';}
            std::cout << " ";
            std::cout << "I";
        }
        std::cout <<'\n';
        std::cout <<"  ---------------------------------\n";
    }
}

std::vector<std::vector<std::vector<int>>> generate_children(std::vector<std::vector<int>> position, int playerk);//declaration weil gcKk gc in def hat und gc gcKk in def hat.

std::vector<std::vector<std::vector<int>>> gcKk(int y, int x, const std::vector<std::vector<int>> pos, int player) {
    std::vector<std::vector<int>> boardc = pos;
    std::vector<std::vector<std::vector<int>>> childrenK;

    int direction_y[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int direction_x[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i=0; i<8; ++i) {
        int new_y = y + direction_y[i];
        int new_x = x + direction_x[i];

        if (new_y>-1 && new_y<8 && new_x>-1 && new_x<8) {
            if ((player==-6 || player==-8) && boardc[new_y][new_x]>=0) {
                boardc[y][x]=0;
                boardc[new_y][new_x]=-6;
                childrenK.push_back(boardc);
                boardc=pos;
            }
            else if ((player==6 || player==8) && boardc[new_y][new_x]<=0) {
                boardc[y][x]=0;
                boardc[new_y][new_x]=6;
                childrenK.push_back(boardc);
                boardc=pos;
            }
        }
    }
    //Rochade
    if (player==-8) {
        if (boardc[0][0]==-7 && boardc[0][1]==0 && boardc[0][2]==0 && boardc[0][3]==0) {
            boardc[0][2]=-6;
            boardc[0][3]=-4;
            boardc[0][0]=0;
            boardc[0][4]=0;
            bool legal = true;
            for (auto child : generate_children(boardc, 6)) {
                if (child[0][2]>0 || child[0][3]>0 || child[0][4]>0) {legal = false; break;}
            }
            if (legal) {childrenK.push_back(boardc);}
            boardc = pos;
        }
        if (boardc[0][7]==-7 && boardc[0][6]==0 && boardc[0][5]==0) {
            boardc[0][6]=-6;
            boardc[0][5]=-4;
            boardc[0][7]=0;
            boardc[0][4]=0;
            bool legal = true;
            for (auto child : generate_children(boardc, 6)) {
                if (child[0][4]>0 || child[0][5]>0 || child[0][6]>0) {legal = false; break;}
            }
            if (legal) {childrenK.push_back(boardc);}
            boardc = pos;
        }
    }
    else if (player==8) {
        if (boardc[7][0]==7 && boardc[7][1]==0 && boardc[7][2]==0 && boardc[7][3]==0) {
            boardc[7][2]=6;
            boardc[7][3]=4;
            boardc[7][0]=0;
            boardc[7][4]=0;
            bool legal = true;
            for (auto child : generate_children(boardc, -6)) {
                if (child[7][2]<0 || child[7][3]<0 || child[7][4]<0) {legal = false; break;}
            }
            if (legal) {childrenK.push_back(boardc);}
            boardc = pos;
        }
        if (boardc[7][7]==7 && boardc[7][6]==0 && boardc[7][5]==0) {
            boardc[7][6]=6;
            boardc[7][5]=4;
            boardc[7][7]=0;
            boardc[7][4]=0;
            bool legal = true;
            for (auto child : generate_children(boardc, -6)) {
                if (child[7][4]<0 || child[7][5]<0 || child[7][6]<0) {legal = false; break;}
            }
            if (legal) {childrenK.push_back(boardc);}
            boardc = pos;
        }
    }
    //
    return childrenK;
}

std::vector<std::vector<std::vector<int>>> gcLl(int y, int x, const std::vector<std::vector<int>> pos, int player) {
    std::vector<std::vector<int>> boardc = pos;
    std::vector<std::vector<std::vector<int>>> childrenL;
    //
    int direction_y[] = {-2, -2, 2, 2, 1, -1, 1, -1};
    int direction_x[] = {1, -1, 1, -1, 2, 2, -2, -2};
    //
    if (player==-2) {
        for (int i=0; i<8; ++i) {
            int new_y= y+direction_y[i];
            int new_x= x+direction_x[i];
            if (new_y>-1 && new_y<8 && new_x>-1 && new_x<8 && pos[new_y][new_x]>=0) {
                boardc=pos;
                boardc[y][x]=0;
                boardc[new_y][new_x]=-2;
                childrenL.push_back(boardc);
            }
        }
    }
    else if (player==2) {
        for (int i=0; i<8; ++i) {
            int new_y= y+direction_y[i];
            int new_x= x+direction_x[i];
            if (new_y>-1 && new_y<8 && new_x>-1 && new_x<8 && pos[new_y][new_x]<=0) {
                boardc=pos;
                boardc[y][x]=0;
                boardc[new_y][new_x]=2;
                childrenL.push_back(boardc);
            }
        }
    }
    //
    return childrenL;
}

std::vector<std::vector<std::vector<int>>> gcBb(int y, int x, const std::vector<std::vector<int>> pos, int player) {
    std::vector<std::vector<int>> boardc = pos;
    std::vector<std::vector<std::vector<int>>> childrenB;
    //
    if (player==-1) {
        if (y==1 && boardc[y+2][x]==0 && boardc[y+1][x]==0) {
            boardc[y][x]=0;
            boardc[y+2][x]=-9;
            childrenB.push_back(boardc);
            boardc=pos;
        }
        if (y+1<8) {
            if (boardc[y+1][x]==0) {
                boardc[y][x]=0;
                boardc[y+1][x]=-1;
                if (y+1==7) {boardc[y+1][x]=-5;}
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        if (x-1>-1 && y+1<8) {
            if (boardc[y+1][x-1]>0) {
                boardc[y][x]=0;
                boardc[y+1][x-1]=-1;
                if (y+1==7) {boardc[y+1][x-1]=-5;}
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        if (x+1<8 && y+1<8) {
            if (boardc[y+1][x+1]>0) {
                boardc[y][x]=0;
                boardc[y+1][x+1]=-1;
                if (y+1==7) {boardc[y+1][x+1]=-5;}
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        //en passant
        if (x-1>-1 && y+1<8) {
            if (boardc[y][x-1]==9 && boardc[y+1][x-1]==0) {
                boardc[y][x]=0;
                boardc[y+1][x-1]=-1;
                boardc[y][x-1]=0;
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        if (x+1<8 && y+1<8) {
            if (boardc[y][x+1]==9 && boardc[y+1][x+1]==0) {
                boardc[y][x]=0;
                boardc[y+1][x+1]=-1;
                boardc[y][x+1]=0;
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
    }
    else if (player==1) {
        if (y==6 && boardc[y-2][x]==0 && boardc[y-1][x]==0) {
            boardc[y][x]=0;
            boardc[y-2][x]=9;
            childrenB.push_back(boardc);
            boardc=pos;
        }
        if (y-1>-1) {
            if (boardc[y-1][x]==0) {
                boardc[y][x]=0;
                boardc[y-1][x]=1;
                if (y-1==0) {boardc[y-1][x]=5;}
                childrenB.push_back(boardc);
                boardc = pos;
            }
        }
        if (x-1>-1 && y-1>-1) {
            if (boardc[y-1][x-1]<0) {
                boardc[y][x]=0;
                boardc[y-1][x-1]=1;
                if (y-1==0) {boardc[y-1][x-1]=5;}
                childrenB.push_back(boardc);
                boardc = pos;
            }
        }
        if (x+1<8 && y-1>-1) {
            if (boardc[y-1][x+1]<0) {
                boardc[y][x]=0;
                boardc[y-1][x+1]=1;
                if (y-1==0) {boardc[y-1][x+1]=5;}
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        //en passant
        if (x-1>-1 && y-1>-1) {
            if (boardc[y][x-1]==-9 && boardc[y-1][x-1]==0) {
                boardc[y][x]=0;
                boardc[y-1][x-1]=1;
                boardc[y][x-1]=0;
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
        if (x+1<8 && y-1>-1) {
            if (boardc[y][x+1]==-9 && boardc[y-1][x+1]==0) {
                boardc[y][x]=0;
                boardc[y-1][x+1]=1;
                boardc[y][x+1]=0;
                childrenB.push_back(boardc);
                boardc=pos;
            }
        }
    }
    return childrenB;
}

std::vector<std::vector<std::vector<int>>> gcTtXxQq(int y, int x, const std::vector<std::vector<int>> pos, int player, std::vector<int> direction_y, std::vector<int> direction_x) {
    std::vector<std::vector<std::vector<int>>> children;
    std::vector<std::vector<int>> boardc;
    //
    if (player<0) {
        for (int dir=0; dir<direction_y.size(); ++dir) {
            for (int step=1; step<8; ++step) {
                int new_y = y+step * direction_y[dir];
                int new_x = x+step * direction_x[dir];
                //
                if (new_y>-1 && new_y<8 && new_x>-1 && new_x<8) {
                    if (pos[new_y][new_x]>=0) {
                        boardc=pos;
                        boardc[y][x]=0;
                        boardc[new_y][new_x]=player;
                        children.push_back(boardc);
                        if (pos[new_y][new_x]!=0) {break;}
                    }
                    else {break;}
                }
                else {break;}
            }
        }
    }
    else if (player>0) {
        for (int dir=0; dir<direction_y.size(); ++dir) {
            for (int step = 1; step <8; ++step) {
                int new_y = y + step * direction_y[dir];
                int new_x = x + step * direction_x[dir];
                //
                if (new_y>-1 && new_y<8 && new_x>-1 && new_x<8) {
                    if (pos[new_y][new_x]<=0) {
                        boardc=pos;
                        boardc[y][x]=0;
                        boardc[new_y][new_x]=player;
                        children.push_back(boardc);
                        if (pos[new_y][new_x]!=0) {break;}
                    }
                    else {break;}
                }
                else {break;}
            }
        }
    }
    //
    return children;
}

std::vector<std::vector<std::vector<int>>> generate_children(std::vector<std::vector<int>> position, int playerk) {
    std::vector<std::vector<std::vector<int>>> children;
    // 9&-9 zu 1&-1
    if (playerk==6) {
        for (int y=0; y<position.size(); ++y) {
            for (int x=0; x<position[y].size(); ++x) {
                if (position[y][x]==9) {position[y][x]=1;}
            }
        }
    }
    else if (playerk==-6) {
        for (int y=0; y<position.size(); ++y) {
            for (int x=0; x<position[y].size(); ++x) {
                if (position[y][x]==-9) {position[y][x]=-1;}
            }
        }
    }
    //
    if (playerk==6) {
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (position[y][x]==1) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcBb(y, x, position, 1);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==6) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcKk(y, x, position, 6);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==4 || position[y][x]==7) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQq(y, x, position, 4, {0, 0, 1, -1}, {1, -1, 0, 0});
                    children.insert(children.end(), new_children.begin(), new_children.end());                
                }
                else if (position[y][x]==3) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQq(y, x, position, 3, {1, -1, -1, 1}, {1, 1, -1, -1});
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==5) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQq(y, x, position, 5, {1, -1, -1, 1, 0, 0, 1, -1}, {1, 1, -1, -1, 1, -1, 0, 0});
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==2) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcLl(y, x, position, 2);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==8) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcKk(y, x, position, 8);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
            }
        }
    }
    else if (playerk==-6) {
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (position[y][x]==-1) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcBb(y, x, position, -1);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==-6) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcKk(y, x, position, -6);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==-4 || position[y][x]==-7) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQq(y, x, position, -4, {0, 0, 1, -1}, {1, -1, 0, 0});
                    children.insert(children.end(), new_children.begin(), new_children.end());                
                }
                else if (position[y][x]==-3) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQq(y, x, position, -3, {1, -1, -1, 1}, {1, 1, -1, -1});
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==-5) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcTtXxQq(y, x, position, -5, {1, -1, -1, 1, 0, 0, 1, -1}, {1, 1, -1, -1, 1, -1, 0, 0});
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==-2) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcLl(y, x, position, -2);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
                else if (position[y][x]==-8) {
                    std::vector<std::vector<std::vector<int>>> new_children=gcKk(y, x, position, -8);
                    children.insert(children.end(), new_children.begin(), new_children.end());
                }
            }
        }
    }
    return children;
}

//

std::vector<std::vector<int>> gorcLl(int y, int x, std::vector<std::vector<int>> boardc, int player) {
    std::vector<std::pair<int, int>> childrenL;
    std::vector<std::pair<int, int>> moves = {{-2, 1}, {-2, -1}, {2, 1}, {2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};
    //
    for (const auto& move : moves) {
        int new_y = y + move.first;
        int new_x = x + move.second;
        if (-1<new_y && new_y<8 && -1<new_x && new_x<8) {
            if ((player==-2 && boardc[new_y][new_x]>=0) || (player==2 && boardc[new_y][new_x]<=0)) {
                childrenL.push_back({new_y, new_x});
            }
        }
    }
    //
    if (!childrenL.empty()) {
        int chosen_index = generate_random_int(0, childrenL.size()-1);
        int new_y = childrenL[chosen_index].first;
        int new_x = childrenL[chosen_index].second;
        boardc[y][x] = 0;
        boardc[new_y][new_x] = player;
        return boardc;
    }
    else {std::vector<std::vector<int>> empty_vector; return empty_vector;}
}

std::vector<std::vector<int>> gorcTtXxQq(int y, int x, std::vector<std::vector<int>> boardc, int player, std::vector<std::pair<int, int>> directions) {
    if (player==-7) {player=-4;}
    else if (player==7) {player=4;}
    std::vector<std::vector<int>> children;
    //
    for (auto direction : directions) {
        for (int i=1; i<8; ++i) {
            int dy = direction.first;
            int dx = direction.second;
            int new_y = y+i*dy;
            int new_x = x+i*dx;
            //
            if (-1<new_x && new_x<8 && -1<new_y && new_y<8) {
                if ((player<0 && boardc[new_y][new_x]>=0) || (player>0 && boardc[new_y][new_x]<=0)) {
                    if (boardc[new_y][new_x] == 0) {children.push_back(std::vector<int>{y+i*dy, x+i*dx});}
                    else {children.push_back(std::vector<int>{y+i*dy, x+i*dx}); break;}
                }
                else {break;}
            }
            else {break;}
        }
    }
    //
    if (!children.empty()) {
        int randomIndex = generate_random_int(0, children.size()-1);
        int new_y = children[randomIndex][0];
        int new_x = children[randomIndex][1];
        boardc[y][x] = 0;
        boardc[new_y][new_x] = player;
        return boardc;
    }
    else {std::vector<std::vector<int>> empty_vector; return empty_vector;}
}

std::vector<std::vector<int>> gorcBb(int y, int x, std::vector<std::vector<int>> boardc, int player) {
    std::vector<int> childrenB;
    //
    if (player==-1) {
        if (y==1 && boardc[y+2][x]==0 && boardc[y+1][x]==0) {childrenB.push_back(1);}
        if (y+1<8) {if (boardc[y+1][x]==0) {childrenB.push_back(2);}}
        if (x-1>-1 && y+1<8) {if (boardc[y+1][x-1]>0) {childrenB.push_back(3);}}
        if (x+1<8 && y+1<8) {if (boardc[y+1][x+1]>0) {childrenB.push_back(4);}}
        // En passant
        if (x-1>-1 && y+1<8) {if (boardc[y][x-1]==9 && boardc[y+1][x-1]==0) {childrenB.push_back(5);}}
        if (x+1<8 && y+1<8) {if (boardc[y][x+1]==9 && boardc[y+1][x+1]==0) {childrenB.push_back(6);}}
    }
    if (player==1) {
        if (y==6 && boardc[y-2][x]==0 && boardc[y-1][x]==0) {childrenB.push_back(1);}
        if (y-1>-1) {if (boardc[y-1][x]==0) {childrenB.push_back(2);}}
        if (x-1>-1 && y-1>-1) {if (boardc[y-1][x-1]<0) {childrenB.push_back(3);}}
        if (x+1<8 && y-1>-1) {if (boardc[y-1][x+1]<0) {childrenB.push_back(4);}}
        // En passant
        if (x-1>-1 && y-1>-1) {if (boardc[y][x-1]==-9 && boardc[y-1][x-1]==0) {childrenB.push_back(5);}}
        if (x+1<8 && y-1>-1) {if (boardc[y][x+1]==-9 && boardc[y-1][x+1]==0) {childrenB.push_back(6);}}
    }
    if (player==-1) {
        if (childrenB.empty()) {std::vector<std::vector<int>> empty_vector; return empty_vector;}
        else {
            int r = generate_random_int(0, childrenB.size()-1);
            int n=childrenB[r];
            if (n==1) {
                boardc[y][x]=0;
                boardc[y+2][x]=-9;
                return boardc;
            }
            else if (n==2) {
                boardc[y][x]=0;
                boardc[y+1][x]=-1;
                if (y+1==7) {boardc[y+1][x]=-5;}
                return boardc;
            }
            else if (n==3) {
                boardc[y][x]=0;
                boardc[y+1][x-1]=-1;
                if (y+1==7) {boardc[y+1][x-1]=-5;}
                return boardc;
            }
            else if (n==4) {
                boardc[y][x]=0;
                boardc[y+1][x+1]=-1;
                if (y+1==7) {boardc[y+1][x+1]=-5;}
                return boardc;
            }
            else if (n==5) {
                boardc[y][x]=0;
                boardc[y+1][x-1]=-1;
                boardc[y][x-1]=0;
                return boardc;
            }
            else if (n==6) {
                boardc[y][x]=0;
                boardc[y+1][x+1]=-1;
                boardc[y][x+1]=0;
                return boardc;
            }
        }
    }
    else if (player==1) {
        if (childrenB.empty()) {std::vector<std::vector<int>> empty_vector; return empty_vector;}
        else {
            int r = generate_random_int(0, childrenB.size()-1);
            int n=childrenB[r];
            if (n==1) {
                boardc[y][x]=0;
                boardc[y-2][x]=9;
                return boardc;
            }
            else if (n==2) {
                boardc[y][x]=0;
                boardc[y-1][x]=1;
                if (y-1==0) {boardc[y-1][x]=5;}
                return boardc;
            }
            else if (n==3) {
                boardc[y][x]=0;
                boardc[y-1][x-1]=1;
                if (y-1==0) {boardc[y-1][x-1]=5;}
                return boardc;
            }
            else if (n==4) {
                boardc[y][x]=0;
                boardc[y-1][x+1]=1;
                if (y-1==0) {boardc[y-1][x+1]=5;}
                return boardc;
            }
            else if (n==5) {
                boardc[y][x]=0;
                boardc[y-1][x-1]=1;
                boardc[y][x-1]=0;
                return boardc;
            } 
            else if (n==6) {
                boardc[y][x]=0;
                boardc[y-1][x+1]=1;
                boardc[y][x+1]=0;
                return boardc;
            }
        }
    }
    return boardc;
}

std::vector<std::vector<int>> gorcKk(int y, int x, std::vector<std::vector<int>> boardc, int player) {
    std::vector<int> childrenK;
    std::vector<std::pair<int, int>> moves = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    //
    std::pair<int, int> move;
    for (int i=0; i<8; ++i) {
        move=moves[i];
        int dy = move.first;
        int dx = move.second;
        int new_y = y + dy;
        int new_x = x + dx;
        //
        if (-1<new_y && new_y<8 && -1<new_x && new_x<8) {
            if ((player==-6 || player==-8) && boardc[new_y][new_x]>=0) {childrenK.push_back(i);}
            else if ((player==6 || player==8) && boardc[new_y][new_x]<=0) {childrenK.push_back(i);}
        }
    }
    // Rochade
    if (player==-8) {
        if (boardc[0][0]==-7 && boardc[0][1]==0 && boardc[0][2]==0 && boardc[0][3]==0) {
            std::vector<std::vector<int>> boardcc = boardc;
            boardcc[0][2] = -6;
            boardcc[0][3] = -4;
            boardcc[0][0] = 0;
            boardcc[0][4] = 0;
            bool legal = true;
            for (const auto& child : generate_children(boardcc, 6)) {
                if (child[0][2]>0 || child[0][3]>0 || child[0][4]>0) {legal=false; break;}
            }
            if (legal) {childrenK.push_back(8);}
        }
        if (boardc[0][7]==-7 && boardc[0][6]==0 && boardc[0][5]==0) {
            std::vector<std::vector<int>> boardcc = boardc;
            boardcc[0][6] = -6;
            boardcc[0][5] = -4;
            boardcc[0][7] = 0;
            boardcc[0][4] = 0;
            bool legal = true;
            for (const auto& child : generate_children(boardcc, 6)) {
                if (child[0][4]>0 || child[0][5]>0 || child[0][6]>0) {legal=false; break;}
            }
            if (legal) {childrenK.push_back(9);}
        }
    }
    else if (player==8) {
        if (boardc[7][0]==7 && boardc[7][1]==0 && boardc[7][2]==0 && boardc[7][3]==0) {
            std::vector<std::vector<int>> boardcc = boardc;
            boardcc[7][2] = 6;
            boardcc[7][3] = 4;
            boardcc[7][0] = 0;
            boardcc[7][4] = 0;
            bool legal = true;
            for (const auto& child : generate_children(boardcc, -6)) {
                if (child[7][2]<0 || child[7][3]<0 || child[7][4]<0) {legal=false; break;}
            }
            if (legal) {childrenK.push_back(10);}
        }
        if (boardc[7][7]==7 && boardc[7][6]==0 && boardc[7][5]==0) {
            std::vector<std::vector<int>> boardcc = boardc;
            boardcc[7][6] = 6;
            boardcc[7][5] = 4;
            boardcc[7][7] = 0;
            boardcc[7][4] = 0;
            bool legal = true;
            for (const auto& child : generate_children(boardcc, -6)) {
                if (child[7][4]<0 || child[7][5]<0 || child[7][6]<0) {legal=false; break;}
            }
            if (legal) {childrenK.push_back(11);}
        }
    }
    //
    if (childrenK.empty()) {std::vector<std::vector<int>> empty_vector; return empty_vector;}
    else {
        int r = generate_random_int(0, childrenK.size()-1);
        int n = childrenK[r];
        if (n<8) {    
            boardc[y][x] = 0;
            std::pair<int,int> move=moves[n];
            int dy = move.first;
            int dx = move.second;
            if (player==8) {player=6;}
            else if (player==-8) {player=-6;}
            boardc[y+dy][x+dx] = player;
            return boardc;
        }
        else if (n==8) {
            boardc[0][2] = -6;
            boardc[0][3] = -4;
            boardc[0][0] = 0;
            boardc[0][4] = 0;
            return boardc;
        }
        else if (n==9) {
            boardc[0][6] = -6;
            boardc[0][5] = -4;
            boardc[0][7] = 0;
            boardc[0][4] = 0;
            return boardc;
        }
        else if (n==10) {
            boardc[7][2] = 6;
            boardc[7][3] = 4;
            boardc[7][0] = 0;
            boardc[7][4] = 0;
            return boardc;
        }
        else if (n==11) {
            boardc[7][6] = 6;
            boardc[7][5] = 4;
            boardc[7][7] = 0;
            boardc[7][4] = 0;
            return boardc;
        }
    }
}

std::vector<std::vector<int>> generate_one_random_child(std::vector<std::vector<int>> position, int playerk) {
    std::vector<std::vector<int>> boardcopy = position;
    //9 and -9 to 1 and -1
    if (playerk==6) {
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (boardcopy[y][x]==9) {boardcopy[y][x]=1;}
            }
        }
    }
    else if (playerk==-6) {
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (boardcopy[y][x]==-9) {boardcopy[y][x]=-1;}
            }
        }
    }
    //
    //Hier haben alle Figuren die gleiche Chance ausgewählt zu werden
    //->Alle Zuege haben nicht die gleiche Chance ausgewählt zu werden
    //Man koente das anders machen, indem man Figuren mehrmals zu piecesy und -x appediert
    //wird in Schach3 so gemacht.
    std::vector<int> piecesy;
    std::vector<int> piecesx;
    if (playerk==6) {
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (boardcopy[y][x]>0) {
                    piecesy.push_back(y);
                    piecesx.push_back(x);
                }
            }
        }
    }
    else if (playerk==-6) {
        for (int y=0; y<8; ++y) {
            for (int x=0; x<8; ++x) {
                if (boardcopy[y][x]<0) {
                    piecesy.push_back(y);
                    piecesx.push_back(x);
                }
            }
        }
    }
    //
    if (piecesx.empty()) {return std::vector<std::vector<int>>();}
    //
    for (int i=0; i<40; ++i) {
        int n = generate_random_int(0, piecesx.size()-1);
        int y = piecesy[n];
        int x = piecesx[n];
        std::vector<std::vector<int>> child;
        //
        if (boardcopy[y][x]==1 || boardcopy[y][x]==-1) {
            child = gorcBb(y, x, boardcopy, boardcopy[y][x]);
        }
        else if (boardcopy[y][x]==2 || boardcopy[y][x]==-2) {
            child = gorcLl(y, x, boardcopy, boardcopy[y][x]);
        }
        else if (boardcopy[y][x]==3 || boardcopy[y][x]==-3) {
            child = gorcTtXxQq(y, x, boardcopy, boardcopy[y][x], {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}});
        }
        else if (boardcopy[y][x]==4 || boardcopy[y][x]==-4 || boardcopy[y][x]==-7 || boardcopy[y][x]==7) {
            child = gorcTtXxQq(y, x, boardcopy, boardcopy[y][x], {{1, 0}, {-1, 0}, {0, 1}, {0, -1}});
        }
        else if (boardcopy[y][x]==5 || boardcopy[y][x]==-5) {
            child = gorcTtXxQq(y, x, boardcopy, boardcopy[y][x], {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}});
        }
        else if (boardcopy[y][x]==6 || boardcopy[y][x]==-6 || boardcopy[y][x]==-8 || boardcopy[y][x]==8) {
            child = gorcKk(y, x, boardcopy, boardcopy[y][x]);
        }
        //
        if (!child.empty()) {
            return child;
        }
    }
    std::vector<std::vector<int>> empty_vector;
    return empty_vector;
}

//

std::vector<std::vector<int>> B_matrix = {
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1},
    { 2,  2,  2,  2,  2,  2,  2,  2},
    { 3,  3,  4,  4,  4,  4,  3,  3},
    { 3,  3,  4,  4,  4,  4,  3,  3},
    { 4,  4,  4,  4,  4,  4,  4,  4},
    { 5,  5,  5,  5,  5,  5,  5,  5},
    { 0,  0,  0,  0,  0,  0,  0,  0}
};

std::vector<std::vector<int>> b_matrix = {
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 5,  5,  5,  5,  5,  5,  5,  5},
    { 4,  4,  4,  4,  4,  4,  4,  4},
    { 3,  3,  4,  4,  4,  4,  3,  3},
    { 3,  3,  4,  4,  4,  4,  3,  3},
    { 2,  2,  2,  2,  2,  2,  2,  2},
    { 1,  1,  1,  1,  1,  1,  1,  1},
    { 0,  0,  0,  0,  0,  0,  0,  0}
};

std::vector<std::vector<int>> Ll_matrix = {
    {-1, -3, -2, -2, -2, -2, -3, -1},
    {-1,  0,  0,  0,  0,  0,  0, -1},
    {-1,  0,  1,  1,  1,  1,  0, -1},
    {-1,  0,  1,  2,  2,  1,  0, -1},
    {-1,  0,  1,  2,  2,  1,  0, -1},
    {-1,  0,  1,  1,  1,  1,  0, -1},
    {-1,  0,  0,  0,  0,  0,  0, -1},
    {-1, -3, -2, -2, -2, -2, -3, -1}
};

std::vector<std::vector<int>> Xx_matrix = {
    {-2, -1, -1, -1, -1, -1, -1, -2},
    {-1,  0,  0,  0,  0,  0,  0, -1},
    {-1,  0,  1,  1,  1,  1,  0, -1},
    {-1,  0,  1,  2,  2,  1,  0, -1},
    {-1,  0,  1,  2,  2,  1,  0, -1},
    {-1,  0,  1,  1,  1,  1,  0, -1},
    {-1,  0,  0,  0,  0,  0,  0, -1},
    {-2, -1, -1, -1, -1, -1, -1, -2}
};

std::vector<std::vector<int>> Tt_matrix = {
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1},
    { 1,  1,  1,  1,  1,  1,  1,  1},
    { 1,  1,  1,  1,  1,  1,  1,  1},
    { 1,  1,  1,  1,  1,  1,  1,  1},
    { 1,  1,  1,  1,  1,  1,  1,  1},
    { 1,  1,  1,  1,  1,  1,  1,  1},
    { 0,  0,  0,  0,  0,  0,  0,  0}
};

std::vector<std::vector<int>> Qq_matrix = {
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1,  0,  0,  0,  0,  0,  0, -1},
    {-1,  0,  1,  1,  1,  1,  0, -1},
    {-1,  0,  1,  2,  2,  1,  0, -1},
    {-1,  0,  1,  2,  2,  1,  0, -1},
    {-1,  0,  1,  1,  1,  1,  0, -1},
    {-1,  0,  0,  0,  0,  0,  0, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1}
};

std::vector<std::vector<int>> K_matrix = {
    { 2,  3,  1,  0,  0,  1,  3,  2},
    { 2,  2,  0,  0,  0,  0,  2,  2},
    {-1, -2, -2, -2, -2, -2, -2, -1},
    {-2, -3, -3, -4, -4, -3, -3, -2},
    {-3, -4, -4, -5, -5, -4, -4, -3},
    {-3, -4, -4, -5, -5, -4, -4, -3},
    {-4, -5, -5, -6, -6, -5, -5, -4},
    {-4, -5, -5, -6, -6, -5, -5, -4}
};

std::vector<std::vector<int>> k_matrix = {
    { -4,  -5,  -5,  -6,  -6,  -5,  -5,  -4},
    { -4,  -5,  -5,  -6,  -6,  -5,  -5,  -4},
    { -3,  -4,  -4,  -5,  -5,  -4,  -4,  -3},
    { -3,  -4,  -4,  -5,  -5,  -4,  -4,  -3},
    { -2,  -3,  -3,  -4,  -4,  -3,  -3,  -2},
    { -1,  -2,  -2,  -2,  -2,  -2,  -2,  -1},
    {  2,   2,   0,   0,   0,   0,   2,   2},
    {  2,   3,   1,   0,   0,   1,   3,   2}
};

std::vector<std::vector<int>> other_B_matrix = matrix_minus(B_matrix);
std::vector<std::vector<int>> other_b_matrix = matrix_minus(b_matrix);
std::vector<std::vector<int>> other_Ll_matrix = matrix_minus(Ll_matrix);
std::vector<std::vector<int>> other_Xx_matrix = matrix_minus(Xx_matrix);
std::vector<std::vector<int>> other_Tt_matrix = matrix_minus(Tt_matrix);
std::vector<std::vector<int>> other_Qq_matrix = matrix_minus(Qq_matrix);
std::vector<std::vector<int>> other_K_matrix = matrix_minus(K_matrix);
std::vector<std::vector<int>> other_k_matrix = matrix_minus(k_matrix);

//

int evaluate_position(const std::vector<std::vector<int>>& pos, int playerk) {
    int val = 0;
    if (playerk==6) {
        for (int p=0; p<8; ++p) {
            for (int o=0; o<8; ++o) {
                if (pos[p][o]==0) {;}
                else if (pos[p][o]==-1 || pos[p][o]==-9) {
                    val += -100;
                    val += other_B_matrix[p][o];
                    // Pawn structure
                    if (p+1<=7 && o+1<=7) {
                        if (pos[p+1][o+1]==-1 || pos[p+1][o+1]==-9) {val += -1;}
                    }
                    if (p-1>=0 && o-1>=0) {
                        if (pos[p-1][o-1]==-1 || pos[p-1][o-1]==-9) {val += -1;}
                    }
                    if (p+1<=7 && o-1>=0) {
                        if (pos[p+1][o-1]==-1 || pos[p+1][o-1]==-9) {val += -1;}
                    }
                    if (p-1>=0 && o+1<=7) {
                        if (pos[p-1][o+1]==-1 || pos[p-1][o+1]==-9) {val += -1;}
                    }
                }
                else if (pos[p][o]==1 || pos[p][o]==9) {
                    val += 100;
                    val += b_matrix[p][o];
                    // Pawn structure
                    if (p+1<=7 && o+1<=7) {
                        if (pos[p+1][o+1]==1 || pos[p+1][o+1]==9) {val += 1;}
                    }
                    if (p-1>=0 && o-1>=0) {
                        if (pos[p-1][o-1]==1 || pos[p-1][o-1]==9) {val += 1;}
                    }
                    if (p+1<=7 && o-1>=0) {
                        if (pos[p+1][o-1]==1 || pos[p+1][o-1]==9) {val += 1;}
                    }
                    if (p-1>=0 && o+1<=7) {
                        if (pos[p-1][o+1]==1 || pos[p-1][o+1]==9) {val += 1;}
                    }
                }
                else if (pos[p][o]==-2) {
                    val += -300;
                    val += other_Ll_matrix[p][o];
                }
                else if (pos[p][o]==-3) {
                    val += -300;
                    val += other_Xx_matrix[o][p];
                }
                else if (pos[p][o]==-4 || pos[p][o]==-7) {
                    val += -500;
                    val += other_Tt_matrix[p][o];
                }
                else if (pos[p][o]==2) {
                    val += 300;
                    val += Ll_matrix[p][o];
                }
                else if (pos[p][o]==3) {
                    val += 300;
                    val += Xx_matrix[o][p];
                }
                else if (pos[p][o]==4 || pos[p][o]==7) {
                    val += 500;
                    val += Tt_matrix[p][o];
                }
                else if (pos[p][o]==-5) {
                    val += -900;
                    val += other_Qq_matrix[p][o];
                }
                else if (pos[p][o]==-6 || pos[p][o]==-8) {
                    val += -100000;
                    val += other_K_matrix[p][o];
                }
                else if (pos[p][o]==5) {
                    val += 900;
                    val += Qq_matrix[p][o];
                }
                else if (pos[p][o]==6 || pos[p][o]==8) {
                    val += 100000;
                    val += k_matrix[p][o];
                }
            }
        }
    }
    else if (playerk==-6) {
        for (int p=0; p<8; ++p) {
            for (int o=0; o<8; ++o) {
                if (pos[p][o]==0) {;}
                else if (pos[p][o]==-1 || pos[p][o]==-9) {
                    val += 100;
                    val += B_matrix[p][o];
                    // Pawn structure
                    if (p+1<=7 && o+1<=7) {
                        if (pos[p+1][o+1]==-1 || pos[p+1][o+1]==-9) {val += 1;}
                    }
                    if (p-1>=0 && o-1>=0) {
                        if (pos[p-1][o-1]==-1 || pos[p-1][o-1]==-9) {val += 1;}
                    }
                    if (p+1<=7 && o-1>=0) {
                        if (pos[p+1][o-1]==-1 || pos[p+1][o-1]==-9) {val += 1;}
                    }
                    if (p-1>=0 && o+1<=7) {
                        if (pos[p-1][o+1]==-1 || pos[p-1][o+1]==-9) {val += 1;}
                    }
                }
                else if (pos[p][o]==1 || pos[p][o]==9) {
                    val += -100;
                    val += other_b_matrix[p][o];
                    // Pawn structure
                    if (p+1<=7 && o+1<=7) {
                        if (pos[p+1][o+1]==1 || pos[p+1][o+1]==9) {val += -1;}
                    }
                    if (p-1>=0 && o-1>=0) {
                        if (pos[p-1][o-1]==1 || pos[p-1][o-1]==9) {val += -1;}
                    }
                    if (p+1<=7 && o-1>=0) {
                        if (pos[p+1][o-1]==1 || pos[p+1][o-1]==9) {val += -1;}
                    }
                    if (p-1>=0 && o+1<=7) {
                        if (pos[p-1][o+1]==1 || pos[p-1][o+1]==9) {val += -1;}
                    }
                }
                else if (pos[p][o]==-2) {
                    val += 300;
                    val += Ll_matrix[p][o];
                }
                else if (pos[p][o]==-3) {
                    val += 300;
                    val += Xx_matrix[p][o];
                }
                else if (pos[p][o]==-4 || pos[p][o]==-7) {
                    val += 500;
                    val += Tt_matrix[p][o];
                }
                else if (pos[p][o]==2) {
                    val += -300;
                    val += other_Ll_matrix[p][o];
                }
                else if (pos[p][o]==3) {
                    val += -300;
                    val += other_Xx_matrix[p][o];
                }
                else if (pos[p][o]==4 || pos[p][o]==7) {
                    val += -500;
                    val += other_Tt_matrix[p][o];
                }
                else if (pos[p][o]==-5) {
                    val += 900;
                    val += other_Qq_matrix[p][o];
                }
                else if (pos[p][o]==-6 || pos[p][o]==-8) {
                    val += 100000;
                    val += other_K_matrix[p][o];
                }
                else if (pos[p][o]==5) {
                    val += -900;
                    val += Qq_matrix[p][o];
                }
                else if (pos[p][o]==6 || pos[p][o]==8) {
                    val += -100000;
                    val += other_k_matrix[p][o];
                }
            }
        }
    }
    return val;
}

//

class HumanPlayer {
public:
    HumanPlayer(int token) : token(token) {}
    int token=token;

    bool do_these_two_vectors_have_the_same_elements(const std::vector<std::vector<int>>& vector1, const std::vector<std::vector<int>>& vector2) {
        for (size_t i=0; i<vector1.size(); ++i) {
            for (size_t j=0; j<vector1[i].size(); ++j) {
                if (vector1[i][j] != vector2[i][j]) {return false;}
            }
        }
        return true;
    }

    std::vector<int> eingabe() {
        try {
            int vx, vy, zx, zy;
            std::cout <<"von x: ";
            std::cin>> vx;
            std::cout <<"von y: ";
            std::cin>> vy;
            std::cout <<"zu x: ";
            std::cin>> zx;
            std::cout <<"zu y: ";
            std::cin >> zy;
            //
            vx -= 1;
            vy -= 1;
            zx -= 1;
            zy -= 1;
            //
            if (vy < 8 && vy > -1 && vx < 8 && vx > -1 && zy < 8 && zy > -1 && zx < 8 && zx > -1 && is_int(vx) && is_int(vy)&& is_int(zx)&& is_int(zy)) {
                    return {vy, vx, zy, zx};
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "EINGABE NICHT KORREKT1" << std::endl;
                return eingabe();
            }
        }
        catch (...) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "EINGABE NICHT KORREKT1" << std::endl;
            return eingabe();
        }
    }

    std::vector<std::vector<int>> player(std::vector<std::vector<int>>& pos) {
        int other_player = (this->token == 6) ? -6 : 6;
        std::vector<std::vector<std::vector<int>>> legal_moves;
        bool legal_move_exists = false;
        std::vector<std::vector<std::vector<int>>> all_moves = generate_children(pos, this->token);
        //
        for (const auto& child_of_root : all_moves) {
            bool king_is_killed = false;
            for (const auto& child_of_child : generate_children(child_of_root, other_player)) {
                if (verloren(child_of_child, this->token)) {
                    king_is_killed = true;
                    break;
                }
            }
            if (!king_is_killed) {
                legal_move_exists = true;
                legal_moves.push_back(child_of_root);
            }
        }
        //
        if (!legal_move_exists) {
            std::vector<std::vector<int>> empty_vector;
            return empty_vector;
        }
        //
        while (true) {
            std::vector<std::vector<int>> boardcopy = pos;
            std::vector<int> input_move = this->eingabe();
            int vy = input_move[0];
            int vx = input_move[1];
            int zy = input_move[2];
            int zx = input_move[3];

            //update boardcopy
            //1 & -1 to 5 & -5
            for (int x=0; x<boardcopy[0].size(); ++x) {
                if (boardcopy[0][x]==1) {boardcopy[0][x]=5;}
            }
            for (int x=0; x<boardcopy[7].size(); ++x) {
                if (boardcopy[7][x]==-1) {boardcopy[7][x]=-5;}
            }
            //9 & -9weg
            if (this->token==6) {
                for (int y=0; y<boardcopy.size(); ++y) {
                    for (int x=0; x<boardcopy[y].size(); ++x) {
                        if (boardcopy[y][x]==9) {boardcopy[y][x] = 1;}
                    }
                }
            }
            else if (this->token==-6) {
                for (int y=0; y<boardcopy.size(); ++y) {
                    for (int x=0; x<boardcopy[y].size(); ++x) {
                        if (boardcopy[y][x]==-9) {boardcopy[y][x] = -1;}
                    }
                }
            }
            //special moves
            bool special = false;
            //rochade
            if (vy==7 && vx==4 && zy==7 && zx==2 && this->token==6 && boardcopy[vy][vx]==8) {
                special = true;
                boardcopy[7][2] = 6;
                boardcopy[7][3] = 4;
                boardcopy[7][0] = 0;
                boardcopy[7][4] = 0;
            }
            if (vy==7 && vx==4 && zy==7 && zx==6 && this->token==6 && boardcopy[vy][vx]==8) {
                special = true;
                boardcopy[7][6] = 6;
                boardcopy[7][5] = 4;
                boardcopy[7][7] = 0;
                boardcopy[7][4] = 0;
            }
            if (vy==0 && vx==4 && zy==0 && zx==2 && this->token==-6 && boardcopy[vy][vx]==-8) {
                special = true;
                boardcopy[0][2] = -6;
                boardcopy[0][3] = -4;
                boardcopy[0][0] = 0;
                boardcopy[0][4] = 0;
            }
            if (vy==0 && vx==4 && zy==0 && zx==6 && this->token==-6 && boardcopy[vy][vx]==-8) {
                special = true;
                boardcopy[0][6] = -6;
                boardcopy[0][5] = -4;
                boardcopy[0][7] = 0;
                boardcopy[0][4] = 0;
            }
            // Bb 2 nach vorne
            if (boardcopy[vy][vx]==1 && vy==6 && boardcopy[zy][zx]==0 && boardcopy[vy-1][vx]==0 && vx==zx && zy==vy-2) {
                special = true;
                boardcopy[zy][zx] = 9;
                boardcopy[vy][vx] = 0;
            }
            if (boardcopy[vy][vx]==-1 && vy==1 && boardcopy[zy][zx]==0 && boardcopy[vy+1][vx]==0 && vx==zx && zy==vy+2) {
                special = true;
                boardcopy[zy][zx] = -9;
                boardcopy[vy][vx] = 0;
            }
            // En passant
            if (boardcopy[vy][vx]==1 && zy==vy-1 && zx==vx-1 && boardcopy[zy][zx]==0) {
                special = true;
                boardcopy[vy][vx] = 0;
                boardcopy[zy][zx] = 1;
                boardcopy[vy][zx] = 0;
            }
            if (boardcopy[vy][vx]==1 && zy==vy-1 && zx==vx+1 && boardcopy[zy][zx]==0) {
                special = true;
                boardcopy[vy][vx] = 0;
                boardcopy[zy][zx] = 1;
                boardcopy[vy][zx] = 0;
            }
            if (boardcopy[vy][vx]==-1 && zy==vy+1 && zx==vx-1 && boardcopy[zy][zx]==0) {
                special = true;
                boardcopy[vy][vx] = 0;
                boardcopy[zy][zx] = -1;
                boardcopy[vy][zx] = 0;
            }
            if (boardcopy[vy][vx]==-1 && zy==vy+1 && zx==vx+1 && boardcopy[zy][zx]==0) {
                special = true;
                boardcopy[vy][vx] = 0;
                boardcopy[zy][zx] = -1;
                boardcopy[vy][zx] = 0;
            }
            //normal
            if (!special) {
                if (boardcopy[vy][vx]==7) {boardcopy[zy][zx] = 4;}
                else if (boardcopy[vy][vx]==-7) {boardcopy[zy][zx] = -4;}
                else if (boardcopy[vy][vx]==8) {boardcopy[zy][zx] = 6;}
                else if (boardcopy[vy][vx]==-8) {boardcopy[zy][zx] = -6;}
                else {boardcopy[zy][zx] = boardcopy[vy][vx];}
                boardcopy[vy][vx] = 0;
            }
            //
            bool move_legal = false;
            for (const auto& legal_move : legal_moves) {
                if (do_these_two_vectors_have_the_same_elements(boardcopy, legal_move)) {
                    move_legal = true;
                    break;
                }
            }
            if (move_legal) {return boardcopy;}
            else {
                print_board(boardcopy);
                std::cout << "legal_children" << std::endl;
                for (const auto& s : legal_moves) {print_board(s);}
                std::cout << "---------------------------------" << std::endl;
                print_board(pos);
                std::cout << "EINGABE NICHT KORREKT2" << std::endl;
            }
        }
    }

    std::vector<std::vector<int>> get_move(std::vector<std::vector<int>> board) {
        return player(board);
    }

};

//

int minimax_counter=0;

class MinimaxNode {
public:
    MinimaxNode() : value(0), value_not_none(false), children(), board(), player_am_zug(0), token(0), depth(0), expanded(false) {}

    int value;
    bool value_not_none;
    std::vector<MinimaxNode> children;
    std::vector<std::vector<int>> board;
    int player_am_zug;
    int token;
    int depth;
    bool expanded;

    std::vector<MinimaxNode> expand_node() {
        std::vector<MinimaxNode> new_children;
        std::vector<std::vector<std::vector<int>>> list_of_positions = generate_children(this->board, this->player_am_zug);
        for (const std::vector<std::vector<int>>& board_position : list_of_positions) {
            MinimaxNode child;
            child.board = board_position;
            child.player_am_zug = -this->player_am_zug;
            child.token = this->token;
            child.depth = this->depth + 1;
            child.value_not_none = false;
            child.value=0;
            child.children;
            new_children.push_back(child);

        }
        return new_children;
    }

    int minimax(int alpha, int beta, bool max_player, const int max_depth) {
        //
        minimax_counter+=1;
        //
        if (this->depth>=max_depth || verloren(this->board,6) || verloren(this->board,-6)) {
            this->value = evaluate_position(this->board, this->token);
            this->value_not_none=true;
            return this->value;
        }
        //
        if (!this->expanded) {this->children=this->expand_node(); this->expanded = true;}
        if (this->children.empty()) {
            this->value = evaluate_position(this->board, this->token);
            this->value_not_none=true;
            return this->value;
        }
        //
        if (max_player) {
            int max_value=-1000000;
            for (MinimaxNode& child : this->children) {
                int eval=child.minimax(alpha,beta,false,max_depth);
                if (eval>max_value) {max_value=eval;}
                //pruning
                if (eval>alpha) {alpha=eval;}
                if (beta<=alpha) {break;}
            }
            this->value=max_value;
            return max_value;
        }
        else if (!max_player) {
            int min_value=1000000;
            for (MinimaxNode& child : this->children) {
                int eval=child.minimax(alpha,beta,true,max_depth);
                if (eval<min_value) {min_value=eval;}
                //pruning
                if (eval<beta) {beta=eval;}
                if (beta<=alpha) {break;}
            }
            this->value=min_value;
            return min_value;
        }

    }

    void sort(bool max_player) {
        if (this->expanded) {
            //
            std::vector<MinimaxNode> not_none_children;
            std::list<MinimaxNode> none_children;
            std::vector<MinimaxNode> sorted_children;
            //
            for (const MinimaxNode& child : this->children) {
                if (!child.value_not_none) {none_children.push_back(child);}
                else {not_none_children.push_back(child);}
            }
            //
            if (max_player) {
                std::sort(not_none_children.begin(), not_none_children.end(),[](const MinimaxNode& a, const MinimaxNode& b) {return a.value > b.value;});
                //
                sorted_children.insert(sorted_children.end(), not_none_children.begin(), not_none_children.end());
                sorted_children.insert(sorted_children.end(), none_children.begin(), none_children.end());
                this->children=sorted_children;
                //
                for (MinimaxNode& child : not_none_children) {child.sort(false);}
            }
            else {
                std::sort(not_none_children.begin(), not_none_children.end(),[](const MinimaxNode& a, const MinimaxNode& b) {return a.value < b.value;});
                //
                sorted_children.insert(sorted_children.end(), not_none_children.begin(), not_none_children.end());
                sorted_children.insert(sorted_children.end(), none_children.begin(), none_children.end());
                this->children=sorted_children;
                //
                for (MinimaxNode& child : not_none_children) {child.sort(true);}
            }
        }
    }

};

class MinimaxPlayer {
public:
    MinimaxPlayer(int token, std::vector<std::vector<int>> board) : token(token), board(board) {
        root_node.board = board;
        root_node.player_am_zug = token;
        root_node.token=token;
        root_node.value_not_none = false;
        root_node.value = 0;
        root_node.depth = 0;
        root_node.children = root_node.expand_node();
        root_node.expanded=true;
        //
        minimax_counter=0;
    }
    MinimaxNode root_node;
    int token;
    std::vector<std::vector<int>> board;
    int max_time=2;
    int max_depth=10;
    int starting_depth=1;

    std::vector<std::vector<int>> minimaxer(const int depth, const std::chrono::duration<double> vergangene_zeit) {
        auto start = std::chrono::high_resolution_clock::now();
        //
        std::vector<int> values;
        std::vector<MinimaxNode> best_moves;
        MinimaxNode best_move;
        int best_value = -100000000000000;
        std::vector<MinimaxNode>& root_node_children=root_node.children;
        std::vector<std::vector<int>> return_board;
        //
        for (MinimaxNode& child : root_node_children){
            int eval;
            if (child.value>-90000) {
                eval=child.minimax(-10000000,10000000,false, depth);
                child.value=eval;
                std::cout<<"a ";//child wurde fertig berechnet
            }
            //
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> vergangene_zeit2 =(now+vergangene_zeit)-start;
            if (vergangene_zeit2.count() >= max_time) {std::cout<<" NICHT FERTIG"; break;}
        }
        //
        for (MinimaxNode& child : root_node_children) {
            if (child.value>-90000) {
                values.push_back(child.value);
            }
        }
        if (values.empty()) {std::vector<std::vector<int>> empty_vector; return empty_vector;}
        for (int v : values) {if (v > best_value) {best_value = v;}}
        for (MinimaxNode& child : root_node_children) {if (child.value==best_value) {best_moves.push_back(child);}}
        //
        //output---------
        std::cout << std::endl;
        std::cout << best_value << std::endl;
        std::cout<<"COUNTER: "; std::cout<<minimax_counter<<std::endl;
        //---------------
        best_move=best_moves[generate_random_int(0, best_moves.size()-1)];
        return_board=deepcopy(best_move.board);
        return return_board;

    }

    std::vector<std::vector<int>> minimaxerer() {
        auto start = std::chrono::high_resolution_clock::now();
        //
        int depth=this->starting_depth;
        std::vector<std::vector<int>> move;
        while (true) {
            //break
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> vergangene_zeit = now - start;
            if (vergangene_zeit.count() >= max_time) {break;}
            else if (depth>max_depth) {break;}
            //calculate move
            std::cout<<"---DEPTH: ";
            std::cout<<depth<<std::endl;
            //
            std::vector<std::vector<int>> new_move=minimaxer(depth,vergangene_zeit);
            if (new_move.empty()) {std::vector<std::vector<int>> empty_vector; return empty_vector;}
            //
            for (MinimaxNode& child : root_node.children) {std::cout<<child.value;  std::cout<<", ";}
            std::cout<<std::endl;
            //sort+depth
            //this->root_node.sort(true);
            move=new_move;
            //
            for (MinimaxNode& child : root_node.children) {std::cout<<child.value;  std::cout<<", ";}
            std::cout<<std::endl;
            if (depth>max_depth) {break;}
            depth+=1;
        }
        return move;
    }

    std::vector<std::vector<int>> get_move(std::vector<std::vector<int>> board) {
        return minimaxerer();
    }
};

//

double c=std::sqrt(2);
int number_of_simulations=20;
int depth=2;

int mcts_counter=0;

class MCTSNode {
public:
    MCTSNode() : value(0), children(),parent(nullptr), visits(0), board(), player_am_zug(0), token(0), expanded(false) {}

    int value;
    std::vector<MCTSNode> children;
    MCTSNode* parent;
    int visits;
    std::vector<std::vector<int>> board;
    int player_am_zug;
    int token;
    bool expanded;

    double calculate_ucb() {
        MCTSNode* par = this->parent;
        if (this->visits == 0) {
            return std::numeric_limits<double>::infinity();
        }
        else {
            double ucb = (static_cast<double>(this->value) / this->visits) +c * (std::sqrt(std::log(static_cast<double>(par->visits)) / this->visits));
            return ucb;
        }
    }

    std::vector<MCTSNode> expand_node() {
        std::vector<MCTSNode> new_children;
        std::vector<std::vector<std::vector<int>>> list_of_positions = generate_children(this->board, this->player_am_zug);
        for (const std::vector<std::vector<int>>& board_position : list_of_positions) {
            MCTSNode child;
            child.value=0;
            child.children;
            child.parent=this;
            child.visits=0;
            child.board=board_position;
            child.player_am_zug=-this->player_am_zug;
            child.token=this->token;
            child.expanded=false;
            //
            new_children.push_back(child);

        }
        return new_children;//vielleicht kein return sondern this->children=new_children?
    }

    MCTSNode* select_leaf_node() {
        double best_value = -std::numeric_limits<double>::infinity();
        MCTSNode* selected_node = nullptr;

        for (MCTSNode& child : this->children) {
            double ucb_of_child = child.calculate_ucb();
            if (ucb_of_child > best_value) {
                best_value = ucb_of_child;
                selected_node = &child;
            }
        }

        if (!selected_node->expanded) {return selected_node;}
        else {return selected_node->select_leaf_node();}
    }

    void backpropagate(double new_value, int number_of_simulations) {
        this->value += new_value;
        this->visits += number_of_simulations;
        //
        if (this->parent != nullptr) {
            parent->backpropagate(new_value, number_of_simulations);
        }
    }

    double simulate() {
        double value = 0.0;
        std::vector<double> values;
        //
        for (int j = 0; j < number_of_simulations; ++j) {
            std::vector<std::vector<int>> pos = this->board;
            int player = this->player_am_zug;
            for (int i = 0; i < depth; ++i) {
                std::vector<std::vector<int>> next_pos = generate_one_random_child(pos, player);
                if (next_pos.empty()) {break;}
                pos = next_pos;
                if (player== -6) {player= 6;}
                else if (player== 6) {player= -6;}
            }
            values.push_back(evaluate_position(pos, this->token));
        }
        for (double val : values) {value += val;}
        value /= values.size();
        //
        return value;
    }

};

class MCTSPlayer {
public:
    MCTSPlayer(int token, std::vector<std::vector<int>> board) : token(token), board(board) {
        root_node.value=0;
        root_node.children;
        root_node.parent=nullptr;
        root_node.visits=0;
        root_node.board=board;
        root_node.player_am_zug=token;
        root_node.token=token;
        root_node.expanded=false;
    }
    MCTSNode root_node;
    int token;
    std::vector<std::vector<int>> board;
    int max_time=1;

    bool mcts2() {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        //filter legal moves
        std::vector<MCTSNode> legal_moves;
        int number_of_legal_moves = 0;
        root_node.children=root_node.expand_node();
        for (MCTSNode& child_of_root : root_node.children) {
            child_of_root.expand_node();
            bool king_is_killed = false;
            for (MCTSNode& child_of_child : child_of_root.children) {
                if (verloren(child_of_child.board, root_node.player_am_zug)) {
                    king_is_killed = true;
                    break;
                }
            }
            if (!king_is_killed) {
                legal_moves.push_back(child_of_root);
                number_of_legal_moves++;
            }
        }
        root_node.children = legal_moves;
        // No legal moves left
        if (number_of_legal_moves == 0) {return false;}
        //
        while (true) {
            mcts_counter += 1;
            MCTSNode* selected_node = root_node.select_leaf_node();
            if (selected_node->visits==0) {
                double new_score = selected_node->simulate();
                selected_node->backpropagate(new_score, number_of_simulations);
            }
            else {selected_node->expand_node();}
            //
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
            if (elapsed_seconds > this->max_time) {
                break;
            }
        }
        return true;
    }

    bool mcts() {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        //filter legal moves
        std::vector<MCTSNode> legal_moves;
        int number_of_legal_moves = 0;
        root_node.children=root_node.expand_node();
        for (MCTSNode& child_of_root : root_node.children) {
            child_of_root.children=child_of_root.expand_node();
            bool king_is_killed = false;
            for (MCTSNode& child_of_child : child_of_root.children) {
                if (verloren(child_of_child.board, root_node.player_am_zug)) {
                    king_is_killed = true;
                    break;
                }
            }
            if (!king_is_killed) {
                legal_moves.push_back(child_of_root);
                number_of_legal_moves++;
            }
        }
        root_node.children = legal_moves;
        // No legal moves left
        if (number_of_legal_moves == 0) {return false;}
        //
        while (true) {
            mcts_counter += 1;
            MCTSNode* selected_node=root_node.select_leaf_node();
            selected_node->children=selected_node->expand_node();
            for (MCTSNode& child_node : selected_node->children) {
                double new_score = child_node.simulate();
                child_node.backpropagate(new_score, number_of_simulations);
            }
            //
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
            //
            if (elapsed_seconds > this->max_time) {break;}
        }
        return true;
    }

    std::vector<std::vector<int>> mctser() {
        mcts_counter = 0;
        //
        bool monte_carlo=mcts();
        //
        if (monte_carlo) {
            std::cout <<"COUNTER: ";
            std::cout << mcts_counter << std::endl;
            //
            std::vector<std::vector<int>> best_move;
            int highest_number_of_visits = -1;
            //
            for (MCTSNode& root_node_child : root_node.children) {
                if (root_node_child.visits > highest_number_of_visits) {
                    best_move = root_node_child.board;
                    highest_number_of_visits = root_node_child.visits;
                }
            }
            return best_move;
        }
        else {std::vector<std::vector<int>> empty_vector; return empty_vector;}
        
    }

    std::vector<std::vector<int>> get_move(std::vector<std::vector<int>> board) {
        return mctser();
    }

};

//

int max_turns=70;

class Schach {
public:
    Schach() : board(), turn(1) {
        board={
            {-7, -2, -3, -5, -8, -3, -2, -7},
            {-1, -1, -1, -1, -1, -1, -1, -1},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {1, 1, 1, 1, 1, 1, 1, 1},
            {7, 2, 3, 5, 8, 3, 2, 7}
        };
    }

    int play() {
        int current = 1;

        while (true) {
            //-----------------------------------------
            HumanPlayer player_1(6);
            MCTSPlayer player_2(-6, this->board);
            //-----------------------------------------
            std::cout<<this->turn<<std::endl;
            print_board(this->board);
            //
            std::vector<std::vector<int>> new_board;
            if (current==1) {
                std::cout <<"k ist am Zug"<<std::endl;
                std::vector<std::vector<int>> board_copy = deepcopy(this->board);
                new_board = player_1.get_move(board_copy);
            }
            else if (current==2) {
                std::cout<<"K ist am Zug"<<std::endl;
                std::vector<std::vector<int>> board_copy = deepcopy(this->board);
                new_board = player_2.get_move(board_copy);
            }
            if (!(new_board.empty())) {this->board = new_board;}
            else {
                bool king_captured = false;
                int other;
                int this_players_token;
                //
                if(current == 1) {other = -6;}
                else {other = 6;}
                if (current == 1) {this_players_token = 6;}
                else {this_players_token = -6;}
                //
                std::vector<std::vector<std::vector<int>>> children = generate_children(this->board, other);
                for (const auto& child : children) {
                    if (verloren(child, this_players_token)) {
                        king_captured = true;
                    }
                }
                if (!king_captured) {
                    print_board(this->board);
                    std::cout << "UNENTSCHIEDEN" << std::endl;
                    return 0;
                }
                else {
                    if (this_players_token == 6) {
                        print_board(this->board);
                        std::cout << "K HAT GEWONNEN" << std::endl;
                        return -1;
                    }
                    else {
                        print_board(this->board);
                        std::cout << "k HAT GEWONNEN" << std::endl;
                        return 1;
                    }
                }
            }
            //
            if (this->turn==max_turns) {print_board(this->board); std::cout<<"UNENTSCHIEDEN"<<std::endl; return 0;}
            //
            if (current==1) {current = 2;}
            else {current = 1;}
            this->turn += 1;
        }
    }

private:
    std::vector<std::vector<int>> board;
    int turn;
};

//

void spielen(int z) {
    std::cout<<"NEUES SPIEL"<<std::endl;
    int k_wins=0;
    int K_wins=0;
    int unentschieden=0;
    //
    for (int i=0; i<z; ++i) {
        Schach game;
        int r = game.play();
        if (r==1) {k_wins+=1;}
        else if (r==-1) {K_wins+=1;}
        else if (r==0) {unentschieden+=1;}
        std::cout<<"k: "<<k_wins<<std::endl;
        std::cout<<"K: "<<K_wins<<std::endl;
        std::cout<<"-: "<<unentschieden<<std::endl;
    }
    std::cout<<"FERTIG"<<std::endl;
}

//

int main() {
    srand(time(0)); //seed
    spielen(3);
}

//

//sort?
//wieso deepc? play
//wieso board_0 bei minimaxerer
//wieso get_move(BOARD)? board bei humanp als param

//evaluatposfür geschlossenes Spiel anpassen.