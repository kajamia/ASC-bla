#include <iostream>

#include <vector.h>

namespace cla = Neo_CLA;


int main()
{
  size_t n = 10;
  cla::Vector<double> x(n), y(n);

  for (size_t i = 0; i < x.Size(); i++)
    {
      x(i) = i;
      y(i) = 10;
    }

  cla::Vector<double> z = x+y;
  
  std::cout << "x+y = " << z << std::endl;
  
  std::cout << "type of (x+3*y) is  " << typeid(x+3*y).name() << std::endl;

  std::cout << "x+3*y = " << x+3*y << std::endl;

  std::cout << "sizeof(x+3*y) = " << sizeof(x+3*y) << std::endl;
  
  x.Range(2,9) = 3;
  x.Slice(1,5) = 10;
  
  std::cout << "x = " << x << std::endl; 

  std::cout << cla::Vector({1, 2, 3, 4})  << std::endl;

  std::cout << L2Norm(cla::Vector({1, 2, 0, 4})) << std::endl;

  x.Range(2,6) = cla::Vector({1, 2, 0, 4});
  std::cout << "x = " << x << std::endl;
}


