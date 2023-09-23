#include <iostream>
#include <string>

class MyException : public std::runtime_error {
public:
    MyException(const std::string& message) : std::runtime_error(message) {}
};

class Token{
    std::string name;
    std::string type;

public:
    Token() : name(""), type("") {};
    Token(std::string name, std::string type) : name(name), type(type) {};
}

class Lexer{
    std::string text;
    int pos;
    char current_char;
public:
    Lexer() : text(""), pos(0), current_char('\0') {};
    Lexer(std::string text) : text(text), pos(0) {
        current_char = text[pos];
    }

    void advance(){
        ++pos;
        if(pos <= text.length() - 1) {
            current_char = text[pos];
        }
        else current_char = '\0';
    }

    void skip_whitespaces(){
        while(current_char && ((current_char) == 32 || int(current_char) == 34)){
            advance();
        }
    }

    Token get_next_token(){
        while(int(current_char) != 0) {
            skip_whitespaces();

        } 
    }
}

class Interpreter{
    Lexer lexer;
    Token current_token;
public:
    Interpreter() : lexer(), current_token {};
    Interpreter(Lexer lexer) : lexer(lexer) {
        current_token = this->lexer.get_next_tocken();
    }

    void create(){
        eat("CREATE");
        eat("SET NAME"); // must be factor or someting
    }

    void contains(){
        eat("CONTAINS");
        eat("SET NAME"); // must be factor or someting
        eat("VALUE"); // must be factor or someting
    }

    void print_tree(){
        eat("PRINT TREE");
        eat("SET NAME"); // must be factor or someting
    }

    void search(){
        eat("SEARCH");
        eat("SET NAME"); // must be factor or someting
        if(current_token.value == "WHERE") {
            query(set_name);
        }
        if(current_token.value == "ASC" || current_token.value == "DESC") {

        }
    }

    void query(std::string set_name){
        if(current_token.value == "BETWEEN") {
            eat("BETWEEN");
            eat("FROM"); // must be factor or someting
            eat(","); // must be factor or someting
            eat("TO"); // must be factor or someting
        }
        else if(current_token.value == "MATCH") {
            eat("MATCH");
            eat("PATTERN"); // must be factor or someting
        }
        else throw MyException("Syntax error");
    }
}