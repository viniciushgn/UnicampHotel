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
class UI{
private:
std::vector<std::string> messagesVector;



};

class Objeto{
private:
int posX,posY;//posicao do canto inferior esquerdo do sprite.
int sizeX,sizeY;
int vel; //unidades somadas a posicao em cada frame
int velMax;//velocidade andando
int dir; //0 - Norte 1- Nordeste 2- Leste 3- Sudeste 4- Sul 5- Sudoeste 6- Oeste 7- Noroeste
int colliderX, colliderY;//collider a partir do canto inferior esquerdo do sprite
std::string spritePath;//caminho para o arquivo da sprite
std::vector<int> spritePointsX;//pontos topleft da sprite
std::vector<int> spritePointsY;//pontos topleft da sprite
int spriteSizeX;
int spriteSizeY;
int estadoSprite;
int estadoSpriteTimer;
bool isCollider;
bool isLinker;
bool linkPendente;
std::string linkPath;
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
void addSpritePoint(int ix, int iy);
int getSpritePointX(int n);
int getSpritePointY(int n);
void setSpriteSize(int nx, int ny);
int getSpriteSizeX();
int getSpriteSizeY();
void setEstadoSprite(int i);
int getEstadoSprite();
void incEstadoTimer();
int getEstadoTimer();
void resetEstadoTimer();
void setCollider();
bool getCollider();
void setLinker(std::string path);
bool getIsLinker();
std::string getLinkPath();
bool getLinkPendente();
void setLinkPendente(std::string path);
};

void Objeto::setLinkPendente(std::string path){
  this->linkPath = path;
  this->linkPendente = true;
}

bool Objeto::getLinkPendente(){
  return this->linkPendente;
}

std::string Objeto::getLinkPath(){
  this->linkPendente = false;
  return this->linkPath;
}

bool Objeto::getIsLinker(){
  return this->isLinker;
}


void Objeto::setLinker(std::string path){
  this->isLinker = true;
  this->linkPath = path;
}


bool Objeto::getCollider(){
  return this->isCollider;
}
void Objeto::setCollider(){
  this->isCollider = true;
}

void Objeto::resetEstadoTimer(){
  this->estadoSpriteTimer = 0;
}

void Objeto::incEstadoTimer(){
  this->estadoSpriteTimer = this->estadoSpriteTimer + 1;
}
int Objeto::getEstadoTimer(){
  return this->estadoSpriteTimer;
}


void Objeto::setEstadoSprite(int i){
  this->estadoSprite = i;
}
int Objeto::getEstadoSprite(){
  return this->estadoSprite;
}
int Objeto::getSpriteSizeY(){
  return this->spriteSizeY;
}
int Objeto::getSpriteSizeX(){
  return this->spriteSizeX;
}
void Objeto::setSpriteSize(int nx, int ny){
  this->spriteSizeX = nx;
  this->spriteSizeY = ny;
}



int Objeto::getSpritePointX(int n){
return this->spritePointsX[n];
}
int Objeto::getSpritePointY(int n){
return this->spritePointsY[n];
}


void Objeto::addSpritePoint(int ix, int iy){
this->spritePointsX.push_back(ix);
this->spritePointsY.push_back(iy);
}
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
this->estadoSpriteTimer = 0;
this->isCollider = false;
this->isLinker = false;
this->linkPendente = false;
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

  SDL_Rect targetPlayer;
  SDL_Rect targetPlayerSprite;
  SDL_Rect targetBackground;

  SDL_Texture *texturePlayer;
  SDL_Texture *textureBackground;
  SDL_Rect targetObjetos;
std::vector<SDL_Texture *> textureObjetos;

public:
  int initView(int posX, int posY, int sizeX, int sizeY);
  void setUpTexture(Room & lugar);
  void resetTexture();
  void render(Room & lugar);
  void changeName(Room & lugar);
};
void View::changeName(Room & lugar){
SDL_SetWindowTitle(this->window, lugar.roomName.c_str());


}
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
void View::setUpTexture(Room & lugar){

  targetPlayer.w = lugar.playerCharacter.getSizeX();
  targetPlayer.h = lugar.playerCharacter.getSizeY();
  targetBackground.w = lugar.backgroundScene.getSizeX();
  targetBackground.h = lugar.backgroundScene.getSizeY();
  targetPlayerSprite.w = lugar.playerCharacter.getSpriteSizeX();
  targetPlayerSprite.h = lugar.playerCharacter.getSpriteSizeY();

  targetPlayerSprite.x = lugar.playerCharacter.getSpritePointX(0);
  targetPlayerSprite.y = lugar.playerCharacter.getSpritePointY(0);

  // Carregando texturas
//personagem
  this->texturePlayer = IMG_LoadTexture(renderer, lugar.playerCharacter.getSpritePath().c_str());
//fundo
  this->textureBackground = IMG_LoadTexture(renderer, lugar.backgroundScene.getSpritePath().c_str());
/*
  int saberX;
  int saberY;
  SDL_QueryTexture(texture, nullptr, nullptr, &saberX, &saberY); //para saber info sobre a textura
std::cout << "x:" << saberX << "y:" << saberY << std::endl;
*/
for(int iterator = 0; iterator < lugar.roomObjects.size(); iterator++){
  targetObjetos.x = lugar.roomObjects[iterator].getPosX();
  targetObjetos.y = lugar.roomObjects[iterator].getPosY();

  textureObjetos.push_back(IMG_LoadTexture(renderer, lugar.roomObjects[iterator].getSpritePath().c_str()));
}
}
void View::resetTexture(){
  this->textureObjetos.clear();
}

void View::render(Room & lugar){

  targetPlayer.x = lugar.playerCharacter.getPosX();
  targetPlayer.y = lugar.playerCharacter.getPosY();
  targetPlayerSprite.x = lugar.playerCharacter.getSpritePointX(lugar.playerCharacter.getEstadoSprite());
  targetPlayerSprite.y = lugar.playerCharacter.getSpritePointY(lugar.playerCharacter.getEstadoSprite());

  targetBackground.x = lugar.backgroundScene.getPosX();
  targetBackground.y = lugar.backgroundScene.getPosY();



// Desenhar a cena
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer,40,40,40,255);

//1- Fundo
  SDL_RenderCopy(renderer, textureBackground, nullptr, &targetBackground);
//2 - Personagem
  SDL_RenderCopy(renderer, texturePlayer, &targetPlayerSprite, &targetPlayer);
//3 - Objetos


for(int iterator = 0; iterator < lugar.roomObjects.size(); iterator++){
  targetObjetos.x = lugar.roomObjects[iterator].getPosX();
  targetObjetos.y = lugar.roomObjects[iterator].getPosY();
  targetObjetos.w = lugar.roomObjects[iterator].getSizeX();
  targetObjetos.h = lugar.roomObjects[iterator].getSizeY();


  SDL_RenderCopy(renderer, textureObjetos[iterator], nullptr, &targetObjetos);

  if((lugar.roomObjects[iterator].getPosY() + lugar.roomObjects[iterator].getSizeY()) < (lugar.playerCharacter.getPosY() + lugar.playerCharacter.getSizeY()))
  {
    if(   (lugar.playerCharacter.getPosX() < (lugar.roomObjects[iterator].getPosX() + lugar.roomObjects[iterator].getSizeX())) && ((lugar.playerCharacter.getPosX() +  lugar.playerCharacter.getSizeX()) > lugar.roomObjects[iterator].getPosX()) )
    {
      if(!lugar.playerCharacter.getLinkPendente()){
      SDL_RenderCopy(renderer, texturePlayer, &targetPlayerSprite, &targetPlayer);//se jogador esta na frente do objeto, renderizeo de novo
    }
    }
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
bool updateRoom(Room & lugar, int & vetorAtual);
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
int direcaoConflito = 9;
bool conflito = false;
bool umaDirecao = true;

for(int iterator = 0; iterator < lugar.roomObjects.size(); iterator++){

if(lugar.roomObjects[iterator].getCollider()){
    if((lugar.roomObjects[iterator].getPosY() ) < (lugar.playerCharacter.getPosY() + lugar.playerCharacter.getSizeY()) && (lugar.roomObjects[iterator].getPosY() + lugar.roomObjects[iterator].getSizeY()  ) > (lugar.playerCharacter.getPosY() + lugar.playerCharacter.getSizeY()))
    {
      if(   (lugar.playerCharacter.getPosX() < (lugar.roomObjects[iterator].getPosX() + lugar.roomObjects[iterator].getSizeX())) && ((lugar.playerCharacter.getPosX() +  lugar.playerCharacter.getSizeX()) > lugar.roomObjects[iterator].getPosX()) )
      {
        direcaoConflito = lugar.playerCharacter.getDir();
        conflito = true;
        if(lugar.roomObjects[iterator].getIsLinker()){
          lugar.playerCharacter.setLinkPendente(lugar.roomObjects[iterator].getLinkPath());
        }
      }
    }
  }
  }




  if (this->state[SDL_SCANCODE_LEFT] && umaDirecao){
    umaDirecao = false;
    parado = false;
    if(conflito == false || direcaoConflito == 2){
    lugar.playerCharacter.setDir(6);
    lugar.playerCharacter.setVel(lugar.playerCharacter.getVelMax());
    lugar.playerCharacter.subPos(lugar.playerCharacter.getVel(),0);
    lugar.playerCharacter.incEstadoTimer();

    if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*2){
          lugar.playerCharacter.setEstadoSprite(3);
        }
    else{
        if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*4){
          lugar.playerCharacter.setEstadoSprite(4);
        }
        else{
          if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*6){
          lugar.playerCharacter.setEstadoSprite(3);
        }
        else{
          if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*8){
            lugar.playerCharacter.setEstadoSprite(5);
          }
          else{
          lugar.playerCharacter.resetEstadoTimer();
        }
        }
      }
    }
    }
}

  if (this->state[SDL_SCANCODE_RIGHT] && umaDirecao ){
    umaDirecao = false;
    parado = false;
    if(conflito == false || direcaoConflito == 6){
    lugar.playerCharacter.setDir(2);
    lugar.playerCharacter.setVel(lugar.playerCharacter.getVelMax());
    lugar.playerCharacter.addPos(lugar.playerCharacter.getVel(),0);
    lugar.playerCharacter.incEstadoTimer();

    if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*2){
          lugar.playerCharacter.setEstadoSprite(6);
        }
    else{
        if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*4){
          lugar.playerCharacter.setEstadoSprite(7);
        }
        else{
          if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*6){
          lugar.playerCharacter.setEstadoSprite(6);
        }
        else{
          if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*8){
            lugar.playerCharacter.setEstadoSprite(8);
          }
          else{
          lugar.playerCharacter.resetEstadoTimer();
        }
        }
      }
    }
  }
}









  if (this->state[SDL_SCANCODE_UP] && umaDirecao ){
    umaDirecao = false;
    parado = false;
    if(conflito == false || direcaoConflito == 4){
    lugar.playerCharacter.setDir(0);
    lugar.playerCharacter.setVel(lugar.playerCharacter.getVelMax());
    lugar.playerCharacter.subPos(0,lugar.playerCharacter.getVel());
    lugar.playerCharacter.incEstadoTimer();

    if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*2){
          lugar.playerCharacter.setEstadoSprite(9);
        }
    else{
        if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*4){
          lugar.playerCharacter.setEstadoSprite(10);
        }
        else{
          if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*6){
          lugar.playerCharacter.setEstadoSprite(9);
        }
        else{
          if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*8){
            lugar.playerCharacter.setEstadoSprite(11);
          }
          else{
          lugar.playerCharacter.resetEstadoTimer();
        }
        }
      }
    }
  }
}

  if (this->state[SDL_SCANCODE_DOWN] && umaDirecao){

    parado = false;
    if(conflito == false || direcaoConflito == 0){
    lugar.playerCharacter.setDir(4);
    lugar.playerCharacter.setVel(lugar.playerCharacter.getVelMax());
    lugar.playerCharacter.addPos(0,lugar.playerCharacter.getVel());
    lugar.playerCharacter.incEstadoTimer();

    if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*2){
          lugar.playerCharacter.setEstadoSprite(0);
        }
    else{
        if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*4){
          lugar.playerCharacter.setEstadoSprite(1);
        }
        else{
          if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*6){
          lugar.playerCharacter.setEstadoSprite(0);
        }
        else{
          if(lugar.playerCharacter.getEstadoTimer() < lugar.playerCharacter.getVel()*8){
            lugar.playerCharacter.setEstadoSprite(2);
          }
          else{
          lugar.playerCharacter.resetEstadoTimer();
        }
        }
      }
    }
  }
}


  if(parado){
    lugar.playerCharacter.setVel(0);
if(lugar.playerCharacter.getEstadoSprite() >= 9){
  lugar.playerCharacter.setEstadoSprite(9);
}
else{
  if(lugar.playerCharacter.getEstadoSprite() >= 6){
      lugar.playerCharacter.setEstadoSprite(6);
  }
  else{
    if(lugar.playerCharacter.getEstadoSprite() >= 3){
      lugar.playerCharacter.setEstadoSprite(3);
    }
    else{
      lugar.playerCharacter.setEstadoSprite(0);
    }
  }
}



    lugar.playerCharacter.resetEstadoTimer();
  }



}

bool Controller::updateRoom(Room & lugar, int & vetorAtual){
  if(lugar.playerCharacter.getLinkPendente()){
vetorAtual = atoi(lugar.playerCharacter.getLinkPath().c_str());
return true;
  }
  else{
    return false;
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

Objeto jogador;
jogador.ObjetoData(175,450,62,116,10,10,"../assets/spriteplayer.png",0,3,0);
jogador.setSpriteSize(31,58);
jogador.addSpritePoint(2,2);jogador.addSpritePoint(39,2);jogador.addSpritePoint(76,2);jogador.addSpritePoint(2,66);jogador.addSpritePoint(39,66);jogador.addSpritePoint(76,66);jogador.addSpritePoint(2,130);jogador.addSpritePoint(39,130);jogador.addSpritePoint(76,130);jogador.addSpritePoint(2,194);jogador.addSpritePoint(39,194);jogador.addSpritePoint(76,194);

Objeto fundoBar;
fundoBar.ObjetoData(0,0,780,600,0,0,"../assets/spriteBar.png",0,5,0);

Objeto colider;
colider.ObjetoData(479,440,300,300,50,50,"../assets/spriteVazia.png",0,0,0);
colider.setCollider();

Objeto colider2;
colider2.ObjetoData(0,0,780,240,50,50,"../assets/spriteVazia.png",0,0,0);
colider2.setCollider();

Objeto colider3;
colider3.ObjetoData(0,0,50,600,50,50,"../assets/spriteVazia.png",0,0,0);
colider3.setCollider();

Objeto colider4;
colider4.ObjetoData(725,0,10,600,50,50,"../assets/spriteVazia.png",0,0,0);
colider4.setCollider();

Objeto link;
link.ObjetoData(155,600,140,60,50,50,"../assets/spriteVazia.png",0,0,0);
link.setCollider();
link.setLinker("1");


Objeto stool;
stool.ObjetoData(423,420,30,51,0,0,"../assets/barStool.png",0,0,0);

Objeto stool2;
stool2.ObjetoData(423,470,30,51,0,0,"../assets/barStool.png",0,0,0);


Objeto barCounter;
barCounter.ObjetoData(0,0,780,600,0,0,"../assets/spriteBarCounter.png",0,5,0);


Room bar("", jogador, fundoBar);

bar.AddObject(colider);
bar.AddObject(colider2);
bar.AddObject(colider3);
bar.AddObject(colider4);
bar.AddObject(stool);
bar.AddObject(stool2);
bar.AddObject(barCounter);
bar.AddObject(link);

//------------------------------------------------------------------------------

Objeto jogador2;
jogador2.ObjetoData(370,100,62,116,10,10,"../assets/spriteplayer.png",0,3,0);
jogador2.setSpriteSize(31,58);
jogador2.addSpritePoint(2,2);jogador2.addSpritePoint(39,2);jogador2.addSpritePoint(76,2);jogador2.addSpritePoint(2,66);jogador2.addSpritePoint(39,66);jogador2.addSpritePoint(76,66);jogador2.addSpritePoint(2,130);jogador2.addSpritePoint(39,130);jogador2.addSpritePoint(76,130);jogador2.addSpritePoint(2,194);jogador2.addSpritePoint(39,194);jogador2.addSpritePoint(76,194);

Objeto fundoBar2;
fundoBar2.ObjetoData(19,0,741,576,0,0,"../assets/spriteHall.png",0,5,0);

Objeto link2;
link2.ObjetoData(340,40,100,20,50,50,"../assets/radio.jpg",0,0,0);
link2.setCollider();
link2.setLinker("0");



Objeto arco;
arco.ObjetoData(283,0,213,243,0,0,"../assets/spriteArco.png",0,0,0);

Room bar2(":.ENTRADA.:", jogador2, fundoBar2);
bar2.AddObject(link2);
bar2.AddObject(arco);

//------------------------------------------------------------------------------





int vetorRoom = 0;
std::vector<Room> gameRooms;
gameRooms.push_back(bar);
gameRooms.push_back(bar2);


//VIEW
View janela;
janela.initView(100,100,780,600);
janela.setUpTexture(gameRooms[vetorRoom]);
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
else{
if(spentOnFrame.count() > millisecondsPerFrame*50)
  {
  SDL_QUIT;
  return 0;}
}
tInicial = std::chrono::system_clock::now();


//GAME LOOP-----------------------------------------
controle.updateInput();
controle.updatePlayer(gameRooms[vetorRoom]);
if(controle.updateRoom(gameRooms[vetorRoom], vetorRoom)){
  janela.resetTexture();
  janela.setUpTexture(gameRooms[vetorRoom]);
  janela.changeName(gameRooms[vetorRoom]);
}
janela.render(gameRooms[vetorRoom]);
//GAME LOOP!----------------------------------------

}


SDL_Quit();
return 0;
}
