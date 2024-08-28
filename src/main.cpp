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

std::vector<std::string> split(std::string str, std::string delimiter) {
    std::vector<std::string> parts;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        parts.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    parts.push_back(str);
    return parts;
}

bool esBiciesto(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

bool esDiaValido(int day, int month, int year) {
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2024) {
        std::cout << "Fecha inválida: " << day << "/" << month << "/" << year << " (fuera de rango)" << std::endl;
        return false;
    }
    if ((month == 2 && day > 29 && esBiciesto(year)) || (month == 2 && day > 28 && !esBiciesto(year))) {
        std::cout << "Fecha inválida: " << day << "/" << month << "/" << year << " (febrero no tiene " << day << " días)" << std::endl;
        return false;
    }
    if (day == 31 && (month == 4 || month == 6 || month == 9 || month == 11)) {
        std::cout << "Fecha inválida: " << day << "/" << month << "/" << year << " (el mes " << month << " no tiene 31 días)" << std::endl;
        return false;
    }
    return true;
}

void ajustarFecha(int& day, int& month, int& year, ListaSimple<Feriado>& feriados) {
    while ((esFeriado(day, month, year, feriados) || !esDiaValido(day, month, year))) {
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

void calcularAmortizacion(double principal, double annualInterestRate, int months, ListaSimple<Feriado>& feriados, int day = 1, int month = 1, int year = 2024) {
    double monthlyInterestRate = annualInterestRate / 12 / 100;
    double monthlyPayment = principal * monthlyInterestRate / (1 - pow(1 + monthlyInterestRate, -months));

    double balance = principal;
    int currentMonth = month;
    int mesOrdinal = 1;

    tabulate::Table table;
    std::string htmlString = "<html><head><style>table {border-collapse: collapse;} th, td {border: 1px solid black; padding: 8px; text-align: center;} th {background-color: #f2f2f2;}</style></head><body>";
    htmlString += "<h1>Calculadora de amortizaci&oacute;n</h1>";
    htmlString += "<h2>Pr&eacute;stamo: $" + formatFlotante(principal) + "</h2>";
    htmlString += "<h2>Tasa de inter&eacute;s anual: " + formatFlotante(annualInterestRate * 100) + "%</h2>";
    htmlString += "<h2>Cuota mensual: $" + formatFlotante(monthlyPayment) + "</h2>";
    htmlString += "<table><tr><th>Mes</th><th>Monto</th><th>Principal</th><th>Interes</th><th>Deuda</th><th>Fecha</th></tr>";
    table.add_row({"Mes", "Monto", "Principal", "Interes", "Deuda", "Fecha"});

    while (mesOrdinal <= months) {
        double interest = balance * monthlyInterestRate;
        double principalPayment = monthlyPayment - interest;
        balance -= principalPayment;

        ajustarFecha(day, currentMonth, year, feriados);
        table.add_row({std::to_string(mesOrdinal), "$" + formatFlotante(monthlyPayment), "$" + formatFlotante(principalPayment), formatFlotante(interest) + "%", "$" + formatFlotante(abs(balance)), std::to_string(day) + "/" + std::to_string(currentMonth) + "/" + std::to_string(year)});

        htmlString += "<tr><td>" + std::to_string(mesOrdinal) + "</td><td>$" + formatFlotante(monthlyPayment) + "</td><td>$" + formatFlotante(principalPayment) + "</td><td>" + formatFlotante(interest) + "%</td><td>$" + formatFlotante(abs(balance)) + "</td><td>" + std::to_string(day) + "/" + std::to_string(currentMonth) + "/" + std::to_string(year) + "</td></tr>";

        currentMonth++;
        mesOrdinal++;
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

    // Utilidades::clearConsole();
    std::cout << "Ingrese el monto del préstamo: $";
    double principal = ingresarFlotante();
    std::cout << "Ingrese la tasa de interés anual (%): ";
    double annualInterestRate = ingresarFlotante() / 100.f;
    std::cout << "Ingrese la cantidad de meses: ";
    int months = ingresarEntero();
    bool validDate = false;
    std::string date;
    int day, month, year;
    while (!validDate) {
        std::cout << "Ingrese la fecha de inicio (dd/mm/yyyy): ";
        std::getline(std::cin, date);
        if (date.length() != 10) {
            std::cout << "Fecha inválida" << std::endl;
            continue;
        }
        std::vector<std::string> dateParts = split(date, "/");
        if (dateParts.size() != 3) {
            std::cout << "Fecha inválida" << std::endl;
            continue;
        }
        day = std::stoi(dateParts[0]);
        month = std::stoi(dateParts[1]);
        year = std::stoi(dateParts[2]);
        if (!esDiaValido(day, month, year)) {
            std::cout << "Fecha inválida" << std::endl;
            continue;
        }
        validDate = true;
    }
    // Utilidades::clearConsole();

    std::cout << "\nMonto del préstamo: $" << formatFlotante(principal) << std::endl;
    std::cout << "Tasa de interés anual: " << formatFlotante(annualInterestRate * 100) << "%" << std::endl;
    std::cout << "Cantidad de meses: " << months << std::endl;
    std::cout << "\nAmortización:\n";

    ListaSimple<Feriado> feriados;
    feriados.Insertar(Feriado(1, 1, 2024));
    feriados.Insertar(Feriado(25, 12, 2024));
    feriados.Insertar(Feriado(13, 2, 2024));
    feriados.Insertar(Feriado(14, 2, 2024));
    feriados.Insertar(Feriado(14, 5, 2024));
    feriados.Insertar(Feriado(1, 5, 2024));
    feriados.Insertar(Feriado(24, 5, 2024));
    feriados.Insertar(Feriado(14, 5, 2024));
    feriados.Insertar(Feriado(9, 10, 2024));
    feriados.Insertar(Feriado(10, 8, 2024));
    feriados.Insertar(Feriado(22, 11, 2024));

    calcularAmortizacion(principal, annualInterestRate, months, feriados, day, month, year);

    return 0;
}
