#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <cmath>

struct Feriado {
    int day;
    int month;
    int year;
};

bool esFeriado(int day, int month, int year, const std::vector<Feriado>& feriados) {
    for (const auto& holiday : feriados) {
        if (holiday.day == day && holiday.month == month && holiday.year == year) {
            return true;
        }
    }
    return false;
}


void ajustarFecha(int& day, int& month, int& year, const std::vector<Feriado>& feriados) {
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

void calcularAmortizacion(double principal, double annualInterestRate, int months, const std::vector<Feriado>& feriados) {
    double monthlyInterestRate = annualInterestRate / 12 / 100;
    double monthlyPayment = principal * monthlyInterestRate / (1 - pow(1 + monthlyInterestRate, -months));

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Mes\tMonto\tPrincipal\tInteres\t\tDeuda\tFecha\n";

    double balance = principal;
    int currentMonth = 1;
    int day = 1;
    int year = 2024;

    while (currentMonth <= months) {
        double interest = balance * monthlyInterestRate;
        double principalPayment = monthlyPayment - interest;
        balance -= principalPayment;

        ajustarFecha(day, currentMonth, year, feriados);

        std::cout << currentMonth << "\t" << monthlyPayment << "\t" << principalPayment << "\t\t" << interest << "\t\t" << balance << "\t"
            << day << "/" << currentMonth << "/" << year << "\n";

        currentMonth++;
    }
}

int main() {
    double principal = 10000;
    double annualInterestRate = 5.0;
    int months = 24;

    std::vector<Feriado> feriados = {
        {1, 1, 2024},
        {25, 12, 2024},
        {12, 2, 2024},
        {13, 2, 2024},
        {14, 5, 2024},
        {1, 5, 2024},
        {24, 5, 2024},
        {14, 5, 2024},
        {9, 10, 2024},
        {10, 8, 2024},
        {22, 11, 2024},
    };

    calcularAmortizacion(principal, annualInterestRate, months, feriados);

    return 0;
}
