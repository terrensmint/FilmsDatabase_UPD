#include "main.h"
#include <iostream>
#include <string>

int main() {
    // БД по умолчанию
    std::string default_db = "example.bin";
    std::string database_file = default_db;

    // пустая БД
    Database db;

    // Пытаемся загрузить БД из файла
    if (db.read(database_file)) {
        std::cout << "База данных загружена из " << database_file << "\n";
    } else {
        std::cout << "Файл " << database_file << " не найден. Создана новая база данных.\n";
    }

    bool run = true;   // флаг работы цикла меню

    // Основной цикл
    while (run) {
        int command = cmd::print_commands(database_file);
        std::cout << "\n";

        // Создаём менеджер команд
        cmd::CmdManager manager(db);
        
        switch (command) {
            case -1: {
                break;
            }
            case 0: {   // выход
                manager.exit(database_file);
                run = false;
                break;
            }
            case 1: {   // импорт
                manager.import_db(database_file, default_db);
                break;
            }
            case 2: {   // экспорт
                manager.export_db(database_file);
                break;
            }
            case 3:     // вывод
                std::cout << db;
                break;
            case 4: {   // добавление записи
                manager.add();
                break;
            }
            case 5: {   // удаление записи
                manager.del();
                break;
            }
            case 6: {   // поиск записей
                manager.search();
                break;
            }
            case 7: {   // редактирование записи
                manager.edit();
                break;
            }
            default:
                std::cout << "====[ Неверный ввод ]====\n";
                std::cout << "Команды с номером " << command << " не существует!\n";
        }

        // Ожидание Enter перед возвратом в меню
        if (run) {
            std::cout << "Нажмите Enter, чтобы вернуться в меню...";
            std::cin.get(); // ждём нажатия
        }
    }
    return 0;
}