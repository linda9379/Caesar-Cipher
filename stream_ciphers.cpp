#include <iostream>
#include <cmath>

//Function declarations
#ifndef MARMOSET_TESTING
int main();
#endif
unsigned int eight_bits_at_k(int number, int k, int i);
char *encode( char *plaintext, unsigned long key );
char *decode( char *cihpertext, unsigned long key );
std::size_t string_length(char *s_str);

// Function definitions
#ifndef MARMOSET_TESTING
int main() {
	 char str0[]{  "4m + 2"  };
	 char str1[]{ ">H1P#" }; // [1]
	 std::cout << "\"" << str0 << "\"" << std::endl;
	 char *ciphertext{ encode( str0, 73655) };
	 std::cout << "\"" << ciphertext << "\"" << std::endl;
	 char *plaintext{ decode( ciphertext, 51323 ) };
	 std::cout << "\"" << plaintext << "\"" << std::endl;
	 delete[] plaintext;
	 delete[] ciphertext;
	 std::cout << "\"" << str1 << "\"" << std::endl;
	 ciphertext = encode( str1, 51231 );
	 std::cout << "\"" << ciphertext << "\"" << std::endl;
	 plaintext = decode( ciphertext, 51231 );
	 std::cout << "\"" << plaintext << "\"" << std::endl;
	 delete[] plaintext;
	 delete[] ciphertext;
	 return 0;

}
#endif

//Get the length of the string
std::size_t string_length(char *s_str){
    for ( std::size_t i{0}; true; ++i ) {
        if ( s_str[i] == '\0' ) {
            return i;
        }
    }
}


char *encode( char *plaintext, unsigned long key ){
	std::size_t text_length {string_length(plaintext)};
	unsigned char s[256] {};
	int  key_in_bits[64] {};
	unsigned long number {key};
	int i{0};
	int j{0};
	int k{0};
	int temp{0};

	for (int i {0}; i < 256; ++i){
		s[i] = i;
	}

	for (std::size_t i{0}; i <64; ++i){
		key_in_bits[i] = (number % 2);
		number = number / 2;
	}

	// Scramble array
	for (int a {0}; a < 256; ++a){
		k = i%64;
		j = (j + s[i] + key_in_bits[k])%256;
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
		i = (1+i)%256;
	}

	//Make length of array a multiple of four
	std::size_t length {0};
	if (text_length %4 == 1){
		length = text_length+3;
	}else if (text_length%4 == 2){
		length = text_length+2;
	}else if (text_length %4 == 3){
		length = text_length+1;
	}else{
		length = text_length;
	}

	char newString [length] {0};
	for (std::size_t i {0}; i < text_length; ++i){
		newString[i] = plaintext[i];
	}

	unsigned int plain_text_or[length] {0};
	unsigned char r_value{0};
	std::size_t r {0};
	// Get R values and xor
	for (std::size_t b{0}; b < length; ++b){
		i=(i+1)%256;
		j = (j + static_cast<int>(s[i]))%256;
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
		r = (s[i] + s[j])%256;
		r_value = s[r];
		plain_text_or[b] = (static_cast<unsigned char>(newString[b])) ^ r_value;

	}

	std::size_t four_byte_text_length {length /4};
	unsigned int four_byte_text [four_byte_text_length] {};

	for (std::size_t k{0}; k < four_byte_text_length; ++k){
		four_byte_text [k] = (plain_text_or[4*k] << 24) + (plain_text_or[4*k + 1] <<16) + (plain_text_or[4*k + 2] <<8) + (plain_text_or[4*k + 3]);
	}

	unsigned int counter {0};
	unsigned int ascii_value {};
	std::size_t cypher_text_length = 5*four_byte_text_length + 1;

	char *p_cypher_text{new char [cypher_text_length]{}};
	std::size_t index {0};
	//ASCII armour
	for (std::size_t i {0}; i < four_byte_text_length; ++i){
	    ascii_value = four_byte_text[i];
	    counter = 0;
	    for (std::size_t k{0}; k < 5; ++k){
	    	index = ((5)*(i+1)-1)-counter;
	    	p_cypher_text[index] = static_cast<unsigned char>((ascii_value % 85)+'!' );
	    	ascii_value = ascii_value/85;
	    	counter = counter +1;
	    }
	}
	return p_cypher_text;
}

char *decode( char *cihpertext, unsigned long key ){
	std::size_t cihper_text_length {string_length(cihpertext)};
	std::size_t integers_length {cihper_text_length/5};
	unsigned int integers [integers_length]{};
	//Remove ASCII armour
	for (std::size_t i {0}; i < integers_length; ++i){
	    integers [i] = (static_cast<unsigned int>(cihpertext [5*i])-'!') * pow(85, 4)+
	    (static_cast<unsigned int>(cihpertext [5*i+1])-'!') * pow(85, 3)+
	    (static_cast<unsigned int>(cihpertext [5*i+2])-'!') * pow(85, 2)+
	    (static_cast<unsigned int>(cihpertext [5*i+3])-'!') * pow(85, 1)+
	    (static_cast<unsigned int>(cihpertext [5*i+4])-'!') * pow(85, 0);
	}
	std::size_t one_byte_length {integers_length*4};

	unsigned int one_byte [one_byte_length]{};
	for (std::size_t i {0}; i < integers_length; ++i){
		one_byte[4*i] = eight_bits_at_k(integers[i], 8, 24);
		one_byte[4*i+1] = eight_bits_at_k(integers[i], 8, 16);
		one_byte[4*i+2] = eight_bits_at_k(integers[i], 8, 8);
		one_byte[4*i+3] = eight_bits_at_k(integers[i], 8, 0);
	}

	unsigned char s[256] {};
	int  key_in_bits[64] {};
	unsigned long number {key};
	int i{0};
	int j{0};
	int k{0};
	int temp{0};

	for (int i {0}; i < 256; ++i){
		s[i] = i;
	}

	for (std::size_t i{0}; i <64; ++i){
		key_in_bits[i] = (number % 2);
		number = number / 2;
	}

	for (int a {0}; a < 256; ++a){
		k = i%64;
		j = (j + s[i] + key_in_bits[k])%256;
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
		i = (1+i)%256;
	}

	char plain_text[one_byte_length+1] {0};
	unsigned char r_value{0};
	std::size_t r {0};
	// Find R values and xor to get plain text back
	for (std::size_t b{0}; b < one_byte_length; ++b){
		i=(i+1)%256;
		j = (j + static_cast<int>(s[i]))%256;
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
		r = (s[i] + s[j])%256;
		r_value = s[r];
		plain_text[b] = static_cast<unsigned char>((one_byte[b]) ^ r_value);

	}
	std::size_t textlength = string_length(plain_text)+1;
	char * p_plain_text{new char[textlength] {0}};
	for (std::size_t b{0}; b < textlength-1; ++b){
		p_plain_text[b] = plain_text[b];

	}
	p_plain_text[textlength] = '\0';

	return p_plain_text;

}

//Calculate the binary value of each 8 bits
unsigned int eight_bits_at_k(int number, int k, int i){
    return (((1 << k) - 1) & (number >> i));
}




