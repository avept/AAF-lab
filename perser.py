class Token:
    def __init__(self, token_type, value):
        self.type = token_type
        self.value = value

class Interpreter:
    def __init__(self, text):
        self.text = text
        self.pos = 0
        self.current_token = None
        self.current_char = self.text[self.pos]

    def error(self):
        raise Exception(f"Error parsing input: token type {self.current_token.type}")
    
    def advance(self):
        self.pos += 1
        if self.pos > len(self.text)-1:
            self.current_char = None
        else:
            self.current_char = self.text[self.pos]

    def skip_whitespace(self):
        while self.current_char is not None and self.current_char.isspace():
            self.advance()

    def integer(self):
        res = ''
        while self.current_char is not None and self.current_char.isdigit():
            res += self.current_char
            self.advance()
        return int(res)
    
    def get_next_token(self):
        text = self.text

        while self.current_char is not None:
            
            if self.current_char.isspace():
                self.skip_whitespace()
                continue # we should come back

            elif self.current_char.isdigit():
                return Token("INTEGER", self.integer())
            
            elif self.current_char == "+":
                self.advance()
                return Token("OPERATION", "PLUS")
            
            elif self.current_char == '-':
                self.advance()
                return Token("OPERATION", "MINUS")
            elif self.current_char == '*':
                self.advance()
                return Token("OPERATION", "MULTIPLY")
            elif self.current_char == '/':
                self.advance()
                return Token("OPERATION", "DIVIDE")
            
            
            self.error()
            
        return Token("EOF", None)

    def eat(self, token_type):
        if self.current_token.type == token_type:
            self.current_token = self.get_next_token()
        else:
            self.error()

    def term(self):
        token = self.current_token
        self.eat("INTEGER")
        return token.value

    def expr(self):
        self.current_token = self.get_next_token()

        res = self.term()
        while self.current_token.value in ("MULTIPLY", "DIVIDE"):
            token = self.current_token
            if token.value == "MULTIPLY":
                self.eat("OPERATION")
                res *= self.term()
            elif token.value == "DIVIDE":
                self.eat("OPERATION")
                res //= self.term()
        
        return res

    

def main():
    while True:
        try:
            text = input('calc> ')
        except EOFError:
            break

        interpreter = Interpreter(text)
        result = interpreter.expr()
        print(result)

if __name__ == '__main__':
    main()