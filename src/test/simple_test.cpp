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
    string source = " abc\n"
                    " abc_123\n"
                    " 1234\n"
                    " .123\n"
                    " 123.123\n"
                    " 123.123.2\n"
                    " .123e123\n"
                    " .123e-123\n"
                    " 'a'\n"
                    " '\\b'\n"
                    " \"sdf\"\"\"\"\\s\\\"\" && & []{}(),-\n"
                    " &&\n"
                    " &\n"
                    " ++\n"
                    " +\n"
                    " extern externaa\n"
                    " if ifaa\n"
                    " for foraa\n"
                    " continue continueaa\n"
                    " break breakaa\n"
                    " \"// sdfsfd\"\n"
                    " // sdfsdf\n"
                    " aaa\n"
                    " /*sdfsf*//*sdf*/sf*/\n";
    cout << source << endl;
    l.reset(&source);
    l.lex(1000);

    for (int i = 0; i < l.getTokenCount(); ++i) {
      auto &token = l.getToken(i);
      std::cout << tok::name(token.kind) << "@" << token.source_location << " " << token.source << std::endl;
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