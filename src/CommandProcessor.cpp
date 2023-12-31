#include "../include/CommandProcessor.h"
#include "../include/Parser.h"

void CommandProcessor::start() {
    std::cout << "OAA> ";
    
    std::vector<std::string> commands = { "create tree;", "insert tree \"aa\";", "insert tree \"ab\";", "insert tree \"ac\";",
                            "insert tree \"abc\";", "search tree where match \"a?*\" DESC;" };
    for(const auto& it : commands)
    {
        try {
            if(it.length() == 0) 
                break;

            Lexer lexer = Lexer(it);
            Interpreter inter = Interpreter(lexer);
            inter.command();
            
            std::cout << "OAA> ";
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
            std::cout << "OAA> ";
        }
    }
}

void CommandProcessor::create(const std::string& set_name)
{
    std::cout << set_name << " has been created\n";
    m_tries.emplace(set_name, Trie());
}

void CommandProcessor::print_tree(const std::string& set_name)
{
    if (m_tries.find(set_name) == m_tries.end())
    {
        std::cout << "Trie" << set_name << " not found." << std::endl;
        return;
    }

    std::cout << set_name << " has been printed\n";
    m_tries[set_name].print();
}

void CommandProcessor::insert(const std::string& set_name, const std::string& value)
{
    if (m_tries.find(set_name) == m_tries.end())
    {
        std::cout << "Trie" << set_name << " not found." << std::endl;
        return;
    }

    std::cout << value << " has been inserted into " << set_name << '\n';
    m_tries[set_name].insert(value);
}

void CommandProcessor::contains(const std::string& set_name, const std::string& value)
{
    if (m_tries.find(set_name) == m_tries.end())
    {
        std::cout << "Trie " << set_name << " not found." << std::endl;
        return;
    }

    if (m_tries[set_name].search(value))
    {
        std::cout << "Trie " << set_name << " contains " << value << "." << std::endl;
    }
    else
    {
        std::cout << "Trie " << set_name << " does not contain " << value << "." << std::endl;
    }
}

void CommandProcessor::search(const std::string& set_name, const std::string& search_type) {
    if (m_tries.find(set_name) == m_tries.end()) {
        std::cout << "Trie " << set_name << " not found." << std::endl;
        return;
    }

    std::cout << "Searching words in " << set_name << " in " << search_type << " order\n";
    auto strings = m_tries[set_name].allStrings("", "");

    if (search_type == "DESC") {
        std::reverse(strings.begin(), strings.end());
    }
    
    for (const auto& it : strings) {
        std::cout << it << std::endl;
    }

}


void CommandProcessor::between(const std::string& set_name, const std::string& from, const std::string& to, 
                const std::string& order)
{
    std::cout << "Searching words in " << set_name 
            << " from " << from << " to " << to << " in " << order << " order\n";

    if (m_tries.find(set_name) == m_tries.end())
    {
         std::cout << "Trie" << set_name << " not found." << std::endl;
        return;
    }

    auto strings = m_tries[set_name].allStrings(from, to);
    
    if (order == "DESC") {
        std::reverse(strings.begin(), strings.end());
    }
    
    for (const auto& it : strings) {
        std::cout << it << std::endl;
    }
}

void CommandProcessor::match(const std::string& set_name, const std::string& pattern, const std::string& order)
{
    std::cout << "Searching words in " << set_name << " with the next pattern: " 
        << pattern << " in " << order << " order\n";

    if (m_tries.find(set_name) == m_tries.end())
    {
        std::cout << "Trie" << set_name << " not found." << std::endl;
        return;
    }

    m_tries[set_name].match(pattern, order);
}