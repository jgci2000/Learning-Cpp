#ifndef CLASS1_HPP
#define CLASS1_HPP

class Class1 
{
    private: 
        int b;
    public: 
        Class1(int b)
        {
            this->b = b;
        }

        int a(int, int);
        int get_b();
        void set_b(int);
};

#endif
