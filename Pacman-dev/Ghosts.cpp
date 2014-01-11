class Ghosts
{
    public:
    int x,y;
    int direction;
    Ghosts()
    {
            x=y=0;
            direction=STAY;
    }
    Ghosts(int a,int b, int c=0)
    {
               x=a;
               y=b;
               direction=c;
    }

    int Move(int map[ROWS][COLS],const Players player1,const Players player2, const Ghosts g[N_GHOSTS])
    {
             int t=(rand())%5;
             if(t==0) direction=STAY;
             /*else if(direction==RIGHT && map[x+1][y]==-1)
                  direction=LEFT;
                  else if(direction==LEFT && map[x-1][y]== -1)
                       direction=RIGHT;
                       else if(direction==UP && map[x][y+1]== -1)
                            direction=DOWN;
                            else if(direction=DOWN && map[x][y-1]== -1)
                                  direction=UP;*/
                                  else switch(t)
                                       {
                                       case 1: if(map[x][y-1]!=-1) {direction=LEFT; break;}
                                       case 2: if(map[x][y+1]!=-1) {direction=RIGHT; break;}
                                       case 3: if(map[x-1][y]!=-1) {direction=UP; break;}
                                       case 4: if(map[x+1][y]!=-1) {direction=DOWN; break;}
                                       default: direction=STAY;
                                            break;
                                       }        
        return direction;
    }
};
