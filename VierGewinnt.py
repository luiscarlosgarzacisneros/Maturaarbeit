import copy
import random
import time
import math

#

def gewonnen(board, player):
    gew = False
    # horizontal
    for q in range(4):
        for w in range(6):
            if board[w][q] == player and board[w][q + 1] == player and board[w][q + 2] == player and board[w][q + 3] == player:
                gew = True
    # vertikal
    for q in range(7):
        for w in range(3):
            if board[w][q] == player and board[w + 1][q] == player and board[w + 2][q] == player and board[w + 3][q] == player:
                gew = True
    # diagonal1
    for q in range(4):
        for w in range(3):
            if board[w][q] == player and board[w + 1][q + 1] == player and board[w + 2][q + 2] == player and board[w + 3][q + 3] == player:
                gew = True
    # diagonal2
    for q in range(4):
        for w in range(3):
            if board[w][q + 3] == player and board[w + 1][q + 2] == player and board[w + 2][q + 1] == player and board[w + 3][q] == player:
                gew = True
    return gew

def game_over(board):
    isover = True
    for q in range(6):
        if board[q].count(0) > 0:
            isover = False
    return isover

def fall( board, y, x, player):
    if y <= 4:
        if board[y + 1][x] == 0:
            board[y + 1][x] = player
            board[y][x] = 0
            y = y + 1
            fall(board, y, x, player)
        else:
            pass

def evaluate_position(board,player):
    score=0
    if player==1:
        otherplayer=-1
    elif player==-1:
        otherplayer=1
    # horizontal
    for q in range(4):
        for w in range(6):
            empty=0
            other=0
            filled=0
            #
            if board[w][q] == player:
                filled=filled+ 1
            elif board[w][q] == 0:
                empty=empty+ 1
            elif board[w][q] == otherplayer:
                other=other+ 1
            #
            if board[w][q + 1] == player:
                filled=filled+ 1
            elif board[w][q + 1] == 0:
                empty=empty+ 1
            elif board[w][q + 1] == otherplayer:
                other=other+ 1
            #
            if board[w][q + 2] == player:
                filled=filled+ 1
            elif board[w][q + 2] == 0:
                empty=empty+ 1
            elif board[w][q + 2] == otherplayer:
                other=other+ 1
            #
            if board[w][q + 3] == player:
                filled=filled+ 1
            elif board[w][q + 3] == 0:
                empty=empty+ 1
            elif board[w][q + 3] == otherplayer:
                other=other+ 1
            #
            if other==0:
                if filled==4:
                    score=score+10000
                elif filled==3:
                    score=score+1000
                elif filled==2:
                    score=score+3
            elif filled==0:
                if other==4:
                    score=score-10000
                elif other==3:
                    score=score-100
                elif other==2:
                    score=score-3
    # vertikal
    for q in range(7):
        for w in range(3):
            empty=0
            other=0
            filled=0
            #
            if board[w][q] == player:
                filled=filled+ 1
            elif board[w][q] == 0:
                empty=empty+ 1
            elif board[w][q] == otherplayer:
                other=other+ 1
            #
            if board[w + 1][q] == player:
                filled=filled+ 1
            elif board[w + 1][q] == 0:
                empty=empty+ 1
            elif board[w + 1][q] == otherplayer:
                other=other+ 1
            #
            if board[w + 2][q] == player:
                filled=filled+ 1
            elif board[w + 2][q] == 0:
                empty=empty+ 1
            elif board[w + 2][q] == otherplayer:
                other=other+ 1
            #
            if board[w + 3][q] == player:
                filled=filled+ 1
            elif board[w + 3][q] == 0:
                empty=empty+ 1
            elif board[w + 3][q] == otherplayer:
                other=other+ 1
            #
            if other==0:
                if filled==4:
                    score=score+10000
                elif filled==3:
                    score=score+10
                elif filled==2:
                    score=score+1
            elif filled==0:
                if other==4:
                    score=score-10000
                elif other==3:
                    score=score-30
                elif other==2:
                    score=score-1
    # diagonal1
    for q in range(4):
        for w in range(3):
            empty=0
            other=0
            filled=0
            #
            if board[w][q] == player:
                filled=filled+ 1
            elif board[w][q] == 0:
                empty=empty+ 1
            elif board[w][q] == otherplayer:
                other=other+ 1
            #
            if board[w + 1][q + 1] == player:
                filled=filled+ 1
            elif board[w + 1][q + 1] == 0:
                empty=empty+ 1
            elif board[w + 1][q + 1] == otherplayer:
                other=other+ 1
            #
            if board[w + 2][q + 2] == player:
                filled=filled+ 1
            elif board[w + 2][q + 2] == 0:
                empty=empty+ 1
            elif board[w + 2][q + 2] == otherplayer:
                other=other+ 1
            #
            if board[w + 3][q + 3] == player:
                filled=filled+ 1
            elif board[w + 3][q + 3] == 0:
                empty=empty+ 1
            elif board[w + 3][q + 3] == otherplayer:
                other=other+ 1
            #
            if other==0:
                if filled==4:
                    score=score+10000
                elif filled==3:
                    score=score+1000
                elif filled==2:
                    score=score+3
            elif filled==0:
                if other==4:
                    score=score-10000
                elif other==3:
                    score=score-100
                elif other==2:
                    score=score-3
    # diagonal2
    for q in range(4):
        for w in range(3):
            empty=0
            other=0
            filled=0
            #
            if board[w][q + 3] == player:
                filled=filled+ 1
            elif board[w][q + 3] == 0:
                empty=empty+ 1
            elif board[w][q + 3] == otherplayer:
                other=other+ 1
            #
            if board[w + 1][q + 2] == player:
                filled=filled+ 1
            elif board[w + 1][q + 2] == 0:
                empty=empty+ 1
            elif board[w + 1][q + 2] == otherplayer:
                other=other+ 1
            #
            if board[w + 2][q + 1] == player:
                filled=filled+ 1
            elif board[w + 2][q + 1] == 0:
                empty=empty+ 1
            elif board[w + 2][q + 1] == otherplayer:
                other=other+ 1
            #
            if board[w + 3][q] == player:
                filled=filled+ 1
            elif board[w + 3][q] == 0:
                empty=empty+ 1
            elif board[w + 3][q] == otherplayer:
                other=other+ 1
            #
            if other==0:
                if filled==4:
                    score=score+10000
                elif filled==3:
                    score=score+1000
                elif filled==2:
                    score=score+3
            elif filled==0:
                if other==4:
                    score=score-10000
                elif other==3:
                    score=score-100
                elif other==2:
                    score=score-3
    return score
 
def generate_children(position, player):
    children = []
    boardcopy = copy.deepcopy(position)
    for x in range(7):
        if boardcopy[0][x] == 0:
            boardcopy[0][x] = player
            fall(boardcopy, 0, x, player)
            children.append(boardcopy)
            boardcopy = copy.deepcopy(position)
    #
    return children

def generate_one_random_child(position,player):#für Monte Carlo Simulation
        boardcopy = copy.deepcopy(position)
        while True:
            x=random.randint(0,6)
            if boardcopy[0][x] == 0:
                break
        boardcopy[0][x] = player
        fall(boardcopy, 0, x, player)
        return boardcopy

#
minimax_counter4=0
#

class VierGewinnt():
    def __init__(self):
        self.board = []
        self.turn=0
        self.players=[]
    
    def printboard(self, board):
        print('  1   2   3   4   5   6   7')
        print('-----------------------------')
        for i in range(6):
            print('I ', end='')
            for j in range(7):
                if board[i][j]==1:
                    print('X', end='')
                elif board[i][j]==-1:
                    print('O', end='')
                else:
                    print(' ', end='')
                print(' I ', end='')
            print('')
            print('-----------------------------')

    def play(self):
        self.board = [
        [0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0]
        ]
        #
        self.turn=0
        #
        # Spieler:innen vorbereiten
        # X spielt immer zuerst
        self.players.clear()
        self.players.append(MinimaxPlayer(1))
        self.players.append(MCTSPlayer(-1))
        #
        current=0
        while True:
            print(self.turn)
            self.printboard(self.board)
            player = self.players[current]
            if player.token==1:
                istamzug='X'
            else:
                istamzug='O'
            print(istamzug, ' ist am Zug')
            self.board=player.get_move(copy.deepcopy(self.board))
            current = (current + 1) % 2
            self.turn+=1
            if game_over(self.board) or gewonnen(self.board,-1)or gewonnen(self.board,1):
                break
        self.printboard(self.board)
        if gewonnen(self.board,-1):
            print('O HAT GEWONNEN')
            return 'O'
        elif gewonnen(self.board,1):
            print('X HAT GEWONNEN')
            return 'X'
        else:
            print('UNENTSCHIEDEN')
            return ' '

#

class HumanPlayer():
    def __init__(self, token):
        self.token=token

    def player(self, board):
        try:
            x = int(input('x: ')) - 1
            if board[0][x] == 0:
                board[0][x] = self.token
                fall(board, 0, x, self.token)
            else:
                print('FELD BESETZT')
                self.player(board)
        except:
            print('EINGABE NICHT KORREKT')
            self.player(board)

    def get_move(self, board):
        self.player(board)
        return board

#

class MCTSPlayer():
    def __init__(self, token):
        self.token=token
        self.counter=0
        self.numberofiterations=0
        #-----
        self.maxtime=5
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
            if selectednode.children==[]:
                selectednode.backpropagate(selectednode.simulate(),selectednode.numberofsimulations)
            else:
                selectednode.expand_node()
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
        #
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
            #
            instance.position=children[i]
            if self.playeramzug==-1:
                instance.playeramzug=1
            elif self.playeramzug==1:
                instance.playeramzug=-1
            instance.parent=self
            instance.score=0
            instance.visits=0
        #
            
    def simulate(self):
        value=0
        values=[]
        for j in range(self.numberofsimulations):
            pos=self.position
            player=self.playeramzug
            for i in range(self.depth):
                nextpos=generate_one_random_child(pos,player)
                pos=nextpos
                if player==-1:
                    player=1
                elif player==1:
                    player=-1
            values.append(evaluate_position(pos,self.token))#wichtig das evaluate_position mit token übereinstimmt.-+
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

    def backpropagate(self, newscore, numberofsimulations):
        self.score += newscore
        self.visits+=1
        #self.visits += numberofsimulations
        parent=self.parent

        if parent is not None:
            parent.backpropagate(newscore, numberofsimulations)

#

class MinimaxPlayer():
    #sucht bis max zeit erreicht ist, depth =+1, move sorting
    def __init__(self, token):
        self.token=token
        self.maxtime=3
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
        elif gewonnen(self.position, 1) or gewonnen(self.position, -1):
            self.value = evaluate_position(self.position, self.token)
            return self.value
        #
        if self.expanded:
            children=self.children
        else:
            self.expand_node()
            self.expanded=True
        #
        if self.children==[]:
            self.value = evaluate_position(self.position, self.token)
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
    game =VierGewinnt()
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
