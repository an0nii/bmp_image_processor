#include "parse.h"

#include <stdexcept>

std::string_view Parser::GetInputName() const {
    return input_name;
}

std::string_view Parser::GetOutputName() const {
    return output_name;
}

std::vector<Token*> Parser::GetTokens() const {
    return tokens;
}

bool Parser::Parse(int argc, char* argv[]) {
    if (argc < MIN_PARAM_SIZE) {
        throw std::invalid_argument("Invalid number of arguments");
    }
    input_name = argv[INPUT_NAME_POS];
    output_name = argv[OUTPUT_NAME_POS];
    for (int i = MIN_PARAM_SIZE; i < argc; ++i) {
        if (argv[i][0] == '-') {
            Token* token = new Token;
            token->name = argv[i];
            tokens.push_back(token);
        } else {
            if (tokens.empty()) {
                return false;
            }
            tokens.back()->params.push_back(argv[i]);
        }
    }
    return true;
}
