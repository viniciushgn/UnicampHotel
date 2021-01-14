//g++ -oGAME game.cpp -lSDL2 -lSDL2_image

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>//tempo para igualar o periodo de cada game loop
#include <thread>// std::this_thread::sleep_for
#include <memory>//lidar com threads

//
#include <boost/asio.hpp> //MULTIPLAYER
using boost::asio::ip::udp;

#include <sstream>//Parse da string recebida pelo server

/*---------------------------MODEL------------------------------------------------*/

class multiplayerSprite{
public:
	int posX,posY; //posicao do canto inferior esquerdo do sprite
	int sizeX,sizeY;
	std::string spritePath;
	int spritePosX, spritePosY;
	int spriteSizeX, spriteSizeY;
	int ID;
	int indexLocal;

	multiplayerSprite(int inputPosX, int inputPosY, int inputSizeX, int inputSizeY,
	std::string inputSpritePath, int inputSpritePosX, int inputSpritePosY, int inputSpriteSizeX, int inputSpriteSizeY, int nIndexLocal, int nID );
  void updateSprite(int nposX, int nposY, int nsizeX, int nsizeY, int nspritePosX, int nspritePosY, int nspriteSizeX, int nSpriteSizeY, int nindexLocal, int nID);

};

multiplayerSprite::multiplayerSprite(int inputPosX, int inputPosY, int inputSizeX,
int inputSizeY,std::string inputSpritePath, int inputSpritePosX, int inputSpritePosY,
int inputSpriteSizeX, int inputSpriteSizeY, int nIndexLocal, int nID){

this->posX = inputPosX;
this->posY = inputPosY;
this->sizeX = inputSizeX;
this->sizeY = inputSizeY;
this->spritePath = inputSpritePath;
this->spritePosX = inputSpritePosX;
this->spritePosY = inputSpritePosY;
this->spriteSizeX = inputSpriteSizeX;
this->spriteSizeY = inputSpriteSizeY;
this->ID = nID;
this->indexLocal = nIndexLocal;

}

void multiplayerSprite::updateSprite(int nposX, int nposY, int nsizeX, int nsizeY, int nspritePosX, int nspritePosY, int nspriteSizeX, int nSpriteSizeY, int nindexLocal, int nID){
	this->posX = nposX;
	this->posY = nposY;
	this->sizeX = nsizeX;
	this->sizeY = nsizeY;
	this->spritePosX = nspritePosX;
	this->spritePosY = nspritePosY;
	this->spriteSizeX = nspriteSizeX;
	this->spriteSizeY = nSpriteSizeY;
	this->ID = nID;
	this->indexLocal = nindexLocal;

}


class Multiplayer{
private:
	std::vector<multiplayerSprite> listaDeJogadores;
	std::string IPHOST;
	int IDmultiplayer;
	boost::asio::io_service io_service;
	udp::endpoint local_endpoint;
	udp::socket meu_socket;
	boost::asio::ip::address ip_remoto;
	udp::endpoint remote_endpoint;
	std::vector<udp::endpoint> paraQuemMandar;
	std::vector<std::string> paraMandar;

public:
void updatePlayer(bool unlock);
std::vector<multiplayerSprite> getListaDeJogadores();
Multiplayer():local_endpoint(udp::v4(), 9001), meu_socket(io_service, local_endpoint){
	std::cout << "--------------UNICAMP HOTEL HOST-------------" << std::endl;
	std::cout << "IP do host do hotel (You):";
	this->local_endpoint = udp::endpoint (udp::v4(), 9001);
	std::cout << this->local_endpoint << std::endl;
	std::cout << "Digite um ID de Jogador:";
	std::cin >> this->IDmultiplayer;
	std::cout << "ID definido. Iniciando hotel..." << std:: endl;



	// Encontrando IP remoto
}
void sendMyData(std::string playerData, bool unlock);
int getIDMultiplayer();
};

int Multiplayer::getIDMultiplayer(){
	return this->IDmultiplayer;
}



std::vector<multiplayerSprite> Multiplayer::getListaDeJogadores(){
	return this->listaDeJogadores;
}

void Multiplayer::updatePlayer(bool unlock){

if(unlock){
	char recv[5000];


	this->meu_socket.receive_from(boost::asio::buffer(recv, 5000), this->remote_endpoint);
	paraQuemMandar.push_back(remote_endpoint);

	std::string recebida(recv);

	int idRecebida;
	bool atualizei = 0;
	std::vector<std::string> parsed;
	std::stringstream ss(recebida);

	while (ss.good()) {
	  std::string substr;
	  std::getline(ss, substr, ',');
	  parsed.push_back(substr);
	  }
	idRecebida = std::stoi(parsed[8]);

	for(int n = 0; n < this->listaDeJogadores.size() && !atualizei; n++){
		if(this->listaDeJogadores[n].ID == idRecebida){
			this->listaDeJogadores[n].updateSprite(std::stoi(parsed[0]), std::stoi(parsed[1]), std::stoi(parsed[2]), std::stoi(parsed[3]), std::stoi(parsed[4]), std::stoi(parsed[5]), std::stoi(parsed[6]), std::stoi(parsed[7]), std::stoi(parsed[8]), std::stoi(parsed[9]));
			atualizei = 1;
			}
	}



	if(!atualizei){
		multiplayerSprite adicionar(std::stoi(parsed[0]), std::stoi(parsed[1]), std::stoi(parsed[2]), std::stoi(parsed[3]),"../assets/spriteplayer.png", std::stoi(parsed[4]), std::stoi(parsed[5]), std::stoi(parsed[6]), std::stoi(parsed[7]), std::stoi(parsed[8]), std::stoi(parsed[9]));
		this->listaDeJogadores.push_back(adicionar);
	}

paraMandar.push_back(recebida);
}
}

void Multiplayer::sendMyData(std::string playerData, bool unlock){
	if(unlock){
while(paraMandar.size() > 0){
	for(int n; n < paraQuemMandar.size(); n++){
	meu_socket.send_to(boost::asio::buffer(playerData), this->paraQuemMandar[n]);
	meu_socket.send_to(boost::asio::buffer(paraMandar[paraMandar.size()]),this->paraQuemMandar[n]);
}
paraMandar.pop_back();
}
}
}


//gameRooms[vetorRoom].playerCharacter.returnPacket(vetorRoom,IDmultiplayer)





// Classe Sprites Simples:
class SpriteSimples{
	private:
		int posX,posY;//posicao do canto inferior esquerdo do sprite.
		int sizeX,sizeY;
		std::string spritePath;//caminho para o arquivo da sprite
		bool isCollider;
		bool isLinker;
		std::string linkPath;
	public:
		void SpriteData(int px, int py, int sx, int sy , std::string sp);
		int getPosX();
		int getPosY();
		int getSizeX();
		int getSizeY();
		std::string getSpritePath();
		void setCollider();
		bool getCollider();
		void setLinker(std::string path);
		bool getIsLinker();
		std::string getLinkPath();
};

std::string SpriteSimples::getLinkPath(){
  return this->linkPath;
}

bool SpriteSimples::getIsLinker(){
  return this->isLinker;
}

void SpriteSimples::setLinker(std::string path){
  this->isLinker = true;
  this->linkPath = path;
}


bool SpriteSimples::getCollider(){
  return this->isCollider;
}

void SpriteSimples::setCollider(){
  this->isCollider = true;
}

void SpriteSimples::SpriteData(int px, int py, int sx, int sy , std::string sp){
	this->posX = px;
	this->posY = py;
	this->sizeX = sx;
	this->sizeY = sy;
	this->spritePath = sp;//caminho para o arquivo de imagem da sprite
	this->isCollider = false;
	this->isLinker = false;
}

int SpriteSimples::getPosX(){
  return this->posX;
}

int SpriteSimples::getPosY(){
  return this->posY;
}

int SpriteSimples::getSizeX(){
  return this->sizeX;
}

int SpriteSimples::getSizeY(){
  return this->sizeY;
}

std::string SpriteSimples::getSpritePath(){
  return this->spritePath;
}

// Classe Player:
class Player{
	private:
		int posX,posY;//posicao do canto inferior esquerdo do sprite.
		int sizeX,sizeY;
		int vel; //unidades somadas a posicao em cada frame
		int velMax;//velocidade andando
		int dir; //0 - Norte 1- Nordeste 2- Leste 3- Sudeste 4- Sul 5- Sudoeste 6- Oeste 7- Noroeste
		std::string spritePath;//caminho para o arquivo da sprite
		std::vector<int> spritePointsX;//pontos topleft da sprite
		std::vector<int> spritePointsY;//pontos topleft da sprite
		int spriteSizeX;
		int spriteSizeY;
		int estadoSprite;
		int estadoSpriteTimer;
		bool linkPendente;
		std::string linkPath;
	public:
		void PlayerData(int px, int py, int sx, int sy, std::string sp, int velIni, int velMaxIni, int dirIni);
		int getPosX();
		int getPosY();
		int getSizeX();
		int getSizeY();
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
		void setLinker(std::string path);
		bool getIsLinker();
		std::string getLinkPath();
		bool getLinkPendente();
		void setLinkPendente(std::string path);

		std::string returnPacket(int local, int id);
};


std::string Player::returnPacket(int local, int id){

std::string pack = std::to_string(this->posX) + "," + std::to_string(this->posY)
+ "," + std::to_string(this->sizeX) + "," + std::to_string(this->sizeY) + ","

+ std::to_string(this->spritePointsX[this->estadoSprite]) + "," + std::to_string(this->spritePointsY[this->estadoSprite]) + ","

+ std::to_string(this->spriteSizeX) + "," + std::to_string(this->spriteSizeY) + ","
+ std::to_string(local) + "," + std::to_string(id);

return pack;

}


void Player::setLinkPendente(std::string path){
  this->linkPath = path;
  this->linkPendente = true;
}

bool Player::getLinkPendente(){
  return this->linkPendente;
}

std::string Player::getLinkPath(){
  this->linkPendente = false;
  return this->linkPath;
}


void Player::setLinker(std::string path){
  this->linkPath = path;
}

void Player::resetEstadoTimer(){
  this->estadoSpriteTimer = 0;
}

void Player::incEstadoTimer(){
  this->estadoSpriteTimer = this->estadoSpriteTimer + 1;
}

int Player::getEstadoTimer(){
  return this->estadoSpriteTimer;
}

void Player::setEstadoSprite(int i){
  this->estadoSprite = i;
}

int Player::getEstadoSprite(){
  return this->estadoSprite;
}

int Player::getSpriteSizeY(){
  return this->spriteSizeY;
}

int Player::getSpriteSizeX(){
  return this->spriteSizeX;
}

void Player::setSpriteSize(int nx, int ny){
  this->spriteSizeX = nx;
  this->spriteSizeY = ny;
}

int Player::getSpritePointX(int n){
	if(n > this->spritePointsX.size()){
		printf("erro aqui\n");
	}
	return this->spritePointsX[n];
}

int Player::getSpritePointY(int n){
	return this->spritePointsY[n];
}

void Player::addSpritePoint(int ix, int iy){
	this->spritePointsX.push_back(ix);
	this->spritePointsY.push_back(iy);
}

void Player::PlayerData(int px, int py, int sx, int sy, std::string sp, int velIni, int velMaxIni, int dirIni){
	this->posX = px;
	this->posY = py;
	this->sizeX = sx;
	this->sizeY = sy;
	this->spritePath = sp;//caminho para o arquivo de imagem da sprite
	this->vel = velIni;
	this->velMax = velMaxIni;
	this->dir = dirIni;
	this->estadoSpriteTimer = 0;
	this->estadoSprite = 0;
	this->linkPendente = false;
}

int Player::getPosX(){
  return this->posX;
}

int Player::getPosY(){
  return this->posY;
}

int Player::getSizeX(){
  return this->sizeX;
}

int Player::getSizeY(){
  return this->sizeY;
}

std::string Player::getSpritePath(){
  return this->spritePath;
}
void Player::setVel(int novaVel){
  this->vel = novaVel;
}

int Player::getVel(){
  return this->vel;
}

void Player::setDir(int novaDir){
  this->dir = novaDir;
}

int Player::getDir(){
  return this->dir;
}

int Player::getVelMax(){
  return this->velMax;
}

void Player::addPos(int nx, int ny){
  this->posX = this->posX + nx;
  this->posY = this->posY + ny;
}

void Player::subPos(int nx, int ny){
  this->posX = this->posX - nx;
  this->posY = this->posY - ny;
}


// Classe Room:
class Room {
	public:
		std::string roomName;
		std::vector<SpriteSimples> roomObjects;
		Player & playerCharacter;
		SpriteSimples & backgroundScene;
		//
		Room(std::string iroomName, Player & iplayerCharacter,SpriteSimples & ibackgroundScene):roomName(iroomName), playerCharacter(iplayerCharacter), backgroundScene(ibackgroundScene){};
		void AddName(std::string iroomName);
		void AddObject(SpriteSimples & objetoAdicionado);
};

void Room::AddName(std::string iroomName){
  this->roomName = iroomName;
}

void Room::AddObject(SpriteSimples & objetoAdicionado){
  this->roomObjects.push_back(objetoAdicionado);
}

/*--------------------------------------------------------------------------------*/



/*-----------------------------VIEW----------------------------------------------*/
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

		SDL_Rect targetNPC;
		SDL_Rect targetNPCSprite;
		std::vector<SDL_Texture *> textureNPC;
		int indexPlayer;

	public:
		int initView(int posX, int posY, int sizeX, int sizeY);
		void setUpTexture(Room & lugar);
		void resetTexture();
		void render(Room & lugar, std::vector<multiplayerSprite>  nlistaDeJogadores);
		void changeName(Room & lugar);
		void setindexPlayer(int nindex);

		void setUpNPC(std::vector<multiplayerSprite> nlistaDeJogadores);
		void resetNPC();
		void renderNPC( std::vector<multiplayerSprite> listaDeJogadores);
};


void View::setindexPlayer(int nindex){
	this->indexPlayer = nindex;
}

void View::setUpNPC(std::vector<multiplayerSprite> nlistaDeJogadores){
for(int iterator = 0; iterator < nlistaDeJogadores.size(); iterator++){
	if(nlistaDeJogadores[iterator].indexLocal == this->indexPlayer){
	textureNPC.push_back(IMG_LoadTexture(renderer, nlistaDeJogadores[iterator].spritePath.c_str()));
}
}
}

void View::resetNPC(){
this->textureNPC.clear();
}



void View::changeName(Room & lugar){
	SDL_SetWindowTitle(this->window, lugar.roomName.c_str());
}

int View::initView(int posX, int posY, int sizeX, int sizeY){
	this->indexPlayer = 0;
  //Inicializando SDL
  if( SDL_Init(SDL_INIT_EVERYTHING) != 0){
    std::cout << "Fail:" << SDL_GetError() << std::endl;
    return -1;
  }

  //Criando Janela
  window = SDL_CreateWindow("Unicamp Hotel", posX,posY,sizeX,sizeY,0);

  if(window == nullptr){
    std::cout << "Erro: " << SDL_GetError();
    return -1;
  }

  //Criar e Inicializar Renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == nullptr){
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

void View::render(Room & lugar, std::vector<multiplayerSprite> nlistaDeJogadores ){
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

		if((lugar.roomObjects[iterator].getPosY() + lugar.roomObjects[iterator].getSizeY()) < (lugar.playerCharacter.getPosY() + lugar.playerCharacter.getSizeY())){
		  if(   (lugar.playerCharacter.getPosX() < (lugar.roomObjects[iterator].getPosX() + lugar.roomObjects[iterator].getSizeX())) && ((lugar.playerCharacter.getPosX() +  lugar.playerCharacter.getSizeX()) > lugar.roomObjects[iterator].getPosX()) ){
		    if(!lugar.playerCharacter.getLinkPendente()){
		    SDL_RenderCopy(renderer, texturePlayer, &targetPlayerSprite, &targetPlayer);//se jogador esta na frente do objeto, renderizeo de novo
		  	}
		  }
		}
	}

//NPC-------------------------------------------------------------
for(int iterator = 0; iterator < nlistaDeJogadores.size(); iterator++){

	if(nlistaDeJogadores[iterator].indexLocal == this->indexPlayer){
	targetNPC.x = nlistaDeJogadores[iterator].posX;
	targetNPC.y = nlistaDeJogadores[iterator].posY;
	targetNPC.w = nlistaDeJogadores[iterator].sizeX;
	targetNPC.h = nlistaDeJogadores[iterator].sizeY;

	targetNPCSprite.x = nlistaDeJogadores[iterator].spritePosX;
	targetNPCSprite.y = nlistaDeJogadores[iterator].spritePosY;
	targetNPCSprite.w = nlistaDeJogadores[iterator].spriteSizeX;
	targetNPCSprite.h = nlistaDeJogadores[iterator].spriteSizeY;


	SDL_RenderCopy(renderer, textureNPC[iterator], &targetNPCSprite, &targetNPC);

}
}
//----------------------------------------------------------------


	//SWAP
  SDL_RenderPresent(renderer);
}




/*--------------------------------------------------------------------------------*/


/*---------------------CONTROLLER-------------------------------------------------*/
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

	while (SDL_PollEvent(&evento)){ //tem eventos na fila?
		if (evento.type == SDL_QUIT){ //se tiver joga fora, se for SDL_QUIT saia
			rodando = false;
  	}
  }
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
			if((lugar.roomObjects[iterator].getPosY() ) < (lugar.playerCharacter.getPosY() + lugar.playerCharacter.getSizeY()) && (lugar.roomObjects[iterator].getPosY() + lugar.roomObjects[iterator].getSizeY()  ) > (lugar.playerCharacter.getPosY() + lugar.playerCharacter.getSizeY())){
				if(   (lugar.playerCharacter.getPosX() < (lugar.roomObjects[iterator].getPosX() + lugar.roomObjects[iterator].getSizeX())) && ((lugar.playerCharacter.getPosX() +  lugar.playerCharacter.getSizeX()) > lugar.roomObjects[iterator].getPosX()) ){
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



/*---------------------------------------------------------------------*/


//MAIN--------------------------------------------------------------------------
std::chrono::system_clock::time_point tFinal = std::chrono::system_clock::now();
std::chrono::system_clock::time_point tInicial = std::chrono::system_clock::now();

int main(int argc, char* args[]){


//DIGITAR IP DO SERVIDOR--------------------------------------------------------

//------------------------------------------------------------------------------





	//MODEL
	const float fps = 60;
	float millisecondsPerFrame =(1/fps)*1000;


	Player jogador;
	jogador.PlayerData(175,450,62,116,"../assets/spriteplayer.png",0,3,0);
	jogador.setSpriteSize(31,58);
	jogador.addSpritePoint(2,2);jogador.addSpritePoint(39,2);jogador.addSpritePoint(76,2);jogador.addSpritePoint(2,66);jogador.addSpritePoint(39,66);jogador.addSpritePoint(76,66);jogador.addSpritePoint(2,130);jogador.addSpritePoint(39,130);jogador.addSpritePoint(76,130);jogador.addSpritePoint(2,194);jogador.addSpritePoint(39,194);jogador.addSpritePoint(76,194);

	SpriteSimples fundoBar;
	fundoBar.SpriteData(0,0,780,600,"../assets/spriteBar.png");

	SpriteSimples colider;
	colider.SpriteData(479,440,300,300,"../assets/spriteVazia.png");
	colider.setCollider();

	SpriteSimples colider2;
	colider2.SpriteData(0,0,780,240,"../assets/spriteVazia.png");
	colider2.setCollider();

	SpriteSimples colider3;
	colider3.SpriteData(0,0,50,600,"../assets/spriteVazia.png");
	colider3.setCollider();

	SpriteSimples colider4;
	colider4.SpriteData(725,0,10,600,"../assets/spriteVazia.png");
	colider4.setCollider();

	SpriteSimples link;
	link.SpriteData(155,600,140,60,"../assets/spriteVazia.png");
	link.setCollider();
	link.setLinker("1");

	SpriteSimples stool;
	stool.SpriteData(423,420,30,51,"../assets/barStool.png");

	SpriteSimples stool2;
	stool2.SpriteData(423,470,30,51,"../assets/barStool.png");

	SpriteSimples barCounter;
	barCounter.SpriteData(0,0,780,600,"../assets/spriteBarCounter.png");

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

	Player jogador2;
	jogador2.PlayerData(370,100,62,116,"../assets/spriteplayer.png",0,3,0);
	jogador2.setSpriteSize(31,58);
	jogador2.addSpritePoint(2,2);jogador2.addSpritePoint(39,2);jogador2.addSpritePoint(76,2);jogador2.addSpritePoint(2,66);jogador2.addSpritePoint(39,66);jogador2.addSpritePoint(76,66);jogador2.addSpritePoint(2,130);jogador2.addSpritePoint(39,130);jogador2.addSpritePoint(76,130);jogador2.addSpritePoint(2,194);jogador2.addSpritePoint(39,194);jogador2.addSpritePoint(76,194);

	SpriteSimples fundoBar2;
	fundoBar2.SpriteData(19,0,741,576,"../assets/spriteHall.png");

	SpriteSimples link2;
	link2.SpriteData(290,40,200,20,"../assets/radio.jpg");
	link2.setCollider();
	link2.setLinker("0");

	SpriteSimples link3;
	link3.SpriteData(290,600,200,20,"../assets/radio.jpg");
	link3.setCollider();
	link3.setLinker("2");



	SpriteSimples arco;
	arco.SpriteData(283,0,213,243,"../assets/spriteArco.png");

	SpriteSimples colider5;
	colider5.SpriteData(0,560,800,800,"../assets/radio.jpg");
	colider5.setCollider();

	SpriteSimples colider6;
	colider6.SpriteData(0,0,40,600,"../assets/spriteVazia.png");
	colider6.setCollider();

	SpriteSimples colider7;
	colider7.SpriteData(740,0,50,800,"../assets/spriteVazia.png");
	colider7.setCollider();

	SpriteSimples colider8;
	colider8.SpriteData(0,0,290,260,"../assets/spriteVazia.jpg");
	colider8.setCollider();

	SpriteSimples colider9;
	colider9.SpriteData(480,0,290,260,"../assets/spriteVazia.jpg");
	colider9.setCollider();

	SpriteSimples colider10;
	colider10.SpriteData(520,125,50,260,"../assets/spriteVazia.jpg");
	colider10.setCollider();

	Room bar2(":.ENTRADA.:", jogador2, fundoBar2);
	bar2.AddObject(link2);
	bar2.AddObject(arco);
	bar2.AddObject(colider5);
	bar2.AddObject(colider6);
	bar2.AddObject(colider7);
	bar2.AddObject(colider8);
	bar2.AddObject(colider9);
	bar2.AddObject(colider10);
 	bar2.AddObject(link3);

	//------------------------------------------------------------------------------

	Player jogador3;
	jogador3.PlayerData(370,100,62,116,"../assets/spriteplayer.png",0,3,0);
	jogador3.setSpriteSize(31,58);
	jogador3.addSpritePoint(2,2);jogador3.addSpritePoint(39,2);jogador3.addSpritePoint(76,2);jogador3.addSpritePoint(2,66);jogador3.addSpritePoint(39,66);jogador3.addSpritePoint(76,66);jogador3.addSpritePoint(2,130);jogador3.addSpritePoint(39,130);jogador3.addSpritePoint(76,130);jogador3.addSpritePoint(2,194);jogador3.addSpritePoint(39,194);jogador3.addSpritePoint(76,194);

	SpriteSimples fundoBar3;
	fundoBar3.SpriteData(19,0,741,576,"../assets/spriteHall.png");

	Room bar3(":.LIMBO.:", jogador3, fundoBar3);




	//----------------------------------------------------------------------------


	bool unlockReceive = 0;
	bool canSend = 0;
	bool unlockSend = 0;
	int vetorRoom = 1;
	std::vector<Room> gameRooms;

	gameRooms.push_back(bar);
	gameRooms.push_back(bar2);
	gameRooms.push_back(bar3);

	//VIEW
	View janela;
	janela.initView(100,100,780,600);
	janela.setUpTexture(gameRooms[vetorRoom]);
	//CONTROLLER
	Controller controle;
	Multiplayer controleMultiplayer;


	while(controle.getRodando()){

//controleMultiplayer.updatePlayer();

std::thread t1(&Multiplayer::updatePlayer, &controleMultiplayer, unlockReceive);
if(unlockReceive){
	canSend = 1;
}
std::thread t2(&Multiplayer::sendMyData, &controleMultiplayer,gameRooms[vetorRoom].playerCharacter.returnPacket(vetorRoom,controleMultiplayer.getIDMultiplayer()), unlockSend);
//		controleMultiplayer.updatePlayer();

		tFinal = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> spentOnFrame = tFinal - tInicial;
		if(spentOnFrame.count() < millisecondsPerFrame ){
			std::chrono::duration<double, std::milli> delta_ms(millisecondsPerFrame- spentOnFrame.count());
			auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
			std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
		}
		else{
			if(spentOnFrame.count() > millisecondsPerFrame*50){
				//SDL_QUIT;
			//	return 0;
			}
		}
		tInicial = std::chrono::system_clock::now();

		//GAME LOOP-----------------------------------------
		controle.updateInput();
		controle.updatePlayer(gameRooms[vetorRoom]);
		if(controle.updateRoom(gameRooms[vetorRoom], vetorRoom)){
			unlockReceive = 1;
			janela.resetTexture();
			janela.setUpTexture(gameRooms[vetorRoom]);
			janela.changeName(gameRooms[vetorRoom]);
			janela.setindexPlayer(vetorRoom);
			janela.resetNPC();
			janela.setUpNPC(controleMultiplayer.getListaDeJogadores());
		}
		janela.render(gameRooms[vetorRoom], controleMultiplayer.getListaDeJogadores());
		//GAME LOOP!----------------------------------------
		//MULTIPLAYER LOOP!---------------------------------

		//mandar estado do jogador para o servidor

	//	controleMultiplayer.sendMyData();

t1.join();
t2.join();
if(canSend){
	unlockSend = 1;
}
		//MULTIPLAYER LOOP!---------------------------------
	}

	SDL_Quit();
	return 0;
}
