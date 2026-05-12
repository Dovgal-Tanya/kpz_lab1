#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

typedef struct Node {
    int key;
    int count;
    struct Node* next;
} Node;

typedef struct {
    int size;
    Node** buckets;
} HashTable;

// ВИПРАВЛЕННЯ Issue 3 (DRY): Функція для перевірки виділення пам'яті
void check_allocation(void* ptr, const char* message) {
    if (!ptr) {
        printf("Помилка: %s\n", message);
        exit(1);
    }
}

void fill_array(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % size;
    }
}

int hash_func(int key, int size) {
    return key % size;
}

HashTable* create_table(int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    check_allocation(table, "створення хеш-таблиці");

    table->size = size;
    table->buckets = (Node**)calloc(size, sizeof(Node*));
    check_allocation(table->buckets, "виділення пам’яті для бакетів");

    return table;
}

void insert(HashTable* table, int key) {
    int idx = hash_func(key, table->size);
    Node* current = table->buckets[idx];

    while (current != NULL) {
        if (current->key == key) {
            current->count++;
            return;
        }
        current = current->next;
    }

    Node* new_node = (Node*)malloc(sizeof(Node));
    check_allocation(new_node, "виділення пам’яті для вузла");

    new_node->key = key;
    new_node->count = 1;
    new_node->next = table->buckets[idx];
    table->buckets[idx] = new_node;
}

void free_table(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        Node* current = table->buckets[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// ВИПРАВЛЕННЯ Issue 2 (Long Method): винесено логіку заповнення
void rebuild_array(HashTable* table, int* keys, int count, int* arr) {
    int index = 0;
    for (int i = 0; i < count; i++) {
        int key = keys[i];
        Node* node = table->buckets[hash_func(key, table->size)];
        while (node != NULL) {
            if (node->key == key) {
                for (int j = 0; j < node->count; j++) {
                    arr[index++] = key;
                }
                break;
            }
            node = node->next;
        }
    }
}

void count_sort(int* arr, int size) {
    HashTable* table = create_table(size);
    for (int i = 0; i < size; i++) {
        insert(table, arr[i]);
    }

    int* keys = (int*)malloc(size * sizeof(int));
    check_allocation(keys, "пам'ять для ключів");

    int count = 0;
    for (int i = 0; i < table->size; i++) {
        Node* node = table->buckets[i];
        while (node != NULL) {
            keys[count++] = node->key;
            node = node->next;
        }
    }

    qsort(keys, count, sizeof(int), compare);
    rebuild_array(table, keys, count, arr);

    free(keys);
    free_table(table);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(NULL));

    int sizes[] = { 100, 1000, 10000 };
    for (int i = 0; i < 3; i++) {
        int size = sizes[i];
        int* arr = (int*)malloc(size * sizeof(int));
        check_allocation(arr, "масив");

        fill_array(arr, size);
        clock_t start = clock();
        count_sort(arr, size);
        clock_t end = clock();

        printf("Сортування (%d елементів): %.3lf сек\n", size, (double)(end - start) / CLOCKS_PER_SEC);
        free(arr);
    }
    return 0;
}
