#include <iostream>
#include <cmath>
#include <cstring>
#include <stdio.h>
using namespace std;
//a0*x^w+(c*y-a)*y  4*2^3 + (12*3-4)*3 == 32 + 32 * 3 = 128
struct Node
{
    double data;
    Node* next;
};

Node* head = NULL;

void push_back(double);
double pop_back();
bool stack_is_empty();

int priority(char);
char* to_postfix(char*);
double* set_values(char* str);
double calculate_postfix(char*,double*);

int main()
{
    cout << "Enter expression: " << endl;
    char input[100];
    gets(input);

    char* output = to_postfix(input);
    cout << "Your expression: " << endl;
    puts(output);

    double* values = set_values(output);

    cout << "Result: " << calculate_postfix(output, values);

    delete [] output;
    delete [] values;
}

void push_back(double value)
{
    Node* element = new Node;
    element -> data = value;
    element -> next = head;
    head = element;
}

double pop_back()
{
    if ( !stack_is_empty() )
    {
        Node* current = head;
        double result = current -> data;
        head = head -> next;
        delete current;
        return result;
    }

}

bool stack_is_empty()
{
    return (head == NULL);
}

int priority(char symbol)
{
    switch(symbol)
    {
        case '(':   case ')':      return 0;
        case '+':   case '-':      return 1;
        case '*':   case '/':      return 2;
        case '^':                  return 3;
        default :                  return -1;
    }
}

char* to_postfix(char* input_str)
{
    char current_symbol;
    char* result = new char[strlen(input_str) + 1]{'\0'};
    int output_position = 0;
    for (int i = 0; i<strlen(input_str); i++)
    {
        current_symbol = input_str[i];
        if (current_symbol >= 'A' && current_symbol != '^')
        {
            result[output_position++] = current_symbol;
            continue;
        }
        if (stack_is_empty() || current_symbol == '(')
        {
            push_back(current_symbol);
            continue;
        }
        if (current_symbol == ')')
        {
            char symbol = '\0';
            while (symbol != '(')
            {
                symbol = pop_back();
                result[output_position++] = symbol;
            }
            result[--output_position] = '\0';
            continue;
        }
        int pr = priority(current_symbol);
        char symbol;
        while(!stack_is_empty())
        {
            char symbol = pop_back();
            if (priority(symbol) < pr)
            {
                push_back(symbol);
                break;
            }
            result[output_position++] = symbol;
        }
        push_back(current_symbol);
    }
    while(!stack_is_empty())
    {
        result[output_position++] = pop_back();
    }
    result[output_position] = '\0';
    return result;
}

double* set_values(char* str)
{
    double* values = new double[58];
    for (int i = 0; i<58; i++)
    {
        values[i] = INT_MAX;
    }
    for (int i = 0; i<strlen(str); i++)
    {
        if ( (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') )
        {
            if (values[int(str[i])-65] == INT_MAX)
            {
                cout << "Enter value of " << str[i] << " : ";
                cin >> values[int(str[i]) - 65];
            }
        }
    }
    return values;
}

double calculate_postfix(char* str, double* values)
{
    char current_symbol;
    for (int i = 0; i<strlen(str); i++)
    {
        current_symbol = str[i];
        if (current_symbol >= 'A' && current_symbol != '^')
        {
            push_back(values[int(str[i]) - 65]);
            continue;
        }
        double second_value = pop_back();
        double first_value = pop_back();
        switch(current_symbol)
        {
            case '+': push_back(first_value + second_value);
                      break;
            case '-': push_back(first_value - second_value);
                      break;
            case '*': push_back(first_value * second_value);
                      break;
            case '/': push_back(first_value / second_value);
                      break;
            case '^': push_back(pow(first_value, second_value));
                      break;
        }
    }
    return pop_back();
}
