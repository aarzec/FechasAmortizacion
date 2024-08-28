/**
UNIVERSIDAD DE LAS FUERZAS ARMADAS - ESPE
Autor: Andrés Romero
Materia: Estructura de Datos - 2024
*/
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <locale>

#include "../lib/tabulate.hpp"
#include "../lib/StrLib.h"
#include "../model/Nodo.h"
#include "../model/ListasSimples.h"
#include "../utils/TermInput.h"
#include "../utils/Utilidades.h"

class Feriado {
    public:
        int day;
        int month;
        int year;
        Feriado(int _day, int _month, int _year) {
            day = _day;
            month = _month;
            year = _year;
        }
        Feriado() {}
};

bool esFeriado(int day, int month, int year, ListaSimple<Feriado>& feriados) {
    bool esFeriado = false;
    feriados.recorrer([&](Nodo<Feriado>* nodo) {
        if (esFeriado) {
            return;
        }
        if (nodo->getDato().day == day && nodo->getDato().month == month && nodo->getDato().year == year) {
            esFeriado = true;
        }
    });
    return esFeriado;
}

void ajustarFecha(int& day, int& month, int& year, ListaSimple<Feriado>& feriados) {
    while (esFeriado(day, month, year, feriados)) {
        day++;
        if (day > 31) {
            day = 1;
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }
}

std::string formatFlotante(double num) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << num;
    std::string ws = ss.str();
    std::size_t posPunto = ws.find(".");
    std::size_t posComa = ws.find(",");
    if (posPunto != std::wstring::npos) {
        ws.replace(posPunto, 1, ",");
    }
    if (posComa != std::wstring::npos) {
        ws.replace(posComa, 1, ".");
    }
    return ws;
}

std::string minWidthStr(std::string str, int width) {
    int rem = width - str.length();
    if (rem <= 0) {
        return str;
    }
    return str + std::string(rem, ' ');
}

void calcularAmortizacion(double principal, double annualInterestRate, int months, ListaSimple<Feriado>& feriados) {
    double monthlyInterestRate = annualInterestRate / 12 / 100;
    double monthlyPayment = principal * monthlyInterestRate / (1 - pow(1 + monthlyInterestRate, -months));

    double balance = principal;
    int currentMonth = 1;
    int day = 1;
    int year = 2024;

    tabulate::Table table;
    std::string htmlString = "<html><head><style>table {border-collapse: collapse;} th, td {border: 1px solid black; padding: 8px; text-align: center;} th {background-color: #f2f2f2;}</style></head><body>";
    htmlString += "<h1>Calculadora de amortizaci&oacute;n</h1>";
    htmlString += "<h2>Pr&eacute;stamo: $" + formatFlotante(principal) + "</h2>";
    htmlString += "<h2>Cuota mensual: $" + formatFlotante(monthlyPayment) + "</h2>";
    htmlString += "<table><tr><th>Mes</th><th>Monto</th><th>Principal</th><th>Interes</th><th>Deuda</th><th>Fecha</th></tr>";
    table.add_row({"Mes", "Monto", "Principal", "Interes", "Deuda", "Fecha"});

    while (currentMonth <= months) {
        double interest = balance * monthlyInterestRate;
        double principalPayment = monthlyPayment - interest;
        balance -= principalPayment;

        ajustarFecha(day, currentMonth, year, feriados);
        table.add_row({std::to_string(currentMonth), "$" + formatFlotante(monthlyPayment), "$" + formatFlotante(principalPayment), formatFlotante(interest) + "%", "$" + formatFlotante(abs(balance)), std::to_string(day) + "/" + std::to_string(currentMonth) + "/" + std::to_string(year)});
        currentMonth++;

        htmlString += "<tr><td>" + std::to_string(currentMonth) + "</td><td>$" + formatFlotante(monthlyPayment) + "</td><td>$" + formatFlotante(principalPayment) + "</td><td>" + formatFlotante(interest) + "%</td><td>$" + formatFlotante(abs(balance)) + "</td><td>" + std::to_string(day) + "/" + std::to_string(currentMonth) + "/" + std::to_string(year) + "</td></tr>";
    }

    table.row(0).format()
        .font_background_color(tabulate::Color::white)
        .font_style({tabulate::FontStyle::bold})
        .font_align(tabulate::FontAlign::center);

    table.column(0).format()
        .font_color(tabulate::Color::grey);
    table.column(1).format()
        .font_color(tabulate::Color::green);
    table.column(2).format()
        .font_color(tabulate::Color::yellow);
    table.column(3).format()
        .font_color(tabulate::Color::yellow);
    table.column(4).format()
        .font_color(tabulate::Color::red);
    table.column(5).format()
        .font_color(tabulate::Color::blue);

    htmlString += "</table></body></html>";
    std::cout << table << std::endl;

    std::ofstream file("amortizacion.html");
    file << htmlString;
    file.close();
    // get absolute path
    std::string htmlAbsPath = std::filesystem::absolute("amortizacion.html");
    std::string pdfAbsPath = std::filesystem::absolute("amortizacion.pdf");
    system(("wkhtmltopdf " + htmlAbsPath + " " + pdfAbsPath).c_str());
    system(("xdg-open " + pdfAbsPath).c_str());
}

int main() {
    std::setlocale(LC_ALL, "");
    std::locale::global(std::locale(""));

    Utilidades::clearConsole();
    std::cout << "Ingrese el monto del préstamo: $";
    double principal = ingresarFlotante();
    std::cout << "Ingrese la tasa de interés anual (%): ";
    double annualInterestRate = ingresarFlotante() / 100.f;
    std::cout << "Ingrese la cantidad de meses: ";
    int months = ingresarEntero();
    Utilidades::clearConsole();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nMonto del préstamo: $" << principal << std::endl;
    std::cout << "Tasa de interés anual: " << annualInterestRate * 100 << "%" << std::endl;
    std::cout << "Cantidad de meses: " << months << std::endl;
    std::cout << "\nAmortización:\n";

    ListaSimple<Feriado> feriados;
    feriados.Insertar(Feriado(1, 1, 2024));
    feriados.Insertar(Feriado(25, 12, 2024));
    feriados.Insertar(Feriado(12, 2, 2024));
    feriados.Insertar(Feriado(13, 2, 2024));
    feriados.Insertar(Feriado(14, 5, 2024));
    feriados.Insertar(Feriado(1, 5, 2024));
    feriados.Insertar(Feriado(24, 5, 2024));
    feriados.Insertar(Feriado(14, 5, 2024));
    feriados.Insertar(Feriado(9, 10, 2024));
    feriados.Insertar(Feriado(10, 8, 2024));
    feriados.Insertar(Feriado(22, 11, 2024));

    calcularAmortizacion(principal, annualInterestRate, months, feriados);

    return 0;
}
