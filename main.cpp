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
    String,
    Print,
    End,
    Unknown
};

struct Token{
    TokenType type;
    string text;
};

vector<Token> tokenize(const std::string& source){
    vector<Token> tokens;
    size_t i = 0;
    while (i < source.length()){
        char current = source[i];
        if (isspace(current)){i++;continue;}
        if (current == '+'){
            tokens.push_back({TokenType::Plus, "+"});
            i++;continue;
        }
        if (current == '*'){
            tokens.push_back({TokenType::Multiply, "*"});
            i++;continue;
        }
        if (current == '^'){
            tokens.push_back({TokenType::Power,"^"});
            i++;continue;
        }
        if (current == '='){
            tokens.push_back({TokenType::Print, "="});
            i++;continue;
        }
        if (isdigit(current)){
            string numstr = "";
            while (i<source.length() && isdigit(source[i])){
                numstr+=source[i];
                i++;
            }
            tokens.push_back({TokenType::Number, numstr});
            continue;
        }
        if (current == '\"'){
            string strstr = "";
            i++;
            while (i<source.length() && source[i]!='\"'){
                if (source[i]=='\\' && source[i+1]=='n'){strstr+='\n';i++;}else{strstr+=source[i];}
                i++;
            }
            i++;
            tokens.push_back({TokenType::String, strstr});
            continue;
        }
        tokens.push_back({TokenType::Unknown, string(1,current)});
        i++;
    }
    tokens.push_back({TokenType::End, ""});
    return tokens;
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
        for (int i = 0; i < tokens.size(); i++){
            switch(tokens[i].type){
                case TokenType::Plus:
                    if (!(i < 1)){
                        if (tokens[i-1].type == TokenType::Number && tokens[i+1].type == TokenType::Number){
                            tokens[i+1].text=to_string(stoi(tokens[i-1].text)+stoi(tokens[i+1].text));
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                case TokenType::Multiply:
                    if (!(i < 1)){
                        if (tokens[i-1].type == TokenType::Number && tokens[i+1].type == TokenType::Number){
                            cout << stoi(tokens[i-1].text)*stoi(tokens[i+1].text) <<'\n';
                            tokens[i+1].text=to_string(stoi(tokens[i-1].text)*stoi(tokens[i+1].text));
                        } else if (tokens[i-1].type == TokenType::String && tokens[i+1].type == TokenType::Number){
                            string res = "";
                            for (int k = 0; k < stoi(tokens[i+1].text); k++){
                                res+=tokens[i-1].text;
                            }
                            tokens[i+1].type = TokenType::String;
                            tokens[i+1].text = res;
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                case TokenType::Power:
                    if (!(i < 1)){
                        if (tokens[i-1].type == TokenType::Number && tokens[i+1].type == TokenType::Number){
                            int val = 1;
                            for(int k = 0; k<stoi(tokens[i+1].text); k++){
                                val = val*stoi(tokens[i-1].text);
                            }
                            tokens[i+1].text=to_string(val);
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                case TokenType::Print:
                    if (!(i < 1)){
                        if (tokens[i-1].type == TokenType::String || tokens[i-1].type == TokenType::Number){
                            cout << tokens[i-1].text;
                        } else {
                            cout << "ERROR: Value not inputted" <<'\n';
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
    catch (const exception& e) {cout<<"Error: "<<e.what()<<'\n';}
    return 0;
}