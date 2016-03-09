
/*
 Evaluation Of postfix Expression in C++
 Only '+'  ,  '-'  , '*' and '/'  operators are expected.
 */
#include<iostream>
#include<stack>
#include<string>
#include<unistd.h>
#include<stdlib.h>

using namespace std;

// Function to evaluate Postfix expression and return output
int evaluatePostfix(string expression);

// Function to perform an operation and return output.
int performOperation(char operation, int operand1, int operand2);

// Function to verify whether a character is operator symbol or not.
bool isOperator(char C);

// Function to verify whether a character is numeric digit.
bool isNumericDigit(char C);

int main (int argc, char* argv[]){
    int c;
    string expression;
    while((c = getopt(argc, argv, "e:")) != -1)
    {
        switch(c)
        {
            case 'e':
                expression = optarg;
                break;
            case '?':
                cerr<<"INVALID OPTION"<<argv[optind-1]<<endl;
                break;
        }
    }
    int finalResult = evaluatePostfix(expression);
    cout<<finalResult<<endl;
}

// Function to evaluate Postfix expression and return output
int evaluatePostfix(string expression)
{
    stack<int> S;
    for(int i = 0;i< expression.length();i++) {
        if(expression[i] == ',') continue;
        else if(isOperator(expression[i])){
            if(i< expression.length() -1 && isNumericDigit(expression[i+1])){
                int temp = (expression[i+1] * -1) + 48;
                S.push(temp);
                i = i+1;
                continue;
            }
            int operand2 = S.top();
            S.pop();
            if(S.empty()){
                exit(-1);
            }
            int operand1 = S.top();
            S.pop();
            int result = performOperation(expression[i], operand1, operand2);
            S.push(result);
        }
        else if(isNumericDigit(expression[i])){
            int operand = 0;
            while(i < expression.length() && isNumericDigit(expression[i])) {
                operand = (operand*10) + (expression[i] - '0');
                i++;
            }
            i--;
            S.push(operand);
        }
    }
    long size = S.size();
    if(size == 1){
        return S.top();
    }
    else{
        exit(-1);
    }
    
}

// Function to verify whether a character is numeric digit.
bool isNumericDigit(char num)
{
    if(num >= '0' && num <= '9') return true;
    return false;
}

// Function to verify whether a character is operator symbol or not.
bool isOperator(char symbol)
{
    if(symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/'){
        return true;
    }else{
        return false;
    }
}

// Function to perform an operation and return output.
int performOperation(char operation, int operand1, int operand2)
{
    if(operation == '+'){
        return operand1 + operand2;
    }
    else if(operation == '-'){
        return operand1 - operand2;
    }
    else if(operation == '*'){
        return operand1 * operand2;
    }
    else if(operation == '/'){
        if(operand2 != 0){
            return operand1 / operand2;
        }else{
            cerr<<"Division by Zero::Error";
            exit(-1);
        }
    }
    else cout<<"Unexpected Error"<<endl;
    return -1;
}


