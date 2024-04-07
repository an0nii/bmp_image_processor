#pragma once

#include <vector>
#include <string_view>

const int MIN_PARAM_SIZE = 3;
const int OUTPUT_NAME_POS = 2;
const int INPUT_NAME_POS = 1;

struct Token {
    std::string_view name;
    std::vector<std::string_view> params;
    Token() : params({}) {
    }
    Token(const std::string_view& name, const std::vector<std::string_view>& params) : name(name), params(params) {
    }
};

class Parser {
protected:
    std::string_view input_name;
    std::string_view output_name;
    std::vector<Token*> tokens;

public:
    bool Parse(int argc, char* argv[]);

    std::string_view GetInputName() const;

    std::string_view GetOutputName() const;

    std::vector<Token*> GetTokens() const;
};
