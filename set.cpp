#include <vector>
#include <algorithm>

#include "set.h"

using namespace std;

class Simple : public Set {
public:
    void Add(const int el) override;
    SetsID getType() const override;
};

class Multi : public Set {
public:
    void Add(const int el) override;
    SetsID getType() const override;
};

Set::Set() {
    elements = {};
}

Set* Set::Create(SetsID typeSet) {
    Set* s = nullptr;
    switch(typeSet) {
        case SimpleSet:
            s = new Simple();
            break;
        case MultiSet:
            s = new Multi();
            break;
        default:
            throw invalid_argument("Неизвестный тип множества");
    }
    return s;
}

SetsID Simple::getType() const {
    return SimpleSet;
}

SetsID Multi::getType() const {
    return MultiSet;
}

Set* Set::switchType(SetsID typeSet) {
    Set* new_class = nullptr;
    try {
        new_class = Create(typeSet);
        for (const auto& el : elements) {
            new_class->Add(el);
        }
    } catch (invalid_argument e) {
        throw invalid_argument("Неизвестный тип множества");
    }
    return new_class;
}

bool Set::isExists(const int el) const {
    return std::binary_search(elements.begin(), elements.end(), el);
}

void Simple::Add(const int el) {
    try {
        if (!isExists(el)) {
            auto pos = lower_bound(elements.begin(), elements.end(), el);
            elements.insert(pos, el);
        }
    } catch (bad_alloc e) {
        throw bad_alloc(e);
    }
}

void Multi::Add(const int el) {
    try {
        auto pos = lower_bound(elements.begin(), elements.end(), el);
        elements.insert(pos, el);
    } catch (bad_alloc e) {
        throw bad_alloc(e);
    }
}

void Set::Delete(const int el) {
    if (isExists(el)) {
        for (int i = 0; i < elements.size(); i++) {
            if (elements[i] == el) {
                elements.erase(elements.begin() + i);
                i--;
            }
        }
    } else {
        throw invalid_argument("Элемент не найден");
    }
}

void Set::Clear(void) {
    elements.clear();
}

int Set::getPower(void) const {
    return elements.size();
}

int Set::getUniqueNumbers(void) const {
    int size = this->getPower();

    if (this->getType() == SimpleSet) {
        return size;
    }

    int numbers = 0;

    for (int i = 0; i < size; i++) {
        numbers++;
        int el = (*this)[i];
        for (int j = i+1; j < size; j++) {
            if ((*this)[j] == el) {
                i++;
            }
        }
    }

    return numbers;
}

int Set::getCountElement(const int el) const {
    return count(elements.begin(), elements.end(), el);
}

Set::~Set() {
    Clear();
}

Set& Set::operator=(const Set& other) {
    if (this == &other) {
        return *this;
    }
    try {
        vector<int> elements_copy = elements;
        try {
            this->Clear();
            int other_size = other.getPower();
            for (int i = 0; i < other_size; i++) {
                this->Add(other[i]);
            }
            return *this;
        } catch (bad_alloc e) {
            elements.swap(elements_copy);
            throw bad_alloc(e);
        }
    } catch (bad_alloc e) {
        throw bad_alloc(e);
    }
}


bool Set::operator==(const Set& other) const {
    int first_size = (*this).getPower();
    int second_size = other.getPower();

    if (first_size != second_size) {
        return false;
    }

    for (int i = 0; i < first_size; i++) {
        int first_el = (*this)[i];
        int second_el = other[i];

        if (first_el != second_el) {
            return false;
        }
    }

    return true;
}

bool Set::operator!=(const Set& other) const {
    return !(*this == other);
}

template <typename F>
Set* Set::math(const Set& first, const Set& second, F formula) {
    Set* result = nullptr;

    try {
        if (first.getType() == MultiSet || second.getType() == MultiSet) {
            result = new Multi();
        } else {
            result = new Simple();
        }

        Simple unique_numbers;

        int size_first = first.getPower();
        for (int i = 0; i < size_first; i++) {
            unique_numbers.Add(first[i]);
        }

        int size_second = second.getPower();
        for (int i = 0; i < size_second; i++) {
            unique_numbers.Add(second[i]);
        }

        for (int i = 0; i < unique_numbers.getPower(); i++) {
            int first_numbers = first.getCountElement(unique_numbers[i]);
            int second_numbers = second.getCountElement(unique_numbers[i]);

            int result_numbers = formula(first_numbers, second_numbers);

            for (int j = 0; j < result_numbers; j++) {
                result->Add(unique_numbers[i]);
            }
        }
        
        return result;
    } catch (bad_alloc e) {
        delete result;
        throw bad_alloc(e);
    }
}

Set* Set::operator+(const Set& other) const {
    return math(*this, other, [](int first_numbers, int second_numbers) {
        return max(first_numbers, second_numbers);
    });
}

Set* Set::operator*(const Set& other) const {
    return math(*this, other, [](int first_numbers, int second_numbers) {
        return min(first_numbers, second_numbers);
    });
}

Set* Set::operator-(const Set& other) const {
    return math(*this, other, [](int first_numbers, int second_numbers) {
        return max(first_numbers-second_numbers, 0);
    });
}

Set* Set::operator%(const Set& other) const {
    return math(*this, other, [](int first_numbers, int second_numbers) {
        return abs(first_numbers-second_numbers);
    });
}
