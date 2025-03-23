#include <iostream>
#include <iomanip>
#include <fstream>  // For file output
#include <cstdlib>  // For NULL

using namespace std;

struct Capacitor {
    double *time;
    double *voltage;
    double *current;
    double C;
};

// Function to initialize capacitor structure
void initializeCapacitor(Capacitor &cap, double C, int num_timesteps) {
    cap.C = C;
    cap.time = new double[num_timesteps];
    cap.voltage = new double[num_timesteps];
    cap.current = new double[num_timesteps];

    for (int i = 0; i < num_timesteps; i++) {
        cap.time[i] = i * 1e-10;
        cap.voltage[i] = 0.0;
        cap.current[i] = 0.0;
    }
}

// Simulate capacitor charging with constant current source
void simulateConstantCurrent(Capacitor &cap, int num_timesteps, double I) {
    for (int t = 1; t < num_timesteps; t++) {
        cap.voltage[t] = cap.voltage[t - 1] + I * 1e-10 * (1 / cap.C);
        cap.current[t] = I;
    }
}

// Simulate capacitor charging with constant voltage source
void simulateConstantVoltage(Capacitor &cap, int num_timesteps, double R, double V0) {
    cap.current[0] = V0 / R;
    for (int t = 1; t < num_timesteps; t++) {
        cap.current[t] = cap.current[t - 1] - (cap.current[t - 1] / (R * cap.C)) * 1e-10;
        cap.voltage[t] = V0 - cap.current[t] * R;
    }
}

// Function to print results every 200 timesteps
void printResults(const Capacitor &cap, int num_timesteps, const char *label) {
    cout << label << " Results:\n";
    cout << "Time (s), Voltage (V), Current (A)\n";

    for (int t = 0; t < num_timesteps; t += 200) {
        cout << fixed << setprecision(8)
             << cap.time[t] << ", " << cap.voltage[t] << ", " << cap.current[t] << "\n";
    }
}

// Function to save results to a file
void saveResultsToFile(const Capacitor &cap, int num_timesteps, const char *filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    file << "Time (s),Voltage (V),Current (A)\n";
    for (int t = 0; t < num_timesteps; t += 200) {
        file << fixed << setprecision(8)
             << cap.time[t] << "," << cap.voltage[t] << "," << cap.current[t] << "\n";
    }
    file.close();
    cout << "Results saved to " << filename << endl;
}

// Free allocated memory
void freeMemory(Capacitor &cap) {
    delete[] cap.time;
    delete[] cap.voltage;
    delete[] cap.current;
}

int main() {
    const int num_timesteps = 50000;
    const double C = 100e-12;
    const double R = 1000.0;
    const double I = 1e-2;
    const double V0 = 10.0;

    // Initialize capacitors
    Capacitor constantCurrentCap, constantVoltageCap;
    initializeCapacitor(constantCurrentCap, C, num_timesteps);
    initializeCapacitor(constantVoltageCap, C, num_timesteps);

    // Simulations
    simulateConstantCurrent(constantCurrentCap, num_timesteps, I);
    simulateConstantVoltage(constantVoltageCap, num_timesteps, R, V0);

    // Print results
    printResults(constantCurrentCap, num_timesteps, "Constant Current Source");
    printResults(constantVoltageCap, num_timesteps, "Constant Voltage Source");

    // Save results to file
    saveResultsToFile(constantCurrentCap, num_timesteps, "constant_current_results.csv");
    saveResultsToFile(constantVoltageCap, num_timesteps, "constant_voltage_results.csv");

    // Free allocated memory
    freeMemory(constantCurrentCap);
    freeMemory(constantVoltageCap);

    return 0;
}

