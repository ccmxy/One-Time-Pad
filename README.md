# One-Time-Pad
This is a series of programs that create a one-time-pad like encryption system.    

https://en.wikipedia.org/wiki/One-time_pad

# To use

In a linux terminal, run the script

      compileall

to compile all of the programs you will need.

# Encryption

Create a key with the program keygen with

    keygen *keylength*

Example:

    % keygen 256 > mykey

Choose a listening port for the otp_enc_d, which you will run as a background daemon, for otp_enc to listen
in on with

    otp_enc_d *listening_port* &

Example:

    % otp_enc_d 57171 &

Encrypt a messege into ciphertext with

    otp_enc *plaintext_file* *key* *port* > *ciphertext_file*

using the port that is being listened on by otp_enc_d. "plaintext_file" would be a file that
you want to encrypt and "ciphertext_file" would be the name of the file you want to output the encrypted text to.
If you don't specify a file it will output to the console. You can also run this program in the background with the & symbol for if you want to run multiple processes at once.

Example:

    otp_enc myplaintext mykey 57171 > myciphertext &

# Decryption

Choose a different port for otp_dec to listen in on and run it in the background with     

    otp_dec_d *listening port* &

Example:

    otp_dec_d 57172 &

then finally decipher the text with

    otp_dec ciphertext_file key port > plaintext_file   

Example:

    otp_dec myciphertext mykey 57172 > new_plaintext

Here is an example of complete list of commands to encrypt and decrypt "plaintext." This would create a file called "new_key" which would contain a key, "ciphertext" which would contain the encrypted text, and "decrypted" which would contain the decrypted text.

    % compileall            
    % otp_enc_d 57171 &         
    % otp_dec_d 57172 &     
    % keygen 1024 > new_key         
    % otp_enc plaintext new_key 57171 > ciphertext     
    % otp_dec ciphertext new_key 57172 > dycrypted        
