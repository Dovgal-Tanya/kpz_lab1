#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

typedef struct Node {
    char* key;
    int value;
    struct Node* next;
} Node;

typedef struct {
    int size;
    Node** buckets;
} HashTable;

int hash(const char* key, int size) {
    int hash = 0;
    while (*key) {
        hash = (hash + *key) % size;
        key++;
    }
    return hash;
}

char* my_strdup(const char* s) {
    size_t len = strlen(s) + 1;
    char* new_str = (char*)malloc(len);
    if (new_str) {
        memcpy(new_str, s, len);
    }
    return new_str;
}

// Створення вузла
Node* new_node(const char* key, int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;

    node->key = my_strdup(key);
    if (!node->key) {
        free(node);
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    return node;
}

// Створення хеш-таблиці
HashTable* create_table(int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->buckets = (Node**)calloc(size, sizeof(Node*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    table->size = size;
    return table;
}

// Вставка або оновлення
void insert(HashTable* table, const char* key, int value) {
    int index = hash(key, table->size);
    Node* current = table->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    Node* node = new_node(key, value);
    if (!node) return;

    node->next = table->buckets[index];
    table->buckets[index] = node;
}

// Пошук
int search(HashTable* table, const char* key, int* value_out) {
    int index = hash(key, table->size);
    Node* current = table->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            *value_out = current->value;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// Видалення
int remove_key(HashTable* table, const char* key) {
    int index = hash(key, table->size);
    Node* current = table->buckets[index];
    Node* prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev)
                prev->next = current->next;
            else
                table->buckets[index] = current->next;

            free(current->key);
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

// Виведення таблиці
void print_table(HashTable* table) {
    for (int i = 0; i < table->size; ++i) {
        printf("Bucket %d: ", i);
        Node* current = table->buckets[i];
        while (current) {
            printf("(%s, %d) -> ", current->key, current->value);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Звільнення пам'яті
void free_table(HashTable* table) {
    for (int i = 0; i < table->size; ++i) {
        Node* current = table->buckets[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

// Тестування
void test(HashTable* table) {
    int val;

    insert(table, "apple", 10);
    insert(table, "banana", 20);
    insert(table, "orange", 30);
    insert(table, "lemon", 40);

    printf("\n-- Таблиця --\n");
    print_table(table);

    printf("\nПошук 'banana':\n");
    if (search(table, "banana", &val)) {
        printf("Знайдено: %d\n", val);
    }
    else {
        printf("Не знайдено\n");
    }

    printf("\nВидалення 'banana'\n");
    remove_key(table, "banana");

    printf("\nПовторний пошук 'banana':\n");
    if (search(table, "banana", &val)) {
        printf("Знайдено: %d\n", val);
    }
    else {
        printf("Не знайдено\n");
    }

    printf("\n-- Таблиця після видалення --\n");
    print_table(table);
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HashTable* table = create_table(10);
    if (!table) {
        fprintf(stderr, "Не вдалося створити таблицю\n");
        return 1;
    }

    test(table);
    free_table(table);

    return 0;
}