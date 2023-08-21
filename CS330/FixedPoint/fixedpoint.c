#include <stdint.h>
#include <stdio.h>

#define FIXED_POINT_FRACTIONAL_BITS 16
#define INTEGER_BITS 16

// Hàm chuyển số thực thành fixed-point
int64_t float_to_fixed(double num) {
    return (int64_t)(num * (1LL << FIXED_POINT_FRACTIONAL_BITS));
}
// Chuyển đổi số thập phân sang fixed-point
int64_t convertToFixedPoint(double number) {
    return (int64_t)(number * (1 << 16));
}
// Hàm chuyển fixed-point về số thực
double fixed_to_float(int64_t num) {
    return (double)num / (1LL << FIXED_POINT_FRACTIONAL_BITS);
}

// Hàm in biểu diễn nhị phân của số fixed-point
void print_binary_representation(int64_t num) {
    int total_bits = FIXED_POINT_FRACTIONAL_BITS + INTEGER_BITS;

    for (int i = total_bits - 1; i >= 0; i--) {
        // In ra dấu chấm thập phân tại vị trí chính xác
        if (i == INTEGER_BITS - 1) {
            putchar('.');
        }
        
        putchar((num & (1LL << i)) ? '1' : '0');
    }
}

int main() {
    double number;
    int64_t fixedPointRepresentation;
    int32_t integerPart;
    int32_t fractionalPart;

    printf("Please enter a decimal number: ");
    scanf("%lf", &number);

    int64_t fixed_point = float_to_fixed(number);
    fixedPointRepresentation = convertToFixedPoint(number);
    // Tách phần nguyên và phần thập phân
    integerPart = fixedPointRepresentation >> 16; // Dịch bit sang phải 16 vị để lấy phần nguyên
    fractionalPart = fixedPointRepresentation & 0xFFFF; // Sử dụng phép AND để lấy 16 bit thấp nhất cho phần thập phân
    
    printf("Number in hexadecimal: %X.%04X\n", integerPart, fractionalPart);
    printf("Number in fixed-point: %lld\n", fixed_point);

    printf("Binary representation: ");
    print_binary_representation(fixed_point);

    double recovered = fixed_to_float(fixed_point);
    printf("\nRecovered number from fixed-point: %f\n", recovered);

    return 0;
}
