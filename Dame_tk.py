import copy
import random
import time
import math
import tkinter as tk

#

open_window=None

def printboard(board):
    global open_window
    zahlen=[8,7,6,5,4,3,2,1]
    print('  a   b   c   d   e   f   g   h')
    print('---------------------------------')
    for i in range(8):
        print('I ', end='')
        for j in range(8):
            if board[i][j]==-1:
                print('O', end='')
            elif board[i][j]==1:
                print('X', end='')
            elif board[i][j]==-2:
                print('M', end='')
            elif board[i][j]==2:
                print('W', end='')
            else:
                print(' ', end='')
            print(' I ', end='')
        print(zahlen[i])
        print('---------------------------------') 
    #---------------------------------------------------
    if open_window!=None:
        open_window.destroy()
    root= tk.Tk()
    root.title("Dame")
    canvas = tk.Canvas(root, width=430, height=430)
    canvas.grid(row=0, column=0, padx=10, pady=10)
    #Board zeichnen
    column_letters = ["a", "b", "c", "d", "e", "f", "g", "h"]
    for j in range(8):
        x0 = j * 50 + 25
        y0 = 8 * 60 -60
        canvas.create_text(x0, y0, text=column_letters[j])
    for i in range(8):
        x0 = 8 * 50 +20
        y0 = i * 50 + 25
        canvas.create_text(x0, y0, text=str(8 - i))
    for i in range(8):
        for j in range(8):
            x0 = j * 50
            y0 = i * 50
            x1 = x0 + 50
            y1 = y0 + 50
            #Farben abwechseln
            if (i + j) % 2 == 0:
                canvas.create_rectangle(x0, y0, x1, y1, fill="beige")
            else:
                canvas.create_rectangle(x0, y0, x1, y1, fill="brown")
    for i in range(8):
        for j in range(8):
            x0 = j * 50
            y0 = i * 50
            x1 = x0 + 40
            y1 = y0 + 40
            # Check board values and draw circles
            if board[i][j]==1:
                canvas.create_oval(x0+10, y0+10, x1, y1, fill="white", outline="black")
            if board[i][j]==-1:
                canvas.create_oval(x0+10, y0+10, x1, y1, fill="black", outline="black")
            if board[i][j]==2:
                canvas.create_oval(x0+10, y0+10, x1, y1, fill="white", outline="black")
                canvas.create_oval(x0+15, y0+15, x1-5, y1-5, fill="white", outline="black")
                canvas.create_oval(x0+20, y0+20, x1-10, y1-10, fill="white", outline="black")
            if board[i][j]==-2:
                canvas.create_oval(x0+10, y0+10, x1, y1, fill="black", outline="black")
                canvas.create_oval(x0+15, y0+15, x1-5, y1-5, fill="black", outline="white")
                canvas.create_oval(x0+20, y0+20, x1-10, y1-10, fill="black", outline="white")
    #
    root.update()
    open_window=root

#

def keine_zugmoeglichkeiten(pos,player):
    if generate_children(pos,player)==[]:
        return True
    else:
        return False

def verloren1(pos,player):
    if player==1:
        player2=2
    else:
        player2=-2
    eval=True
    for y in range(len(pos)):
        for x in range(len(pos)):
            if pos[y][x]==player or player2:
                eval=False
                break
    return eval

def verloren2(pos,player):
    eval=0
    if player==1:
        player2=2
    else:
        player2=-2
    for sl in range(len(pos)):
        for o in range(pos[sl].count(player)):
            eval=eval+1
        for p in range(pos[sl].count(player2)):
            eval=eval+1
    if eval==0:
        return True
    elif keine_zugmoeglichkeiten(pos,player):
        return True
    else:
        return False

#

children_schlagen_XO=[]
children_schlagen_WM=[]

def generate_children(position, player):
    children1=[]
    children2=[]
    boardcopy = copy.deepcopy(position)
    for y in range(8):
        for x in range(8):
            if player==1:
                if boardcopy[y][x] == 1:
                    if y-1>-1 and x-1>-1 and boardcopy[y-1][x-1]==0:
                        boardcopy[y-1][x-1]=1
                        boardcopy[y][x]=0
                        if y-1==0:
                            boardcopy[y-1][x-1]=2
                            children1.append(boardcopy)
                        else:
                            children2.append(boardcopy)
                        boardcopy = copy.deepcopy(position)
                    if y-1>-1 and x+1<8 and boardcopy[y-1][x+ 1]==0:
                        boardcopy[y-1][x+ 1]=1
                        boardcopy[y][x]=0
                        if y-1==0:
                            boardcopy[y-1][x+1]=2
                            children1.append(boardcopy)
                        else:
                            children2.append(boardcopy)
                        boardcopy = copy.deepcopy(position)
                    else:
                        pass
                    #
                    for h in gen_children_schlagen(y,x,boardcopy,1,True):
                        children1.append(h)
                #
                elif boardcopy[y][x] == 2:
                    for r in generate_children_WM(y,x,boardcopy,2):
                        children1.append(r)
            elif player==-1:
                if boardcopy[y][x] == -1:
                    if y+ 1<8 and x-1>-1 and  boardcopy[y+ 1][x-1]==0:
                        boardcopy[y+1][x-1]=-1
                        boardcopy[y][x]=0
                        if y+1==7:
                            boardcopy[y+1][x-1]=-2
                            children1.append(boardcopy)
                        else:
                            children2.append(boardcopy)
                        boardcopy = copy.deepcopy(position)
                    if y+ 1<8 and x+ 1<8 and boardcopy[y+ 1][x+ 1]==0:
                        boardcopy[y+ 1][x+ 1]=-1
                        boardcopy[y][x]=0
                        if y+1==7:
                            boardcopy[y+1][x+1]=-2
                            children1.append(boardcopy)
                        else:
                            children2.append(boardcopy)
                        boardcopy = copy.deepcopy(position)
                    else:
                        pass
                    #
                    for h in gen_children_schlagen(y,x,boardcopy,-1,True):
                        children1.append(h)
                #
                elif boardcopy[y][x] == -2:
                    for r in generate_children_WM(y,x,boardcopy,-2):
                        children1.append(r)
    #
    children1.extend(children2)
    #
    return children1

def gen_children_schlagen(y,x,position,player,new):
    global children_schlagen_XO
    if new:
        children_schlagen_XO=[]
    boardcopy = copy.deepcopy(position)
    if player==1:
        if y-2>-1 and x-2>-1 and  boardcopy[y-2][x-2]==0:
            if boardcopy[y-1][x-1]<0:
                boardcopy[y-1][x-1]=0
                boardcopy[y-2][x-2]=1
                boardcopy[y][x]=0
                if y-2==0:
                    boardcopy[y-2][x-2]=2
                children_schlagen_XO.append(copy.deepcopy(boardcopy))
                gen_children_schlagen(y-2,x-2,boardcopy,player,False)
                boardcopy = copy.deepcopy(position)
        if y-2>-1 and x+ 2<8 and  boardcopy[y-2][x+2]==0:
            if boardcopy[y-1][x+ 1]<0:
                boardcopy[y-1][x+1]=0
                boardcopy[y-2][x+2]=1
                boardcopy[y][x]=0
                if y-2==0:
                    boardcopy[y-2][x+2]=2
                children_schlagen_XO.append(copy.deepcopy(boardcopy))
                gen_children_schlagen(y-2,x+2,boardcopy,player,False)
                boardcopy = copy.deepcopy(position)
    elif player==-1:
        if y+ 2<8 and x-2>-1 and boardcopy[y+2][x-2]==0:
            if boardcopy[y+ 1][x-1]>0:
                boardcopy[y+1][x-1]=0
                boardcopy[y+2][x-2]=-1
                boardcopy[y][x]=0
                if y+2==7:
                    boardcopy[y+2][x-2]=-2
                children_schlagen_XO.append(copy.deepcopy(boardcopy))
                gen_children_schlagen(y+2,x-2,boardcopy,player,False)
                boardcopy = copy.deepcopy(position)
        if y+ 2<8 and x+ 2<8 and boardcopy[y+2][x+2]==0:
            if boardcopy[y+ 1][x+ 1]>0:
                boardcopy[y+1][x+1]=0
                boardcopy[y+2][x+2]=-1
                boardcopy[y][x]=0
                if y+2==7:
                    boardcopy[y+2][x+2]=-2
                children_schlagen_XO.append(copy.deepcopy(boardcopy))
                gen_children_schlagen(y+2,x+2,boardcopy,player,False)
                boardcopy = copy.deepcopy(position)
    children_schlagen_XO.reverse()
    return children_schlagen_XO

def gen_children_schlagen_WM(y,x,pos,player,new):
    global children_schlagen_WM
    if new:
        children_schlagen_WM=[]
    directions=[(1,1), (-1,1), (1,-1), (-1,-1)]
    boardcopy=copy.deepcopy(pos)
    if player==-2:
        for direction in directions:
            dy=direction[0]
            dx=direction[1]
            for o in range(1,8):
                if y+(1+o)*dy>7 or x+(1+o)*dx>7 or y+(1+o)*dy<0 or x+(1+o)*dx<0:
                    break
                if boardcopy[y+o*dy][x+o*dx]<0:
                    break
                if boardcopy[y+o*dy][x+o*dx]>0:
                    if boardcopy[y+(1+o)*dy][x+(1+o)*dx]==0:
                        boardcopy[y+(1+o)*dy][x+(1+o)*dx]=-2
                        boardcopy[y+o*dy][x+o*dx]=0
                        boardcopy[y][x]=0
                        children_schlagen_WM.append(boardcopy)
                        gen_children_schlagen_WM(y+(1+o)*dy,x+(1+o)*dx,boardcopy,-2,False)
                        boardcopy=copy.deepcopy(pos)
                        break
                    else:
                        break
    #
    if player==2:
        for direction in directions:
            dy=direction[0]
            dx=direction[1]
            for o in range(1,8):
                if y+(1+o)*dy>7 or x+(1+o)*dx>7 or y+(1+o)*dy<0 or x+(1+o)*dx<0:
                    break
                if boardcopy[y+o*dy][x+o*dx]>0:
                    break
                if boardcopy[y+o*dy][x+o*dx]<0:
                    if boardcopy[y+(1+o)*dy][x+(1+o)*dx]==0:
                        boardcopy[y+(1+o)*dy][x+(1+o)*dx]=2
                        boardcopy[y+o*dy][x+o*dx]=0
                        boardcopy[y][x]=0
                        children_schlagen_WM.append(boardcopy)
                        gen_children_schlagen_WM(y+(1+o)*dy,x+(1+o)*dx,boardcopy,2,False)
                        boardcopy=copy.deepcopy(pos)
                        break
                    else:
                        break
    #
    children_schlagen_WM.reverse()
    return children_schlagen_WM

def generate_children_WM(y,x,pos,player):
    directions=[(1,1), (-1,1), (1,-1), (-1,-1)]
    childrenWM1=[]
    childrenWM2=[]
    boardcopy=copy.deepcopy(pos)
    schlagen=False
    if player==-2:
        for direction in directions:
            dy=direction[0]
            dx=direction[1]
            for o in range(1,8):
                schlagen=False
                if y+o*dy>7 or x+o*dx>7 or y+o*dy<0 or x+o*dx<0:
                    break
                if boardcopy[y+o*dy][x+o*dx]<0:
                    break
                if boardcopy[y+o*dy][x+o*dx]>0:
                    if not y+(o+1)*dy>7 and not x+(o+1)*dx>7 and not y+(o+1)*dy<0 and not x+(o+1)*dx<0:
                        schlagen=True
                if boardcopy[y+o*dy][x+o*dx]==0:
                    boardcopy[y+o*dy][x+o*dx]=-2
                    boardcopy[y][x]=0
                    childrenWM2.append(boardcopy)
                    boardcopy=copy.deepcopy(pos)
                if schlagen:
                    if boardcopy[y+(o+1)*dy][x+(o+1)*dx]==0:
                        boardcopy[y+(o+1)*dy][x+(o+1)*dx]=-2
                        boardcopy[y][x]=0
                        boardcopy[y+o*dy][x+o*dx]=0
                        childrenWM1.append(boardcopy)
                        for r in gen_children_schlagen_WM(y+(o+1)*dy,x+(o+1)*dx,boardcopy,-2,True):
                            childrenWM1.append(r)
                        boardcopy=copy.deepcopy(pos)
                        schlagen=False
                        break
                    else:
                        break
    elif player==2:
        for direction in directions:
            dy=direction[0]
            dx=direction[1]
            for o in range(1,8):
                schlagen=False
                if y+o*dy>7 or x+o*dx>7 or y+o*dy<0 or x+o*dx<0:
                    break
                if boardcopy[y+o*dy][x+o*dx]>0:
                    break
                if boardcopy[y+o*dy][x+o*dx]<0:
                    if not y+(o+1)*dy>7 and not x+(o+1)*dx>7 and not y+(o+1)*dy<0 and not x+(o+1)*dx<0:
                        schlagen=True
                if boardcopy[y+o*dy][x+o*dx]==0:
                    boardcopy[y+o*dy][x+o*dx]=2
                    boardcopy[y][x]=0
                    childrenWM2.append(boardcopy)
                    boardcopy=copy.deepcopy(pos)
                if schlagen:
                    if boardcopy[y+(o+1)*dy][x+(o+1)*dx]==0:
                        boardcopy[y+(o+1)*dy][x+(o+1)*dx]=2
                        boardcopy[y][x]=0
                        boardcopy[y+o*dy][x+o*dx]=0
                        childrenWM1.append(boardcopy)
                        for r in gen_children_schlagen_WM(y+(o+1)*dy,x+(o+1)*dx,boardcopy,2,True):
                            childrenWM1.append(r)
                        boardcopy=copy.deepcopy(pos)
                        schlagen=False
                        break
                    else:
                        break
    #
    childrenWM1.extend(childrenWM2)
    return childrenWM1

#

schlagen_XO_c=3
schlagen_WM_c=5
WM_c=5

gorc_XO_schlagen_children=[]
gorc_XO_schlagen_children_delete=[]
gorc_WM_schlagen_children=[]
gorc_WM_schlagen_children_delete=[]

def gorcXO_schlagen(y,x,boardc,player,delete_list):
    geschlagen=False
    if player==1:
        if y-2>-1 and x-2>-1 and boardc[y-2][x-2]==0 and boardc[y-1][x-1]<0:
            geschlagen=True
            delete_list_1=[]
            delete_list_1.extend(delete_list)
            delete_list_1.append([y-1,x-1])
            gorcXO_schlagen(y-2,x-2,boardc,player,delete_list_1)
        if y-2>-1 and x+2<8 and  boardc[y-2][x+2]==0 and boardc[y-1][x+1]<0:
            geschlagen=True
            delete_list_2=[]
            delete_list_2.extend(delete_list)
            delete_list_2.append([y-1,x+1])
            gorcXO_schlagen(y-2,x+2,boardc,player,delete_list_2)
        if not geschlagen:
            gorc_XO_schlagen_children.append((y+1) * 10 + (x+1))
            gorc_XO_schlagen_children_delete.append(delete_list)
    elif player==-1:
        if y+2<8 and x-2>-1 and boardc[y+2][x-2]==0 and boardc[y+1][x-1]>0:
            geschlagen=True
            delete_list_1=[]
            delete_list_1.extend(delete_list)
            delete_list_1.append([y+1,x-1])
            gorcXO_schlagen(y+2,x-2,boardc,player,delete_list_1)
        if y+2<8 and x+2<8 and boardc[y+2][x+2]==0 and boardc[y+1][x+1]>0:
            geschlagen=True
            delete_list_2=[]
            delete_list_2.extend(delete_list)
            delete_list_2.append([y+1,x+1])
            gorcXO_schlagen(y+2,x+2,boardc,player,delete_list_2)
        if not geschlagen:
            gorc_XO_schlagen_children.append((y+1) * 10 + (x+1))
            gorc_XO_schlagen_children_delete.append(delete_list)

def gorcXO(y,x,boardc,player):
    #
    childrenXO=[]
    gorc_XO_schlagen_children.clear()
    gorc_XO_schlagen_children_delete.clear()
    #
    if player==1:
        #normal
        if y-1>-1 and x-1>-1 and boardc[y-1][x-1]==0:
            childrenXO.append(1)
        if y-1>-1 and x+ 1<8 and boardc[y-1][x+1]==0:
            childrenXO.append(2)
        #schlagen
        if y-2>-1 and x-2>-1 and  boardc[y-2][x-2]==0:
            if boardc[y-1][x-1]<0:
                gorcXO_schlagen(y,x,boardc,player,[])
        if y-2>-1 and x+ 2<8 and  boardc[y-2][x+2]==0:
            if boardc[y-1][x+1]<0:
                gorcXO_schlagen(y,x,boardc,player,[])
    elif player==-1:
        #normal
        if y+1<8 and x-1>-1 and  boardc[y+1][x-1]==0:
            childrenXO.append(1)
        if y+1<8 and x+ 1<8 and boardc[y+1][x+1]==0:
            childrenXO.append(2)
        #schlagen
        if y+2<8 and x-2>-1 and boardc[y+2][x-2]==0:
            if boardc[y+1][x-1]>0:
                gorcXO_schlagen(y,x,boardc,player,[])
        if y+2<8 and x+ 2<8 and boardc[y+2][x+2]==0:
            if boardc[y+1][x+1]>0:
                gorcXO_schlagen(y,x,boardc,player,[])
    #
    for i in range(schlagen_XO_c):
        childrenXO.extend(gorc_XO_schlagen_children)
    #
    if childrenXO==[]:
        return []
    else:
        n=random.choice(childrenXO)
        if player==1:
            if n==1:
                boardc[y][x]=0
                if y-1==0:
                    boardc[y-1][x-1]=2
                else:
                    boardc[y-1][x-1]=1
                return boardc
            elif n==2:
                boardc[y][x]=0
                if y-1==0:
                    boardc[y-1][x+1]=2
                else:
                    boardc[y-1][x+1]=1
                return boardc
            else: #schlagen
                n_y = n//10-1
                n_x = n%10-1
                boardc[y][x]=0
                if n_y==0:
                    boardc[n_y][n_x]=2
                else:
                    boardc[n_y][n_x]=1
                delete=gorc_XO_schlagen_children_delete[gorc_XO_schlagen_children.index(n)]
                for feld in delete:
                    boardc[feld[0]][feld[1]]=0
                return boardc
        elif player==-1:
            if n==1:
                boardc[y][x]=0
                if y+1==7:
                    boardc[y-1][x-1]=-2
                else:
                    boardc[y+1][x-1]=-1
                return boardc
            elif n==2:
                boardc[y][x]=0
                if y+1==7:
                    boardc[y-1][x+1]=-2
                else:
                    boardc[y+1][x+1]=-1
                return boardc
            else: #schlagen
                n_y = n//10-1
                n_x = n%10-1
                boardc[y][x]=0
                if n_y==0:
                    boardc[n_y][n_x]=-2
                else:
                    boardc[n_y][n_x]=-1
                delete=gorc_XO_schlagen_children_delete[gorc_XO_schlagen_children.index(n)]
                for feld in delete:
                    boardc[feld[0]][feld[1]]=0
                return boardc   
 
def gorcWM_schlagen(y,x,boardc,player,delete_list):
    geschlagen=False
    directions=[(1,1), (-1,1), (1,-1), (-1,-1)]
    if player==2:
        for direction in directions:
            dy=direction[0]
            dx=direction[1]
            for i in range(1,8):
                if y+i*dy>7 or x+i*dx>7 or y+i*dy<0 or x+i*dx<0:
                    break
                if boardc[y+i*dy][x+i*dx]>0:
                    break
                if boardc[y+i*dy][x+i*dx]<0:
                    r=False
                    for piece in delete_list: #um or/ul inf recursion zu verhindern
                        if piece[0]==y+i*dy and piece[1]==x+i*dx:
                            r=True
                            break
                    if r:
                        continue
                    if not y+(i+1)*dy>7 and not x+(i+1)*dx>7 and not y+(i+1)*dy<0 and not x+(i+1)*dx<0 and boardc[y+(i+1)*dy][x+(i+1)*dx]==0:
                        geschlagen=True
                        delete_list_1=[]
                        delete_list_1.extend(delete_list)
                        delete_list_1.append([y+i*dy, x+i*dx])
                        gorcWM_schlagen(y+(i+1)*dy,x+(i+1)*dx,boardc,player,delete_list_1)#
                        break
                    else:
                        break
    #
    elif player==-2:
        for direction in directions:
            dy=direction[0]
            dx=direction[1]
            for i in range(1,8):
                if y+i*dy>7 or x+i*dx>7 or y+i*dy<0 or x+i*dx<0:
                    break
                if boardc[y+i*dy][x+i*dx]<0:
                    break
                if boardc[y+i*dy][x+i*dx]>0:
                    r=False
                    for piece in delete_list: #um or/ul inf recursion zu verhindern
                        if piece[0]==y+i*dy and piece[1]==x+i*dx:
                            r=True
                            break
                    if r:
                        continue
                    if not y+(i+1)*dy>7 and not x+(i+1)*dx>7 and not y+(i+1)*dy<0 and not x+(i+1)*dx<0 and boardc[y+(i+1)*dy][x+(i+1)*dx]==0:
                        geschlagen=True
                        delete_list_1=[]
                        delete_list_1.extend(delete_list)
                        delete_list_1.append([y+i*dy, x+i*dx])
                        gorcWM_schlagen(y+(i+1)*dy,x+(i+1)*dx,boardc,player,delete_list_1)#
                        break
                    else:
                        break
    #
    if not geschlagen:
        gorc_WM_schlagen_children.append(((y+1) * 10) + (((x+1) + 100)))
        gorc_WM_schlagen_children_delete.append(delete_list)

def gorcWM(y,x,boardc,player):
    #
    childrenWM=[]
    directions=[(1,1,10), (1,-1,20), (-1,1,30), (-1,-1,40)]
    gorc_WM_schlagen_children.clear()
    gorc_WM_schlagen_children_delete.clear()
    #
    if player==2:
        for direction in directions:
            dy=direction[0]
            dx=direction[1]
            d=direction[2]
            for i in range(1,8):
                if y+i*dy>7 or x+i*dx>7 or y+i*dy<0 or x+i*dx<0:
                    break
                if boardc[y+i*dy][x+i*dx]>0:
                    break
                if boardc[y+i*dy][x+i*dx]==0:
                    childrenWM.append(d+i)
                if boardc[y+i*dy][x+i*dx]<0:
                    if not y+(i+1)*dy>7 and not x+(i+1)*dx>7 and not y+(i+1)*dy<0 and not x+(i+1)*dx<0:
                        if boardc[y+(i+1)*dy][x+(i+1)*dx]==0:
                            gorcWM_schlagen(y,x,boardc,player,[])
                            break
                        else:
                            break
                    else:
                        break
    #
    elif player==-2:
        for direction in directions:
            dy=direction[0]
            dx=direction[1]
            d=direction[2]
            for i in range(1,8):
                if y+i*dy>7 or x+i*dx>7 or y+i*dy<0 or x+i*dx<0:
                    break
                if boardc[y+i*dy][x+i*dx]<0:
                    break
                if boardc[y+i*dy][x+i*dx]==0:
                    childrenWM.append(d+i)
                if boardc[y+i*dy][x+i*dx]>0:
                    if not y+(i+1)*dy>7 and not x+(i+1)*dx>7 and not y+(i+1)*dy<0 and not x+(i+1)*dx<0:
                        if boardc[y+(i+1)*dy][x+(i+1)*dx]==0:
                            gorcWM_schlagen(y,x,boardc,player,[])
                            break
                        else:
                            break
                    else:
                        break   
    #
    for i in range(schlagen_WM_c):
        childrenWM.extend(gorc_WM_schlagen_children)
    #
    if childrenWM==[]:
        return []
    else:
        n=random.choice(childrenWM)
        #ur
        if n>10 and n<20:
            boardc[y][x]=0
            boardc[y+(n-10)][x+(n-10)]=player
            return boardc
        #ul
        elif n>20 and n<30:
            boardc[y][x]=0
            boardc[y+(n-20)][x-(n-20)]=player
            return boardc
        #or
        elif n>30 and n<40:
            boardc[y][x]=0
            boardc[y-(n-30)][x+(n-30)]=player
            return boardc
        #ol
        elif n>40 and n<50:
            boardc[y][x]=0
            boardc[y-(n-40)][x-(n-40)]=player
            return boardc
        #schlagen
        elif n>100:
            n_y = ((n-100)//10)-1
            n_x = ((n-100)%10)-1
            #
            boardc[y][x]=0
            boardc[n_y][n_x]=player
            delete=gorc_WM_schlagen_children_delete[gorc_WM_schlagen_children.index(n)]
            for feld in delete:
                boardc[feld[0]][feld[1]]=0
            return boardc

def generate_one_random_child(position, player):#pick rand piece, then pick rand move
    boardcopy = copy.deepcopy(position)
    #
    piecesy=[]
    piecesx=[]
    if player==1:
        for y in range(8):
            for x in range(8):
                if boardcopy[y][x]==1:
                    piecesy.append(y)
                    piecesx.append(x)
                elif boardcopy[y][x]==2:
                    for i in range(WM_c):
                        piecesy.append(y)
                        piecesx.append(x)
    elif player==-1:
        for y in range(8):
            for x in range(8):
                if boardcopy[y][x]==-1:
                    piecesy.append(y)
                    piecesx.append(x)
                elif boardcopy[y][x]==-2:
                    for i in range(WM_c):
                        piecesy.append(y)
                        piecesx.append(x)
    #
    if piecesx==[]:
        return []
    #
    for i in range(30): #für wenn figuren vorhanden sind, aber keinen zug
        child=[]
        n = random.randint(0, len(piecesy) - 1)
        y = piecesy[n]
        x = piecesx[n]
        #
        if player==1:
            if boardcopy[y][x]==1:
                child=gorcXO(y,x,boardcopy,1)
            elif boardcopy[y][x]==2:
                pass
                child=gorcWM(y,x,boardcopy,2)
        elif player==-1:
            if boardcopy[y][x]==-1:
                child=gorcXO(y,x,boardcopy,-1)
            elif boardcopy[y][x]==-2:
                pass
                child=gorcWM(y,x,boardcopy,-2)
        #
        if child!=[]: 
            break
    #
    if child!=[]: 
        return child
    else:
        return []

#

X_matrix=[2,0,0,1,2,3,4,5]

O_matrix=[5,4,3,2,1,0,0,2]

def evaluate_position(pos,player):
    eval=0
    if player==1:
        anz_X=0
        anz_O=0
        anz_W=0
        anz_M=0
        for l in range(len(pos)):
            for o in range(pos[l].count(1)):
                eval+=11
                anz_X+=1
                eval+=X_matrix[l]
            for o in range(pos[l].count(-1)):
                eval+=-9
                anz_O+=1
                eval+=-O_matrix[l]
            for o in range(pos[l].count(2)):
                eval+=49
                anz_W+=1
            for o in range(pos[l].count(-2)):
                eval+=-51
                anz_M+=1
        if anz_X==0 and anz_W==0:
            eval+=-8888
        elif anz_O==0 and anz_M==0:
            eval+=8888
        return eval
    elif player==-1:
        anz_X=0
        anz_O=0
        anz_W=0
        anz_M=0
        for l in range(len(pos)):
            for o in range(pos[l].count(1)):
                eval+=-9
                anz_X=anz_X+1
                eval+=-X_matrix[l]
            for o in range(pos[l].count(-1)):
                eval+=11
                anz_O+=1
                eval+=O_matrix[l]
            for o in range(pos[l].count(2)):
                eval+=-51
                anz_W+=1
            for o in range(pos[l].count(-2)):
                eval+=49
                anz_M+=1
        if anz_X==0 and anz_W==0:
            eval+=8888
        elif anz_O==0 and anz_M==0:
            eval+=8888
        return eval

#

class Dame():
    def __init__(self):
        self.board = []
        self.turn=0
        self.players=[]
        self.max_turns=50
    
    def play(self):
        self.board = [
            [0,-1,0,-1,0,-1,0,-1],
            [-1,0,-1,0,-1,0,-1,0],
            [0,-1,0,-1,0,-1,0,-1],
            [0,0,0,0,0,0,0,0],
            [0,0,0,0,0,0,0,0],
            [1,0,1,0,1,0,1,0],
            [0,1,0,1,0,1,0,1],
            [1,0,1,0,1,0,1,0]
        ]
        #
        self.turn=0
        #
        # Spieler:innen vorbereiten
        # X spielt immer zuerst
        self.players.clear()
        self.players.append(HumanPlayer(1))
        self.players.append(MinimaxPlayer(-1))
        #
        current=0
        while True:
            if self.turn==self.max_turns:
                break
            print(self.turn)
            printboard(self.board)
            player = self.players[current]
            if player.token==1:
                istamzug='X'
            else:
                istamzug='O'
            print(istamzug, ' ist am Zug')
            new_board=player.get_move(copy.deepcopy(self.board))
            if new_board!=[]:
                self.board=new_board
            current = (current + 1) % 2
            self.turn+=1
            if verloren2(self.board,-1)or verloren2(self.board,1):
                break
        printboard(self.board)
        if verloren2(self.board,1):
            print('O HAT GEWONNEN')
            return 'O'
        elif verloren2(self.board,-1):
            print('X HAT GEWONNEN')
            return 'X'
        else:
            print('UNENTSCHIEDEN')
            return ' '

#

x_inputs=['a', 'b','c', 'd', 'e', 'f', 'g', 'h']
y_inputs=[8,7,6,5,4,3,2,1]

class HumanPlayer():
    def __init__(self, token):
        self.token = token

    #

    def eingabe(self):
        while True:
            try:
                vx_s = str(input('von x: '))
                vy_i = int(input('von y: '))
                zx_s = str(input('zu x: '))
                zy_i = int(input('zu y: '))
                vy=y_inputs.index(vy_i)
                zy=y_inputs.index(zy_i)
            except:
                print('EINGABE NICHT KORREKT1')
                continue

            if vy<8 and vy>-1 and zy<8 and zy>-1 and vx_s in x_inputs and zx_s in x_inputs:
                vx = x_inputs.index(vx_s)
                zx = x_inputs.index(zx_s)
                return [vy, vx, zy, zx]
            else:
                print('EINGABE NICHT KORREKT1')
                continue

    def eingabe_schlagen_XO(self,vy,vx,pos):
        #schlagen muss moeglich sein
        while True:
            korrekt=False
            try:
                zx_s = str(input('zu x: '))
                zy_i = int(input('zu y: '))
                zx = x_inputs.index(zx_s)
                zy=y_inputs.index(zy_i)
            except:
                print('EINGABE NICHT KORREKT3')
                continue
            #
            if zx==vx and zy==vy:
                korrekt=True
            #
            if zy<8 and zy>-1 and zx<8 and zx>-1:
                if self.token==1:
                    if zy==vy-2 and zx==vx-2 and pos[vy][vx]==1 and pos[zy][zx]==0 and pos[vy-1][vx-1]<0:
                        korrekt=True
                    if zy==vy-2 and zx==vx+2 and pos[vy][vx]==1 and pos[zy][zx]==0 and pos[vy-1][vx+1]<0:
                        korrekt=True
                elif self.token==-1:
                    if zy==vy+2 and zx==vx-2 and pos[vy][vx]==-1 and pos[zy][zx]==0 and pos[vy+1][vx-1]>0:
                        korrekt=True
                    if zy==vy+2 and zx==vx+2 and pos[vy][vx]==-1 and pos[zy][zx]==0 and pos[vy+1][vx+1]>0:
                        korrekt=True
            #
            if korrekt:
                return [zy,zx]
            else:
                print('EINGABE NICHT KORREKT3')
                continue

    def eingabe_schlagen_WM(self,vy,vx,pos):
        #schlagen muss moeglich sein
        while True:
            korrekt=False
            try:
                zx_s = int(input('zu x: '))
                zy_i = int(input('zu y: '))
                zx = x_inputs.index(zx_s)
                zy=y_inputs.index(zy_i)
            except:
                print('EINGABE NICHT KORREKT4')
                continue
            #
            if zx==vx and zy==vy:#um nicht mehr zu schlagen
                korrekt=True
            #
            if zy<8 and zy>-1 and zx<8 and zx>-1:
                #
                if self.token==1:
                    for i in range(7):
                        if vy+2+i>7 or vx+2+i>7:
                            break
                        if pos[vy+1+i][vx+1+i]>0:
                            break
                        if pos[vy+1+i][vx+1+i]<0 and pos[vy+2+i][vx+2+i]==0 and vy+2+i==zy and vx+2+i==zx:
                            korrekt=True
                            break
                    if not korrekt:
                        for i in range(7):
                            if vy+2+i>7 or vx-2-i<0:
                                break
                            if pos[vy+1+i][vx-1-i]>0:
                                break
                            if pos[vy+1+i][vx-1-i]<0 and pos[vy+2+i][vx-2-i]==0 and vy+2+i==zy and vx-2-i==zx:
                                korrekt=True
                                break
                    if not korrekt:
                        for i in range(7):
                            if vy-2-i<0 or vx-2-i<0:
                                break
                            if pos[vy-1-i][vx-1-i]>0:
                                break
                            if pos[vy-1-i][vx-1-i]<0 and pos[vy-2-i][vx-2-i]==0 and vy-2-i==zy and vx-2-i==zx:
                                korrekt=True
                                break
                    if not korrekt:
                        for i in range(7):
                            if vy-2-i<0 or vx+2+i>7:
                                break
                            if pos[vy-1-i][vx+1+i]>0:
                                break
                            if pos[vy-1-i][vx+1+i]<0 and pos[vy-2-i][vx+2+i]==0 and vy-2-i==zy and vx+2+i==zx:
                                korrekt=True
                                break
                elif self.token==-1:
                    for i in range(7):
                        if vy+2+i>7 or vx+2+i>7:
                            break
                        if pos[vy+1+i][vx+1+i]<0:
                            break
                        if pos[vy+1+i][vx+1+i]>0 and pos[vy+2+i][vx+2+i]==0 and vy+2+i==zy and vx+2+i==zx:
                            korrekt=True
                            break
                    if not korrekt:
                        for i in range(7):
                            if vy+2+i>7 or vx-2-i<0:
                                break
                            if pos[vy+1+i][vx-1-i]<0:
                                break
                            if pos[vy+1+i][vx-1-i]>0 and pos[vy+2+i][vx-2-i]==0 and vy+2+i==zy and vx-2-i==zx:
                                korrekt=True
                                break
                    if not korrekt:
                        for i in range(7):
                            if vy-2-i<0 or vx-2-i<0:
                                break
                            if pos[vy-1-i][vx-1-i]<0:
                                break
                            if pos[vy-1-i][vx-1-i]>0 and pos[vy-2-i][vx-2-i]==0 and vy-2-i==zy and vx-2-i==zx:
                                korrekt=True
                                break
                    if not korrekt:
                        for i in range(7):
                            if vy-2-i<0 or vx+2+i>7:
                                break
                            if pos[vy-1-i][vx+1+i]<0:
                                break
                            if pos[vy-1-i][vx+1+i]>0 and pos[vy-2-i][vx+2+i]==0 and vy-2-i==zy and vx+2+i==zx:
                                korrekt=True
                                break
            #
            if korrekt:
                return [zy,zx]
            else:
                print('EINGABE NICHT KORREKT4')
                continue

    #

    def schlagen_moeglich_XO(self,y,x,pos):
        moeglich=False
        if self.token==1:
            if y-2>-1 and x-2>-1:
                if pos[y-2][x-2]==0 and pos[y-1][x-1]<0:
                    moeglich=True
            if y-2>-1 and x+2<8:
                if pos[y-2][x+2]==0 and pos[y-1][x+1]<0:
                    moeglich=True
        elif self.token==-1:
            if y+2<8 and x-2>-1:
                if pos[y+2][x-2]==0 and pos[y+1][x-1]>0:
                    moeglich=True
            if y+2<8 and x+2<8:
                if pos[y+2][x+2]==0 and pos[y+1][x+1]>0:
                    moeglich=True
        return moeglich

    def schlagen_moeglich_WM(self,y,x,pos):
        moeglich=False
        if self.token==1:
            for i in range(7):
                if y+2+i>7 or x+2+i>7:
                    break
                if pos[y+1+i][x+1+i]>0:
                    break
                if pos[y+1+i][x+1+i]<0:
                    if pos[y+2+i][x+2+i]==0:
                        moeglich=True
                        break
                    else:
                        break
            if not moeglich:
                for i in range(7):
                    if y-2-i<0 or x+2+i>7:
                        break
                    if pos[y-1-i][x+1+i]>0:
                        break
                    if pos[y-1-i][x+1+i]<0:
                        if pos[y-2-i][x+2+i]==0:
                            moeglich=True
                            break
                        else:
                            break
            if not moeglich:
                for i in range(7):
                    if y-2-i<0 or x-2-i<0:
                        print(y)
                        print(x)
                        print(y-2-i)
                        print(x-2-i)
                        print("jk")
                        break
                    if pos[y-1-i][x-1-i]>0:
                        break
                    if pos[y-1-i][x-1-i]<0:
                        if pos[y-2-i][x-2-i]==0:
                            moeglich=True
                            break
                        else:
                            break
            if not moeglich:
                for i in range(7):
                    if y+2+i>7 or x-2-i<0:
                        break
                    if pos[y+1+i][x-1-i]>0:
                        break
                    if pos[y+1+i][x-1-i]<0:
                        if pos[y+2+i][x-2-i]==0:
                            moeglich=True
                            break
                        else:
                            break
        elif self.token==-1:
            for i in range(7):
                if y+2+i>7 or x+2+i>7:
                    break
                if pos[y+1+i][x+1+i]<0:
                    break
                if pos[y+1+i][x+1+i]>0:
                    if pos[y+2+i][x+2+i]==0:
                        moeglich=True
                        break
                    else:
                        break
            if not moeglich:
                for i in range(7):
                    if y-2-i<0 or x+2+i>7:
                        break
                    if pos[y-1-i][x+1+i]<0:
                        break
                    if pos[y-1-i][x+1+i]>0:
                        if pos[y-2-i][x+2+i]==0:
                            moeglich=True
                            break
                        else:
                            break
            if not moeglich:
                for i in range(7):
                    if y-2-i<0 or x-2-i<0:
                        break
                    if pos[y-1-i][x-1-i]<0:
                        break
                    if pos[y-1-i][x-1-i]>0:
                        if pos[y-2-i][x-2-i]==0:
                            moeglich=True
                            break
                        else:
                            break
            if not moeglich:
                for i in range(7):
                    if y+2+i>7 or x-2-i<0:
                        break
                    if pos[y+1+i][x-1-i]<0:
                        break
                    if pos[y+1+i][x-1-i]>0:
                        if pos[y+2+i][x-2-i]==0:
                            moeglich=True
                            break
                        else:
                            break
        #
        return moeglich

    #

    def player_schlagen_chain_XO(self,vy,vx,pos):
        #erstes Mal schlagen bei playerschlagen
        if self.schlagen_moeglich_XO(vy,vx,pos):
            printboard(pos)
            #
            input_move=self.eingabe_schlagen_XO(vy,vx,pos)
            zy,zx=input_move
            #
            if zx==vx and vy==zy:#um nicht mehr zu schlagen
                pass
            #
            if self.token==1:
                if zy==vy-2 and zx==vx-2: #and pos[vy][vx]==1 and pos[zy][zx]==0 and pos[vy-1][vx-1]<0: 
                    pos[vy][vx]=0
                    pos[zy][zx]=1
                    if zy==0:
                        pos[zy][zx]=2
                    pos[vy-1][vx-1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_XO(vy,vx,pos)
                #
                elif zy==vy-2 and zx==vx+2: #and pos[vy][vx]==1 and pos[zy][zx]==0 and pos[vy-1][vx+1]<0:
                    pos[vy][vx]=0
                    pos[zy][zx]=1
                    if zy==0:
                        pos[zy][zx]=2
                    pos[vy-1][vx+1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_XO(vy,vx,pos)
            #
            elif self.token==-1:
                if zy==vy+2 and zx==vx-2: #and pos[vy][vx]==-1 and pos[zy][zx]==0 and pos[vy+1][vx-1]>0:
                    pos[vy][vx]=0
                    pos[zy][zx]=-1
                    if zy==0:
                        pos[zy][zx]=-2
                    pos[vy+1][vx-1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_XO(vy,vx,pos)
                #
                elif zy==vy+2 and zx==vx+2: #and pos[vy][vx]==-1 and pos[zy][zx]==0 and pos[vy+1][vx+1]>0:
                    pos[vy][vx]=0
                    pos[zy][zx]=-1
                    if zy==0:
                        pos[zy][zx]=-2
                    pos[vy+1][vx+1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_XO(vy,vx,pos)
        #
        return pos

    def player_schlagen_chain_WM(self,vy,vx,pos):
        #erstes Mal schlagen bei playerschlagen
        if self.schlagen_moeglich_WM(vy,vx,pos):
            printboard(pos)
            #
            input_move=self.eingabe_schlagen_WM(vy,vx,pos)
            zy,zx=input_move
            #
            if zx==vx and vy==zy:#um nicht mehr zu schlagen
                pass
            #
            if self.token==1:
                if zy<vy and zx<vx:
                    pos[vy][vx]=0
                    pos[zy][zx]=2
                    pos[zy+1][zx+1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_WM(vy,vx,pos)
                elif zy>vy and zx<vx:
                    pos[vy][vx]=0
                    pos[zy][zx]=2
                    pos[zy-1][zx+1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_WM(vy,vx,pos)
                elif zy<vy and zx>vx:
                    pos[vy][vx]=0
                    pos[zy][zx]=2
                    pos[zy+1][zx-1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_WM(vy,vx,pos)
                elif zy>vy and zx>vx:
                    pos[vy][vx]=0
                    pos[zy][zx]=2
                    pos[zy-1][zx+1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_WM(vy,vx,pos)
            elif self.token==-1:
                if zy<vy and zx<vx:
                    pos[vy][vx]=0
                    pos[zy][zx]=-2
                    pos[zy+1][zx+1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_WM(vy,vx,pos)
                elif zy>vy and zx<vx:
                    pos[vy][vx]=0
                    pos[zy][zx]=-2
                    pos[zy-1][zx+1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_WM(vy,vx,pos)
                elif zy<vy and zx>vx:
                    pos[vy][vx]=0
                    pos[zy][zx]=-2
                    pos[zy+1][zx-1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_WM(vy,vx,pos)
                elif zy>vy and zx>vx:
                    pos[vy][vx]=0
                    pos[zy][zx]=-2
                    pos[zy-1][zx+1]=0
                    printboard(pos)
                    #
                    vy = zy
                    vx = zx
                    self.player_schlagen_chain_WM(vy,vx,pos)
        #
        return pos

    #

    def player(self,pos):
        while True:
            input_move = self.eingabe()
            vy, vx, zy, zx = input_move
            #
            if self.token==1:
                #X normal------------------------------
                if zy==vy-1 and zx==vx-1 and pos[vy][vx]==1 and pos[zy][zx]==0:
                    pos[vy][vx]=0
                    pos[zy][zx]=1
                    if zy==0:
                        pos[zy][zx]=2
                    return pos
                elif zy==vy-1 and zx==vx+1 and pos[vy][vx]==1 and pos[zy][zx]==0:
                    pos[vy][vx]=0
                    pos[zy][zx]=1
                    if zy==0:
                        pos[zy][zx]=2
                    return pos
                #X schlagen------------------------------
                elif zy==vy-2 and zx==vx-2 and pos[vy][vx]==1 and pos[zy][zx]==0 and pos[vy-1][vx-1]<0:
                    pos[vy][vx]=0
                    pos[zy][zx]=1
                    if zy==0:
                        pos[zy][zx]=2
                    pos[vy-1][vx-1]=0
                    #
                    return self.player_schlagen_chain_XO(zy,zx,pos)
                elif zy==vy-2 and zx==vx+2 and pos[vy][vx]==1 and pos[zy][zx]==0 and pos[vy-1][vx+1]<0:
                    pos[vy][vx]=0
                    pos[zy][zx]=1
                    if zy==0:
                        pos[zy][zx]=2
                    pos[vy-1][vx+1]=0
                    #
                    return self.player_schlagen_chain_XO(zy,zx,pos)
                #W normal+erstes Mal schlagen------------------------------
                elif pos[vy][vx]==2 and pos[zy][zx]==0 and zy<vy and zx<vx:
                    for i in range(7):
                        if vy-i-1<0 or vx-i-1<0:#ende von board erreicht: nicht korrekt
                            break
                        if vy-i-1==zy and vx-i-1==zx:#ziel erreicht: normal bew
                            pos[vy][vx]=0
                            pos[zy][zx]=2
                            return pos
                        if pos[vy-i-1][vx-i-1]>0:#weg von eigene figur blockiert: nicht korrekt
                            break
                        if pos[vy-i-1][vx-i-1]<0:#auf gegnerische figur gestossen: schlagen oder nicht korrekt?
                            if not vy-i-2<0 or vx-i-2<0:
                                if vy-i-2==zy and vx-i-2==zx:#schlagen
                                    pos[vy][vx]=0
                                    pos[zy][zx]=2
                                    pos[vy-i-1][vx-i-1]=0
                                    return self.player_schlagen_chain_WM(zy,zx,pos)
                                else:
                                    break
                            else:
                                break
                elif pos[vy][vx]==2 and pos[zy][zx]==0 and zy>vy and zx<vx:
                    for i in range(7):
                        if vy+i+1>7 or vx-i-1<0:#ende von board erreicht: nicht korrekt
                            break
                        if vy+i+1==zy and vx-i-1==zx:#ziel erreicht: normal bew
                            pos[vy][vx]=0
                            pos[zy][zx]=2
                            return pos
                        if pos[vy+i+1][vx-i-1]>0:#weg von eigene figur blockiert: nicht korrekt
                            break
                        if pos[vy+i+1][vx-i-1]<0:#auf gegnerische figur gestossen: schlagen oder nicht korrekt?
                            if not vy+i+2>7 or vx-i-2<0:
                                if vy+i+2==zy and vx-i-2==zx:#schlagen
                                    pos[vy][vx]=0
                                    pos[zy][zx]=2
                                    pos[vy+i+1][vx-i-1]=0
                                    return self.player_schlagen_chain_WM(zy,zx,pos)
                                else:
                                    break
                            else:
                                break
                elif pos[vy][vx]==2 and pos[zy][zx]==0 and zy<vy and zx>vx:
                    for i in range(7):
                        if vy-i-1<0 or vx+i+1>7:#ende von board erreicht: nicht korrekt
                            break
                        if vy-i-1==zy and vx+i+1==zx:#ziel erreicht: normal bew
                            pos[vy][vx]=0
                            pos[zy][zx]=2
                            return pos
                        if pos[vy-i-1][vx+i+1]>0:#weg von eigene figur blockiert: nicht korrekt
                            break
                        if pos[vy-i-1][vx+i+1]<0:#auf gegnerische figur gestossen: schlagen oder nicht korrekt?
                            if not vy-i-2<0 or vx+i+2>7:
                                if vy-i-2==zy and vx+i+2==zx:#schlagen
                                    pos[vy][vx]=0
                                    pos[zy][zx]=2
                                    pos[vy-i-1][vx+i+1]=0
                                    return self.player_schlagen_chain_WM(zy,zx,pos)
                                else:
                                    break
                            else:
                                break
                elif pos[vy][vx]==2 and pos[zy][zx]==0 and zy>vy and zx>vx:
                    for i in range(7):
                        if vy+i+1>7 or vx+i+1>7:#ende von board erreicht: nicht korrekt
                            break
                        if vy+i+1==zy and vx+i+1==zx:#ziel erreicht: normal bew
                            pos[vy][vx]=0
                            pos[zy][zx]=2
                            return pos
                        if pos[vy+i+1][vx+i+1]>0:#weg von eigene figur blockiert: nicht korrekt
                            break
                        if pos[vy+i+1][vx+i+1]<0:#auf gegnerische figur gestossen: schlagen oder nicht korrekt?
                            if not vy+i+2>7 or vx+i+2>7:
                                if vy+i+2==zy and vx+i+2==zx:#schlagen
                                    pos[vy][vx]=0
                                    pos[zy][zx]=2
                                    pos[vy+i+1][vx+i+1]=0
                                    return self.player_schlagen_chain_WM(zy,zx,pos)
                                else:
                                    break
                            else:
                                break
                #------------------------------
            elif self.token==-1:
                #O normal
                if zy==vy+1 and zx==vx-1 and pos[vy][vx]==-1 and pos[zy][zx]==0:
                    pos[vy][vx]=0
                    pos[zy][zx]=-1
                    if zy==7:
                        pos[zy][zx]=-2
                    return pos
                elif zy==vy+1 and zx==vx+1 and pos[vy][vx]==-1 and pos[zy][zx]==0:
                    pos[vy][vx]=0
                    pos[zy][zx]=-1
                    if zy==7:
                        pos[zy][zx]=-2
                    return pos
                #O schlagen------------------------------
                elif zy==vy+2 and zx==vx-2 and pos[vy][vx]==-1 and pos[zy][zx]==0 and pos[vy+1][vx-1]>0:
                    pos[vy][vx]=0
                    pos[zy][zx]=-1
                    if zy==0:
                        pos[zy][zx]=-2
                    pos[vy+1][vx-1]=0
                    #
                    return self.player_schlagen_chain_XO(zy,zx,pos)
                elif zy==vy+2 and zx==vx+2 and pos[vy][vx]==-1 and pos[zy][zx]==0 and pos[vy+1][vx+1]>0:
                    pos[vy][vx]=0
                    pos[zy][zx]=-1
                    if zy==0:
                        pos[zy][zx]=-2
                    pos[vy+1][vx+1]=0
                    #
                    return self.player_schlagen_chain_XO(zy,zx,pos)
                #M normal+erstes Mal schlagen------------------------------
                elif pos[vy][vx]==-2 and pos[zy][zx]==0 and zy<vy and zx<vx:
                    for i in range(7):
                        if vy-i-1<0 or vx-i-1<0:#ende von board erreicht: nicht korrekt
                            break
                        if vy-i-1==zy and vx-i-1==zx:#ziel erreicht: normal bew
                            pos[vy][vx]=0
                            pos[zy][zx]=-2
                            return pos
                        if pos[vy-i-1][vx-i-1]<0:#weg von eigene figur blockiert: nicht korrekt
                            break
                        if pos[vy-i-1][vx-i-1]>0:#auf gegnerische figur gestossen: schlagen oder nicht korrekt?
                            if not vy-i-2<0 or vx-i-2<0:
                                if vy-i-2==zy and vx-i-2==zx:#schlagen
                                    pos[vy][vx]=0
                                    pos[zy][zx]=-2
                                    pos[vy-i-1][vx-i-1]=0
                                    return self.player_schlagen_chain_WM(zy,zx,pos)
                                else:
                                    break
                            else:
                                break
                elif pos[vy][vx]==-2 and pos[zy][zx]==0 and zy>vy and zx<vx:
                    for i in range(7):
                        if vy+i+1>7 or vx-i-1<0:#ende von board erreicht: nicht korrekt
                            break
                        if vy+i+1==zy and vx-i-1==zx:#ziel erreicht: normal bew
                            pos[vy][vx]=0
                            pos[zy][zx]=-2
                            return pos
                        if pos[vy+i+1][vx-i-1]<0:#weg von eigene figur blockiert: nicht korrekt
                            break
                        if pos[vy+i+1][vx-i-1]>0:#auf gegnerische figur gestossen: schlagen oder nicht korrekt?
                            if not vy+i+2>7 or vx-i-2<0:
                                if vy+i+2==zy and vx-i-2==zx:#schlagen
                                    pos[vy][vx]=0
                                    pos[zy][zx]=-2
                                    pos[vy+i+1][vx-i-1]=0
                                    return self.player_schlagen_chain_WM(zy,zx,pos)
                                else:
                                    break
                            else:
                                break
                elif pos[vy][vx]==-2 and pos[zy][zx]==0 and zy<vy and zx>vx:
                    for i in range(7):
                        if vy-i-1<0 or vx+i+1>7:#ende von board erreicht: nicht korrekt
                            break
                        if vy-i-1==zy and vx+i+1==zx:#ziel erreicht: normal bew
                            pos[vy][vx]=0
                            pos[zy][zx]=-2
                            return pos
                        if pos[vy-i-1][vx+i+1]<0:#weg von eigene figur blockiert: nicht korrekt
                            break
                        if pos[vy-i-1][vx+i+1]>0:#auf gegnerische figur gestossen: schlagen oder nicht korrekt?
                            if not vy-i-2<0 or vx+i+2>7:
                                if vy-i-2==zy and vx+i+2==zx:#schlagen
                                    pos[vy][vx]=0
                                    pos[zy][zx]=-2
                                    pos[vy-i-1][vx+i+1]=0
                                    return self.player_schlagen_chain_WM(zy,zx,pos)
                                else:
                                    break
                            else:
                                break
                elif pos[vy][vx]==-2 and pos[zy][zx]==0 and zy>vy and zx>vx:
                    for i in range(7):
                        if vy+i+1>7 or vx+i+1>7:#ende von board erreicht: nicht korrekt
                            break
                        if vy+i+1==zy and vx+i+1==zx:#ziel erreicht: normal bew
                            pos[vy][vx]=0
                            pos[zy][zx]=-2
                            return pos
                        if pos[vy+i+1][vx+i+1]<0:#weg von eigene figur blockiert: nicht korrekt
                            break
                        if pos[vy+i+1][vx+i+1]>0:#auf gegnerische figur gestossen: schlagen oder nicht korrekt?
                            if not vy+i+2>7 or vx+i+2>7:
                                if vy+i+2==zy and vx+i+2==zx:#schlagen
                                    pos[vy][vx]=0
                                    pos[zy][zx]=-2
                                    pos[vy+i+1][vx+i+1]=0
                                    return self.player_schlagen_chain_WM(zy,zx,pos)
                                else:
                                    break
                            else:
                                break
                #------------------------------
            #
            print('EINGABE NICHT KORREKT2')#wenn korrekt, dann return

    def get_move(self, board):
        return self.player(copy.deepcopy(board))   

    #

#

class MCTSPlayer():
    def __init__(self, token):
        self.token=token
        self.counter=0
        self.numberofiterations=0
        #-----
        self.maxtime=1
        self.c=math.sqrt(2)
        self.depth=2
        self.numberofsimulations=30
        #-----
        
    def mcts(self,board):
        self.rootnode=MCTSNode(self.token)
        self.rootnode.position=board
        self.rootnode.playeramzug=self.token
        self.rootnode.score=0
        self.rootnode.visits=0
        self.rootnode.children=[]
        #
        self.rootnode.expand_node()
        start = time.time()
        while True:
            self.counter+=1
            selectednode=self.rootnode.select_leafnode()
            selectednode.expand_node()
            for child_node in selectednode.children:
                child_node.backpropagate(child_node.simulate())
                if (time.time() - start) > self.maxtime:
                    break
            #
            if (time.time() - start) > self.maxtime:
                break

    def get_move(self,board):
        self.counter=0
        self.mcts(board)
        print(self.counter)
        bestmove=[]
        highestnumberofvisits=-1
        for rootnodechild in self.rootnode.children:
            if rootnodechild.visits>highestnumberofvisits:
                bestmove=rootnodechild
                highestnumberofvisits=rootnodechild.visits
        return bestmove.position

class MCTSNode(MCTSPlayer):
    def __init__(self,token):
        super().__init__(token)
        self.position=[]
        self.playeramzug=0
        self.parent=None
        self.children=[]
        self.score=0
        self.visits=0
    
    def calculate_ucb(self):
        par=self.parent
        if self.visits==0:
            ucb=math.inf
        else:
            ucb=(self.score/self.visits)+self.c*(math.sqrt(math.log(par.visits)/self.visits))
        return ucb
    
    def expand_node(self):
        children=generate_children(self.position,self.playeramzug)
        for i in range(len(children)):
            self.numberofiterations+=1
            instance = MCTSNode(self.token)
            self.children.append(instance)
            instance.position=children[i]
            instance.playeramzug=-self.playeramzug
            instance.parent=self
            instance.score=0
            instance.visits=0
            
    def simulate(self):
        value=0
        values=[]
        for j in range(self.numberofsimulations):
            pos=self.position
            player=self.playeramzug
            for i in range(self.depth):
                nextpos=generate_one_random_child(pos,player)
                if nextpos==[]:
                    break
                else:
                    pos=nextpos
                    player=-player
            values.append(evaluate_position(pos,self.token))#wichtig das inarow mit token übereinstimmt.-+
        value=sum(values)/len(values)
        return value
    
    def select_leafnode(self):
        bestvalue = -math.inf
        if self.children == []:
            return self
        else: 
            for child in self.children:
                ucbofchild = child.calculate_ucb()
                if ucbofchild > bestvalue:
                    bestvalue = ucbofchild
                    selectednode = child
            return selectednode.select_leafnode()

    def backpropagate2(self, newscore, numberofsimulations):
        self.score += newscore
        self.visits += numberofsimulations
        #self.visits+=1
        parent=self.parent
        if parent is not None:
            parent.backpropagate(newscore, numberofsimulations)

    def backpropagate(self, newscore):
        self.score += newscore
        self.visits+=1
        parent=self.parent
        if parent is not None:
            parent.backpropagate(newscore)

#

minimax_counter4=0

class MinimaxPlayer():
    #sucht bis max zeit erreicht ist, depth =+1, move sorting
    def __init__(self, token):
        self.token = token
        self.maxtime=5
        self.starting_depth=1 #wenn suche bei layer1 nicht fertig wird: crash

    def minimaxer(self, depth, vergangene_zeit):
        start=time.time()
        for child in self.rootnode.children:
            child.minimax(-math.inf,math.inf,False, depth)
            print("a ",end="") # child wurde fertig berechnet
            if ((time.time()+vergangene_zeit) - start) > self.maxtime:
                break
        #
        values=[]
        for child in self.rootnode.children:
            values.append(child.value)
        #
        bestmoves=[]
        bestvalue=max(values)
        for child in self.rootnode.children:
            if child.value==bestvalue:
                bestmoves.append(child)
        #output---------
        print("")
        print(values)
        print(bestvalue)
        #---------------
        bestmove=random.choice(bestmoves)
        return bestmove.position
    
    def get_move(self, board):
        start=time.time()
        global minimax_counter4
        minimax_counter4=0
        #rootnode
        self.rootnode=MinimaxNode()
        self.rootnode.position=board
        self.rootnode.playeramzug=self.token
        self.rootnode.value=None
        self.rootnode.token=self.token
        self.rootnode.depth=0
        self.rootnode.expand_node()
        #
        depth=self.starting_depth
        while (time.time() - start) < self.maxtime:
            print("DEPTH: ",depth)
            move=self.minimaxer(depth,(time.time() - start))
            bestmove=move
            if (time.time() - start) > self.maxtime:
                print("NICHT FERTIG")
            else:
                self.rootnode.sort(True)
                depth+=1
        print("---",minimax_counter4)
        return bestmove

class MinimaxNode():
    def __init__(self):
        self.value=None
        self.children=[]
        self.position=[]
        self.playeramzug=None
        self.token=None
        self.depth=None
        self.expanded=False

    def expand_node(self):
        children=generate_children(self.position,self.playeramzug)
        for i in range(len(children)):
            instance=MinimaxNode()
            instance.position=children[i]
            instance.playeramzug = -self.playeramzug
            instance.value=None
            instance.token=self.token
            instance.depth=self.depth+1
            instance.expanded=False
            self.children.append(instance)

    def minimax(self, alpha, beta, maxplayer, maxdepth):
        #
        global minimax_counter4
        minimax_counter4+=1
        #
        if self.depth==maxdepth:
            self.value = evaluate_position(self.position, self.token)
            return self.value
        elif verloren1(self.position, 1) or verloren1(self.position, -1):
            self.value = evaluate_position(self.position, self.token)
            return self.value
        #
        if not self.expanded:
            self.expand_node()
            self.expanded=True
        #
        if self.children == []:
            if self.playeramzug==self.token:
                self.value = -8888
            else:
                self.value = +8888
            return self.value
        #
        if maxplayer:
            maxvalue = -math.inf
            for child in self.children:
                eval = child.minimax(alpha, beta, False, maxdepth)
                if eval>maxvalue:
                    maxvalue=eval
                # pruning
                if eval > alpha:
                    alpha = eval
                if beta <= alpha:
                    break
            self.value=maxvalue
            return maxvalue
        #
        else:
            minvalue = math.inf
            for child in self.children:
                eval = child.minimax(alpha, beta, True, maxdepth)
                if eval<minvalue:
                    minvalue=eval
                # pruning
                if eval < beta:
                    beta = eval
                if beta <= alpha:
                    break
            self.value=minvalue
            return minvalue
        
    def sort(self, maxplayer):
        not_none_children=[]
        none_children=[]
        for child in self.children:
            if child.value==None:
                none_children.append(child)
            else:
                not_none_children.append(child)
        #
        if maxplayer:
            sorted_children = sorted(not_none_children, key=lambda x: x.value, reverse=True)
            sorted_children.extend(none_children)
            self.children=sorted_children
            #
            for child in not_none_children:
                child.sort(False)
        #
        else:
            sorted_children = sorted(not_none_children, key=lambda x: x.value, reverse=False)
            sorted_children.extend(none_children)
            self.children=sorted_children
            #
            for child in not_none_children:
                child.sort(True)
        
#

def spielen(z):
    game =Dame()
    x_wins = 0
    o_wins=0
    unentschieden=0
    for i in range(z):
        r=game.play() 
        if r== 'X':
            x_wins += 1
        elif r=='O':
            o_wins+=1
        else:
            unentschieden+=1
        print('X:',x_wins)
        print('O:',o_wins)
        print('-:',unentschieden)
    print('FERTIG')

spielen(20)

