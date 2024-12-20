#include <iostream>
#include <stdexcept>
#include <numeric>

class Fraction
{
public:
    // Fraction(const std::pair<int, int> &f)
    //     : Fraction(f.first, f.second)
    // {
    // }

    Fraction(int the_numerator, int the_denominator)
        : numerator{the_numerator}, denominator{the_denominator}
    {
        if (denominator == 0)
        {
            throw std::invalid_argument{"Denominator must be non-zero."};
        }
        if (denominator < 0)
        {
            //If we were provided a negative denominator, multiply
            //the whole fraction by (-1)/(-1) to move the negative
            //sign to the numerator.
            denominator = -denominator;
            numerator = -numerator;
        }
    }

    double to_real() const
    {
        return numerator / (double)denominator;
    }

    int get_numerator() const
    {
        return numerator;
    }
    int get_denominator() const
    {
        return denominator;
    }

    Fraction lowest_terms() const
    {
        //Find the greatest common divisor of the numerator and denominator
        auto common_divisor = std::gcd(numerator, denominator);

        //Now return a new Fraction with both components divided by that
        //common divisor
        Fraction result{numerator / common_divisor, denominator / common_divisor};
        return result;
    }

    // When operators are written as member functions, one of the
    // operands is implicitly interpreted to be the "current object"
    // For a unary operator (like negation), the current object is
    // the only operand (so the operator takes no explicit arguments).
    //
    // For a binary operator (like multiplication), the current object
    // is the left operand (so the only explicit argument to the operator
    // is the right operand).

    Fraction operator-() const
    {
        //This is the unary minus (i.e. negation) operator
        Fraction result{-numerator, denominator};
        return result; //Not in lowest terms, for symmetry with the non-negated fraction
    }

    Fraction operator+(Fraction const &other) const
    {
        Fraction result{numerator * other.denominator + denominator * other.numerator,
                        denominator * other.denominator};
        return result.lowest_terms();
    }
    Fraction operator-(Fraction const &other) const
    {
        //This is the binary minus (i.e. subtraction) operator
        Fraction result{numerator * other.denominator - denominator * other.numerator,
                        denominator * other.denominator};
        return result.lowest_terms();
    }
    Fraction operator*(Fraction const &other) const
    {
        Fraction result{numerator * other.numerator,
                        denominator * other.denominator};
        return result.lowest_terms();
    }
    Fraction operator/(Fraction const &other) const
    {
        Fraction result{numerator * other.denominator,
                        denominator * other.denominator};
        return result.lowest_terms();
    }

    bool operator<(Fraction const &other) const
    {
        return numerator * other.denominator < denominator * other.numerator;
    }

    //Task: Define an assignment operator which allows std::pair<int, int>
    //      instances to be assigned to Fraction objects.
    Fraction &operator=(const std::pair<int, int> &p)
    {
        this->numerator = p.first;
        this->denominator = p.second;

        return *this;
        // return Fraction{numerator, denominator};
    }

private:
    Fraction() : numerator{0}, denominator{1}
    {
    }
    int numerator;
    int denominator;
};

std::ostream &operator<<(std::ostream &stream, Fraction const &fraction)
{
    stream << fraction.get_numerator() << "/" << fraction.get_denominator();
    return stream;
}

Fraction assign(Fraction &f, const std::pair<int, int> &p)
{
    f = p;

    return f;
}

int main()
{

    Fraction x{6, 10};
    Fraction y{1, 6};

    std::pair<int, int> P{1, 87};

    //Task 1: Make the following assignment statement work.
    x = P;

    //Task 2: Make the following assignment statement work.
    x = y = P;

    // int a{}, b{};
    // a = (b = 6);

    std::cout << "n: " << (x = P).get_denominator() << std::endl;
    // assign(x, P)

    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    std::cout << "x+y: " << x + y << std::endl;

    return 0;
}
