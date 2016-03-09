#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include<cmath>
using namespace std;

class Utility{
    
public:
    void pad(vector<int> &vector, int positionsToPad);
    vector<int> twosComplement(vector<int> v);
    vector<int> getBinary(int decimalNumber);
    vector<char> getHexaDecimal(int decimalNumber);
    void showVector(vector<int> vectorToShow);
    void padding(vector<int> &padVector, int size, int injectedBit);
    long extract_dec(char* &x,int y);
    bool validate(char *input);
    bool checkDecimal(char *input);
    bool checkPositive(char* argv[], int index);
    int extractDecimal(char* &input);
    vector<int> createVectorFromInteger(int number, int lenght);
    vector<int> createVectorFromCharArray(char *charArray);
    void shiftleft(vector<int> &binaryNumber, int left_shift_position, bool injected_bit);
    void shiftright(vector<int> &binaryNumber, int right_shift_position, bool injected_bit);
    vector<int> reorganiseVector(vector<int> v, int size);
    int getDecimal(vector<int> binaryNumber);
    int getDecimalFromPositiveBinary(vector<int> binaryNumber);
    bool isSameSign(vector<int> binaryNumber1, vector<int> binaryNumber2);
};

class Register
{
    vector<int> finalResultVector;
    Utility utility;
    
public:
    Register();
    friend ostream& operator << (ostream &cout,Register &obj);
    vector<int> add(vector<int> binaryNumber1, vector<int> binaryNumber2);
    vector<int> subtract(vector<int> binaryNumber1, vector<int> binaryNumber2);
    vector<int> multiply(vector<int> binaryNumber1, vector<int> binaryNumber2);
    vector<int> getFinalVector();
    
};

Register::Register(){
    this->utility = Utility();
}


vector<int> Register :: add(vector<int> binaryNumber1, vector<int> binaryNumber2)
{
    int vector1Size = binaryNumber1.size();
    int vector2Size = binaryNumber2.size();
    if (vector1Size != vector2Size) {
        if (vector1Size > vector2Size) {
            int sizeDiff = vector1Size - vector2Size;
            utility.pad(binaryNumber2, sizeDiff);
        }
        else {
            int sizeDiff = vector2Size - vector1Size;
            utility.pad(binaryNumber1, sizeDiff);
        }
    }
    int carry = 0;
    vector<int> addVector;
    int size = binaryNumber2.size();
    int carryIn, carryOut;
    for(int i = size -1 ; i >= 0; i--)
    {
        addVector.insert(addVector.begin(),((binaryNumber1[i] ^ binaryNumber2[i]) ^ carry));
        carry = ((binaryNumber1[i] & binaryNumber2[i]) | (binaryNumber1[i] & carry) | (binaryNumber2[i] & carry));
        if (i == 1) {
            carryIn = carry;
        }
    }
    carryOut = carry;
    if (carryIn != carryOut) {
        addVector.insert(addVector.begin(), carryOut);
    }
    
    return addVector;
}


vector<int> Register:: subtract(vector<int> binaryNumber1, vector<int> binaryNumber2) {
    vector<int> subtractVector;
    vector<int> twoScomplementedVector;
    twoScomplementedVector = utility.twosComplement(binaryNumber2);
    subtractVector = add(binaryNumber1, twoScomplementedVector);
    return subtractVector;
}

vector<int> Register:: multiply(vector<int> binaryNumber1, vector<int> binaryNumber2) {
    vector<int> productRegister;
    for (int i = 0; i < 32; i++) {
        if (binaryNumber2[binaryNumber2.size() - 1] == 1) {
            productRegister = add(productRegister, binaryNumber1);
        }
        utility.shiftleft(binaryNumber1, 1, 0);
        utility.shiftright(binaryNumber2, 1, 0);
    }
    return productRegister;
}
void Utility:: shiftleft(vector<int> &binaryNumber, int left_shift_position, bool injected_bit)
{
    for(int i = 0; i < left_shift_position; i++)
    {
        for(int j = 0; j < binaryNumber.size()-1; j++)
        {
            binaryNumber[j] = binaryNumber[j+1];
        }
        binaryNumber[binaryNumber.size()-1] = injected_bit;
    }
}

void Utility:: shiftright(vector<int> &binaryNumber, int right_shift_position, bool injected_bit)
{
    for(int i = 0; i < right_shift_position; i++)
    {
        for(int j = binaryNumber.size()-1; j>0; j--)
        {
            binaryNumber[j]=binaryNumber[j-1];
        }
        binaryNumber[0]= injected_bit;
    }
}
void Utility:: pad(vector<int> &vector, int positionsToPad) {
    for (int i=0; i < positionsToPad; i++) {
        vector.insert(vector.begin(), 0);
    }
}

vector<int> Utility:: twosComplement(vector<int> v){
    vector<int> negationVector;
    vector<int> oneVector;
    vector<int> twosComplementVector;
    for(int i = 0; i < v.size(); i++){
        if(v[i] == 0){
            negationVector.push_back(1);
        }
        else{
            negationVector.push_back(0);
        }
    }
    for(int i = 0; i < v.size() -1; i++){
        oneVector.push_back(0);
    }
    oneVector.push_back(1);
    int carry = 0;
    int size = oneVector.size();
    for(int i = size -1 ; i >= 0; i--)
    {
        twosComplementVector.insert(twosComplementVector.begin(),((negationVector[i] ^ oneVector[i]) ^ carry));
        carry = ((negationVector[i] & oneVector[i]) | (negationVector[i] & carry) | (oneVector[i] & carry));
    }
    if(carry == 1){
        twosComplementVector.insert(twosComplementVector.begin(), carry);
    }
    
    return twosComplementVector;
}


int main (int argc, char* argv[])
{
    string number;
    vector<int> binaryVector;
    int c, direction= 3, right_shift_position = 0, left_shift_position = 0;
    extern int optind;
    bool injected_bit= false ;
    bool leftFlag = false;
    bool rightFlag= false;
    bool isValid, isDecimal, isPositive;
    
    
    vector<int> binaryNumber1, binaryNumber2;
    int sizeNumber1 = -1, sizeNumber2 = -1;
    bool numberOfBitsFlag1, numberOfBitsFlag2;
    int numLeftShift1 = -1, numLeftShift2 = -1;
    int numRightShift1 = -1, numRightShift2 = -1;
    bool isLeftBeforeRight1 = false, isLeftBeforeRight2 = false;
    bool isOtherCalled1 = false, isOtherCalled2 = false;
    int injectedBit;
    int operation = 0;
    int print = 0, hex_dec_print = 0;
    Utility utility;
    while((c = getopt(argc, argv, "i:I:s:S:r:R:l:L:v:o:dhp")) != -1)
    {
        switch(c)
        {
            case 'i':
                
                isValid = utility.validate(optarg);
                if (!isValid) {
                    cerr<<"invalid entry";
                    return -1;
                }
                isDecimal = utility.checkDecimal(optarg);
                if (isDecimal) {
                    int decimalNumber = utility.extractDecimal(optarg);
                    binaryNumber1 = utility.getBinary(decimalNumber);
                } else {
                    binaryNumber1 = utility.createVectorFromCharArray(optarg);
                }
                break;
            case 'I':
                isValid = utility.validate(optarg);
                if (!isValid) {
                    cerr<<"invalid entry";
                    return -1;
                }
                isDecimal = utility.checkDecimal(optarg);
                if (isDecimal) {
                    int decimalNumber = utility.extractDecimal(optarg);
                    binaryNumber2 = utility.getBinary(decimalNumber);
                } else {
                    binaryNumber2 = utility.createVectorFromCharArray(optarg);
                }
                break;
            case 's':
                if(argv[optind-1]== optarg && optarg[0]=='-') {
                    cerr<<"**Register Size Not Specified**"<<endl;
                    return -1;
                } else {
                    numberOfBitsFlag1= true;
                    sizeNumber1 = atoi(optarg);
                }
                break;
            case 'S':
                if(argv[optind-1][0]== '-') {
                    cerr<<"**Register Size Not Specified**"<<endl;
                    return -1;
                } else {
                    numberOfBitsFlag2= true;
                    sizeNumber2 = atoi(optarg);
                }
                break;
            case 'r':
                if (isOtherCalled1) {
                    isLeftBeforeRight1 = true;
                } else {
                    isLeftBeforeRight1 = false;
                }
                isOtherCalled1 = true;
                if(argv[optind-1][0]== '-')
                {
                    numRightShift1=0;
                    optind= optind-1;
                }
                else
                {
                    numRightShift1 = atoi(optarg);
                    
                }
                break;
            case 'l':
                if (isOtherCalled1) {
                    isLeftBeforeRight1 = false;
                } else {
                    isLeftBeforeRight1 = true;
                }
                isOtherCalled1 = true;
                if (argv[optind-1][0]== '-')
                {
                    numLeftShift1=0;
                    optind=optind-1;
                    
                }
                else
                {
                    numLeftShift1 = atoi(optarg);
                    
                }
                break;
            case 'R':
                if (isOtherCalled2) {
                    isLeftBeforeRight2 = true;
                } else {
                    isLeftBeforeRight2 = false;
                }
                isOtherCalled2 = true;
                if(argv[optind-1][0]== '-')
                {
                    numRightShift2=0;
                    optind= optind-1;
                }
                else
                {
                    numRightShift2 = atoi(optarg);
                    
                }
                break;
            case 'L':
                if (isOtherCalled2) {
                    isLeftBeforeRight2 = false;
                } else {
                    isLeftBeforeRight2 = true;
                }
                isOtherCalled2 = true;
                if (argv[optind-1][0]== '-')
                {
                    numLeftShift2=0;
                    optind=optind-1;
                }
                else{
                    numLeftShift2 = atoi(optarg);
                }
                break;
                
            case 'v':
                if (argv[optind-1][0]== '-'){
                    injectedBit = 0;
                    optind=optind-1;
                }
                else{
                    injectedBit = atoi(optarg);
                }
                break;
                
            case 'p':
                print = 1;
                break;
                
            case 'h':
                hex_dec_print = 1;
                break;
                
            case 'd':
                hex_dec_print = 2;
                break;
            case 'o' :
                if(strlen(optarg) == 1) {
                    if(optarg[0] == '+') {
                        operation = 1;
                    } else if(optarg[0] == '-') {
                        operation = 2;
                    } else if (optarg[0] == 'x') {
                        operation = 3;
                    } else {
                        cerr << optarg[0] <<"::::"<< "****Sorry !! Unsupported Operation" << endl;
                        return -1;
                    }
                }
                break;
            case '?':
                if(argv[optind-1][1] == 'i')
                {
                    string binary_number = "0";
                    break;
                }
                else
                {
                    if(argv[optind-1][1] == 'v')
                    {
                        injected_bit = 0;
                        break;
                    }
                    else
                    {
                        if(argv[optind-1][1] == 's')
                        {
                            cerr<<"**Register Size Not Specified**"<<endl;
                            exit(-1);
                            break;
                        }
                        else
                        {
                            if(argv[optind-1][1] == 'l')
                            {
                                direction=2;
                                leftFlag = true;
                                left_shift_position = 0;
                                break;
                            }
                            else
                            {
                                if(argv[optind-1][1] == 'r')
                                {
                                    direction=1;
                                    rightFlag = true;
                                    right_shift_position = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
               	cerr << "no such option:" << argv[optind-1] << endl; break;
        }
    }
    if(binaryNumber1.size() != sizeNumber1) {
        binaryNumber1 = utility.reorganiseVector(binaryNumber1, sizeNumber1);
    }
    if(binaryNumber2.size() != sizeNumber1) {
        binaryNumber2 = utility.reorganiseVector(binaryNumber2, sizeNumber1);
    }
    if (isLeftBeforeRight1) {
        utility.shiftleft(binaryNumber1, numLeftShift1,injectedBit);
        utility.shiftright(binaryNumber1, numRightShift1, injectedBit);
    } else {
        utility.shiftright(binaryNumber1, numRightShift1, injectedBit);
        utility.shiftleft(binaryNumber1, numLeftShift1, injectedBit);
    }
    
    if (isLeftBeforeRight2) {
        utility.shiftleft(binaryNumber2, numLeftShift2, injectedBit);
        utility.shiftright(binaryNumber2, numRightShift2, injectedBit);
    } else {
        utility.shiftright(binaryNumber2, numRightShift2, injectedBit);
        utility.shiftleft(binaryNumber2, numLeftShift2, injectedBit);
    }
    vector<int> resultVector;
    Register registerObject;
    
    switch (operation) {
        case 1:
            resultVector = registerObject.add(binaryNumber1, binaryNumber2);
            break;
        case 2:
            resultVector = registerObject.subtract(binaryNumber1, binaryNumber2);
            break;
        case 3:
            utility.padding(binaryNumber1, sizeNumber1 * 2, binaryNumber1[0]);
            resultVector = registerObject.multiply(binaryNumber1, binaryNumber2);
            break;
        default:
            break;
    }
    if(print == 1 && hex_dec_print== 2){
        utility.showVector(resultVector);
        int resultInDecimal = utility.getDecimal(resultVector);
        cout << "(" << resultInDecimal <<")"<< endl;
    }else if(print == 1 && hex_dec_print== 1){
        utility.showVector(resultVector);
        int resultInHexadecimal = utility.getDecimalFromPositiveBinary(resultVector) ;
        cout << "(" << std::hex <<resultInHexadecimal  <<")"<< endl;
    }else if(print ==1 && hex_dec_print == 0){
        utility.showVector(resultVector);
        cout<<endl;
    }
    return 0;
}

int Utility:: extractDecimal(char* &input)
{
    int inputLength = strlen(input);
    string str;
    for(unsigned int i=0; i < inputLength - 1;i++){
        str += input[i];
    }
    int num = atoi(&str[0]);
    return num;
}

void Utility:: padding(vector<int> &padVector, int size, int injectedBit){
    int currentSize = padVector.size();
    int digitsToPad = size - currentSize;
    for(unsigned int i = 0; i < digitsToPad; i++){
        padVector.insert(padVector.begin(),injectedBit);
    }
}
bool Utility:: checkDecimal(char *input) {
    int inputLength = strlen(input);
    return (input[inputLength -1] == 'D');
}

bool Utility:: checkPositive(char* argv[], int index) {
    return (argv[index-1][0] == '-');
}
vector<int> Utility:: getBinary(int decimalNumber) {
    bool isNegative = false;
    if (decimalNumber < 0) {
        isNegative = true;
        decimalNumber = abs(decimalNumber);
    }
    vector<int> binaryNumberVector;
    while(decimalNumber > 0){
        int binaryNumber = decimalNumber % 2;
        decimalNumber = decimalNumber / 2;
        binaryNumberVector.insert(binaryNumberVector.begin(), binaryNumber);
    }
    if (isNegative) {
        binaryNumberVector = twosComplement(binaryNumberVector);
        binaryNumberVector.insert(binaryNumberVector.begin(), 1);
    } else {
        binaryNumberVector.insert(binaryNumberVector.begin(), 0);
    }
    return binaryNumberVector;
}



int Utility:: getDecimal(vector<int> binaryNumber) {
    int msb = binaryNumber[0];
    if (msb == 1) {
        vector<int> positiveEquivalent = twosComplement(binaryNumber);
        return getDecimalFromPositiveBinary(positiveEquivalent) * -1;
    } else {
        return getDecimalFromPositiveBinary(binaryNumber);
    }
}

int Utility:: getDecimalFromPositiveBinary(vector<int> positiveBinaryNumber) {
    int decimalNumber = 0;
    int size = positiveBinaryNumber.size();
    int powerIndex = 0;
    for (int i = size - 1; i>= 0; i--) {
        decimalNumber = decimalNumber + positiveBinaryNumber[i] * pow(2, powerIndex);
        powerIndex++;
    }
    return decimalNumber;
}
vector<int> Utility:: createVectorFromInteger(int binaryNumber, int length){
    vector<int> vector;
    for (int i = 0; i < length; i++) {
        int n1 = binaryNumber % 10;
        vector.insert(vector.begin(),n1);
        binaryNumber = binaryNumber / 10;
    }
    return vector;
}

vector<int> Utility:: createVectorFromCharArray(char *charArray) {
    vector<int> vector;
    int index = 0;
    while(charArray[index] != '\0'){
        int currentNumber = charArray[index] - 48;
        vector.push_back(currentNumber);
        index++;
    }
    return vector;
}

void Utility:: showVector(vector<int> vectorToShow) {
    for (int i=0; i < vectorToShow.size() ; i++) {
        cout << vectorToShow[i];
    }
}
vector<int> Utility:: reorganiseVector(vector<int> inputVector, int requiredSize){
    vector<int> outputVector;
    int inputVectorSize = inputVector.size();
    if (inputVectorSize == requiredSize) {
        outputVector = inputVector;
        return outputVector;
    }
    if (inputVectorSize > requiredSize) {
        for (int i = inputVectorSize-1; i >= (inputVectorSize - requiredSize); i--) {
            outputVector.insert(outputVector.begin(), inputVector[i]);
        }
    } else {
        for (int i=inputVectorSize-1; i >= 0; i--) {
            outputVector.insert(outputVector.begin(), inputVector[i]);
        }
        int sizeToPad = requiredSize - inputVectorSize;
        int bitToPad = inputVector[0];
        for (int i=0; i < sizeToPad; i++) {
            outputVector.insert(outputVector.begin(), bitToPad);
        }
    }
    return outputVector;
}
bool Utility::isSameSign(vector<int> binaryNumber1, vector<int> binaryNumber2) {
    int msb1 = binaryNumber1[0];
    int msb2 = binaryNumber2[0];
    return msb1 == msb2;
}
bool Utility:: validate(char *input) {
    //todo implement validation
    return true;
}

