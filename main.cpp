#include <iostream>
#include <vector>
#include <limits>
#include <windows.h>

#include "set.h"

using namespace std;

enum Sets {None, A, B};
const std::vector<std::string> MainMenu = {
                                            string("Неизвестная операция"),
                                            string("Множество A"),
                                            string("Множество B"),
                                            string("Проверить равенство множеств"),
                                            string("Математические операции"),
                                            string("Передать значения A B"),
                                            string("Передать значения B A"),
                                            string("Закрыть программу")
                                          };
const std::vector<std::string> EditSetMenu = {
                                                string("Неизвестная операция"),
                                                string("Добавить элемент"),
                                                string("Удалить элемент"),
                                                string("Очистить множество"),
                                                string("Вывести элементы множества"),
                                                string("Изменить тип множества"),
                                                string("Проверить принадлежность элемента множеству"),
                                                string("Вывести мощность множества"),
                                                string("Вывести количество уникальных элементов"),
                                                string("Вернуться назад")
                                             };
const std::vector<std::string> OperationsMenu = {
                                                    string("Неизвестная операция"),
                                                    string("Объединение"),
                                                    string("Пересечение"),
                                                    string("Разность A\\B"),
                                                    string("Разность B\\A"),
                                                    string("Симметрическая разность"),
                                                    string("Вернуться назад")
                                                };

int Input(string label) {
    int number;
    while (true) {
        cout << label;
        if (cin >> number && cin.peek() == '\n') {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return number;
        } else {
            cout << "Некорректный ввод" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


void Print(Set *s) {
    cout << "{ ";
    int size = s->getPower();
    for (int i = 0; i < size; i++) {
        if (i != size-1) {
            cout << (*s)[i] << ", ";
        } else {
            cout << (*s)[i];
        }
    }
    cout << " }" << endl;
}

int DoMenu(const std::vector<std::string>& vec) {
    int i = 0;
    for (const auto& s : vec) {
        if (i != 0)
            cout << i << " - " << s << endl;
		i++;
    }
    cout << "-> ";
    if (!(cin >> i)) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 0;
    }
    return i<1 || i > vec.size()-1 ? 0 : i;
}

void EditSet(Set** s, char name) {
    cout << endl;
    int action;
    do {
        try {
            cout << "Обрабатываемое множество: " << name << endl;
            action = DoMenu(EditSetMenu);
            switch (action) {
                case 0:
                    cout << EditSetMenu[action] << endl;
                    break;
                case 1: {
                    int el = Input("Введите одно число: ");
                    (*s)->Add(el);
                    break;
                }
                case 2: {
                    int el = Input("Введите одно число: ");
                    (*s)->Delete(el);
                    break;
                }
                case 3:
                    (*s)->Clear();
                    break;
                case 4:
                    Print(*s);
                    break;
                case 5: {
                    Set* new_set;
                    if ((*s)->getType() == SimpleSet) {
                        new_set = (*s)->switchType(MultiSet);
                        cout << "Множество " << name << " стало мультимножеством" << endl;
                    } else {
                        new_set = (*s)->switchType(SimpleSet);
                        cout << "Множество " << name << " стало стандартным множеством" << endl;
                    }
                    delete (*s);
                    (*s) = new_set;
                    break;
                }
                case 6: {
                    int el = Input("Введите одно число: ");
                    cout << "Элемент ";
                    cout << el;
                    if ((*s)->isExists(el)) {
                        cout << " входит ";
                    } else {
                        cout << " не входит ";
                    }
                    cout << "в множество " << name << endl;
                    break;
                }
                case 7:
                    cout << "Мощность множества: ";
                    cout << (*s)->getPower();
                    cout << endl;
                    break;
                case 8:
                    cout << "Количество уникальных элементов множества: ";
                    cout << (*s)->getUniqueNumbers();
                    cout << endl;
                    break;
                case 9:
                    return;
            }
            cout << endl;
        } catch (invalid_argument e) {
            cout << e.what() << endl;
        } catch (bad_alloc e) {
            cout << e.what() << endl;
        }
    } while (true);
}

void Operations(Set* A, Set* B) {
    cout << endl;
    int action;
    Set* result = nullptr;
    try {
        do {
            action = DoMenu(OperationsMenu);
            switch (action) {
                case 0:
                    cout << OperationsMenu[action] << endl;
                    break;
                case 1:
                    result = (*A) + (*B);
                    break;
                case 2:
                    result = (*A) * (*B);
                    break;
                case 3:
                    result = (*A) - (*B);
                    break;
                case 4:
                    result = (*B) - (*A);
                    break;
                case 5:
                    result = (*A) % (*B);
                    break;
                case 6:
                    return;
            }
            if (result != nullptr) {
                Print(result);
                delete result;
                result = nullptr;
            }
            cout << endl;
        } while (true);
    } catch (bad_alloc e) {
        cout << e.what() << endl;
    }
}

int main() {
    SetConsoleCP(65001);       // Включаем UTF-8 для ввода с клавиатуры
    SetConsoleOutputCP(65001); // Включаем UTF-8 для вывода в консоль

    int action;
    bool stop = true;

    Set* A = Set::Create(SimpleSet);
    Set* B = Set::Create(MultiSet);

    do {
        action = DoMenu(MainMenu);
        switch (action) {
            case 0:
                cout << MainMenu[action] << endl;
                break;
            case 1:
                EditSet(&A, 'A');
                break;
            case 2:
                EditSet(&B, 'B');
                break;
            case 3: {
                if ((*A) == (*B)) {
                    cout << "Множество A равно множеству B" << endl;
                }
                if (((*A) != (*B))) {
                    cout << "Множество A не равно множеству B" << endl;
                }
                break;
            }
            case 4:
                Operations(A, B);
                break;
            case 5:
                (*B) = (*A);
                break;
            case 6:
                (*A) = (*B);
                break;
            case 7:
                stop = false;
                break;
        }
        cout << endl;
    } while (stop);

    delete A;
    delete B;

    return 0;
}
