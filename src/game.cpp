//g++ -oGAME game.cpp -lSDL2 -lSDL2_image

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
//
#include <chrono>//tempo para igualar o periodo de cada game loop
#include <thread>         // std::this_thread::sleep_for
//

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
void ObjetoData(int px, int py, int sx, int sy, int colx, int coly, std::string sp, int velIni, int velMaxIni, int dirIni);
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
void Objeto::ObjetoData(int px, int py, int sx, int sy, int colx, int coly, std::string sp, int velIni,int velMaxIni,int dirIni){
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

class Room {
public:
std::string roomName;
std::vector<Objeto> roomObjects;
Objeto & playerCharacter;
Objeto & backgroundScene;
//
Room(std::string iroomName, Objeto & iplayerCharacter,Objeto & ibackgroundScene):roomName(iroomName), playerCharacter(iplayerCharacter), backgroundScene(ibackgroundScene){};
void AddName(std::string iroomName);
void AddObject(Objeto & objetoAdicionado);

};



void Room::AddName(std::string iroomName)
{
  this->roomName = iroomName;
}
void Room::AddObject(Objeto & objetoAdicionado)
{
  this->roomObjects.push_back(objetoAdicionado);
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
  void render(Room & lugar);
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

    SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, 0);//pixel art interpolation

  return 0;
}
void View::render(Room & lugar){
  SDL_Rect targetPlayer;
  targetPlayer.x = lugar.playerCharacter.getPosX();
  targetPlayer.y = lugar.playerCharacter.getPosY();
  targetPlayer.w = lugar.playerCharacter.getSizeX();
  targetPlayer.h = lugar.playerCharacter.getSizeY();

  SDL_Rect targetBackground;
  targetBackground.x = lugar.backgroundScene.getPosX();
  targetBackground.y = lugar.backgroundScene.getPosY();
  targetBackground.w = lugar.backgroundScene.getSizeX();
  targetBackground.h = lugar.backgroundScene.getSizeY();

  // Carregando texturas
//personagem
  SDL_Texture *texturePlayer = IMG_LoadTexture(renderer, lugar.playerCharacter.getSpritePath().c_str());
//fundo
  SDL_Texture *textureBackground = IMG_LoadTexture(renderer, lugar.backgroundScene.getSpritePath().c_str());
/*
  int saberX;
  int saberY;
  SDL_QueryTexture(texture, nullptr, nullptr, &saberX, &saberY); //para saber info sobre a textura
std::cout << "x:" << saberX << "y:" << saberY << std::endl;
*/
// Desenhar a cena
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0,0,255);

//1- Fundo
  SDL_RenderCopy(renderer, textureBackground, nullptr, &targetBackground);
//2 - Personagem
  SDL_RenderCopy(renderer, texturePlayer, nullptr, &targetPlayer);
//3 - Objetos
  SDL_Rect targetObjetos;
  SDL_Texture *textureObjetos;
for(int iterator = 0; iterator < lugar.roomObjects.size(); iterator++){

  targetObjetos.x = lugar.roomObjects[iterator].getPosX();
  targetObjetos.y = lugar.roomObjects[iterator].getPosY();
  targetObjetos.w = lugar.roomObjects[iterator].getSizeX();
  targetObjetos.h = lugar.roomObjects[iterator].getSizeY();
  textureObjetos = IMG_LoadTexture(renderer, lugar.roomObjects[iterator].getSpritePath().c_str());
  SDL_RenderCopy(renderer, textureObjetos, nullptr, &targetObjetos);
  if(lugar.roomObjects[iterator].getPosY() < lugar.playerCharacter.getPosY())
  {
    SDL_RenderCopy(renderer, texturePlayer, nullptr, &targetPlayer);//se jogador esta na frente do objeto, renderizeo de novo
  }
}






//SWAP
  SDL_RenderPresent(renderer);


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
void updatePlayer(Room & lugar);
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
void Controller::updatePlayer(Room & lugar){

  bool parado = true;

  if (this->state[SDL_SCANCODE_LEFT]){
    parado = false;
    lugar.playerCharacter.setDir(6);
    lugar.playerCharacter.setVel(lugar.playerCharacter.getVelMax());
    lugar.playerCharacter.subPos(lugar.playerCharacter.getVel(),0);
  }

  if (this->state[SDL_SCANCODE_RIGHT]){
    parado = false;
    lugar.playerCharacter.setDir(2);
    lugar.playerCharacter.setVel(lugar.playerCharacter.getVelMax());
    lugar.playerCharacter.addPos(lugar.playerCharacter.getVel(),0);
  }

  if (this->state[SDL_SCANCODE_UP]){
    parado = false;
    lugar.playerCharacter.setDir(0);
    lugar.playerCharacter.setVel(lugar.playerCharacter.getVelMax());
    lugar.playerCharacter.subPos(0,lugar.playerCharacter.getVel());
  }

  if (this->state[SDL_SCANCODE_DOWN]){
    parado = false;
    lugar.playerCharacter.setDir(4);
    lugar.playerCharacter.setVel(lugar.playerCharacter.getVelMax());
    lugar.playerCharacter.addPos(0,lugar.playerCharacter.getVel());
  }

  if(parado){
    lugar.playerCharacter.setVel(0);
  }



}


//------------------------------------------------------------------------------




//MAIN--------------------------------------------------------------------------
std::chrono::system_clock::time_point tFinal = std::chrono::system_clock::now();
std::chrono::system_clock::time_point tInicial = std::chrono::system_clock::now();


int main(int argc, char* args[]){
//MODEL
const float fps = 60;
float millisecondsPerFrame =(1/fps)*1000;

Objeto luke;
luke.ObjetoData(50,50,75,125,10,10,"../assets/banana.png",0,5,0);
Objeto fundoBar;
fundoBar.ObjetoData(0,0,780,600,0,0,"../assets/bar.png",0,5,0);

Objeto radio;
radio.ObjetoData(50,51,100,100,80,80,"../assets/radio.jpg",0,5,0);


Room bar("O Bar", luke, fundoBar);
bar.AddObject(radio);



//VIEW
View janela;
janela.initView(100,100,800,800);

Controller controle;

//CONTROLLER
while(controle.getRodando()){
tFinal = std::chrono::system_clock::now();
std::chrono::duration<double, std::milli> spentOnFrame = tFinal - tInicial;
if(spentOnFrame.count() < millisecondsPerFrame )
{
  std::chrono::duration<double, std::milli> delta_ms(millisecondsPerFrame- spentOnFrame.count());
  auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
  std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
}
tInicial = std::chrono::system_clock::now();


//GAME LOOP-----------------------------------------
controle.updateInput();
controle.updatePlayer(bar);
janela.render(bar);
//GAME LOOP!----------------------------------------

}


SDL_Quit();
return 0;
}
