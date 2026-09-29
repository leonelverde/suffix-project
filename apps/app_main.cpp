#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "suffix_tree.h"
#include "suffix_array.h"

static void printPositions(const std::string& label, const std::vector<int>& positions) {
    std::cout << label << ": ";
    if (positions.empty()) {
        std::cout << "sin coincidencias\n";
        return;
    }
    for (size_t i = 0; i < positions.size(); ++i)
        std::cout << positions[i] << (i + 1 < positions.size() ? ", " : "\n");
}

static void showMenu(const std::string& text) {
    std::cout << "\n=== Suffix Tree y Suffix Array ===\n"
              << "Texto actual: \"" << text << "\"\n"
              << "1. Ingresar nuevo texto\n"
              << "2. Mostrar suffix array con LCP\n"
              << "3. Mostrar suffix tree\n"
              << "4. Buscar un patron\n"
              << "5. Subcadena repetida mas larga (usa LCP)\n"
              << "0. Salir\n"
              << "Opcion: ";
}

int main() {
    std::string text = "mississippi";
    std::unique_ptr<SuffixTree> tree(new SuffixTree(text));
    std::unique_ptr<SuffixArray> array(new SuffixArray(text));

    std::string line;
    while (true) {
        showMenu(text);
        if (!std::getline(std::cin, line) || line.empty()) {
            if (std::cin.eof()) break;
            continue;
        }

        char option = line[0];
        if (option == '0') break;

        if (option == '1') {
            std::cout << "Nuevo texto (sin el caracter '$'): ";
            std::string newText;
            std::getline(std::cin, newText);
            if (newText.empty() || newText.find('$') != std::string::npos) {
                std::cout << "Texto invalido.\n";
                continue;
            }
            text = newText;
            tree.reset(new SuffixTree(text));
            array.reset(new SuffixArray(text));
            std::cout << "Estructuras reconstruidas.\n";

        } else if (option == '2') {
            array->print();

        } else if (option == '3') {
            tree->print();
            std::cout << "Nodos totales: " << tree->countNodes() << "\n";

        } else if (option == '4') {
            std::cout << "Patron: ";
            std::string pattern;
            std::getline(std::cin, pattern);
            if (pattern.empty()) {
                std::cout << "Patron vacio.\n";
                continue;
            }
            printPositions("Suffix tree ", tree->search(pattern));
            printPositions("Suffix array", array->search(pattern));
            SearchRange range = array->searchRange(pattern);
            if (!range.empty())
                std::cout << "Rango en SA: SA[" << range.left << ".." << range.right - 1 << "]\n";

        } else if (option == '5') {
            std::string repeated = array->longestRepeatedSubstring();
            if (repeated.empty()) std::cout << "No hay subcadenas repetidas.\n";
            else std::cout << "Subcadena repetida mas larga: \"" << repeated
                           << "\" (longitud " << repeated.size() << ")\n";

        } else {
            std::cout << "Opcion invalida.\n";
        }
    }
    return 0;
}