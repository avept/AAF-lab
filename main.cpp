#include "parser.h"


int main() {
    std::cout << "OAA> ";
    std::string input;
    std::getline(std::cin, input);
    
    while (true) {
        try {
            if (input.length() == 0)
                break;
            Lexer lexer = Lexer(input);
            Interpreter inter = Interpreter(lexer);
            inter.command();
            std::cout << "OAA> ";
            std::getline(std::cin, input);
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << '\n';
            std::cout << "OAA> ";
            std::getline(std::cin, input);
        }
    }
}