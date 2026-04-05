#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <string>

// Запись в БД
class Fields {
    int id;
    std::string title;
    std::string director;
    int release_year;
    float rating;

public:
    Fields() : id(-1), title(""), director(""), release_year(-1), rating(-1.0f) {}  // пустая запись

    Fields(int id_, const std::string& t, const std::string& d, int year, float r)
        : id(id_), title(t), director(d), release_year(year), rating(r) {}

    int get_id() const { return id; }
    const std::string& get_title() const { return title; }
    const std::string& get_director() const { return director; }
    int get_year() const { return release_year; }
    float get_rating() const { return rating; }

    void set_id(int new_id) { 
        if (new_id >= 0) 
        id = new_id; 
    }
    void set_title(const std::string& t) { title = t; }
    void set_director(const std::string& d) { director = d; }
    void set_year(int y) { release_year = y; }
    void set_rating(float r) { rating = r; }

    // Сериализация в бинарный поток
    void write_binary(std::ostream& os) const;
    void read_binary(std::istream& is);
};

class Vector {
private:
    Fields* data;
    unsigned int size;
    unsigned int capacity;

    void resize();

public:
    Vector();
    ~Vector();

    Vector(const Vector& other);
    Vector& operator=(const Vector& other);

    void push_back(const Fields& record);
    Fields& operator[](unsigned int i);
    const Fields& operator[](unsigned int i) const;
    unsigned int get_size() const { return size; }

    void clear();
};

#endif