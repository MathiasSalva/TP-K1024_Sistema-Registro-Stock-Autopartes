//
// Created by mathias.salva on 4/12/2021.
//
#include <iostream>
#include <cstdio>
#include <cstring>
#define PATH_MODELOS "archivo_modelos.data"
#define PATH_PARTES "archivo_partes.data"

using namespace std;

////////////////////
//// Registros ////
//////////////////
/* Registros de Datos:
 * Modelo: Contiene informacion de los modelos de automoviles.
 * Autoparte: Contiene informacion de las autopartes de cada automovil.
 */
struct Modelo{
    int id_modelo;
    char descripcion[100];
    int anio_fabricacion;
};

struct Autoparte{
    int id_autoparte;
    int nro_modelo_vehiculo;
    char descripcion[100];
    float precio;
    unsigned int stock;
};
/* Registros de Listas/Nodos:
 * NodoListaDEModelo: Registro que estructura los Nodos de la lista doblemente enlazada de tipo Modelo.
 * NodoListaSEAutoparte: Registro que estructura los Nodos de la sublista simplemente enlazada de tipo Autoparte.
 */
struct NodoListaSEAutoparte{
    Autoparte info;
    NodoListaSEAutoparte *sgte;
};
struct NodoListaDEModelo{
    Modelo info;
    NodoListaDEModelo *sgte;
    NodoListaDEModelo *ant;
    NodoListaSEAutoparte *listaAutopartes;
};

///////////////////////
//// Subprogramas ////
/////////////////////

/*
 * Primitivas: subprogramas con funciones basicas para las listas DE Modelos y listas SE Autopartes
 */


//  Primitivas Lista DE Modelos
void listaModeloInsertarSiguiente (NodoListaDEModelo  *&n, Modelo x){
    NodoListaDEModelo *nuevo= new NodoListaDEModelo();
    nuevo -> info = x;
    nuevo -> listaAutopartes = NULL;
    if (n != NULL){
        nuevo -> sgte = n -> sgte;
        n -> sgte = nuevo;
        nuevo->ant=n;
        if (nuevo->sgte){
            nuevo->sgte->ant = nuevo;
        }
    }
    else{
        nuevo->sgte= n;
        n= nuevo;
        nuevo->ant=NULL;
    }
    return;
}
NodoListaDEModelo *listaModeloObtenerSiguiente (NodoListaDEModelo *m){
    if (m)
        return m->sgte;
    else
        return NULL;
}
NodoListaDEModelo *buscarIdModelo(NodoListaDEModelo *inicio, int id_modelo){
    while (inicio && inicio -> info.id_modelo != id_modelo){
        inicio = inicio -> sgte;
    }
    return inicio;
}
NodoListaDEModelo *ObtenerUltimoModelo(NodoListaDEModelo * m){
    if (m){
        while (m->sgte){
            m = m->sgte;
        }
    }
    return m;
}
void listaModeloInsertarFinal (NodoListaDEModelo  *&n, Modelo x)
{
    NodoListaDEModelo *aux;

    if (n){
        aux = ObtenerUltimoModelo(n);
        listaModeloInsertarSiguiente(aux,x);
    }
    else{
        listaModeloInsertarSiguiente(n,x);
    }
    return;
}
void listaAutoparteBorrarLista(NodoListaSEAutoparte *&lista){
    NodoListaSEAutoparte *ant;
    NodoListaSEAutoparte *paux;
    while (lista!=NULL){
        ant=NULL;
        paux= lista;
        while (paux && paux->sgte){
            ant = paux;
            paux= paux->sgte;
        }
        if (paux){
            delete paux;
            if (ant!=NULL)
                ant->sgte=NULL;
            else
                lista =NULL;
        }
    }
    return;
}
void listaModeloBorrarSiguiente(NodoListaDEModelo *n){
    NodoListaDEModelo *n_borrar = n -> sgte;
    NodoListaDEModelo *sgt_n_borrar;
    if (n_borrar)
        sgt_n_borrar = n_borrar -> sgte;
    else
        sgt_n_borrar = NULL;
    n -> sgte = sgt_n_borrar;
    if (sgt_n_borrar)
        sgt_n_borrar -> ant = n;
    listaAutoparteBorrarLista(n_borrar->listaAutopartes);
    delete n_borrar;
    return;
}


// Primitivas Lista Autopartes
void listaAutoparteInsertarSiguiente(NodoListaSEAutoparte *&n, Autoparte a){
    NodoListaSEAutoparte *nueva = new NodoListaSEAutoparte();
    nueva -> info = a;
    if (n){
        nueva -> sgte = n -> sgte;
        n -> sgte = nueva;
    }
    else {
        nueva -> sgte = n;
        n = nueva;
    }

    return;
}
NodoListaSEAutoparte *obtenerUltimaAutoparte(NodoListaSEAutoparte * a){
    if (a)
        while (a -> sgte)
            a = a -> sgte;
    return a;
}
NodoListaSEAutoparte *buscarIdAutoparte(NodoListaSEAutoparte *inicio, int id_autoparte){
    while (inicio && inicio -> info.id_autoparte != id_autoparte){
        inicio = inicio -> sgte;
    }
    return inicio;
}
NodoListaDEModelo *ObtenerModeloConRepuesto(NodoListaDEModelo *lista, int id_autoparte){
    while (lista && buscarIdAutoparte(lista -> listaAutopartes, id_autoparte) == NULL)
        lista = lista->sgte;
    return lista;
}
void listaAutoparteInsertarAlFinal(NodoListaSEAutoparte *&lista, Autoparte a){
    NodoListaSEAutoparte *aux;
    aux = obtenerUltimaAutoparte(lista);
    if (aux)
        listaAutoparteInsertarSiguiente(aux, a);
    else
        listaAutoparteInsertarSiguiente(lista, a);

}
NodoListaSEAutoparte *listaAutoparteObtenerSiguiente(NodoListaSEAutoparte *a){
    if (a)
        return a->sgte;
    else
        return NULL;
}

/*
 * Subprogramas especificos: Aquellos subprogramas utilizados de manera principal
 * para este programa. Constituyen, de manera general, las funciones centrales de este programa.
 */

/* Procedimiento cargarModelo:
 * Precondiciones: Requiere la direccion de un puntero de lista de modelos de automovil.
 * El ID del modelo debe de ser diferente a todos los que existan en dicha lista.
 * Postcondiciones: Agrega al final de la lista el modelo de automovil si no existe.
 * */
void cargarModelo(NodoListaDEModelo *&lModelos){
    Modelo m;
    cout << "Ingrese datos del modelo de automovil: "<< endl;
    cout << "Numero de modelo: ";
    cin >> m.id_modelo;
    if(buscarIdModelo(lModelos, m.id_modelo)){
        cout << "*** YA EXISTE UN AUTOMOVIL CON ESE NUMERO DE MODELO ***" << endl;
    }
    else{
        cout << "Descripcion:" << endl;
        fflush(stdin);
        fgets(m.descripcion, 100, stdin);
        cout << "Anio de fabricacion:" << endl;
        cin >> m.anio_fabricacion;
        listaModeloInsertarFinal(lModelos, m);
        cout << "Modelo #" << m.id_modelo << " cargado con exito!" << endl;
    }
}

/* Procedimiento cargarAutoparte:
 * Precondiciones: Requiere la direccion de un puntero de sublista de autopartes de un automovil
 * Postcondiciones: Agrega una nueva autoparte al final de la sublista de un automovil especificado por el usuario.
 * */
void cargarAutoparte(NodoListaDEModelo *&lPartes){
    NodoListaDEModelo *automovil;
    int modelo_automovil;
    cout << "Ingrese numero de modelo del automovil de la autoparte: " << endl;
    cin >> modelo_automovil;
    automovil = buscarIdModelo(lPartes, modelo_automovil);
    if (automovil){
        Autoparte nueva_autoparte;
        cout << "*** Ingrese los siguientes datos de la autoparte ***" << endl;
        cout << "Numero de identificacion:" << endl;
        cin >> nueva_autoparte.id_autoparte;
        cout << "Descripcion:" << endl;
        fflush(stdin);
        fgets(nueva_autoparte.descripcion, 100, stdin);
        cout << "Precio (solo numeros):" << endl;
        cin >> nueva_autoparte.precio;
        cout << "Stock:" << endl;
        cin >> nueva_autoparte.stock;
        nueva_autoparte.nro_modelo_vehiculo = automovil -> info.id_modelo;
        listaAutoparteInsertarAlFinal(automovil->listaAutopartes, nueva_autoparte);
        cout << "Se ha agregado la autoparte ID #" << nueva_autoparte.id_autoparte << " con exito!" << endl;
    }
}

/* Procedimiento actualizarStock:
 * Precondiciones: Requiere un puntero de lista de modelos de automovil.
 * Postcondiciones: Actualiza el valor "stock" de uno de los nodos de la sublista de autopartes
 * de acuerdo a un ID de autoparte especifico.
 * */
void actualizarStock(NodoListaDEModelo *lista){
    NodoListaDEModelo *modeloConAutoparte, *paux;
    NodoListaSEAutoparte *autoparte_buscada, *paux2;
    int id_autoparte;
    unsigned int stock_actual;
    cout << "Ingrese ID de la autoparte" << endl;
    cin >> id_autoparte;
    modeloConAutoparte = ObtenerModeloConRepuesto(lista,id_autoparte);
    if (modeloConAutoparte){
        autoparte_buscada = buscarIdAutoparte(modeloConAutoparte->listaAutopartes, id_autoparte);
        cout << "Ingrese stock actual" << endl;
        cin >> stock_actual;
        paux2 = modeloConAutoparte -> listaAutopartes;
        modeloConAutoparte -> listaAutopartes = autoparte_buscada;
        modeloConAutoparte -> listaAutopartes -> info.stock = stock_actual;
        modeloConAutoparte -> listaAutopartes = paux2;
    }
    else{
        cout << "No existe autoparte con ese ID" << endl;
    }
    return;

}

/* Procedimiento listaModeloMostrar:
 * Precondiciones: Requiere un puntero de lista de modelos de automovil.
 * Postcondiciones: Muestra todos los modelos de automoviles de la lista.
 * */
void listaModeloMostrar(NodoListaDEModelo *lista)
{
    while (lista){
        cout << "******** DATOS DEL MODELO #" << lista->info.id_modelo << " ******** " << endl;
        cout << "Descripcion: " << lista->info.descripcion << endl;
        cout << "Fecha de fabricacion: " << lista->info.anio_fabricacion << endl;
        lista = lista->sgte;
    }
}


/* Procedimiento listaAutoparteMostrar:
 * Precondiciones: Requiere un puntero de sublista de autopartes
 * Postcondiciones: Muestra al usuario los datos de cada autoparte de la lista
 * */
void listaAutoparteMostrar(NodoListaSEAutoparte *lista){

    while (lista){
        cout << "*******************" << endl;
        cout << "ID Autoparte: " << lista -> info.id_autoparte << endl;
        cout << "Descripcion: " << lista -> info.descripcion << endl;
        cout << "Precio: $" << lista -> info.precio << endl;
        cout << "Stock: " << lista -> info.stock << endl;
        lista = lista->sgte;
    }
    return;
}

/* Procedimiento eliminarModelo:
 * Precondiciones: Requiere una direccion de puntero de inicio de una lista DE. de modelos.
 * Ademas, es requerido una direccion de puntero de un nodo a borrar.
 * Postcondiciones: Se elimina de la lista de modelos el nodo especificado asi como su sublista de autopartes.
 * */
void eliminarModelo(NodoListaDEModelo *&inicio, NodoListaDEModelo *n_borrar){
    NodoListaDEModelo *auxsgte;
    NodoListaDEModelo *auxant;

    if (n_borrar && n_borrar -> ant){
        auxant = n_borrar -> ant;
        listaModeloBorrarSiguiente(auxant);
    }
    else if (n_borrar){
        auxsgte = n_borrar -> sgte;
        listaAutoparteBorrarLista(n_borrar -> listaAutopartes);
        delete n_borrar;
        if (auxsgte)
            auxsgte -> ant = NULL;
        inicio = auxsgte;
    }
    return;

}


/* Procedimiento mainMenu:
 * Precondiciones: Ninguna
 * Postcondiciones: Muestra al usuario opciones del Menu Principal
 * */
void mainMenu(){
    cout << "***********************************************************" << endl;
    cout << "Por favor, seleccione una opcion y presione la tecla ENTER" << endl;
    cout << "1. Cargar Modelo de Vehiculo" << endl;
    cout << "2. Cargar Autoparte" << endl;
    cout << "3. Actualizar Stock" << endl;
    cout << "4. Listar Modelos de Automoviles" << endl;
    cout << "5. Listar Repuestos de Automovil" << endl;
    cout << "6. Eliminar Modelo de Automovil" << endl;
    cout << "7. Salir" << endl;
    cout << "***********************************************************" << endl;

}

/*
 * Subprogramas de manejo de archivos: Se utilizan para administrar los archivos
 * que almacenan la informacion utilizada por este programa, con el objetivo de persistir la misma
 * en el sistema de archivos del computador.
 *
 */

/* Procedimiento guardarEnArchivo:
 * Precondiciones: Requiere un puntero de lista de Modelo
 * Postcondiciones: Persiste la informacion de la lista de Modelo asi como de las sublistas de Autoparte en
 * archivos binarios generados por defecto en la misma ruta de ejecucion del programa compilado.
 * Por defecto, los nombres de los archivos son los siguientes:
 * >Archivo binario de Modelos: archivo_modelos.data
 * >Archivo binario de Autopartes: archivo_partes.data
 * */
    void guardarEnArchivo(NodoListaDEModelo *lModelo){
    FILE *f_modelos;
    FILE *f_parts;
    NodoListaDEModelo *auxMod = NULL;
    NodoListaSEAutoparte *auxPart = NULL;
    if (f_modelos = fopen(PATH_MODELOS, "wb")){
        if (f_parts = fopen(PATH_PARTES, "wb")){
            auxMod = lModelo;
            while (auxMod){
                fwrite(&(auxMod->info), sizeof(Modelo), 1, f_modelos);
                auxPart= auxMod->listaAutopartes;
                while(auxPart){
                    fwrite(&(auxPart->info), sizeof(Autoparte), 1, f_parts);
                    auxPart = listaAutoparteObtenerSiguiente(auxPart);
                }
                auxMod = listaModeloObtenerSiguiente(auxMod);
            }
            fclose(f_parts);
        }
        else
            cout << "ERROR AL INTENTAR GRABAR ARCHIVO DE MATERIAS" << endl;
        fclose(f_modelos);
    }
    else
        cout << "ERROR AL INTENTAR GRABAR ARCHIVO DE ALUMNOS" << endl;
    return;
}

/* Procedimiento cargarArchivo:
 * Precondiciones: Ninguna
 * Postcondiciones: Muestra al usuario opciones del Menu Principal
 * */
void cargarArchivo(NodoListaDEModelo *&lModelo)
{
    FILE *f;
    Modelo mod;
    Autoparte part;
    f=fopen(PATH_MODELOS,"r");
    while (fread(&mod,sizeof(Modelo),1,f))
        listaModeloInsertarSiguiente(lModelo,mod);
    fclose(f);
    f=fopen(PATH_PARTES,"r");
    while (fread(&part,sizeof(Autoparte),1,f))
    {
        NodoListaDEModelo *aux= buscarIdModelo(lModelo, part.nro_modelo_vehiculo);
        if (aux) //Por si no se econtr� el alumno
            listaAutoparteInsertarSiguiente(aux->listaAutopartes,part);
    }
    fclose(f);
    return;
}

// Programa principal
int main(){
    NodoListaDEModelo *listaModelos = NULL;
    cargarArchivo(listaModelos);
    int opcion, modelo_vehiculo, id_autoparte;
    unsigned int stock_actual;
    bool salir = false;
    while(!salir){
        mainMenu();
        cin >> opcion;
        switch (opcion) {
            case 1:
                cargarModelo(listaModelos);
                break;
            case 2:
                cargarAutoparte(listaModelos);
                break;
            case 3:
                actualizarStock(listaModelos);
                break;
            case 4:
                listaModeloMostrar(listaModelos);
                cout << "Presione una tecla cualquiera para continuar..." << endl;
                fflush(stdin);
                getchar();
                break;
            case 5:
                cout << "Ingrese ID de modelo de automovil: " << endl;
                cin >> modelo_vehiculo;
                if (buscarIdModelo(listaModelos, modelo_vehiculo)){
                    cout << "Listado de Autopartes de Modelo #"
                    << buscarIdModelo(listaModelos, modelo_vehiculo) -> info.id_modelo << endl;
                    listaAutoparteMostrar(buscarIdModelo(listaModelos, modelo_vehiculo) -> listaAutopartes);
                }
                else
                    cout << "No se pudo encontrar el automovil" << endl;
                break;
            case 6:
                cout << "Ingrese ID de modelo a borrar: "<< endl;
                cin >> modelo_vehiculo;
                modelo_vehiculo = buscarIdModelo(listaModelos, modelo_vehiculo) -> info.id_modelo;
                cout << "Modelo #" << modelo_vehiculo << " sera borrado" << endl;
                eliminarModelo(listaModelos, buscarIdModelo(listaModelos, modelo_vehiculo));
                cout << "Modelo #" << modelo_vehiculo << " ha sido borrado con exito!" << endl;
                break;
            case 7:
                salir = true;
                guardarEnArchivo(listaModelos);
                cout << "Adios!" << endl;
                break;
        }
    }
    return 0;
}