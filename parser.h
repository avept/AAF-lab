#pragma once

#include <string>
#include <regex>
#include <cctype>
#include <iostream>

bool is_digit(char c);
bool is_space(char c);
bool is_alpha(char c);
std::string upper(const std::string& input);
std::string clean(std::string& input);
bool matches_regex(const std::string& input);
bool proper_pattern(const std::string& input);

class MyException : public std::runtime_error {
public:
    MyException(const std::string& message) : std::runtime_error(message) {}
};

struct Token {
    std::string type, value;
    Token();
    Token(std::string type, std::string value);
};

class Lexer {
public:
    Lexer();
    Lexer(std::string text);
    void advance();
    void skip_whitespace();
    std::string get_string();
    std::string get_full_string();
    Token get_next_token();
    
private:
    std::string text;
    int pos;
    char current_char;
};

void create(std::string set_name);
void print_tree(std::string set_name);
void search(std::string set_name, std::string search_type = "ASC");
void between(std::string set_name, std::string from, std::string to, std::string order = "ASC");
void match(std::string set_name, std::string pattern, std::string order = "ASC");

class Interpreter {
public:
    Interpreter();
    Interpreter(Lexer lexer);
    void command();
    
private:
    Lexer lexer;
    Token current_token;
    void eat(std::string expected_type);
    void type1();
    void type2();
    void type3();
    void query(std::string set_name);
};
