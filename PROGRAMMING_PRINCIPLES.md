# Принципи програмування у проекті сортування хеш-таблицею

У цьому проекті реалізовано алгоритм сортування з використанням динамічної хеш-таблиці. Нижче наведено принципи, яких я дотримуюся.

### 1. DRY (Don't Repeat Yourself — не повторюйся)
Я використовую окремі функції для повторюваних операцій. Наприклад, логіка очищення пам'яті винесена в окрему функцію, щоб не дублювати цикли `while` кожного разу, коли потрібно звільнити таблицю.
[void free_table(HashTable* table) {](https://github.com/Dovgal-Tanya/kpz_lab1/blob/c4c19b0bca23900890a294f8db9b08da16fc841b/task2/task2.cpp#L69)

### 2. KISS (Keep It Simple, Stupid — роби це простіше)
Структура хеш-таблиці та метод вирішення колізій (ланцюжки) обрані як найбільш зрозумілий та ефективний спосіб для цієї задачі без зайвого ускладнення архітектури.
https://github.com/Dovgal-Tanya/kpz_lab1/blob/c4c19b0bca23900890a294f8db9b08da16fc841b/task2/task2.cpp#L6

### 3. Single Responsibility Principle (Принцип єдиної відповідальності)
Кожна функція виконує рівно одну задачу. Функція `hash` тільки обчислює індекс, функція `insert` тільки додає елемент, а `fill_array` тільки заповнює масив даними.
https://github.com/Dovgal-Tanya/kpz_lab1/blob/c4c19b0bca23900890a294f8db9b08da16fc841b/task2/task2.cpp#L23

### 4. Належна обробка помилок (Error Handling)
Перед використанням пам'яті, виділеної через `malloc` або `calloc`, програма завжди перевіряє, чи не повернув системний виклик `NULL`. Це запобігає аварійному завершенню (Segmentation fault).
(https://github.com/Dovgal-Tanya/kpz_lab1/blob/c4c19b0bca23900890a294f8db9b08da16fc841b/task2/task2.cpp#L28)

### 5. Очищення ресурсів (Resource Management)
Кожен виділений блок пам'яті (`malloc`, `calloc`) має відповідний виклик `free`. Це демонструє принцип відповідального керування ресурсами.
https://github.com/Dovgal-Tanya/kpz_lab1/blob/c4c19b0bca23900890a294f8db9b08da16fc841b/task2/task2.cpp#L135
