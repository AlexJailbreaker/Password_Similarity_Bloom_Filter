# Password Similarity Analysis
# Bloom Filter Approach


Introduction and Background Information
Objective: Assess password similarity to decide on new password acceptance. 

Cryptographic Hashing Functions : Transforms data into unique, fixed-size fingerprints.

MD5 Hashing Function 
•	Widely used hashing function, but now considered insecure due to vulnerabilities like collisions.
•	Creates 128-bit hash values from input data of any length. 

SHA256 Hashing Function
•	Cryptographically secure hashing function that produces 256-bit hash values and is widely used for data integrity and security purposes. 
•	Offers significantly more resistance to attacks than MD5.

Bloom Filters and their Importance
Bloom filters are probabilistic data structures used for set membership tests. They help determine password similarity without storing actual passwords, enhancing security.

