#include "Parser/parse.h"
#include "Image/image.h"
#include "Filters/make_filter.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <cassert>

int main(int argc, char* argv[]) {
    Parser parser;
    if (!parser.Parse(argc, argv)) {
        throw std::invalid_argument("Invalid arguments");
    }
    Image img;
    std::string input_name = {parser.GetInputName().begin(), parser.GetInputName().end()};
    assert(img.Read(input_name.c_str()));
    assert(!img.IsEmpty());
    FilterFactory factory;
    for (auto token : parser.GetTokens()) {
        std::shared_ptr<Filter> filter(factory.CreateFilter(*token));
        assert(filter);
        filter->Apply(img);
        delete token;
    }
    std::string output_name = {parser.GetOutputName().begin(), parser.GetOutputName().end()};
    assert(img.Write(output_name.c_str()));
}
