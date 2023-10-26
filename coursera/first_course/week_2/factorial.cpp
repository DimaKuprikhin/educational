int Factorial(int number) {
    if(number <= 0) {
        return 1;
    }
    int result = 1;
    for(int i = 2; i <= number; ++i) {
        result *= i;
    }
    return result;
}

