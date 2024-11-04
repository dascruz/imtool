#include <iostream>
#include <string>   // Para std::string
#include <vector>   // Para std::vector
#include "../common/metadata.hpp"
#include "../imgsoa/imagesoa.hpp"

namespace {
    constexpr int ARG_COUNT = 5;
    constexpr int MIN_MAX_COLOR = 0;
    constexpr int MAX_MAX_COLOR = 65535;

    // Función para validar que el número de argumentos esté dentro del rango
    bool validarArgumentos(int argc, const std::string& operation, int expectedArgs) {
        if (argc != expectedArgs) {
            std::cerr << "Error: Invalid number of extra arguments for " << operation << ": " << argc - 3 << '\n';
            return false;
        }
        return true;
    }

    // Función para verificar que el valor de maxlevel esté en el rango permitido
    bool verificarValorMaxlevel(const std::string& valor) {
        try {
            const int nivel = std::stoi(valor);
            if (nivel < MIN_MAX_COLOR || nivel > MAX_MAX_COLOR) {
                std::cerr << "Error: Invalid maxlevel: " << valor << '\n';
                return false;
            }
        } catch (...) {
            std::cerr << "Error: Invalid maxlevel: " << valor << '\n';
            return false;
        }
        return true;
    }
} // namespace

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Error: Número incorrecto de argumentos. Se requieren al menos 3 argumentos." << '\n';
        return -1;
    }

    // Copiar los argumentos a un vector de strings para evitar aritmética de punteros
    std::vector<std::string> args(argv, argv + argc);

    const std::string& inputPath = args[1];
    const std::string& outputPath = args[2];
    const std::string& operation = args[3];

    // Validar operación
    if (operation != "info" && operation != "maxlevel" && operation != "resize" &&
        operation != "cutfreq" && operation != "compress") {
        std::cerr << "Error: Invalid option: " << operation << '\n';
        return -1;
    }

    if (operation == "info") {
        if (!validarArgumentos(argc, operation, 4)) return -1;
        mostrarMetadatos(inputPath);
    } else if (operation == "maxlevel") {
        if (!validarArgumentos(argc, operation, ARG_COUNT)) return -1;
        if (!verificarValorMaxlevel(args[4])) return -1;

        const int nuevoMaxColor = std::stoi(args[4]);
        ImageSOA image(inputPath);
        image.escalarIntensidad(nuevoMaxColor);
        image.guardarImagen(outputPath);
    } else if (operation == "resize") {
        if (!validarArgumentos(argc, operation, ARG_COUNT)) return -1;

        const int nuevoAncho = std::stoi(args[4]);
        const int nuevoAlto = std::stoi(args[5]);

        ImageSOA image(inputPath);
        image.redimensionar(nuevoAncho, nuevoAlto);
        image.guardarImagen(outputPath);
    } else if (operation == "cutfreq") {
        if (argc < ARG_COUNT) {
            std::cerr << "Error: Invalid number of arguments for cutfreq.\n";
            return -1;
        }

        const int n = std::stoi(args[4]);
        ImageSOA image(inputPath);
        image.eliminarColoresMenosFrecuentes(n);
        image.guardarImagen(outputPath);
    } else if (operation == "compress") {
        if (!validarArgumentos(argc, operation, 4)) return -1;

        ImageSOA image(inputPath);
        image.comprimir();
        image.guardarImagen(outputPath);
    }

    return 0;
}

