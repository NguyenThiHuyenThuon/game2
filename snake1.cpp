
#include<bits/stdc++.h>
#include<SDL.h>
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <deque>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Snake";

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Mix_Chunk* chunk = NULL;
Mix_Music* music = NULL;
TTF_Font* font = NULL;
SDL_Event e;
SDL_Rect food{rand()%750+10,rand()%441 + 120,20,20};
SDL_Rect tail;
SDL_Rect loa{5,5,50,50};
SDL_Rect stone{rand()%700+10,rand()%420 + 120,rand()%51+20,stone.w};
bool running = false;

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();


void logSDLError(std::ostream& os,const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}


SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{

	SDL_Texture *texture = nullptr;

	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());

	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);

		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



void renderTexture( SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = w;
    dst.h = h;

	SDL_RenderCopy(ren, tex, NULL, &dst);
    SDL_RenderCopy(ren, tex, NULL, &dst);


}

void board(int score, int level){
    SDL_SetRenderDrawColor(renderer, 61, 72, 73, 12);

    SDL_Rect fillRect = {0, 0, SCREEN_WIDTH, 120 };
    SDL_Rect fillRect2 = {0,0, 10, SCREEN_HEIGHT};
    SDL_Rect fillRect3 = {0,590,SCREEN_WIDTH,10};
    SDL_Rect fillRect4 = {790,0,10,SCREEN_HEIGHT};

	stringstream ss, sl;
	ss << (score);
	sl << (level);
    string str = ss.str();
    string str2 = sl.str();
	const char* scoreChar = str.c_str();
	const char* levelChar = str2.c_str();

	TTF_Init();
	int texW = 0;
	int texH = 0;

	TTF_Font* font = TTF_OpenFont("arial.ttf", 30);
	SDL_Color color = { 255, 255, 255 };


	SDL_Surface* surfaceTwo = TTF_RenderText_Solid(font,
		"SCORE: ", color);
	SDL_Texture* textureTwo = SDL_CreateTextureFromSurface(renderer, surfaceTwo);
	SDL_QueryTexture(textureTwo, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectTwo = { 230, 70, texW, texH };

	SDL_Surface* surfaceThree = TTF_RenderText_Solid(font,
		scoreChar, color);
	SDL_Texture* textureThree = SDL_CreateTextureFromSurface(renderer, surfaceThree);
	SDL_QueryTexture(textureThree, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectThree = { 350, 70, texW, texH };

	SDL_Surface* surfaceFour = TTF_RenderText_Solid(font,
		levelChar, color);
	SDL_Texture* textureFour = SDL_CreateTextureFromSurface(renderer, surfaceFour);
	SDL_QueryTexture(textureFour, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectFour = { 350, 30, texW, texH };

    SDL_Surface* surfaceFive = TTF_RenderText_Solid(font,
		"LEVEL: ", color);
	SDL_Texture* textureFive = SDL_CreateTextureFromSurface(renderer, surfaceFive);
	SDL_QueryTexture(textureFive, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectFive = { 230, 30,texW, texH };


	SDL_RenderFillRect(renderer, &fillRect);
    SDL_RenderFillRect(renderer, &fillRect2);
    SDL_RenderFillRect(renderer, &fillRect3);
    SDL_RenderFillRect(renderer, &fillRect4);

	SDL_RenderCopy(renderer, textureTwo, NULL, &dstrectTwo);
	SDL_RenderCopy(renderer, textureThree, NULL, &dstrectThree);
	SDL_RenderCopy(renderer,textureFour,NULL,&dstrectFour);
	SDL_RenderCopy(renderer, textureFive,NULL,&dstrectFive);

}

bool start = true;
bool lose = false;
bool game = true;
bool modeboard =false;
int level = 1;
bool ismusic = true;
void startboard(){
        SDL_Texture* menu=loadTexture("menu.bmp",renderer);
        SDL_Texture* texture1 = loadTexture("gmenu.bmp",renderer);
        SDL_Texture* texture2 = loadTexture("start.bmp",renderer);
        SDL_Texture* m2 = loadTexture("quit.bmp",renderer);
       renderTexture(menu, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        renderTexture(texture1,renderer,100,150,400,150);
        renderTexture(texture2,renderer,150,300,300,100);
        renderTexture(m2,renderer,150,400,300,100);
}
void startgame(){

    SDL_Texture* opensound = loadTexture("open.bmp", renderer);
    SDL_Texture* closesound = loadTexture("close.bmp", renderer);

    if(!(Mix_PlayingMusic())){
        Mix_PlayMusic(music,0);
    }


    while (start){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) {
                start = false;
                lose = false;
                running = false;
                game = false;
                modeboard = false;
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.x>=150 && e.button.x<=450 && e.button.y>=300 && e.button.y<=400){
                    start = false;
                    modeboard = true;
                }
                else if(e.button.x >= 150 && e.button.x <= 450 && e.button.y >= 400 && e.button.y <= 500){
                    SDL_Quit();
                }
                else if(e.button.x>=5 && e.button.x<=55 && e.button.y>=5 && e.button.y<=55){
                    if(ismusic){
                        Mix_PauseMusic();
                        ismusic = !ismusic;
                    }
                    else{
                        Mix_ResumeMusic();
                        ismusic = !ismusic;
                    }
                }
            }
        }


          SDL_RenderClear(renderer);
         startboard();
        if(ismusic) {
            renderTexture(opensound,renderer,5,5,50,50);
             SDL_RenderPresent(renderer);
        }
        else {
            renderTexture(closesound,renderer,5,5,50,50);
             SDL_RenderPresent(renderer);
        }




    }
}

void endgame(int score){
    TTF_Font* font = TTF_OpenFont("arial.ttf", 30);
	SDL_Color color = { 0, 255, 255 };

    SDL_Surface* Quitsurface = TTF_RenderText_Solid(font," QUIT", color);
    SDL_Texture* quitTexture = SDL_CreateTextureFromSurface(renderer,Quitsurface);

    while(lose){
            SDL_Surface* surfaceTwo = TTF_RenderText_Solid(font,
		"SCORE: ", color);
	SDL_Texture* textureTwo = SDL_CreateTextureFromSurface(renderer, surfaceTwo);
        SDL_Texture* opensound = loadTexture("open.bmp", renderer);
        SDL_Texture* closesound = loadTexture("close.bmp", renderer);
        SDL_Surface* surface = TTF_RenderText_Solid(font, "YOU LOSE!", color);
        SDL_Texture* loseMessage = SDL_CreateTextureFromSurface(renderer,surface);
        SDL_Surface* PAsurface = TTF_RenderText_Solid(font, "PLAY AGAIN", color);
        SDL_Texture* PA = SDL_CreateTextureFromSurface(renderer,PAsurface);
        cout<<lose;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) {
                start = false;
                lose = false;
                running = false;
                game = false;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.x>=300 && e.button.x<=550 && e.button.y>=350 && e.button.y<=420){

                         modeboard = true;
                        lose = false;
                }
                else if(e.button.x >= 290 && e.button.x <= 440 && e.button.y >= 450 && e.button.y <= 520){
                        lose = false;
                        game = false;
                }
                else if(e.button.x>=5 && e.button.x<=55 && e.button.y>=5 && e.button.y<=55){
                        if(ismusic){
                            Mix_PauseMusic();
                            ismusic = !ismusic;
                        }
                        else{
                            Mix_ResumeMusic();
                            ismusic = !ismusic;
                        }
                    }
            }
        }
        renderTexture(loseMessage,renderer,250,200,300,100);
        renderTexture(PA, renderer,300,350,200,70 );
        renderTexture(quitTexture,renderer,290,450,150,70);
        if(ismusic) {
            renderTexture(opensound,renderer,5,5,50,50);
        }
        else {
            renderTexture(closesound,renderer,5,5,50,50);
        }
        SDL_RenderPresent(renderer);

    }
}

bool classic = false;
bool rockmode = false;
void gmode(){
        SDL_Texture* menu=loadTexture("menu.bmp",renderer);
        SDL_Texture* mode = loadTexture("mode.bmp",renderer);
        SDL_Texture* mode1 = loadTexture("classic.bmp",renderer);
        SDL_Texture* mode2 = loadTexture("rockmode.bmp",renderer);

        while (modeboard){
            renderTexture(menu, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            renderTexture(mode,renderer,50,200,500,150);
            renderTexture(mode1, renderer,200,350,200,100);
            renderTexture(mode2,renderer,200,430,200,100);
            while(SDL_PollEvent(&e)){
                if(e.type == SDL_QUIT) {
                    start = false;
                    lose = false;
                    running = false;
                    game = false;
                    modeboard = false;
                }
                if(e.type == SDL_MOUSEBUTTONDOWN){
                    if(e.button.x>=200 && e.button.x<=400 && e.button.y>=350 && e.button.y<=450){
                            running = true;
                            modeboard = false;
                            classic = true;
                    }
                    else if(e.button.x >= 200 && e.button.x <= 400 && e.button.y >= 430 && e.button.y <= 530){
                            running = true;
                            modeboard = false;
                            rockmode = true;
                    }
                }
            }
            SDL_RenderPresent(renderer);
        }

}

int main(int argc, char* argv[]){
     initSDL(window, renderer);

     if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		cout<<"MIX ERROR";
		return -1;
	}
	TTF_Init();
	//load sound effect
	music = Mix_LoadMUS("background.mp3");
	if (music == NULL)
	{
		cout<<" Mix_GetError";
	}
	chunk = Mix_LoadWAV("applause.wav");
    if (chunk == NULL)
	{
		cout<<" Mix_GetError";
		return -1;
	}
	//load image

	SDL_Texture* snakefood=loadTexture("food.bmp", renderer);
	SDL_Texture* snakebody=loadTexture("body.bmp", renderer);
    SDL_Texture* snakerock=loadTexture("stone.bmp", renderer);
    SDL_Texture* er1=loadTexture("er1.bmp",renderer);
    SDL_Texture* er2=loadTexture("er2.bmp",renderer);
    SDL_Texture* opensound = loadTexture("open.bmp", renderer);
    SDL_Texture* closesound = loadTexture("close.bmp", renderer);
    SDL_Texture* headup = loadTexture("headup.bmp",renderer);
    SDL_Texture* headdown = loadTexture("headdown.bmp",renderer);
    SDL_Texture* headleft = loadTexture("headleft.bmp",renderer);
    SDL_Texture* headright = loadTexture("headright.bmp",renderer);


    int i=0;


    SDL_Rect head {rand()%700+10,rand()%400 + 120,30,30};
if(head.x>=stone.x &&head.x<=stone.x+stone.w&&head.y>=stone.y&&head.y<=stone.y+stone.w){
        stone.x = rand()%700+10;
        stone.y =rand()%421 + 120;
    }
    while(game){
            startgame();
            gmode();

             int score=0;

    enum Direction
    {
        DOWN,
        LEFT,
        RIGHT,
        UP
    };

    deque<SDL_Rect> rq;
    int size = 3;

    int dir = 0;
    SDL_Rect head {rand()%700+10,rand()%400 + 120,30,30};

    while(running){


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        if(rockmode){
            if((i>=150&&level==1) || (i>=200&&level==2)||(i>=260&&level==3)||(i>=350&&level>=4)) {
                stone.x = rand()%700+10;
                stone.y =rand()%421 + 120;
                stone.w=rand()%61+20;
                stone.h=stone.w;
                i=0;
            }
            if(level==1){
            if(i>=0&&i<=10) SDL_RenderCopy(renderer,er1,NULL,&stone);
            if(i>=11&&i<=20) SDL_RenderCopy(renderer,er2,NULL,&stone);
            if(i>=20&&i<=100) SDL_RenderCopy(renderer,snakerock,NULL,&stone);
        }
        else if(level == 2){
            if(i>=0&&i<=30) SDL_RenderCopy(renderer,er1,NULL,&stone);
            if(i>=31&&i<=60) SDL_RenderCopy(renderer,er2,NULL,&stone);
            if(i>=61&&i<=150) SDL_RenderCopy(renderer,snakerock,NULL,&stone);
        }
        else if(level == 3){
            if(i>=0&&i<=50) SDL_RenderCopy(renderer,er1,NULL,&stone);
            if(i>=51&&i<=81) SDL_RenderCopy(renderer,er2,NULL,&stone);
            if(i>=81&&i<=210) SDL_RenderCopy(renderer,snakerock,NULL,&stone);
        }
        else if(level >= 4){
            if(i>=0&&i<=61) SDL_RenderCopy(renderer,er1,NULL,&stone);
            if(i>=61&&i<=91) SDL_RenderCopy(renderer,er2,NULL,&stone);
            if(i>=92&&i<=300) SDL_RenderCopy(renderer,snakerock,NULL,&stone);
        }
        if(food.x>=stone.x &&food.x<=stone.x+stone.w&&food.y>=stone.y&&food.y<=stone.y+stone.w){
            food.x=rand()%750+10;
            food.y=rand()%451 + 120;
        }
        if((((i>=20&&i<=100)&&level==1) || ((i>=61&&i<=150)&&level==2)||((i>=81&&i<=210)&&level==3)||((i>=92&&i<=300)&&level>=4))
           &&(head.x>=stone.x &&head.x<=stone.x+stone.w&&head.y>=stone.y&&head.y<=stone.y+stone.h/2)){
            running = false;
            lose = true;
        }
        }
        while(SDL_PollEvent(&e)){

            if(e.type == SDL_QUIT) {
                start = false;
                lose = false;
                running = false;
                game = false;
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_DOWN){
                        dir = DOWN;
                }
                if(e.key.keysym.sym == SDLK_UP) {
                        dir = UP;
                }
                if(e.key.keysym.sym == SDLK_LEFT) {
                        dir = LEFT;
                }
                if(e.key.keysym.sym == SDLK_RIGHT) {
                        dir = RIGHT;
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.x>=5 && e.button.x<=55 && e.button.y>=5 && e.button.y<=55){
                    if(ismusic){
                        Mix_PauseMusic();
                        ismusic = !ismusic;
                    }
                    else{
                        Mix_ResumeMusic();
                        ismusic = !ismusic;
                    }
                }
            }
        }
        switch(dir){
            case DOWN:
                head.y += 10;
                 SDL_RenderCopy(renderer,headdown,NULL,&rq[0]);
                break;
            case UP:
                head.y -= 10;
                SDL_RenderCopy(renderer,headup,NULL,&rq[0]);
                break;
            case LEFT:
                head.x -= 10;
                SDL_RenderCopy(renderer,headleft,NULL,&rq[0]);
                break;
            case RIGHT:
                head.x += 10;
                SDL_RenderCopy(renderer,headright,NULL,&rq[0]);
                break;
        }

        if(head.x>=food.x-20 && head.x<=food.x+20 && head.y>=food.y-20 && head.y<=food.y+20 ){
                if(ismusic)
            Mix_PlayChannel(-1, chunk, 0);
            food.x=rand()%750+10;
            food.y=rand()%451 + 120;
            score+=10;
            size+=2;
        }

        for(int j=2; j<size; j+=2){
                 if(head.x==rq[j].x&&head.y==rq[j].y){
                    running = false;
                    lose = true;
                 }
        }
        rq.push_front(head);
        while(rq.size()>size){
            rq.pop_back();
        }
        if(head.x <0||head.x >= 770||head.y >= 570  ||head.y < 110){
            running = false;
            lose = true;
        }
        if(food.x <= 10||food.x >= 760 || food.y >= 560|| food.y <= 120){
            food.x=rand()%750+10;
            food.y=rand()%451 + 120;
            // food.= rand()%chieu cao khung choi + do rong le tren
        }

        if(score<=100) level = 1;
        else if(score>100 && score<=300) level = 2;
        else if(score>300&&score<=500) level = 3;
        else if(score>500 && score<=1000) level = 4;
        else level=5;


        for(int j=3;j<size;j+=2){
                    SDL_RenderCopy(renderer,snakebody,NULL,&rq[j]);
        }



        SDL_RenderCopy(renderer,snakefood,NULL,&food);
        board(score,level);
        if(ismusic) {
            renderTexture(opensound,renderer,5,5,50,50);
        }
        else {
            renderTexture(closesound,renderer,5,5,50,50);
        }
        if(level == 1) SDL_Delay(150);
        else if(level == 2) SDL_Delay(100);
        else if(level == 3) SDL_Delay(70);
        else if(level >= 4) SDL_Delay(50);
        else SDL_Delay(25);
        SDL_RenderPresent(renderer);
        i++;
    }
    endgame(score);
      head.x = rand()%700+10;
      head.y = rand()%401 + 120;
    if(head.x>=stone.x &&head.x<=stone.x+stone.w&&head.y>=stone.y&&head.y<=stone.y+stone.w){
        stone.x = rand()%700+10;
        stone.y =rand()%421 + 120;
    }
    if(rockmode){
    stone.x = rand()%700+10;
    stone.y =rand()%421 + 120;
    stone.w=rand()%61+20;
    stone.h=stone.w;
    i=0;
    }
    }

    SDL_RenderClear(renderer);
    Mix_CloseAudio();
    TTF_Quit();
    quitSDL(window, renderer);

}

