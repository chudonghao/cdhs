//
// Created by chudonghao on 2022/1/28.
//

#include <fstream>
#include <iostream>

#include <cdhs/CompileError.h>
#include <cdhs/Lexer.h>
#include <cdhs/Parser.h>

using namespace cdhs;
using namespace std;

int main() {
  Lexer l;

  {
    string source = " abc abc_123 1234 .123 123.123 123.123.2 .123e123 .123e-123 'a' '\\b' \"sdf\"\"\"\"\\s\\\"\" && & []{}()\n"
                    " \"// sdfsfd\"\n"
                    " // sdfsdf\n"
                    " aaa\n"
                    " /*sdfsf*//*sdf*/sf*/";
    cout << source << endl;
    l.reset(&source);
    l.lex(1000);

    for (int i = 0; i < l.getTokenCount(); ++i) {
      auto &token = l.getToken(i);
      switch (token.kind) {
      case tok::unknown:
        cout << "unknown@" << token.source_location << " " << token.source << endl;
        break;
      case tok::comment:
        cout << "comment@" << token.source_location << " " << token.source << endl;
        break;
      case tok::identifier:
        cout << "identifier@" << token.source_location << " " << token.source << endl;
        break;
      case tok::char_constant:
        cout << "char_constant@" << token.source_location << " " << token.source << endl;
        break;
      case tok::numeric_constant:
        cout << "numeric_constant@" << token.source_location << " " << token.source << endl;
        break;
      case tok::string_literal:
        cout << "string_literal@" << token.source_location << " " << token.source << endl;
        break;
      case tok::l_square:
        cout << "l_square@" << token.source_location << " " << token.source << endl;
        break;
      case tok::r_square:
        cout << "r_square@" << token.source_location << " " << token.source << endl;
        break;
      case tok::l_paren:
        cout << "l_paren@" << token.source_location << " " << token.source << endl;
        break;
      case tok::r_paren:
        cout << "r_paren@" << token.source_location << " " << token.source << endl;
        break;
      case tok::l_brace:
        cout << "l_brace@" << token.source_location << " " << token.source << endl;
        break;
      case tok::r_brace:
        cout << "r_brace@" << token.source_location << " " << token.source << endl;
        break;
      case tok::period:
        cout << "period@" << token.source_location << " " << token.source << endl;
        break;
      case tok::amp:
        cout << "amp@" << token.source_location << " " << token.source << endl;
        break;
      case tok::ampamp:
        cout << "ampamp@" << token.source_location << " " << token.source << endl;
        break;
      case tok::star:
        cout << "star@" << token.source_location << " " << token.source << endl;
        break;
      case tok::plus:
        cout << "plus@" << token.source_location << " " << token.source << endl;
        break;
      case tok::plusplus:
        cout << "plusplus@" << token.source_location << " " << token.source << endl;
        break;
      case tok::minus:
        cout << "minus@" << token.source_location << " " << token.source << endl;
        break;
      case tok::arrow:
        cout << "arrow@" << token.source_location << " " << token.source << endl;
        break;
      case tok::minusminus:
        cout << "minusminus@" << token.source_location << " " << token.source << endl;
        break;
      case tok::tilde:
        cout << "tilde@" << token.source_location << " " << token.source << endl;
        break;
      case tok::exclaim:
        cout << "exclaim@" << token.source_location << " " << token.source << endl;
        break;
      case tok::exclaimequal:
        cout << "exclaimequal@" << token.source_location << " " << token.source << endl;
        break;
      case tok::slash:
        cout << "slash@" << token.source_location << " " << token.source << endl;
        break;
      case tok::percent:
        cout << "percent@" << token.source_location << " " << token.source << endl;
        break;
      case tok::less:
        cout << "less@" << token.source_location << " " << token.source << endl;
        break;
      case tok::lessequal:
        cout << "lessequal@" << token.source_location << " " << token.source << endl;
        break;
      case tok::greater:
        cout << "greater@" << token.source_location << " " << token.source << endl;
        break;
      case tok::greaterequal:
        cout << "greaterequal@" << token.source_location << " " << token.source << endl;
        break;
      case tok::caret:
        cout << "caret@" << token.source_location << " " << token.source << endl;
        break;
      case tok::pipe:
        cout << "pipe@" << token.source_location << " " << token.source << endl;
        break;
      case tok::pipepipe:
        cout << "pipepipe@" << token.source_location << " " << token.source << endl;
        break;
      case tok::question:
        cout << "question@" << token.source_location << " " << token.source << endl;
        break;
      case tok::colon:
        cout << "colon@" << token.source_location << " " << token.source << endl;
        break;
      case tok::semi:
        cout << "semi@" << token.source_location << " " << token.source << endl;
        break;
      case tok::equal:
        cout << "equal@" << token.source_location << " " << token.source << endl;
        break;
      case tok::equalequal:
        cout << "equalequal@" << token.source_location << " " << token.source << endl;
        break;
      case tok::comma:
        cout << "comma@" << token.source_location << " " << token.source << endl;
        break;
      case tok::blank:
        cout << "blank@" << token.source_location << " " << token.source << endl;
        break;
      case tok::eol:
        cout << "eol@" << token.source_location << endl;
        break;
      case tok::eof:
        cout << "eof@" << token.source_location << endl;
        break;
      case tok::tok_max:
      default:
        cout << "error " << token.source << endl;
        break;
      }
    }
  }

  {
    string source;
    ifstream ifs(TEST_DATA_DIR "/simple_test.cdhs");
    if (!ifs) {
      return -1;
    }
    ifs.seekg(0, ios_base::end);
    auto len = ifs.tellg();
    ifs.seekg(0, ios_base::beg);
    source.resize(len);
    ifs.read(source.data(), len);
    ifs.close();

    Lexer l;
    l.reset(&source);
    Parser p;
    try {
      auto S = p.parse(l);
    } catch (CompileError &e) {
      cerr << e.what() << endl;
    }
  }

  return 0;
}