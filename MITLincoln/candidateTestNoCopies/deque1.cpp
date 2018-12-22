#include <iostream>
#include <deque>

int main ()
{
  std::deque<int> mydeque;
  int sum (0);
  mydeque.push_front (10);
  mydeque.push_front (20);
  mydeque.push_front (30);

  while (!mydeque.empty())
  {
    sum+=mydeque.back();
    mydeque.pop_back();
  }

  std::cout << "The elements of mydeque add up to " << sum << '\n';

  return 0;
}
