#include<iostream>
#include<cmath>
#include<algorithm>
#include<limits>
#include<vector>
#include<bitset>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<fstream>
#include<time.h>

using namespace std;

#define ROWS 19
#define COLS 43
#define N_GHOSTS 3
#define MAX_HEALTH 3
#define MAX_MOVES ROWS*COLS
#define STAY   0
#define UP     1
#define DOWN  -1
#define RIGHT  2
#define LEFT  -2
#define ENABLEGUI 1
struct Players
{
    int x,y;
    int direction;
    int health;
    int points;
};

#include "Ghosts.cpp"
#include "player00.cpp"
#include "playerXX.cpp"
#if ENABLEGUI
#include "gameGUI.cpp"
#endif
class Controller
{
    private:
    #if ENABLEGUI
    GUI game;
    #endif
    int map[ROWS][COLS];
    Ghosts ghosts[N_GHOSTS];
    Players player1,player2;
    player00 p1;
    playerXX p2;
    int totalCoins;
    int moves;
    int p1_reset;
    int p2_reset;
    int end;
    public:

    Controller(char * mapfile, int gx, int gy)
    {
        end=0;
        p1_reset=1;
        p2_reset=1;
        totalCoins=0;
        moves=0;
        ifstream in;
        in.open(mapfile);                       /*Open the file containing the MAP*/
        for(int i=0;i<ROWS;i++)                 /*Initialize the map*/
        {
            char tempStr[COLS+1];
            in>>tempStr;
            for(int j=0;j<COLS;j++)
            {
                if(tempStr[j]=='o')
                {
                    map[i][j]=1;
                    totalCoins++;
                }
                else if(tempStr[j]=='#')
                map[i][j]=-1;
                else
                map[i][j]=0;
            }
        }
        in.close();

        /*Randomly situate the two plyers*/
        srand(time(NULL));
        reset();
        /*Starting positions had now been fixed*/

        player1.points=player2.points=0;                                 /*Fix the initial health and poits for both players*/
        player1.health=player2.health=MAX_HEALTH;
       
        totalCoins = totalCoins-2;                                       /*Two player taking the positions of two coins*/

        for(int i=0;i<N_GHOSTS;i++)
        {
            ghosts[i].x=gx;
            ghosts[i].y=gy;
        }
        #if ENABLEGUI
        game.copy(player1,player2,ghosts);
        #endif

    }
    ~Controller()
    {
                 #if ENABLEGUI
                 game.cleanUp();
                 #endif
    }
    void Display(int move1,int move2)
    {
        char tmap[ROWS][COLS];
        for(int i=0;i<ROWS;i++)
        {
            for(int j=0;j<COLS;j++)
            {
                if(map[i][j]==-1)
                tmap[i][j]='#';
                else if(map[i][j]==0)
                tmap[i][j]='.';
                else
                tmap[i][j]='o';
            }
        }

        for(int i=0;i<N_GHOSTS;i++)
        tmap[ghosts[i].x][ghosts[i].y]='g';

        if(player1.health!=0)
        tmap[player1.x][player1.y]='1';
        if(player2.health!=0)
        tmap[player2.x][player2.y]='2';
        #if ENABLEGUI
        game.displayGUI(tmap,move1,move2,totalCoins,player1,player2,ghosts,moves,end);
        #else
        for(int i=0;i<ROWS;i++)
        {
        for(int j=0;j<COLS;j++)
        cout<<tmap[i][j];
        cout<<endl;
        }
        cout<<"Player1 Points- "<<player1.points<<endl<<"Player1 Health- "<<player1.health<<endl;
        cout<<"Player2 Points- "<<player2.points<<endl<<"Player2 Health- "<<player2.health<<endl;
        cout<<"Number of moves-"<<moves<<endl;
        cout<<endl<<endl;
        if(end)
        {
               if(player1.points>player2.points)
               cout<<"Player1 Wins!!!";
               else
               if(player2.points>player1.points)
               cout<<"Player2 Wins!!!";
               else
               cout<<"Its a TIE!!!";
        }
        #endif
    }


    void Copy(Players &temp1,Players &temp2,int tempMap[ROWS][COLS], Ghosts tempGhosts[])
    {
        temp1=player1;
        temp2=player2;
        for(int i=0;i<ROWS;i++)
        for(int j=0;j<COLS;j++)
        tempMap[i][j]=map[i][j];

        for(int i=0;i<N_GHOSTS;i++)
        tempGhosts[i]=ghosts[i];
    }

    int ValidateMove(Players temp1,Players temp2, int &moveMade, Players &by, Ghosts g[])
    {
        int x=by.x;
        int y=by.y;
        if(moveMade==UP)
        {
            if(map[x-1][y]==-1)
            {moveMade=STAY;
			return 0;}
        }
        else if(moveMade==DOWN)
        {
            if(map[x+1][y]==-1)
            {moveMade=STAY;
			return 0;}
        }
        else if(moveMade==RIGHT)
        {
            if(map[x][y+1]==-1)
            {moveMade=STAY;
			return 0;}
        }
        else if(moveMade==LEFT)
        {
            if(map[x][y-1]==-1)
            {moveMade=STAY;
			return 0;}
        }
        if(by.health==0)
        {
             by.x=-1;
             by.y=-1;
             moveMade=STAY;
             return 0;
        }
        if(temp1.x!=player1.x || temp2.x!=player2.x || temp1.y!=player1.y || temp2.y!=player2.y)
        return 0;
        if(temp1.health!=player1.health || temp2.health!=player2.health || temp1.points!=player1.points || temp2.points!=player2.points)
        return 0;
        if(temp1.direction!=player1.direction || temp2.direction!=player2.direction)
        return 0;

        for(int i=0;i<N_GHOSTS;i++)
        if(g[i].x!=ghosts[i].x || g[i].y!=ghosts[i].y || g[i].direction!=ghosts[i].direction)
        return 0;

        return 1;
    }

    void CrossOvers(int &m1, int &m2, int m[])
    {
        int newCoordinates[2+N_GHOSTS][2];
        int oldCoordinates[2+N_GHOSTS][2];

        oldCoordinates[0][0]=player1.x;
        oldCoordinates[0][1]=player1.y;
        if(player1.health!=0)
        {
            if(m1==UP)
            {
                newCoordinates[0][0]=player1.x-1;
                newCoordinates[0][1]=player1.y;
            }
            else if(m1==DOWN)
            {
                newCoordinates[0][0]=player1.x+1;
                newCoordinates[0][1]=player1.y;
            }
            else if(m1==RIGHT)
            {
                newCoordinates[0][0]=player1.x;
                newCoordinates[0][1]=player1.y+1;
            }
            else if(m1==LEFT)
            {
                newCoordinates[0][0]=player1.x;
                newCoordinates[0][1]=player1.y-1;
            }
            else
            {
                newCoordinates[0][0]=player1.x;
                newCoordinates[0][1]=player1.y;
            }
        }
        else
        {
            newCoordinates[0][0]=player1.x;
            newCoordinates[0][1]=player1.y;
        }
        oldCoordinates[1][0]=player2.x;
        oldCoordinates[1][1]=player2.y;
        if(player2.health!=0)
        {
            if(m2==UP)
            {
                newCoordinates[1][0]=player2.x-1;
                newCoordinates[1][1]=player2.y;
            }
            else if(m2==DOWN)
            {
                newCoordinates[1][0]=player2.x+1;
                newCoordinates[1][1]=player2.y;
            }
            else if(m2==RIGHT)
            {
                newCoordinates[1][0]=player2.x;
                newCoordinates[1][1]=player2.y+1;
            }
            else if(m2==LEFT)
            {
                newCoordinates[1][0]=player2.x;
                newCoordinates[1][1]=player2.y-1;
            }
            else
            {
                newCoordinates[1][0]=player2.x;
                newCoordinates[1][1]=player2.y;
            }
        }
        else
        {
                newCoordinates[1][0]=player2.x;
                newCoordinates[1][1]=player2.y;
        }
        for(int i=0;i<N_GHOSTS;i++)
        {
            oldCoordinates[i+2][0]=ghosts[i].x;
            oldCoordinates[i+2][1]=ghosts[i].y;
            if(m[i]==UP)
            {
                newCoordinates[i+2][0]=ghosts[i].x-1;
                newCoordinates[i+2][1]=ghosts[i].y;
            }
            else if(m[i]==DOWN)
            {
                newCoordinates[i+2][0]=ghosts[i].x+1;
                newCoordinates[i+2][1]=ghosts[i].y;
            }
            else if(m[i]==RIGHT)
            {
                newCoordinates[i+2][0]=ghosts[i].x;
                newCoordinates[i+2][1]=ghosts[i].y+1;
            }
            else if(m[i]==LEFT)
            {
                newCoordinates[i+2][0]=ghosts[i].x;
                newCoordinates[i+2][1]=ghosts[i].y-1;
            }
            else
            {
                newCoordinates[i+2][0]=ghosts[i].x;
                newCoordinates[i+2][1]=ghosts[i].y;
            }
        }


        bool shift[2+N_GHOSTS];
        for(int i=0;i<2+N_GHOSTS;i++)
        shift[i]=true;

        for(int i=0;i<2;i++)
        {
            for(int j=i+1;j<2+N_GHOSTS;j++)
            {
                if(newCoordinates[i][0]==newCoordinates[j][0] && newCoordinates[i][1]==newCoordinates[j][1])
                shift[i]=shift[j]=false;
            }
        }


       for(int i=0;i<2;i++)
        {
            for(int j=i+1;j<2+N_GHOSTS;j++)
            {
                if(newCoordinates[i][0]==oldCoordinates[j][0] && newCoordinates[i][1]==oldCoordinates[j][1] && oldCoordinates[i][0]==newCoordinates[j][0] && oldCoordinates[i][1]==newCoordinates[j][1])
                shift[i]=shift[j]=false;
            }
        }

        if(!shift[0]&&player1.health>=1)
        {
             player1.x=newCoordinates[0][0];
             player1.y=newCoordinates[0][1];
             player1.health--;
             p1_reset=1;
        }
        else
        {
            int tx=player1.x=newCoordinates[0][0];
            int ty=player1.y=newCoordinates[0][1];
            if(map[tx][ty]==1)
            {
                player1.points++;
                map[tx][ty]=0;
                totalCoins--;
            }
        }
        if(!shift[1]&&player2.health>=1)
        {
              player2.x=newCoordinates[1][0];
              player2.y=newCoordinates[1][1];
              player2.health--;
              p2_reset=1;
        }      
        else
        {
            int tx=player2.x=newCoordinates[1][0];
            int ty=player2.y=newCoordinates[1][1];
            if(map[tx][ty]==1)
            {
                player2.points++;
                map[tx][ty]=0;
                totalCoins--;
            }
        }

        for(int i=0;i<N_GHOSTS;i++)
        {
            //if(shift[i+2]/*1*/)
            {
                ghosts[i].x=newCoordinates[i+2][0];
                ghosts[i].y=newCoordinates[i+2][1];
            }
        }
        if(player1.health==0&&player2.health==0)
        end=1;
        if(moves==MAX_MOVES)
        end=1;
        

    }
    void reset()
    {
         if(p1_reset||p2_reset)
         {
            /* int pos1=(rand())%(totalCoins);
            pos1++;
            int pos2=(rand())%(totalCoins-pos1);
			pos2+=pos1;
            int cnt=0;
            for(int i=0;i<ROWS;i++)
            {
                for(int j=0;j<COLS;j++)
                {
                    if(map[i][j]==1||map[i][j]==0)
                    {
                        cnt++;
                        if(cnt==pos1&&p1_reset)
                        {
                            player1.x=i;
                            player1.y=j;
                        }
                        else if(cnt==pos2&&p2_reset)
                        {
                            player2.x=i;
                            player2.y=j;
                        }
                    }
                }
            }*/
            if(p1_reset)
            {
               if(player2.x==ROWS/2&&player2.y==1)
               {
                   player1.x=ROWS/2;
                   player1.y=COLS-2;
                   for(int i=0;i<N_GHOSTS;++i)
                   {
                      if(ghosts[i].x==ROWS/2 && ghosts[i].y==COLS-2)
                      {
                         ghosts[i].x=ROWS/2;
                         ghosts[i].y=COLS/2;
                      }
                   }
               }
               else
               {
                   player1.x=ROWS/2;
                   player1.y=1;
                   for(int i=0;i<N_GHOSTS;++i)
                   {
                      if(ghosts[i].x==ROWS/2 && ghosts[i].y==1)
                      {
                         ghosts[i].x=ROWS/2;
                         ghosts[i].y=COLS/2;
                      }
                   }
               }
            }
            if(p2_reset)
            {
               if(player1.x==ROWS/2&&player1.y==COLS-2)
               {
                   player2.x=ROWS/2;
                   player2.y=1;
                   for(int i=0;i<N_GHOSTS;++i)
                   {
                      if(ghosts[i].x==ROWS/2 && ghosts[i].y==1)
                      {
                         ghosts[i].x=ROWS/2;
                         ghosts[i].y=COLS/2;
                      }
                   }
               }
               else
               {
                   player2.x=ROWS/2;
                   player2.y=COLS-2;
                   for(int i=0;i<N_GHOSTS;++i)
                   {
                      if(ghosts[i].x==ROWS/2 && ghosts[i].y==COLS-2)
                      {
                         ghosts[i].x=ROWS/2;
                         ghosts[i].y=COLS/2;
                      }
                   }
               }
            }
            p1_reset=0;
            p2_reset=0;
            }
    }
    void Game()
    {
        Players tempPlayer1,tempPlayer2;
        int allowP1=1,allowP2=1;
        int tempMap[ROWS][COLS];
        Ghosts tempGhosts[N_GHOSTS];
       
        int move1,move2,move[N_GHOSTS];

        while(moves!=MAX_MOVES&&!end)
        {
            moves++;
            reset();
            Copy(tempPlayer1,tempPlayer2,tempMap,tempGhosts);
            if(allowP1)
            {
                move1=p1.Move(map,tempPlayer1,tempPlayer2,tempGhosts);
                allowP1=ValidateMove(tempPlayer1,tempPlayer2,move1,player1,tempGhosts);
            }
            Copy(tempPlayer1,tempPlayer2,tempMap,tempGhosts);
            if(allowP2)
            {
                move2=p2.Move(map,tempPlayer2,tempPlayer1,tempGhosts);
                allowP2=ValidateMove(tempPlayer1,tempPlayer2,move2,player2,tempGhosts);
            }
            if((!allowP1&&!allowP2)||totalCoins==0)
            end=1;
            Copy(tempPlayer1,tempPlayer2,tempMap,tempGhosts);
            for(int i=0;i<N_GHOSTS;i++)
            move[i]=ghosts[i].Move(tempMap,tempPlayer1,tempPlayer2,tempGhosts);
            CrossOvers(move1,move2,move);
            Display(move1,move2);
        }
    }

};


int main(int argc,char *argv[])
{
   Controller c("map1.txt",ROWS/2,COLS/2);
   //c.Display();
   c.Game();
#if !ENABLEGUI
   getchar();
#endif
   return 0;
}
