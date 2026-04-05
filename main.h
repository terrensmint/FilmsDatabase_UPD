#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>
#include "vector.h"

// БД
class Database {
    Vector db;
public:
    Database();    // конструктор по умолчанию (пустая БД)
    Database(Vector& database);    // конструктор с инициализацией

    unsigned int size() const;
    void print(std::ostream& os) const;
    Fields& operator[](unsigned int i);
    const Fields& operator[](unsigned int i) const;

    void add_rec(const Fields& record);
    Database search_recs(const Fields& search_rec) const;
    void delete_rec(int id);
    void edit_rec(const Fields& record);
    bool read(const std::string& filename);
    bool write(const std::string& filename) const;
};

// вывод БД
std::ostream& operator<<(std::ostream& os, const Database& db);

// пространство имен для управления консольным интерфейсом
namespace cmd {
    class CmdManager {
        Database& db;   // ссылка на БД
    public:
        CmdManager(Database& database) : db(database) {}
        void import_db(const std::string& database_file, const std::string& default_db);
        void export_db(const std::string& database_file);
        void add();
        void del();
        void search();
        void edit();
        void exit(const std::string& database_file);
    };

    int print_commands(const std::string& database_file);
    void clear();   // очистка буфера ввода
}

#endif