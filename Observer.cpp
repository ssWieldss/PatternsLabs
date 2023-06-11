#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <Windows.h>

using namespace std;

class Teacher;

struct Date {
    int day;
    int month;
    int year;
    int term;
    Date(int d, int m, int y, int t) {
        this->day = d;
        this->month = m;
        this->year = y;
        this->term = t;
    }
};


class Info {
public:
    virtual ~Info() { };
};


class DataBase : public Info {
private:
    std::vector<std::pair<Teacher*, std::vector<Date*>>> teacherUPDDate;
public:
    void addTeacher(Teacher* teacher) {
        std::vector<Date*> temp;
        this->teacherUPDDate.push_back(std::make_pair(teacher, temp));
    }
    void addStatement(Teacher* teacher, Date* date) {
        auto finded = std::find_if(teacherUPDDate.begin(), teacherUPDDate.end(),
            [&](const auto& x) { return x.first == teacher; });
        this->teacherUPDDate[finded - teacherUPDDate.begin()].second.push_back(date);
    }
    std::vector<std::pair<Teacher*, std::vector<Date*>>> getDataBase() {
        return this->teacherUPDDate;
    }
};

class IObserver {
public:
    virtual void update(Info* info) = 0;
};

class IObservable {
private:
    Info* info;
protected:
    vector<IObserver*> observers;

public:
    virtual void registerObserver(IObserver* o) {
        this->observers.push_back(o);
    };

    virtual void removeObserver(IObserver* o) {
        this->observers.erase(std::remove(this->observers.begin(), this->observers.end(), o), this->observers.end());
    };

    virtual void notifyObservers() {
        for (auto& observer : this->observers) {
            observer->update(this->info);
        }
    };
};


class Teacher {
private:
    string fio;
public:
    string getFio() {
        return this->fio;
    }

    Teacher(string fio) {
        this->fio = fio;
    }

    void addStatement(Date* date, DataBase* db) {
        db->addStatement(this, date);
    }
};


class Faculty : public IObservable {
private:
    DataBase* db;
public:
    Faculty(DataBase* db) {
        this->db = db;
    }
    void notifyObservers() override {
        for (auto& observer : this->observers) {
            observer->update(this->db);
        }
    };

    void verify() {
        notifyObservers();
    }
};


class Dean : public IObserver {
private:
    Date* controlDate;
public:
    Date getControlDate() {
        return *this->controlDate;
    }
    void setControlDate(Date* date) {
        this->controlDate = date;
    }

    void update(Info* info) override {

        DataBase* db = dynamic_cast<DataBase*>(info);
        std::vector<std::pair<Teacher*, std::vector<Date*>>> data = db->getDataBase();

        for (size_t i = 0; i < data.size(); i++ ) {
            for(size_t j = 0; j < data[i].second.size(); j++){
                if (data[i].second[j]->term == this->controlDate->term 
                    && data[i].second[j]->day >= this->controlDate->day 
                    && data[i].second[j]->month >= this->controlDate->month
                    && data[i].second[j]->year >= this->controlDate->year) 

                    cout << "Teacher " << data[i].first->getFio() << " hasn't given the grades yet." << endl;

            }
        }
    }
};

int main() {

    SetConsoleOutputCP(1251);

    Teacher* teacher1 = new Teacher("Ivanov Ivan Ivanovich");
    Teacher* teacher2 = new Teacher("Petrov Petr Petrovich");

    Date* dateVer;
    Date* date1;
    Date* date2;

    DataBase* db = new DataBase();
    db->addTeacher(teacher1);
    db->addTeacher(teacher2);

    dateVer = new Date(15, 1, 2023, 1);
    date1 = new Date(14, 1, 2023, 1);
    date2 = new Date(16, 1, 2023, 1);

    Dean* dean = new Dean();
    dean->setControlDate(dateVer);

    teacher1->addStatement(date1, db);
    teacher2->addStatement(date2, db);

    Faculty* fac = new Faculty(db);
    fac->registerObserver(dean);

    fac->verify();
}