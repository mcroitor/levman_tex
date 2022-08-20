#include <fstream>
#include <iostream>
#include <string>
#include <map>

typedef std::map<std::string, std::string> keymap;

void build_keys(keymap& keys, int argc, char** argv);

void info();

void OutPopeyeFormat(std::ostream& out, std::string& fen, int index);

int main(int argc, char** argv) {
    keymap keys;

    if (argc < 2) {
        std::cout << "no params transmited" << std::endl;
        info();
        return 1;
    }

    build_keys(keys, argc, argv);

    std::string infilename("input.txt");
    std::string outfilename("output.txt");

    if (keys.find("--i") != keys.end()) {
        infilename = keys["--i"];
    }

    if (keys.find("--o") != keys.end()) {
        outfilename = keys["--o"];
    }
    std::string line;

    std::ifstream fin(infilename.c_str());
    std::ofstream fout(outfilename.c_str(), std::ios::app);

    int count = 1, i;

    while (!fin.eof()) {
        std::getline(fin, line);
        if (line.find("setfen=") != line.npos) {
            line.erase(0, 7);
            line.erase(line.end() - 1);
            for (i = 0; i != line.size(); ++i) {
                if (line[i] == 'n') line[i] = 's';
                if (line[i] == 'N') line[i] = 'S';
            }
            std::cout << line << std::endl;
            if (keys.find("--mode") != keys.end() && keys["--mode"] == std::string("popeye")) {
                OutPopeyeFormat(fout, line, count);
                ++count;
            } else {
                fout << line << std::endl;
            }

        }
    }

    return 0;
}

void info() {
    std::cout << "using: \n"
            << "\t extractfen --i=<input.tex> [--o=<output.txt>] [--mode=popeye|fen]" << std::endl;
}

void build_keys(keymap& keys, int argc, char** argv) {
    int i, tmp;
    std::string key, value, aux;
    for (i = 0; i != argc; ++i) {
        aux = std::string(argv[i]);
        tmp = aux.find('=');
        if (tmp != aux.npos) {
            key = aux.substr(0, tmp);
            value = aux.substr(tmp+1, aux.size() - tmp - 1);
            keys[key] = value;
        }
    }
}

void OutPopeyeFormat(std::ostream& out, std::string& fen, int index) {
    out << "BeginProblem" << std::endl;
    out << "Author\t" << index << std::endl;
    out << "Option\tVariation" << std::endl;
    out << "Stipulation #2" << std::endl;
    out << "Forsyth\t" << fen << std::endl;
    out << "NextProblem" << std::endl;
}