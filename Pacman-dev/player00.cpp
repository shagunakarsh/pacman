class player00
{
      public:
    int Move(int map[ROWS][COLS],const Players myPlayer,const Players opponent, const Ghosts g[N_GHOSTS])
    {
        int direction;
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
                                       case 1: if(map[myPlayer.x][myPlayer.y-1]!=-1) {direction=LEFT; break;}
                                       case 2: if(map[myPlayer.x][myPlayer.y+1]!=-1) {direction=RIGHT; break;}
                                       case 3: if(map[myPlayer.x-1][myPlayer.y]!=-1) {direction=UP; break;}
                                       case 4: if(map[myPlayer.x+1][myPlayer.y]!=-1) {direction=DOWN; break;}
                                       default: direction=STAY;
                                            break;
                                       }    
        return direction;
    }

};
