#include "Ball_Tree.h"
#include "NaiveKNN.h"
#include <string>
#include <chrono>

const int dim = 13;

std::vector<Point<dim>> readData(const std::string& nombreArchivo);
void writeClosestPoints(vector<Point<dim>> d);
void writeObjPoint(Point<dim> p);

int main() {
    Ball_Tree <dim> bt;
    /*Leer datos*/
    cout << "Leyendo datos...\n";
    std::vector <Point<dim>> d = readData("../textfiles/data.txt");

    /*Construir Ball * Tree */
    cout << "Construyendo Ball Tree...\n";
    auto start = std::chrono::steady_clock::now();
    bt.bulkData(d);
    auto end = std::chrono::steady_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    cout << "Tiempo de construccion: " << time << "ns\n";
    time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    cout << "Tiempo de construccion: " << time << "ms\n";

    /*Input de KNN*/
    cout << "Ingrese el nombre de la canción a buscar(sin comas):\n";
    std::string canción;
    getline(std::cin, canción);
    cout << "Ingrese el número de vecinos que quiere buscar de la canción \'" << canción << "\': \n";
    int K;
    std::cin >> K;
    cout << "Calculando los " << K << " vecinos más cercanos de " << canción << "...\n";

    /*Buscar canción dentro de los datos*/
    Point <dim> t = Point<dim>(false);
    for (auto i: d) {
        if (i.name == canción)
            t = Point<dim>(i);
    }

    /*Canción encontrada:*/
    if (!t.empty) {

        /*Datos de canción elegida*/
        cout << "Canción:" << t.name << " ";
        for (auto i: t.coord) cout << i << " ";
        cout << endl;

        /*Medir KNN*/
        start = std::chrono::steady_clock::now();
        vector <Point<dim>> r = bt.KNN(t, K);
        end = std::chrono::steady_clock::now();
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        cout << "Tiempo de KNN: " << time << "ns\n";
        time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        cout << "Tiempo de KNN: " << time << "ms\n";

        /*Imprimir resultados KNN*/
        cout << "Presione p para imprimir los resultados\n";
        char temp;
        std::cin >> temp;
        if (temp == 'p') {
            for (const auto& i: r) {
                cout << i.name << ": ";
                for (auto j: i.coord)
                    cout << j << " ";
                cout << endl;
            }
            cout << endl;
        }else cout<<"Comando incorrecto, cancelando impresión.\n";


        start = std::chrono::steady_clock::now();
        NaiveKNN <dim> prueba;
        prueba.calculate(K, t, d);
        end = std::chrono::steady_clock::now();
        time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        cout << "tiempo de KNN naive: " << time << "ms\n";
        prueba.printResults();
        prueba.printDistances(t, d);
    }

}

/*Leer datos del archivo.*/
std::vector<Point<dim>> readData(const std::string& nombreArchivo) {
    std::vector<Point<dim>> puntos;
    puntos.reserve(40000);
    std::ifstream archivo(nombreArchivo);
    std::string name;
    if (archivo.is_open()) {
        std::string linea;
        for (int i = 0; std::getline(archivo, linea); i++) {
            name.clear();
            if(i == 0) continue;
            std::stringstream ss(linea);
            std::string valorStr;
            std::vector<double> valores;
            valores.reserve(dim);
            for (int j = 0; std::getline(ss, valorStr, ','); j++) {
                if(j >= dim)
                {
                    name += valorStr;
                    continue;
                }
                double valor = std::stod(valorStr);
                valores.push_back(valor);
            }
            if (valores.size() == dim) {
                puntos.emplace_back(valores,name);
                //puntos.emplace_back(valores);
            } else {
                cout<<"Error, valores incompletos en línea\n";
            }
        }
        archivo.close();
    } else {
        // Manejar el caso de error al abrir el archivo
    }

    return puntos;
}

