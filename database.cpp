#include "main.h"
#include "vector.h"
#include <fstream>
#include <iostream>

// пустая БД
Database::Database() {}

// конструктор с инициализацией
Database::Database(Vector& database) : db(database) {}

// вернуть количество записей в БД
unsigned int Database::size() const {
    return db.get_size();
}

// добавить запись в БД
void Database::add_rec(const Fields& record) {
    db.push_back(record);
}

// вывод БД
void Database::print(std::ostream& os) const {
    if (db.get_size() == 0) {
        os << "База данных пуста.\nИмпортируйте базу данных или добавьте новые элементы.\n";
        return;
    }
    os << "=====================\n";
    for (int i = 0; i < db.get_size(); i++) {
        const Fields& record = db[i];
        os << "ID: " << record.get_id() << "\n";
        os << "Title: " << record.get_title() << "\n";
        os << "Director: " << record.get_director() << "\n";
        os << "Release: " << record.get_year() << "\n";
        os << "Rating: " << record.get_rating() << "\n";
        os << "--------\n";
    }
    os << "=====================\n";
}

// возвращает ссылку на запись с индексом i в БД, изменить запись можно
Fields& Database::operator[](unsigned int i) {
    return db[i];
}

// возвращает константную ссылку на запись, изменить нельзя
const Fields& Database::operator[](unsigned int i) const {
    return db[i];
}


// поиск записи: по эталонной записи сравниваются все записи в БД и возвращается новая БД из найденных записей
Database Database::search_recs(const Fields& search_rec) const {
    Database result;
    for (int i = 0; i < db.get_size(); i++) {
        const Fields& record = db[i];
        bool flag = true;

        if (search_rec.get_id() != -1 && record.get_id() != search_rec.get_id()) flag = false;
        if (!search_rec.get_title().empty() && record.get_title() != search_rec.get_title()) flag = false;
        if (!search_rec.get_director().empty() && record.get_director() != search_rec.get_director()) flag = false;
        if (search_rec.get_year() != -1 && record.get_year() != search_rec.get_year()) flag = false;
        if (search_rec.get_rating() != -1.0f && record.get_rating() != search_rec.get_rating()) flag = false;

        if (flag) result.add_rec(record);
    }
    return result;
}

// удаление записи
void Database::delete_rec(int id) {
    Vector temp;
    for (int i = 0; i < db.get_size(); i++) {
        if (db[i].get_id() != id) {
            temp.push_back(db[i]);
        }
    }
    db = std::move(temp);
}

// редактирование записи. Заменяем запись с выбранным id на новую, которая создается пользователем через cmd manager
void Database::edit_rec(const Fields& record) {
    for (int i = 0; i < db.get_size(); i++) {
        if (db[i].get_id() == record.get_id()) {
            db[i] = record;
            return;
        }
    }
}

// запись БД в бинарный файл
bool Database::write(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    for (int i = 0; i < db.get_size(); i++) {
        db[i].write_binary(file);   // сериализация
    }
    return true;
}

// чтение БД из бинарного файла
bool Database::read(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;

    // очищаем БД
    Vector new_db;
    db = new_db;

    Fields record;
    while (file.peek() != EOF) {
        record.read_binary(file);   // десериализация
        add_rec(record);    // добавление записи в БД
    }
    return true;
}

// вывод БД
std::ostream& operator<<(std::ostream& os, const Database& db) {
    db.print(os);
    return os;
}