#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <sstream>
class GUI
{
        SDL_Surface *screen;
        SDL_Surface *player1;
        SDL_Surface *player2;
        SDL_Surface *ghosts;
        SDL_Surface *wall;
        SDL_Surface *coins;
        Mix_Music *music;
        Mix_Chunk *pac;
        Mix_Chunk *pacdies;
        Mix_Music *siren;
        TTF_Font *font[2];
        SDL_Color textColor;
        SDL_Color p1Color;
        SDL_Color p2Color;
        Uint32 clearColor;
        SDL_Surface *message;
        SDL_Rect clip[3][4];
        SDL_Rect clpgh[2][2];
        Players play1,play2;
        int pd1,pd2;
        Ghosts g[N_GHOSTS];
        int speed;
        int sound;
        fstream setfile;
        int ofx,ofy;
        int fs;
        public:
        GUI()
        {
            screen = NULL;
            player1=NULL;
            player2=NULL;
            ghosts=NULL;
            wall=NULL;
            coins=NULL;
            music = NULL;
            pac=NULL;
            pacdies=NULL;
            siren = NULL;
            font[0] = NULL;
            font[1] = NULL;
            textColor.r=255;
            textColor.g=255;
            textColor.b=255;
            p1Color.r=255;
            p1Color.g=0;
            p1Color.b=0;
            p2Color.r=0;
            p2Color.g=255;
            p2Color.b=0;
            message=NULL;
            speed=200;
            sound=1;
            fs=0;
            pd1=0;
            pd2=0;
            ofx=(640-COLS*14)/2;
            ofy=(480-(ROWS+4)*14)/2; 
            setfile.open("settings.dat",ios::in);
             int a;
             setfile>>a;
             sound=a;
             setfile>>a;
             if(a<=100&&a>=0)
             speed=a;
             setfile>>a;
             if(a==0||a==1)
             fs=a;
             if (SDL_Init (SDL_INIT_VIDEO) < 0)
            {
                fprintf ( stdout,"Error:Could not initialize SDL" );
                exit (1);
            }
            TTF_Init();
            atexit (SDL_Quit);
            if(fs)
            screen = SDL_SetVideoMode (640, 480, 24, SDL_SWSURFACE | SDL_DOUBLEBUF|SDL_FULLSCREEN);
			else
			screen = SDL_SetVideoMode (640, 480, 24, SDL_SWSURFACE | SDL_DOUBLEBUF);
            if (screen == NULL)
            {
                fprintf ( stdout,"Error:Could not allocate screen memory" );
                exit (2);
            }
            SDL_WM_SetCaption ("Pacman", NULL);
            Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
            player1=SDL_LoadBMP("data/player1.bmp");
            player2=SDL_LoadBMP("data/player2.bmp");
            ghosts=SDL_LoadBMP("data/ghost.bmp");
            wall=SDL_LoadBMP("data/walls.bmp");
            coins=SDL_LoadBMP("data/coins.bmp");
            pac=Mix_LoadWAV( "data/pacman_x1.wav" );
            pacdies=Mix_LoadWAV( "data/pacman_dies.wav" );
            music = Mix_LoadMUS("data/pacman_intro.wav");
            siren = Mix_LoadMUS("data/siren.wav");
            font[1] = TTF_OpenFont( "data/font.ttf", 30 );
            font[0] = TTF_OpenFont( "data/font.ttf", 14 );
            SDL_SetColorKey(player1,SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0,0,0));
            SDL_SetColorKey(player2,SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0,0,0));
            SDL_SetColorKey(ghosts,SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0,0,0));
            SDL_SetColorKey(coins,SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0,0,0));
            clearColor = SDL_MapRGB(screen->format, 0, 0, 0);
            for(int h=0;h<3;h++)
            {
            clip[h][0].x=0+h*28;
            clip[h][0].y=0;
            clip[h][0].h=14;
            clip[h][0].w=14;
            clip[h][1].x=14+h*28;
            clip[h][1].y=0;
            clip[h][1].h=14;
            clip[h][1].w=14;
            clip[h][2].x=0+h*28;
            clip[h][2].y=14;
            clip[h][2].h=14;
            clip[h][2].w=14;
            clip[h][3].x=14+h*28;
            clip[h][3].y=14;
            clip[h][3].h=14;
            clip[h][3].w=14;
            }
            for(int i=0;i<2;i++)
            for(int j=0;j<2;j++)
            {
            clpgh[i][j].x=j*14;
            clpgh[i][j].y=i*14;
            clpgh[i][j].w=14;
            clpgh[i][j].h=14;
            }
            displayStart();
        }
        private:
        void renderText(int x,int y,std::string dispstr,SDL_Color dispcol,int fsize)
        {
             message = TTF_RenderText_Solid( font[fsize], dispstr.c_str(), dispcol );
            SDL_Rect off;
            off.x=x;
            off.y=y;
            SDL_BlitSurface(message,NULL,screen,&off);
        }
        void renderText(int x,int y,std::string dispstr,SDL_Color dispcol,int fsize,int num)
        {
             SDL_Rect off;
                std::string str=dispstr;
                std::stringstream sstr;sstr << num;
                std::string str1 = sstr.str();
                str.insert(str.length(),str1);
                message = TTF_RenderText_Solid( font[fsize], str.c_str(),dispcol);
                off.x=x;
                off.y=y;
                SDL_BlitSurface(message,NULL,screen,&off);
        }
        void detectWall(char map[ROWS][COLS],int i,int j,SDL_Rect &clip1)
        {
             clip1.w=clip1.h=14;
             int ab=0;
             if(map[i-1][j]=='#'&&i>0)
             ab+=1;
             if(map[i][j-1]=='#'&&j>0)
             ab+=2;
             if(map[i][j+1]=='#'&&j<COLS-1)
             ab+=4;
             if(map[i+1][j]=='#'&&i<ROWS-1)
             ab+=8;
             switch(ab)
             {
                       case 1:
                       clip1.x=56,clip1.y=0;
                       break;
                       case 2:
                       clip1.x=56,clip1.y=14;
                       break;
                       case 3:
                       clip1.x=28,clip1.y=14;
                       break;
                       case 4:
                      clip1.x=42,clip1.y=14;
                       break;
                       case 5:
                       clip1.x=14,clip1.y=14;
                       break;
                       case 6:
                        clip1.x=0,clip1.y=0;
                       break;
                       case 7:
                       clip1.x=84,clip1.y=0;
                       break;
                       case 8:
                       clip1.x=42,clip1.y=0;
                       break;
                       case 9:
                       clip1.x=0,clip1.y=14;
                       break;
                       case 10:
                       clip1.x=28,clip1.y=0;
                       break;
                       case 11:
                       clip1.x=84,clip1.y=14;
                       break;
                       case 12:
                       clip1.x=14,clip1.y=0;
                       break;
                       case 13:
                       clip1.x=98,clip1.y=14;
                       break;
                       case 14:
                       clip1.x=98,clip1.y=0;
                       break;
                       case 15:
                       clip1.x=70,clip1.y=14;
                       break;
                       case 0:
                       clip1.x=70,clip1.y=0;
                       break;
             }
        } 
        void plays(int id)
        {
             if(sound)
             {
             switch(id)
             {
                       case 1:
                       Mix_PlayChannel( -1, pac, 0 );
                       //SDL_Delay (170);
                       break;
                       case 2:
                       Mix_PlayChannel( -1, pacdies, 0 );
                       //SDL_Delay (1500);
                       break;
                       case 3:
                       Mix_PlayMusic( music, 1 );
                       break;
                       case 4:
                       Mix_PlayMusic( siren, -1 );
                       break;
             }
             }
        }
        void CheckForEvents()
        {
             int pause=0, done=0;
               SDL_Event event;
                ev:
                while (SDL_PollEvent (&event))
                {
                    switch (event.type)
                    {
                    case SDL_KEYDOWN:
                         if(event.key.keysym.sym==SDLK_ESCAPE)
                         done=1;
                         if(event.key.keysym.sym==SDLK_p)
                         {
                         if(pause)
                         pause=0;
                         else
                         pause=1;
                         }
                         if(event.key.keysym.sym==SDLK_f)
                         {
                         if(fs)
                         {
                               fs=0;
                               screen = SDL_SetVideoMode (640, 480, 24, SDL_SWSURFACE | SDL_DOUBLEBUF);
                         }
                         else
                         {
                             fs=1;
                               screen = SDL_SetVideoMode (640, 480, 24, SDL_SWSURFACE | SDL_DOUBLEBUF|SDL_FULLSCREEN);
                         }
                         }
                         if(event.key.keysym.sym==SDLK_UP&&speed>=10)
                          speed-=10;
                         if(event.key.keysym.sym==SDLK_DOWN&&speed<=190)
                         speed+=10;
                         if(event.key.keysym.sym==SDLK_s)
                         {
                         if(sound)
                         {
                         Mix_PauseMusic();
                         sound=0;
                         }
                         else
                         {
                              sound=1;
                             if(Mix_PlayingMusic()==0)
                             plays(4);
                             else
                             Mix_ResumeMusic();
                            
                         }
                         }
                         break;
                    case SDL_QUIT:
                        done = 1;
                        break;
                    default:
                        break;
                    }
                }
                if(done)
                {
                        cleanUp();
                        exit(0);
                }
                if(pause)
                goto ev;
        }
        void displayStart()
        {
            plays(3);
            renderText(150,210,"iFest IIT Roorkee",textColor,1);
            renderText(235,240,"presents",textColor,1);
            SDL_Flip(screen);
            CheckForEvents();
            SDL_Delay(1000);
            SDL_FillRect(screen, NULL, clearColor);
            renderText(210,210,"PACMAN",textColor,1);
            SDL_Flip(screen);
            CheckForEvents();
            SDL_Delay (1000);
            SDL_FillRect(screen, NULL, clearColor);
            renderText(240,180,"CONTROLS",textColor,0);
            renderText(240,205,"Sound - S",textColor,0);
            renderText(240,220,"Pause - P",textColor,0);
            renderText(240,235,"Speed - UP, DOWN",textColor,0);
            renderText(240,250,"Exit - ESC",textColor,0);
            renderText(240,265,"Toggle FullScreen - F",textColor,0);
            SDL_Flip(screen);
            CheckForEvents();
            SDL_Delay(2500);
            plays(4);
        }
        void displayEnd()
        {
             SDL_FillRect(screen, NULL, clearColor);
             SDL_Rect off;
             if(play1.points>play2.points)
             renderText(200,220,"Player1 Wins!!!",textColor,1);
             if(play2.points>play1.points)
             renderText(200,220,"Player2 Wins!!!",textColor,1);
             if(play1.points==play2.points)
             renderText(200,220,"Its a TIE!!!",textColor,1);
            SDL_Flip(screen);
            SDL_Delay(500);
        }
        public:
        void displayGUI(char map[ROWS][COLS],int move1,int move2,int tc,int moves,Players p1,Players p2,Ghosts ghost[N_GHOSTS])
        {
             for(int k=0;k<=6;k++)
            {
                CheckForEvents();
                Uint32 color;
                SDL_Rect off;
                SDL_Rect clip1;
                color = SDL_MapRGB (screen->format, 0, 0, 0);
                SDL_FillRect (screen, NULL, color);
                for(int i=0;i<ROWS;i++)
                { 
                        for(int j=0;j<COLS;j++)
                        {
                        off.x=(j%COLS)*14+ofx;
                        off.y=i*14+ofy; 
                        switch(map[i][j])
                        {
                                 case '#':
                                 detectWall(map,i,j,clip1);
                                 SDL_BlitSurface(wall,&clip1,screen,&off);    
                                 break;
                                 case 'o':
                                 SDL_BlitSurface(coins,NULL,screen,&off);
                                 break;
                                 case '1':
                                      off.x+=(play1.y-p1.y)*(6-k)*2;
                                      off.y+=(play1.x-p1.x)*(6-k)*2;
                                      if(pd1)
                                      {
                                             SDL_Rect clp;
                                             clp.y=28;
                                             clp.h=clp.w=14;
                                             clp.x=k*14;
                                             SDL_BlitSurface(player1,&clp,screen,&off);
                                      }
                                      else
                                      {
                                      switch((move1==STAY)?play1.direction:move1)
                                      {
                                                   case UP:
                                                   SDL_BlitSurface(player1,&clip[k%3][0],screen,&off);
                                                   break;
                                                   case DOWN:
                                                   SDL_BlitSurface(player1,&clip[k%3][1],screen,&off);
                                                   break;
                                                   case LEFT:
                                                   SDL_BlitSurface(player1,&clip[k%3][2],screen,&off);
                                                   break;
                                                   case RIGHT:
                                                   SDL_BlitSurface(player1,&clip[k%3][3],screen,&off);
                                                   break;
                                      }
                                      }
                                 break;
                                 case '2':
                                      off.x+=(play2.y-p2.y)*(6-k)*2;
                                      off.y+=(play2.x-p2.x)*(6-k)*2;
                                      if(pd2)
                                      {
                                             SDL_Rect clp;
                                             clp.y=28;
                                             clp.h=clp.w=14;
                                             clp.x=k*14;
                                             SDL_BlitSurface(player2,&clp,screen,&off);
                                      }
                                      else
                                      {
                                      switch((move2==STAY)?play2.direction:move2)
                                      {
                                                   case UP:
                                                   SDL_BlitSurface(player2,&clip[k%3][0],screen,&off);
                                                   break;
                                                   case DOWN:
                                                   SDL_BlitSurface(player2,&clip[k%3][1],screen,&off);
                                                   break;
                                                   case LEFT:
                                                   SDL_BlitSurface(player2,&clip[k%3][2],screen,&off);
                                                   break;
                                                   case RIGHT:
                                                   SDL_BlitSurface(player2,&clip[k%3][3],screen,&off);
                                                   break;
                                      }
                                      }
                                 break;
                                 case 'g':
                                 int h;
                                 for(h=0;h<N_GHOSTS;h++)
                                 if(i==ghost[h].x&&j==ghost[h].y)
                                 break;
                                 off.x+=(g[h].y-ghost[h].y)*(6-k)*2;
                                 off.y+=(g[h].x-ghost[h].x)*(6-k)*2;
                                 if(ghost[h].direction==UP)
                                 SDL_BlitSurface(ghosts,&clpgh[0][0],screen,&off);
                                 else
                                 if(ghost[h].direction==DOWN)
                                 SDL_BlitSurface(ghosts,&clpgh[0][1],screen,&off);
                                 else
                                 if(ghost[h].direction==LEFT)
                                 SDL_BlitSurface(ghosts,&clpgh[1][0],screen,&off);
                                 else
                                 if(ghost[h].direction==RIGHT)
                                 SDL_BlitSurface(ghosts,&clpgh[1][1],screen,&off);
                                 else
                                 SDL_BlitSurface(ghosts,&clpgh[0][0],screen,&off);
                                 break;
                                 default:
                                 break;
                        };
                        }
                }
                renderText(0,ROWS*14+ofy,"Player1 Points- ",p1Color,0,p1.points);
                if(p1.health<=0)
                renderText(0,(ROWS+1)*14+ofy,"Player1 Gameover ",p1Color,0);
                for(int i=0;i<p1.health;i++)
                {
                        off.x=14*i;
                        off.y=(ROWS+1)*14+ofy;
                        SDL_BlitSurface(player1,&clip[0][3],screen,&off);
                }
                renderText(0,(ROWS+2)*14+ofy,"Player2 Points- ",p2Color,0,p2.points);
                if(p2.health<=0)
                renderText(0,(ROWS+3)*14+ofy,"Player2 Gameover ",p2Color,0);
                for(int i=0;i<p2.health;i++)
                {
                        off.x=14*i;
                        off.y=(ROWS+3)*14+ofy;
                        SDL_BlitSurface(player2,&clip[0][3],screen,&off);
                }
                if(sound)
                renderText(320,ROWS*14+ofy,"Sound-ON",p2Color,0);
                else
                renderText(320,ROWS*14+ofy,"Sound-OFF",p1Color,0);
                renderText(320,(ROWS+1)*14+ofy,"Speed",textColor,0);
                std::string spd1="";
                for(int i=0;i<=(10-speed/20);i++)
                spd1.insert(spd1.length(),"|");
                renderText(320,(ROWS+2)*14+ofy,spd1,textColor,0);
                renderText(320,(ROWS+3)*14+ofy,"Moves : ",textColor,0,moves);
                SDL_Flip(screen);
                static int i;
                if(!pd1&&!pd2)
                SDL_Delay(speed/7);
                else
                SDL_Delay(220);
                if(k==6)
                {
                        for(int o=0;o<N_GHOSTS;o++)
                        g[o]=ghost[o];
                     if(tc<i)
                     {
                             plays(1);
                     }
                     /*else
                     SDL_Delay(speed);*/
                     i=tc;
                     play1.x=p1.x;
                     play1.y=p1.y;
                     if(move1!=STAY)
                     play1.direction=move1;
                     play1.points=p1.points;
                     play2.x=p2.x;
                     play2.y=p2.y;
                     if(p1.health<play1.health&&!pd1&&p2.health<play2.health&&!pd2)
                    {
                         pd1=1;
                         pd2=1;
                         play1.health=p1.health;
                         play2.health=p2.health;
                         plays(2);
                         displayGUI(map,move1,move2,tc,moves,p1,p2,ghost);
                         pd1=0;pd2=0;
                     }
                     else
                     {
                         if(p1.health<play1.health&&!pd1)
                        {
                         pd1=1;
                         play1.health=p1.health;
                         plays(2);
                         displayGUI(map,move1,move2,tc,moves,p1,p2,ghost);
                         pd1=0;
                         }
                         if(p2.health<play2.health&&!pd2)
                        {
                         pd2=1;
                         play2.health=p2.health;
                         plays(2);
                         displayGUI(map,move1,move2,tc,moves,p1,p2,ghost);
                         pd2=0;
                         }
                     }
                     play1.health=p1.health;
                     play2.health=p2.health;
                     if(move2!=STAY)
                     play2.direction=move2;
                     play2.points=p2.points;
                     if(moves==MAX_MOVES)
                     {
                          SDL_Delay(2000);
                          displayEnd();
                     }
                 }
            }
        }
        void copy(Players p1,Players p2,Ghosts g1[N_GHOSTS])
        {
             play1=p1;
             play2=p2;
             for(int h=0;h<N_GHOSTS;h++)
             g[h]=g1[h];
        }
        void cleanUp()
        {
             setfile.close();
             ofstream st;
             st.open("settings.dat",ios::out);
             st<<sound<<"\n"<<speed<<"\n"<<fs;
             st.close();
            SDL_FreeSurface( player1 );
            SDL_FreeSurface( player2 );
            SDL_FreeSurface( coins );
            SDL_FreeSurface( ghosts );
            SDL_FreeSurface( wall );
            Mix_FreeChunk( pac );
            Mix_FreeChunk( pacdies );
            Mix_FreeMusic( music );
            Mix_FreeMusic( siren );
            SDL_Quit();    
        }
};

