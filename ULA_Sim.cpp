/*
* Diogo Fasciani Viggiani Rios de Castro
* 2021/09/17
* v0.1.0
* 1 bit ALU in Arduino
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

using namespace std;

enum Value
{
    Zero,
    One
};

enum Instruction
{
    And,
    Or,
    Not,
    Sum
};

// structure that holds the instruction
// and values
struct signal
{
    Value A;
    Value B;
    Instruction OP;
};

// structure that holds the return
struct result
{
    Value Output;
    Value CarryOut;
};

// logic_and() function
result logic_and(Value A, Value B)
{
    result r;
    r.Output = static_cast<Value>(A && B);
    r.CarryOut = static_cast<Value>(A && B);
    return r;
} // logic_and() end

// logic_or() function
result logic_or(Value A, Value B)
{
    result r;
    r.Output = static_cast<Value>(A || B);
    r.CarryOut = static_cast<Value>(A && B);
    return r;
} // logic_or() end

// logic_not() function
result logic_not(Value A, Value B)
{
    result r;
    r.Output = (A ? Zero : One);
    r.CarryOut = static_cast<Value>(A && B);
    return r;
} // logic_not() end

// logic_sum() function
result logic_sum(Value A, Value B)
{
    result r;
    r.Output = static_cast<Value>(A ^ B);
    r.CarryOut = static_cast<Value>(A && B);
    return r;
} // logic_sum() end

// mux4x1() function
Instruction mux4x1(int select)
{
    switch (select)
    {
    case 0:
        return And;
        break;
    case 1:
        return Or;
        break;
    case 2:
        return Not;
        break;
    case 3:
        return Sum;
        break;
    default:
        return And;
    }
} // mux4x1() end

// is_valid() function
// checks if string is valid input
bool is_valid(String str)
{
    if (str.length() != 4)
        return false;
    else if (isdigit(str.charAt(0)) && isdigit(str.charAt(1)) && isdigit(str.charAt(2)))
        return true;
    else
        return false;
} // is_valid() end

// translate() function
// receives a string and assigns the
// values to the signal structure
signal translate(String str)
{
    signal sig;
    if (is_valid(str))
    {
        sig.A = ((str.charAt(0) - '0') ? One : Zero);
        sig.B = ((str.charAt(1) - '0') ? One : Zero);
        sig.OP = mux4x1(str.charAt(2) - '0');
    }
    else
    {
        sig.A = Zero;
        sig.B = Zero;
        sig.OP = And;
    }
    return sig;
} // translate() end

// execute() function
// receives a signal and returns a result
result execute(signal sig)
{
    result res;
    switch (sig.OP)
    {
    case And:
        res = logic_and(sig.A, sig.B);
        break;
    case Or:
        res = logic_or(sig.A, sig.B);
        break;
    case Not:
        res = logic_not(sig.A, sig.B);
        break;
    case Sum:
        res = logic_sum(sig.A, sig.B);
        break;
    default:
        res.Output = Zero;
        res.CarryOut = Zero;
    }
    return res;
}
// execute() end

// setup() function
void setup()
{
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(11, OUTPUT);
} // setup() end

// loop() function
void loop()
{
    // Variable declaration
    String input;
    signal sig;
    result out;

    // Obtains input
    if (Serial.available() > 0)
    {
        input = Serial.readString();
        Serial.println();
        Serial.print("Entrada: ");
        Serial.println(input);

        // Translates it to a signal
        sig = translate(input);
        Serial.print("Operacao: ");
        Serial.println(sig.OP);
    
        // Execute signal and receive result
        out = execute(sig);
    
        // Use signals and result to turn leds on and off
        Serial.print("A: ");
        Serial.println(sig.A);
        digitalWrite(13, (sig.A ? HIGH : LOW));
        Serial.print("B: ");
        Serial.println(sig.B);
        digitalWrite(12, (sig.B ? HIGH : LOW));
        Serial.print("Out: ");
        Serial.println(out.Output);
        digitalWrite(11, (out.Output ? HIGH : LOW));
        Serial.print("Cout: ");
        Serial.println(out.CarryOut);
        digitalWrite(10, (out.CarryOut ? HIGH : LOW));
    }
} // loop() end