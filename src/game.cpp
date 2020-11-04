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
