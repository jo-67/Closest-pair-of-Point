#include <stdio.h>
#include <stdlib.h>

#define PRIME_NUMBER 100000007



// Estructura para un nodo en la lista enlazada de listas. Estos nodos representan los puntos
typedef struct ListNode {
    float data; //Estas son las coordenadas 
    struct ListNode* next; //Este es el siguiente punto
} ListNode;

// Estructura para un nodo en la tabla de hashing
typedef struct Node {
    int key; //este es el valor del elemento
    ListNode* values;  // Lista enlazada de listas de floats
    struct Node* next; //Puntero al siguiente elemento para solventar colisiones
} Node;

// Estructura para la tabla de hashing
typedef struct HashTable {
    int size; //este es el numero de celdas en la tabla. Se calcula usando el d
    Node** table; //Puntero que apunta a la lista de nodos
    int a; //entero aleatorio para el hashing
    int b; //entero aleatorio para el hashing
} HashTable;

// Función de hash universal
unsigned int hashFunction(HashTable* ht, int x) {
    //Para esta funcion usamos la formula (ax+b) mod p mod m, donde P sera el primo definido al comienzo y m el tamaño de la tabla
    return ((((ht->a * x) + ht->b) % PRIME_NUMBER) % ht->size);
}

// Inicializar la lista de listas. Nos servira para inicializar cada nodo al crear la tabla
ListNode* initializeList() {
    return NULL;
}

// Agregar un valor al final de la lista de listas. Nos servira para agregar los puntos despues
ListNode* appendToList(ListNode* head, int data) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nuevo nodo de lista\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = NULL;

    if (head == NULL) {
        return newNode;
    }

    ListNode* current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;
    return head;
}




int calcularNumeroDeCeldas(int d){
    //Ya que las celdas son de dxd, significa que el numero total de celdas seria 1/d^2 redondeando hacia arriba.
    int celdasEnUnaFila = (int) ceil(1/d); //Obtenemos las de una fila y redondeamos hacia arriba para cubrir todo el rango
    return pow(celdasEnUnaFila, 2); //lo elevamos al cuadrado
}

// Inicializar la tabla de hashing
HashTable* initializeHashTable(int size) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable)); //Pedimos la memoria para la estructura, esto no incluye la memoria para los nodos, solo la informacion de los parametros
                                                           // de la tabla y el puntero a los datos en si
    if (ht == NULL) { //en caso que no funcione
        fprintf(stderr, "Error: No se pudo asignar memoria para la tabla de hashing\n");
        exit(EXIT_FAILURE);
    }

    ht->size = size;                                    // Asignamos los valores de la tabla
    ht->table = (Node**)malloc(size * sizeof(Node*));   // Pedimos la memoria para la tabla en si
    if (ht->table == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para la tabla interna\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        ht->table[i] = NULL;
    }

    return ht;
}



// Insertar un par clave-valor en la tabla de hashing. Key seria la posicion del cuadro de la grilla. 
void insert(HashTable* ht, int key, int* values, int numValues) {
    int index = hashFunction(ht, key);

    // Crear un nuevo nodo
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nuevo nodo\n");
        exit(EXIT_FAILURE);
    }

    // Inicializar el nuevo nodo
    newNode->key = key;
    newNode->values = initializeList();
    newNode->next = NULL;

    // Agregar los valores a la lista de listas
    for (int i = 0; i < numValues; i++) {
        newNode->values = appendToList(newNode->values, values[i]);
    }

    // Verificar si hay colisión en el índice
    if (ht->table[index] == NULL) {
        ht->table[index] = newNode;  // No hay colisión, simplemente inserta el nuevo nodo
    } else {
        // Colisión, agregamos el nuevo nodo al principio de la lista enlazada
        newNode->next = ht->table[index];
        ht->table[index] = newNode;
    }
}

// Buscar valores en la tabla de hashing dado una clave
void search(HashTable* ht, int key) {
    int index = hashFunction(ht, key);

    // Recorrer la lista enlazada en el índice correspondiente
    Node* current = ht->table[index];
    while (current != NULL) {
        if (current->key == key) {
            printf("Valores de clave %d: ", key);

            // Imprimir los valores en la lista de listas
            ListNode* values = current->values;
            while (values != NULL) {
                printf("%d ", values->data);
                values = values->next;
            }

            printf("\n");
            return;
        }
        current = current->next;
    }

    printf("Clave %d no encontrada\n", key);
}

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
            freeList(temp->values);
            free(temp);
        }
    }

    free(ht->table);
    free(ht);
}

// Función principal
int main() {
    int tableSize = 100;
    HashTable* ht = initializeHashTable(tableSize);

    // Insertar algunos datos de ejemplo
    int values1[] = {10, 20, 30};
    int values2[] = {40, 50};
    int values3[] = {60, 70, 80};
    insert(ht, 1, values1, sizeof(values1) / sizeof(values1[0]));
    insert(ht, 2, values2, sizeof(values2) / sizeof(values2[0]));
    insert(ht, 3, values3, sizeof(values3) / sizeof(values3[0]));

    // Buscar valores por clave
    search(ht, 1);
    search(ht, 2);
    search(ht, 3);
    search(ht, 4);

    // Liberar la memoria
    freeHashTable(ht);

    return 0;
}
