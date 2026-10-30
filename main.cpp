#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

enum class TokenType {
    Number,
    Plus,
    Multiply,
    Power,
    Minus,
    DivideD,
    DivideI,
    String,
    Print,
    Group,
    End,
    Unknown
};

struct Token{
    TokenType type;
    string value;
};

vector<Token> tokenize(const std::string& source){
    vector<Token> tokens;
    size_t i = 0;
    while (i < source.length()){
        char current = source[i];
        if (isspace(current)){
            i++;
            continue;
        }
        switch (current) {
            case '+':
                tokens.push_back({TokenType::Plus, "+"});
                i++;
                break;
            case '*':
                tokens.push_back({TokenType::Multiply, "*"});
                i++;
                break;
            case '^':
                tokens.push_back({TokenType::Power, "^"});
                i++;
                break;
            case '-':
                tokens.push_back({TokenType::Minus,"-"});
                i++;
                break;
            case 'p':
                tokens.push_back({TokenType::Print, "="});
                i++;
                break;
            case '/':
                if (i < source.length()-1){
                    if (source[i+1] == '/'){
                        tokens.push_back({TokenType::DivideI,"//"});
                        i++;
                    } else {
                        tokens.push_back({TokenType::DivideD,"/"});
                    }
                } else {
                    tokens.push_back({TokenType::DivideD,"/"});
                }
                i++;
                break;
            case '(': {
                string strstr = "";
                i++;
                while (i < source.length() && source[i] != ')'){
                    strstr += source[i];
                    i++;
                }
                i++;
                tokens.push_back({TokenType::Group, strstr});
                break;
            }
            case '\"': {
                string strstr = "";
                i++;
                while (i < source.length() && source[i] != '\"'){
                    if (source[i] == '\\' && source[i+1] == 'n'){
                        strstr += '\n';
                        i++;
                    } else {
                        strstr += source[i];
                    }
                    i++;
                }
                i++;
                tokens.push_back({TokenType::String, strstr});
                break;
            }
            case '\'': {
                string strstr = "";
                i++;
                while (i < source.length() && source[i] != '\''){
                    if (source[i] == '\\' && source[i+1] == 'n'){
                        strstr += '\n';
                        i++;
                    } else {
                        strstr += source[i];
                    }
                    i++;
                }
                i++;
                tokens.push_back({TokenType::String, strstr});
                break;
            }
            default:
                if (isdigit(current)){
                    string numstr = "";
                    while (i < source.length() && (isdigit(source[i]) || source[i] == '.')){
                        numstr += source[i];
                        i++;
                    }
                    tokens.push_back({TokenType::Number, numstr});
                } else {
                    tokens.push_back({TokenType::Unknown, string(1, current)});
                    i++;
                }
                break;
        }
    }
    tokens.push_back({TokenType::End, ""});
    return tokens;
}

Token compilepart(vector<Token> tokens){
    for (int i = 0; i < tokens.size(); i++){
            switch(tokens[i].type){
                case TokenType::Plus:
                    if (!(i < 1)){
                        bool leftNum = (tokens[i-1].type == TokenType::Number || tokens[i-1].type == TokenType::Group);
                        bool rightNum = (tokens[i+1].type == TokenType::Number || tokens[i+1].type == TokenType::Group);
                        if (leftNum && rightNum){
                            int lval = (tokens[i-1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i-1].value)).value) : stoi(tokens[i-1].value);
                            int rval = (tokens[i+1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i+1].value)).value) : stoi(tokens[i+1].value);
                            tokens[i+1].value = to_string(lval + rval);
                            tokens[i+1].type = TokenType::Number;
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                case TokenType::Multiply:
                    if (!(i < 1)){
                        bool leftNum = (tokens[i-1].type == TokenType::Number || tokens[i-1].type == TokenType::Group);
                        bool rightNum = (tokens[i+1].type == TokenType::Number || tokens[i+1].type == TokenType::Group);
                        if (leftNum && rightNum){
                            int lval = (tokens[i-1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i-1].value)).value) : stoi(tokens[i-1].value);
                            int rval = (tokens[i+1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i+1].value)).value) : stoi(tokens[i+1].value);
                            tokens[i+1].value = to_string(lval * rval);
                            tokens[i+1].type = TokenType::Number;
                        } else if (tokens[i-1].type == TokenType::String && rightNum){
                            int rval = (tokens[i+1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i+1].value)).value) : stoi(tokens[i+1].value);
                            string res = "";
                            for (int k = 0; k < rval; k++){
                                res += tokens[i-1].value;
                            }
                            tokens[i+1].type = TokenType::String;
                            tokens[i+1].value = res;
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                case TokenType::Power:
                    if (!(i < 1)){
                        bool leftNum = (tokens[i-1].type == TokenType::Number || tokens[i-1].type == TokenType::Group);
                        bool rightNum = (tokens[i+1].type == TokenType::Number || tokens[i+1].type == TokenType::Group);
                        if (leftNum && rightNum){
                            int lval = (tokens[i-1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i-1].value)).value) : stoi(tokens[i-1].value);
                            int rval = (tokens[i+1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i+1].value)).value) : stoi(tokens[i+1].value);
                            int val = 1;
                            for(int k = 0; k < rval; k++){
                                val = val * lval;
                            }
                            tokens[i+1].value = to_string(val);
                            tokens[i+1].type = TokenType::Number;
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                case TokenType::Minus:
                    if (!(i < 1)){
                        bool leftNum = (tokens[i-1].type == TokenType::Number || tokens[i-1].type == TokenType::Group);
                        bool rightNum = (tokens[i+1].type == TokenType::Number || tokens[i+1].type == TokenType::Group);
                        if (leftNum && rightNum){
                            int lval = (tokens[i-1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i-1].value)).value) : stoi(tokens[i-1].value);
                            int rval = (tokens[i+1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i+1].value)).value) : stoi(tokens[i+1].value);
                            tokens[i+1].value = to_string(lval - rval);
                            tokens[i+1].type = TokenType::Number;
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                case TokenType::DivideD:
                    if (!(i < 1)){
                        bool leftNum = (tokens[i-1].type == TokenType::Number || tokens[i-1].type == TokenType::Group);
                        bool rightNum = (tokens[i+1].type == TokenType::Number || tokens[i+1].type == TokenType::Group);
                        if (leftNum && rightNum){
                            float lval = (tokens[i-1].type == TokenType::Group) ? stof(compilepart(tokenize(tokens[i-1].value)).value) : stof(tokens[i-1].value);
                            float rval = (tokens[i+1].type == TokenType::Group) ? stof(compilepart(tokenize(tokens[i+1].value)).value) : stof(tokens[i+1].value);
                            tokens[i+1].value = to_string(float(lval/rval));
                            tokens[i+1].type = TokenType::Number;
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                case TokenType::DivideI:
                    if (!(i < 1)){
                        bool leftNum = (tokens[i-1].type == TokenType::Number || tokens[i-1].type == TokenType::Group);
                        bool rightNum = (tokens[i+1].type == TokenType::Number || tokens[i+1].type == TokenType::Group);
                        if (leftNum && rightNum){
                            int lval = (tokens[i-1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i-1].value)).value) : stoi(tokens[i-1].value);
                            int rval = (tokens[i+1].type == TokenType::Group) ? stoi(compilepart(tokenize(tokens[i+1].value)).value) : stoi(tokens[i+1].value);
                            tokens[i+1].value = to_string(int(lval/rval));
                            tokens[i+1].type = TokenType::Number;
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                case TokenType::Print:
                    if (!(i < 1)){
                        if (tokens[i-1].type == TokenType::String || tokens[i-1].type == TokenType::Number){
                            cout << tokens[i-1].value;
                        } else if (tokens[i-1].type == TokenType::Group){
                            Token res = compilepart(tokenize(tokens[i-1].value));
                            cout << res.value;
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    return tokens[tokens.size() - 2];
}

string readFile(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + filepath);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]){
    try{
        string code = readFile(argv[1]);
        vector<Token> tokens = tokenize(code);
        compilepart(tokens);
    }
    catch (const exception& e) {cout<<"Error: "<<e.what()<<'\n';}
    return 0;
}