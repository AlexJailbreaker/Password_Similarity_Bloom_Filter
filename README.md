# Password Similarity Analysis: Bloom Filter Approach

## Introduction and Background Information

**Objective**: Assess password similarity to decide on new password acceptance.

### Cryptographic Hashing Functions
Transforms data into unique, fixed-size fingerprints.

#### MD5 Hashing Function 
- Widely used hashing function, but now considered insecure due to vulnerabilities like collisions.
- Creates 128-bit hash values from input data of any length. 

#### SHA256 Hashing Function
- Cryptographically secure hashing function that produces 256-bit hash values and is widely used for data integrity and security purposes. 
- Offers significantly more resistance to attacks than MD5.

### Bloom Filters and their Importance
Bloom filters are probabilistic data structures used for set membership tests. They help determine password similarity without storing actual passwords, enhancing security.

### Jaccard Coefficients and their Importance
Jaccard coefficients measure set similarity, aiding in password comparison for security decisions. 

### Antognini's Solution
Antognini's solution combines Bloom filters and Jaccard coefficients to assess password similarity. It efficiently estimates the Jaccard coefficient without directly comparing password sets, enhancing privacy and security. 

## Design

### N-Grams Text Analysis
**Function**: `GenerateBigrams(inputString)`

- **Purpose**: To split the input into bigrams (pairs of consecutive characters). 
- **Process**: 
  1. Add a space before and after the string to include leading and trailing characters.
  2. Iterate through the padded string, extracting pairs of adjacent characters as bigrams. 
  3. Return a list of these bigrams. 
- **Example**: 
  - Input: "MUELLER"
  - Output: ["_M", "MU", "UE", "EL", "LL", "LE", "ER", "R_"]

### Bloom Filters
**Function**: `GenerateBloomFilter(bigrams)`

- **Purpose**: To create a Bloom filter from a list of bigrams.
- **Process**: 
  1. Initialize a bit array (Bloom filter) of size 1000.
  2. For each bigram, apply 15 hash functions (`Fifteen_Hash_Function`), which combine MAD5 and SHA256 hashes.
     a. Convert each hash to an index using `convertHashtoIndex`.
     b. Set the corresponding positions in the bit array to 1.
  3. Return the populated Bloom filter.
- **Example**:
  - Input: ["_M", "MU", "UE", "EL", "LL", "LE", "ER", "R_"]
  - Output: A bit array where certain positions are set to 1 based on the bigrams' hashes.

### Jaccard Coefficient Method
**Function**: `getJaccardCoff(inputString1, inputString2)`

- **Purpose**: To measure the similarity using the Jaccard coefficient.
- **Process**:
  1. Generate bigrams for each input string.
  2. Create Bloom filters for each set of bigrams.
  3. Count the number of common true values between the two Bloom filters (`Get_Num_of_Common_True_Values`).
  4. Count the total true values in each Bloom filter (`Count_Num_of_True`).
  5. Calculate the Jaccard coefficient as the ratio of common true values to the union of true values.
  6. Return the Jaccard coefficient.
- **Example**:
  - Input: "MUELLER", "MILLER"
  - Output: A similarity score between 0 and 1.

## Threshold Study for the Jaccard Coefficient Method

Determining the optimal Jaccard Coefficient Threshold for password rejection.

### Hypothesis
Identifying the appropriate Jaccard coefficient value to reject passwords.

### Base
Strings of 8 characters

### Materials/Tools Used
![image](https://github.com/user-attachments/assets/7b703bf5-3e14-4856-9a92-96a69b70c3fb)


### Procedure
Using an n-grams text analysis and Jaccard coefficient in measuring the similarity of two passwords:
- The Main.cpp program prompts the user to enter two password strings.
- Each string is split into a list of bi-grams
- On each bi-gram, hashing is performed using MD5 and SHA256 algorithms.
- 15 different hash functions are employed on each bi-gram in the list for the string input.
- The 15 hash functions H(password) are defined as:
  ```
  H(bi-gram) = MD5(bi-gram) + (i) × SHA256H (bi-gram) 
             = MD5(bi-gram) + (i) x SHA256H (bi-gram)
  ```
- The program converts H(bi-gram) into an integer index value.
- The bloom filter for the string is marked as TRUE on the index given by H(bi-gram)
- It creates a Bloom filter (atom) based on the hash function values of each string.
- Two Bloom filters are generated.
- The Jaccard Coefficient between the two Bloom filters is calculated.

### Dependent Variables
- 8-character strings from data set 1.
- Algorithm-generated passwords that need rejection.

## Algorithm Design

### Function #1: Fifteen_Hash_Function
Using cryptographic hashing functions in a practical security context.

```python
Function Fifteen_Hash_Function(inputString, function_number):
    IF function_number in range(0,14):
      SET hash_value = MD5(inputString) + function_number * SHA256(string)
      return hash_value
    else:
      return ERROR //Invalid function number
```

**Explanation**: According to the outline and documentation provided by lecturer, 15 hash functions mechanism should be as follows:
- The functions g and h be MD5 and SHA256. 
- H(b) = g(b) + j*h(b) where j is from 0 to 14 (15 hash functions)
- Function Input: MUELLER, 2
- Function Output: Cryptographic hash

### Function #2: GenerateBigrams
```python
Function GenerateBigrams(inputString):
  SET paddedString = " " + inputString + " "
  SET bigrams = []
  //Iterate through the string to form bigrams
  For i from 0 to Length(paddedString) - 2:
    // Extract a bigram from position i
    SET bigram = paddedString[i] + paddedString[i + 1]
    //Add the bigram to the list
    Append bigram to bigrams
  return bigrams
```

**Explanation**: According to the documentation provided by lecturer, bigrams are generated by adding space before and after string.
- Function input: MUELLER
- Function output: [ '_M' , 'MU' , 'UE' , 'EL' , 'LL' , 'LE' , 'ER' , 'R_']

### Function #3: convertHashtoIndex
```python
Function convertHashtoIndex(hashValue):
  Integer = TypeCast hexadecimal (hashValue) to integer
  //Integer%1000 to cap at 1000 because bloom filter size is 1000 
  return integer % 1000
```

**Explanation**: Function is needed to convert hash value in hexadecimal to integer index. The output integer is in range between 0 to 999 because mod by 1000.

### Function #4: GenerateBloomFilter
```python
Function GenerateBloomFilter(bigrams(list)):
  //Initializing bloom filter of size 1000
  SET Bitarray[1000]
  FOR each bigram IN bigrams DO
      //For each bigram run 15 hash functions
      For Loop (Set I = 0 ; i < 15 ; i++):
        hash_digest = Fifteen_Hash_Function(bigram,i)
        index = convertHashtoIndex(hash_digest)
        Bitarray[index] = 1
  END FOR 
  return Bitarray
```

**Explanation**: For each bigram in [ '_M' , 'MU' , 'UE' , 'EL' , 'LL' , 'LE' , 'ER' , 'R_'], 15 hash functions are used, hash converted to integer index. Bloomfilter [index] = 1. Return bloomFilter.

### Function #5: Get_Num_of_Common_True_Values
```python
FUNCTION Get_Num_of_Common_True_Values(atom1, atom2):
    SET counter = 0
    // Iterate through each element in the Bloom filters
    FOR i FROM 0 TO BLOOM_FILTER_SIZE - 1 DO
        // Check if both atoms have true values at index i
        IF atom1[i] AND atom2[i] THEN
            counter += 1
    END FOR
    return counter
```

**Explanation**: Simply finds the number of true values in two bloom filters.

### Function #6: Count_Num_of_True
```python
FUNCTION Count_Num_of_True(atom):
    SET counter = 0
    // Iterate through each element in the Bloom filter
    FOR i FROM 0 TO BLOOM_FILTER_SIZE - 1 DO
        // Check if atom has a true value at index i
        IF atom[i] THEN
            INCREMENT counter
    END FOR
    RETURN counter
```

### Function #7: getJaccardCoff
```python
Function getJaccardCoff(inputString1, inputString2):
  bigrams1 = GenerateBigrams(inputString1)
  bigrams2 = GenerateBigrams(inputString2)
  atom1 = GenerateBloomFilter(bigrams1)
  atom2 = GenerateBloomFilter(bigrams2)
  commons = Get_Num_of_Common_True_Values(atom1, atom2)
  atom1_true_count = Count_Num_of_True(atom1)
  atom2_true_count = Count_Num_of_True(atom2)
  jaccard = commons / (atom1_true_count + atom2_true_count - commons)
  return jaccard
```

**Explanation**: This is Jaccard Coefficient.
![image](https://github.com/user-attachments/assets/8c42d7c9-87eb-4e0d-b113-f390f384521a)

## Data Structures used in the Program

### Bloom filter
```cpp
bool * bloomfilterAtom = new bool[BLOOM_FILTER_SIZE]{0};
```
We have used a bloom filter of type Boolean because the Boolean is of 1 byte and the smallest 1 byte is the smallest memory unit C++ can provide. Using an int type Boolean array is inefficient because each integer will be 4 bytes. The bloom filter is declared in the heap section when the function is called for each string, and we initialise all 1000 values of the bloom filter array to 0 to remove junk values.

## Data Analysis

We conducted a procedure involving five cases:
1. Case 1: Increasing the length by 1 character.
2. Case 2: Increasing the length by 2 characters.
3. Case 3: Decreasing the length by 1 character.
4. Case 4: Decreasing the length by 2 characters.
5. Case 5: Keeping the length fixed but altering 1 character first.

Each case involved evaluating passwords that were deemed too similar and should be rejected. For each case:
- The program ran 5 times with different inputs.
- Jaccard coefficients were calculated and recorded.
- Mean and standard deviation were computed from these Jaccard coefficients.

The final threshold was determined as follows:
- Final threshold calculation involved finding the mean and standard deviation of the thresholds from the following set: {case 1 threshold, case 2 threshold, case 3 threshold, case 4 threshold, case 5 threshold}.
- Final threshold = Mean of thresholds - Standard deviation of thresholds.

This procedure helped establish a conclusive threshold for determining the similarity of passwords across different manipulation scenarios.

## Table of working
![image](https://github.com/user-attachments/assets/d7f52f96-b46c-458c-a90e-b9ae4be99b64)

These are the passwords as designers of the program should be rejected due to similarity. Justifying what threshold must be used when comparing the similarity of two strings.

### Purpose of using Standard Deviation
Standard deviation measures the amount of variation or dispersion of a set of values from their mean. In this context, it helps assess the spread or variability of the Jaccard Similarity scores between the original and modified strings.

### Reasons for using Standard Deviation
1. **Assessing Variability**:
   - By measuring how much the similarity scores vary, the standard deviation helps in understanding the consistency of these scores. A low standard deviation indicates that the scores are closely clustered around the mean, while a high standard deviation indicates more variability.

2. **Establishing a Robust Threshold**:
   - The standard deviation can be used to define a threshold that distinguishes significant similarity from noise or less meaningful changes. Subtracting the standard deviation from the lowest value creates a conservative threshold, filtering out less similar or noise-prone comparisons.

3. **Final Threshold calculation**
   - Achieve a More Conservative Threshold: Subtracting the standard deviation of the individual thresholds from the mean helps create a more conservative final rejection threshold.
   - The standard deviation captures the spread of the individual thresholds within each modification type. Subtracting it ensures the final threshold is stricter than most of the individual thresholds, reducing the chances of accepting overly dissimilar strings.

## Password Datasets
- Clarification: While making the dataset we found alphabetically ranked 100 passwords is not feasible 
- The number of character alphabets = 26, and 26 is not divisible by 100 therefore it is not possible to include every alphabet character evenly.
- We have chosen to include 104 passwords 104/26 = 4 
- From each character alphabetic we have selected 4 password strings of 8, 10, 12 passwords respectively, in Dataset 1, Dataset 2, Dataset 3.

## Similarity Test Between Datasets
- The basis of our similarity test is checking for the presence of 2-character sequences (3-grams) in each file.
- For each dataset, we create a Bloom filter using these 2-grams.
- We use a Bloom filter with 1,000 slots and 1 hashing function to hash the 2-grams.
- Let's explain with an example using the first 5 passwords from Dataset 1:
  - "angelica, anthony1, amorcito, angelito, brandon1"
  - For the word "angelica", we extract its 2-grams: [_a, an, ng, ge, el, li, ic, ca, a_]
  - Each word in the dataset will be made into its 2gram and marked in bit array
  - Each 2-gram is hashed and marked in the Bloom filter.
- We repeat this process for Datasets 2 and 3.
- Finally, we calculate the Jaccard similarity based on the resulting three Bloom filters.

Number of possible permutations of 2 grams = 26^1 x 26^1 = 676 

Therefore, using 2 gram for similarity test is not efficient, using 3 or 4 gram is much more accurate as the number of permutations is greater than 15,000.
However, to stick to CSCI262 project requirements 2 gram and bloom filter is size 1000 is used.

### FUNCTION place_word_in_bloom_filter
Input ("OMAR")
OUTPUT 
![image](https://github.com/user-attachments/assets/7927587f-786c-4e53-bccc-886b38aec81d)

The following index in bloom filter will be set to one. 
The following command is run for each of 2 grams to mark its presence in bloom filter. 
Modulus operator is used to cap the index at 1000.
```cpp
BIT_ARRAY[hashSHA256_to_index(SHA256)%BLOOM_FILTER_SIZE] = 1;
```

## SimilarityTest.cpp/Similarity.exe
Screenshots:
![image](https://github.com/user-attachments/assets/22a749d9-a9ec-423d-8977-1f61342aca4e)

### Similarity of two grams table
![image](https://github.com/user-attachments/assets/7455632b-539d-4125-b622-546ed6d6e3c2)

## Main.cpp /main.exe
Apply the Bloom filter techniques for preventing the use of a slightly modified version of an already used password by the user.

### Option 1:
![image](https://github.com/user-attachments/assets/2518a1e3-bd88-4fbe-ba40-8ed531fbe36f)

### Option 2:
![image](https://github.com/user-attachments/assets/365c214e-0234-4e3e-8ae1-d12576610bad)
![image](https://github.com/user-attachments/assets/4f811cc3-8d87-4534-bd34-716e48d72bec)


### Option 3:
Using an n-grams text analysis and Jaccard coefficient in measuring the similarity of two passwords

#### Accepted
![Accepted password similarity](https://github.com/user-attachments/assets/b719389e-0567-4d21-be0a-8268bd042108)

#### Rejected
![Rejected password similarity](https://github.com/user-attachments/assets/b53fd54d-fdb0-4ff0-a121-3bc86a5e00d5)

## Observations
Limitations of calculating Jaccard coefficient using Bigrams approach:
- Fails to identify identical consecutive bigrams, which cause them to get ignored 
- An example of this limitation:

The Jaccard coefficient found by our program for the following words:
- UNIVERSITY and UNIVERISTYY = 0.91515
- When added UNIVERSITY and UNIVERISTYYY, this has the same Jaccard coefficient: 

![Jaccard coefficient example](https://github.com/user-attachments/assets/552c42a1-4a8b-436f-8b0f-55eb4baee9b6)

- Hence UNIVERSITY and UNIVERISTYYYYYYYYYYYYYYYYYYYYY will have the same Jaccard coefficient of 0.915152
- This is because of the Bigram approach:
  - UNIVERISTYY bigrams are UN, NI, IV, VE, ER, RI, IS, ST, TY, YY, Y_
  - UNIVERISTYYY bigrams are UN, NI, IV, VE, ER, RI, IS, ST, TY, YY, YY, Y_
  - UNIVERISTYYYY bigrams are UN, NI, IV, VE, ER, RI, IS, ST, TY, YY, YY, YY, Y_
- YY has the same hash value, hence causing the issues.
- Therefore, this concludes the limitation. 

## References
1. SHA256 Function by SystemGlitch:
   SystemGlitch (2024). System-Glitch/SHA256. [online] GitHub. Available at: https://github.com/System-Glitch/SHA256 [Accessed 18 Jun. 2024].

2. MD5 by Frank Thiolo (thilo@unix-ag.org):
   www.zedwood.com. (n.d.). C++ md5 function :: zedwood.com. [online] Available at: http://www.zedwood.com/article/cpp-md5-function.

## Source Code Files
![image](https://github.com/user-attachments/assets/2b97538e-7b26-403e-88a2-b3fb66887cdc)
