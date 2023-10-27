#include "parser.h"

std::string STRING = "STRING";
std::string FULL_STRING = "FULL_STRING";
std::string CREATE = "CREATE";
std::string PRINT_TREE = "PRINT_TREE";
std::string INSERT = "INSERT";
std::string CONTAINS = "CONTAINS";
std::string SEARCH = "SEARCH";
std::string COMMA = "COMMA";
std::string QUOTE = "QUOTE ";
std::string WHERE = "WHERE";
std::string BETWEEN = "BETWEEN";
std::string MATCH = "MATCH";
std::string ASC = "ASC";
std::string DESC = "DESC";
std::string eof = "eof";
std::string SEMICOLON = "SEMICOLON";

bool is_digit(char c) {
    return (int(c) >= 48 || int(c) <= 57);
}

bool is_space(char c) {
    return (int(c) == 32 || c == '\n' || c == '\t' || c == '\r');
}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

std::string upper(const std::string& input) {
    std::string result = input;
    for (char& c : result) {
        c = std::toupper(c);
    }
    return result;
}

std::string clean(std::string& input) {
    std::string cleaned_string = "";
    for (char& c : input) {
        if (c != '*' && c != '?') {
            cleaned_string += c;
        }
    }
    return cleaned_string;
}

bool matches_regex(const std::string& input) {
    std::regex pattern("[a-zA-Z][a-zA-Z0-9_]*");
    return std::regex_match(input, pattern);
}

bool proper_pattern(const std::string& input) {
    std::regex pattern("[a-zA-Z?][a-zA-Z0-9_?]*[*]?");
    return std::regex_match(input, pattern);
}

Token::Token() : type(""), value("") {}

Token::Token(std::string type, std::string value) : type(type), value(value) {}

Lexer::Lexer() : text(""), pos(0), current_char(0) {}

Lexer::Lexer(std::string text) : text(text), pos(0), current_char(text[0]) {}

void Lexer::advance() {
    ++pos;
    if (pos <= text.length() - 1) {
        current_char = text[pos];
    } else current_char = '\0';
}

void Lexer::skip_whitespace() {
    while (is_space(current_char)) {
        advance();
    }
}

std::string Lexer::get_string() {
    std::string result = "";
    while (current_char && !is_space(current_char) && current_char != ';') {
        result += current_char;
        advance();
    }
    if (matches_regex(result)) return result;
    else throw MyException("Invalid characters within the input string");
}

std::string Lexer::get_full_string() {
    std::string result = "";
    while (current_char && current_char != '"') {
        result += current_char;
        advance();
    }
    return result;
}

Token Lexer::get_next_token() {
    while (int(current_char) != 0) {
        skip_whitespace();
        if (is_alpha(current_char)) {
            std::string input = get_string();
            std::string upper_input = upper(input);
            
            if (upper_input == CREATE || upper_input == INSERT || upper_input == PRINT_TREE || upper_input == CONTAINS || upper_input == SEARCH || upper_input == ASC || upper_input == DESC || upper_input == WHERE || upper_input == BETWEEN || upper_input == MATCH) {
                return Token(upper_input, upper_input);
            } else return Token(STRING, input);
        } else if (current_char == ',') {
            advance();
            return Token(COMMA, "COMMA");
        } else if (current_char == '"') {
            advance();
            std::string result = get_full_string();
            advance();
            return Token(FULL_STRING, result);
        } else if (current_char == ';') {
            advance();
            return Token(SEMICOLON, ";");
        } else throw MyException("Invalid character: " + std::string(1, current_char));
    }
    return Token(eof, "");
}

void create(std::string set_name) {
    set_name = clean(set_name);
    std::cout << set_name << " has been created\n";
}

void print_tree(std::string set_name) {
    set_name = clean(set_name);
    std::cout << set_name << " has been printed\n";
}

void search(std::string set_name, std::string search_type) {
    set_name = clean(set_name);
    std::cout << "Searching words in " << set_name << " in " << search_type << " order\n";
}

void between(std::string set_name, std::string from, std::string to, std::string order) {
    set_name = clean(set_name);
    std::cout << "Searching words in " << set_name << " from " << from << " to " << to << " in " << order << " order\n";
}

void match(std::string set_name, std::string pattern, std::string order) {
    set_name = clean(set_name);
    std::cout << "Searching words in " << set_name << " with the next pattern: " << pattern << " in " << order << " order\n";
}

Interpreter::Interpreter() : lexer(), current_token() {}

Interpreter::Interpreter(Lexer lexer) : lexer(lexer) {
    current_token = this->lexer.get_next_token();
}

void Interpreter::eat(std::string expected_type){
    if(current_token.type == expected_type) {
        current_token = lexer.get_next_token();
    }
    else throw MyException("Syntax error. Ivanlid token type. Expecnted: " + expected_type + " Got: " + current_token.type);
}

void Interpreter::command() {
    if (current_token.type == CREATE || current_token.type == PRINT_TREE) {
        type1();
    } else if (current_token.type == CONTAINS || current_token.type == INSERT) {
        type2();
    } else if (current_token.type == SEARCH) {
        type3();
    } else throw MyException("Syntax error: unknown command: " + current_token.value);
    eat(SEMICOLON); // Every input should contain ";" in the end. After that none of the symbols are read.
}

void Interpreter::type1() {
    if(current_token.type == CREATE) {
        eat(CREATE);

        Token token = current_token;
        if(current_token.type == STRING) eat(STRING);
        else if(current_token.type == FULL_STRING) eat(FULL_STRING);
        else throw MyException("Syntax error! Expected STRING or FULL_STING token type. Got: " + current_token.type);

        create(token.value);
    }
    else if(current_token.type == PRINT_TREE) {
        eat(PRINT_TREE);

        Token token = current_token;
        if(current_token.type == STRING) eat(STRING);
        else if(current_token.type == FULL_STRING) eat(FULL_STRING);
        else throw MyException("Syntax error! Expected STRING or FULL_STING token type. Got: " + current_token.type);

        print_tree(token.value);
    }
}
    

// calling type 2 functions: insert and contains
void Interpreter::type2() {
    if(current_token.type == INSERT) {
        eat(INSERT);

        std::string set_name = current_token.value;
        if(current_token.type == STRING) eat(STRING);
        else if(current_token.type == FULL_STRING) eat(FULL_STRING);
        else throw MyException("Syntax error! Expected STRING or FULL_STING token type. Got: " + current_token.type);
    
        std::string value = current_token.value;
        eat(FULL_STRING);
        std::cout << value << " has been inserted into " << set_name << '\n';
    }
    else if(current_token.type == CONTAINS) {
        eat(CONTAINS);

        std::string set_name = current_token.value;
        if(current_token.type == STRING) eat(STRING);
        else if(current_token.type == FULL_STRING) eat(FULL_STRING);
        else throw MyException("Syntax error! Expected STRING or FULL_STING token type. Got: " + current_token.type);

        std::string value = current_token.value;
        eat(FULL_STRING);
        std::cout << value << " has been looked at " << set_name << '\n';
    }
}

void Interpreter::type3() {
    eat(SEARCH);

    std::string set_name = current_token.value;
    if(current_token.type == STRING) eat(STRING);
    else if(current_token.type == FULL_STRING) eat(FULL_STRING);
    else throw MyException("Syntax error! Expected STRING or FULL_STING token type. Got: " + current_token.type);

    if(current_token.type == WHERE) {
        eat(WHERE);
        query(set_name);
    }
    else if(current_token.type == ASC || current_token.type == DESC || current_token.type == SEMICOLON) {
        if(current_token.type == SEMICOLON) {
            search(set_name, ASC);
        }
        else if(current_token.type == ASC) {
            eat(ASC);
            search(set_name, ASC);
        }
        else if(current_token.type == DESC){
            eat(DESC);
            search(set_name, DESC);
        }
    }
    else throw MyException("Syntax error! Invalid token. Expected: ASC or DESC or oef Got: " + current_token.type);
}
    
// calling query, which can be either "BETWEEN" or "MATCH"
void Interpreter::query(std::string set_name) {
    if(current_token.type == BETWEEN) {
        eat(BETWEEN);
        std::string from = current_token.value;
        eat(FULL_STRING);
        eat(COMMA);
        std::string to = current_token.value;
        eat(FULL_STRING);

        std::string order = ASC;
        if(current_token.type == ASC) {
            eat(ASC);
        }
        else if(current_token.type == DESC){
            eat(DESC);
            order = DESC;
        }
        else if(current_token.type == SEMICOLON) {
            order = ASC;
        }
        else {
            throw MyException("Syntax error! Invalid token. Expected: ASC or DESC. Got: " + current_token.type);
        }
        between(set_name, from, to, order);
    }
    else if(current_token.type == MATCH) {
        eat(MATCH);

        std::string pattern = current_token.value;
        if(!proper_pattern(pattern)) throw MyException("Invalid pattern!");
        eat(FULL_STRING);

        std::string order = ASC;
        if(current_token.type == ASC) {
            eat(ASC);
        }
        else if(current_token.type == DESC){
            eat(DESC);
            order = DESC;
        }
        else if(current_token.type == SEMICOLON) {
            order = ASC;
        }
        else {
            throw MyException("Syntax error! Invalid token. Expected: ASC or DESC. Got: " + current_token.type);
        }
        match(set_name, pattern, order);
    }
    else throw MyException("Syntax error! Expected: MATCH or BETWEEN. Got: " + current_token.type);
}