#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <QAudioBuffer>
#include <QByteArray>
#include <ctime>
#include "Lista.h"
#include "Lista.cpp"
#include <QFileInfo>

using namespace std;

struct Superblock
{
	char particion;
	char nombre[25];
	int size;
    int used_blocks;
    int free_blocks;
	int block_size;
	int cant_block;
    int size_bitmap;
    int inicioInodeTable;
    int finInodeTable;
    int inicioInode;
    int finInode;
    int inicioDataBlocks;
    int num_inodos;
    int inodos_libres;
    int canciones_cargadas;
};

struct InodeOnTable
{
    char fileName[35];
    int inumber;
    bool available;


    InodeOnTable()
    {
        this->inumber = -1;
        this->available = false;
    }
};

struct Inode
{
    char title[30];
    char artist[30];
    char album[30];
    char year[5];
    char permissions[2];
    char fechaC[10];
    char fechaM[10];
    int size;
    int bloques_usados;
    int bloques_directos[10];
    int indirectos_simples;
    int indirectos_dobles;

    Inode()
    {
        this->size = -1;
        this->bloques_usados = -1;
        this->indirectos_simples = -1;
        this->indirectos_dobles = -1;
    }
};

class Disk
{
private:    
    void createFirstDiskData();
    void updateBitmapFirstTime();
    void updateBitmap();
    void updateInodeTable(string song_name, int inumber);
    void updateSuperBlock(int bloques_usados);
    void readTAG(Inode* inodo, string song_name);
    void readInode(int inumber, string nombre);
    void actualizarApuntadoresInodo(Inode* inodo, Lista<int>* pos_bloques);
    void loadBuffer(Lista<int>* posiciones, string nombre);
    int getInodeTableLength();
    int getInodoDisponible(string song_name);
    int guardarEnInodo(string song_name, Lista<int>* pos_bloques);
    Lista<int>* leerApuntadoresInodo(int cant_bloques, Inode* inodo);
    Lista<int>* getBloqueDisponible(int size_song);

public:
	string name;
	string full_name;
	int cantidad_bloque;
	int size_bloque;
    int size_bitmap;
	char particion;
    char* bitmap;
    Superblock super;
    InodeOnTable* inodeOnTable;
    fstream file;

    Disk();
    void createDisk(string name, int cant_bloques, int size_bloque, char particion);
    void readSuperBlock();
    void readBitMap();
    void buscarCancion(string nombre);
    string loadSong(string song);
    Lista<string>* openDisk(string name);
    Lista<string>* showTAGS(int inumber);

	~Disk();
};

