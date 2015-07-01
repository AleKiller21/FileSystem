#include "Disk.h"
#include <QDebug>

Disk::Disk()
{
    this->bitmap = NULL;
    this->inodeOnTable = NULL;
}

void Disk::createDisk(string name, int cant_bloques, int size_bloque, char particion)
{
    this->name = name;
    this->cantidad_bloque = cant_bloques;
    this->size_bloque = size_bloque;
    this->particion = particion;
    this->full_name = this->name + " " + this->particion + ".disk";
    this->size_bitmap = (cant_bloques/8)+1;
    this->bitmap = new char[this->size_bitmap];

    createFirstDiskData();
}

Lista<string>* Disk::openDisk(string name)
{
    //int number = -1;

    if(file.is_open())// Esto para cerrar el archivo despues de llamar el updateBitmap por primera vez al crear el disco.
        file.close();

    file.open(name.c_str(), ios::binary | ios::in | ios::out);

    if(!file.is_open())
    {
        cout << "No se pudo cargar disco virtual" << endl;
        return NULL;
    }

//    else
//    {
//        cout << "No se pudo cargar disco virtual" << endl;
//        return NULL;
//    }

    file.read((char*)&this->super, sizeof(Superblock));

    this->bitmap = new char[this->super.size_bitmap];
    file.seekg((this->super.block_size-sizeof(Superblock)), ios::cur);
    file.read(this->bitmap, this->super.size_bitmap);

    file.seekg(this->super.inicioInodeTable, ios::beg);
    this->inodeOnTable = new InodeOnTable[this->super.num_inodos];
    file.read((char*)this->inodeOnTable, (this->super.num_inodos * sizeof(InodeOnTable)));

    if(super.canciones_cargadas == 0)
        return NULL;

    Lista<string>* canciones = new Lista<string>();

    for(int i = 0; i < super.canciones_cargadas; i++)
    {
        string nombre = this->inodeOnTable[i].fileName;
        canciones->agregar(nombre);
    }

    return canciones;
}

void Disk::readSuperBlock()
{
    Lista<string>* disk_metadata = new Lista<string>();

    cout << "Nombre: " << this->super.nombre << endl;
    cout << "Peso de bloque: " << this->super.block_size <<  " bytes" << endl;
    cout << "Cantidad de bloques: " << this->super.cant_block << endl;
    cout << "Particion: " << this->super.particion << endl;
    cout << "Peso del disco: " << this->super.size << " bytes" << endl;
    cout << "Numero de bytes en bitmap: " << this->super.size_bitmap << endl;
    cout << "Inicio del InodeTable: " << this->super.inicioInodeTable << endl;
    cout << "Inicio de Inodes: " << this->super.inicioInode << endl;
    cout << "Fin del InodeTable: " << this->super.finInodeTable << endl;
    cout << "Fin de los Inodes: " << this->super.finInode << endl;
    cout << "Inicio data blocks: " << this->super.inicioDataBlocks << endl;
    cout << "Bloques ocupados: " << this->super.used_blocks << endl;
    cout << "Bloques libres: " << this->super.free_blocks << endl;
    cout << "Cantidad de inodos: " << this->super.num_inodos << endl;
    cout << "Inodos libres: " << this->super.inodos_libres << endl;
    cout << "Archivos caragdos: " << this->super.canciones_cargadas << endl;

//    disk_metadata->agregar(super.nombre);
//    disk_metadata->agregar(super.block_size + "");
//    disk_metadata->agregar(super.canciones_cargadas + "");
//    disk_metadata->agregar(super.cant_block + "");
//    disk_metadata->agregar(super.finInode + "");
//    disk_metadata->agregar(super.finInodeTable + "");
//    disk_metadata->agregar(super.free_blocks + "");
//    disk_metadata->agregar(super.inicioDataBlocks + "");
//    disk_metadata->agregar(super.inicioInode + "");
//    disk_metadata->agregar(super.inicioInodeTable + "");
//    disk_metadata->agregar(super.inodos_libres + "");
//    disk_metadata->agregar(super.nombre);
//    disk_metadata->agregar(super.num_inodos + "");
//    disk_metadata->agregar(super.particion + "");
//    disk_metadata->agregar(super.size + "");
//    disk_metadata->agregar(super.size_bitmap + "");
//    disk_metadata->agregar(super.used_blocks + "");

//    return disk_metadata;
}

void Disk::createFirstDiskData()
{
    //SUPER BLOCK
    this->super.block_size = this->size_bloque;
    this->super.cant_block = this->cantidad_bloque;
    this->super.particion = this->particion;
    this->super.size = this->cantidad_bloque * this->size_bloque;
    this->super.size_bitmap = this->size_bitmap;
    this->super.used_blocks = 0;
    this->super.free_blocks = 0;
    this->super.inicioInodeTable = 0;
    this->super.inicioInode = 0;
    this->super.finInodeTable = 0;
    this->super.finInode = 0;
    this->super.inicioDataBlocks = 0;
    this->super.used_blocks = 0;
    this->super.free_blocks = 0;
    this->super.inodos_libres = 0;
    this->super.num_inodos = 0;
    strcpy_s(this->super.nombre, this->full_name.c_str());

    cout << sizeof(this->super) << endl;

    ofstream out(this->full_name.c_str(), ios::out);
    out.close();
    out.open(this->full_name.c_str(), ios::binary | ios::out | ios::in);

    //BASURA
    int length = this->super.size;
    char* basura = new char[length];

    for(int i = 0; i < length; i++)
    {
        basura[i] = 'x';
    }

    out.write(basura, length);
    out.seekp(0);
    out.write((char*)&super, sizeof(super));
    delete basura;

    //BITMAP
    for(int i = 0; i < this->super.size_bitmap; i++)
    {
        this->bitmap[i] = 0;
    }

    char bits[this->super.size_bitmap];

    for(int i = 0; i < this->super.size_bitmap; i++)
    {
        bits[i] = 0;
    }

    out.seekp((this->size_bloque-sizeof(Superblock)), ios::cur);
    out.write(bits, sizeof(bits));

    //InodeTable
    int posicionInodeTable = ((((this->super.size_bitmap + this->super.block_size) / this->super.block_size) + 1) * this->super.block_size);
    this->super.inicioInodeTable = posicionInodeTable;
    out.seekp(posicionInodeTable, ios::beg);

    int inodeTableLength = getInodeTableLength();
    this->super.finInodeTable = posicionInodeTable + inodeTableLength;
    InodeOnTable* table = new InodeOnTable[(inodeTableLength / sizeof(InodeOnTable))];

    for(int i = 0; i < (inodeTableLength / sizeof(InodeOnTable)); i++)
    {
        InodeOnTable inode;
        inode.inumber = i;
        table[i] = inode;
    }

    out.write((char*)table, inodeTableLength);
    delete[] table;

    //Inodes
    int posicionInodes = ((((posicionInodeTable + inodeTableLength) / this->super.block_size) + 1) * this->super.block_size);
    this->super.inicioInode = posicionInodes;
    out.seekp(posicionInodes);

    int inodeLength = (getInodeTableLength() / sizeof(InodeOnTable)) * sizeof(Inode);
    this->super.finInode = posicionInodes + inodeLength;
    Inode* inodes = new Inode[inodeLength / sizeof(Inode)];
    cout << inodes[0].size << endl;

    out.write((char*)inodes, inodeLength);
    delete[] inodes;

    int inicioDataBlocks = ((this->super.finInode / this->super.block_size) + 1) * this->super.block_size;
    this->super.inicioDataBlocks = inicioDataBlocks;

    this->super.used_blocks = this->super.inicioDataBlocks / this->super.block_size;
    this->super.free_blocks = this->super.cant_block - this->super.used_blocks;
    this->super.num_inodos = getInodeTableLength() / sizeof(InodeOnTable);
    this->super.inodos_libres = getInodeTableLength() / sizeof(InodeOnTable);
    this->super.canciones_cargadas = 0;

    out.seekp(0);
    out.write((char*)&super, sizeof(super));
    out.close();

    updateBitmapFirstTime();
    delete[] this->bitmap;
    this->file.close();
}

void Disk::updateBitmapFirstTime()
{
    //ofstream file(this->super.nombre, ios::binary | ios::out | ios::in);
    if(!file.is_open())
    {
        file.open(this->super.nombre, ios::binary | ios::out | ios::in);
    }
    file.seekp(this->super.block_size);
    int bits_apagados = 0;

    for(int i = 0; i < this->super.size_bitmap; i++)
    {
        for(int x = 0; x < 8; x++)
        {
            if(bits_apagados == this->super.used_blocks)
            {
                break;
            }

            else
            {
                this->bitmap[i]^= (1 << x);
                bits_apagados++;
            }
        }

        if(bits_apagados == this->super.used_blocks)
            break;
    }

    char bits[this->super.size_bitmap];

    for(int i = 0; i < this->super.size_bitmap; i++)
    {
        bits[i] = this->bitmap[i];
    }

    file.write(bits, sizeof(bits));
    file << flush;
    //file.close();
}

int Disk::getInodeTableLength()
{
    int espacioSuperBitmap = (((this->super.size_bitmap + this->super.block_size) / this->super.block_size) + 1) * this->super.block_size;
    int espacioDisponible = this->super.size - espacioSuperBitmap;
    int x1 = espacioDisponible * 0.10;
    int x2 = x1 + 1;
    int x3 = x2 / sizeof(Inode);//176 es el peso del inodo
    int x4 = x3 + 1;
    int x5 = x4 * sizeof(InodeOnTable);//44 es el peso de cada inodo en la tabla de inodos, InodeOnTable
    return x5;
}

Lista<int>* Disk::getBloqueDisponible(int size_song)
{
    int block_count = -1;
    int bloques_seleccionados = 0;
    Lista<int>* posicion_bloques = new Lista<int>();
    int bloques_necesarios = size_song / this->super.block_size;

    for(int i = 0; i < this->super.size_bitmap; i++)
    {
        for(int x = 0; x < 8; x++)
        {
            if(bloques_seleccionados == bloques_necesarios)
                break;

            block_count++;
            bool t = this->bitmap[i] & (1 << x);

            if(t != true)
            {
                int pos = block_count * this->super.block_size;
                posicion_bloques->agregar(pos);
                bloques_seleccionados++;
                this->bitmap[i]^= (1 << x);
            }
        }

        if(bloques_seleccionados == bloques_necesarios)
            break;
    }

    return posicion_bloques;
}

Lista<string>*Disk::showTAGS(int inumber)
{
    Lista<string>* tags = new Lista<string>();
    for(int i = 0; i < super.num_inodos; i++)
    {
        if(this->inodeOnTable[i].inumber == inumber)
        {
            string title = this->inodeOnTable[i].fileName;
            tags->agregar(title);
        }
    }

    Inode inode;
    int pos_inodo = super.inicioInode + (inumber * sizeof(Inode));
    file.seekg(pos_inodo, ios::beg);
    file.read((char*)&inode, sizeof(Inode));

    string artist = inode.artist;
    string album = inode.album;
    string year = inode.year;

    tags->agregar(artist);
    tags->agregar(album);
    tags->agregar(year);

    return tags;
}

int Disk::getInodoDisponible(string song_name)
{
    for(int i = 0; i < this->super.num_inodos; i++)
    {
        if(this->inodeOnTable[i].available != true)
        {
            this->inodeOnTable[i].available = 1;
            strcpy(this->inodeOnTable[i].fileName, song_name.c_str());
            return this->inodeOnTable[i].inumber;
        }
    }
}

//PROBAR LA FUNCION DE CARGAR CANCION Y VERIFICAR RESULTADOS CON LOS DATOS DEL SUPERBLOCK
string Disk::loadSong(string song)
{
    ifstream song_file(song.c_str(), ios::ate | ios::binary);
    cout << song.c_str() << endl;
    int size = song_file.tellg();
    if(size > super.size)
        return "NO";
    cout << size << endl;
    Lista<int>* pos_dataBlocks = getBloqueDisponible(size);
    char buffer[super.block_size];
    song_file.seekg(0);

    for(int i = 0; i < pos_dataBlocks->getCantidad(); i++)
    {
        int pos_block = pos_dataBlocks->obtenerValor(i);
        file.seekp(pos_block, ios::beg);
        song_file.read(buffer, super.block_size);
        file.write(buffer, super.block_size);
    }

    song_file.close();

    QFileInfo file(song.c_str());
    string name = file.baseName().toStdString();
    name.append(".mp3");
    cout << name.c_str() << endl;
    //guardarEnInodo(song.c_str(), pos_dataBlocks);
    guardarEnInodo(name, pos_dataBlocks);

    updateBitmap();
    updateSuperBlock(pos_dataBlocks->getCantidad());

    delete pos_dataBlocks;

    return name;
}

int Disk::guardarEnInodo(string song_name, Lista<int>* pos_bloques)
{
    int inumber = getInodoDisponible(song_name);
    int pos_disco = this->super.inicioInode + (sizeof(Inode) * inumber);
    Inode inodo;
    file.seekg(pos_disco, ios::beg);
    file.read((char*)&inodo, sizeof(Inode));
    readTAG(&inodo, song_name.c_str());

    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);
    strftime(inodo.fechaC, sizeof(inodo.fechaC), "%m/%d/%y", &tstruct);
    strftime(inodo.fechaM, sizeof(inodo.fechaC), "%m/%d/%y", &tstruct);

    actualizarApuntadoresInodo(&inodo, pos_bloques);

    file.seekp(pos_disco, ios::beg);
    file.write((char*)&inodo, sizeof(Inode));
    file << flush;

    updateInodeTable(song_name, inumber);
}

void Disk::updateInodeTable(string song_name, int inumber)
{
//    for(int i = 0; i < this->super.num_inodos; i++)
//    {
//        if(this->inodeOnTable[i].inumber == inumber)
//        {
//            strcpy(this->inodeOnTable[i].fileName, song_name.c_str());
//            break;
//        }
//    }

    file.seekp(super.inicioInodeTable, ios::beg);
    file.write((char*)this->inodeOnTable, (super.num_inodos * sizeof(InodeOnTable)));
    file << flush;
}

void Disk::updateSuperBlock(int bloques_usados)
{
    super.inodos_libres--;
    super.canciones_cargadas++;
    super.used_blocks+= bloques_usados;
    super.free_blocks-= bloques_usados;

    file.seekp(0);
    file.write((char*)&super, sizeof(Superblock));
    file << flush;
}

void Disk::updateBitmap()
{
    file.seekp(super.block_size, ios::beg);

    char bits[this->super.size_bitmap];

    for(int i = 0; i < this->super.size_bitmap; i++)
    {
        bits[i] = this->bitmap[i];
    }

    file.write(bits, sizeof(bits));
    file << flush;
}

void Disk::readTAG(Inode* inodo, string song_name)
{
    ifstream in(song_name.c_str(), ios::binary | ios::ate);
    int size = in.tellg();
    in.seekg(size-125);

    in.read(inodo->title, 30);
    in.read(inodo->artist, 30);
    in.read(inodo->album, 30);
    in.read(inodo->year, 4);

    inodo->title[29] = '\0';
    inodo->artist[29] = '\0';
    inodo->album[29] = '\0';
    inodo->year[4] = '\0';
    inodo->size = size;

    in.close();
}

void Disk::buscarCancion(string nombre)
{
    if(this->inodeOnTable == NULL)
        this->inodeOnTable = new InodeOnTable[super.num_inodos];

    file.seekg(super.inicioInodeTable, ios::beg);
    file.read((char*)this->inodeOnTable, (super.num_inodos * sizeof(InodeOnTable)));
    int inumber = -1;

    for(int i = 0; i < super.num_inodos; i++)
    {
        if(nombre.compare(this->inodeOnTable[i].fileName) == 0)
        {
            inumber = this->inodeOnTable[i].inumber;
            break;
        }
    }

    if(inumber == -1)
    {
        cout << "No se encontro el inodo de la cancion" << endl;
        return;
    }

    readInode(inumber,nombre);
}

void Disk::readInode(int inumber, string nombre)
{
    Inode inode;
    int pos_disco = this->super.inicioInode + (sizeof(Inode) * inumber);
    file.seekg(pos_disco, ios::beg);
    file.read((char*)&inode, sizeof(Inode));
    Lista<int>* posiciones = leerApuntadoresInodo((inode.size / super.block_size), &inode);
    loadBuffer(posiciones, nombre);
}


void Disk::loadBuffer(Lista<int>* posiciones, string nombre)
{
//    char temp[super.block_size];
//    ofstream out("Master.mp3", ios::binary);

//    for(int i = 0; i < posiciones->getCantidad(); i++)
//    {
//        //char temp[super.block_size];
//        int bloque = posiciones->obtenerValor(i);
//        file.seekg(bloque, ios::beg);
//        file.read(temp, super.block_size);
//        out.write(temp, super.block_size);
//    }
//    out.close();

    //QByteArray* buffer = new QByteArray();
    QByteArray buffer;

    for(int i = 0; i < posiciones->getCantidad(); i++)
    {
        char temp[super.block_size];
        int bloque = posiciones->obtenerValor(i);
        file.seekg(bloque, ios::beg);
        file.read(temp, super.block_size);
        buffer.append(temp, super.block_size);
    }

    cout << buffer.size() << endl;
    ofstream out(nombre.c_str(), ios::binary);
    out.write(buffer, buffer.size());
    out.close();
}

Lista<int>* Disk::leerApuntadoresInodo(int cant_bloques, Inode* inodo)
{
    int blockIndex = 0;
    Lista<int>* pos_bloques = new Lista<int>();

    for(; blockIndex < cant_bloques; blockIndex++)
    {
        if(blockIndex == 10)
            break;

        int pos = inodo->bloques_directos[blockIndex];
        pos_bloques->agregar(pos);
    }

    if(blockIndex == cant_bloques)
        return pos_bloques;

    //Apuntador simple
    int bloqueSimplePos = inodo->indirectos_simples;
    int apuntadores_directos[super.block_size / 4];
    file.seekg(bloqueSimplePos, ios::beg);
    file.read((char*)&apuntadores_directos, sizeof(apuntadores_directos));

    for(int i = 0; i < (super.block_size / 4); i++)
    {
        int pos = apuntadores_directos[i];
        pos_bloques->agregar(pos);
        blockIndex++;

        if(blockIndex == cant_bloques)
            break;
    }

    if(blockIndex == cant_bloques)
        return pos_bloques;

    //Apuntadores dobles
    int bloqueDoblePos = inodo->indirectos_dobles;
    int apuntadoresDobles[super.block_size / 4];
    file.seekg(bloqueDoblePos, ios::beg);
    file.read((char*)&apuntadoresDobles, sizeof(apuntadoresDobles));

    for(int i = 0; i < (super.block_size / 4); i++)
    {
        int bloqueSimple = apuntadoresDobles[i];
        int apuntadores[super.block_size / 4];
        file.seekg(bloqueSimple, ios::beg);
        file.read((char*)&apuntadores, sizeof(apuntadores));

        for(int x = 0; x < (super.block_size / 4); x++)
        {
            int pos = apuntadores[x];
            pos_bloques->agregar(pos);
            blockIndex++;

            if(blockIndex == cant_bloques)
                break;
        }

        if(blockIndex == cant_bloques)
            break;
    }

    return pos_bloques;
}

void Disk::actualizarApuntadoresInodo(Inode* inodo, Lista<int>* pos_bloques)
{
    int contador_bloques = 0;

    //Primeros 10 apuntadores directos
    for(; contador_bloques < pos_bloques->getCantidad(); contador_bloques++)
    {
        if(contador_bloques == 10)
            break;

        inodo->bloques_directos[contador_bloques] = pos_bloques->obtenerValor(contador_bloques);
    }

    if(contador_bloques == pos_bloques->getCantidad())
        return;

    //Apuntador simple indirecto
    Lista<int>* bloque_simple = getBloqueDisponible(super.block_size);
    int pos_bloque_simple = bloque_simple->obtenerValor(0);
    inodo->indirectos_simples = pos_bloque_simple;
    file.seekp(pos_bloque_simple, ios::beg);

    int cantidad_apuntadores = this->super.block_size / 4;
    int apuntadores_directos[cantidad_apuntadores];

    for(int i = 0; i < cantidad_apuntadores; i++)
    {
        apuntadores_directos[i] = pos_bloques->obtenerValor(contador_bloques);
        contador_bloques++;

        if(contador_bloques == pos_bloques->getCantidad())
            break;
    }

    file.write((char*)&apuntadores_directos, sizeof(apuntadores_directos));
    //file << flush;
    delete bloque_simple;

    if(contador_bloques == pos_bloques->getCantidad())
        return;

    //Apuntador doble indirecto
    Lista<int>* bloque_doble = getBloqueDisponible(super.block_size);
    int pos_bloque_doble = bloque_doble->obtenerValor(0);
    inodo->indirectos_dobles = pos_bloque_doble;

    delete bloque_doble;
    int apuntadores_dobles[cantidad_apuntadores];

    for(int i = 0; i < cantidad_apuntadores; i++)
    {
        Lista<int>* bloque_simple = getBloqueDisponible(super.block_size);
        int pos_simple = bloque_simple->obtenerValor(0);

        delete bloque_simple;

        apuntadores_dobles[i] = pos_simple;
        file.seekp(pos_simple, ios::beg);
        int apuntadores_simples[cantidad_apuntadores];

        for(int x = 0; x < cantidad_apuntadores; x++)
        {
            apuntadores_simples[x] = pos_bloques->obtenerValor(contador_bloques);
            contador_bloques++;

            if(contador_bloques == pos_bloques->getCantidad())
                break;
        }

        file.write((char*)&apuntadores_simples, sizeof(apuntadores_simples));

        if(contador_bloques == pos_bloques->getCantidad())
            break;
    }

    file.seekp(pos_bloque_doble, ios::beg);
    file.write((char*)&apuntadores_dobles, sizeof(apuntadores_dobles));

    return;
}

void Disk::readBitMap()
{
//    for(int i = 0; i < this->super.size_bitmap; i++)
//    {
//        for(int x = 0; x < 8; x++)
//        {
//            bool t = this->bitmap[i] & (1 << x);
//            cout << t << " ";
//        }

//        cout << "\n";
//    }

    //Haciendo pruebas
    InodeOnTable inode;
    file.seekg(super.inicioInodeTable);
    file.read((char*)&inode, sizeof(InodeOnTable));
    cout << inode.fileName << endl;
}


Disk::~Disk()
{
}
