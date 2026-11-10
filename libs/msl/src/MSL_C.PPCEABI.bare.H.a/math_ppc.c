double tan(double x);
double sin(double x);
double cos(double x);

__declspec(weak) float tanf(float x) {
    return tan(x);
}

__declspec(weak) float sinf(float x) {
    return sin(x);
}

__declspec(weak) float cosf(float x) {
    return cos(x);
}
