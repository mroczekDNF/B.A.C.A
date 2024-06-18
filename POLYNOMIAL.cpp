// Mikolaj Mroczek
#include <iostream>
#include <cstdarg>
#include <fstream>

using namespace std;

// TRZEBA OSBLUZYC DZIELENIE I WSZYSTKIE OPERACJE Z NIM ZWIAZANE
class POLYNOMIAL{
    public:
        long int degree;
        int* arr;
        static int overloaded;

        POLYNOMIAL();
        POLYNOMIAL(long int x){
            degree = x;
            arr = new int[int(x) + 1];
        };


        POLYNOMIAL(long num, ...);

        POLYNOMIAL(const POLYNOMIAL& obj){

            if(obj.arr){
                degree = obj.degree;

                arr = new int[degree + 1];

                for(int i = 0; i <= degree; i++){
                    arr[i] = obj.arr[i];
                }    
            }
        };
        
        ~POLYNOMIAL(){
            delete[] arr;
            arr = NULL;
        };

        POLYNOMIAL& operator= (const POLYNOMIAL& obj);

        POLYNOMIAL operator+ (const POLYNOMIAL obj);

        POLYNOMIAL operator- ();
        
        POLYNOMIAL operator- (const POLYNOMIAL& obj);

        POLYNOMIAL operator* (const POLYNOMIAL& obj);

        POLYNOMIAL operator/ (const POLYNOMIAL& obj);

        POLYNOMIAL operator<<(int) const;

        POLYNOMIAL operator>>(int) const;

        POLYNOMIAL& operator+=(const POLYNOMIAL obj);

        POLYNOMIAL& operator-=(const POLYNOMIAL obj);

        POLYNOMIAL& operator*=(const POLYNOMIAL obj);

        POLYNOMIAL& operator<<=(int);

        POLYNOMIAL& operator>>=(int);

        POLYNOMIAL& operator++();

        POLYNOMIAL& operator--();

        POLYNOMIAL operator++ (int);

        POLYNOMIAL operator-- (int);

        POLYNOMIAL operator* (int);

        POLYNOMIAL operator% (const POLYNOMIAL& obj);

        POLYNOMIAL& operator/=(const POLYNOMIAL& obj);

        POLYNOMIAL& operator %=(const POLYNOMIAL& obj);

        void operator delete(void* ptr){
            overloaded--;

            ::operator delete (ptr);
        };

        void* operator new(size_t amount);

    protected:
        int nww(int a, int b);
        int findHighestDivider();
        int gcd(int a, int b);
    public:
        void reduceDegree();
        void reduceFactors();

};

POLYNOMIAL::POLYNOMIAL(){
    degree = 0;
    arr = NULL;
}

POLYNOMIAL::POLYNOMIAL(long num, ...){
        degree = num;

        arr = new int[degree + 1];
        va_list args;

        va_start(args, num);

        for(int i = 0; i <= degree; i++){
            arr[i] = va_arg(args, int);
        }          
        va_end(args);

        reduceFactors();

};

void POLYNOMIAL::reduceDegree(){
    int n = degree;
    
    while(arr[n] == 0 && n > 0){
        n--;
    }

    if(n != degree){
        int* newArr = new int[degree];

        for(int i = 0; i <= degree; i++){
            newArr[i] = arr[i];
        }
        
        delete[] arr;

        arr = newArr;
        degree = n;
    }    
};

int POLYNOMIAL::gcd(int a, int b){

    if(a < 0) a = -a;
    if(b < 0) b = -b;

    while(b != 0){
        int tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
};

int POLYNOMIAL::findHighestDivider(){
    int maxDivider = 0;
    int i;
    for(i = 0; i <= degree; i++){
        if(arr[i] != 0){
            maxDivider = arr[i];
            break;
        }
    }
    if(maxDivider < 0) maxDivider = -maxDivider;

    for(i = i; i <= degree && maxDivider > 1; i++){
        
        if(arr[i] != 0){    
            maxDivider = gcd(maxDivider , arr[i]);
        }
    }
    return maxDivider;
};

void POLYNOMIAL::reduceFactors(){
    int div = findHighestDivider();

    if(div == 0) return;

    for(int i = 0; i <= degree; i++){
        arr[i] = arr[i] / div;
    }
};

std::ostream& operator <<(std::ostream& stream, const POLYNOMIAL& w){
    
    stream << "( ";
    for(int i = 0; i <= w.degree - 1; i++){
        stream << w.arr[i] << ", ";
    }
    stream << w.arr[w.degree];

    stream << " )";
    return stream;
}

std::istream& operator >> (std::istream& stream, POLYNOMIAL& w){
    stream >> w.degree;
    delete[] w.arr;
    w.arr = new int[w.degree + 1];
    
    for (int i = 0; i <= w.degree; i++) {
        stream >> w.arr[i];
    }

    w.reduceFactors();
    return stream;
};

POLYNOMIAL& POLYNOMIAL::operator= (const POLYNOMIAL& obj){

    if(this == &obj) return *this;

    int* newArr = new int[obj.degree + 1];
    degree = obj.degree;
    for(int i = 0; i <= degree; i++){
        newArr[i] = obj.arr[i];
    }

    delete[] arr;

    arr = newArr;
    return *this;
};

POLYNOMIAL POLYNOMIAL:: operator+ (const POLYNOMIAL obj){
    POLYNOMIAL newObj;

    if(degree >= obj.degree){
        newObj.arr = new int[int(degree) + 1];
        newObj.degree = degree;
    }else{
        newObj.arr = new int[obj.degree + 1];
        newObj.degree = obj.degree;
    }

    for(int i = 0; i <= newObj.degree; i++){
        if(i <= degree && i <= obj.degree){
            newObj.arr[i] = arr[i] + obj.arr[i];
        }else if(i <= degree){
            newObj.arr[i] = arr[i];
        }else{
            newObj.arr[i] = obj.arr[i];
        }
    }
    newObj.reduceDegree();

    newObj.reduceFactors();

    return newObj;
};

POLYNOMIAL POLYNOMIAL::operator- (){
    POLYNOMIAL newObj(*this);
    for(int i = 0; i <= newObj.degree; i++){
        newObj.arr[i] = -newObj.arr[i];
    }
    return newObj;
};

POLYNOMIAL POLYNOMIAL::operator- (const POLYNOMIAL& obj){
    POLYNOMIAL newObj;
    POLYNOMIAL W(obj);

    newObj = *this + (-W);

    return newObj;
};

POLYNOMIAL POLYNOMIAL::operator* (const POLYNOMIAL& obj){

    POLYNOMIAL newObj;

    newObj.degree = degree + obj.degree;

    newObj.arr = new int[newObj.degree + 1];

    for(int i = 0; i <= newObj.degree; i++){
        newObj.arr[i] = 0;
    }

    for(int i = 0; i <= degree; i++){
        for(int j = 0; j <= obj.degree; j++){
            newObj.arr[i + j] += arr[i] * obj.arr[j];
        }
    }

    newObj.reduceDegree();

    newObj.reduceFactors();

    return newObj;
};

POLYNOMIAL POLYNOMIAL::operator<<(int x) const{

    POLYNOMIAL newObj;

    if(x > degree){
        newObj.degree = 0;
        newObj.arr = new int[1];
        newObj.arr[0] = 0;
    }else{
        newObj.degree = degree - x;
        newObj.arr = new int[newObj.degree + 1];

        for(int i = x; i <= degree; i++){
            newObj.arr[i - x] = arr[i];
        }
    } 
    return newObj;
};

POLYNOMIAL POLYNOMIAL::operator>> (int x) const{
    POLYNOMIAL newObj;

    newObj.degree = degree + x;
    newObj.arr = new int[newObj.degree + 1];

    for(int i = 0; i <= newObj.degree; i++){
        if(i < x){
            newObj.arr[i] = 0;
        }else{
            newObj.arr[i] = arr[i - x];
        }
    }
    return newObj;
};

POLYNOMIAL& POLYNOMIAL::operator+=(const POLYNOMIAL obj){
    
    *this = *this + obj;
    return *this;
};

POLYNOMIAL& POLYNOMIAL::operator-= (const POLYNOMIAL obj){
    *this = *this - obj;
    return *this;
};

POLYNOMIAL& POLYNOMIAL::operator*= (const POLYNOMIAL obj){
    *this = *this * obj;
    return *this;
};

POLYNOMIAL& POLYNOMIAL::operator <<= (int x){
    *this = *this << x;
    return *this;
};

POLYNOMIAL& POLYNOMIAL::operator >>= (int x){

    *this = *this >> x;
    return *this;
};

POLYNOMIAL& POLYNOMIAL::operator++ (){

    for(int i = 0; i <= degree; i++){
        arr[i]++;
    }
    reduceDegree();
    reduceFactors();

    return *this;
};

POLYNOMIAL& POLYNOMIAL::operator--(){
    *this = -(++(-*this));
    return *this;
};

POLYNOMIAL POLYNOMIAL::operator++(int x){
    POLYNOMIAL obj(*this);
    return ++obj;
};

POLYNOMIAL POLYNOMIAL::operator--(int x){
    POLYNOMIAL obj(*this);
    return --obj;

};

POLYNOMIAL POLYNOMIAL::operator* (int x){
    POLYNOMIAL obj(*this);

    for(int i = 0; i <= obj.degree; i++){
        obj.arr[i] *= x;
    }
    obj.reduceDegree();
    obj.reduceFactors();

    return obj;
};


int POLYNOMIAL::nww(int a, int b){

    return a*b/gcd(a,b);

};

POLYNOMIAL POLYNOMIAL::operator/ (const POLYNOMIAL& P){
    POLYNOMIAL Q;

    if(degree >= P.degree){
        POLYNOMIAL W(*this);
        Q.degree = degree - P.degree;

        Q.arr = new int[Q.degree + 1];
        long sizeQ = (long)Q.degree;

        // zerujemy tablice
        for(int i = 0; i <= sizeQ; i++){
            Q.arr[i] = 0;
        }
        if(P.degree == 0 && P.arr[P.degree] != 0){
            
            if(P.arr[P.degree] < 0) W =  W * -1;


            for(int i = 0; i<= Q.degree; i++){
                Q.arr[i] = W.arr[i];
            }
        }else{
            while(W.degree >= P.degree){
            if(W.arr[W.degree] == 0){
                Q.arr[sizeQ] = 0;
                W.degree--;
                continue;
            }
            int x = (nww(W.arr[W.degree],P.arr[P.degree]) / W.arr[W.degree]);
            if(x < 0) x = -x;
            for(int i = 0; i <= Q.degree; i++){
                Q.arr[i] *= x;
            }

            if(W.arr[W.degree] % P.arr[P.degree]){
                W = W * x;
            }
            Q.arr[sizeQ] = W.arr[W.degree] / P.arr[P.degree];
            
            W -= ((P >> sizeQ) * Q.arr[sizeQ]);
            
            W.degree--;
            sizeQ--;
        }
    }
        Q.reduceDegree();
        Q.reduceFactors();
        return Q;
    }
    Q.degree = 0;
    Q.arr = new int[1];
    Q.arr[0] = 0;

    return Q;
};

POLYNOMIAL POLYNOMIAL::operator %(const POLYNOMIAL& P){
    POLYNOMIAL R(*this);

    R -= (R/P) * P;

    return R;
}

void* POLYNOMIAL::operator new(std::size_t amount){
    overloaded++;

    return ::new char[amount];
};


POLYNOMIAL& POLYNOMIAL::operator /= (const POLYNOMIAL& obj){

    *this = *this * obj;
    return *this;
};

POLYNOMIAL& POLYNOMIAL::operator %= (const POLYNOMIAL& obj){

    *this = *this * obj;
    return *this;
};
bool operator< (const POLYNOMIAL& W , const POLYNOMIAL& P){
    if(W.degree < P.degree){
        return true;
    }else if(W.degree == P.degree){
        for(int i = W.degree; i >= 0; i--){
            if(W.arr[i] < P.arr[i]){
                return true;
            }else if(W.arr[i] > P.arr[i]){
                return false;
            }
        }
    }    
    return false;
};
bool operator <= (const POLYNOMIAL& W, const POLYNOMIAL& P){
     if(W.degree < P.degree){
        return true;
    }else if(W.degree == P.degree){
        for(int i = W.degree; i >= 0; i--){
            if(W.arr[i] < P.arr[i]){
                return true;
            }else if(W.arr[i] > P.arr[i]){
                return false;
            }
        }
    }    
    return true;
};

bool operator > (const POLYNOMIAL& W, const POLYNOMIAL& P){
    return P < W;
};
bool operator >= (const POLYNOMIAL& W, const POLYNOMIAL& P){
    return P <= W;
};

bool operator == (const POLYNOMIAL& W, const POLYNOMIAL& P){
    if(W.degree != P.degree){
        return false;
    }
    
    for(int i = 0; i <= W.degree; i++){
        if(W.arr[i] != P.arr[i]){
            return false;
        }
    }
    return true;
};

bool operator != (const POLYNOMIAL& W, const POLYNOMIAL& P){
    return !(W == P);
};

int POLYNOMIAL::overloaded = 0;

#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ofstream outFile("output.txt");
    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    outFile << POLYNOMIAL(3, 1, 1, 1, 1) / POLYNOMIAL(4, 1, 1, 1, 1, 1) << std::endl;
    outFile << POLYNOMIAL(0,1) / POLYNOMIAL(0,1) << std::endl;
    outFile << POLYNOMIAL(0,1) / POLYNOMIAL(1,1,1) << endl;
    outFile << POLYNOMIAL(0,1) / POLYNOMIAL(2, 1, 1, 1) << endl;
    outFile << POLYNOMIAL(0,1) / POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    outFile << POLYNOMIAL(3, 1, 1, 1, 1) % POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    outFile << POLYNOMIAL(0,1) % POLYNOMIAL(0,1) << endl;
    outFile << POLYNOMIAL(0,1) % POLYNOMIAL(1,1,1) << endl;
    outFile << POLYNOMIAL(0,1) % POLYNOMIAL(2, 1, 1, 1) << endl;
    outFile << POLYNOMIAL(0,1) % POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl << endl;
    
    outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) / POLYNOMIAL(3, 1, 1, 1, 1) << endl;
    outFile << POLYNOMIAL(0,1) / POLYNOMIAL(0,1) << endl;
    outFile << POLYNOMIAL(1,1,1) / POLYNOMIAL(0,1) << endl;
    outFile << POLYNOMIAL(2, 1, 1, 1) / POLYNOMIAL(0,1) << endl;
    outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) / POLYNOMIAL(0,1) << endl;
    outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) % POLYNOMIAL(3, 1, 1, 1, 1) << endl;
    outFile << POLYNOMIAL(0,1) % POLYNOMIAL(0,1) << endl;
    outFile << POLYNOMIAL(1,1,1) % POLYNOMIAL(0,1) << endl;
    outFile << POLYNOMIAL(2, 1, 1, 1) % POLYNOMIAL(0,1) << endl;
    outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) % POLYNOMIAL(0,1) << endl << endl;
    
    outFile << POLYNOMIAL(1,0,1) / POLYNOMIAL(0,1) << endl;
    outFile << POLYNOMIAL(1,0,1) / POLYNOMIAL(1,1,1) << endl;
    outFile << POLYNOMIAL(1,0,1) / POLYNOMIAL(2, 1, 1, 1) << endl;
    outFile << POLYNOMIAL(1,1,1) / POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    outFile << POLYNOMIAL(1,0,1) % POLYNOMIAL(0,1) << endl;
    outFile << POLYNOMIAL(1,0,1) % POLYNOMIAL(1,1,1) << endl;
    outFile << POLYNOMIAL(1,0,1) % POLYNOMIAL(2, 1, 1, 1) << endl;
    outFile << POLYNOMIAL(1,1,1) % POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl << endl;
    
    outFile << POLYNOMIAL(0,1) / POLYNOMIAL(1,0,1) << endl;
    outFile << POLYNOMIAL(1,1,1) / POLYNOMIAL(1,0,1)<< endl;
    outFile << POLYNOMIAL(2, 1, 1, 1)  / POLYNOMIAL(1,0,1)<< endl;
    // outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) / POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(0,1) % POLYNOMIAL(1,0,1) << endl;
    // outFile << POLYNOMIAL(1,1,1) % POLYNOMIAL(1,0,1)<< endl;
    // outFile << POLYNOMIAL(2, 1, 1, 1)  % POLYNOMIAL(1,0,1)<< endl;
    // outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) % POLYNOMIAL(1,1,1) << endl << endl;
    
    // outFile << POLYNOMIAL(1,1,1) / POLYNOMIAL(0,1) << endl;
    // outFile << POLYNOMIAL(1,1,1) / POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(1,1,1) / POLYNOMIAL(2, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(1,1,1) / POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(1,1,1) / POLYNOMIAL(4, 1, 1, 1, 2, 1) << endl;
    // outFile << POLYNOMIAL(1,1,1) % POLYNOMIAL(0,1) << endl;
    // outFile << POLYNOMIAL(1,1,1) % POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(1,1,1) % POLYNOMIAL(2, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(1,1,1) % POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(1,1,1) % POLYNOMIAL(4, 1, 1, 1, 2, 1) << endl << endl;
    
    // outFile << POLYNOMIAL(0,1) / POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(1,1,1) / POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(2, 1, 1, 1) / POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) / POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) / POLYNOMIAL(1,2,1) << endl;
    // outFile << POLYNOMIAL(0,1) % POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(1,1,1) % POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(2, 1, 1, 1) % POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) % POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) % POLYNOMIAL(1,2,1) << endl << endl;
    
    
    // outFile << POLYNOMIAL(1,3,17) / POLYNOMIAL(0,1) << endl;
    // outFile << POLYNOMIAL(1,4,15) / POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(1,6,25) / POLYNOMIAL(2, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(1,8,34) / POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(1,3,17) % POLYNOMIAL(0,1) << endl;
    // outFile << POLYNOMIAL(1,4,15) % POLYNOMIAL(1,1,1) << endl;
    // outFile << POLYNOMIAL(1,6,25) % POLYNOMIAL(2, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(1,8,34) % POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl << endl;
    
    
    // outFile << POLYNOMIAL(0,1) / POLYNOMIAL(1,3,17) << endl;
    // outFile << POLYNOMIAL(1,1,1) / POLYNOMIAL(1,4,15) << endl;
    // outFile << POLYNOMIAL(2, 1, 1, 1) / POLYNOMIAL(1,6,25) << endl;
    // outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) / POLYNOMIAL(1,8,34) << endl;
    // outFile << POLYNOMIAL(0,1) % POLYNOMIAL(1,3,17) << endl;
    // outFile << POLYNOMIAL(1,1,1) % POLYNOMIAL(1,4,15) << endl;
    // outFile << POLYNOMIAL(2, 1, 1, 1) % POLYNOMIAL(1,6,25) << endl;
    // outFile << POLYNOMIAL(4, 1, 1, 1, 1, 1) % POLYNOMIAL(1,8,34) << endl << endl;
    
    // outFile << POLYNOMIAL(1,3,17) / POLYNOMIAL(0,6) << endl;
    // outFile << POLYNOMIAL(1,4,15) / POLYNOMIAL(1,2, 7) << endl;
    // outFile << POLYNOMIAL(1,6,25) / POLYNOMIAL(2, 9, 1, 18) << endl;
    // outFile << POLYNOMIAL(1,8,34) / POLYNOMIAL(4, 4, 8, 6, 12, 24) << endl;
    // outFile << POLYNOMIAL(1,3,17) % POLYNOMIAL(0,6) << endl;
    // outFile << POLYNOMIAL(1,4,15) % POLYNOMIAL(1,2, 7) << endl;
    // outFile << POLYNOMIAL(1,6,25) % POLYNOMIAL(2, 9, 1, 18) << endl;
    // outFile << POLYNOMIAL(1,8,34) % POLYNOMIAL(4, 4, 8, 6, 12, 24) << endl << endl;
    
    // outFile << POLYNOMIAL(0,6) / POLYNOMIAL(1,3,17) << endl;
    // outFile << POLYNOMIAL(1,2, 7) / POLYNOMIAL(1,4,15) << endl;
    // outFile << POLYNOMIAL(2, 9, 1, 18) / POLYNOMIAL(1,6,25) << endl;
    // outFile << POLYNOMIAL(4, 4, 8, 6, 12, 24) / POLYNOMIAL(1,8,34) << endl;
    // outFile << POLYNOMIAL(0,6) % POLYNOMIAL(1,3,17) << endl;
    // outFile << POLYNOMIAL(1,2, 7) % POLYNOMIAL(1,4,15) << endl;
    // outFile << POLYNOMIAL(2, 9, 1, 18) % POLYNOMIAL(1,6,25) << endl;
    // outFile << POLYNOMIAL(4, 4, 8, 6, 12, 24) % POLYNOMIAL(1,8,34) << endl << endl;
    
    // outFile << POLYNOMIAL(1,17,3) / POLYNOMIAL(0,6) << endl;
    // outFile << POLYNOMIAL(1,15,4) / POLYNOMIAL(1,2, 7) << endl;
    // outFile << POLYNOMIAL(1,25,6) / POLYNOMIAL(2, 9, 1, 18) << endl;
    // outFile << POLYNOMIAL(1,34,8) / POLYNOMIAL(4, 4, 8, 6, 12, 24) << endl;
    // outFile << POLYNOMIAL(1,17,3) % POLYNOMIAL(0,6) << endl;
    // outFile << POLYNOMIAL(1,15,4) % POLYNOMIAL(1,2, 7) << endl;
    // outFile << POLYNOMIAL(1,25,6) % POLYNOMIAL(2, 9, 1, 18) << endl;
    // outFile << POLYNOMIAL(1,34,8) % POLYNOMIAL(4, 4, 8, 6, 12, 24) << endl << endl;
    
    // outFile << POLYNOMIAL(0,6) / POLYNOMIAL(1,17,3) << endl;
    // outFile << POLYNOMIAL(1,2, 7) / POLYNOMIAL(1,15,4) << endl;
    // outFile << POLYNOMIAL(2, 9, 1, 18) / POLYNOMIAL(1,25,6) << endl;
    // outFile << POLYNOMIAL(4, 4, 8, 6, 12, 24) / POLYNOMIAL(1,34,8) << endl;
    // outFile << POLYNOMIAL(0,6) % POLYNOMIAL(1,17,3) << endl;
    // outFile << POLYNOMIAL(1,2, 7) % POLYNOMIAL(1,15,4) << endl;
    // outFile << POLYNOMIAL(2, 9, 1, 18) % POLYNOMIAL(1,25,6) << endl;
    // outFile << POLYNOMIAL(4, 4, 8, 6, 12, 24) % POLYNOMIAL(1,34,8) << endl << endl;
    
    // outFile << POLYNOMIAL(3, 1, 2, 3, 4) / POLYNOMIAL(4, 1, 1, 2, 1, 4) << endl;
    // outFile << POLYNOMIAL(3, 0, 0, 0, 4) / POLYNOMIAL(4, 1, 1, 13, 1, 115) << endl;
    // outFile << POLYNOMIAL(3, 3, 0, 0, 17) / POLYNOMIAL(4, 1, 16, 1, 22, 1) << endl;
    // outFile << POLYNOMIAL(3, 2, 8, 9, 13) / POLYNOMIAL(4, 1, 2, 2, 1, 1) << endl;
    // outFile << POLYNOMIAL(3, 1, 2, 3, 4) % POLYNOMIAL(4, 1, 1, 2, 1, 4) << endl;
    // outFile << POLYNOMIAL(3, 0, 0, 0, 4) % POLYNOMIAL(4, 1, 1, 13, 1, 115) << endl;
    // outFile << POLYNOMIAL(3, 3, 0, 0, 17) % POLYNOMIAL(4, 1, 16, 1, 22, 1) << endl;
    // outFile << POLYNOMIAL(3, 2, 8, 9, 13) % POLYNOMIAL(4, 1, 2, 2, 1, 1) << endl << endl;
    
    // outFile << POLYNOMIAL(4, 1, 1, 2, 1, 4) / POLYNOMIAL(3, 1, 2, 3, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, 1, 13, 1, 115) / POLYNOMIAL(3, 0, 0, 0, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, 16, 1, 22, 1) / POLYNOMIAL(3, 3, 0, 0, 17) << endl;
    // outFile << POLYNOMIAL(4, 1, 2, 2, 1, 1) / POLYNOMIAL(3, 2, 8, 9, 13) << endl;
    // outFile << POLYNOMIAL(4, 1, 1, 2, 1, 4) % POLYNOMIAL(3, 1, 2, 3, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, 1, 13, 1, 115) % POLYNOMIAL(3, 0, 0, 0, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, 16, 1, 22, 1) % POLYNOMIAL(3, 3, 0, 0, 17) << endl;
    // outFile << POLYNOMIAL(4, 1, 2, 2, 1, 1) % POLYNOMIAL(3, 2, 8, 9, 13) << endl << endl;
    
    // outFile << POLYNOMIAL(3, 1, 32, 3, 4) / POLYNOMIAL(4, 1, 1, 22, 1, 4) << endl;
    // outFile << POLYNOMIAL(3, 10, 0, -10, 4) / POLYNOMIAL(4, 31, 1, -13, 1, 115) << endl;
    // outFile << POLYNOMIAL(3, 13, 10, 0, 17) / POLYNOMIAL(4, 1, -16, 1, 22, 1) << endl;
    // outFile << POLYNOMIAL(3, 2, 28, 39, 13) / POLYNOMIAL(4, 1, 22, -2, 1, 1) << endl;
    // outFile << POLYNOMIAL(3, 1, 32, 3, 4) % POLYNOMIAL(4, 1, 1, 22, 1, 4) << endl;
    // outFile << POLYNOMIAL(3, 10, 0, -10, 4) % POLYNOMIAL(4, 31, 1, -13, 1, 115) << endl;
    // outFile << POLYNOMIAL(3, 13, 10, 0, 17) % POLYNOMIAL(4, 1, -16, 1, 22, 1) << endl;
    // outFile << POLYNOMIAL(3, 2, 28, 39, 13) % POLYNOMIAL(4, 1, 22, -2, 1, 1) << endl << endl;
    
    // outFile << POLYNOMIAL(4, 1, 1, 22, 1, 4) / POLYNOMIAL(3, 1, 32, 3, 4) << endl;
    // outFile << POLYNOMIAL(4, 31, 1, -13, 1, 115) / POLYNOMIAL(3, 10, 0, -10, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, -16, 1, 22, 1) / POLYNOMIAL(3, 13, 10, 0, 17) << endl;
    // outFile << POLYNOMIAL(4, 1, 22, -2, 1, 1) / POLYNOMIAL(3, 2, 28, 39, 13) << endl;
    // outFile << POLYNOMIAL(4, 1, 1, 22, 1, 4) % POLYNOMIAL(3, 1, 32, 3, 4) << endl;
    // outFile << POLYNOMIAL(4, 31, 1, -13, 1, 115) % POLYNOMIAL(3, 10, 0, -10, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, -16, 1, 22, 1) % POLYNOMIAL(3, 13, 10, 0, 17) << endl;
    // outFile << POLYNOMIAL(4, 1, 22, -2, 1, 1) % POLYNOMIAL(3, 2, 28, 39, 13) << endl << endl;
    
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) / POLYNOMIAL(4, 1, 8, 22, 1, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, -2, 3, 4, 1) / POLYNOMIAL(4, 7, 1, 2, 1, -4) << endl;
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) / POLYNOMIAL(4, 1, 6, 22, 1, -99) << endl;
    // outFile << POLYNOMIAL(4, 1, 2, -3, 4, 1) / POLYNOMIAL(4, 1, -1, 19, 1, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) % POLYNOMIAL(4, 1, 8, 22, 1, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, -2, 3, 4, 1) % POLYNOMIAL(4, 7, 1, 2, 1, -4) << endl;
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) % POLYNOMIAL(4, 1, 6, 22, 1, -99) << endl;
    // outFile << POLYNOMIAL(4, 1, 2, -3, 4, 1) % POLYNOMIAL(4, 1, -1, 19, 1, 4) << endl << endl;
    
    // outFile << POLYNOMIAL(4, 1, 8, 22, 1, 4) / POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(4, 7, 1, 2, 1, -4) / POLYNOMIAL(4, 1, -2, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(4, 1, 6, 22, 1, -99) / POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(4, 1, -1, 19, 1, 4) / POLYNOMIAL(4, 1, 2, -3, 4, 1) << endl;
    // outFile << POLYNOMIAL(4, 1, 8, 22, 1, 4) % POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(4, 7, 1, 2, 1, -4) % POLYNOMIAL(4, 1, -2, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(4, 1, 6, 22, 1, -99) % POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(4, 1, -1, 19, 1, 4) % POLYNOMIAL(4, 1, 2, -3, 4, 1) << endl << endl;
    
    
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) / POLYNOMIAL(5, 1, 8, 22, 1, 4, 14) << endl;
    // outFile << POLYNOMIAL(4, 1, -2, 3, 4, 1) / POLYNOMIAL(5, 7, 1, 2, 1, -4, 8) << endl;
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) / POLYNOMIAL(5, 3, 1, 6, 22, 1, -99) << endl;
    // outFile << POLYNOMIAL(4, 1, 2, -3, 4, 1) / POLYNOMIAL(5, 1, -1, 5, 19, 1, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) % POLYNOMIAL(5, 1, 8, 22, 1, 4, 14) << endl;
    // outFile << POLYNOMIAL(4, 1, -2, 3, 4, 1) % POLYNOMIAL(5, 7, 1, 2, 1, -4, 8) << endl;
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) % POLYNOMIAL(5, 3, 1, 6, 22, 1, -99) << endl;
    // outFile << POLYNOMIAL(4, 1, 2, -3, 4, 1) % POLYNOMIAL(5, 1, -1, 5, 19, 1, 4) << endl << endl;
    
    // outFile << POLYNOMIAL(5, 1, 8, 22, 1, 4, 14) / POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(5, 7, 1, 2, 1, -4, 8) / POLYNOMIAL(4, 1, -2, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(5, 3, 1, 6, 22, 1, -99) / POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(5, 1, -1, 5, 19, 1, 4) / POLYNOMIAL(4, 1, 2, -3, 4, 1) << endl;
    // outFile << POLYNOMIAL(5, 1, 8, 22, 1, 4, 14) % POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(5, 7, 1, 2, 1, -4, 8) % POLYNOMIAL(4, 1, -2, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(5, 3, 1, 6, 22, 1, -99) % POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(5, 1, -1, 5, 19, 1, 4) % POLYNOMIAL(4, 1, 2, -3, 4, 1) << endl << endl;
    
    
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) / POLYNOMIAL(6, 1, 8, 22, 1, 13, 4, 14) << endl;
    // outFile << POLYNOMIAL(4, 1, -2, 3, 4, 1) / POLYNOMIAL(6, 7, 1, 2, 0, 1, -4, 8) << endl;
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) / POLYNOMIAL(6, 3, 1, 0, 6, 22, 1, -99) << endl;
    // outFile << POLYNOMIAL(4, 1, 2, -3, 4, 1) / POLYNOMIAL(6, 1, -1, 3, 5, 19, 1, 4) << endl;
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) % POLYNOMIAL(6, 1, 8, 22, 1, 13, 4, 14) << endl;
    // outFile << POLYNOMIAL(4, 1, -2, 3, 4, 1) % POLYNOMIAL(6, 7, 1, 2, 0, 1, -4, 8) << endl;
    // outFile << POLYNOMIAL(4, 1, 32, 3, 4, 1) % POLYNOMIAL(6, 3, 1, 0, 6, 22, 1, -99) << endl;
    // outFile << POLYNOMIAL(4, 1, 2, -3, 4, 1) % POLYNOMIAL(6, 1, -1, 3, 5, 19, 1, 4) << endl << endl;
    
    // outFile << POLYNOMIAL(6, 1, 8, 22, 1, 13, 4, 14) / POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 7, 1, 2, 0, 1, -4, 8) / POLYNOMIAL(4, 1, -2, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 3, 1, 0, 6, 22, 1, -99) / POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 1, -1, 3, 5, 19, 1, 4) / POLYNOMIAL(4, 1, 2, -3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 1, 8, 22, 1, 13, 4, 14) % POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 7, 1, 2, 0, 1, -4, 8) % POLYNOMIAL(4, 1, -2, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 3, 1, 0, 6, 22, 1, -99) % POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 1, -1, 3, 5, 19, 1, 4) % POLYNOMIAL(4, 1, 2, -3, 4, 1) << endl << endl;
    
    
    // outFile << POLYNOMIAL(3, 1, 32, 4, 1) / POLYNOMIAL(6, 1, 8, 22, 1, 13, 4, 14) << endl;
    // outFile << POLYNOMIAL(3, 1, -2, 4, 1) / POLYNOMIAL(6, 7, 1, 2, 0, 1, -4, 8) << endl;
    // outFile << POLYNOMIAL(3, 1, 3, 4, 1) / POLYNOMIAL(6, 3, 1, 0, 6, 22, 1, -99) << endl;
    // outFile << POLYNOMIAL(3, 1, -3, 4, 1) / POLYNOMIAL(6, 1, -1, 3, 5, 19, 1, 4) << endl;
    // outFile << POLYNOMIAL(3, 1, 32, 4, 1) % POLYNOMIAL(6, 1, 8, 22, 1, 13, 4, 14) << endl;
    // outFile << POLYNOMIAL(3, 1, -2, 4, 1) % POLYNOMIAL(6, 7, 1, 2, 0, 1, -4, 8) << endl;
    // outFile << POLYNOMIAL(3, 1, 3, 4, 1) % POLYNOMIAL(6, 3, 1, 0, 6, 22, 1, -99) << endl;
    // outFile << POLYNOMIAL(3, 1, -3, 4, 1) % POLYNOMIAL(6, 1, -1, 3, 5, 19, 1, 4) << endl << endl;
    
    // outFile << POLYNOMIAL(6, 1, 8, 22, 1, 13, 4, 14) / POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 7, 1, 2, 0, 1, -4, 8) / POLYNOMIAL(4, 1, -2, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 3, 1, 0, 6, 22, 1, -99) / POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 1, -1, 3, 5, 19, 1, 4) / POLYNOMIAL(4, 1, 2, -3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 1, 8, 22, 1, 13, 4, 14) % POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 7, 1, 2, 0, 1, -4, 8) % POLYNOMIAL(4, 1, -2, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 3, 1, 0, 6, 22, 1, -99) % POLYNOMIAL(4, 1, 32, 3, 4, 1) << endl;
    // outFile << POLYNOMIAL(6, 1, -1, 3, 5, 19, 1, 4) % POLYNOMIAL(4, 1, 2, -3, 4, 1) << endl << endl;
    
    
    // outFile << POLYNOMIAL(3, 1, 1, 1, 1) / POLYNOMIAL() << endl;
    // outFile << POLYNOMIAL() / POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(3, 1, 1, 1, 1) / POLYNOMIAL(0, 0) << endl;
    // outFile << POLYNOMIAL(0, 0) / POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(3, 1, 1, 1, 1) / POLYNOMIAL(0,1) << endl;
    // outFile << POLYNOMIAL(0,1) / POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL() / POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(3, 1, 1, 1, 1) / POLYNOMIAL() << endl;
    // outFile << POLYNOMIAL(3, 1, 1, 1, 1) % POLYNOMIAL() << endl;
    // outFile << POLYNOMIAL() % POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(3, 1, 1, 1, 1) % POLYNOMIAL(0, 0) << endl;
    // outFile << POLYNOMIAL(0, 0) % POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(3, 1, 1, 1, 1) % POLYNOMIAL(0,1) << endl;
    // outFile << POLYNOMIAL(0,1) % POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL() % POLYNOMIAL(4, 1, 1, 1, 1, 1) << endl;
    // outFile << POLYNOMIAL(3, 1, 1, 1, 1) % POLYNOMIAL() << endl << endl;
    
    outFile.close();
    return 0;
}









// int main(){

//     POLYNOMIAL x(2,1,1,1);

//     std::cout << x;


//     return 0;
// }