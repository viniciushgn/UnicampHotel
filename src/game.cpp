//g++ -oGAME game.cpp -lSDL2 -lSDL2_image

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

//MODEL-------------------------------------------------------------------------
class Objeto{
private:
int posX,posY;//posicao do canto inferior esquerdo do sprite.
int sizeX,sizeY;
int colliderX, colliderY;//collider a partir do canto inferior esquerdo do sprite
std::string spritePath;//caminho para o arquivo da sprite

public:
Objeto(int px, int py, int sx, int sy, int colx, int coly, std::string sp);
int getPosX();
int getPosY();
int getSizeX();
int getSizeY();
int getColX();
int getColY();
std::string getSpritePath();

void addPos(int nx, int ny);
void subPos(int nx, int ny);
};

Objeto::Objeto(int px, int py, int sx, int sy, int colx, int coly, std::string sp){
this->posX = px;
this->posY = py;
this->sizeX = sx;
this->sizeY = sy;
this->colliderX = colx;
this->colliderY = coly;
this->spritePath = sp;//caminho para o arquivo de imagem da sprite
}
int Objeto::getPosX(){
  return this->posX;
}
int Objeto::getPosY(){
  return this->posY;
}
int Objeto::getSizeX(){
  return this->sizeX;
}
int Objeto::getSizeY(){
  return this->sizeY;
}
int Objeto::getColX(){
  return this->colliderX;
}
int Objeto::getColY(){
  return this->colliderY;
}
std::string Objeto::getSpritePath(){
  return this->spritePath;
}
void Objeto::addPos(int nx, int ny){
  this->posX = this->posX + nx;
  this->posY = this->posY + ny;
}

void Objeto::subPos(int nx, int ny){
  this->posX = this->posX - nx;
  this->posY = this->posY - ny;
}


//------------------------------------------------------------------------------



//VIEW--------------------------------------------------------------------------
class View {
private:
  //Setando Window e Renderer
  SDL_Window* window;
  SDL_Renderer* renderer;

public:
  int initView(int posX, int posY, int sizeX, int sizeY);
  void render(Objeto square);
};
int View::initView(int posX, int posY, int sizeX, int sizeY){


  //Inicializando SDL
  if( SDL_Init(SDL_INIT_EVERYTHING) != 0){
    std::cout << "Fail:" << SDL_GetError() << std::endl;
    return -1;
  }
  //Criando Janela
  window = SDL_CreateWindow("Unicamp Hotel", posX,posY,sizeX,sizeY,0);

  if(window == nullptr)
  {
    std::cout << "Erro: " << SDL_GetError();
    return -1;
  }

  //Criar e Inicializar Renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == nullptr)
  {
    std::cout << "Erro: " << SDL_GetError();
    return -1;
  }
  //Setando resolucao
    SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);
  return 0;
}
void View::render(Objeto square){
  SDL_Rect target;
  target.x = square.getPosX();
  target.y = square.getPosY();
  // Carregando texturas
  // personagem
  SDL_Texture *texture = IMG_LoadTexture(renderer, square.getSpritePath().c_str());

  SDL_QueryTexture(texture, nullptr, nullptr, &target.w, &target.h);

  // fundo
  //SDL_Texture *texture2 = IMG_LoadTexture(renderer, "./park.jpeg");

  // Desenhar a cena
  SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0,0,255);
  //SDL_RenderCopy(renderer, texture2, nullptr, nullptr);

  SDL_RenderCopy(renderer, texture, nullptr, &target);
  SDL_RenderPresent(renderer);

  // Delay para diminuir o framerate
  SDL_Delay(10);


}
//------------------------------------------------------------------------------

//CONTROLLER--------------------------------------------------------------------
//this->sprite = IMG_LoadTexture(renderer1, sp.c_str());//endereco da textura

class Controller {
private:

// Variaveis para verificar eventos
SDL_Event evento; // eventos discretos
const Uint8* state = SDL_GetKeyboardState(nullptr); // estado do teclado
// Controlador:
bool rodando = true;

public:
bool getRodando();
void updateInput();
const Uint8* getState();
};

bool Controller::getRodando(){
  return this->rodando;
}

void Controller::updateInput(){
    SDL_PumpEvents(); // atualiza estado do teclado

while (SDL_PollEvent(&evento)) {//tem eventos na fila?
      if (evento.type == SDL_QUIT) {//se tiver joga fora, se for SDL_QUIT saia
        rodando = false;
      }}
}

const Uint8* Controller::getState(){
  return this->state;
}


//------------------------------------------------------------------------------



int main(int argc, char* args[]){
int teste;
teste = 0;
Objeto banana(50,50,10,10,10,10,"./banana.png");

View janela;
janela.initView(100,100,600,600);

Controller controle;
janela.render(banana);

while(teste < 1000 && controle.getRodando()){
teste++;

controle.updateInput();

if (controle.getState()[SDL_SCANCODE_LEFT]){
  banana.subPos(1,0);}
if (controle.getState()[SDL_SCANCODE_RIGHT]) {
  banana.addPos(1,0);}
if (controle.getState()[SDL_SCANCODE_UP]) {
  banana.subPos(0,1);}
if (controle.getState()[SDL_SCANCODE_DOWN]) {
  banana.addPos(0,1);}

janela.render(banana);
}


SDL_Quit();

return 0;

}





/*
bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();

void Render();
void RunGame();

void AddEnemy();
void MoveEnemies();

void ResetPlayerPos();

bool CheckCollision( const SDL_Rect &rect1, const SDL_Rect &rect2 );
bool CheckEnemyCollisions();

// Window pos
int posX = 900;
int posY = 300;
int sizeX = 300;
int sizeY = 400;

int movementFactor = 15; // Amount of pixels the player move per key press
int lastEnemyPos = 50;

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Rect playerPos;
SDL_Rect topBar;
SDL_Rect bottomBar;

enum class Direction
{
	Left,
	Right
};
struct Enemy
{
	Enemy( SDL_Rect pos_, int speed_, Direction dir_ )
	{
		pos = pos_;
		speed = speed_;
		dir = dir_;
	}

	SDL_Rect pos;
	int speed;
	Direction dir;
};

std::vector< Enemy > enemies;

int main( int argc, char* args[] )
{
	if ( !InitEverything() )
		return -1;

	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();

	// Init top and bottom bar
	topBar.x = 0;
	topBar.y = 0;
	topBar.w = sizeX;
	topBar.h = 20;

	bottomBar.x = 0;
	bottomBar.y = sizeY - 20;
	bottomBar.w = sizeX;
	bottomBar.h = 20;

	// Initlaize our player
	playerPos.w = 20;
	playerPos.h = 20;
	ResetPlayerPos();

	RunGame();
}
void RunGame()
{
	bool loop = true;

	while ( loop )
	{
		SDL_Event event;
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT )
				loop = false;
			else if ( event.type == SDL_KEYDOWN )
			{
				switch ( event.key.keysym.sym )
				{
					case SDLK_RIGHT:
						playerPos.x += movementFactor;
						break;
					case SDLK_LEFT:
						playerPos.x -= movementFactor;
						break;
						// Remeber 0,0 in SDL is left-top. So when the user pressus down, the y need to increase
					case SDLK_DOWN:
						playerPos.y += movementFactor;
						break;
					case SDLK_UP:
						playerPos.y -= movementFactor;
						break;
					default :
						break;
				}
			}
		}

		MoveEnemies();

		// Check collisions against enemies
		if ( CheckEnemyCollisions() )
			ResetPlayerPos();

		// Check collusion against bottom bar
		// Since top bar covers the entire width, we only need to check the y value
		// topBar.y refers to top of top bar ( top of the screen )
		// Since 0,0 is the top-right we need to add topBar.h to find the bottom of topBar
		if ( playerPos.y < ( topBar.y + topBar.h) )
			ResetPlayerPos();

		Render();

		// Add a 16msec delay to make our game run at ~60 fps
		SDL_Delay( 16 );
	}
}
void Render()
{
	// Clear the window and make it all red
	SDL_RenderClear( renderer );

	// Change color to black!
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

	// Render top and bottom bar
	SDL_RenderFillRect( renderer, &bottomBar );
	SDL_RenderFillRect( renderer, &topBar );

	// Change color to blue!
	SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );

	// Render our "player"
	SDL_RenderFillRect( renderer, &playerPos );

	// Change color to red!
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

	for ( const auto &p : enemies )
		SDL_RenderFillRect( renderer, &p.pos );

	// Change color to green!
	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );

	// Render the changes above
	SDL_RenderPresent( renderer);
}
bool InitEverything()
{
	if ( !InitSDL() )
		return false;

	if ( !CreateWindow() )
		return false;

	if ( !CreateRenderer() )
		return false;

	SetupRenderer();

	return true;
}
bool InitSDL()
{
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}
bool CreateWindow()
{
	window = SDL_CreateWindow( "Server", posX, posY, sizeX, sizeY, 0 );

	if ( window == nullptr )
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return false;
	}

	return true;
}
bool CreateRenderer()
{
	renderer = SDL_CreateRenderer( window, -1, 0 );

	if ( renderer == nullptr )
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return false;
	}

	return true;
}
void SetupRenderer()
{
	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, sizeX, sizeY );

	// Set color of renderer to red
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
}
void MoveEnemies()
{
	for (  auto &p : enemies )
	{
		if ( p.dir == Direction::Right )
		{
			p.pos.x += p.speed;

			if ( p.pos.x >= sizeX )
				p.pos.x = 0;
		}
		else
		{
			p.pos.x -= p.speed;

			if ( ( p.pos.x + p.pos.w )  <= 0 )
				p.pos.x = sizeX - p.pos.w;
		}
	}
}
bool CheckCollision( const SDL_Rect &rect1, const SDL_Rect &rect2 )
{
	// Find edges of rect1
	int left1 = rect1.x;
	int right1 = rect1.x + rect1.w;
	int top1 = rect1.y;
	int bottom1 = rect1.y + rect1.h;

	// Find edges of rect2
	int left2 = rect2.x;
	int right2 = rect2.x + rect2.w;
	int top2 = rect2.y;
	int bottom2 = rect2.y + rect2.h;

	// Check edges
	if ( left1 > right2 )// Left 1 is right of right 2
		return false; // No collision

	if ( right1 < left2 ) // Right 1 is left of left 2
		return false; // No collision

	if ( top1 > bottom2 ) // Top 1 is below bottom 2
		return false; // No collision

	if ( bottom1 < top2 ) // Bottom 1 is above top 2
		return false; // No collision

	return true;
}
bool CheckEnemyCollisions()
{
	for ( const auto &p : enemies )
	{
		if ( CheckCollision( p.pos, playerPos) )
			return true;
	}

	return false;
}
void AddEnemy()
{
	if ( ( rand() % 2 ) == 0 )
	{
		enemies.push_back( Enemy( { rand() % 300, lastEnemyPos, 20, 20 }, 1, Direction::Right )  );
	}
	else
	{
		enemies.push_back( Enemy( { rand() % 300, lastEnemyPos, 20, 20 }, 1, Direction::Left )  );
	}

	lastEnemyPos += 25;
}
void ResetPlayerPos()
{
	// sizeX / 2 = middle pixel of the screen
	// playerPos.w / 2 = middle of the player
	// So setting player x pos to [middle of screen] - [middle of player] means it will be centerd in the screen.
	playerPos.x = ( sizeX / 2 ) - ( playerPos.w / 2 );
	playerPos.y = sizeY - bottomBar.h;

}*/
