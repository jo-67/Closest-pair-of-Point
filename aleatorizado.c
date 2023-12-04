#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PRIME_NUMBER 100000007

//Funcion que devuelve un punto de la lista.
float seleccionarPunto(float lista[]){
    int position = rand() % (sizeof(lista)+1);
    return lista[position];
}

//funcion que calcula la distancia entre 2 puntos
float calcularDist(float pt1[], float pt2[]){
    float distancia = sqrt(pow(pt2[1] - pt1[1],2) + pow(pt2[2] - pt1[2],2));//usamos la formula de distancia entre 2pts
    return distancia;
}

//funcion que calcula el d para luego crear la grilla
float encontrarD(int lista[],int n){
    float d=1.5; //Debido a los valores que toman los puntos, la maxima distancia posible entre 2 es sqrt(2), asi que puse una cota superior arbitraria que siempre sera sustituida
    //hacemos un for que ejecute n veces el calculo de la distancia de dos puntos aleatorios
    for(int i=0 ; i<n ; i++){
        float pt1=seleccionarPunto(lista);//seleccionamos un punto al azar
        float pt2=seleccionarPunto(lista);//seleccionamos otro punto al azar
        //exite una pequeña posibilidad de que sea el mismo punto, pero me dio paja comprobarlo jeje
        float distancia=calcularDist(&pt1 , &pt2);
        if(distancia<d){ //Si la distancia es menor que el minimo, se cambia. Si no se mantiene.
            d=distancia;
        }
    }
}


//Ahora teniendo el d, podemos proceder a definir lo que necesitaremos para la tabla de hashing.

//Nuestro tabla estara representada por una estructura, dentro de la cual entre otros parametros se encuentra el puntero a una lista que representa cada nodo.
//En cada nodo de nuestra lista ademas, tendremos un puntero a otra lista enlazada que representa los puntos que estan dentro de este nodo, y ademas un puntero al siguiente elemento
//de forma que esta sea la solucion a las colisiones.

// Estructura para un nodo en la lista enlazada de listas. Estos nodos representan los puntos
typedef struct ListNode {
    float data[2]; //Estas son las coordenadas, se representan por una lista de 2 valores
    struct ListNode* next; //Este es el siguiente punto
} ListNode;

// Estructura para un nodo en la tabla de hashing
typedef struct Node {
    int key; //este es el valor del elemento
    ListNode* points;  // Lista enlazada de listas de floats
    struct Node* next; //Puntero al siguiente elemento para solventar colisiones
} Node;

// Estructura para la tabla de hashing
typedef struct HashTable {
    int size; //este es el numero de celdas en la tabla. Se calcula usando el d
    Node** table; //Puntero que apunta a la lista de nodos
    int a; //entero aleatorio para el hashing
    int b; //entero aleatorio para el hashing
} HashTable;


//Ademas necesitamos nuestra funcion de hashing, en este caso hashing universal
// Función de hash universal
unsigned int hashFunction(HashTable* ht, int x) {
    //Para esta funcion usamos la formula (ax+b) mod p mod m, donde P sera el primo definido al comienzo y m el tamaño de la tabla
    return ((((ht->a * x) + ht->b) % PRIME_NUMBER) % ht->size);
}


//Ahora incluimos las funciones para inicializar cada una de las estructuras de mas arriba


// Inicializar la lista de listas. Esta es la lista que contiene los puntos
ListNode* initializeList() {
    return NULL;
}



// Agregar un valor al final de la lista de listas. Nos servira para agregar los puntos despues
ListNode* appendToList(ListNode* head, int data[]) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nuevo nodo de lista\n");
        exit(EXIT_FAILURE);
    }

    newNode->data[0] = data[0]; //coordenada X
    newNode->data[1] = data[1]; //coordenada Y
    newNode->next = NULL; //No apunta a nada


    //En el caso que la lista este vacia, entonces devuelve este nodo 
    if (head == NULL) {
        return newNode;
    }

    //si no esta vacia pasamos para aca
    //partimos apuntando a la cabeza
    ListNode* current = head;
    //y vamos a recorrer la lista enlazada hasta que lleguemos al final, donde el next seria nulo pues no apunta a nada
    while (current->next != NULL) {
        current = current->next;
    }
    //ya habiendo conseguido el ultimo valor, apuntamos su next a nuestro nodo actual
    current->next = newNode;
    //y finalmente devolvemos la cabeza de la lista enlazada
    return head;
}


//Ahora, para inicializar la tabla de hash, necesitamos cuantas celdas va a tener. 
//Ya que sabemos que el ancho de la celda es d, y el rango del plano va de 0 a 1, entonces el numero de celdas sera 1/d redondeado hacia arriba. Y el total seria eso al cuadrado:
int calcularNumeroDeCeldas(float d){
    int celdasEnUnaFila = (int) ceil(1/d);//obtenemos el numero de celdas
    return pow(celdasEnUnaFila, 2); //lo elevamos al cuadrado
}

//ahora, para inicializar tabla de hashing necesitamos solo entregarle el tamaño

// Inicializar la tabla de hashing
HashTable* initializeHashTable(int size) {
    //Inicialmente solo pedimos la memoria para la estructura, y asi inicializamos la hash table. Esto aun no considera la memoria para la grilla en si
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable)); 
    
    //En caso que no funcione tenemos este if
    if (ht == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para la tabla de hashing\n");
        exit(EXIT_FAILURE);
    }

    ht->size = size; // Asignamos los valores de la tabla
    //ya que debemos definir el a y el b, procedemos a hacerlo con el random:

    //________________________________________________________________// Falta eso, no se en que rango deberian estar

    
    ht->table = (Node**)malloc(size * sizeof(Node*));   // Pedimos la memoria para la tabla en si
    //Lo mismo de antes, en caso que falle la asignacion de memoria para la tabla en si, entonces no podemos seguir y lanza el error
    if (ht->table == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para la tabla interna\n");
        exit(EXIT_FAILURE); 
    }

    //Aca inicializamos todas las posiciones como NULL, de forma que sepamos cuales estan vacias.
    for (int i = 0; i < size; i++) {
        ht->table[i] = NULL;
    }

    return ht;
}


//Ahora incluimos la funcion insert que agrega los nodos a la tabla de hashing 
// Insertar un par clave-valor en la tabla de hashing. Key seria la posicion del cuadro de la grilla. El cuadro se agrega vacio, falta gregar todos los puntos
void insert(HashTable* ht, int key) {
    int index = hashFunction(ht, key);

    // Crear un nuevo nodo
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nuevo nodo\n");
        exit(EXIT_FAILURE);
    }

    // Inicializar el nuevo nodo
    newNode->key = key;
    newNode->points = initializeList();
    newNode->next = NULL;


    // Verificar si hay colisión en el índice
    if (ht->table[index] == NULL) {
        ht->table[index] = newNode;  // No hay colisión, simplemente inserta el nuevo nodo
    } else {
        // Colisión, agregamos el nuevo nodo al principio de la lista enlazada
        newNode->next = ht->table[index];
        ht->table[index] = newNode;
    }
}



//Me esta faltando la funcion que agrega a la lista de listas en cada nodo

//para esto necesito una funcion que determine a que cuadro pertenece:
int getKeyFromPoint(float coordenadas[], float d){
    //con el valor de d podemos determinar a que fila y a que columna pertenece.

    //para saber la fila, necesitariamos calcular la parte entera de la division.

    //para saber la columna se hace lo mismo, simplemente la parte entera de la division y/d

    //para obtener el key, debemos recordar que el key que le asignamos a cada cuadro viene dado por la posicion en cuanto a fila y columna. Por ejemplo el de la fila 0:0 es 0, a medida que avanza en la fila aumenta en 1 hasta llegar a 1/d. Si aumentamos la fila, es decir nos movemos en la columna, aumenta en  1/d que es el largo de la fila. Es decir, la formula para calcularlo seria algo como : (parte entera x/d)+ 1/d(parte entera y/d).  
}

//Despues la hare pero seria algo como insertIntoNode
void insertIntoNode(HashTable* ht, float coordenadas[]){
    //primero debo determinar a que coordenada corresponde

    //luego calculo el index con la funcion de hashing

    //luego debo buscar este nodo en la tabla, si no esta en esta posicion entonces paso a la siguiente ya que significa que habia colision, esto lo veo comparando las llaves

    //Finalmente, cuando consigo el nodo verifico si esta vacio. Si esta vacio lo agrego nomas. En caso que no este vacio busco el final de la lista y lo agrego al final. Ver linea 116 del hashing1 para una idea de como hacerlo
}


//ya teniendo la forma de agregar los puntos, tenemos que crear la funcion de como buscarlos:

// Buscar valores en la tabla de hashing dada la key a la que pertenece. Esto nos servira para obtener todos los puntos de un cuadro.
float** search(HashTable* ht, int key) {
    int index = hashFunction(ht, key);

    // busca la lista enlazada correspondiente al nodo
    Node* current = ht->table[index];
    //ahora recorre la lista
    while (current != NULL) {
        //en caso que la llave sea igual entra aca
        if (current->key == key) {
            //obtenemos la lista enlazada de puntos
            ListNode* points = current->points;
            //creamos una lista para guardar los valores 
            float** listaDePuntos;
            int i= 0;
            //mientras queden puntos en la lista enlazada los recorrera
            while (points != NULL) {
                //para cada punto, toma sus coordenadas y las agrega a la lista
                listaDePuntos[i] = points->data;
                //Movemos el contador para la posicion
                i+=1;
                //y pasamos al siguiente punto
                points = points->next;
            }

            return listaDePuntos;
        }
        //si no era la llave, es que hubo una colision y buscara en el siguiente enlazado
        current = current->next;
    }

    printf("Clave %d no encontrada\n", key);
}


//Ya que estamos usando malloc corresponde tener funciones que liberen memoria:
// Liberar la memoria de la lista de listas
void freeList(ListNode* head) {
    while (head != NULL) {
        ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

// Liberar la memoria de la tabla de hashing
void freeHashTable(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        Node* current = ht->table[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            freeList(temp->points);
            free(temp);
        }
    }

    free(ht->table);
    free(ht);
}

//Algunas cosas que podria agregar pero que no se que tan necesarias sean son: una funcion que me diga cuales son las casillas adyacentes.



//Finalmente teniendo todo lo anterior podemos crear la funcion que hara todo lo anterior. Solo debe recibir una lista de puntos y un n que sera para calcular el d inicial.
float hashingUniversal(float lista[], int n){

    //Primero calculamos la distancia d para crear todo lo demas:
    float d = encontrarD(lista,n);

    //Ahora teniendo el d, debemos calcular el tamaño de la tabla:
    int size = calcularNumeroDeCeldas(d);

    //Con el tamaño de la tabla de hashing, finalmente podemos crear la tabla
    HashTable* ht = initializeHashTable(size);

    //Ahora tenemos nuestra tabla, debemos inicializar todos los nodos. Para esto haremos un for que va desde 0 hasta size, y que inicializa con el key siendo el numero.
    for(int i=0; i<size; i++){
        insert(ht,i);
    }
    
    //En este punto tenemos la tabla, con todos sus nodos inicializados. Corresponderia agregar cada elemento a la tabla, por lo que recorremos la lista y vamos a ir agregando cada punto a su nodo correspondiente

    //Ya habiendo hecho esto, tenemos listo todo para poder calcular las distancias de cada punto. Para esto tomamos un punto, definimos su key, y vamos a recorrer todos los puntos de su cuadro y los adyacentes. Con esto obtendremos un valor que sera la distancia minima entre el y los demas de su grupo. 

    //finalmente retornamos ese minimo global que conseguimos



    //ojala funcione esta wea porque me tarde mas que la chucha xd
}




//Cosas extra:
//Una optimizacion podria ser guardar la distancia minima dentro de cada cuadrado. Haciendo esto no tendriamos que calcular las distacias entre un punto y los de su mismo cuadro, ya que su distancia sera mayor o igual y esto no afecta para el global. Esto serviria para d grandes, con muchos puntos dentro de cada bloque. Para un d chico no creo que cambie mucho.