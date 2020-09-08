#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#define SIZE 100


// groups structure is used to group the character and its count together
struct groups
{
    char ch;
    int count;
};

// Encodes structure is used to store the character its fixed length binary and total count binary
struct Encodes
{
    char ch;
    std::string char_code;
    std::string deci_code;
};


// consecutive function is used to get the total consecutive character of each type in data and returned to the frequency
int consecutive_chars (std::string str, int n)
{
    char next_char;
    std::vector<char> char_arr;

    for (int j=0; j<n - 1; j++)
    {
        if (str[j + 1] != str[j])
        {
            char_arr.push_back(str[j]);
        }
        next_char = str[j + 1];
    }
    char_arr.push_back(next_char);

    return char_arr.size();
}


// frequency() function is used to group the total count of each character with them
std::vector<groups> frequency(std::string str)
{
    int count = 1, index = 0;
    int str_len = str.length();
    char current_char, prev_char = str[0];
    int type_per_char = consecutive_chars(str, str_len);
    std::vector<groups> char_freq(type_per_char);

    for(int i=1; i<str_len; i++)
    {
        current_char = str[i];

        if (current_char == prev_char)
        {
            count++;
        } else
        {
            char_freq[index].ch = prev_char;
            char_freq[index].count = count;
            index++;
            count = 1;
        }

        prev_char = current_char;
    }

    char_freq[index].ch = prev_char;
    char_freq[index].count = count;

    return char_freq;
}

// charcter_types function is used to get the total different types of character in data stream
std::map<char, int> character_types(std::string str)
{
    std::map<char, int> char_types;

    for (int i=0; i<str.length(); i++)
    {
        char_types[str[i]]++;
    }

    return char_types;
}

// fixed_bits_chars() funtion is used to get the number of bits required for each character encoding
int fixed_bits_chars(std::string str)
{
    std::map<char, int> char_types = character_types(str);
    int tot_chars = char_types.size();
    int bit_len;

    for (int j=0; j<tot_chars; j++)
    {
        if (pow(2, j) >= tot_chars)
        {
            bit_len = j;
            break;
        }
    }
    
    return bit_len;
}


// fixed_bits_decimals() funtion is used to get the number of bits required for each character encoding
int fixed_bits_decimals(std::vector<groups> char_freq)
{
    int max = char_freq[0].count;
    std::string bin;


    for (int i=1; i<char_freq.size(); i++)
    {
        if (char_freq[i].count > max)
        {
            max = char_freq[i].count;
        }
    }

    while(max > 0)
    {
        int result = max % 2;
        bin = bin + std::to_string(result);
        max = max / 2;
    }

    return bin.length();
}

// binary() function is used to generate the binaries of fixed lengths
std::string binary(int n, int bit_len)
{
    std::vector<int> a(SIZE);
    int i, required_bits;
    std::string bit, zeroes;

    if (n == 0)
    {
        bit = '0';
    }
    else 
    {
        for (i=0; n>0; i++)
        {
            a[i] = n%2;
            n = n/2;
        }
        
        for (i = i-1; i>=0; i--)
        {
            bit = bit + std::to_string(a[i]);
        }
    }

    if (bit.length() < bit_len)
    {
        required_bits = bit_len - bit.length();

        while (required_bits > 0)
        {
            zeroes = zeroes + "0";
            required_bits--;
        }   

        bit = zeroes + bit;
    }

    return bit;
}


void rle (std::string str)
{
    std::vector<groups> char_freq;
    char_freq = frequency(str);
    std::map<char, int> char_types = character_types(str);
    int bit_len_char = fixed_bits_chars(str);
    int bit_len_deci = fixed_bits_decimals(char_freq);
    std::map <char, std::string> chars_with_code;
    std::vector<Encodes> encode(char_freq.size());
    int counter = 0;

    for (auto pair: char_types)
    {
        std::string value = binary(counter, bit_len_char);
        chars_with_code[pair.first] = value;
        counter++;
    }

    for (int i=0; i<char_freq.size(); i++)
    {
        std::string value = binary(char_freq[i].count, bit_len_deci);
        encode[i].ch = char_freq[i].ch;
        encode[i].deci_code = value; 
        for (auto pair: chars_with_code)
        {
            if (encode[i].ch == pair.first)
            {
                encode[i].char_code = pair.second;
            }
        }
    }

    std::cout << "\nCharacter\tCharacter_Encode\tDeciaml_Encode" << std::endl; 

    for (int i=0; i<encode.size(); i++)
    {
        std::cout << encode[i].ch << "\t\t\t" << encode[i].char_code << "\t\t\t" << encode[i].deci_code << std::endl;
    }

    std::cout << "\nEncoded binary stream: ";

    for (int i=0; i<encode.size(); i++)
    {
        std::cout << encode[i].char_code << encode[i].deci_code;
    }

    std::cout << "\nEncoded string: "; 

    for (int i=0; i<char_freq.size(); i++)
    {
        std::cout << char_freq[i].ch << char_freq[i].count;
    } 

    std::cout << std::endl;
}


int main(int argc, char* argv[])
{
    std::string str;
    std::cout << "Enter string: ";
    std::cin >> str;
    rle(str);
    return 0;
}