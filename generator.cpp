//
// Created by minirop on 04/11/17.
//


#include <fstream>
#include <vector>
#include <string>
#include <boost/algorithm/string/split.hpp>
#include <iostream>

using namespace std::string_literals;

static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

void defineAst(std::ofstream & file, std::string_view baseName, std::vector<std::string_view> types);
void defineType(std::ofstream & file, std::string_view baseName, std::string_view className, std::string_view fieldsList);
void defineVisitor(std::ofstream & file, std::string_view baseName, std::vector<std::string> types);

int main(int argc, char** argv)
{
    std::ofstream file("ast.h"s);

    file<< "#ifndef LOXPLUS_AST_H\n"
        << "#define LOXPLUS_AST_H\n"
        << "\n"
        << "#include \"Token.h\"\n"
        << "#include \"Object.h\"\n"
        << "#include <vector>\n"
        << "\n";

    defineAst(file, "Expr", {
        "Assign   : Token name, Expr* value",
        "Binary   : Expr* left, Token op, Expr* right",
        "Call     : Expr* callee, Token paren, std::vector<Expr*> arguments",
        "Get      : Expr* object, Token name",
        "Grouping : Expr* expression",
        "Literal  : Object value",
        "Logical  : Expr* left, Token op, Expr* right",
        "Set      : Expr* object, Token name, Expr* value",
        "This     : Token keyword",
        "Unary    : Token op, Expr* right",
        "Variable : Token name"
    });

    defineAst(file, "Stmt", {
        "Block      : std::vector<Stmt*> statements",
        "Class      : Token name, std::vector<FunctionStmt*> methods",
        "Expression : Expr* expression",
        "Function   : Token name, std::vector<Token> parameters, std::vector<Stmt*> body",
        "If         : Expr* condition, Stmt* thenBranch, Stmt* elseBranch",
        "Print      : Expr* expression",
        "Return     : Token keyword, Expr* value",
        "Var        : Token name, Expr* initializer",
        "While      : Expr* condition, Stmt* body"
    });

    file<< "#endif //LOXPLUS_AST_H\n";
    file.close();
    return 0;
}

void defineAst(std::ofstream & file, std::string_view baseName, std::vector<std::string_view> types)
{
    std::vector<std::pair<std::string, std::string>> typeFields;
    std::vector<std::string> typesList;
    for (auto & type : types)
    {
        auto it = type.find_first_of(':');
        auto typeName = std::string { type.substr(0, it) };
        auto fieldsNames = std::string { type.substr(it+1) };

        trim(typeName);
        trim(fieldsNames);

        typeFields.emplace_back(typeName, fieldsNames);
        typesList.push_back(typeName);
        file<< "class " << typeName << baseName << ";\n";
    }
    file<< '\n';

    defineVisitor(file, baseName, typesList);

    file<< "struct " << baseName << "\n"
        << "{\n"
        << "\tvirtual void accept(Visitor" << baseName << " & visitor) = 0;\n"
        << "};\n"
        << "\n";

    for (auto & type : typeFields)
    {
        defineType(file, baseName, type.first, type.second);
    }
}

void defineVisitor(std::ofstream & file, std::string_view baseName, std::vector<std::string> types)
{
    file<< "struct Visitor" << baseName << "\n"
        << "{\n"
        << "\tvirtual ~Visitor" << baseName << "() = default;\n\n";

    for (auto & type : types)
    {
        std::string varName { baseName };
        varName[0] = static_cast<char>(std::tolower(varName[0]));
        file<< "\tvirtual void visit" << type << baseName << "(" << type << baseName << " & " << varName << ") = 0;\n";
    }
    file<< "};\n\n";
}

void defineType(std::ofstream & file, std::string_view baseName, std::string_view className, std::string_view fieldsList)
{
    file<< "struct " << className << baseName << " : " << baseName << "\n"
        << "{\n";

    std::vector<std::string> fields;
    boost::split(fields, fieldsList, [](char c){ return c == ','; });

    file<< "\t" << (fields.size() == 1 ? "explicit " : "") << className << baseName << "(" << fieldsList << ")\n"
        << "\t\t: ";
    int i = 0;
    for (auto & field : fields)
    {
        trim(field);
        std::vector<std::string> data;
        boost::split(data, field, [](char c){ return c == ' '; });

        file<< data[1] << " { " << data[1] << " }";
        i++;
        if (i < fields.size()) file<< ", ";
        else file<< "\n";
    }
    file<< "\t{\n\t}\n\n";

    for (auto & field : fields)
    {
        trim(field);
        file<< "\t" << field << ";\n";
    }

    file<< "\n"
        << "\tvoid accept(Visitor" << baseName << " & visitor)\n"
        << "\t{\n"
        << "\t\tvisitor.visit" << className << baseName << "(*this);\n"
        << "\t}\n";

    file<< "};\n"
        << "\n";
}
