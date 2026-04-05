#include "vector.h"

// расширение массива
void Vector::resize() {
    unsigned int new_capacity = capacity * 2;
    Fields* new_data = new Fields[new_capacity];
    for (int i = 0; i < size; i++)
        new_data[i] = data[i];
    delete[] data;
    data = new_data;
    capacity = new_capacity;
}

// конструктор
Vector::Vector() : size(0), capacity(5) {
    data = new Fields[capacity];
}

// деструктор, освобождение памяти
Vector::~Vector() {
    delete[] data;
}

// конструктор копирования
Vector::Vector(const Vector& other) : size(other.size), capacity(other.capacity) {
    data = new Fields[capacity];
    for (int i = 0; i < size; i++)
        data[i] = other.data[i];
}

// оператор присваивания
Vector& Vector::operator=(const Vector& other) {
    if (this == &other) return *this;
    delete[] data;
    size = other.size;
    capacity = other.capacity;
    data = new Fields[capacity];
    for (int i = 0; i < size; i++)
        data[i] = other.data[i];
    return *this;
}

// добавление новой записи в массив
void Vector::push_back(const Fields& record) {
    if (size == capacity) resize();
    data[size++] = record;
}

// возвращает ссылку на элемент с индексом i. Ссылка позволяет изменять элемент
Fields& Vector::operator[](unsigned int i) {
    return data[i];
}

// возвращает константную ссылку, изменить элемент нельзя
const Fields& Vector::operator[](unsigned int i) const {
    return data[i];
}

// очистка массива
void Vector::clear() {
    delete[] data;
    size = 0;
    capacity = 5;
    data = new Fields[capacity];
}

// Сериализация в бинарный поток
void Fields::write_binary(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&id), sizeof(id));

    // запись строк в бинарный файл: сначала длина строки, потом ее символы
    int len = title.size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(title.c_str(), len);
    len = director.size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(director.c_str(), len);

    os.write(reinterpret_cast<const char*>(&release_year), sizeof(release_year));
    os.write(reinterpret_cast<const char*>(&rating), sizeof(rating));
}

// Десериализация из бинарного потока
void Fields::read_binary(std::istream& is) {
    is.read(reinterpret_cast<char*>(&id), sizeof(id));

    //чтение строк из бинарного файла: читаем длину строки, выделяем под нее память, читаем ее символы
    int len;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    title.resize(len);
    if (len) { is.read(&title[0], len); }
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    director.resize(len);
    if (len) { is.read(&director[0], len); }

    is.read(reinterpret_cast<char*>(&release_year), sizeof(release_year));
    is.read(reinterpret_cast<char*>(&rating), sizeof(rating));
}