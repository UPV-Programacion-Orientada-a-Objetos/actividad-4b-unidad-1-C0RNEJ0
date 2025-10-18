#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "Simulador2D.h"

int main() {
    using std::cout;
    using std::endl;

    Simulador2D<double> sim(8, 12);
    sim.setConstantes(0.25f, 1.0f, 0.1f);

    sim.agregarFuente(100.0);
    sim.agregarFuente(50.0);
    sim.agregarFuente(0.0);

    sim.aplicarFuenteEn(4, 6, 0);
    sim.aplicarFuenteEn(2, 3, 1);
    sim.aplicarFuenteEn(6, 9, 1);
    sim.setCelda(1, 10, 25.0);

    cout << "== Estado inicial ==" << endl;
    sim.imprimir();
    cout << endl;

    const int pasos = 5;
    for (int t = 1; t <= pasos; ++t) {
        sim.simularPaso();
        cout << "== Paso " << t << " ==" << endl;
        sim.imprimir();
        cout << endl;
    }

    cout << "== Redimensionando grid a 10x14 ==" << endl;
    sim.redimensionarGrid(10, 14);
    sim.imprimir();
    cout << endl;

    sim.aplicarFuenteEn(9, 13, 1);
    cout << "== Tras aplicar fuente templada en (9,13) ==" << endl;
    sim.imprimir();
    cout << endl;

    cout << "== Probando constructor de copia ==" << endl;
    Simulador2D<double> copia(sim);
    copia.aplicarFuenteEn(0, 0, 0);
    cout << "-- Original --" << endl;
    sim.imprimir();
    cout << "-- Copia --" << endl;
    copia.imprimir();
    cout << endl;

    cout << "== Probando operador de asignacion ==" << endl;
    Simulador2D<double> otra(3, 3);
    otra = sim;
    otra.aplicarFuenteEn(5, 5, 2);
    cout << "-- Original --" << endl;
    sim.imprimir();
    cout << "-- Asignada --" << endl;
    otra.imprimir();
    cout << endl;

    cout << "== Dos pasos mas de simulacion sobre el original ==" << endl;
    sim.simularPaso();
    sim.simularPaso();
    sim.imprimir();

    cout << "\nListo. Filas: " << sim.filas()
         << ", Columnas: " << sim.columnas()
         << ", #Fuentes: " << sim.numFuentes() << endl;

    return 0;
}
