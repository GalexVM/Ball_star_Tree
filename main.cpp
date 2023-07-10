#include "Ball_Tree.h"
#include "NaiveKNN.h"
#include <string>
#include <chrono>


const int dim = 13;

std::vector<Point<dim>> readData(const std::string& nombreArchivo);
void writeClosestPoints(vector<Point<dim>> d);
void writeObjPoint(Point<dim> p);



int main() {
    Ball_Tree<dim> bt;
    cout<<"Leyendo datos...\n";
    std::vector<Point<dim>> d = readData("../textfiles/data.txt");
    cout<<"Construyendo Ball Tree...\n";
    auto start = std::chrono::steady_clock::now();
    bt.bulkData(d);
    auto end = std::chrono::steady_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    cout<<"Tiempo de construccion: "<<time<<"ns\n";

    cout<<"Ingrese el nombre de la canción a buscar(sin comas):\n";
    std::string canción;
    getline(std::cin,canción);
    cout<<"Ingrese el número de vecinos que quiere buscar de la canción \'"<<canción<<"\': \n";
    int K;
    std::cin>>K;
    cout<<"Calculando los "<<K<<" vecinos más cercanos de "<<canción<<"...\n";

    Point<dim>* t = bt.findTarget(canción);
    if(t){
        start = std::chrono::steady_clock::now();
        vector<Point<dim>> r = bt.KNN(*t,K);
        end = std::chrono::steady_clock::now();
        time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        cout<<"Tiempo de KNN: "<<time<<"ms\n";
        cout<<"Presione p para imprimir los resultados\n";
        char temp;
        std::cin>>temp;
        if(temp == 'p')
        {
            for(auto i : r)
            {
                cout << i.name <<": ";
                for(auto j : i.coord)
                    cout << j << " ";
                cout << endl;
            }
            cout<<endl;
        }
        start = std::chrono::steady_clock::now();
        NaiveKNN<dim> prueba;
        prueba.calculate(K,*t, d);
        end = std::chrono::steady_clock::now();
        time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        cout<<"tiempo de KNN naive: "<<time<<"ms\n";
        //prueba.printResults();
    }





}

void writeObjPoint(Point<dim> p)
{
    std::ofstream outputfile("../textfiles/objPoint.txt");
    if(!outputfile.is_open()){
        cout<<"Error al abrir el archivo.\n";
        return;
    }
    std::ostringstream  oss;
    for (int i = 0; i < dim; ++i) {
        oss << p.coord[i] << " ";
    }
    outputfile<<oss.str();
    outputfile.close();
}

void writeClosestPoints(vector<Point<dim>> d)
{
    std::ofstream outputfile("../textfiles/closePoints.txt");
    if(!outputfile.is_open()){
        cout<<"Error al abrir el archivo.\n";
        return;
    }
    std::ostringstream  oss;
    for(auto a : d)
    {
        for (int i = 0; i < dim; ++i) {
            oss << a.coord[i] << " ";
        }
        oss << "\n";
    }
    outputfile<<oss.str();
    outputfile.close();
}


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

