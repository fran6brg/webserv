/* sscanf example */
#include <stdio.h>

// int main ()
// {
//   char sentence []="Rudolph is 12 years old";
//   char str [20];
//   int i;

//   sscanf (sentence, "%s %*s %d",str, &i);
//   printf ("%s -> %d\n",str,i);
  
//   return 0;
// }

#include <iostream>
#include <string>

int main ()
{
  std::string str="We think in generalities, but we live in details.";
                                           // (quoting Alfred N. Whitehead)
  std::string str2 = str.substr (3,5);     // "think"

  std::size_t pos = str.find("live");      // position of "live" in str

  std::string str3 = str.substr (0, pos);     // get from "live" to the end

  std::cout << str2 << ' ' << str3 << '\n';

  return 0;
}