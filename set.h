#ifndef SET_H
#define SET_H

#include <vector>
#include <stdexcept>

enum SetsID {SimpleSet, MultiSet};

class Set {
private:
    template <typename F>
    static Set* math(const Set& first, const Set& second, F formula);
protected:
    std::vector<int> elements;
public:
    Set();

    static Set* Create(SetsID typeSet);
    virtual SetsID getType(void) const = 0;
    Set* switchType(SetsID typeSet);

    virtual void Add(const int el) = 0;
    void Delete(const int el);
    void Clear(void);

    bool isExists(const int el) const;
    int getPower(void) const;
    int getUniqueNumbers(void) const;
    int getCountElement(const int el) const;

    int operator[](const int n) const {
        if (n < 0 || n >= this->getPower()) {
            throw std::out_of_range("Индекс выходит за допустимые границы");
        }
        return elements[n];
    }

    Set& operator=(const Set& other);

    bool operator==(const Set& other) const; // равенство
    bool operator!=(const Set& other) const; // не равенство

    Set* operator+(const Set& other) const; // объединение
    Set* operator*(const Set& other) const; // пересечение
    Set* operator-(const Set& other) const; // разность
    Set* operator%(const Set& other) const; // симметрическая разность

    virtual ~Set();
};

#endif // SET_H
