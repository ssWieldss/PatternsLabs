#include <iostream>
#include <string>
#include <regex>
#include <Windows.h>

class SpacesInterpreter;
class DashesInterpreter;
class Quotes;
class BracketsInterpreter;
class PunctuationAndSpacesInterpreter;
class SlashInterpreter;
class TabsInterpreter;


class MainInterpreter {
private:
    MainInterpreter* spaces;
    MainInterpreter* dashes;
    MainInterpreter* quotes;
    MainInterpreter* brackets;
    MainInterpreter* punctuationAndSpacesInterpreter;
    MainInterpreter* slashNs;
    MainInterpreter* tabs;

    std::vector<MainInterpreter* > temp;

protected:
    MainInterpreter(int i) {};

    virtual std::regex search_str() {
        return std::regex();
    }
    virtual std::string replace_str() {
        return std::string();
    }

public:
    MainInterpreter();

    virtual std::string interpret(std::string input) {
        for (int i = 0; i < 7; i++) {
            input = temp[i]->interpret(input);
        }
        //input = slashNs->interpret(input);
        //input = tabs->interpret(input);
        //input = spaces->interpret(input);
        //input = dashes->interpret(input);
        //input = quotes->interpret(input);
        //input = brackets->interpret(input);
        //input = punctuationAndSpacesInterpreter->interpret(input);
        return input;
    }
};

class SpacesInterpreter : public MainInterpreter {
public:
    SpacesInterpreter() : MainInterpreter(1) {}

protected:
    virtual std::string interpret(std::string input) {
        std::string result;
        std::regex_replace(std::back_inserter(result), input.begin(), input.end(), search_str(), replace_str());
        return result;
    }
    std::regex search_str() {
        return std::regex(" +");
    }

    std::string replace_str() {
        return " ";
    }
};

class DashesInterpreter : public MainInterpreter {
public:
    DashesInterpreter() : MainInterpreter(1) {}

protected:
    virtual std::string interpret(std::string input) {
        std::string result;
        std::regex_replace(std::back_inserter(result), input.begin(), input.end(), search_str(), replace_str());
        return result;
    }
    std::regex search_str() {
        return std::regex("\\s-\\s");
    }
    std::string replace_str() {
        return "Ч";
    }
};

class QuotesInterpreter : public MainInterpreter {
public:
    QuotesInterpreter() : MainInterpreter(1) {}

protected:
    virtual std::string interpret(std::string input) {
        std::string result1, result2;
        std::regex_replace(std::back_inserter(result1), input.begin(), input.end(), std::regex("Д"), "Ђ");
        std::regex_replace(std::back_inserter(result2), result1.begin(), result1.end(), std::regex("У"), "ї");
        return result2;
    }
};

class BracketsInterpreter : public MainInterpreter {
public:
    BracketsInterpreter() : MainInterpreter(1) {}

protected:
    virtual std::string interpret(std::string input) {
        std::string result1, result2;
        std::regex_replace(std::back_inserter(result1), input.begin(), input.end(), std::regex("\\( "), "(");
        std::regex_replace(std::back_inserter(result2), result1.begin(), result1.end(), std::regex(" \\)"), ")");
        return result2;
    }
};


class PunctuationAndSpacesInterpreter : public MainInterpreter {
public:
    PunctuationAndSpacesInterpreter() : MainInterpreter(1) {}

protected:
    virtual std::string interpret(std::string input) {
        std::string result;
        std::regex_replace(std::back_inserter(result), input.begin(), input.end(), search_str(), replace_str());
        return result;
    }
    std::regex search_str() {
        return std::regex(" +([.,;:])");
    }
    std::string replace_str() {
        return "$1";
    }
};


class SlashInterpreter : public MainInterpreter {
public:
    SlashInterpreter() : MainInterpreter(1) {}

protected:
    virtual std::string interpret(std::string input) {
        std::string result;
        std::regex_replace(std::back_inserter(result), input.begin(), input.end(), search_str(), replace_str());
        return result;
    }
    std::regex search_str() {
        return std::regex("\\n+");
    }
    std::string replace_str() {
        return "\n";
    }
};


class TabsInterpreter : public MainInterpreter {
public:
    TabsInterpreter() : MainInterpreter(1) {}

protected:
    virtual std::string interpret(std::string input) {
        std::string result;
        std::regex_replace(std::back_inserter(result), input.begin(), input.end(), search_str(), replace_str());
        return result;
    }
    std::regex search_str() {
        return std::regex("\\t+");
    }
    std::string replace_str() {
        return "\t";
    }
};


MainInterpreter::MainInterpreter() {
    /*this->spaces = new SpacesInterpreter();
    this->dashes = new DashesInterpreter();
    this->quotes = new QuotesInterpreter();
    this->brackets = new BracketsInterpreter();
    this->punctuationAndSpacesInterpreter = new PunctuationAndSpacesInterpreter();
    this->slashNs = new SlashInterpreter();
    this->tabs = new TabsInterpreter();*/
    this->temp = { new SpacesInterpreter(), new DashesInterpreter(), new QuotesInterpreter(), 
        new BracketsInterpreter(), new PunctuationAndSpacesInterpreter(), 
        new SlashInterpreter(), new TabsInterpreter() };

};

int main() {
    SetConsoleOutputCP(1251);
    MainInterpreter* interpreter = new MainInterpreter();
    std::string old_text = " уча   пробелов, использование - дефиса, ДкавычкиУ,    табул€ци€, ( лишние , пробелы  ), " 
        "ѕере\n\n\nнос";

    std::string new_text = interpreter->interpret(old_text);
    std::cout << "»значальный текст " << std::endl << std::endl << old_text << std::endl << std::endl;
    std::cout << "Ќовый текст " << std::endl << std::endl << new_text << std::endl << std::endl;
    return 0;
}