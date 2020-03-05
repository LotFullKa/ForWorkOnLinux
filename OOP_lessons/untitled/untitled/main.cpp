#include <iostream>

class Puk{
public:
    Puk(int a) : a(a) {}
    void Pukayu(){
        std::cout << "Puk" << std::endl;
    }

private:
    int a = 0;
};

class Jopa : public Puk{
public:
    Jopa(int a, int b, int c) : Puk(a), a(b), b(c) {}

    void doit(){
        std::cout << "сможешь так ?" << std::endl;
        Pukayu();
    }

private:
    int a, b;
};


struct Granny{
    int a;
    void f();
};

struct Mom : private Granny{
    int b;
    void f(int){
        std::cout << "Hi, Im Mom" << std:endl;
    }
};

struct Son : public Mom {
    int c;
    void lol(){

    }
private:
    void f(double);
};

int main() {
    std::cout << "Hello, World!" << std::endl;

    Jopa jopa(1, 2, 3);

    jopa.doit();

    std::cout << "ты : ";
    jopa.Pukayu();

    Son bitch;

    bitch.Mom::f(1); -

    return 0;
}
