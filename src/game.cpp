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
int vel; //unidades somadas a posicao em cada frame
int velMax;//velocidade andando
int dir; //0 - Norte 1- Nordeste 2- Leste 3- Sudeste 4- Sul 5- Sudoeste 6- Oeste 7- Noroeste
int colliderX, colliderY;//collider a partir do canto inferior esquerdo do sprite
std::string spritePath;//caminho para o arquivo da sprite

public:
Objeto(int px, int py, int sx, int sy, int colx, int coly, std::string sp, int velIni, int velMaxIni, int dirIni);
int getPosX();
int getPosY();
int getSizeX();
int getSizeY();
int getColX();
int getColY();
int getVel();
void setVel(int novaVel);
int getDir();
void setDir(int novaDir);
std::string getSpritePath();
int getVelMax();

void addPos(int nx, int ny);
void subPos(int nx, int ny);
};

Objeto::Objeto(int px, int py, int sx, int sy, int colx, int coly, std::string sp, int velIni,int velMaxIni,int dirIni){
this->posX = px;
this->posY = py;
this->sizeX = sx;
this->sizeY = sy;
this->colliderX = colx;
this->colliderY = coly;
this->spritePath = sp;//caminho para o arquivo de imagem da sprite
this->vel = velIni;
this->velMax = velMaxIni;
this->dir = dirIni;
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
void Objeto::setVel(int novaVel){
  this->vel = novaVel;
}
int Objeto::getVel(){
  return this->vel;
}
void Objeto::setDir(int novaDir){
  this->dir = novaDir;
}
int Objeto::getDir(){
  return this->dir;
}
int Objeto::getVelMax(){
  return this->velMax;
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
void updatePlayer(Objeto & jogador);
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

void Controller::updatePlayer(Objeto & jogador){

  bool parado = true;

  if (this->state[SDL_SCANCODE_LEFT]){
    parado = false;
    jogador.setDir(6);
    jogador.setVel(jogador.getVelMax());
    jogador.subPos(jogador.getVel(),0);
  }

  if (this->state[SDL_SCANCODE_RIGHT]){
    parado = false;
    jogador.setDir(2);
    jogador.setVel(jogador.getVelMax());
    jogador.addPos(jogador.getVel(),0);
  }

  if (this->state[SDL_SCANCODE_UP]){
    parado = false;
    jogador.setDir(0);
    jogador.setVel(jogador.getVelMax());
    jogador.subPos(0,jogador.getVel());
  }

  if (this->state[SDL_SCANCODE_DOWN]){
    parado = false;
    jogador.setDir(4);
    jogador.setVel(jogador.getVelMax());
    jogador.addPos(0,jogador.getVel());
  }

  if(parado){
    jogador.setVel(0);
  }



}


//------------------------------------------------------------------------------



int main(int argc, char* args[]){
int teste;
teste = 0;
Objeto banana(50,50,10,10,10,10,"./banana.png",0,5,0);

View janela;
janela.initView(100,100,600,600);

Controller controle;
janela.render(banana);
//GAME LOOP
while(teste < 1000 && controle.getRodando()){
teste++;

controle.updateInput();
controle.updatePlayer(banana);

janela.render(banana);
}
//GAME LOOP

SDL_Quit();

return 0;

}
