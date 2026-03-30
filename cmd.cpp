#include "main.h"
#include <iostream>

namespace cmd {

// очистка буфера ввода
void clear() {
    std::cin.clear();
    while (std::cin.get() != '\n' && !std::cin.eof());
}

// безопасное чтение числа с очисткой буфера
bool read_int(int& value, const std::string& prompt) {
    std::cout << prompt;
    if (std::cin >> value) {
        clear();
        return true;
    }
    // если чтение не удалось
    std::cout << "====[ Неверный ввод ]====\n";
    clear();  // очищаем буфер
    return false;
}

// безопасное чтение числа float
bool read_float(float& value, const std::string& prompt) {
    std::cout << prompt;
    if (std::cin >> value) {
        clear();
        return true;
    }
    std::cout << "====[ Неверный ввод ]====\n";
    clear();
    return false;
}

// чтение строки с проверкой на пустую строку
void input_string(const std::string& field, std::string& buffer) {
    while (true) {
        std::cout << "[" << field << "] = ";
        std::getline(std::cin, buffer);
        
        if (buffer.empty()) {
            std::cout << "====[ Поле не может быть пустым ]====\n";
            continue;
        }
        break;
    }
}

// запрос да/нет (1/0)
int ask_yes_no(const std::string& question) {
    int answer;
    while (true) {
        std::cout << question << " (1 - Да, 0 - Нет): ";
        if (std::cin >> answer && (answer == 1 || answer == 0)) {
            clear();
            return answer;
        }
        std::cout << "====[ Неверный ввод ]====\n";
        clear();
    }
}

// запрос на заполнение поля
bool fill_question(const std::string& field) {
    return (ask_yes_no("Заполнить [" + field + "]?") == 1);
}

// интерфейс добавления записи
void CmdManager::add() {
    std::cout << "====[ Создание новой записи ]====\n\n";

    int rec_id;
    if (db.size() == 0) {
        rec_id = 1;
    } else {
        rec_id = db[db.size() - 1].get_id() + 1;
    }

    Fields record;
    record.set_id(rec_id);
    std::cout << "[ID] = " << rec_id << "\n";

    std::string title, director;
    int year;
    float rating;

    input_string("TITLE", title);
    input_string("DIRECTOR", director);

    while (!read_int(year, "[YEAR OF RELEASE] = "));
    while (!read_float(rating, "[RATING] = "));

    record.set_title(title);
    record.set_director(director);
    record.set_year(year);
    record.set_rating(rating);

    db.add_rec(record);
    std::cout << "Запись успешно добавлена!\n";
}

// интерфейс удаления записи
void CmdManager::del() {
    std::cout << "====[ Удаление записи ]====\n\n";

    int rec_id;
    while (!read_int(rec_id, "Введите ID записи, которую хотите удалить: "));

    // проверяем существование записи через поиск по ID
    Fields search_rec;
    search_rec.set_id(rec_id);
    if (db.search_recs(search_rec).size() == 0) {
        std::cout << "Запись с ID " << rec_id << " не найдена.\n";
        return;
    }

    db.delete_rec(rec_id);
    std::cout << "Запись с ID " << rec_id << " успешно удалена!\n";
}

// интерфейс поиска записи
void CmdManager::search() {
    std::cout << "====[ Поиск ]====\n";
    std::cout << "Выберите команду:\n";
    std::cout << "0 - Отменить поиск\n";
    std::cout << "1 - Искать по ID\n";
    std::cout << "2 - Искать по другим полям\n";

    int answer;
    if (!read_int(answer, "Введите номер команды: ")) {
        return; // отмена поиска
    }

    // пустая запись. если поле пустое, то по нему проверяться записи не будут
    Fields search_rec;
    search_rec.set_id(-1);
    search_rec.set_title("");
    search_rec.set_director("");
    search_rec.set_year(-1);
    search_rec.set_rating(-1.0f);

    switch(answer) {
        case 0:
            std::cout << "Поиск отменен.\n";
            return;
            
        case 1: {
            int id;
            while (!read_int(id, "Введите ID для поиска: "));
            search_rec.set_id(id);
            break;
        }
        
        case 2: {
            std::string title, director;
            int year;
            float rating;

            if (fill_question("TITLE")) {
                input_string("TITLE", title);
                search_rec.set_title(title);
            }
            if (fill_question("DIRECTOR")) {
                input_string("DIRECTOR", director);
                search_rec.set_director(director);
            }
            if (fill_question("YEAR OF RELEASE")) {
                while (!read_int(year, "[YEAR OF RELEASE] = "));
                search_rec.set_year(year);
            }
            if (fill_question("RATING")) {
                while (!read_float(rating, "[RATING] = "));
                search_rec.set_rating(rating);
            }
            break;
        }
        
        default:
            std::cout << "====[ Неверный ввод ]====\nПоиск отменен.\n";
            return;
    }

    Database result = db.search_recs(search_rec);
    if (result.size() == 0) {
        std::cout << "Записи не найдены.\n";
    } else {
        std::cout << "\nНайдено записей: " << result.size() << "\n";
        result.print(std::cout);    // вывод БД с найденными записями
    }
}

// интерфейс редактирования записи
void CmdManager::edit() {
    std::cout << "====[ Редактирование записи ]====\n\n";

    int rec_id;
    while (!read_int(rec_id, "Введите ID записи для редактирования: "));

    // Ищем запись по ID
    Fields search_rec;
    search_rec.set_id(rec_id);
    Database found = db.search_recs(search_rec);
    
    if (found.size() == 0) {
        std::cout << "Запись с ID " << rec_id << " не найдена.\n";
        return;
    }

    Fields record = found[0];  // копируем найденную запись
    
    std::string title = record.get_title();
    std::string director = record.get_director();
    int year = record.get_year();
    float rating = record.get_rating();

    std::cout << "\nТекущие значения:\n";
    std::cout << "TITLE: " << title << "\n";
    std::cout << "DIRECTOR: " << director << "\n";
    std::cout << "YEAR OF RELEASE: " << year << "\n";
    std::cout << "RATING: " << rating << "\n\n";

    if (fill_question("TITLE")) input_string("TITLE", title);
    if (fill_question("DIRECTOR")) input_string("DIRECTOR", director);
    if (fill_question("YEAR OF RELEASE")) {
        while (!read_int(year, "[YEAR OF RELEASE] = "));
    }
    if (fill_question("RATING")) {
        while (!read_float(rating, "[RATING] = "));
    }

    record.set_title(title);
    record.set_director(director);
    record.set_year(year);
    record.set_rating(rating);

    db.edit_rec(record);
    std::cout << "Запись с ID " << rec_id << " успешно отредактирована!\n";
}

// импорт базы данных
void CmdManager::import_db(std::string database_file, std::string default_db) {
    std::cout << "Введите название файла, который необходимо импортировать:\n";
    
    std::string new_file;
    std::getline(std::cin, new_file);
    
    // Если пользователь ничего не ввел или ввел пустую строку
    if (new_file.empty()) {
        std::cout << "Введено некорректное название. Будет импортирована база данных по умолчанию.\n";
        new_file = default_db;
    }
    
    // Пытаемся загрузить базу данных из указанного файла
    if (!db.read(new_file)) {
        std::cout << "====[ Ошибка ]====\n";
        std::cout << "Файл с названием '" << new_file << "' не найден.\n";
        std::cout << "Импортируем базу данных по умолчанию: " << default_db << "\n";
        
        if (!db.read(default_db)) {
            std::cout << "Файл по умолчанию также не найден. Создана пустая база данных.\n";
        }
    } else {
        std::cout << "База данных успешно импортирована из " << new_file << "!\n";
    }
}

// экспорт (сохранение) БД в бинарный файл
void CmdManager::export_db(std::string database_file) {
    std::string new_file;
    std::cout << "Введите название файла, в который хотите экспортировать базу данных.\n";
    std::cout << "(Введите 0 для экспорта в текущий файл)\n";
    
    std::getline(std::cin, new_file);
    
    // если пользователь ввел 0, используем текущий файл
    if (new_file == "0") {
        if (db.write(database_file)) {
            std::cout << "База данных успешно сохранена в файл " << database_file << "!\n";
        } else {
            std::cout << "Ошибка при сохранении базы данных.\n";
        }
    } 
    // если пользователь ввел новое название
    else if (!new_file.empty()) {
        if (db.write(new_file)) {
            std::cout << "База данных успешно сохранена в файл " << new_file << "!\n";
        } else {
            std::cout << "Ошибка при сохранении базы данных.\n";
        }
    } 
    // если введена пустая строка
    else {
        std::cout << "Введено некорректное название. Экспорт отменен.\n";
    }
}

// Выход из программы
void CmdManager::exit(std::string database_file) {
    int answer = ask_yes_no("Сохранить изменения перед выходом?");

    if (answer == 1) {
        if (db.write(database_file)) {
            std::cout << "База данных сохранена в " << database_file << "\n";
        } else {
            std::cout << "Ошибка при сохранении. Данные не сохранены.\n";
        }
    } else {
        std::cout << "Изменения не сохранены.\n";
    }

    std::cout << "====[ Выход из программы ]====\n";
}

// вывод списка команд
int print_commands(std::string database_file) {
    std::cout << "=================================\n";
    std::cout << "*    Управление базой данных    *\n";
    std::cout << "*            Netflix            *\n";
    std::cout << "=================================\n";
    std::cout << "Текущий файл базы данных: " << database_file << "\n\n";
    std::cout << "0 - Выход\n";
    std::cout << "1 - Импорт базы данных\n";
    std::cout << "2 - Экспорт базы данных\n";
    std::cout << "3 - Вывод базы данных\n";
    std::cout << "4 - Добавить запись\n";
    std::cout << "5 - Удалить запись\n";
    std::cout << "6 - Поиск записи\n";
    std::cout << "7 - Редактировать запись\n";

    int command;
    if (!read_int(command, "Введите номер команды: ")) {
        return -1;
    }
    return command;
}

} // namespace cmd